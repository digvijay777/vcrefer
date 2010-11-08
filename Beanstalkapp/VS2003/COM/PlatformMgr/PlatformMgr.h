

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Sat Jan 06 11:04:08 2007
 */
/* Compiler settings for .\PlatformMgr.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __PlatformMgr_h__
#define __PlatformMgr_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICloudCenter_FWD_DEFINED__
#define __ICloudCenter_FWD_DEFINED__
typedef interface ICloudCenter ICloudCenter;
#endif 	/* __ICloudCenter_FWD_DEFINED__ */


#ifndef ___ICloudCenterEvents_FWD_DEFINED__
#define ___ICloudCenterEvents_FWD_DEFINED__
typedef interface _ICloudCenterEvents _ICloudCenterEvents;
#endif 	/* ___ICloudCenterEvents_FWD_DEFINED__ */


#ifndef __CloudCenter_FWD_DEFINED__
#define __CloudCenter_FWD_DEFINED__

#ifdef __cplusplus
typedef class CloudCenter CloudCenter;
#else
typedef struct CloudCenter CloudCenter;
#endif /* __cplusplus */

#endif 	/* __CloudCenter_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ICloudCenter_INTERFACE_DEFINED__
#define __ICloudCenter_INTERFACE_DEFINED__

/* interface ICloudCenter */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICloudCenter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("58E8A06E-8BBB-4B60-B84A-5B5612D1FF80")
    ICloudCenter : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Login( 
            /* [in] */ BSTR strCookie,
            /* [retval][out] */ VARIANT_BOOL *pbSuccess) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICloudCenterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICloudCenter * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICloudCenter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICloudCenter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICloudCenter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICloudCenter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICloudCenter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICloudCenter * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Login )( 
            ICloudCenter * This,
            /* [in] */ BSTR strCookie,
            /* [retval][out] */ VARIANT_BOOL *pbSuccess);
        
        END_INTERFACE
    } ICloudCenterVtbl;

    interface ICloudCenter
    {
        CONST_VTBL struct ICloudCenterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICloudCenter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICloudCenter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICloudCenter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICloudCenter_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICloudCenter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICloudCenter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICloudCenter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICloudCenter_Login(This,strCookie,pbSuccess)	\
    (This)->lpVtbl -> Login(This,strCookie,pbSuccess)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICloudCenter_Login_Proxy( 
    ICloudCenter * This,
    /* [in] */ BSTR strCookie,
    /* [retval][out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB ICloudCenter_Login_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICloudCenter_INTERFACE_DEFINED__ */



#ifndef __PlatformMgrLib_LIBRARY_DEFINED__
#define __PlatformMgrLib_LIBRARY_DEFINED__

/* library PlatformMgrLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_PlatformMgrLib;

#ifndef ___ICloudCenterEvents_DISPINTERFACE_DEFINED__
#define ___ICloudCenterEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ICloudCenterEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ICloudCenterEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9E56D3B3-D26E-4217-9791-10928F7A34A6")
    _ICloudCenterEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ICloudCenterEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ICloudCenterEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ICloudCenterEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ICloudCenterEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ICloudCenterEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ICloudCenterEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ICloudCenterEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ICloudCenterEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ICloudCenterEventsVtbl;

    interface _ICloudCenterEvents
    {
        CONST_VTBL struct _ICloudCenterEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ICloudCenterEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ICloudCenterEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ICloudCenterEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ICloudCenterEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ICloudCenterEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ICloudCenterEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ICloudCenterEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ICloudCenterEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CloudCenter;

#ifdef __cplusplus

class DECLSPEC_UUID("AA14C6A7-7568-4E1C-81DD-390B26FE5837")
CloudCenter;
#endif
#endif /* __PlatformMgrLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


