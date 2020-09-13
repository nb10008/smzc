#pragma once
#include "..\WorldDefine\creature_define.h"
#include ".\BaseData.h"

enum ETagNode
{
	ETN_Start,
	ETN_Face = ETN_Start,	// 面部
	ETN_Hat,				// 头部
	ETN_Back,				// 背部
	ETN_RSld,				// 右肩
	ETN_LSld,				// 左肩
	ETN_RHd,				// 右手
	ETN_LHd,				// 左手
	ETN_Wep1, 				// 右腰间
	ETN_Wep2,				// 左腰间
	ETN_Wep3,				// 腰后1
	ETN_Wep4,				// 腰后2
	ETN_Wep5,				// 背后
	ETN_Wep6,				// 右手腕
	ETN_Num,
};
const tstring g_strTagNodeName[ETN_Num] = 
{
	_T("tag_Face"),       // 面部
	_T("tag_Hat"),        // 头部
	_T("tag_Back"),       // 背部
	_T("tag_RSld"),       // 右肩
	_T("tag_LSld"),       // 左肩
	_T("tag_RHd"),        // 右手
	_T("tag_LHd"),        // 左手
	_T("tag_Wep1"),       // 右腰间
	_T("tag_Wep2"),       // 左腰间
	_T("tag_Wep3"),       // 腰后1
	_T("tag_Wep4"),       // 腰后2
	_T("tag_Wep5"),       // 背后
	_T("tag_Wep6"),       // 右手腕
};

/** \生物模型相关数据
*/
struct tagCreatureMdlData
{
	float   fBeAttackSfxScale;		// 被攻击特效缩放系数
	bool    bCanPitch;				// 是否俯仰
	tstring szDeadSound;			// 死亡音效
	tstring szCombatSound;			// 进入战斗音效
	tstring szRoarSound;			// 进攻喊叫音效
	tstring szHowlSound;			// 挨打喊叫音效
	tstring szFootSound;			// 脚步声（前缀，后缀为表面类型）
	tstring szNormalAttackBeAtkSfx;	// 普通攻击对方被攻击特效
	Vector3 vec3Camera;				// 观察模型的摄像机位置
	float   fMoveSpeedFactor;		// 模型播放走跑动作时的播放速度
};

/** class	CreatureData
	brief	怪物，NPC，地物，宠物原始属性结构存储
*/
class CreatureData 
	: public BaseData
{
public:
	CreatureData(void);
	~CreatureData(void);
	
	static CreatureData* Inst();
	
	/** 载入属性文件
	*/
	virtual void LoadFromFile();
	/** 获取NPC模型文件路径
	*/
	static tstring GetNpcMdlPath(const tagCreatureProto *pNpcAtt);
	/**	获取NPC蒙皮文件路径
	*/
	static tstring GetNpcSkinPath(const tagCreatureProto *pNpcAtt, int index);
	/** 查找NPC的原始属性
	*/
	const tagCreatureProto* FindNpcAtt(DWORD attID)
	{
		return FindInMap(m_creatureMap,attID);
	}
	/** 获取所有的NPC原始属性
	*/
	void GetNpcMap(map<DWORD,tagCreatureProto>& npcMap)
	{
		npcMap = m_creatureMap;
	}
	/** 获取模型相关数据
	*/
	const tagCreatureMdlData* FindMdlData(const TCHAR* szMdlName);
	/** 获取骨骼名称
	*/
	const tstring FindNpcBoneName( const DWORD dwTypeID, const TCHAR* szTagNodeName );
	const tstring FindNpcBoneName( const TCHAR* szMdlName, const TCHAR* szTagNodeName );
	/**获取灵感值
	*/
	INT FindCreatureInspiration(DWORD dwTypeID);
private:
	DWORD MakeNpcBoneNameKey( const TCHAR* szMdlName, const TCHAR* szTagNodeName );
private:
	map<DWORD,tagCreatureProto>			m_creatureMap;
	map<DWORD,tagCreatureMdlData>		m_mdlData;
	map<DWORD,tstring>					m_npcBoneNameMap;
	map<DWORD,INT>						m_mapInspiration;
};
