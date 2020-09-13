// HtmLanch.h : Declaration of the CHtmLanch
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "htmlancher.h"
#include "_IHtmLanchEvents_CP.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// CHtmLanch
class ATL_NO_VTABLE CHtmLanch :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CStockPropImpl<CHtmLanch, IHtmLanch>,
	public IPersistStreamInitImpl<CHtmLanch>,
	public IOleControlImpl<CHtmLanch>,
	public IOleObjectImpl<CHtmLanch>,
	public IOleInPlaceActiveObjectImpl<CHtmLanch>,
	public IViewObjectExImpl<CHtmLanch>,
	public IOleInPlaceObjectWindowlessImpl<CHtmLanch>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CHtmLanch>,
	public CProxy_IHtmLanchEvents<CHtmLanch>,
	public IObjectWithSiteImpl<CHtmLanch>,
	public IServiceProviderImpl<CHtmLanch>,
	public IPersistStorageImpl<CHtmLanch>,
	public ISpecifyPropertyPagesImpl<CHtmLanch>,
	public IQuickActivateImpl<CHtmLanch>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CHtmLanch>,
#endif
	public IProvideClassInfo2Impl<&CLSID_HtmLanch, &__uuidof(_IHtmLanchEvents), &LIBID_htmlancherLib>,
	public IPropertyNotifySinkCP<CHtmLanch>,
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	public IObjectSafetyImpl<CHtmLanch, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
#endif
	public CComCoClass<CHtmLanch, &CLSID_HtmLanch>,
	public CComControl<CHtmLanch>,
	public IObjectSafetyImpl<CHtmLanch,INTERFACESAFE_FOR_UNTRUSTED_CALLER>

{
public:


	CHtmLanch()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_HTMLANCH)


BEGIN_COM_MAP(CHtmLanch)
	COM_INTERFACE_ENTRY(IHtmLanch)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IServiceProvider)
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
#endif
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_PROP_MAP(CHtmLanch)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	PROP_ENTRY("Appearance", DISPID_APPEARANCE, CLSID_NULL)
	PROP_ENTRY("AutoSize", DISPID_AUTOSIZE, CLSID_NULL)
#ifndef _WIN32_WCE
	PROP_ENTRY("BackColor", DISPID_BACKCOLOR, CLSID_StockColorPage)
#endif
	PROP_ENTRY("BackStyle", DISPID_BACKSTYLE, CLSID_NULL)
#ifndef _WIN32_WCE
	PROP_ENTRY("BorderColor", DISPID_BORDERCOLOR, CLSID_StockColorPage)
#endif
	PROP_ENTRY("BorderStyle", DISPID_BORDERSTYLE, CLSID_NULL)
	PROP_ENTRY("BorderVisible", DISPID_BORDERVISIBLE, CLSID_NULL)
	PROP_ENTRY("BorderWidth", DISPID_BORDERWIDTH, CLSID_NULL)
	PROP_ENTRY("Caption", DISPID_CAPTION, CLSID_NULL)
	PROP_ENTRY("DrawMode", DISPID_DRAWMODE, CLSID_NULL)
	PROP_ENTRY("DrawStyle", DISPID_DRAWSTYLE, CLSID_NULL)
	PROP_ENTRY("DrawWidth", DISPID_DRAWWIDTH, CLSID_NULL)
	PROP_ENTRY("Enabled", DISPID_ENABLED, CLSID_NULL)
#ifndef _WIN32_WCE
	PROP_ENTRY("FillColor", DISPID_FILLCOLOR, CLSID_StockColorPage)
#endif
	PROP_ENTRY("FillStyle", DISPID_FILLSTYLE, CLSID_NULL)
#ifndef _WIN32_WCE
	PROP_ENTRY("Font", DISPID_FONT, CLSID_StockFontPage)
#endif
#ifndef _WIN32_WCE
	PROP_ENTRY("ForeColor", DISPID_FORECOLOR, CLSID_StockColorPage)
#endif
#ifndef _WIN32_WCE
	PROP_ENTRY("MouseIcon", DISPID_MOUSEICON, CLSID_StockPicturePage)
#endif
	PROP_ENTRY("MousePointer", DISPID_MOUSEPOINTER, CLSID_NULL)
#ifndef _WIN32_WCE
	PROP_ENTRY("Picture", DISPID_PICTURE, CLSID_StockPicturePage)
#endif
	PROP_ENTRY("ReadyState", DISPID_READYSTATE, CLSID_NULL)
	PROP_ENTRY("TabStop", DISPID_TABSTOP, CLSID_NULL)
	PROP_ENTRY("Text", DISPID_TEXT, CLSID_NULL)
	PROP_ENTRY("Valid", DISPID_VALID, CLSID_NULL)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CHtmLanch)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(__uuidof(_IHtmLanchEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CHtmLanch)
	CHAIN_MSG_MAP(CComControl<CHtmLanch>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] =
		{
			&IID_IHtmLanch,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IHtmLanch
public:
		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
		RECT& rc = *(RECT*)di.prcBounds;
		// Set Clip region to the rectangle specified by di.prcBounds
		HRGN hRgnOld = NULL;
		if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
			hRgnOld = NULL;
		bool bSelectOldRgn = false;

		HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

		if (hRgnNew != NULL)
		{
			bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
		}

		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("ATL 8.0 : HtmLanch");
#ifndef _WIN32_WCE
		TextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			pszText,
			lstrlen(pszText));
#else
		ExtTextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			ETO_OPAQUE,
			NULL,
			pszText,
			ATL::lstrlen(pszText),
			NULL);
#endif

		if (bSelectOldRgn)
			SelectClipRgn(di.hdcDraw, hRgnOld);

		return S_OK;
	}

	void OnAutoSizeChanged()
	{
		ATLTRACE(_T("OnAutoSizeChanged\n"));
	}
	SHORT m_nAppearance;
	void OnAppearanceChanged()
	{
		ATLTRACE(_T("OnAppearanceChanged\n"));
	}
	OLE_COLOR m_clrBackColor;
	void OnBackColorChanged()
	{
		ATLTRACE(_T("OnBackColorChanged\n"));
	}
	LONG m_nBackStyle;
	void OnBackStyleChanged()
	{
		ATLTRACE(_T("OnBackStyleChanged\n"));
	}
	OLE_COLOR m_clrBorderColor;
	void OnBorderColorChanged()
	{
		ATLTRACE(_T("OnBorderColorChanged\n"));
	}
	LONG m_nBorderStyle;
	void OnBorderStyleChanged()
	{
		ATLTRACE(_T("OnBorderStyleChanged\n"));
	}
	BOOL m_bBorderVisible;
	void OnBorderVisibleChanged()
	{
		ATLTRACE(_T("OnBorderVisibleChanged\n"));
	}
	LONG m_nBorderWidth;
	void OnBorderWidthChanged()
	{
		ATLTRACE(_T("OnBorderWidthChanged\n"));
	}
	CComBSTR m_bstrCaption;
	void OnCaptionChanged()
	{
		ATLTRACE(_T("OnCaptionChanged\n"));
	}
	LONG m_nDrawMode;
	void OnDrawModeChanged()
	{
		ATLTRACE(_T("OnDrawModeChanged\n"));
	}
	LONG m_nDrawStyle;
	void OnDrawStyleChanged()
	{
		ATLTRACE(_T("OnDrawStyleChanged\n"));
	}
	LONG m_nDrawWidth;
	void OnDrawWidthChanged()
	{
		ATLTRACE(_T("OnDrawWidthChanged\n"));
	}
	BOOL m_bEnabled;
	void OnEnabledChanged()
	{
		ATLTRACE(_T("OnEnabledChanged\n"));
	}
	OLE_COLOR m_clrFillColor;
	void OnFillColorChanged()
	{
		ATLTRACE(_T("OnFillColorChanged\n"));
	}
	LONG m_nFillStyle;
	void OnFillStyleChanged()
	{
		ATLTRACE(_T("OnFillStyleChanged\n"));
	}
	CComPtr<IFontDisp> m_pFont;
	void OnFontChanged()
	{
		ATLTRACE(_T("OnFontChanged\n"));
	}
	OLE_COLOR m_clrForeColor;
	void OnForeColorChanged()
	{
		ATLTRACE(_T("OnForeColorChanged\n"));
	}
	CComPtr<IPictureDisp> m_pMouseIcon;
	void OnMouseIconChanged()
	{
		ATLTRACE(_T("OnMouseIconChanged\n"));
	}
	LONG m_nMousePointer;
	void OnMousePointerChanged()
	{
		ATLTRACE(_T("OnMousePointerChanged\n"));
	}
	CComPtr<IPictureDisp> m_pPicture;
	void OnPictureChanged()
	{
		ATLTRACE(_T("OnPictureChanged\n"));
	}
	LONG m_nReadyState;
	void OnReadyStateChanged()
	{
		ATLTRACE(_T("OnReadyStateChanged\n"));
	}
	BOOL m_bTabStop;
	void OnTabStopChanged()
	{
		ATLTRACE(_T("OnTabStopChanged\n"));
	}
	CComBSTR m_bstrText;
	void OnTextChanged()
	{
		ATLTRACE(_T("OnTextChanged\n"));
	}
	BOOL m_bValid;
	void OnValidChanged()
	{
		ATLTRACE(_T("OnValidChanged\n"));
	}
	STDMETHOD(_InternalQueryService)(REFGUID guidService, REFIID riid, void** ppvObject)
	{
		return E_NOTIMPL;
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
	STDMETHOD(Lanch)(BSTR sApp, BSTR sParam);
	STDMETHOD(GetInterfaceSafetyOptions)(REFIID riid, DWORD *pdwSupportedOptions, DWORD *pdwEnabledOptions)
	{
		*pdwSupportedOptions = 1;
		*pdwEnabledOptions   = 2;
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(HtmLanch), CHtmLanch)
