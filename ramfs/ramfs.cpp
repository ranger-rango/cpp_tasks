#include "ramfs.hpp"
#include <unordered_map>
#include <functional>

int main()
{
    // cmds - cd, ls, touch, rm, mkdir, rmdir, exit
    RamFS ramFS;
    string fsCmd;
    unordered_map<string, function< void(const string&) > > commands = 
    {
        {"cd", [&](const string& arg){ramFS.cd(arg.empty() ? "." : arg);} },
        {"ls", [&](const string& arg){ramFS.ls(arg.empty() ? "." : arg);} },
        {"touch", [&](const string& arg){ramFS.mknod(arg);} },
        {"rm", [&](const string& arg){ramFS.rmnod(arg);} },
        {"mkdir", [&](const string& arg){ramFS.mkdir(arg);} },
        {"rmdir", [&](const string& arg){ramFS.rmdir(arg);} },
        {"exit", [&](const string& arg){cout << "Catchya Later:)" << endl; exit(0);} }
    };

    while (true)
    {
        cout << "--(RamFS)--[" << ramFS.getCurrentDir() << "]--# ";
        getline(cin, fsCmd);
        if (fsCmd.empty()) continue;

        auto fsCmds = ramFS.split(fsCmd, ' ');
        string cmd = fsCmds[0];
        string path;
        if (fsCmds.size() == 2)
        {
            path = fsCmds[1];
        }
        auto it = commands.find(cmd);
        if (it != commands.end())
        {
            it->second(path);
        }
        else
        {
            cout << "Unknown command" << endl;
        }
    }
    
    return 0;
}