#pragma once

#include "..\WorldDefine\skill_define.h"
#include "..\WorldDefine\talent_define.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\msg_talent.h"
#include "..\WorldDefine\msg_combat.h"

#define MTransSkillTypeID(skill,lv) (DWORD)(skill*100+lv)
#define MTransSkillID(id) (DWORD)(id/100)
#define MTransSkillLevel(id) (INT)(id%100)
#define MTransProduceSkill(id) (DWORD)(id/10)

const int MAX_BATTLE_TALENT_COUNT = 1;

/** \struct tagSkillData
	\brief 技能数据结构（未完）
*/
struct tagSkillData
{
	DWORD					dwSkillID;		// 技能ID
	DWORD					dwTypeID;		// 技能模板ID
	INT						nCurCD;			// 当前冷却值（毫秒）
	INT						nLevel;			// 当前等级
	INT						nLearnLevel;	// 当前学习等级
	INT						nExp;			// 当前经验
	DWORD					dwLimitTime;	// 技能剩余时间

	//--主动技能拥有的属性
	INT						nSkillDmg;		// 技能威力
	FLOAT					fOPDist;		// 作用距离（世界单位）
	INT						nPrepareTime;	// 起手时间（毫秒）
	INT						nCooldown;		// 冷却时间（毫秒）
	DWORD					dwStartTime;	// 冷却计算起始时间
	INT						nCost[ESCT_End];// 消耗
						
	tagSkillProtoClient*	pProto;
	
	tagSkillData(){ZeroMemory(this, sizeof(tagSkillData));}
	tagSkillData(DWORD dwID){ZeroMemory(this, sizeof(tagSkillData)); dwSkillID = dwID;}
};

class NetSession;
/** \class SkillMgr
	\brief 技能管理器
	\remarks 管理客户端本地玩家所学会的技能
*/
class SkillMgr
{
public:
	SkillMgr(void);
	~SkillMgr(void);

	static SkillMgr* Inst();
	
	void Init();
	void Update();
	void Destroy();
	
	/** \根据技能ID获得技能数据
	*/
	const tagSkillData* GetSkillData(DWORD dwID);
	/** \根据技能TypeID获得技能数据
	*/
	const tagSkillData* GetSkillDataByTypeID(DWORD dwTypeID);

	/** \获取普通攻击ID
	*/
	DWORD GetNormalAttackID();
	/** \获取普通攻击的TypeID
	*/
	DWORD GetNormalAttackTypeID();
	/** \获取开采类技能的TypeID
	*/
	DWORD GetGatherSkillTypeID(INT nType);

	/** \使用技能
	*/
	VOID OnUseSkill(DWORD dwTypeID);
	
	
	/** \获取技能列表
	*/
	void GetSkillList(TList<DWORD>& List);

	/** \获取被动技能列表
	*/
	void GetPassiveSkillList(TList<DWORD>& List, TList<DWORD>& lstOther );
	
	/** \获得生产技能
	*/
	void GetProduceSkillList( TList<DWORD>& list );

	/** \获得公会技能
	*/
	void GetGuildSkillList( TList<DWORD>& list );
	/** \获得领域技能技能（包括变身技能）
	*/
	void GetGodSkillList( TList<DWORD>& list );
	/** \获得变身技能
	*/
	void GetTransformSkillList( TList<DWORD>& list );

	/** \获取技能图片
	*/
	tstring GetSkillIconPath(DWORD dwTypeID);
	/** \是否在可投天资范畴
	*/
	BOOL IsInsideOfMaxTalent(ETalentType eTalent);
	/** \获取天资点数
	*/
	INT GetTalentPoint(ETalentType eTalent);

	/** \是否有该技能
	*/
	BOOL IsHaveSkillByTypeID(DWORD dwTypeID);
	BOOL IsHaveSkillBySkillID(DWORD dwSkillID);
	BOOL IsHaveProduceSkillByTypeID(DWORD dwTypeID);

	/** \根据模板ID获取技能是否冷却
	*/
	BOOL IsSkillCDByTypeID(DWORD dwTypeID);
	/** \获取技能CD
	*/
	VOID GetSkillCDTime(DWORD dwID, FLOAT& fCDCount, FLOAT& fRestoreRatio);
	/** \技能是否可学习
		\param dwTypeID为该级技能的原型
		\param n8Type学习类型
	*/
	DWORD IsSkillCanLearn(DWORD dwTypeID);

	/** \网络消息错误提示
	*/
	void ShowNetError(INT nErrorCode);

    /** \获得当前玩家战斗系天资个数
    */
    int GetBattleTalentCount();
    /** \是否获得此天资
    */
    BOOL IsTalentAquired(ETalentType eType);

	/** 获得总共已经投的天资点数
	*/
	INT GetTotalBidden();

	/** 得到投点最多的天资,如果两系天资投点相同，返回ETalentType值小的
	*/
	ETalentType GetMainTalentType();

	/** 判断一个技能是否已经学会
	*/
	BOOL IsSkillLearned( DWORD dwSkillTypeID );

	/** 得到可修炼的天资点数，就是已经投的点加上未投的点还没有到达当前等级的天资上线
		\return 返回值是可修炼的点数，可能是负数，如果是负数就是没有可修炼的点数
				如果配置表里没有填写此级别的天资上线，返回值肯定是负数
	*/
	INT GetExtraTalentPointNum();

	/** 得到各个级别的天资上限
	*/
	INT GetTalentLimit( DWORD dwLevel );

	DWORD AdaptFreshHandSkillID(DWORD dwSkillID);

	/** 获得凝聚技能
	*/
	DWORD GetCondensedSkillID();

	VOID SetSkillRenewTime(DWORD dwSkillID,DWORD dwTime);

	VOID SetLastUseSkillTime(DWORD dwTime);
	DWORD GetLastUseSkillTime();

private:

	
	/** \增加新技能
	*/
	void AddSkill(tagSkillMsgInfo data);
	/** \删除一个已有技能
	*/
	void RemoveSkill(DWORD dwID);
	/** \更新一个技能
	*/
	void UpdateSkill(tagSkillMsgInfo data);
	
	/** \设置天资点数
	*/
	void SetTalentPoint(tagRoleTalent Talent);

	/** \初始化玩家技能
	*/
	DWORD NetInitLocalPlayerSkill(tagNS_GetRoleInitState_Skill* pNetCmd, DWORD dwParam);

	/** \网络消息的处理
	*/
	DWORD NetAddSkill(tagNS_AddSkill* pNetCmd, DWORD dwParam);
	DWORD NetUpdateSkill(tagNS_UpdateSkill* pNetCmd, DWORD dwParam);
	DWORD NetRemoveSkill(tagNS_RemoveSkill* pNetCmd, DWORD dwParam);
	DWORD NetLearnSkill(tagNS_LearnSkill* pNetCmd, DWORD dwParam);
	DWORD NetLevelUpSkill(tagNS_LevelUpSkill* pNetCmd, DWORD dwParam);
	DWORD NetForgetSkill(tagNS_ForgetSkill* pNetCmd, DWORD dwParam);
	DWORD NetClearTalent(tagNS_ClearTalent* pNetCmd, DWORD dwParam);
	DWORD NetAddTalent(tagNS_AddTalent* pNetCmd, DWORD dwParam);
	DWORD NetRemoveTalent(tagNS_RemoveTalent* pNetCmd, DWORD dwParam);
	DWORD NetUpdateTalent(tagNS_UpdateTalent* pNetCmd, DWORD dwParam);
	DWORD NetUpdateSkillCD(tagNS_UpdateSkillCoolDown* pNetCmd, DWORD dwParam);
	DWORD NetUpdateSkillCDEx(tagNS_UpdateSkillCoolDownEx* pNetCmd, DWORD dwParam);
	DWORD OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam);
	DWORD OnNS_RoleGetTalentPoint(tagNS_RoleGetTalentPoint* pNetCmd, DWORD dwParam);

	/** \释放技能Map
	*/
	void FreeMap(); 
private:
	TSFPTrunk<SkillMgr>			m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	
	TMap<DWORD, tagSkillData*>	m_skillMap;
	tagRoleTalent				m_talentPoint[X_MAX_TALENT_PER_ROLE];

	DWORD						m_dwLastUpdateTime;
	DWORD						m_dwLastUseSkillTime;	//上一次使用技能时间
};
