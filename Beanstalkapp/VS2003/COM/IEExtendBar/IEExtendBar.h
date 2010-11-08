

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed Aug 25 13:25:45 2010
 */
/* Compiler settings for .\IEExtendBar.idl:
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

#ifndef __IEExtendBar_h__
#define __IEExtendBar_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICookiesLook_FWD_DEFINED__
#define __ICookiesLook_FWD_DEFINED__
typedef interface ICookiesLook ICookiesLook;
#endif 	/* __ICookiesLook_FWD_DEFINED__ */


#ifndef __CookiesLook_FWD_DEFINED__
#define __CookiesLook_FWD_DEFINED__

#ifdef __cplusplus
typedef class CookiesLook CookiesLook;
#else
typedef struct CookiesLook CookiesLook;
#endif /* __cplusplus */

#endif 	/* __CookiesLook_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ICookiesLook_INTERFACE_DEFINED__
#define __ICookiesLook_INTERFACE_DEFINED__

/* interface ICookiesLook */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICookiesLook;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0AB45B53-7B12-4012-883F-1DC8B0847B05")
    ICookiesLook : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICookiesLookVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICookiesLook * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICookiesLook * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICookiesLook * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICookiesLook * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICookiesLook * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICookiesLook * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICookiesLook * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ICookiesLookVtbl;

    interface ICookiesLook
    {
        CONST_VTBL struct ICookiesLookVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICookiesLook_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICookiesLook_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICookiesLook_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICookiesLook_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICookiesLook_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICookiesLook_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICookiesLook_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICookiesLook_INTERFACE_DEFINED__ */



#ifndef __IEExtendBarLib_LIBRARY_DEFINED__
#define __IEExtendBarLib_LIBRARY_DEFINED__

/* library IEExtendBarLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_IEExtendBarLib;

EXTERN_C const CLSID CLSID_CookiesLook;

#ifdef __cplusplus

class DECLSPEC_UUID("82AE3FB7-CDA9-4F46-B6CB-939DDD14613A")
CookiesLook;
#endif
#endif /* __IEExtendBarLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


