

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed Aug 25 10:18:12 2010
 */
/* Compiler settings for .\IEExtend.idl:
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

#ifndef __IEExtend_h__
#define __IEExtend_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IILookSoruce_FWD_DEFINED__
#define __IILookSoruce_FWD_DEFINED__
typedef interface IILookSoruce IILookSoruce;
#endif 	/* __IILookSoruce_FWD_DEFINED__ */


#ifndef __ILookSoruce_FWD_DEFINED__
#define __ILookSoruce_FWD_DEFINED__

#ifdef __cplusplus
typedef class ILookSoruce ILookSoruce;
#else
typedef struct ILookSoruce ILookSoruce;
#endif /* __cplusplus */

#endif 	/* __ILookSoruce_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IILookSoruce_INTERFACE_DEFINED__
#define __IILookSoruce_INTERFACE_DEFINED__

/* interface IILookSoruce */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IILookSoruce;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D45540F7-9CD7-4F21-A660-D5582BA12598")
    IILookSoruce : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IILookSoruceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IILookSoruce * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IILookSoruce * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IILookSoruce * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IILookSoruce * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IILookSoruce * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IILookSoruce * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IILookSoruce * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IILookSoruceVtbl;

    interface IILookSoruce
    {
        CONST_VTBL struct IILookSoruceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IILookSoruce_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IILookSoruce_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IILookSoruce_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IILookSoruce_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IILookSoruce_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IILookSoruce_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IILookSoruce_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IILookSoruce_INTERFACE_DEFINED__ */



#ifndef __IEExtendLib_LIBRARY_DEFINED__
#define __IEExtendLib_LIBRARY_DEFINED__

/* library IEExtendLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_IEExtendLib;

EXTERN_C const CLSID CLSID_ILookSoruce;

#ifdef __cplusplus

class DECLSPEC_UUID("37EF2A4D-C80A-44FC-A678-6DF2F22D1794")
ILookSoruce;
#endif
#endif /* __IEExtendLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


