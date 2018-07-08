#include "CacheFileSystem.h"
#include <Windows.h>
#include <cassert>

using namespace std;

CacheFileSystem& CacheFileSystem::GetInst()
{
	return sInst;
}

CacheFile* CacheFileSystem::OpenFile(int ver, const unsigned char* path)
{
	SharedCacheFile pFile = nullptr;
	{
		uint64_t hash = HashVerPath(ver, path);
		lock_guard<mutex> guard(mFileMapGuard);
		auto iter = mFileMap.find(hash);
		if (iter != mFileMap.end())
		{
			pFile = iter->second;
		}
	}

	if (pFile)
	{
		CacheFileHandle handle = pFile->GetID();
		auto iter = mOpenedFiles.find(handle);
		if (iter != mOpenedFiles.end())
		{
			iter->second.ref += 1;
		}
		else
		{
			mOpenedFiles[handle] = { pFile, 1 };
		}
		return pFile.get();
	}
	return nullptr;
}

void CacheFileSystem::CloseFile(CacheFileHandle handle)
{
	auto iter = mOpenedFiles.find(handle);
	if (iter != mOpenedFiles.end())
	{
		if (--iter->second.ref <= 0)
		{
			mOpenedFiles.erase(iter);
		}
	}
}

CacheFile* CacheFileSystem::CreateFile(int ver, const unsigned char* path)
{
	SharedCacheFile pFile = make_shared<CacheFile>(ver, (const char*)path);
	pFile->SetID(NextCacheID());

	mOpenedFiles[pFile->GetID()] = { pFile, 1 };
	return pFile.get();
}

//void CacheFileSystem::DeleteFile(int ver, const unsigned char* path)
//{
//	uint64_t hash = HashVerPath(ver, path);
//	
//	lock_guard<mutex> guard(mFileMapGuard);
//	mFileMap.erase(hash);
//}

void CacheFileSystem::UploadFile(CacheFileHandle handle, int bufferSize, const unsigned char* buffer)
{
	SharedCacheFile pFile = GetOpenedFile(handle);
	if (pFile)
	{
		pFile->Upload(mRoot, bufferSize, buffer);
	}
}

bool CacheFileSystem::CommitFile(CacheFileHandle handle)
{
	SharedCacheFile pFile = GetOpenedFile(handle);
	if (pFile)
	{
		uint64_t hash = HashVerPath(pFile->GetVer(), (const unsigned char*)pFile->GetPath());

		lock_guard<mutex> guard(mFileMapGuard);
		auto iter = mFileMap.find(hash);
		if (iter == mFileMap.end())
		{
			mFileMap[hash] = pFile;
			return true;
		}
	}
	return false;
}

bool CacheFileSystem::FileExists(int ver, const unsigned char* path)
{
	uint64_t hash = HashVerPath(ver, path);

	lock_guard<mutex> guard(mFileMapGuard);
	return mFileMap.find(hash) != mFileMap.end();
}

CacheFileSystem::SharedCacheFile CacheFileSystem::GetOpenedFile(CacheFileHandle handle)
{
	SharedCacheFile pFile = nullptr;
	auto it = mOpenedFiles.find(handle);
	if (it != mOpenedFiles.end())
	{
		pFile = it->second.pFile;
	}
	return pFile;
}

CacheFileSystem CacheFileSystem::sInst;

/////////////// CacheFile //////////////////////
CacheFile::CacheFile(int ver, const char* path): mVer(ver),mPath(path)
{

}

CacheFile::~CacheFile()
{
	if (mMMF != NULL)
	{
		CloseHandle(mMMF);
	}
}

void CacheFile::Upload(unsigned int root, int bufferSize, const unsigned char* buffer)
{
	assert(mSIZE == 0 && mMMF == NULL);
	if (mMMF == NULL && mSIZE == 0)
	{
		mSIZE = bufferSize;
		
		char mmfName[64] = { 0 };
		ConstructMMFName(root, mHND, mmfName);
		mMMF = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, mSIZE, mmfName);
		assert(mMMF != NULL);
		auto pData = (unsigned char*)MapViewOfFile(mMMF, FILE_MAP_ALL_ACCESS, 0, 0, mSIZE);
		assert(pData != nullptr);
		if (pData != nullptr)
		{
			memcpy(pData, buffer, mSIZE);
			UnmapViewOfFile(pData);
		}
	}
}
