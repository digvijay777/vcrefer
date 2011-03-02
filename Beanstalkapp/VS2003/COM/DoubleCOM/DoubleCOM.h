

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Nov 26 14:00:05 2010
 */
/* Compiler settings for .\DoubleCOM.idl:
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

#ifndef __DoubleCOM_h__
#define __DoubleCOM_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Itest1_FWD_DEFINED__
#define __Itest1_FWD_DEFINED__
typedef interface Itest1 Itest1;
#endif 	/* __Itest1_FWD_DEFINED__ */


#ifndef __Itest2_FWD_DEFINED__
#define __Itest2_FWD_DEFINED__
typedef interface Itest2 Itest2;
#endif 	/* __Itest2_FWD_DEFINED__ */


#ifndef __test1_FWD_DEFINED__
#define __test1_FWD_DEFINED__

#ifdef __cplusplus
typedef class test1 test1;
#else
typedef struct test1 test1;
#endif /* __cplusplus */

#endif 	/* __test1_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __Itest1_INTERFACE_DEFINED__
#define __Itest1_INTERFACE_DEFINED__

/* interface Itest1 */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_Itest1;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("764AD375-1FD0-41D0-AB6B-862B764DEF59")
    Itest1 : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetStr1( 
            /* [retval][out] */ BSTR *lpVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct Itest1Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Itest1 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Itest1 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Itest1 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Itest1 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Itest1 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Itest1 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Itest1 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetStr1 )( 
            Itest1 * This,
            /* [retval][out] */ BSTR *lpVal);
        
        END_INTERFACE
    } Itest1Vtbl;

    interface Itest1
    {
        CONST_VTBL struct Itest1Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Itest1_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define Itest1_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define Itest1_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define Itest1_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define Itest1_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define Itest1_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define Itest1_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define Itest1_GetStr1(This,lpVal)	\
    (This)->lpVtbl -> GetStr1(This,lpVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Itest1_GetStr1_Proxy( 
    Itest1 * This,
    /* [retval][out] */ BSTR *lpVal);


void __RPC_STUB Itest1_GetStr1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __Itest1_INTERFACE_DEFINED__ */


#ifndef __Itest2_INTERFACE_DEFINED__
#define __Itest2_INTERFACE_DEFINED__

/* interface Itest2 */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_Itest2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("85434049-A03C-46B2-AC77-F56ED61D9033")
    Itest2 : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetStr2( 
            /* [retval][out] */ BSTR *lpVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct Itest2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Itest2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Itest2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Itest2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Itest2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Itest2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Itest2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Itest2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetStr2 )( 
            Itest2 * This,
            /* [retval][out] */ BSTR *lpVal);
        
        END_INTERFACE
    } Itest2Vtbl;

    interface Itest2
    {
        CONST_VTBL struct Itest2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Itest2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define Itest2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define Itest2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define Itest2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define Itest2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define Itest2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define Itest2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define Itest2_GetStr2(This,lpVal)	\
    (This)->lpVtbl -> GetStr2(This,lpVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Itest2_GetStr2_Proxy( 
    Itest2 * This,
    /* [retval][out] */ BSTR *lpVal);


void __RPC_STUB Itest2_GetStr2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __Itest2_INTERFACE_DEFINED__ */



#ifndef __DoubleCOMLib_LIBRARY_DEFINED__
#define __DoubleCOMLib_LIBRARY_DEFINED__

/* library DoubleCOMLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DoubleCOMLib;

EXTERN_C const CLSID CLSID_test1;

#ifdef __cplusplus

class DECLSPEC_UUID("FB72DE77-1D76-47AD-A335-29CCEA39A469")
test1;
#endif
#endif /* __DoubleCOMLib_LIBRARY_DEFINED__ */

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


