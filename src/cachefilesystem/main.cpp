#include <Windows.h>
#include "CacheFileCommon.h"
#include "CacheFileSystemProto.h"
#include "CacheFileSystem.h"

int main()
{
	//make sure there is only one application instance under the same directory
	char szCurDir[1024] = { 0 };
	GetCurrentDirectory(1024, szCurDir);
	_strlwr_s(szCurDir);
	unsigned int curDirId = GetCurDirID(szCurDir);
	char mutexName[100] = { 0 };
	snprintf(mutexName, 100, "qs-cfs-%d", curDirId);
	HANDLE hMutex = CreateMutex(NULL, FALSE, mutexName);
	if (hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		exit(-1);
	}

	CacheFileSystem::GetInst().SetRootID(curDirId);
	
	RPC_STATUS status;
	const char* pszProtocolSeq	= "ncacn_np"; //TODO: use lrpc to accelerate
	const char* pszSecurity = NULL;
	char pszEndPoint[128] = { 0 };
	snprintf(pszEndPoint, 128, "\\pipe\\qs-cfs-%d", curDirId);
	unsigned int cMinCalls = 1;
	unsigned int fDontWait = FALSE;

	//protseq
	status = RpcServerUseProtseqEp((RPC_CSTR)pszProtocolSeq,
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,
		(RPC_CSTR)pszEndPoint,
		(RPC_CSTR)pszSecurity);
	if (status) exit(status);

	//binding
	status = RpcServerRegisterIf(CacheFileSystemProto_v1_0_s_ifspec,
		NULL, NULL);
	if (status) exit(status);

	status = RpcServerListen(cMinCalls, RPC_C_LISTEN_MAX_CALLS_DEFAULT,
		fDontWait);
	if (status) exit(status);
}

void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len)
{
	return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
{
	free(ptr);
}