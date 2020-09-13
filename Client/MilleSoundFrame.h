#pragma once
#include "gameframe.h"
#include "ChatSysDefine.h"
#include "EmotionFrame.h"  //表情

struct tagGetHeadpicByID;
struct tagRoleGetNameEvent;
struct tagNS_RoleChat;

class HttpPicEx;
class RichEditBoxForChatShow;

class MilleSoundFrame : public GameFrame
{
	
public:
	MilleSoundFrame(void);
	~MilleSoundFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	VOID PushCastText( LPCTSTR szText, DWORD dwRoleID, ESex eSex, float fTime = 30.0f );

	VOID Move( tagPoint& pos );

private:

	// 处理GUI消息
	DWORD EventHandler(tagGUIEvent* pEvent);
	
	//显示一条传音信息
	VOID ShowCastInfo();

	//得到角色名字
	DWORD OnRecvNetRoleGetName( tagRoleGetNameEvent* pMsg );				//处理 PlayerNameTab 加载玩家名字
	
	//得到角色URL
	DWORD OnRecvHeadpicByID( tagGetHeadpicByID* pGameEvent );				//处理 PlayerNameTab 加载玩家URL

	//接受聊天信息，不过只对传音信息感兴趣
	DWORD OnNS_RoleChat( tagNS_RoleChat* pMsg, DWORD dwParam );

	void RegisterPicForRichEdit(const TCHAR *szName,int index);             //注册表情图片

private:
	TSFPTrunk<MilleSoundFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	
	std::list<tagScroText>		m_lstCast;			//传音缓存队列
	std::map<DWORD,tagScroText>		m_mapCastNoName;	//保存的传音,没有得到角色名s
	

	GUIWnd*						m_pWnd;				//主窗口
	HttpPicEx*					m_pStcImg;			//图片
	GUIStatic*					m_pStcName;			//名字
	RichEditBoxForChatShow*		m_pStcExContent;	//传音内容  //更改为支持表情类型控件
	EmotionFrame*               m_pEmotionFrame;    //表情

	DWORD						m_dwStartTime;		//当前传音信息开始的时间
	
	DWORD						m_dwMsgPersistTime;	//每个传音最少停留时间
	DWORD						m_dwWndCloseTime;	//如果没有传音多长时间关闭

	BOOL						m_bDisplaying;		//现在正在显示内容
};
