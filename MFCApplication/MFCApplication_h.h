

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for MFCApplication.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __MFCApplication_h_h__
#define __MFCApplication_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMFCApplication_FWD_DEFINED__
#define __IMFCApplication_FWD_DEFINED__
typedef interface IMFCApplication IMFCApplication;

#endif 	/* __IMFCApplication_FWD_DEFINED__ */


#ifndef __CMFCApplicationDoc_FWD_DEFINED__
#define __CMFCApplicationDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CMFCApplicationDoc CMFCApplicationDoc;
#else
typedef struct CMFCApplicationDoc CMFCApplicationDoc;
#endif /* __cplusplus */

#endif 	/* __CMFCApplicationDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __MFCApplication_LIBRARY_DEFINED__
#define __MFCApplication_LIBRARY_DEFINED__

/* library MFCApplication */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_MFCApplication;

#ifndef __IMFCApplication_DISPINTERFACE_DEFINED__
#define __IMFCApplication_DISPINTERFACE_DEFINED__

/* dispinterface IMFCApplication */
/* [uuid] */ 


EXTERN_C const IID DIID_IMFCApplication;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("faa7acdb-56e2-45eb-a130-11f9ff12a57f")
    IMFCApplication : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMFCApplicationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFCApplication * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFCApplication * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFCApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMFCApplication * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMFCApplication * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMFCApplication * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMFCApplication * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IMFCApplicationVtbl;

    interface IMFCApplication
    {
        CONST_VTBL struct IMFCApplicationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFCApplication_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFCApplication_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFCApplication_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFCApplication_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMFCApplication_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMFCApplication_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMFCApplication_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMFCApplication_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CMFCApplicationDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("a589a381-8c85-4fb3-8596-eb25cb7b8c90")
CMFCApplicationDoc;
#endif
#endif /* __MFCApplication_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


