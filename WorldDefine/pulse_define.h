#pragma once
#pragma pack(push, 1)


#define MIN_PULSE_LEVEL 50
#define MAX_PULSE_TIMEONEDAY 100

struct tagPulseProto
{
	DWORD	dwId;
	
	DWORD	dwPageType;	// 分页类型
	DWORD	dwPos;				// 页面位置
	DWORD	dwLevel;

	DWORD	dwGodLock;
	DWORD	dwLevelLock;
	DWORD	dwMaxLevel;
	DWORD	dwSkillID;
	bool			bIsAciveSkill;
	DWORD	dwSuccessRate;
	DWORD	dwItemID;
	DWORD	dwItemNum;
	DWORD	dwItemID2;	//新加的道具
	DWORD	dwItemNum2;	//新加的道具数目
	DWORD	dwMoney;

	DWORD	dwPerSkill1;
	DWORD	dwPerSkill2;
	DWORD	dwPerSkill3;

	DWORD	dwNextSkill1;
	DWORD	dwNextSkill2;
	DWORD	dwNextSkill3;
};

struct tagPulseClientProto : tagPulseProto
{					
	bool					bIsSpecSkill;							// 是否是特殊能力（大图标）
	TCHAR				szName[X_LONG_NAME];		//名称
	TCHAR				szIcon[X_LONG_NAME];		//图标
	TCHAR				szSkillDesc[X_LONG_NAME];		//说明
	TCHAR				szNextSkillDesc[X_LONG_NAME];	//领域类型描述
};
#pragma pack(pop)