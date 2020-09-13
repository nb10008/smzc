#pragma once
#include "player.h"
#include "AvatarEquipNode.h"

//class Role;
class FSM_RP;

#define VR_HEIGHT_OFFSET		(-15.0f)
#define VR_X_Z_OFFSET			(60.0f)

enum EPLayActionType
{
	EPAT_NULL,			//无动作
	EPAT_MOVE,			//移动动作
	EPAT_FIGHT,			//作战动作
	EPAT_STAND,		//站立动作
	EPAT_CALL,			//召唤动作
	EPAT_DISPPEAR,	//消失动作
};

enum EPLaySoundType
{
	EPST_NULL = -1, 			//---------------------无声音

	//按时间间隔播放
	EPST_Fighting			=	0, 		//攻击
	EPST_Winning			=	1, 		//胜利
	EPST_Leisure				=	2, 		//休闲
	EPST_BeAttack			= 3,		//被攻击
	EPST_TimeEnd			=	3,		//------时间间隔播放结尾
	//即时播放
	EPST_Disppear			=	4,		//消失
	EPST_Calling				=	5, 		//召唤
	EPST_TypeNum			= 6,		//------类型数目

};

struct tagVirtualRoleEvent : public tagGameEvent
{
	EPLayActionType  ePlayAct;
	tagVirtualRoleEvent()
	:tagGameEvent(_T("tagVirtualRoleEvent"),NULL)
	,ePlayAct(EPAT_NULL)
	{}
	tagVirtualRoleEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		,ePlayAct(EPAT_NULL)
	{}
};

//击杀消息
struct tagVRSkillEvent : public tagGameEvent
{
	tagVRSkillEvent():tagGameEvent(_T("tagVRSkillEvent"),NULL)
	{}
	tagVRSkillEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
	: tagGameEvent(szEventName,pSenderFrame)
	{}
};


/** \class VirtualRole
\brief 虚拟角色
用来制作模拟人物的特效等
*/
class VirtualRole :	public Player
{
private:
	

public:
	VirtualRole(void);
	virtual ~VirtualRole(void);

	//--Role-------------------------------------------------------------------
	virtual void Update();
		
	virtual void InitFSM(DWORD dwRoleID);
	
	BOOL IsInitFSM() const { return m_bIsInitFSM;} 

	void OnGotoMap(const Vector3& pos,const Vector3 faceTo);

	void OnGameEvent(tagGameEvent* pGameEvent);
	void OnNetCmd(tagNetCmd* pNetCmd);

	virtual void Hide( const bool bHide );

	//同步虚拟角色和人物作战动作
	VOID SynCombatAction(tstring strActionName);

	//设置或许虚拟角色ID
	VOID SetHolyManID( DWORD dwID) { m_dwVRID = dwID; }
	DWORD GetHolyManID() { return m_dwVRID; }

	VOID	SetState(EPLayActionType eType, BOOL bForce = FALSE);

	/*	\供虚拟角色调用 ---用于虚拟角色
	*/
	bool		LoadVirtualAvatar( tstring strResName);
	VOID   LoadVirtualEffect( tstring strName );
	//召唤时初始化元神模型
	VOID CallHolyManInit();
private:
	//--设置朝向
	BOOL RotateVRole( Vector3 srcPos,  Vector3 dstPos );
	//播放动作
	VOID  PlayAction();
	//同步位置
	VOID SynchPos();
	//刷新声效控制
	VOID UpdateSound();
	//设置朝向
	VOID FaceToTarget( Player* pSelf,Role* pTarget,BOOL bMoveable, BOOL bAniBlend );
	//获得名称
	const TCHAR* GetVRMoveTrackName( float speed );
	//播放声效
	VOID PlayVRSound( EPLaySoundType eType );
	//按性别和声效数目选择随机播放声效  
	//  nMaleRandNum,   nFemaleRandNum 由策划提供的资源数目决定
	VOID PlayRandSound( INT nSound, INT nMaleRandNum, INT nFemaleRandNum );
	//显示模型特效
	VOID ShowVREffect();
	//不显示模型特效
	VOID CloseVREffect();
	//设置本地玩家元神声效
	VOID SetLocalSoundType( EPLaySoundType eType );
private:
	TObjRef<AudioSys>		m_pAudioSys;
	TObjRef<Util>				m_pUtil;

	DWORD					m_dwVRID;				//虚拟角色模型ID(元神ID)

	DWORD					m_dwRoleID;				//所属玩家id

	BOOL						m_bIsInitFSM;			//fsm时都初始化完
	
	EPLayActionType		m_eActionType;		//动作播放类型

	EPLaySoundType		m_eSoundType;		//声效播放类型

	DWORD					m_dwTimer/*[EPST_TypeNum]*/; //声效时间

	DWORD					m_dwEscapeTime;

	tstring						m_strCombatAction;

	tstring						m_strActionName;

	std::map<tstring, tstring>		m_mapVREffect;	//虚拟角色特效，<特效绑定骨骼, 特效名称>

	DECL_RTTI(VirtualRole);
};
