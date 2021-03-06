

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for cachefilesystemproto.idl:
    Os, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, app_config, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __cachefilesystemproto_h__
#define __cachefilesystemproto_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __CacheFileSystemProto_INTERFACE_DEFINED__
#define __CacheFileSystemProto_INTERFACE_DEFINED__

/* interface CacheFileSystemProto */
/* [implicit_handle][version][uuid] */ 

typedef int CacheFileHandle;

typedef struct _CacheFileInfo
    {
    CacheFileHandle handle;
    int size;
    } 	CacheFileInfo;

boolean CFS_Exists( 
    /* [in] */ int ver,
    /* [string][in] */ const unsigned char *path);

CacheFileInfo CFS_Open( 
    /* [in] */ int ver,
    /* [string][in] */ const unsigned char *path);

void CFS_Close( 
    /* [in] */ CacheFileHandle handle);

CacheFileHandle CFS_Create( 
    /* [in] */ int ver,
    /* [string][in] */ const unsigned char *path);

void CFS_Upload( 
    /* [in] */ CacheFileHandle handle,
    /* [in] */ int bufferSize,
    /* [size_is][in] */ const unsigned char buffer[  ]);

boolean CFS_Commit( 
    /* [in] */ CacheFileHandle handle);


extern handle_t CFS_IfHandle;


extern RPC_IF_HANDLE CacheFileSystemProto_v1_0_c_ifspec;
extern RPC_IF_HANDLE CacheFileSystemProto_v1_0_s_ifspec;
#endif /* __CacheFileSystemProto_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


