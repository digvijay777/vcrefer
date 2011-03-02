

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Jan 20 11:07:56 2011
 */
/* Compiler settings for .\WebTest.idl:
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

#ifndef __WebTest_h__
#define __WebTest_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IThreadEvent_FWD_DEFINED__
#define __IThreadEvent_FWD_DEFINED__
typedef interface IThreadEvent IThreadEvent;
#endif 	/* __IThreadEvent_FWD_DEFINED__ */


#ifndef __ICallBack_FWD_DEFINED__
#define __ICallBack_FWD_DEFINED__
typedef interface ICallBack ICallBack;
#endif 	/* __ICallBack_FWD_DEFINED__ */


#ifndef __IMiniObject_FWD_DEFINED__
#define __IMiniObject_FWD_DEFINED__
typedef interface IMiniObject IMiniObject;
#endif 	/* __IMiniObject_FWD_DEFINED__ */


#ifndef ___IThreadEventEvents_FWD_DEFINED__
#define ___IThreadEventEvents_FWD_DEFINED__
typedef interface _IThreadEventEvents _IThreadEventEvents;
#endif 	/* ___IThreadEventEvents_FWD_DEFINED__ */


#ifndef __ThreadEvent_FWD_DEFINED__
#define __ThreadEvent_FWD_DEFINED__

#ifdef __cplusplus
typedef class ThreadEvent ThreadEvent;
#else
typedef struct ThreadEvent ThreadEvent;
#endif /* __cplusplus */

#endif 	/* __ThreadEvent_FWD_DEFINED__ */


#ifndef __CallBack_FWD_DEFINED__
#define __CallBack_FWD_DEFINED__

#ifdef __cplusplus
typedef class CallBack CallBack;
#else
typedef struct CallBack CallBack;
#endif /* __cplusplus */

#endif 	/* __CallBack_FWD_DEFINED__ */


#ifndef __MiniObject_FWD_DEFINED__
#define __MiniObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class MiniObject MiniObject;
#else
typedef struct MiniObject MiniObject;
#endif /* __cplusplus */

#endif 	/* __MiniObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IThreadEvent_INTERFACE_DEFINED__
#define __IThreadEvent_INTERFACE_DEFINED__

/* interface IThreadEvent */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IThreadEvent;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C5315687-7FBE-4168-AEE3-F48C8B21954A")
    IThreadEvent : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Start( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetPos( 
            /* [in] */ ULONG nPos) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IThreadEventVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IThreadEvent * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IThreadEvent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IThreadEvent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IThreadEvent * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IThreadEvent * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IThreadEvent * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IThreadEvent * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Start )( 
            IThreadEvent * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IThreadEvent * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetPos )( 
            IThreadEvent * This,
            /* [in] */ ULONG nPos);
        
        END_INTERFACE
    } IThreadEventVtbl;

    interface IThreadEvent
    {
        CONST_VTBL struct IThreadEventVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IThreadEvent_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IThreadEvent_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IThreadEvent_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IThreadEvent_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IThreadEvent_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IThreadEvent_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IThreadEvent_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IThreadEvent_Start(This)	\
    (This)->lpVtbl -> Start(This)

#define IThreadEvent_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IThreadEvent_SetPos(This,nPos)	\
    (This)->lpVtbl -> SetPos(This,nPos)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IThreadEvent_Start_Proxy( 
    IThreadEvent * This);


void __RPC_STUB IThreadEvent_Start_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IThreadEvent_Stop_Proxy( 
    IThreadEvent * This);


void __RPC_STUB IThreadEvent_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IThreadEvent_SetPos_Proxy( 
    IThreadEvent * This,
    /* [in] */ ULONG nPos);


void __RPC_STUB IThreadEvent_SetPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IThreadEvent_INTERFACE_DEFINED__ */


#ifndef __ICallBack_INTERFACE_DEFINED__
#define __ICallBack_INTERFACE_DEFINED__

/* interface ICallBack */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICallBack;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FBDC0BC5-2B90-4A27-A520-0C3E0654BDF8")
    ICallBack : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CallBack( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CallBack( 
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DoWork( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCallBack( 
            /* [in] */ VARIANT var) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICallBackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICallBack * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICallBack * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICallBack * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICallBack * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICallBack * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICallBack * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICallBack * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CallBack )( 
            ICallBack * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CallBack )( 
            ICallBack * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DoWork )( 
            ICallBack * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCallBack )( 
            ICallBack * This,
            /* [in] */ VARIANT var);
        
        END_INTERFACE
    } ICallBackVtbl;

    interface ICallBack
    {
        CONST_VTBL struct ICallBackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICallBack_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICallBack_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICallBack_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICallBack_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICallBack_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICallBack_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICallBack_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICallBack_get_CallBack(This,pVal)	\
    (This)->lpVtbl -> get_CallBack(This,pVal)

#define ICallBack_put_CallBack(This,newVal)	\
    (This)->lpVtbl -> put_CallBack(This,newVal)

#define ICallBack_DoWork(This)	\
    (This)->lpVtbl -> DoWork(This)

#define ICallBack_SetCallBack(This,var)	\
    (This)->lpVtbl -> SetCallBack(This,var)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICallBack_get_CallBack_Proxy( 
    ICallBack * This,
    /* [retval][out] */ VARIANT *pVal);


void __RPC_STUB ICallBack_get_CallBack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICallBack_put_CallBack_Proxy( 
    ICallBack * This,
    /* [in] */ VARIANT newVal);


void __RPC_STUB ICallBack_put_CallBack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICallBack_DoWork_Proxy( 
    ICallBack * This);


void __RPC_STUB ICallBack_DoWork_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICallBack_SetCallBack_Proxy( 
    ICallBack * This,
    /* [in] */ VARIANT var);


void __RPC_STUB ICallBack_SetCallBack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICallBack_INTERFACE_DEFINED__ */


#ifndef __IMiniObject_INTERFACE_DEFINED__
#define __IMiniObject_INTERFACE_DEFINED__

/* interface IMiniObject */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMiniObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("62F069C8-CA28-4F7F-8017-CF5DF453E922")
    IMiniObject : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CallBack( 
            /* [in] */ IDispatch *newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Do( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMiniObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMiniObject * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMiniObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMiniObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMiniObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMiniObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMiniObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMiniObject * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CallBack )( 
            IMiniObject * This,
            /* [in] */ IDispatch *newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Do )( 
            IMiniObject * This);
        
        END_INTERFACE
    } IMiniObjectVtbl;

    interface IMiniObject
    {
        CONST_VTBL struct IMiniObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMiniObject_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMiniObject_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMiniObject_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMiniObject_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMiniObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMiniObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMiniObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMiniObject_put_CallBack(This,newVal)	\
    (This)->lpVtbl -> put_CallBack(This,newVal)

#define IMiniObject_Do(This)	\
    (This)->lpVtbl -> Do(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMiniObject_put_CallBack_Proxy( 
    IMiniObject * This,
    /* [in] */ IDispatch *newVal);


void __RPC_STUB IMiniObject_put_CallBack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMiniObject_Do_Proxy( 
    IMiniObject * This);


void __RPC_STUB IMiniObject_Do_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMiniObject_INTERFACE_DEFINED__ */



#ifndef __WebTestLib_LIBRARY_DEFINED__
#define __WebTestLib_LIBRARY_DEFINED__

/* library WebTestLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_WebTestLib;

#ifndef ___IThreadEventEvents_DISPINTERFACE_DEFINED__
#define ___IThreadEventEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IThreadEventEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IThreadEventEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D99756EA-6CDD-438D-AD50-E90608FDBDAB")
    _IThreadEventEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IThreadEventEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IThreadEventEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IThreadEventEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IThreadEventEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IThreadEventEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IThreadEventEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IThreadEventEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IThreadEventEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IThreadEventEventsVtbl;

    interface _IThreadEventEvents
    {
        CONST_VTBL struct _IThreadEventEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IThreadEventEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IThreadEventEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IThreadEventEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IThreadEventEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IThreadEventEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IThreadEventEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IThreadEventEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IThreadEventEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ThreadEvent;

#ifdef __cplusplus

class DECLSPEC_UUID("FCD3826B-72C0-423A-B792-37EF54B3B62D")
ThreadEvent;
#endif

EXTERN_C const CLSID CLSID_CallBack;

#ifdef __cplusplus

class DECLSPEC_UUID("695E2785-111E-4E6A-9FF4-A3C24FD4D9C7")
CallBack;
#endif

EXTERN_C const CLSID CLSID_MiniObject;

#ifdef __cplusplus

class DECLSPEC_UUID("A7E5B589-3014-46CB-8F97-DA621FA09690")
MiniObject;
#endif
#endif /* __WebTestLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


