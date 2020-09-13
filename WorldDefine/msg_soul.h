//-----------------------------------------------------------------------------
//!\brief  神魔技能相关
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push,1)

//任务接取失败原因
enum
{
	E_SOUL_JOB								= 1,		//职业不符合
	E_SOUL_POINT							= 2,		//声望不符合
	E_SOUL_PRESKILL							= 3,		//前置技能不符合
	E_SOUL_SOUL								= 4,		//神魔魂不符合
	E_SOUL_MONEY							= 5,		//金钱不符合
	E_SOUL_SKILL							= 6,		//技能不存在
	E_SOUL_ITEM								= 7,		//道具不存在
};

//神魂魔魂以及其声望改变
CMD_START(NS_ChangeSoul)
	INT nGod;					//神魂
	INT nMonster;				//魔魂
	INT nGodPoint;				//神魂声望
	INT nMonsterPoint;			//魔魂声望
CMD_END


//激活神魔技能
CMD_START(NC_ActiveSoulSkill)
	DWORD dwSkillID;			//技能ID
CMD_END

CMD_START(NS_ActiveSoulSkill)
	DWORD	dwSkillID;			//技能ID
	DWORD	dwTime;				//激活后的时长
	DWORD	dwResult;			//结果
CMD_END


//延续神魔技能时间
CMD_START(NC_ContinueSoulSkill)
	DWORD dwSkillID;			//技能ID
	INT64 n64ItemID;			//物品ID
CMD_END

CMD_START(NS_ContinueSoulSkill)
	DWORD dwSkillID;			//技能ID
	DWORD dwResult;				//结果
	DWORD dwTime;				//续约后的时长(秒)
CMD_END

#pragma pack(pop)

