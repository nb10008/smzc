

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Thu Jan 20 14:18:57 2011
 */
/* Compiler settings for .\htmlancher.idl:
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

#ifndef __htmlancher_h__
#define __htmlancher_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IHtmLanch_FWD_DEFINED__
#define __IHtmLanch_FWD_DEFINED__
typedef interface IHtmLanch IHtmLanch;
#endif 	/* __IHtmLanch_FWD_DEFINED__ */


#ifndef ___IHtmLanchEvents_FWD_DEFINED__
#define ___IHtmLanchEvents_FWD_DEFINED__
typedef interface _IHtmLanchEvents _IHtmLanchEvents;
#endif 	/* ___IHtmLanchEvents_FWD_DEFINED__ */


#ifndef __HtmLanch_FWD_DEFINED__
#define __HtmLanch_FWD_DEFINED__

#ifdef __cplusplus
typedef class HtmLanch HtmLanch;
#else
typedef struct HtmLanch HtmLanch;
#endif /* __cplusplus */

#endif 	/* __HtmLanch_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IHtmLanch_INTERFACE_DEFINED__
#define __IHtmLanch_INTERFACE_DEFINED__

/* interface IHtmLanch */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHtmLanch;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8AEF255C-5431-4D81-8C25-F7BA25D17488")
    IHtmLanch : public IDispatch
    {
    public:
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_AutoSize( 
            /* [in] */ VARIANT_BOOL vbool) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_AutoSize( 
            /* [retval][out] */ VARIANT_BOOL *pbool) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_BackColor( 
            /* [in] */ OLE_COLOR clr) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_BackColor( 
            /* [retval][out] */ OLE_COLOR *pclr) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_BackStyle( 
            /* [in] */ long style) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_BackStyle( 
            /* [retval][out] */ long *pstyle) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_BorderColor( 
            /* [in] */ OLE_COLOR clr) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_BorderColor( 
            /* [retval][out] */ OLE_COLOR *pclr) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_BorderStyle( 
            /* [in] */ long style) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_BorderStyle( 
            /* [retval][out] */ long *pstyle) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_BorderWidth( 
            /* [in] */ long width) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_BorderWidth( 
            /* [retval][out] */ long *width) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_DrawMode( 
            /* [in] */ long mode) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_DrawMode( 
            /* [retval][out] */ long *pmode) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_DrawStyle( 
            /* [in] */ long style) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_DrawStyle( 
            /* [retval][out] */ long *pstyle) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_DrawWidth( 
            /* [in] */ long width) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_DrawWidth( 
            /* [retval][out] */ long *pwidth) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_FillColor( 
            /* [in] */ OLE_COLOR clr) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_FillColor( 
            /* [retval][out] */ OLE_COLOR *pclr) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_FillStyle( 
            /* [in] */ long style) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_FillStyle( 
            /* [retval][out] */ long *pstyle) = 0;
        
        virtual /* [id][requestedit][bindable][propputref] */ HRESULT STDMETHODCALLTYPE putref_Font( 
            /* [in] */ IFontDisp *pFont) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_Font( 
            /* [in] */ IFontDisp *pFont) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_Font( 
            /* [retval][out] */ IFontDisp **ppFont) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_ForeColor( 
            /* [in] */ OLE_COLOR clr) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_ForeColor( 
            /* [retval][out] */ OLE_COLOR *pclr) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL vbool) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *pbool) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_HWND( 
            /* [retval][out] */ LONG_PTR *pHWND) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_TabStop( 
            /* [in] */ VARIANT_BOOL vbool) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_TabStop( 
            /* [retval][out] */ VARIANT_BOOL *pbool) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_Text( 
            /* [in] */ BSTR strText) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_Text( 
            /* [retval][out] */ BSTR *pstrText) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_Caption( 
            /* [in] */ BSTR strCaption) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_Caption( 
            /* [retval][out] */ BSTR *pstrCaption) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_BorderVisible( 
            /* [in] */ VARIANT_BOOL vbool) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_BorderVisible( 
            /* [retval][out] */ VARIANT_BOOL *pbool) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_Appearance( 
            /* [in] */ short nAppearance) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_Appearance( 
            /* [retval][out] */ short *pnAppearance) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_MousePointer( 
            /* [in] */ long pointer) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_MousePointer( 
            /* [retval][out] */ long *ppointer) = 0;
        
        virtual /* [id][requestedit][bindable][propputref] */ HRESULT STDMETHODCALLTYPE putref_MouseIcon( 
            /* [in] */ IPictureDisp *pMouseIcon) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_MouseIcon( 
            /* [in] */ IPictureDisp *pMouseIcon) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_MouseIcon( 
            /* [retval][out] */ IPictureDisp **ppMouseIcon) = 0;
        
        virtual /* [id][requestedit][bindable][propputref] */ HRESULT STDMETHODCALLTYPE putref_Picture( 
            /* [in] */ IPictureDisp *pPicture) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_Picture( 
            /* [in] */ IPictureDisp *pPicture) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_Picture( 
            /* [retval][out] */ IPictureDisp **ppPicture) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_Valid( 
            /* [in] */ VARIANT_BOOL vbool) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_Valid( 
            /* [retval][out] */ VARIANT_BOOL *pbool) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_ReadyState( 
            /* [in] */ long state) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_ReadyState( 
            /* [retval][out] */ long *pstate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Lanch( 
            /* [in] */ BSTR sApp,
            /* [in] */ BSTR sParam) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHtmLanchVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHtmLanch * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHtmLanch * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHtmLanch * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHtmLanch * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHtmLanch * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHtmLanch * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHtmLanch * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AutoSize )( 
            IHtmLanch * This,
            /* [in] */ VARIANT_BOOL vbool);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AutoSize )( 
            IHtmLanch * This,
            /* [retval][out] */ VARIANT_BOOL *pbool);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BackColor )( 
            IHtmLanch * This,
            /* [in] */ OLE_COLOR clr);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BackColor )( 
            IHtmLanch * This,
            /* [retval][out] */ OLE_COLOR *pclr);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BackStyle )( 
            IHtmLanch * This,
            /* [in] */ long style);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BackStyle )( 
            IHtmLanch * This,
            /* [retval][out] */ long *pstyle);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BorderColor )( 
            IHtmLanch * This,
            /* [in] */ OLE_COLOR clr);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BorderColor )( 
            IHtmLanch * This,
            /* [retval][out] */ OLE_COLOR *pclr);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BorderStyle )( 
            IHtmLanch * This,
            /* [in] */ long style);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BorderStyle )( 
            IHtmLanch * This,
            /* [retval][out] */ long *pstyle);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BorderWidth )( 
            IHtmLanch * This,
            /* [in] */ long width);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BorderWidth )( 
            IHtmLanch * This,
            /* [retval][out] */ long *width);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DrawMode )( 
            IHtmLanch * This,
            /* [in] */ long mode);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DrawMode )( 
            IHtmLanch * This,
            /* [retval][out] */ long *pmode);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DrawStyle )( 
            IHtmLanch * This,
            /* [in] */ long style);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DrawStyle )( 
            IHtmLanch * This,
            /* [retval][out] */ long *pstyle);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DrawWidth )( 
            IHtmLanch * This,
            /* [in] */ long width);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DrawWidth )( 
            IHtmLanch * This,
            /* [retval][out] */ long *pwidth);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FillColor )( 
            IHtmLanch * This,
            /* [in] */ OLE_COLOR clr);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FillColor )( 
            IHtmLanch * This,
            /* [retval][out] */ OLE_COLOR *pclr);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FillStyle )( 
            IHtmLanch * This,
            /* [in] */ long style);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FillStyle )( 
            IHtmLanch * This,
            /* [retval][out] */ long *pstyle);
        
        /* [id][requestedit][bindable][propputref] */ HRESULT ( STDMETHODCALLTYPE *putref_Font )( 
            IHtmLanch * This,
            /* [in] */ IFontDisp *pFont);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Font )( 
            IHtmLanch * This,
            /* [in] */ IFontDisp *pFont);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Font )( 
            IHtmLanch * This,
            /* [retval][out] */ IFontDisp **ppFont);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ForeColor )( 
            IHtmLanch * This,
            /* [in] */ OLE_COLOR clr);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ForeColor )( 
            IHtmLanch * This,
            /* [retval][out] */ OLE_COLOR *pclr);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IHtmLanch * This,
            /* [in] */ VARIANT_BOOL vbool);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IHtmLanch * This,
            /* [retval][out] */ VARIANT_BOOL *pbool);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
            IHtmLanch * This,
            /* [retval][out] */ LONG_PTR *pHWND);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TabStop )( 
            IHtmLanch * This,
            /* [in] */ VARIANT_BOOL vbool);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TabStop )( 
            IHtmLanch * This,
            /* [retval][out] */ VARIANT_BOOL *pbool);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Text )( 
            IHtmLanch * This,
            /* [in] */ BSTR strText);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Text )( 
            IHtmLanch * This,
            /* [retval][out] */ BSTR *pstrText);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IHtmLanch * This,
            /* [in] */ BSTR strCaption);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IHtmLanch * This,
            /* [retval][out] */ BSTR *pstrCaption);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BorderVisible )( 
            IHtmLanch * This,
            /* [in] */ VARIANT_BOOL vbool);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BorderVisible )( 
            IHtmLanch * This,
            /* [retval][out] */ VARIANT_BOOL *pbool);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Appearance )( 
            IHtmLanch * This,
            /* [in] */ short nAppearance);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Appearance )( 
            IHtmLanch * This,
            /* [retval][out] */ short *pnAppearance);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MousePointer )( 
            IHtmLanch * This,
            /* [in] */ long pointer);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MousePointer )( 
            IHtmLanch * This,
            /* [retval][out] */ long *ppointer);
        
        /* [id][requestedit][bindable][propputref] */ HRESULT ( STDMETHODCALLTYPE *putref_MouseIcon )( 
            IHtmLanch * This,
            /* [in] */ IPictureDisp *pMouseIcon);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MouseIcon )( 
            IHtmLanch * This,
            /* [in] */ IPictureDisp *pMouseIcon);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MouseIcon )( 
            IHtmLanch * This,
            /* [retval][out] */ IPictureDisp **ppMouseIcon);
        
        /* [id][requestedit][bindable][propputref] */ HRESULT ( STDMETHODCALLTYPE *putref_Picture )( 
            IHtmLanch * This,
            /* [in] */ IPictureDisp *pPicture);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Picture )( 
            IHtmLanch * This,
            /* [in] */ IPictureDisp *pPicture);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Picture )( 
            IHtmLanch * This,
            /* [retval][out] */ IPictureDisp **ppPicture);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Valid )( 
            IHtmLanch * This,
            /* [in] */ VARIANT_BOOL vbool);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Valid )( 
            IHtmLanch * This,
            /* [retval][out] */ VARIANT_BOOL *pbool);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ReadyState )( 
            IHtmLanch * This,
            /* [in] */ long state);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ReadyState )( 
            IHtmLanch * This,
            /* [retval][out] */ long *pstate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Lanch )( 
            IHtmLanch * This,
            /* [in] */ BSTR sApp,
            /* [in] */ BSTR sParam);
        
        END_INTERFACE
    } IHtmLanchVtbl;

    interface IHtmLanch
    {
        CONST_VTBL struct IHtmLanchVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHtmLanch_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHtmLanch_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHtmLanch_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHtmLanch_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHtmLanch_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHtmLanch_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHtmLanch_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHtmLanch_put_AutoSize(This,vbool)	\
    (This)->lpVtbl -> put_AutoSize(This,vbool)

#define IHtmLanch_get_AutoSize(This,pbool)	\
    (This)->lpVtbl -> get_AutoSize(This,pbool)

#define IHtmLanch_put_BackColor(This,clr)	\
    (This)->lpVtbl -> put_BackColor(This,clr)

#define IHtmLanch_get_BackColor(This,pclr)	\
    (This)->lpVtbl -> get_BackColor(This,pclr)

#define IHtmLanch_put_BackStyle(This,style)	\
    (This)->lpVtbl -> put_BackStyle(This,style)

#define IHtmLanch_get_BackStyle(This,pstyle)	\
    (This)->lpVtbl -> get_BackStyle(This,pstyle)

#define IHtmLanch_put_BorderColor(This,clr)	\
    (This)->lpVtbl -> put_BorderColor(This,clr)

#define IHtmLanch_get_BorderColor(This,pclr)	\
    (This)->lpVtbl -> get_BorderColor(This,pclr)

#define IHtmLanch_put_BorderStyle(This,style)	\
    (This)->lpVtbl -> put_BorderStyle(This,style)

#define IHtmLanch_get_BorderStyle(This,pstyle)	\
    (This)->lpVtbl -> get_BorderStyle(This,pstyle)

#define IHtmLanch_put_BorderWidth(This,width)	\
    (This)->lpVtbl -> put_BorderWidth(This,width)

#define IHtmLanch_get_BorderWidth(This,width)	\
    (This)->lpVtbl -> get_BorderWidth(This,width)

#define IHtmLanch_put_DrawMode(This,mode)	\
    (This)->lpVtbl -> put_DrawMode(This,mode)

#define IHtmLanch_get_DrawMode(This,pmode)	\
    (This)->lpVtbl -> get_DrawMode(This,pmode)

#define IHtmLanch_put_DrawStyle(This,style)	\
    (This)->lpVtbl -> put_DrawStyle(This,style)

#define IHtmLanch_get_DrawStyle(This,pstyle)	\
    (This)->lpVtbl -> get_DrawStyle(This,pstyle)

#define IHtmLanch_put_DrawWidth(This,width)	\
    (This)->lpVtbl -> put_DrawWidth(This,width)

#define IHtmLanch_get_DrawWidth(This,pwidth)	\
    (This)->lpVtbl -> get_DrawWidth(This,pwidth)

#define IHtmLanch_put_FillColor(This,clr)	\
    (This)->lpVtbl -> put_FillColor(This,clr)

#define IHtmLanch_get_FillColor(This,pclr)	\
    (This)->lpVtbl -> get_FillColor(This,pclr)

#define IHtmLanch_put_FillStyle(This,style)	\
    (This)->lpVtbl -> put_FillStyle(This,style)

#define IHtmLanch_get_FillStyle(This,pstyle)	\
    (This)->lpVtbl -> get_FillStyle(This,pstyle)

#define IHtmLanch_putref_Font(This,pFont)	\
    (This)->lpVtbl -> putref_Font(This,pFont)

#define IHtmLanch_put_Font(This,pFont)	\
    (This)->lpVtbl -> put_Font(This,pFont)

#define IHtmLanch_get_Font(This,ppFont)	\
    (This)->lpVtbl -> get_Font(This,ppFont)

#define IHtmLanch_put_ForeColor(This,clr)	\
    (This)->lpVtbl -> put_ForeColor(This,clr)

#define IHtmLanch_get_ForeColor(This,pclr)	\
    (This)->lpVtbl -> get_ForeColor(This,pclr)

#define IHtmLanch_put_Enabled(This,vbool)	\
    (This)->lpVtbl -> put_Enabled(This,vbool)

#define IHtmLanch_get_Enabled(This,pbool)	\
    (This)->lpVtbl -> get_Enabled(This,pbool)

#define IHtmLanch_get_HWND(This,pHWND)	\
    (This)->lpVtbl -> get_HWND(This,pHWND)

#define IHtmLanch_put_TabStop(This,vbool)	\
    (This)->lpVtbl -> put_TabStop(This,vbool)

#define IHtmLanch_get_TabStop(This,pbool)	\
    (This)->lpVtbl -> get_TabStop(This,pbool)

#define IHtmLanch_put_Text(This,strText)	\
    (This)->lpVtbl -> put_Text(This,strText)

#define IHtmLanch_get_Text(This,pstrText)	\
    (This)->lpVtbl -> get_Text(This,pstrText)

#define IHtmLanch_put_Caption(This,strCaption)	\
    (This)->lpVtbl -> put_Caption(This,strCaption)

#define IHtmLanch_get_Caption(This,pstrCaption)	\
    (This)->lpVtbl -> get_Caption(This,pstrCaption)

#define IHtmLanch_put_BorderVisible(This,vbool)	\
    (This)->lpVtbl -> put_BorderVisible(This,vbool)

#define IHtmLanch_get_BorderVisible(This,pbool)	\
    (This)->lpVtbl -> get_BorderVisible(This,pbool)

#define IHtmLanch_put_Appearance(This,nAppearance)	\
    (This)->lpVtbl -> put_Appearance(This,nAppearance)

#define IHtmLanch_get_Appearance(This,pnAppearance)	\
    (This)->lpVtbl -> get_Appearance(This,pnAppearance)

#define IHtmLanch_put_MousePointer(This,pointer)	\
    (This)->lpVtbl -> put_MousePointer(This,pointer)

#define IHtmLanch_get_MousePointer(This,ppointer)	\
    (This)->lpVtbl -> get_MousePointer(This,ppointer)

#define IHtmLanch_putref_MouseIcon(This,pMouseIcon)	\
    (This)->lpVtbl -> putref_MouseIcon(This,pMouseIcon)

#define IHtmLanch_put_MouseIcon(This,pMouseIcon)	\
    (This)->lpVtbl -> put_MouseIcon(This,pMouseIcon)

#define IHtmLanch_get_MouseIcon(This,ppMouseIcon)	\
    (This)->lpVtbl -> get_MouseIcon(This,ppMouseIcon)

#define IHtmLanch_putref_Picture(This,pPicture)	\
    (This)->lpVtbl -> putref_Picture(This,pPicture)

#define IHtmLanch_put_Picture(This,pPicture)	\
    (This)->lpVtbl -> put_Picture(This,pPicture)

#define IHtmLanch_get_Picture(This,ppPicture)	\
    (This)->lpVtbl -> get_Picture(This,ppPicture)

#define IHtmLanch_put_Valid(This,vbool)	\
    (This)->lpVtbl -> put_Valid(This,vbool)

#define IHtmLanch_get_Valid(This,pbool)	\
    (This)->lpVtbl -> get_Valid(This,pbool)

#define IHtmLanch_put_ReadyState(This,state)	\
    (This)->lpVtbl -> put_ReadyState(This,state)

#define IHtmLanch_get_ReadyState(This,pstate)	\
    (This)->lpVtbl -> get_ReadyState(This,pstate)

#define IHtmLanch_Lanch(This,sApp,sParam)	\
    (This)->lpVtbl -> Lanch(This,sApp,sParam)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_AutoSize_Proxy( 
    IHtmLanch * This,
    /* [in] */ VARIANT_BOOL vbool);


void __RPC_STUB IHtmLanch_put_AutoSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_AutoSize_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ VARIANT_BOOL *pbool);


void __RPC_STUB IHtmLanch_get_AutoSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_BackColor_Proxy( 
    IHtmLanch * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB IHtmLanch_put_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_BackColor_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ OLE_COLOR *pclr);


void __RPC_STUB IHtmLanch_get_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_BackStyle_Proxy( 
    IHtmLanch * This,
    /* [in] */ long style);


void __RPC_STUB IHtmLanch_put_BackStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_BackStyle_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ long *pstyle);


void __RPC_STUB IHtmLanch_get_BackStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_BorderColor_Proxy( 
    IHtmLanch * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB IHtmLanch_put_BorderColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_BorderColor_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ OLE_COLOR *pclr);


void __RPC_STUB IHtmLanch_get_BorderColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_BorderStyle_Proxy( 
    IHtmLanch * This,
    /* [in] */ long style);


void __RPC_STUB IHtmLanch_put_BorderStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_BorderStyle_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ long *pstyle);


void __RPC_STUB IHtmLanch_get_BorderStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_BorderWidth_Proxy( 
    IHtmLanch * This,
    /* [in] */ long width);


void __RPC_STUB IHtmLanch_put_BorderWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_BorderWidth_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ long *width);


void __RPC_STUB IHtmLanch_get_BorderWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_DrawMode_Proxy( 
    IHtmLanch * This,
    /* [in] */ long mode);


void __RPC_STUB IHtmLanch_put_DrawMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_DrawMode_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ long *pmode);


void __RPC_STUB IHtmLanch_get_DrawMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_DrawStyle_Proxy( 
    IHtmLanch * This,
    /* [in] */ long style);


void __RPC_STUB IHtmLanch_put_DrawStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_DrawStyle_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ long *pstyle);


void __RPC_STUB IHtmLanch_get_DrawStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_DrawWidth_Proxy( 
    IHtmLanch * This,
    /* [in] */ long width);


void __RPC_STUB IHtmLanch_put_DrawWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_DrawWidth_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ long *pwidth);


void __RPC_STUB IHtmLanch_get_DrawWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_FillColor_Proxy( 
    IHtmLanch * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB IHtmLanch_put_FillColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_FillColor_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ OLE_COLOR *pclr);


void __RPC_STUB IHtmLanch_get_FillColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_FillStyle_Proxy( 
    IHtmLanch * This,
    /* [in] */ long style);


void __RPC_STUB IHtmLanch_put_FillStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_FillStyle_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ long *pstyle);


void __RPC_STUB IHtmLanch_get_FillStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propputref] */ HRESULT STDMETHODCALLTYPE IHtmLanch_putref_Font_Proxy( 
    IHtmLanch * This,
    /* [in] */ IFontDisp *pFont);


void __RPC_STUB IHtmLanch_putref_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_Font_Proxy( 
    IHtmLanch * This,
    /* [in] */ IFontDisp *pFont);


void __RPC_STUB IHtmLanch_put_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_Font_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ IFontDisp **ppFont);


void __RPC_STUB IHtmLanch_get_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_ForeColor_Proxy( 
    IHtmLanch * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB IHtmLanch_put_ForeColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_ForeColor_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ OLE_COLOR *pclr);


void __RPC_STUB IHtmLanch_get_ForeColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_Enabled_Proxy( 
    IHtmLanch * This,
    /* [in] */ VARIANT_BOOL vbool);


void __RPC_STUB IHtmLanch_put_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_Enabled_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ VARIANT_BOOL *pbool);


void __RPC_STUB IHtmLanch_get_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_HWND_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ LONG_PTR *pHWND);


void __RPC_STUB IHtmLanch_get_HWND_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_TabStop_Proxy( 
    IHtmLanch * This,
    /* [in] */ VARIANT_BOOL vbool);


void __RPC_STUB IHtmLanch_put_TabStop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_TabStop_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ VARIANT_BOOL *pbool);


void __RPC_STUB IHtmLanch_get_TabStop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_Text_Proxy( 
    IHtmLanch * This,
    /* [in] */ BSTR strText);


void __RPC_STUB IHtmLanch_put_Text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_Text_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ BSTR *pstrText);


void __RPC_STUB IHtmLanch_get_Text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_Caption_Proxy( 
    IHtmLanch * This,
    /* [in] */ BSTR strCaption);


void __RPC_STUB IHtmLanch_put_Caption_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_Caption_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ BSTR *pstrCaption);


void __RPC_STUB IHtmLanch_get_Caption_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_BorderVisible_Proxy( 
    IHtmLanch * This,
    /* [in] */ VARIANT_BOOL vbool);


void __RPC_STUB IHtmLanch_put_BorderVisible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_BorderVisible_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ VARIANT_BOOL *pbool);


void __RPC_STUB IHtmLanch_get_BorderVisible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_Appearance_Proxy( 
    IHtmLanch * This,
    /* [in] */ short nAppearance);


void __RPC_STUB IHtmLanch_put_Appearance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_Appearance_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ short *pnAppearance);


void __RPC_STUB IHtmLanch_get_Appearance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_MousePointer_Proxy( 
    IHtmLanch * This,
    /* [in] */ long pointer);


void __RPC_STUB IHtmLanch_put_MousePointer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_MousePointer_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ long *ppointer);


void __RPC_STUB IHtmLanch_get_MousePointer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propputref] */ HRESULT STDMETHODCALLTYPE IHtmLanch_putref_MouseIcon_Proxy( 
    IHtmLanch * This,
    /* [in] */ IPictureDisp *pMouseIcon);


void __RPC_STUB IHtmLanch_putref_MouseIcon_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_MouseIcon_Proxy( 
    IHtmLanch * This,
    /* [in] */ IPictureDisp *pMouseIcon);


void __RPC_STUB IHtmLanch_put_MouseIcon_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_MouseIcon_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ IPictureDisp **ppMouseIcon);


void __RPC_STUB IHtmLanch_get_MouseIcon_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propputref] */ HRESULT STDMETHODCALLTYPE IHtmLanch_putref_Picture_Proxy( 
    IHtmLanch * This,
    /* [in] */ IPictureDisp *pPicture);


void __RPC_STUB IHtmLanch_putref_Picture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_Picture_Proxy( 
    IHtmLanch * This,
    /* [in] */ IPictureDisp *pPicture);


void __RPC_STUB IHtmLanch_put_Picture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_Picture_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ IPictureDisp **ppPicture);


void __RPC_STUB IHtmLanch_get_Picture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_Valid_Proxy( 
    IHtmLanch * This,
    /* [in] */ VARIANT_BOOL vbool);


void __RPC_STUB IHtmLanch_put_Valid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_Valid_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ VARIANT_BOOL *pbool);


void __RPC_STUB IHtmLanch_get_Valid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE IHtmLanch_put_ReadyState_Proxy( 
    IHtmLanch * This,
    /* [in] */ long state);


void __RPC_STUB IHtmLanch_put_ReadyState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE IHtmLanch_get_ReadyState_Proxy( 
    IHtmLanch * This,
    /* [retval][out] */ long *pstate);


void __RPC_STUB IHtmLanch_get_ReadyState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHtmLanch_Lanch_Proxy( 
    IHtmLanch * This,
    /* [in] */ BSTR sApp,
    /* [in] */ BSTR sParam);


void __RPC_STUB IHtmLanch_Lanch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHtmLanch_INTERFACE_DEFINED__ */



#ifndef __htmlancherLib_LIBRARY_DEFINED__
#define __htmlancherLib_LIBRARY_DEFINED__

/* library htmlancherLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_htmlancherLib;

#ifndef ___IHtmLanchEvents_DISPINTERFACE_DEFINED__
#define ___IHtmLanchEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IHtmLanchEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IHtmLanchEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C0F93300-65C0-4F10-BFFC-C1AA3EE28F93")
    _IHtmLanchEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IHtmLanchEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IHtmLanchEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IHtmLanchEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IHtmLanchEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IHtmLanchEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IHtmLanchEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IHtmLanchEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IHtmLanchEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IHtmLanchEventsVtbl;

    interface _IHtmLanchEvents
    {
        CONST_VTBL struct _IHtmLanchEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IHtmLanchEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IHtmLanchEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IHtmLanchEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IHtmLanchEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IHtmLanchEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IHtmLanchEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IHtmLanchEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IHtmLanchEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_HtmLanch;

#ifdef __cplusplus

class DECLSPEC_UUID("F062BC96-8EAF-4F38-A43A-90538792E0CE")
HtmLanch;
#endif
#endif /* __htmlancherLib_LIBRARY_DEFINED__ */

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


