#include "FileSystemWatcher.h"
#include <iostream>

void OnFileCreate(const cFileSystemEvent& evt)
{
    std::cout << evt.GetFullPath() << " created" << std::endl;
}

void OnFileChange(const cFileSystemEvent& evt)
{
    std::cout << evt.GetFullPath() << " changed" << std::endl;
}

void OnFileDelete(const cFileSystemEvent& evt)
{
    std::cout << evt.GetFullPath() << " deleted" << std::endl;
}

void OnFileRename(const cFileSystemEvent& evt)
{
    std::cout << evt.GetOldFullPath() << " rename to " << evt.GetNewFullPath() << std::endl;
}

void main()
{
    cFileSystemWatcher watcher("C:\\temp");

    watcher.SetEventHandler(cFileSystemEvent::CREATED, OnFileCreate);
    watcher.SetEventHandler(cFileSystemEvent::CHANGED, OnFileChange);
    watcher.SetEventHandler(cFileSystemEvent::DELETED, OnFileDelete);
    watcher.SetEventHandler(cFileSystemEvent::RENAMED, OnFileRename);

    while (true)
    {
        watcher.Heartbeat();
    }
}