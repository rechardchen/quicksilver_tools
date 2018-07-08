

/* this ALWAYS GENERATED file contains the RPC server stubs */


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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


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

/* Standard interface: CacheFileSystemProto, ver. 1.0,
   GUID={0xb7d4b182,0x1c00,0x4d5f,{0x95,0x32,0xc9,0x5f,0xff,0xe0,0x44,0xd3}} */


extern const RPC_DISPATCH_TABLE CacheFileSystemProto_v1_0_DispatchTable;

static const RPC_SERVER_INTERFACE CacheFileSystemProto___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0xb7d4b182,0x1c00,0x4d5f,{0x95,0x32,0xc9,0x5f,0xff,0xe0,0x44,0xd3}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    (RPC_DISPATCH_TABLE*)&CacheFileSystemProto_v1_0_DispatchTable,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE CacheFileSystemProto_v1_0_s_ifspec = (RPC_IF_HANDLE)& CacheFileSystemProto___RpcServerInterface;

extern const MIDL_STUB_DESC CacheFileSystemProto_StubDesc;

void __RPC_STUB
CacheFileSystemProto_CFS_Exists(
    PRPC_MESSAGE _pRpcMessage )
{
    boolean _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    const unsigned char *path;
    int ver;
    RPC_STATUS _Status;
    
    ((void)(_Status));
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &CacheFileSystemProto_StubDesc);
    
    ( ver = 0 ) ;
    ( ( unsigned char * )path = 0 ) ;
    RpcTryFinally
        {
        RpcTryExcept
            {
            if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &cachefilesystemproto__MIDL_ProcFormatString.Format[0] );
            
            _StubMsg.Buffer = (unsigned char *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            
            if(( ( _StubMsg.Buffer + 4 )  > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            ( ver = *(( int * )_StubMsg.Buffer)++ ) ;
            
            NdrConformantStringUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                           (unsigned char * *)&path,
                                           (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[4],
                                           (unsigned char)0 );
            
            if(( _StubMsg.Buffer > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            }
        RpcExcept( RPC_BAD_STUB_DATA_EXCEPTION_FILTER )
            {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
        RpcEndExcept
        
        _RetVal = CFS_Exists(ver,path);
        
        _StubMsg.BufferLength = 5;
        _pRpcMessage->BufferLength = _StubMsg.BufferLength;
        
        _Status = I_RpcGetBuffer( _pRpcMessage ); 
        if ( _Status )
            RpcRaiseException( _Status );
        
        _StubMsg.Buffer = (unsigned char *) _pRpcMessage->Buffer;
        
        _StubMsg.Buffer = (unsigned char *)(((long)_StubMsg.Buffer + 0) & ~ 0);
        ( *(( boolean * )_StubMsg.Buffer)++ = _RetVal ) ;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)(_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
CacheFileSystemProto_CFS_Open(
    PRPC_MESSAGE _pRpcMessage )
{
    CacheFileInfo _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    const unsigned char *path;
    int ver;
    RPC_STATUS _Status;
    
    ((void)(_Status));
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &CacheFileSystemProto_StubDesc);
    
    ( ver = 0 ) ;
    ( ( unsigned char * )path = 0 ) ;
    MIDL_memset(
               &_RetVal,
               0,
               sizeof( CacheFileInfo  ));
    RpcTryFinally
        {
        RpcTryExcept
            {
            if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &cachefilesystemproto__MIDL_ProcFormatString.Format[8] );
            
            _StubMsg.Buffer = (unsigned char *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            
            if(( ( _StubMsg.Buffer + 4 )  > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            ( ver = *(( int * )_StubMsg.Buffer)++ ) ;
            
            NdrConformantStringUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                           (unsigned char * *)&path,
                                           (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[4],
                                           (unsigned char)0 );
            
            if(( _StubMsg.Buffer > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            }
        RpcExcept( RPC_BAD_STUB_DATA_EXCEPTION_FILTER )
            {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
        RpcEndExcept
        
        _RetVal = CFS_Open(ver,path);
        
        _StubMsg.BufferLength = 24;
        _pRpcMessage->BufferLength = _StubMsg.BufferLength;
        
        _Status = I_RpcGetBuffer( _pRpcMessage ); 
        if ( _Status )
            RpcRaiseException( _Status );
        
        _StubMsg.Buffer = (unsigned char *) _pRpcMessage->Buffer;
        
        NdrSimpleStructMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                 (unsigned char *)&_RetVal,
                                 (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[6] );
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)(_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
CacheFileSystemProto_CFS_Close(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    CacheFileHandle handle;
    RPC_STATUS _Status;
    
    ((void)(_Status));
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &CacheFileSystemProto_StubDesc);
    
    ( handle = 0 ) ;
    RpcTryFinally
        {
        RpcTryExcept
            {
            if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &cachefilesystemproto__MIDL_ProcFormatString.Format[18] );
            
            _StubMsg.Buffer = (unsigned char *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            
            if(( ( _StubMsg.Buffer + 4 )  > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            ( handle = *(( CacheFileHandle * )_StubMsg.Buffer)++ ) ;
            
            if(( _StubMsg.Buffer > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            }
        RpcExcept( RPC_BAD_STUB_DATA_EXCEPTION_FILTER )
            {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
        RpcEndExcept
        
        CFS_Close(handle);
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)(_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
CacheFileSystemProto_CFS_Create(
    PRPC_MESSAGE _pRpcMessage )
{
    CacheFileHandle _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    const unsigned char *path;
    int ver;
    RPC_STATUS _Status;
    
    ((void)(_Status));
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &CacheFileSystemProto_StubDesc);
    
    ( ver = 0 ) ;
    ( ( unsigned char * )path = 0 ) ;
    RpcTryFinally
        {
        RpcTryExcept
            {
            if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &cachefilesystemproto__MIDL_ProcFormatString.Format[22] );
            
            _StubMsg.Buffer = (unsigned char *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            
            if(( ( _StubMsg.Buffer + 4 )  > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            ( ver = *(( int * )_StubMsg.Buffer)++ ) ;
            
            NdrConformantStringUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                           (unsigned char * *)&path,
                                           (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[4],
                                           (unsigned char)0 );
            
            if(( _StubMsg.Buffer > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            }
        RpcExcept( RPC_BAD_STUB_DATA_EXCEPTION_FILTER )
            {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
        RpcEndExcept
        
        _RetVal = CFS_Create(ver,path);
        
        _StubMsg.BufferLength = 8;
        _pRpcMessage->BufferLength = _StubMsg.BufferLength;
        
        _Status = I_RpcGetBuffer( _pRpcMessage ); 
        if ( _Status )
            RpcRaiseException( _Status );
        
        _StubMsg.Buffer = (unsigned char *) _pRpcMessage->Buffer;
        
        _StubMsg.Buffer = (unsigned char *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        ( *(( CacheFileHandle * )_StubMsg.Buffer)++ = _RetVal ) ;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)(_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
CacheFileSystemProto_CFS_Upload(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    const unsigned char ( *buffer )[  ];
    int bufferSize;
    CacheFileHandle handle;
    RPC_STATUS _Status;
    
    ((void)(_Status));
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &CacheFileSystemProto_StubDesc);
    
    ( handle = 0 ) ;
    ( bufferSize = 0 ) ;
    ( buffer = 0 ) ;
    RpcTryFinally
        {
        RpcTryExcept
            {
            if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &cachefilesystemproto__MIDL_ProcFormatString.Format[30] );
            
            _StubMsg.Buffer = (unsigned char *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            
            if(( ( _StubMsg.Buffer + 4 )  > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            ( handle = *(( CacheFileHandle * )_StubMsg.Buffer)++ ) ;
            
            _StubMsg.Buffer = (unsigned char *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            
            if(( ( _StubMsg.Buffer + 4 )  > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            ( bufferSize = *(( int * )_StubMsg.Buffer)++ ) ;
            
            NdrConformantArrayUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                          (unsigned char * *)&buffer,
                                          (PFORMAT_STRING) &cachefilesystemproto__MIDL_TypeFormatString.Format[14],
                                          (unsigned char)0 );
            
            if(( _StubMsg.Buffer > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            }
        RpcExcept( RPC_BAD_STUB_DATA_EXCEPTION_FILTER )
            {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
        RpcEndExcept
        
        CFS_Upload(
              handle,
              bufferSize,
              *buffer);
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)(_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
CacheFileSystemProto_CFS_Commit(
    PRPC_MESSAGE _pRpcMessage )
{
    boolean _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    CacheFileHandle handle;
    RPC_STATUS _Status;
    
    ((void)(_Status));
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &CacheFileSystemProto_StubDesc);
    
    ( handle = 0 ) ;
    RpcTryFinally
        {
        RpcTryExcept
            {
            if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &cachefilesystemproto__MIDL_ProcFormatString.Format[40] );
            
            _StubMsg.Buffer = (unsigned char *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            
            if(( ( _StubMsg.Buffer + 4 )  > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            ( handle = *(( CacheFileHandle * )_StubMsg.Buffer)++ ) ;
            
            if(( _StubMsg.Buffer > _StubMsg.BufferEnd ) )
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            }
        RpcExcept( RPC_BAD_STUB_DATA_EXCEPTION_FILTER )
            {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
        RpcEndExcept
        
        _RetVal = CFS_Commit(handle);
        
        _StubMsg.BufferLength = 5;
        _pRpcMessage->BufferLength = _StubMsg.BufferLength;
        
        _Status = I_RpcGetBuffer( _pRpcMessage ); 
        if ( _Status )
            RpcRaiseException( _Status );
        
        _StubMsg.Buffer = (unsigned char *) _pRpcMessage->Buffer;
        
        _StubMsg.Buffer = (unsigned char *)(((long)_StubMsg.Buffer + 0) & ~ 0);
        ( *(( boolean * )_StubMsg.Buffer)++ = _RetVal ) ;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)(_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer);
    
}


#if !defined(__RPC_WIN32__)
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
			0x1,		/* x86 stack size = 1 */
/*  4 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */
/*  6 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x3,		/* FC_SMALL */
/*  8 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 10 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* x86 stack size = 1 */
/* 12 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */
/* 14 */	
			0x52,		/* FC_RETURN_PARAM */
			0x2,		/* x86 stack size = 2 */
/* 16 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */
/* 18 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 20 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */
/* 22 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 24 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* x86 stack size = 1 */
/* 26 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */
/* 28 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 30 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 32 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 34 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* x86 stack size = 1 */
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
/* 20 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
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
    (void *)& CacheFileSystemProto___RpcServerInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    0,
    0,
    0,
    0,
    0,
    cachefilesystemproto__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x10001, /* Ndr library version */
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

static const RPC_DISPATCH_FUNCTION CacheFileSystemProto_table[] =
    {
    CacheFileSystemProto_CFS_Exists,
    CacheFileSystemProto_CFS_Open,
    CacheFileSystemProto_CFS_Close,
    CacheFileSystemProto_CFS_Create,
    CacheFileSystemProto_CFS_Upload,
    CacheFileSystemProto_CFS_Commit,
    0
    };
static const RPC_DISPATCH_TABLE CacheFileSystemProto_v1_0_DispatchTable = 
    {
    6,
    (RPC_DISPATCH_FUNCTION*)CacheFileSystemProto_table
    };
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

