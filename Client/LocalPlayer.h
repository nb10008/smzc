#pragma once
#include "player.h"
#include "../WorldDefine/time.h"

class FSM_LP;
struct tagNS_GetRoleInitState_Att;

/** \class LocalPlayer
	\brief 本地玩家角色
*/
class LocalPlayer : public Player
{
public:
	LocalPlayer(void);
	virtual ~LocalPlayer(void);

	//--Role
	virtual void InitFSM();
	virtual void Update();
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pGameEvent);
	virtual bool IsDead();

	//--Player
	virtual bool IsReadyState();
	virtual bool IsStandState();
	virtual bool IsStyleActionState();
	virtual bool IsIdleState();
	virtual EActionState GetActionState();
	virtual void ReplayLowerAni();
	/** \返回角色是否处于追踪状态
	*/
	virtual bool IsMoveState();

	//--角色属性相关
	/** \读取和写入玩家投点
	*/
	INT GetAttPointAdd(ERoleAttribute eType){return m_rolepointadd[MTransERAAttA2Index(eType)];}
	VOID SetAttPointAdd(ERoleAttribute eType, INT nValue);
	/** \读取玩家原始一级属性
	*/
	INT GetOriginAtt(ERoleAttribute eType);
	/** \处理本地玩家属性初始化
	*/
	VOID InitAttribute(tagNS_GetRoleInitState_Att* pCmd);
	/** \读取和写入玩家当前经验
	*/
	INT64 GetRoleLevelExp(){return m_rolecurlevelexp;}
	VOID SetRoleLevelExp(INT64 nValue, INT64 nInc = 0, INT nFairyExp = 0, INT nFairyLvCha = 0,bool bShare = false );

	virtual void SetPlayerClass(EClassType eClass);

	//--
	/** \返回角色是否处于战斗状态
	*/
	bool IsCombatState();
	/** \返回角色是否处于使用技能状态
	*/
	bool IsSkillState();
	/** \返回角色是否处于游泳状态
	*/
	bool IsSwimState();
	/** \返回角色是否处于水上行走状态
	*/
	bool IsPursueState();
	/** \返回角色是否处于移动状态
	*/
	bool IsOnWaterState();
	/**  普通攻击技能状态
	*/
	bool IsNormalSkillState();
	/** \设置角色自身状态
	*/
	void SetRoleStateEx(DWORD dwValue);
	/** \设置角色自身状态
	*/
	void SetRoleStateEx(ERoleStateEx eType, bool bFlag);
	/** \获取角色自身状态
	*/
	bool GetRoleStateEx(ERoleStateEx eType){return (m_roleStateEx&eType) != 0;}
	/** \切换地图或传送后，设置坐标、朝向等
	*/
	void OnGotoMap(const Vector3& pos,const Vector3 faceTo);

	/** \玩家的个人设置
	*/
	const tagDWPlayerPersonalSet& GetPersonalSet() const { return m_personalSet; }
	void SetPersonalSet( const tagDWPlayerPersonalSet& pps ) { m_personalSet = pps; }

	/** \钓鱼,忽略一次悠闲动作
	*/
	void DontPlayEaseSkeleton();

	/** \钓鱼,强制转换到idle状态, 为钓鱼系统设计
	*/
	void ChangeUpperToIdle();

	/** \当前状态是否为idle
	*/
	bool IsUpperIdleState();

	/** 
		\ 得到正在使用的物品的 typeid，如果当前不是使用物品状态返回0
	*/
	DWORD GetUsingItemTypeID();

	void SetMiraclePrcNum(INT16 nNum);
	INT16 GetMiraclePrcNum() const { return m_n16MiraclePrcNum; }

	void SetSoarSkillLearnTimes(INT16 nNum);
	INT16 GetSoarSkillLearnTimes() const { return m_nSoarSkillLearnTimesToday; }

	/*	\特殊特效，目前仅为飞升特效
	*/
	void UpdatePlayerEffect();

	bool IsHookState();

	void SetHookState(bool state);

	void LoadEquipEffect(const tagAvatarEquip& ep);

	//  [8/4/2010 zhangsu] 跨场景寻路
	void SetRePathFinding(bool bReFind,DWORD npcID = 0 ,tstring strMapName = _T(""));
	void KeepMoving();

	bool IsCrazyState();

	void SetVipDays(tagDWORDTime nNum);
	tagDWORDTime GetVipDays() const { return m_nVipDays; }

	void SetVipDaysCanLast(const INT& day){m_nVipMaxDay = day;}
	INT GetVipDaysCanLast() const { return m_nVipMaxDay;}
	VOID SetLPActiveDegree(INT nDegree){ m_nLPActiveDegree = nDegree;}
	INT	GetLPActiveDegree(){ return m_nLPActiveDegree;}
protected:
	FSM_LP*					m_pFSM;
	INT						m_rolepointadd[X_ERA_ATTA_NUM];
	INT64						m_rolecurlevelexp;
	DWORD					m_roleStateEx;
	INT16                   m_n16MiraclePrcNum;
	INT16					m_nSoarSkillLearnTimesToday;

	bool					m_bHooking;

	tagDWPlayerPersonalSet  m_personalSet;

	bool					bNeedRePathFinding;
	DWORD					dwReFindNPCid;
	tstring					m_strMapName;
	tagDWORDTime			m_nVipDays;		// vip开始的时间
	INT						m_nVipMaxDay;	// vip能持续多少天
	INT						m_nLPActiveDegree; //本地玩家家族活跃度

	Vector3							m_v3OldPos;

	DECL_RTTI(LocalPlayer);

};


