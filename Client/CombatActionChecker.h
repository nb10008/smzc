#pragma once
#include "..\WorldDefine\skill_define.h"

class Player;
class Role;
class LocalPlayer;

struct tagSkillData;

/** \class CombatActionChecker
	\brief 战斗行为检查
*/
class CombatActionChecker
{
public:
	enum EErrCode
	{
		EEC_CannotAttack=1,
		EEC_CannotAttackSafeguard=2,
		EEC_PVP=3,
		EEC_Passive=4,
		EEC_InvalidTarget=5,
		EEC_NoTarget=6,
		EEC_TargetNotExist=7,
		EEC_TargetIsDead=8,
		EEC_TargetBlocked=9,
		EEC_SelfStateLimit=10,
		EEC_HPNotEnough=11,
		EEC_MPNotEnough=12,
		EEC_RageNotEnough=13,//怒气不足
		EEC_WeaponLimit=14,
		EEC_SwimLimit=15,//游泳状态无法进行该操作
		EEC_NoSkill=16,
		EEC_PrisonTransLimit=17,//在牢狱中不能使用传送类道具
		EEC_TooFar=18,//距离太远
		EEC_SkillBuffLimit=19,//技能施放条件不足
        EEC_CanNotUseItem=20,//不能使用物品
		EEC_CabNotGoToPos=21,//不能到达指定地点（手动输入坐标寻路功能）
		EEC_NoHunjing=22,//没有佩戴魂晶，无法释放技能
	};
public:
	CombatActionChecker(void);
	~CombatActionChecker(void);

	void Init();
	void Destroy();

	bool IfCanMove(bool bShowErrMsg);
	bool IfCanTalkNPC(bool bShowErrMsg);
	bool IfCanPickGroundItem(bool bShowErrMsg);
	bool IfCanUseSkill(DWORD skillID,DWORD targetID,bool bShowErrMsg);
	bool IfCanUseItem(DWORD itemID,DWORD targetID,bool bShowErrMsg);
	bool IfCanAttack(DWORD targetID);
	bool IfCanPlayStyleAction(bool bMutual, DWORD dwFriendVal, DWORD targetID,bool bShowErrMsg);

	/** 目标射线检测
		\return 检测通过返回true
	*/
	bool RayCollideTarget(Role* pTarget);

	/** 检查目标是否有效
	*/
	bool IfTargetIsValid(DWORD targetID,DWORD dwTargetLimit,BOOL bHostile,BOOL bFriendly,BOOL bIndependent);

	static CombatActionChecker* Inst();
private:
	/** 是否在PVP区域
	*/
	bool IsInPVPArea(Player* pTarget);
	/** 是否在安全区域
	*/
	bool IsInSafeArea(Player * pTarget);
	/** 是否在战场区域
	*/
	bool IsInArena(Player * pTarget);
	/** 计算目标类型
	*/
	DWORD GetTargetType(Role* pTarget);
	/** 判断目标是否是敌方
	*/
	bool IsEnemy(Role* pTarget,DWORD targetType);
	/** 判断目标是否是友方
	*/
	bool IsFriend(Role* pTarget,DWORD targetType);
	/** 判断目标是否是中立方
	*/
	bool IsIndependent(Role* pTarget,DWORD targetType);
	/** 计算角色状态标志位
	*/
	DWORD GetRoleStateFlag(Role* pRole);

	/** 得到使用一次技能，应该话费多少东西
	*/
	INT GetCostByType( LocalPlayer* pSelf, const tagSkillData* pSkillData, ESkillCostType eSCT );

	INT GetCostByType( LocalPlayer* pSelf, const tagSkillProtoClient* pSkillProto, ESkillCostType eSCT );

	/** 显示武器错误码
	*/
	VOID WeaponLimitMessage( INT limitType );

	/** 显示目标类型错误码
	*/
	VOID TargetTypeLimitMessage( DWORD targetType );

	/** 显示目标状态错误码
	*/
	VOID TargetStateLimitMessage( DWORD dwTargetStateFlag );

	/**	显示玩家自身状态错误码
	*/
	VOID PlayerStateLimitMseeage( DWORD dwSelfStateFlag );
};
