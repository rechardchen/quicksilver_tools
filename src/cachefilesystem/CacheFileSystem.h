#pragma once
#include <cstdint>
#include <map>
#include <mutex>
#include <string>

#include "CacheFileSystemProto.h"
#include "CacheFileCommon.h"

class CacheFile;
class CacheFileSystem
{
public:
	static CacheFileSystem& GetInst();
	CacheFile* OpenFile(int ver, const unsigned char* path);
	void CloseFile(CacheFileHandle handle);
	bool FileExists(int ver, const unsigned char* path);

	CacheFile* CreateFile(int ver, const unsigned char* path);
	//void DeleteFile(int ver, const unsigned char* path);
	void UploadFile(CacheFileHandle handle, int bufferSize, const unsigned char* buffer);
	bool CommitFile(CacheFileHandle handle);

	CacheFileHandle NextCacheID() { return mNextCacheID++; }
	void SetRootID(unsigned int id) { mRoot = id; }
private:
	CacheFileSystem() {}
	CacheFileSystem(const CacheFileSystem&) = delete;
	CacheFileSystem& operator=(const CacheFileSystem&) = delete;

private:
	typedef std::shared_ptr<CacheFile> SharedCacheFile;
	SharedCacheFile GetOpenedFile(CacheFileHandle handle);

	CacheFileHandle mNextCacheID = 0;
	std::mutex mFileMapGuard;
	std::map<uint64_t, SharedCacheFile> mFileMap;

	struct TempFile
	{
		SharedCacheFile pFile = nullptr;
		int ref = 0;
	};
	std::map<CacheFileHandle, TempFile> mOpenedFiles;

	unsigned int mRoot = 0u;	//root dir id
private:
	static CacheFileSystem sInst;
};

class CacheFile
{
public:
	CacheFile(int ver, const char* path);
	~CacheFile();

	CacheFileHandle GetID() const { return mHND; }
	void SetID(CacheFileHandle hnd) { mHND = hnd; }
	int GetFileSize() const { return mSIZE; }
	const char* GetPath() const { return mPath.c_str(); }
	int GetVer() const { return mVer; }

	//Upload to mmf
	void Upload(unsigned int root, int bufferSize, const unsigned char* buffer);

private:
	CacheFileHandle mHND = INVALID_CACHE_FILE_HANDLE;
	int mSIZE = 0;
	int mVer = 0;
	std::string mPath;
	HANDLE mMMF = NULL;
};