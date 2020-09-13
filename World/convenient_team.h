/********************************************************************
	created:	2010/09/25
	created:	25:9:2010   17:10
	filename: 	e:\F-Project\src\World\convenient_team.h
	file path:	e:\F-Project\src\World
	file base:	convenient_team
	file ext:	h
	author:		Jason
	
	purpose:	便捷组队
*********************************************************************/


#ifndef convenient_team_h__
#define convenient_team_h__

class Map;
class Team;
class Unit;
struct tagNS_GetEasyTeamInfo;
struct tagNS_GetSinglePlayers;
struct tagNS_GetPersonalInfo;
struct tagNC_PutOutInfo;
/**
 * \ingroup World
 *
 *
 *
 * \version 1.0
 * 实现杀怪的便捷组队信息提示功能
 * \version 1.1
 * 活动副本的便捷组队提示
 *
 * \date 2010-09-25
 * \date 2010-10-10 加入副本以及活动相关功能
 *
 * \author jpma
 */
class ConvenientTeam
{
	BOOL	m_bOpen;
	BOOL	m_bSendTeamChange;
	ConvenientTeam();
	~ConvenientTeam();

	INT		m_nPlyNumLimit;
	INT		m_nPckLimit;

public:
	struct tagCTInfo
	{
		BYTE byTeamType;
		BYTE byScene;
		BYTE byTeamTypeDetail;
		BYTE byTeamNum;
		BYTE byMinLevel;
		BYTE byMaxLevel;
		DWORD dwRoleID; // 申请者id，或者队长（至少是未来的）
		tstring strContent;
	};// 36bytes
private:

	// 地图CRCID与场景id的映射，在初始化时完成数据填入；运行时只是读取，不需要同步安全策略。
	typedef map<DWORD,vector<INT> >	TMapScene;
	TMapScene	m_MapIDScene;

	// 玩家id与组队信息，组队int是负数时，表示是求租的，要做一下0-操作，并用m_QzCTInfos；否则用m_ZdCTInfos
	// 需要线程安全
	typedef map<DWORD,INT>	TPlayerInfoID;
	TPlayerInfoID	m_PlayerInfoID;

	// 与group_scene.xml相关的，《SceneID,组队信息id/包括求组的》
	// 需要线程安全
	typedef map<INT,vector<INT> >		TSceneCTInfos;
	TSceneCTInfos	m_SceneCTInfos;

	typedef map<INT,tagCTInfo*> TCTInfo;
	// 维护组队信息的容器
	TCTInfo m_ZdCTInfos;
	// 维护求组信息的容器
	TCTInfo m_QzCTInfos;

	tagCTInfo * GetCTInfo(INT ndx); // 返回组队信息，
	INT	InsertCTInfo(tagCTInfo * pInfo,bool bIsZudui);	// 加入组队信息，
	VOID	DeleteCTInfo(INT CTInfoNdx,INT nSceneNdx = GT_INVALID);

	BOOL GetZuduiInfo(tagNS_GetEasyTeamInfo*& pMsg,Unit * pUnit,ServerMemPool* pMemPool,set<DWORD> & team_leaders);

public:

	// 必须在系统初始化时调用，以免在系统运行时出现崩溃；该单件方法并不是线程安全的
	static ConvenientTeam& GetInstance();
	// 必须在系统被终止前调用；该方法并不是线程安全的
	static VOID Destroy();

	VOID Open(BOOL bOpen){m_bOpen = bOpen;}
	VOID OpenTeamNotice(BOOL bOpen) {m_bSendTeamChange = bOpen;}

	// 杀怪，副本，活动
	enum ECTType{ ECT_Creature = 0, ECT_Instance, ECT_Activity };

	
	/**	【作废】客户端索取时再发
	 * 通知玩家附近的（九宫格内）的队伍信息，当然是在附近有怪的情况下，没有队伍信息不会发
	 * 该方法只是算法，并不缓存数据，因此是线程安全的
	 * \param pMap 玩家地图
	 * \param pNewMap 玩家要去的新地图
	 * \param nOldPos 原格子信息，如果玩家是队长，则通知原九宫格内的玩家，队伍消失；
	 * \param nNewVisTileNdx 新格子信息，发送新格子内队伍信息给该玩家
	 */
	VOID OnRoleVisTileChange( Unit * pUnit, Map * pMap,Map * pNewMap,INT nOldPos, INT nNewVisTileNdx );

	enum ETeamEvent{ ETE_New = 0,ETE_Del,ETE_MemAdd,ETE_MemRemove,ETE_ChangeLeader };
	// 当小队人数发生变化时，调用
	VOID OnTeamChanged( ECTType eChangeType, INT nEventType,Team * pTeam );

	// 当客户端请求时调用，返回需要发送的消息体
	BOOL GetEasyTeamInfo(tagNS_GetEasyTeamInfo*& pMsg,Unit * pUnit,ServerMemPool* pMemPool = NULL);

	// 得到散户，没有组队的玩家列表
	BOOL GetFreePlayers(tagNS_GetSinglePlayers*& pMsg,Unit * pUnit,ServerMemPool* pMemPool = NULL);

	BOOL IsOpen() CONST {return m_bOpen;}

	// 初始化方法，从配置文件读取组队相关信息
	VOID Init(VarContainer * pVar,LPCTSTR field)	;

	// 获取求组的玩家
	BOOL GetPersonalInfo(tagNS_GetPersonalInfo *& pMsg, Unit * ply,ServerMemPool* pMemPool = NULL);
	// 玩家发布组队/求组信息
	DWORD IssueCTInfo(tagNC_PutOutInfo * pMsg, Unit * ply);

	// 删除组队或者个人求组信息
	VOID DeleteCTInfoOfPlayer(Unit * ply,BOOL bTeamOrNot = TRUE);

	// 限制便捷组队申请人数，包括求组的
	VOID LimitTeamInfoNum(INT num);
	VOID LimitPckSize(INT num) {m_nPckLimit = num;}
};



#endif // convenient_team_h__


