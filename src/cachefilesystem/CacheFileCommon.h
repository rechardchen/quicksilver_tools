#pragma once
#include <cstdint>
#include <stdio.h>
#include "CacheFileSystemProto.h"

#define INVALID_CACHE_FILE_HANDLE (-1)
#define NON_COMPILE_RES_VER (0)	//version is meaningless for resource such as pictures

inline uint64_t HashVerPath(int ver, const unsigned char* path)
{
	char temp[256] = { 0 };
	snprintf(temp, 256, "%s:%d", path, ver);
	uint64_t uRet = 0x1712e27f4d2960db;
	for (const char *pc = temp; *pc; pc++)
		uRet = uRet * 67 + *pc;
	return uRet;
}

inline void ConstructMMFName(unsigned int root, CacheFileHandle handle, char name[64])
{
	snprintf(name, 64, "qscfs%d:%d", root, handle);
}

inline void StandardPath(char *path)
{
	//TODO
}

//expect lower case dir path from GetCurrentDirectory
inline unsigned int GetCurDirID(const char *directory)
{
	unsigned int dirId = 0;
	for (int i = 0; directory[i]; ++i)
	{
		dirId = dirId * 67 + directory[i];
	}
	return dirId;
}
