

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Fri Apr 22 17:47:21 2011
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

#ifndef __INPWebPlugin_FWD_DEFINED__
#define __INPWebPlugin_FWD_DEFINED__
typedef interface INPWebPlugin INPWebPlugin;
#endif 	/* __INPWebPlugin_FWD_DEFINED__ */


#ifndef __NPWebPlugin_FWD_DEFINED__
#define __NPWebPlugin_FWD_DEFINED__

#ifdef __cplusplus
typedef class NPWebPlugin NPWebPlugin;
#else
typedef struct NPWebPlugin NPWebPlugin;
#endif /* __cplusplus */

#endif 	/* __NPWebPlugin_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __INPWebPlugin_INTERFACE_DEFINED__
#define __INPWebPlugin_INTERFACE_DEFINED__

/* interface INPWebPlugin */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_INPWebPlugin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8FD9A7C8-58CC-4241-8070-1195EA6E4EF0")
    INPWebPlugin : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct INPWebPluginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INPWebPlugin * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INPWebPlugin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INPWebPlugin * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            INPWebPlugin * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            INPWebPlugin * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            INPWebPlugin * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            INPWebPlugin * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } INPWebPluginVtbl;

    interface INPWebPlugin
    {
        CONST_VTBL struct INPWebPluginVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INPWebPlugin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define INPWebPlugin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define INPWebPlugin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define INPWebPlugin_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define INPWebPlugin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define INPWebPlugin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define INPWebPlugin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INPWebPlugin_INTERFACE_DEFINED__ */



#ifndef __XWebPlugins2Lib_LIBRARY_DEFINED__
#define __XWebPlugins2Lib_LIBRARY_DEFINED__

/* library XWebPlugins2Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_XWebPlugins2Lib;

EXTERN_C const CLSID CLSID_NPWebPlugin;

#ifdef __cplusplus

class DECLSPEC_UUID("FD8A830B-4C7D-435A-8D35-E277BC76AA3D")
NPWebPlugin;
#endif
#endif /* __XWebPlugins2Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


