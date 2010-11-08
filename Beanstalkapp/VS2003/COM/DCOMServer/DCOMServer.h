

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Dec 25 15:57:06 2009
 */
/* Compiler settings for .\DCOMServer.idl:
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

#ifndef __DCOMServer_h__
#define __DCOMServer_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFirstDCOM_FWD_DEFINED__
#define __IFirstDCOM_FWD_DEFINED__
typedef interface IFirstDCOM IFirstDCOM;
#endif 	/* __IFirstDCOM_FWD_DEFINED__ */


#ifndef __FirstDCOM_FWD_DEFINED__
#define __FirstDCOM_FWD_DEFINED__

#ifdef __cplusplus
typedef class FirstDCOM FirstDCOM;
#else
typedef struct FirstDCOM FirstDCOM;
#endif /* __cplusplus */

#endif 	/* __FirstDCOM_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IFirstDCOM_INTERFACE_DEFINED__
#define __IFirstDCOM_INTERFACE_DEFINED__

/* interface IFirstDCOM */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IFirstDCOM;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FC2FCA1B-EA4B-4BAA-A7EB-2A219DBF8550")
    IFirstDCOM : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetServerName( 
            /* [retval][out] */ BSTR *lpServer) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFirstDCOMVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFirstDCOM * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFirstDCOM * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFirstDCOM * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFirstDCOM * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFirstDCOM * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFirstDCOM * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFirstDCOM * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetServerName )( 
            IFirstDCOM * This,
            /* [retval][out] */ BSTR *lpServer);
        
        END_INTERFACE
    } IFirstDCOMVtbl;

    interface IFirstDCOM
    {
        CONST_VTBL struct IFirstDCOMVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFirstDCOM_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFirstDCOM_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFirstDCOM_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFirstDCOM_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFirstDCOM_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFirstDCOM_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFirstDCOM_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IFirstDCOM_GetServerName(This,lpServer)	\
    (This)->lpVtbl -> GetServerName(This,lpServer)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFirstDCOM_GetServerName_Proxy( 
    IFirstDCOM * This,
    /* [retval][out] */ BSTR *lpServer);


void __RPC_STUB IFirstDCOM_GetServerName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFirstDCOM_INTERFACE_DEFINED__ */



#ifndef __DCOMServerLib_LIBRARY_DEFINED__
#define __DCOMServerLib_LIBRARY_DEFINED__

/* library DCOMServerLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DCOMServerLib;

EXTERN_C const CLSID CLSID_FirstDCOM;

#ifdef __cplusplus

class DECLSPEC_UUID("42D6559F-6472-4A84-B8B0-53938B18CCB4")
FirstDCOM;
#endif
#endif /* __DCOMServerLib_LIBRARY_DEFINED__ */

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


