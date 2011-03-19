

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sat Mar 05 22:04:01 2011
 */
/* Compiler settings for .\TestCom.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "TestCom.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ISimple_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ISimple_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ISimple2_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ISimple2_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IATLControl_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IATLControl_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ITestWindow_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ITestWindow_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ISimple, ver. 0.0,
   GUID={0x5485B63C,0xB773,0x49BD,{0xB1,0x4A,0x28,0x91,0x84,0x61,0x5D,0x70}} */

#pragma code_seg(".orpc")
static const unsigned short ISimple_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ISimple_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ISimple_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ISimple_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ISimple_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _ISimpleProxyVtbl = 
{
    0,
    &IID_ISimple,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION ISimple_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _ISimpleStubVtbl =
{
    &IID_ISimple,
    &ISimple_ServerInfo,
    7,
    &ISimple_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ISimple2, ver. 0.0,
   GUID={0x85EB30BA,0x1820,0x43D9,{0x8D,0x2D,0xC7,0x64,0x55,0x21,0xC6,0x8B}} */

#pragma code_seg(".orpc")
static const unsigned short ISimple2_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ISimple2_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ISimple2_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ISimple2_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ISimple2_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _ISimple2ProxyVtbl = 
{
    0,
    &IID_ISimple2,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION ISimple2_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _ISimple2StubVtbl =
{
    &IID_ISimple2,
    &ISimple2_ServerInfo,
    7,
    &ISimple2_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IATLControl, ver. 0.0,
   GUID={0xCCE220DF,0x2F87,0x4B13,{0x95,0xBD,0x5B,0xDD,0x16,0x1E,0x51,0xBF}} */

#pragma code_seg(".orpc")
static const unsigned short IATLControl_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IATLControl_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IATLControl_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IATLControl_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IATLControl_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _IATLControlProxyVtbl = 
{
    0,
    &IID_IATLControl,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION IATLControl_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IATLControlStubVtbl =
{
    &IID_IATLControl,
    &IATLControl_ServerInfo,
    7,
    &IATLControl_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ITestWindow, ver. 0.0,
   GUID={0x29D30587,0x8E31,0x4595,{0x8D,0xF5,0xBA,0x82,0x59,0xB6,0x64,0x70}} */

#pragma code_seg(".orpc")
static const unsigned short ITestWindow_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ITestWindow_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ITestWindow_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ITestWindow_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ITestWindow_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _ITestWindowProxyVtbl = 
{
    0,
    &IID_ITestWindow,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION ITestWindow_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _ITestWindowStubVtbl =
{
    &IID_ITestWindow,
    &ITestWindow_ServerInfo,
    7,
    &ITestWindow_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x600016e, /* MIDL Version 6.0.366 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _TestCom_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ISimpleProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ITestWindowProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ISimple2ProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IATLControlProxyVtbl,
    0
};

const CInterfaceStubVtbl * _TestCom_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ISimpleStubVtbl,
    ( CInterfaceStubVtbl *) &_ITestWindowStubVtbl,
    ( CInterfaceStubVtbl *) &_ISimple2StubVtbl,
    ( CInterfaceStubVtbl *) &_IATLControlStubVtbl,
    0
};

PCInterfaceName const _TestCom_InterfaceNamesList[] = 
{
    "ISimple",
    "ITestWindow",
    "ISimple2",
    "IATLControl",
    0
};

const IID *  _TestCom_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _TestCom_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _TestCom, pIID, n)

int __stdcall _TestCom_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _TestCom, 4, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _TestCom, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _TestCom, 4, *pIndex )
    
}

const ExtendedProxyFileInfo TestCom_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _TestCom_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _TestCom_StubVtblList,
    (const PCInterfaceName * ) & _TestCom_InterfaceNamesList,
    (const IID ** ) & _TestCom_BaseIIDList,
    & _TestCom_IID_Lookup, 
    4,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

