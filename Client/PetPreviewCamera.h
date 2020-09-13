#pragma once
#include "gameframe.h"
#include "..\WorldDefine\pet_define.h"
#include "PetShowWindow.h"
//! \class   PetShowWindow
//! \brief   宠物模型预览工具
//! \author  JGuo
//! \date    2010-9


//------------仅供测试用------------
struct tagStartNPCTalkEvent;
//----------------------------------

struct tagNS_PetCamera;
// 消息用于petframe获取petID
struct tagPetCameraUI: public tagGameEvent
{
	DWORD  dwPetID; 
	tagPetCameraUI(LPCTSTR szEventName, GameFrame* pSenderFrame)
		:tagGameEvent(szEventName, pSenderFrame) ,dwPetID(GT_INVALID){}
};

enum ESetAxis
{
	E_Axis_X = 0,
	E_Axis_Y = 1,
	E_Axis_Z = 2,
};

// <! class 1:  PetCameraWindow;
// 重写 PetShowWindow 的 UpdateCamera()
class PetCameraWindow :
	public PetShowWindow
{
public:
	PetCameraWindow(void);
	virtual ~PetCameraWindow(void){};

	VOID SetCamera(ESetAxis ESAxis,float fValue);
protected:
	virtual void UpdateCamera();
private:
	float				m_fCamX;
	float				m_fCamY;
	float				m_fCamZ;
};

//<!  class 2: PetPreviewCamera 
//窗口界面
class PetPreviewCamera :
	public GameFrame
{
public:
	
	PetPreviewCamera(void);
	~PetPreviewCamera(void){};

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	virtual VOID Render3D();

	void Show( bool bShow )
	{
		if (P_VALID(m_pWnd))
			m_pWnd->SetInvisible( !bShow );
	}
private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD SetAxis(ESetAxis eAxis);
	DWORD RevPetCameraUI(tagPetCameraUI* pGameEvent);

private:
	TSFPTrunk<PetPreviewCamera>	m_Trunk;
	TObjRef<GUISystem>		m_pGUI;
	TObjRef<Util>			m_pUtil;
	TObjRef<GameFrameMgr>	m_pFrameMgr;
	TObjRef<NetCmdMgr>		m_pCmdMgr;

	GUIWnd*					m_pWnd;     
	PetCameraWindow*		m_pPteWindow;
	GUIEditBox*				m_pEBCameraX;
	GUIEditBox*				m_pEBCameraY;
	GUIEditBox*				m_pEBCameraZ;
	GUIButton*				m_pBtnExit;

	DWORD					m_dwPetID;
};

//<! class 3:  PetPreviewMgr
//Frame 全局管理器
class PetPreviewMgr
{
public:
	PetPreviewMgr(void);
	~PetPreviewMgr(void){};
	static PetPreviewMgr* Inst();
	void Init();
	void Destroy();
private:
	DWORD OnOpenFrame(tagNS_PetCamera* pEvt, DWORD);



	//------------仅供测试用------------
	DWORD Test_OnOpenFrame(tagStartNPCTalkEvent* pEvt);
	//----------------------------------
	
private:
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TSFPTrunk<PetPreviewMgr>	m_Trunk;
	PetPreviewCamera*			m_pPetCamera;
};

//一个H和CPP了定义了3个类，挺2的；
//不过继承类（class 1）和Mgr类（class 3）都挺简单，所以。。。
