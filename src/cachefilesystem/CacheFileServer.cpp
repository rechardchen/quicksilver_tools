#include "CacheFileSystemProto.h"
#include "CacheFileSystem.h"
#include <stdio.h>

boolean CFS_Exists(int ver, const unsigned char* path)
{
	return CacheFileSystem::GetInst().FileExists(ver, path);
}

CacheFileInfo CFS_Open(int ver, const unsigned char* path)
{
	CacheFileInfo info;
	CacheFile* pFile = CacheFileSystem::GetInst().OpenFile(ver, path);
	if (pFile)
	{
		info.handle = pFile->GetID();
		info.size = pFile->GetFileSize();
	}
	else
	{
		info.handle = INVALID_CACHE_FILE_HANDLE;
		info.size = 0;
	}
	return info;
}

void CFS_Close(CacheFileHandle handle)
{
	CacheFileSystem::GetInst().CloseFile(handle);
}

CacheFileHandle CFS_Create(int ver, const unsigned char* path)
{
	CacheFile* pFile = CacheFileSystem::GetInst().CreateFile(ver, path);
	return pFile->GetID();
}

//应用侧调用upload直接传数据
void CFS_Upload(/* [in] */ CacheFileHandle handle, /* [in] */ int bufferSize, /* [size_is][in] */ const unsigned char buffer[])
{
	CacheFileSystem::GetInst().UploadFile(handle, bufferSize, buffer);
}

boolean CFS_Commit(CacheFileHandle handle)
{
	return CacheFileSystem::GetInst().CommitFile(handle);
}