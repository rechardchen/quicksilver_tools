#pragma once
#include <windows.h>
#include <functional>
//sync profile server

class SyncRemoteProfileServer
{
public:
    SyncRemoteProfileServer(const char* name, int pkgSize);
    ~SyncRemoteProfileServer();

    void ServeForEver();
    
    void RegisterPkgHandler(std::function<void(void*)> handler) { mPkgHandler = handler; }


private:
    HANDLE mPipeInst;
    void* mBuffer;
    int mPkgSize;
    std::function<void(void*)> mPkgHandler;
};