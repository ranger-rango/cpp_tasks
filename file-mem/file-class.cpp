#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <filesystem>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <sys/mman.h>
#include <cstring>

using namespace std;
using namespace filesystem;

class File
{
    private:
        path file;
        struct stat info;
        int fd;
        char* data;
    public:
        File(const char* fileName)
        : file(fileName), fd(open(fileName, O_RDWR | O_CREAT, 0644))
        {
            if (fstat(fd, &info) == -1)
            {
                perror("fstat");
                close(fd);
                throw runtime_error("Could not fstat file descriptor");
            }

            size_t fileSize = getSize();
            if (fileSize == 0)
            {
                if (ftruncate(fd, 1) == -1) {
                    perror("ftruncate");
                    close(fd);
                    throw runtime_error("Could not resize empty file");
                }
                if (fstat(fd, &info) == -1) {
                    perror("fstat");
                    close(fd);
                    throw runtime_error("Could not fstat after truncate");
                }
                fileSize = 1;
            }
            void* mapped = mmap(nullptr, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

            if (mapped == MAP_FAILED)
            {
                perror("mmap");
                close(fd);
                throw runtime_error("Mapping Failed");
            }

            data = static_cast<char *> (mapped);
        }

        string getName()
        {
            return file.filename();
        }

        string getExt()
        {
            return file.extension();
        }

        string getParent()
        {
            return file.parent_path();
        }

        unsigned long getInode()
        {
            return info.st_ino;
        }

        size_t getSize()
        {
            return info.st_size;
        }

        string getLastModified()
        {
            return ctime(&info.st_mtime);
        }


        void writeFile(const string& newContent)
        {
            size_t newSize = newContent.size();

            if (newSize > getSize())
            {
                if (ftruncate(fd, newSize) == -1)
                {
                    perror("ftruncate");
                    throw runtime_error("Could not resize file");
                }

                munmap(data, getSize());
                void* mapped = mmap(nullptr, newSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

                if(mapped == MAP_FAILED)
                {
                    perror("mmap");
                    throw runtime_error("Remapping failed");
                }
                data = static_cast<char *> (mapped);
                info.st_size = newSize;

            }
            memcpy(data, newContent.data(), newSize);

            if (msync(data, newSize, MS_SYNC) == -1)
            {
                perror("msync");
                throw runtime_error("msync failed");
            }

        }

        string readFile()
        {
            return string(data, getSize());
        }


        ~File()
        {
            munmap(data, getSize());
            close(fd);
        }
};

int main()
{
    File file("example.txt");
    string jsonStr = R"({
    "student": {
            "id": "12345",
            "fullname": "Oscar Ngombo",
            "age_dob": {
                "age": 28,
                "dob": "01-May-1997"
            }
        }
    })";
    file.writeFile(jsonStr);
    cout << file.readFile() << endl;
    cout << file.getName() << endl;

    cout << file.getExt() << endl;
    cout << file.getInode() << endl;
    cout << file.getSize() << endl;
    return 0;
}