#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <ctime>

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <time.h>

using namespace std;
using namespace filesystem;

void fileSysMetaData(string filePath)
{
    cout << endl << endl << "FILESYSYEM METADATA" << endl;
    // Uses string manipulation for name, extension, and parent_path.
    // Others call POSIX stat()
    path file(filePath);
    if (exists(file))
    {
        // auto ftime = last_write_time(file);
        // auto sctp = decltype(ftime)::clock::to_sys(ftime);
        // time_t cftime = chrono::system_clock::to_time_t(sctp);
        cout << "File Name: " << file.filename() << endl;
        cout << "File extension" << file.extension() << endl;
        cout << "Parent Directory:" << file.parent_path() << endl;
        cout << "Size: " << file_size(file) << " bytes" << endl;
        // cout << "Last Modified: " << asctime(localtime(&cftime)) << endl;
    }

    return;
}

void posixMetaData(const char* fileName)
{
    cout << endl << endl << "POSIX stat() METADATA:" << endl;
    struct stat info;

    if (stat(fileName, &info) == 0)
    {
        printf("Inode: %lu\n", info.st_ino);
        printf("File Size: %ld bytes\n", info.st_size);
        printf("Owner: %s\n", getpwuid(info.st_uid)->pw_name);
        printf("Group: %s\n", getgrgid(info.st_gid)->gr_name);
        printf("Permission: %o\n", info.st_mode & 0777);
        printf("Last modified: %s\n", ctime(&info.st_mtime));
    }
    else
    {
        perror("stat");
    }
}

int posixFileManipulation()
{
    cout << endl << endl << "POSIX FILE MANIPULATION:" << endl;
    int fd = open("example.txt", O_CREAT | O_WRONLY, 0644);
    // printf("fd: %d", &fd);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    write(fd, "#!/bin/bash\necho Hello from runnable example.txt\n", 49);
    close(fd);

    printf("Created example.txt successfully\n");

    if (chmod("example.txt", 0755) == -1)
    {
        perror("chmod");
        return 1;
    }

    printf("Change permissions to 755 (rwxr-xr-x)\n");

    printf("Running exmaple.txt ...\n");
    system("./example.txt");

    if (unlink("example.txt") == -1)
    {
        perror("unlink");
        return 1;
    }
    printf("Removed example.txt\n");

    return 0;
}

void writeJson()
{
    cout << endl << endl << "WRITING json-data.txt:" << endl;
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

    ofstream outfile("json-data.txt");
    outfile << jsonStr;
    outfile.close();

    return;
}

void readJson()
{
    string jsonStr;
    ifstream infile("json-data.txt");
    const int MAX = 50;
    char buffer[MAX];

    fileSysMetaData("/C++/cpp_tasks/file-mem/json-data.txt");
    posixMetaData("json-data.txt");
    // while (infile)
    // {
    //     infile.getline(buffer, MAX);
    //     cout << buffer << endl;

    //     // infile >> jsonStr;
    //     // cout << jsonStr << endl;

    // }

    // cout << infile.rdbuf() << endl;
    ostringstream buff;
    buff << infile.rdbuf();
    string fileContent = buff.str();
    cout << "File Size: " << fileContent.size() << " bytes\n" << endl;
    cout << fileContent << endl;

    infile.close();
    return;
}

int main()
{
    // writeJson();
    readJson();
    posixFileManipulation();
    return 0;
}