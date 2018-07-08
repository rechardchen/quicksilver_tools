#include "CacheFileSystemProto.h"
#include <stdio.h>

int main()
{
	RPC_STATUS status;
	unsigned char* pszUuid = NULL;
	unsigned char* pszProtocolSeq = "ncacn_np";
	unsigned char* pszNetworkAddress = NULL;
	unsigned char* pszEndpoint = "\\pipe\\qs-cfs";
	unsigned char* pszOptions = NULL;
	unsigned char* pszStringBinding = NULL;
	unsigned char* pszFileName = "xxx.exe";
	unsigned long ulCode;

	status = RpcStringBindingCompose(pszUuid,
		pszProtocolSeq,
		pszNetworkAddress,
		pszEndpoint,
		pszOptions,
		&pszStringBinding);
	if (status) exit(status);
	
	status = RpcBindingFromStringBinding(pszStringBinding,
		&CFS_IfHandle);
	if (status) exit(status);

	RpcTryExcept
	{
		FileExists(pszFileName);
		//Shutdown();
	}
		RpcExcept(1)
	{
		ulCode = RpcExceptionCode();
		printf("Runtime reported exception %ld\n", ulCode);
	}
	RpcEndExcept

	status = RpcStringFree(&pszStringBinding);
	if (status) exit(status);
	status = RpcBindingFree(&CFS_IfHandle);
	if (status) exit(status);
	exit(0);
}

void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len)
{
	return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
{
	free(ptr);
}