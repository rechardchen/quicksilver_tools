

/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for cachefilesystemproto.idl:
    Os, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : dce , ms_ext, app_config, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/

#include <string.h>

#include "cachefilesystemproto.h"

#define TYPE_FORMAT_STRING_SIZE   25                                
#define PROC_FORMAT_STRING_SIZE   45                                
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _cachefilesystemproto_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } cachefilesystemproto_MIDL_TYPE_FORMAT_STRING;

typedef struct _cachefilesystemproto_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } cachefilesystemproto_MIDL_PROC_FORMAT_STRING;

typedef struct _cachefilesystemproto_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } cachefilesystemproto_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const cachefilesystemproto_MIDL_TYPE_FORMAT_STRING cachefilesystemproto__MIDL_TypeFormatString;
extern const cachefilesystemproto_MIDL_PROC_FORMAT_STRING cachefilesystemproto__MIDL_ProcFormatString;
extern const cachefilesystemproto_MIDL_EXPR_FORMAT_STRING cachefilesystemproto__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: CacheFileSystemProto, ver. 1.0,
   GUID={0xb7d4b182,0x1c00,0x4d5f,{0x95,0x32,0xc9,0x5f,0xff,0xe0,0x44,0xd3}} */

handle_t CFS_IfHandle;


static const RPC_CLIENT_INTERFACE CacheFileSystemProto___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0xb7d4b182,0x1c00,0x4d5f,{0x95,0x32,0xc9,0x5f,0xff,0xe0,0x44,0xd3}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE CacheFileSystemProto_v1_0_c_ifspec = (RPC_IF_HANDLE)& CacheFileSystemProto___RpcClientInterface;

extern const MIDL_STUB_DESC CacheFileSystemProto_StubDesc;

static RPC_BINDING_HANDLE CacheFileSystemProto__MIDL_AutoBindHandle;


boolean CFS_Exists( 
    /* [in] */ int ver,
    /* [string][in] */ const unsigned char *path)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    boolean _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(!path)
        {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
        }
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&CacheFileSystemProto_StubDesc,
                          0);
        
        
        _Handle = CFS_IfHandle;
        
        
        _StubMsg.BufferLength = 8;
        NdrConformantStringBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                       (unsigned char *)path,
                                       (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[4] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        _StubMsg.Buffer = (unsigned char *)(((__int64)_StubMsg.Buffer + 3) & ~ 0x3);
        ( *(( int * )_StubMsg.Buffer)++ = ver ) ;
        
        NdrConformantStringMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                     (unsigned char *)path,
                                     (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[4] );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char *) _StubMsg.Buffer );
        
        _StubMsg.BufferStart = (unsigned char *) _RpcMessage.Buffer; 
        _StubMsg.BufferEnd   = _StubMsg.BufferStart + _RpcMessage.BufferLength;
        
        if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &cachefilesystemproto__MIDL_ProcFormatString.Format[0] );
        
        _StubMsg.Buffer = (unsigned char *)(((__int64)_StubMsg.Buffer + 0) & ~ 0);
        
        if(( ( _StubMsg.Buffer + 1 )  > _StubMsg.BufferEnd ) )
            {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
        ( _RetVal = *(( boolean * )_StubMsg.Buffer)++ ) ;
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


CacheFileInfo CFS_Open( 
    /* [in] */ int ver,
    /* [string][in] */ const unsigned char *path)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    CacheFileInfo _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    void *_p__RetVal;
    
    if(!path)
        {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
        }
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&CacheFileSystemProto_StubDesc,
                          1);
        
        
        _Handle = CFS_IfHandle;
        
        
        _StubMsg.BufferLength = 8;
        NdrConformantStringBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                       (unsigned char *)path,
                                       (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[4] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        _StubMsg.Buffer = (unsigned char *)(((__int64)_StubMsg.Buffer + 3) & ~ 0x3);
        ( *(( int * )_StubMsg.Buffer)++ = ver ) ;
        
        NdrConformantStringMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                     (unsigned char *)path,
                                     (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[4] );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char *) _StubMsg.Buffer );
        
        _StubMsg.BufferStart = (unsigned char *) _RpcMessage.Buffer; 
        _StubMsg.BufferEnd   = _StubMsg.BufferStart + _RpcMessage.BufferLength;
        
        if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &cachefilesystemproto__MIDL_ProcFormatString.Format[8] );
        
        _p__RetVal = (void *) &_RetVal;
        MIDL_memset(
               _p__RetVal,
               0,
               sizeof( CacheFileInfo  ));
        NdrSimpleStructUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                   (unsigned char * *)&_p__RetVal,
                                   (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[6],
                                   (unsigned char)0 );
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


void CFS_Close( 
    /* [in] */ CacheFileHandle handle)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&CacheFileSystemProto_StubDesc,
                          2);
        
        
        _Handle = CFS_IfHandle;
        
        
        _StubMsg.BufferLength = 8;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        _StubMsg.Buffer = (unsigned char *)(((__int64)_StubMsg.Buffer + 3) & ~ 0x3);
        ( *(( CacheFileHandle * )_StubMsg.Buffer)++ = handle ) ;
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char *) _StubMsg.Buffer );
        
        _StubMsg.BufferStart = (unsigned char *) _RpcMessage.Buffer; 
        _StubMsg.BufferEnd   = _StubMsg.BufferStart + _RpcMessage.BufferLength;
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
}


CacheFileHandle CFS_Create( 
    /* [in] */ int ver,
    /* [string][in] */ const unsigned char *path)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    CacheFileHandle _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(!path)
        {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
        }
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&CacheFileSystemProto_StubDesc,
                          3);
        
        
        _Handle = CFS_IfHandle;
        
        
        _StubMsg.BufferLength = 8;
        NdrConformantStringBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                       (unsigned char *)path,
                                       (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[4] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        _StubMsg.Buffer = (unsigned char *)(((__int64)_StubMsg.Buffer + 3) & ~ 0x3);
        ( *(( int * )_StubMsg.Buffer)++ = ver ) ;
        
        NdrConformantStringMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                     (unsigned char *)path,
                                     (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[4] );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char *) _StubMsg.Buffer );
        
        _StubMsg.BufferStart = (unsigned char *) _RpcMessage.Buffer; 
        _StubMsg.BufferEnd   = _StubMsg.BufferStart + _RpcMessage.BufferLength;
        
        if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &cachefilesystemproto__MIDL_ProcFormatString.Format[22] );
        
        _StubMsg.Buffer = (unsigned char *)(((__int64)_StubMsg.Buffer + 3) & ~ 0x3);
        
        if(( ( _StubMsg.Buffer + 4 )  > _StubMsg.BufferEnd ) )
            {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
        ( _RetVal = *(( CacheFileHandle * )_StubMsg.Buffer)++ ) ;
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


void CFS_Upload( 
    /* [in] */ CacheFileHandle handle,
    /* [in] */ int bufferSize,
    /* [size_is][in] */ const unsigned char buffer[  ])
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&CacheFileSystemProto_StubDesc,
                          4);
        
        
        _Handle = CFS_IfHandle;
        
        
        _StubMsg.BufferLength = 16;
        ( _StubMsg.MaxCount = ( unsigned long  )bufferSize ) ;
        
        NdrConformantArrayBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char *)buffer,
                                      (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[14] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        _StubMsg.Buffer = (unsigned char *)(((__int64)_StubMsg.Buffer + 3) & ~ 0x3);
        ( *(( CacheFileHandle * )_StubMsg.Buffer)++ = handle ) ;
        
        _StubMsg.Buffer = (unsigned char *)(((__int64)_StubMsg.Buffer + 3) & ~ 0x3);
        ( *(( int * )_StubMsg.Buffer)++ = bufferSize ) ;
        
        ( _StubMsg.MaxCount = ( unsigned long  )bufferSize ) ;
        
        NdrConformantArrayMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char *)buffer,
                                    (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[14] );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char *) _StubMsg.Buffer );
        
        _StubMsg.BufferStart = (unsigned char *) _RpcMessage.Buffer; 
        _StubMsg.BufferEnd   = _StubMsg.BufferStart + _RpcMessage.BufferLength;
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
}


boolean CFS_Commit( 
    /* [in] */ CacheFileHandle handle)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    boolean _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&CacheFileSystemProto_StubDesc,
                          5);
        
        
        _Handle = CFS_IfHandle;
        
        
        _StubMsg.BufferLength = 8;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        _StubMsg.Buffer = (unsigned char *)(((__int64)_StubMsg.Buffer + 3) & ~ 0x3);
        ( *(( CacheFileHandle * )_StubMsg.Buffer)++ = handle ) ;
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char *) _StubMsg.Buffer );
        
        _StubMsg.BufferStart = (unsigned char *) _RpcMessage.Buffer; 
        _StubMsg.BufferEnd   = _StubMsg.BufferStart + _RpcMessage.BufferLength;
        
        if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &cachefilesystemproto__MIDL_ProcFormatString.Format[40] );
        
        _StubMsg.Buffer = (unsigned char *)(((__int64)_StubMsg.Buffer + 0) & ~ 0);
        
        if(( ( _StubMsg.Buffer + 1 )  > _StubMsg.BufferEnd ) )
            {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
        ( _RetVal = *(( boolean * )_StubMsg.Buffer)++ ) ;
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const cachefilesystemproto_MIDL_PROC_FORMAT_STRING cachefilesystemproto__MIDL_ProcFormatString =
    {
        0,
        {
			0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/*  2 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* X64 stack size = 1 */
/*  4 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */
/*  6 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x3,		/* FC_SMALL */
/*  8 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 10 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* X64 stack size = 1 */
/* 12 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */
/* 14 */	
			0x52,		/* FC_RETURN_PARAM */
			0x1,		/* X64 stack size = 1 */
/* 16 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */
/* 18 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 20 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */
/* 22 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 24 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* X64 stack size = 1 */
/* 26 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */
/* 28 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 30 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 32 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 34 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* X64 stack size = 1 */
/* 36 */	NdrFcShort( 0xe ),	/* Type Offset=14 */
/* 38 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */
/* 40 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 42 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x3,		/* FC_SMALL */

			0x0
        }
    };

static const cachefilesystemproto_MIDL_TYPE_FORMAT_STRING cachefilesystemproto__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/*  8 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 12 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 14 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 16 */	NdrFcShort( 0x1 ),	/* 1 */
/* 18 */	0x28,		/* Corr desc:  parameter, FC_LONG */
			0x0,		/*  */
/* 20 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 22 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const unsigned short CacheFileSystemProto_FormatStringOffsetTable[] =
    {
    0,
    8,
    18,
    22,
    30,
    40
    };


static const MIDL_STUB_DESC CacheFileSystemProto_StubDesc = 
    {
    (void *)& CacheFileSystemProto___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &CFS_IfHandle,
    0,
    0,
    0,
    0,
    cachefilesystemproto__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50000, /* Ndr library version */
    0,
    0x801026e, /* MIDL Version 8.1.622 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

