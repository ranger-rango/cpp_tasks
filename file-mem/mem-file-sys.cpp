// memfs.cpp — a tiny in‑RAM filesystem using FUSE3 (C++17)
// Build:  g++ -std=c++17 -O2 -Wall memfs.cpp -o memfs `pkg-config fuse3 --cflags --libs`
// Deps:   sudo apt install libfuse3-dev pkg-config
// Usage:  mkdir mnt && ./memfs mnt
// Unmount: fusermount3 -u mnt

#define FUSE_USE_VERSION 35
#include <fuse3/fuse.h>

#include <cerrno>
#include <cstring>
#include <ctime>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>

using namespace std;

// -----------------------------
// Simple in-memory node model
// -----------------------------
struct Node {
    bool is_dir = false;
    mode_t mode = 0;     // permission bits + file type
    nlink_t nlink = 1;   // link count
    off_t size = 0;      // file size (for dirs: 0)
    timespec atime{};    // access time
    timespec mtime{};    // modification time
    timespec ctime{};    // status change time

    vector<uint8_t> data;                 // for files
    map<string, shared_ptr<Node>> kids;   // for dirs
    weak_ptr<Node> parent;                // for .. traversal / bookkeeping

    Node(bool dir=false, mode_t perm=0755) : is_dir(dir) {
        timespec now{}; clock_gettime(CLOCK_REALTIME, &now);
        atime = mtime = ctime = now;
        if (is_dir) {
            mode = S_IFDIR | perm;
            nlink = 2; // "." and parent reference
        } else {
            mode = S_IFREG | perm;
            nlink = 1;
        }
    }
};

// -----------------------------
// The MemFS singleton
// -----------------------------
class MemFS {
  public:
    static MemFS& I() {
        static MemFS inst; return inst;
    }

    // Resolve absolute path to node; returns nullptr if not found
    shared_ptr<Node> resolve(const string& path) {
        if (path == "/") return root;
        if (path.empty() || path[0] != '/') return nullptr;
        auto cur = root;
        size_t i = 1;
        while (i <= path.size()) {
            size_t j = path.find('/', i);
            string name = path.substr(i, (j==string::npos? path.size(): j) - i);
            if (name.empty()) break;
            auto it = cur->kids.find(name);
            if (it == cur->kids.end()) return nullptr;
            cur = it->second;
            if (j == string::npos) break;
            i = j + 1;
        }
        return cur;
    }

    // Split parent dir and leaf name; return parent or nullptr
    shared_ptr<Node> split_parent(const string& path, string& leaf) {
        if (path.empty() || path[0] != '/') return nullptr;
        if (path == "/") return nullptr;
        auto pos = path.find_last_of('/');
        leaf = path.substr(pos+1);
        string parent_path = (pos==0)? "/" : path.substr(0, pos);
        return resolve(parent_path);
    }

    int getattr(const char* path, struct stat* st) {
        lock_guard<mutex> lock(mu);
        memset(st, 0, sizeof(*st));
        auto n = resolve(path);
        if (!n) return -ENOENT;
        st->st_mode = n->mode;
        st->st_nlink = n->nlink;
        st->st_size = n->size;
        st->st_atim = n->atime;
        st->st_mtim = n->mtime;
        st->st_ctim = n->ctime;
        st->st_uid = getuid();
        st->st_gid = getgid();
        return 0;
    }

    int readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t /*off*/, struct fuse_file_info* /*fi*/, enum fuse_readdir_flags /*flags*/) {
        lock_guard<mutex> lock(mu);
        auto n = resolve(path);
        if (!n || !n->is_dir) return -ENOENT;
        filler(buf, ".", nullptr, 0, static_cast<fuse_fill_dir_flags>(0));
        filler(buf, "..", nullptr, 0, static_cast<fuse_fill_dir_flags>(0));
        for (auto& [name, child]: n->kids) {
            struct stat st{}; st.st_mode = child->mode; st.st_nlink = child->nlink; st.st_size = child->size;
            filler(buf, name.c_str(), &st, 0, static_cast<fuse_fill_dir_flags>(0));
        }
        timespec now{}; clock_gettime(CLOCK_REALTIME, &now);
        n->atime = now;
        return 0;
    }

    int open(const char* path, struct fuse_file_info* /*fi*/) {
        lock_guard<mutex> lock(mu);
        auto n = resolve(path);
        if (!n) return -ENOENT;
        if (n->is_dir) return -EISDIR;
        return 0;
    }

    int read(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* /*fi*/) {
        lock_guard<mutex> lock(mu);
        auto n = resolve(path);
        if (!n) return -ENOENT;
        if (n->is_dir) return -EISDIR;
        if (offset >= (off_t)n->data.size()) return 0;
        size_t to_read = min(size, n->data.size() - (size_t)offset);
        memcpy(buf, n->data.data() + offset, to_read);
        timespec now{}; clock_gettime(CLOCK_REALTIME, &now);
        n->atime = now;
        return (int)to_read;
    }

    int write(const char* path, const char* buf, size_t size, off_t offset, struct fuse_file_info* /*fi*/) {
        lock_guard<mutex> lock(mu);
        auto n = resolve(path);
        if (!n) return -ENOENT;
        if (n->is_dir) return -EISDIR;
        size_t need = (size_t)offset + size;
        if (need > n->data.size()) n->data.resize(need, 0);
        memcpy(n->data.data() + offset, buf, size);
        n->size = (off_t)n->data.size();
        timespec now{}; clock_gettime(CLOCK_REALTIME, &now);
        n->mtime = n->ctime = now;
        return (int)size;
    }

    int truncate(const char* path, off_t new_size, struct fuse_file_info* /*fi*/) {
        lock_guard<mutex> lock(mu);
        auto n = resolve(path);
        if (!n) return -ENOENT;
        if (n->is_dir) return -EISDIR;
        n->data.resize((size_t)new_size, 0);
        n->size = new_size;
        timespec now{}; clock_gettime(CLOCK_REALTIME, &now);
        n->mtime = n->ctime = now;
        return 0;
    }

    int create(const char* path, mode_t mode, struct fuse_file_info* /*fi*/) {
        lock_guard<mutex> lock(mu);
        string name; auto parent = split_parent(path, name);
        if (!parent) return -ENOENT;
        if (!parent->is_dir) return -ENOTDIR;
        if (parent->kids.count(name)) return -EEXIST;
        auto f = make_shared<Node>(false, mode & 0777);
        f->parent = parent;
        parent->kids[name] = f;
        parent->nlink += 0; // files don't affect parent nlink in this toy FS
        timespec now{}; clock_gettime(CLOCK_REALTIME, &now);
        parent->mtime = parent->ctime = now;
        return 0;
    }

    int mkdir(const char* path, mode_t mode) {
        lock_guard<mutex> lock(mu);
        string name; auto parent = split_parent(path, name);
        if (!parent) return -ENOENT;
        if (!parent->is_dir) return -ENOTDIR;
        if (parent->kids.count(name)) return -EEXIST;
        auto d = make_shared<Node>(true, mode & 0777);
        d->parent = parent;
        parent->kids[name] = d;
        parent->nlink++; // new subdir adds a link to parent
        timespec now{}; clock_gettime(CLOCK_REALTIME, &now);
        parent->mtime = parent->ctime = now;
        return 0;
    }

    int unlink(const char* path) {
        lock_guard<mutex> lock(mu);
        string name; auto parent = split_parent(path, name);
        if (!parent) return -ENOENT;
        auto it = parent->kids.find(name);
        if (it == parent->kids.end()) return -ENOENT;
        if (it->second->is_dir) return -EISDIR;
        parent->kids.erase(it);
        timespec now{}; clock_gettime(CLOCK_REALTIME, &now);
        parent->mtime = parent->ctime = now;
        return 0;
    }

    int rmdir(const char* path) {
        lock_guard<mutex> lock(mu);
        if (string(path) == "/") return -EBUSY;
        string name; auto parent = split_parent(path, name);
        if (!parent) return -ENOENT;
        auto it = parent->kids.find(name);
        if (it == parent->kids.end()) return -ENOENT;
        auto dir = it->second;
        if (!dir->is_dir) return -ENOTDIR;
        if (!dir->kids.empty()) return -ENOTEMPTY;
        parent->kids.erase(it);
        parent->nlink--; // removing a subdir reduces parent link count
        timespec now{}; clock_gettime(CLOCK_REALTIME, &now);
        parent->mtime = parent->ctime = now;
        return 0;
    }

    int rename(const char* from, const char* to, unsigned int /*flags*/) {
        lock_guard<mutex> lock(mu);
        string from_name; auto from_parent = split_parent(from, from_name);
        if (!from_parent) return -ENOENT;
        auto it = from_parent->kids.find(from_name);
        if (it == from_parent->kids.end()) return -ENOENT;

        string to_name; auto to_parent = split_parent(to, to_name);
        if (!to_parent) return -ENOENT;
        if (!to_parent->is_dir) return -ENOTDIR;

        // If destination exists and is non-empty dir, error
        auto jt = to_parent->kids.find(to_name);
        if (jt != to_parent->kids.end()) {
            if (jt->second->is_dir && !jt->second->kids.empty()) return -ENOTEMPTY;
            to_parent->kids.erase(jt);
        }
        auto n = it->second;
        from_parent->kids.erase(it);
        to_parent->kids[to_name] = n;
        n->parent = to_parent;
        timespec now{}; clock_gettime(CLOCK_REALTIME, &now);
        from_parent->mtime = from_parent->ctime = now;
        to_parent->mtime = to_parent->ctime = now;
        return 0;
    }

    int utimens(const char* path, const timespec ts[2], struct fuse_file_info* /*fi*/) {
        lock_guard<mutex> lock(mu);
        auto n = resolve(path);
        if (!n) return -ENOENT;
        n->atime = ts[0];
        n->mtime = ts[1];
        timespec now{}; clock_gettime(CLOCK_REALTIME, &now);
        n->ctime = now;
        return 0;
    }

  private:
    MemFS() {
        root = make_shared<Node>(true, 0755);
        root->nlink = 2;
    }

    mutex mu;
    shared_ptr<Node> root;
};

// -----------------------------
// FUSE callback glue (C wrappers)
// -----------------------------

static int memfs_getattr(const char* path, struct stat* st, struct fuse_file_info* fi) {
    (void)fi; return MemFS::I().getattr(path, st);
}

static int memfs_readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi, enum fuse_readdir_flags flags) {
    return MemFS::I().readdir(path, buf, filler, offset, fi, flags);
}

static int memfs_open(const char* path, struct fuse_file_info* fi) {
    return MemFS::I().open(path, fi);
}

static int memfs_read(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* fi) {
    return MemFS::I().read(path, buf, size, offset, fi);
}

static int memfs_write(const char* path, const char* buf, size_t size, off_t offset, struct fuse_file_info* fi) {
    return MemFS::I().write(path, buf, size, offset, fi);
}

static int memfs_truncate(const char* path, off_t size, struct fuse_file_info* fi) {
    return MemFS::I().truncate(path, size, fi);
}

static int memfs_create(const char* path, mode_t mode, struct fuse_file_info* fi) {
    return MemFS::I().create(path, mode, fi);
}

static int memfs_mkdir(const char* path, mode_t mode) {
    return MemFS::I().mkdir(path, mode);
}

static int memfs_unlink(const char* path) {
    return MemFS::I().unlink(path);
}

static int memfs_rmdir(const char* path) {
    return MemFS::I().rmdir(path);
}

static int memfs_rename(const char* from, const char* to, unsigned int flags) {
    return MemFS::I().rename(from, to, flags);
}

static int memfs_utimens(const char* path, const timespec ts[2], struct fuse_file_info* fi) {
    return MemFS::I().utimens(path, ts, fi);
}

int main(int argc, char* argv[]) {
    fuse_operations ops{};
    ops.getattr  = memfs_getattr;
    ops.readdir  = memfs_readdir;
    ops.open     = memfs_open;
    ops.read     = memfs_read;
    ops.write    = memfs_write;
    ops.truncate = memfs_truncate;
    ops.create   = memfs_create;
    ops.mkdir    = memfs_mkdir;
    ops.unlink   = memfs_unlink;
    ops.rmdir    = memfs_rmdir;
    ops.rename   = memfs_rename;
    ops.utimens  = memfs_utimens;

    // Allow running as foreground process by default; add -f to keep logs on stdout
    return fuse_main(argc, argv, &ops, nullptr);
}
