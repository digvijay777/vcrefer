

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Wed Apr 27 13:31:31 2011
 */
/* Compiler settings for .\XWebPlugins2.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __XWebPlugins2_h__
#define __XWebPlugins2_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IIEWebPlugin_FWD_DEFINED__
#define __IIEWebPlugin_FWD_DEFINED__
typedef interface IIEWebPlugin IIEWebPlugin;
#endif 	/* __IIEWebPlugin_FWD_DEFINED__ */


#ifndef __IEWebPlugin_FWD_DEFINED__
#define __IEWebPlugin_FWD_DEFINED__

#ifdef __cplusplus
typedef class IEWebPlugin IEWebPlugin;
#else
typedef struct IEWebPlugin IEWebPlugin;
#endif /* __cplusplus */

#endif 	/* __IEWebPlugin_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IIEWebPlugin_INTERFACE_DEFINED__
#define __IIEWebPlugin_INTERFACE_DEFINED__

/* interface IIEWebPlugin */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IIEWebPlugin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5077BB90-4C02-44AE-BC08-5AE599A4A1A0")
    IIEWebPlugin : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IIEWebPluginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IIEWebPlugin * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IIEWebPlugin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IIEWebPlugin * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IIEWebPlugin * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IIEWebPlugin * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IIEWebPlugin * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IIEWebPlugin * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IIEWebPluginVtbl;

    interface IIEWebPlugin
    {
        CONST_VTBL struct IIEWebPluginVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIEWebPlugin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IIEWebPlugin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IIEWebPlugin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IIEWebPlugin_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IIEWebPlugin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IIEWebPlugin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IIEWebPlugin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IIEWebPlugin_INTERFACE_DEFINED__ */



#ifndef __XWebPlugins2Lib_LIBRARY_DEFINED__
#define __XWebPlugins2Lib_LIBRARY_DEFINED__

/* library XWebPlugins2Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_XWebPlugins2Lib;

EXTERN_C const CLSID CLSID_IEWebPlugin;

#ifdef __cplusplus

class DECLSPEC_UUID("94C4BE48-5122-4823-960E-823E62393209")
IEWebPlugin;
#endif
#endif /* __XWebPlugins2Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


