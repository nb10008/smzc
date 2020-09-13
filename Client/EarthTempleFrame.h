#pragma once
#include "gameframe.h"
#include "AppliedTree.h"
/** \class EarthTempleFrame

\brief ：土地庙

*/
struct tagSpecItemEvent;
struct tagNS_EarthTemple;
struct tagNS_Skill;

class EarthTempleFrame :
	public GameFrame
{
private:
	struct tagZoneMapShow
	{
		DWORD 				dwMapID;
		tstring             strShowName;
		tagZoneMapShow() :  dwMapID( GT_INVALID ) {}
	};
	struct tagMarkIndex
	{
		int nType;
		int nMask;
		tagMarkIndex() : nType( 0 ), nMask( 0 ){}
		tagMarkIndex( const int type, const int mask ) : nType( type ), nMask( mask ) {}
	};
	struct tagEarthTempleUnit
	{
		DWORD		dwMapID;
		Vector3		position;
		tstring     name;
		tagEarthTempleUnit() : dwMapID(0),name(_T("")){ position = Vector3(0,0,0);}
	};
public:
	EarthTempleFrame(void);
	~EarthTempleFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

	BOOL IsInSendingState();

private:
	BOOL OnWnd_Temple(tagGUIEvent* pEvent);
	VOID InitMapInfo();
	VOID FillMapListBox();
	VOID ReBuidMarksTree( const DWORD dwMapID );
	DWORD OnOpenEarthTemple(tagSpecItemEvent* pEvent);
	bool CanGoto();
	VOID SendMsg();
	VOID StopSendMsg();
	bool PlayerStateCheck();
	VOID SortUnits();
	DWORD OnNS_EarthTemple(tagNS_EarthTemple* pMsg,DWORD pParam);

	DWORD OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam);

private:

	TSFPTrunk<EarthTempleFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	GUIWnd*							m_pWnd;			//主窗口
	GUIButton*                      m_pBn_Ok_Set;					//确定
	GUIButton*                      m_pBn_Cancel_Set;				//取消
	GUIButton*                      m_pBn_Close_Set;				//关闭

	GUIComboBox*					m_pCb_Earth_Box;				//组合框

	AppliedTree*					m_pTree;							

	vector<tagZoneMapShow>          m_vecEarthMapShow;				// 区域地图列表

	DWORD 							dwSelectMapID;
	INT64							CurrItemID;		//当前土地庙道具的64ID
	Vector3							m_PosVector3;		//当前选择的地点的世界坐标

	map<DWORD, tagMarkIndex>        m_mapMarkIndex;       // KEY:ID in AppliedTree

	tagMarkIndex					m_strTempMark;		//为了屏蔽牢狱地点设计

	bool							m_bIsSpelling;		//是否起手中

	DWORD							m_dwTimeCounter;

	GUIPushButton*					m_pBnFrist;			//土地庙第一个缓存
	GUIPushButton*					m_pBnSecond;		//土地庙第2个缓存

	tagEarthTempleUnit				m_strFristUnit;		//土地庙第一个缓存对应信息
	tagEarthTempleUnit				m_strSecondUnit;	//土地庙第2个缓存对应信息

	tstring							m_szTempName;		//当前传送目的地的名字

	BOOL							m_bDirectOpen;		//是否由技能直接打开的界面
	
};
