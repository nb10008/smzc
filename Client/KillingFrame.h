#pragma once
#include "gameframe.h"

struct tagNS_SendRoleKillCount;

const int MAX_KILLING_NUM = 5;				// 时间数字位数
const float BombTime = 0.2f;				// 弹出爆炸效果时间

enum EKillingTpye
{
	EKT_Monster,	//怪物
	EKT_Player		//角色
};

enum EShowStyleTpye
{
	KSST_None,	
	KSST_Enter,		// 刚刚开始
	KSST_Explode,	// 字体爆炸
	KSST_Remain		// 保持阶段
};
struct tagKillingCounter
{
	int nMonster;
	int nPlayer;
	tagKillingCounter():nMonster(0),nPlayer(0){}

	void clear()
	{
		nMonster = 0;
		nPlayer = 0;
	}
};

struct tagRenderPosFlag
{
	int nPos;	// 位置编号 1-10	
	EKillingTpye eType;	// 数字类型	
	int nNum;	// 号码编号 0-9  kill 100  ko 200
	bool bNeedRende;// 是否需要渲染
	bool bNeedBomb;	// 是否需要爆字
	int  nBombState;// 爆炸状态
	tagRenderPosFlag():nPos(0),nNum(0),bNeedRende(true),bNeedBomb(false),nBombState(0){}
	

	/*
	编号位置对应空间顺序
	[5] [4] [3][2][1] [0]--KILL
	[11][10][9][8][7] [6]--KO
	*/
};



class KillingFrame :
	public GameFrame
{
public:
	typedef  std::map<int,tagRenderPosFlag> KillNumMap;

	KillingFrame(void);
	virtual ~KillingFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	virtual VOID Render3D();

	VOID DrawNumPic(RECT rc, ResTexture* pRes);
	VOID CalculateNumPos(EKillingTpye eType);
	VOID InsertToNumMap(EKillingTpye eType,int num,int pos,bool needRender = true,bool Bomb = true);
	VOID InsertToNumMap(tagRenderPosFlag flag);
	VOID EraseFromNumMap(int pos);
	bool LoadPic(int ch,const TCHAR* szPicFileName,IFS *pFS);

	// 预加载一下所有特效资源
	VOID LoadAllRes();
	VOID LoadRes(const TCHAR* szPicFileName);
	VOID PlayScreenEffect();

private:
	DWORD OnNS_SendRoleKillCount(tagNS_SendRoleKillCount* pNetCmd, DWORD dwParam);
	DWORD OnOpenCloseKillingShow(tagGameEvent *pEvent);

	VOID NumberGenerator();
	VOID UpdataLocalPlayerUIeffect();
private:
	std::map<int, ResTexture*>	m_mapResPic;	// 资源图 0-9图片 + 100 [kill] 200[ko] 
	KillNumMap					m_mapNumInfo;
	std::map<int,tagPoint>		m_mapPosInfo;

	std::vector<Vector3>		m_vecPosforOneNum;
	std::vector<Vector3>		m_vecPosforTwoNum;
	std::vector<Vector3>		m_vecPosforThreeNum;
	std::vector<Vector3>		m_vecPosforFourNum;
	std::vector<Vector3>		m_vecPosforFiveNum;
	EShowStyleTpye				m_eState;
	int							m_nTexMark;
	int							m_nProgressMark;

	ResTexture*					m_texProgress;
	tagPoint					m_ProgressPoints;
	// tools
	TSFPTrunk<KillingFrame>		m_Trunk;
	TObjRef<GUISystem>          m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	tagKillingCounter			m_strKillingNum;
	

	// GUI Controls
	GUIWnd*                     m_pWnd;

	GUIStatic*					m_pNumMonsters[MAX_KILLING_NUM];
	GUIStatic*					m_pNumPlayers[MAX_KILLING_NUM];
	GUIStatic*					m_pProgressBar;

	GUIStatic*					m_pKills;
	GUIStatic*					m_pKO;

	// ui特效
	DWORD						m_dwTime_HP;
	DWORD						m_dwTime_MP;
	DWORD						m_dwTime_Anger;

	bool						m_bMarkHP;
	bool						m_bMarkMP;
	bool						m_bMarkAnger;

	bool						m_bMarkHPloop;
	bool						m_bMarkMPloop;
	bool						m_bMarkAngerloop;

	DWORD						m_dwFadeTime;

	DWORD						m_dwMonsterConter;
	DWORD						m_dwPlayConter;

	// 死亡黑屏
	ResTexture*					m_pTex;
	RECT						m_rectSrc;
	RECT						m_rectDest;
	INT							m_nAlpha;
};
