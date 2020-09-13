//-----------------------------------------------------------------------------
//!\file vuieditor.h
//!\author Lyp
//!
//!\date 2004-08-11
//! last 2008-01-23
//!
//!\brief vengine gui editor
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
// test
//-----------------------------------------------------------------------------
class CVUIEditor
{
public:
	BOOL Init(HINSTANCE hInst);
	VOID Mainloop();
	VOID Destroy();

private:
	TObjRef<D3DRender>			m_pRender;
	TObjRef<Util>				m_pUtil;
	TObjRef<WindowWrap>			m_pWindow;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Console>			m_pConsole;
	TObjRef<FastCode>			m_pFastCode;

	IGUIRenderNative			m_render;
	
	TObjRef<GUIEditor>			m_pEditor;
};

