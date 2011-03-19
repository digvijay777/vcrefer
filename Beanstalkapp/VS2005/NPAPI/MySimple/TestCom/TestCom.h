

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __TestCom_h__
#define __TestCom_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISimple_FWD_DEFINED__
#define __ISimple_FWD_DEFINED__
typedef interface ISimple ISimple;
#endif 	/* __ISimple_FWD_DEFINED__ */


#ifndef __ISimple2_FWD_DEFINED__
#define __ISimple2_FWD_DEFINED__
typedef interface ISimple2 ISimple2;
#endif 	/* __ISimple2_FWD_DEFINED__ */


#ifndef __IATLControl_FWD_DEFINED__
#define __IATLControl_FWD_DEFINED__
typedef interface IATLControl IATLControl;
#endif 	/* __IATLControl_FWD_DEFINED__ */


#ifndef __ITestWindow_FWD_DEFINED__
#define __ITestWindow_FWD_DEFINED__
typedef interface ITestWindow ITestWindow;
#endif 	/* __ITestWindow_FWD_DEFINED__ */


#ifndef __Simple_FWD_DEFINED__
#define __Simple_FWD_DEFINED__

#ifdef __cplusplus
typedef class Simple Simple;
#else
typedef struct Simple Simple;
#endif /* __cplusplus */

#endif 	/* __Simple_FWD_DEFINED__ */


#ifndef __Simple2_FWD_DEFINED__
#define __Simple2_FWD_DEFINED__

#ifdef __cplusplus
typedef class Simple2 Simple2;
#else
typedef struct Simple2 Simple2;
#endif /* __cplusplus */

#endif 	/* __Simple2_FWD_DEFINED__ */


#ifndef __ATLControl_FWD_DEFINED__
#define __ATLControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class ATLControl ATLControl;
#else
typedef struct ATLControl ATLControl;
#endif /* __cplusplus */

#endif 	/* __ATLControl_FWD_DEFINED__ */


#ifndef __TestWindow_FWD_DEFINED__
#define __TestWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class TestWindow TestWindow;
#else
typedef struct TestWindow TestWindow;
#endif /* __cplusplus */

#endif 	/* __TestWindow_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ISimple_INTERFACE_DEFINED__
#define __ISimple_INTERFACE_DEFINED__

/* interface ISimple */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISimple;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5485B63C-B773-49BD-B14A-289184615D70")
    ISimple : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ISimpleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISimple * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISimple * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISimple * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISimple * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISimple * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISimple * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISimple * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ISimpleVtbl;

    interface ISimple
    {
        CONST_VTBL struct ISimpleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISimple_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISimple_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISimple_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISimple_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISimple_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISimple_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISimple_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISimple_INTERFACE_DEFINED__ */


#ifndef __ISimple2_INTERFACE_DEFINED__
#define __ISimple2_INTERFACE_DEFINED__

/* interface ISimple2 */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISimple2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("85EB30BA-1820-43D9-8D2D-C7645521C68B")
    ISimple2 : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ISimple2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISimple2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISimple2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISimple2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISimple2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISimple2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISimple2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISimple2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ISimple2Vtbl;

    interface ISimple2
    {
        CONST_VTBL struct ISimple2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISimple2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISimple2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISimple2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISimple2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISimple2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISimple2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISimple2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISimple2_INTERFACE_DEFINED__ */


#ifndef __IATLControl_INTERFACE_DEFINED__
#define __IATLControl_INTERFACE_DEFINED__

/* interface IATLControl */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IATLControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CCE220DF-2F87-4B13-95BD-5BDD161E51BF")
    IATLControl : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IATLControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IATLControl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IATLControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IATLControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IATLControl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IATLControl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IATLControl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IATLControl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IATLControlVtbl;

    interface IATLControl
    {
        CONST_VTBL struct IATLControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IATLControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IATLControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IATLControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IATLControl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IATLControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IATLControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IATLControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IATLControl_INTERFACE_DEFINED__ */


#ifndef __ITestWindow_INTERFACE_DEFINED__
#define __ITestWindow_INTERFACE_DEFINED__

/* interface ITestWindow */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ITestWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("29D30587-8E31-4595-8DF5-BA8259B66470")
    ITestWindow : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ITestWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITestWindow * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITestWindow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITestWindow * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITestWindow * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITestWindow * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITestWindow * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITestWindow * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ITestWindowVtbl;

    interface ITestWindow
    {
        CONST_VTBL struct ITestWindowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITestWindow_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITestWindow_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITestWindow_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITestWindow_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITestWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITestWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITestWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITestWindow_INTERFACE_DEFINED__ */



#ifndef __TestComLib_LIBRARY_DEFINED__
#define __TestComLib_LIBRARY_DEFINED__

/* library TestComLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_TestComLib;

EXTERN_C const CLSID CLSID_Simple;

#ifdef __cplusplus

class DECLSPEC_UUID("F3D5C9B3-6C35-4066-8ADC-63D729C35F10")
Simple;
#endif

EXTERN_C const CLSID CLSID_Simple2;

#ifdef __cplusplus

class DECLSPEC_UUID("9082122F-3E3E-492E-8F8B-8E618449E488")
Simple2;
#endif

EXTERN_C const CLSID CLSID_ATLControl;

#ifdef __cplusplus

class DECLSPEC_UUID("23AFB164-E01F-4AD1-AAC1-3E8EBE9B168F")
ATLControl;
#endif

EXTERN_C const CLSID CLSID_TestWindow;

#ifdef __cplusplus

class DECLSPEC_UUID("D867829A-60F8-446E-A949-E0D24062D97C")
TestWindow;
#endif
#endif /* __TestComLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


