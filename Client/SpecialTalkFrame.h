#pragma once
#include "gameframe.h"

class SpecialTalkFrame :public GameFrame
{

public:
	enum ESpecialTalkOptionType
	{
		ESTOT_None			= -1,		
		ESTOT_WalkWare					// 随身仓库，没办法本来不应该加到这，但随身仓库的要求是要用这个界面
	};

public:
	SpecialTalkFrame(void);
	~SpecialTalkFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID		 SetCaption( LPCTSTR szCaption );
	VOID		 SetContent( LPCTSTR szContent );
	VOID		 ClearOptions();
	VOID		 AddTalkOption( LPCTSTR szOption, DWORD param1, DWORD param2, ESpecialTalkOptionType optionType );

	VOID		 SetTarget(INT targetType, DWORD targetID) { m_targetType=targetType;m_targetID=targetID; }
	VOID Show();

private:
	BOOL		 EventHandler( tagGUIEvent* pEvent );

private:
	TSFPTrunk<SpecialTalkFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd						*m_pWnd;				//主界面
	GUIPatch					*m_pCaption;			//标题
	GUIStaticEx					*m_pStcCondition;		//对话内容窗口
	GUIListBox					*m_pListOptions;		//对话选项
	GUIButton					*m_pBtnCloseWnd;		//关闭界面按钮，上

	DWORD						m_curRow;
	INT							m_targetType;
	DWORD						m_targetID;

	struct tagOptAttr 
	{
		DWORD		id;
		DWORD		step;
		ESpecialTalkOptionType	type;
	};
	vector<tagOptAttr>			m_optAttrs;
};
