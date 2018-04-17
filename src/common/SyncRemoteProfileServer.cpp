#include "SyncRemoteProfileServer.h"
#include <string>
#include <cassert>

SyncRemoteProfileServer::SyncRemoteProfileServer(const char* name, int pkgSize)
{
    std::string servername = std::string("\\\\.\\pipe\\") + name;
    mPipeInst = CreateNamedPipe(servername.c_str(),
        PIPE_ACCESS_INBOUND,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,
        0,
        pkgSize,
        0,
        NULL);
    mBuffer = malloc(pkgSize);
    mPkgSize = pkgSize;
    mPkgHandler = nullptr;
}

SyncRemoteProfileServer::~SyncRemoteProfileServer()
{
    free(mBuffer);
    CloseHandle(mPipeInst);
}

void SyncRemoteProfileServer::ServeForEver()
{
    BOOL fConnected = ConnectNamedPipe(mPipeInst, NULL) ? TRUE :
        GetLastError() == ERROR_PIPE_CONNECTED;
    if (fConnected)
    {
        while (1)
        {
            DWORD numberOfBytesRead;
            BOOL fSuccess = ReadFile(mPipeInst, mBuffer, mPkgSize, &numberOfBytesRead, NULL);
            if (!fSuccess || numberOfBytesRead == 0)
            {
                if (GetLastError() == ERROR_BROKEN_PIPE)
                {
                    //Client disconnected
                    break;
                }
                else
                {
                    //ReadFile Failed: why?
                    break;
                }
            }
            assert(numberOfBytesRead == mPkgSize);
            //handle pkg
            if (mPkgHandler != nullptr)
            {
                mPkgHandler(mBuffer);
            }
        }
    }
}

