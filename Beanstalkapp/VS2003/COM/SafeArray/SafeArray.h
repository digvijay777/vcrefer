

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Oct 15 10:01:27 2010
 */
/* Compiler settings for .\SafeArray.idl:
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

#ifndef __SafeArray_h__
#define __SafeArray_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IArrayTest_FWD_DEFINED__
#define __IArrayTest_FWD_DEFINED__
typedef interface IArrayTest IArrayTest;
#endif 	/* __IArrayTest_FWD_DEFINED__ */


#ifndef __ISTAtest_FWD_DEFINED__
#define __ISTAtest_FWD_DEFINED__
typedef interface ISTAtest ISTAtest;
#endif 	/* __ISTAtest_FWD_DEFINED__ */


#ifndef __ArrayTest_FWD_DEFINED__
#define __ArrayTest_FWD_DEFINED__

#ifdef __cplusplus
typedef class ArrayTest ArrayTest;
#else
typedef struct ArrayTest ArrayTest;
#endif /* __cplusplus */

#endif 	/* __ArrayTest_FWD_DEFINED__ */


#ifndef __STAtest_FWD_DEFINED__
#define __STAtest_FWD_DEFINED__

#ifdef __cplusplus
typedef class STAtest STAtest;
#else
typedef struct STAtest STAtest;
#endif /* __cplusplus */

#endif 	/* __STAtest_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IArrayTest_INTERFACE_DEFINED__
#define __IArrayTest_INTERFACE_DEFINED__

/* interface IArrayTest */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IArrayTest;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E131B8EE-A6BE-4045-B5CB-8A86E9E0D23E")
    IArrayTest : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TestBSTRArray( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetObject( 
            /* [in] */ VARIANT vScript,
            /* [retval][out] */ VARIANT *lpVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IArrayTestVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IArrayTest * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IArrayTest * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IArrayTest * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IArrayTest * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IArrayTest * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IArrayTest * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IArrayTest * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TestBSTRArray )( 
            IArrayTest * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetObject )( 
            IArrayTest * This,
            /* [in] */ VARIANT vScript,
            /* [retval][out] */ VARIANT *lpVal);
        
        END_INTERFACE
    } IArrayTestVtbl;

    interface IArrayTest
    {
        CONST_VTBL struct IArrayTestVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IArrayTest_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IArrayTest_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IArrayTest_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IArrayTest_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IArrayTest_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IArrayTest_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IArrayTest_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IArrayTest_TestBSTRArray(This,pVal)	\
    (This)->lpVtbl -> TestBSTRArray(This,pVal)

#define IArrayTest_GetObject(This,vScript,lpVal)	\
    (This)->lpVtbl -> GetObject(This,vScript,lpVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IArrayTest_TestBSTRArray_Proxy( 
    IArrayTest * This,
    /* [retval][out] */ VARIANT *pVal);


void __RPC_STUB IArrayTest_TestBSTRArray_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IArrayTest_GetObject_Proxy( 
    IArrayTest * This,
    /* [in] */ VARIANT vScript,
    /* [retval][out] */ VARIANT *lpVal);


void __RPC_STUB IArrayTest_GetObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IArrayTest_INTERFACE_DEFINED__ */


#ifndef __ISTAtest_INTERFACE_DEFINED__
#define __ISTAtest_INTERFACE_DEFINED__

/* interface ISTAtest */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISTAtest;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4C4C3A14-975A-44D1-A990-81C4D02BDE9C")
    ISTAtest : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetID( 
            /* [retval][out] */ LONG *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISTAtestVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISTAtest * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISTAtest * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISTAtest * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISTAtest * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISTAtest * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISTAtest * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISTAtest * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetID )( 
            ISTAtest * This,
            /* [retval][out] */ LONG *pVal);
        
        END_INTERFACE
    } ISTAtestVtbl;

    interface ISTAtest
    {
        CONST_VTBL struct ISTAtestVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISTAtest_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISTAtest_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISTAtest_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISTAtest_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISTAtest_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISTAtest_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISTAtest_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISTAtest_GetID(This,pVal)	\
    (This)->lpVtbl -> GetID(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISTAtest_GetID_Proxy( 
    ISTAtest * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB ISTAtest_GetID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISTAtest_INTERFACE_DEFINED__ */



#ifndef __SafeArrayLib_LIBRARY_DEFINED__
#define __SafeArrayLib_LIBRARY_DEFINED__

/* library SafeArrayLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SafeArrayLib;

EXTERN_C const CLSID CLSID_ArrayTest;

#ifdef __cplusplus

class DECLSPEC_UUID("D731B46C-2575-4014-AF65-687DA0534B43")
ArrayTest;
#endif

EXTERN_C const CLSID CLSID_STAtest;

#ifdef __cplusplus

class DECLSPEC_UUID("10820A32-D879-4A48-AD53-3AEF68CEF04B")
STAtest;
#endif
#endif /* __SafeArrayLib_LIBRARY_DEFINED__ */

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


