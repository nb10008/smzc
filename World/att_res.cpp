//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: att_res.cpp
// author: Sxg
// actor:
// data: 2008-05-29
// last:
// brief: 资源管理器实现
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/skill_define.h"
#include "../WorldDefine/buff_define.h"
#include "../WorldDefine/trigger_define.h"
#include "../WorldDefine/compose_define.h"
#include "../WorldDefine/shop_define.h"
#include "../WorldDefine/func_npc_define.h"
#include "../WorldDefine/move_define.h"
#include "../WorldDefine/compose_define.h"
#include "../WorldDefine/suit_define.h"
#include "../WorldDefine/mall_define.h"
#include "../WorldDefine/longhun_define.h"
#include "../WorldDefine/city_define.h"
#include "../WorldDefine/miracle_define.h"

#include "pet_define.h"
#include "../WorldDefine/pet_equip_define.h"
#include "../WorldDefine/motion_define.h"
#include "../WorldDefine/clergy_define.h"
#include "../WorldDefine/family_define.h"

#include "../ServerDefine/pet_skill_define.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/consolidate_define.h"
#include "../ServerDefine/sspawnpoint_define.h"
#include "../ServerDefine/vip_netbar_define.h"
#include "../ServerDefine/guild_define.h"


#include "creature.h"
#include "att_res.h"
#include "script_mgr.h"
#include "guild_upgrade.h"
#include "guild_CofC.h"

#include <functional>

#include "player_regression.h"

#include "online_reward.h"
// Jason 2010-3-30
#include "title_mgr.h"
#include "convenient_team.h"
#include "shielding_mgr.h"
#include "wardrobe.h"

//********************************* 读取标志位常量 **********************************
const INT X_READ_FILE_1	= 1;
const INT X_READ_FILE_2	= 2;

//*********************************** 文件名常量 ************************************
const LPCTSTR FileName_NameFilter		= _T("data/local/zhCN/attdata/name_filter.xml");
//const LPCTSTR FileName_ChatFilter		= _T("data/local/zhCN/attdata/chat_filter.xml");

const LPCTSTR FileName_RoleAttLevelUp	= _T("data/system/attdata/role_att_level_up.xml");
const LPCTSTR FileName_AttDefMinMax		= _T("data/system/attdata/att_def_min_max.xml");

const LPCTSTR FileName_ServerFunctionSwitch		= _T("server_data/server_function_switch.xml");

const LPCTSTR FileName_Item				= _T("data/system/attdata/item_proto.xml");
const LPCTSTR FileName_Equip			= _T("data/system/attdata/equip_proto.xml");
const LPCTSTR FileName_Gem				= _T("data/system/attdata/gem_proto.xml");
const LPCTSTR FileName_EquipQltyPct		= _T("data/system/attdata/equip_qlty_pct.xml");
const LPCTSTR FileName_EquipQltyEffect	= _T("data/system/attdata/equip_qlty_effect.xml");
const LPCTSTR FileName_Suit				= _T("data/system/attdata/equip_suit.xml");

const LPCTSTR FileName_SoulAttProto		= _T("data/system/attdata/soul_att_def.xml");
const LPCTSTR FileName_StarEnhanceProto		= _T("data/system/attdata/star_enhance.xml");
const LPCTSTR FileName_SoulRebornProto	= _T("data/system/attdata/soul_reborn.xml");
const LPCTSTR FileName_SoulEquipHoleProto	= _T("data/system/attdata/soul_equip_hole.xml");
const LPCTSTR FileName_SoulEquipInlayProto	= _T("data/system/attdata/soul_equip_inlay.xml");
const LPCTSTR FileName_LongHun			= _T("data/system/attdata/longhun_proto.xml");

const LPCTSTR FileName_FashionQltyEffect= _T("server_data/fashion_qlty_effect.xml");
const LPCTSTR FileName_FashionColorPct	= _T("server_data/fashion_color_pct.xml");

const LPCTSTR FileName_Shop				= _T("data/system/attdata/shop_proto.xml");
const LPCTSTR FileName_ShopRare			= _T("data/system/attdata/shop_proto_rare.xml");

const LPCTSTR FileName_Dak				= _T("data/system/attdata/map_switch_proto.xml");

const LPCTSTR FileName_buff             = _T("data/system/attdata/buff_proto.xml");
const LPCTSTR FileName_skill            = _T("data/system/attdata/skill_proto.xml");
const LPCTSTR FileName_trigger          = _T("data/system/attdata/trigger_proto.xml");

const LPCTSTR FileName_creature_ai      = _T("data/system/attdata/creature_ai.xml");
const LPCTSTR FileName_creature			= _T("data/system/attdata/creature_proto.xml");

const LPCTSTR FileName_Posy_Pos			= _T("data/system/attdata/posy_pos.xml");
const LPCTSTR FileName_Posy_Proto		= _T("data/system/attdata/posy_proto.xml");

const LPCTSTR FileName_Engrave_Pos		= _T("data/system/attdata/engrave_pos.xml");
const LPCTSTR FileName_Engrave_Proto    = _T("data/system/attdata/engrave_proto.xml");

const LPCTSTR FileName_Consolidate_Proto= _T("data/system/attdata/consolidate_item.xml");

const LPCTSTR FileName_Produce_Proto	= _T("data/system/attdata/compose_proto.xml");
const LPCTSTR FileName_Quench_Proto		= _T("data/system/attdata/quench_proto.xml");
const LPCTSTR FileName_DeCompose_Proto  = _T("data/system/attdata/decompose_proto.xml");
const LPCTSTR FileName_Extract_Proto	= _T("data/system/attdata/extract_proto.xml");

const LPCTSTR FileName_MallItem			= _T("server_data/mall_item_proto.xml");
const LPCTSTR FileName_MallPack			= _T("server_data/mall_pack_proto.xml");
const LPCTSTR FileName_MallFreeItem		= _T("server_data/mall_free_item_proto.xml");
const LPCTSTR FilaName_RecommendingItem = _T("server_data/recommending_item.xml");
const LPCTSTR FilaName_MallCfg			= _T("server_data/mall_cfg.xml");

const LPCTSTR FileName_VIPStall			= _T("server_data/vip_stall_proto.xml");

const LPCTSTR FileName_Instance_Item	= _T("data/system/attdata/instance_item.xml");
const LPCTSTR FileName_Instance_Skill	= _T("data/system/attdata/instance_item.xml");
const LPCTSTR FileName_Spawn_Point		= _T("data/system/attdata/spawn_point.xml");
const LPCTSTR FileName_Level_Mapping	= _T("data/system/attdata/level_mapping.xml");
const LPCTSTR FileName_Instance_Proto	= _T("data/system/attdata/instance_proto.xml");

const LPCTSTR FileName_Title_Proto		= _T("data/system/attdata/role_title_proto.xml");
const LPCTSTR FileName_ClanTreasure		= _T("data/system/attdata/clantreasure_proto.xml");

const LPCTSTR FileName_GuildPower		= _T("data/system/attdata/guild_power.xml");
const LPCTSTR FileName_GuildKick		= _T("data/system/attdata/guild_kick.xml");
const LPCTSTR FileName_GuildAppoint		= _T("data/system/attdata/guild_appoint.xml");

const LPCTSTR FileName_GuildUpgrade		= _T("data/system/attdata/guild_facilities.xml");
const LPCTSTR FileName_GuildAffair		= _T("data/system/attdata/guild_affair.xml");
const LPCTSTR FileName_GuildSkill		= _T("data/system/attdata/guild_skill.xml");
const LPCTSTR FileName_GuildCommerce	= _T("data/system/attdata/guild_commerce.xml");
const LPCTSTR FileName_GuildCommodity	= _T("data/system/attdata/guild_commodity.xml");
const LPCTSTR FileName_GuildCofC		= _T("data/system/attdata/guild_CofC.xml");
const LPCTSTR FileName_GuildCofC_SP		= _T("data/system/attdata/guild_CofC_spec.xml");

const LPCTSTR FileName_SingleSpawnPoint_Proto	= _T("data/system/attdata/sspawnpoint_proto.xml");

const LPCTSTR FileName_Pet_Proto				= _T("data/system/attdata/pet_proto.xml");
const LPCTSTR FileName_PetLvlUpProto			= _T("data/system/attdata/pet_levelup_proto.xml");
const LPCTSTR FileName_PetLvlUpItemProto		= _T("data/system/attdata/pet_lvlup_item_proto.xml");
const LPCTSTR FileName_PetSkill_Proto			= _T("data/system/attdata/pet_skill_proto.xml");
const LPCTSTR FileName_PetEquip_Proto			= _T("data/system/attdata/pet_equip_proto.xml");
//const LPCTSTR FileName_PetDefMinMax_Proto		= _T("data/system/attdata/pet_att_def_proto.xml");
const LPCTSTR FileName_PetGather_Proto			= _T("data/system/attdata/pet_gather_proto.xml");
const LPCTSTR FileName_PetWuXing_Proto			= _T("data/system/attdata/pet_wuxing_proto.xml");
const LPCTSTR FileName_VNBEquip_Proto			= _T("data/system/attdata/vip_netbar_equip_proto.xml");
const LPCTSTR FileName_VNBGift_Proto			= _T("data/system/attdata/vip_netbar_gift_proto.xml");
const LPCTSTR FileName_Motion_Proto				= _T("data/system/attdata/motion_proto.xml");

const LPCTSTR FileName_QuestUseSpeciItem_Proto  =   _T("data/system/quest/complete_fuzhu.xml");

// Jason 2009-12-7 神龙赐福配置文件
const LPCTSTR FileName_LessingLoong				= _T("data/system/attdata/lessing_loong.xml");
// Jason 2009-12-8 点金石
const LPCTSTR FileName_PhilosophersStoneConfig	= _T("data/system/attdata/BeGoldStone_Info.xml");
const LPCTSTR FileName_City_Proto				= _T("data/system/attdata/city_proto.xml");
const LPCTSTR FileName_Master_Apprentice		= _T("data/system/attdata/master_apprentice.xml");

const LPCTSTR FileName_Player_Regression_Rule	= _T("data/system/attdata/player_back.xml");

const LPCTSTR FileName_MiracleSkill_Proto		= _T("data/system/attdata/miracle_skill_proto.xml");

const LPCTSTR FileName_Online_Reward_Param		= _T("server_data/online_reward.xml");
//  天资购买的花费表
const LPCTSTR FileName_Talent_Avail_Cost        = _T("data/system/attdata/TalentAvail_buy.xml");

const LPCTSTR FileName_MapBuffInfo               = _T("data/system/attdata/buff_countdown.xml");

// 击杀经验表
const LPCTSTR FileName_Kill_EXP					= _T("data/system/attdata/KillEXP.xml");

const LPCTSTR FileName_Spin_Proto				= _T("data/system/attdata/spin_proto.xml");
const LPCTSTR FileName_Spin_Rate_Proto			= _T("data/system/attdata/spin_rate_proto.xml");

const LPCTSTR FileName_WuXunProto               = _T("data/system/attdata/arena_proto.xml");

//龙之试炼系统-试炼列表
const LPCTSTR FileName_Train_Proto				= _T("data/system/attdata/trial_proto.xml");

// Jason 2010-4-13 v2.0.0
const LPCTSTR	FileName_FabaoLevelProto		= _T("data/system/attdata/level_fabao.xml");
const LPCTSTR	FileName_FabaoWuxingProto		= _T("data/system/attdata/wuxing_proto.xml");
const LPCTSTR	FileName_FabaoProto				= _T("data/system/attdata/MagicWeapon.xml");

// 圣灵属性[wcy 2012-03-30]
const LPCTSTR	FileName_HolyProto				= _T("data/system/attdata/HolyGods_proto.xml");
// 圣灵升级表[wcy 2012-04-09]
const LPCTSTR	FileName_HolyLevelUp				= _T("data/system/attdata/HolyGods_growup.xml");
// 圣纹附加属性[wcy 2012-04-17]
const LPCTSTR	FileName_HolyEquipProto				= _T("data/system/attdata/holy_equip_proto.xml");
// 圣纹强化属性[wcy 2012-04-26]
const LPCTSTR	FileName_HolyEquipEnhance				= _T("data/system/attdata/Holy_empowerment.xml");

// Jay 2010-4-21 添加限制只能放在背包中的物品
const LPCTSTR	FileName_ContainerLimit			= _T("data/system/attdata/container_limit.xml");
// Jason 2010-5-11	任务完成条件之一，无阻令牌
const LPCTSTR	FileName_QuestCompleteFuzhu		= _T("data/system/quest/complete_fuzhu.xml");
// Jason 2010-5-24 v2.1.2 飞升技能
const LPCTSTR	FileName_LearnSoaringSkill		= _T("data/system/attdata/soar_skill_proto.xml");
// 熔铸
const LPCTSTR	FileName_CastingProto			= _T("data/system/attdata/casting_proto.xml");
const LPCTSTR	FileName_MsgDeliverProto		= _T("server_data/msg_switch.xml");
const LPCTSTR	FileName_SkillBoxProto		= _T("data/system/attdata/skill_box.xml");

// 任务板
const LPCTSTR	FileName_BoardQuestProto		= _T("data/system/attdata/Questdata.xml");
const LPCTSTR	FileName_BoardQuestRandRate		= _T("data/system/attdata/Randomrefresh.xml");
// Jason 2010-6-18 v1.0.0
LPCTSTR FileName_FConsolidateProto			= _T("data/system/attdata/streng_proto.xml");
LPCTSTR FileName_FConsolidateLuckySignPct	= _T("data/system/attdata/lucky_pct.xml");
LPCTSTR FileName_FFuhunStone					= _T("data/system/attdata/soulstone.xml");

// 天生任务
const LPCTSTR FileName_InitialSkillProto	= _T("data/system/attdata/initial_skill.xml");
// 神铸等级文件
LPCTSTR		FileName_GodStrengthenProto		= _T("data/system/attdata/soar_levelup.xml");
// 副本活动等组队信息
LPCTSTR		FileName_GroupScene				= _T("data/system/attdata/group_scene.xml");
// 妖精食物原型文件
LPCTSTR		FileName_FabaoFood				= _T("data/system/attdata/fabaofood_proto.xml");
// 妖精果实
LPCTSTR		FileName_FabaoFruit				= _T("data/system/attdata/fabaofruit_proto.xml");
// 妖精妖精特技
LPCTSTR		FileName_FabaoStunt				= _T("data/system/attdata/fabao_stunt.xml");
// 妖精点数提升 [3/13/2012 zhangzhihua]
LPCTSTR		FileName_FabaoPromote			= _T("data/system/attdata/fairy_upgrade.xml");
// 妖精融合 [3/22/2012 zhangzhihua]
LPCTSTR		FileName_FairyFusion			= _T("data/system/attdata/Fairy_smelting.xml");
LPCTSTR		FileName_FairyFusionType		= _T("data/system/attdata/Fairy_smelting_type.xml");
// 守护地图奖励
LPCTSTR		FileName_WarRewardPoint			= _T("data/system/attdata/Rewards_point.xml");

const LPCTSTR	FileName_MallOfferProto		= _T("server_data/mall_offer_proto.xml");
// 公会战场每日奖励
const LPCTSTR	FileName_GuildDailyReward	= _T("data/system/attdata/war_award.xml");

const LPCTSTR	FileName_GodBidProto = _T("server_data/bid_proto.xml");
// 妖精繁殖表
LPCTSTR		FileName_FabaoBirth		= _T("data/system/attdata/fabao_birth.xml");
// 妖精繁殖类型表
LPCTSTR		FileName_FabaoBirth_Type	= _T("data/system/attdata/fabao_birth_type.xml");
// 海外筛选器
LPCTSTR		FileName_Shielding	= _T("data/system/attdata/overseas_hide.xml");

// 砸金蛋属性表
LPCTSTR		FileName_Gold_Egg_Proto	= _T("data/system/attdata/gold_egg.xml");
LPCTSTR		FileName_Gold_Egg_Pct_Proto	= _T("server_data/gold_egg_pct.xml");

// 衣橱系统属性表
LPCTSTR		FileName_Wardrobe_Proto = _T("data/system/attdata/wardrobe_proto.xml");
LPCTSTR		FileName_Advanced_Compose_Proto = _T("data/system/attdata/advanced_compose_proto.xml");
//  神职属性表
LPCTSTR		FileName_Clergy	= _T("data/system/attdata/clergy_proto.xml");
// jason 神格
LPCTSTR		FileName_GodHead	= _T("data/system/attdata/godhead.xml");

//  神职领域属性表
LPCTSTR		FileName_GodArea	= _T("data/system/attdata/godarea_proto.xml");

//  所谓地图盒子
LPCTSTR		FileName_MapBox	= _T("data/system/attdata/mapbox_proto.xml");

// 屏蔽字表
LPCTSTR		FileName_FilterWords = _T("data/system/attdata/filter_words.xml");

//  稀有领域特效
LPCTSTR		FileName_AreaEffect	= _T("data/system/attdata/areaeffect_proto.xml");
//	家族任务条件
LPCTSTR		FileName_FamilyQuestCondition = _T("data/system/attdata/FamilyQuest_Att_Proto.xml"); 
LPCTSTR		FileName_FamilyQuest =  _T("data/system/attdata/FamilyQuest_Proto.xml"); 
LPCTSTR		FileName_FamilyQuestRewardProto =  _T("data/system/attdata/FamilyQuest_Reward_Proto.xml"); 

// Key码领取奖励表
LPCTSTR		FileName_KeyCodeReward = _T("data/system/attdata/multi_medium.xml");

//  守护妖精属性表
LPCTSTR		FileName_Sprite	= _T("data/system/attdata/sprite_proto.xml");

//  家族守护妖精修炼属性表
LPCTSTR		FileName_SpriteTraining	= _T("data/system/attdata/sprite_training_proto.xml");

//  礼包属性表
LPCTSTR		FileName_Idbuyback	= _T("data/system/attdata/Idbuyback.xml");

// 经脉
LPCTSTR		FileName_Pulse_Proto	= _T("data/system/attdata/GodBlood_proto.xml");
// 装备成长
LPCTSTR		FileName_EquipGrowProto = _T("data/system/attdata/equip_levelup.xml");

// 老虎机
LPCTSTR		FileName_Tiger_Proto = _T("data/system/attdata/tiger.xml");

// 离线经验
LPCTSTR		FileName_ExpOffline_Proto = _T("data/system/attdata/exp_offline.xml");

// 神之战灵技能
LPCTSTR		FileName_ForceSkill_Proto = _T("data/system/attdata/force_skill.xml");

LPCTSTR		FileName_ForceLevel_Proto = _T("data/system/attdata/force_level.xml");

LPCTSTR		FileName_SoulCrystalLvlLine_Proto = _T("data/system/attdata/soulcrystallevel.xml");
LPCTSTR		FileName_SoulCrystalAtt_Proto = _T("data/system/attdata/soulcrystalatt_proto.xml");

AttRes g_attRes;
tagLongHunCanActivePos g_sLongHunCanActivePos[EEP_MaxEquip];

AttRes::AttRes()
:m_Trunk(this), m_TitleProto(NULL),m_nFabaoMaxStage(0)
{
}

//*****************************************************************************************

//-----------------------------------------------------------------------------
// 读取所有资源文件
//-----------------------------------------------------------------------------
BOOL AttRes::Init()
{
	CreateObj("AttResContainer", "VarContainer");
	m_pVar = "AttResContainer";
	m_pUtil = "Util";
	m_pFun = NULL;	
	m_nFabaoMaxStage = 0;

	INT num = MAX_TITLE_NUM_EX;
	if( num <= 0 )
	{
		m_TitleProto = new tagTitleProto;
		num = 1;
	}
	else
		m_TitleProto = new tagTitleProto[num];
	ZeroMemory(m_TitleProto,sizeof(tagTitleProto)*num);

	// 从脚步管理器中获得不同系统中相关字段长度
	InitVarLen();
	
	// 加载过滤词表
	InitFilterWords(m_vectNameFilter, FileName_NameFilter);
	//InitFilterWords(m_vectChatFilter, FileName_ChatFilter);

	// 加载聊天过滤词表
	InitChatFilterWords(m_vectChatFilter, FileName_FilterWords);
	
	// 加载人物升级静态数据
	m_LevelUpEffect[0].nExpLevelUp = 0; // 重新初始化该值
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitRoleAttLevelUp));
	LoadResArray(m_LevelUpEffect, 1, g_world.GetMaxRoleLevel()/*MAX_ROLE_LEVEL*/, FileName_RoleAttLevelUp);

	// 属性默认值及最小最大值静态数据
	for(INT n = 0; n < ERA_End; n++)
	{
		m_AttDefMinMax[n].nDefRole		=	0;
		m_AttDefMinMax[n].nDefCreature	=	0;
		m_AttDefMinMax[n].nMax			=	INT_MAX;
		m_AttDefMinMax[n].nMin			=	INT_MIN;
	}

	// 初始化可铭纹装备部位
	for(INT n = 0; n < EPosyAtt_End; ++n)
	{
		for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
		{
			m_PosyPos[n][m].byConsolidate = false;
			m_PosyPos[n][m].ePos = EEP_Start;
		}
	}

	m_bsServerFunctionSwitch.reset();
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::IniServerFunctionSwitch));
	LoadResArray(&m_bsServerFunctionSwitch, 0, ESFS_End - 1, FileName_ServerFunctionSwitch);

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitAttDefMinMax));
	LoadResArray(m_AttDefMinMax, ERA_Physique, ERA_End - 1, FileName_AttDefMinMax);

	// 精力系统相关
	InitSpiritSystem();

	// 加载帮派权限
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePosGuildPower));
	LoadResArray(m_GuildPowerDefault, EGMP_Start, EGMP_End, FileName_GuildPower);

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePosGuildKick));
	LoadResArray(m_GuildKickDefault, EGMP_Start, EGMP_End, FileName_GuildKick);

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePosGuildAppoint));
	LoadResArray(m_GuildAppointDefault, EGMP_Start, EGMP_End, FileName_GuildAppoint);

	//加载武勋属性表
    m_WuXunProto[0].n16WuXunLevel = 0;
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::IniWuXunProto));
	LoadResArray(m_WuXunProto, 0,MAX_WUXUN_LEVEL,FileName_WuXunProto);

	//加载天资花费表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitTalentAvailCost));
	LoadResMap(m_mapTalentAvailCost, FileName_Talent_Avail_Cost);

	//加载进地图buff表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitMapBuffInfo));
	LoadResMap(m_mapMapBuffInfo, FileName_MapBuffInfo);


	// 加载帮派设施升级需求信息
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneGuildUpgradeProto));
	LoadResMap(m_GuildUpgradeNeedInfo, FileName_GuildUpgrade);

	// 加载帮派事务信息
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneGuildAffairProto));
	LoadResMap(m_GuildAffairInfo, FileName_GuildAffair);

	// 加载帮派技能信息
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneGuildSkillProto));
	LoadResMap(m_GuildSkillInfo, FileName_GuildSkill);
	InitGuildSkillMaxLvlInfo();	// 初始化公会技能上限map

	// 加载帮派跑商信息
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneGuildCommerceProto));
	LoadResMap(m_GuildCommerceInfo, FileName_GuildCommerce);

	// 商货信息
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneCommodityProto));
	LoadResMap(m_GuildCommodityProto, FileName_GuildCommodity);

	// 商会信息
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneCofCProto));
	LoadResMap(m_CofCProto, FileName_GuildCofC);

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneCofCSPProto));
	LoadResMap(m_CofCSPProto, FileName_GuildCofC_SP);

	// 加载物品
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneItemProto));
	LoadResMap(m_mapItemProto, FileName_Item);

	// 加载装备
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneEquipProto));
	LoadResMap(m_mapEquipProto, FileName_Equip);

	// 加载宝石等
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneGemProto));
	LoadResMap(m_mapGemProto, FileName_Gem);

	
	// 加载套装等
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSuitProto));
	LoadResMap(m_mapSuitProto, FileName_Suit);

	// 加载装备各品级鉴定几率
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneEquipQltyPct));
	LoadResMap(m_mapEquipQltyPct, FileName_EquipQltyPct);

	// 加载元神基本属性表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneStarEnhanceProto));
	LoadResMap(m_mapStarEnhanceProto, FileName_StarEnhanceProto);

	// 加载元神装备强化属性表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSoulAttProto));
	LoadResMap(m_mapSoulAttProto, FileName_SoulAttProto);

	// 加载元神转生属性表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSoulRebornProto));
	LoadResMap(m_mapSoulRebornProto, FileName_SoulRebornProto);

	// 加载元神凿孔属性表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSoulEquipHoleProto));
	LoadResMap(m_mapSoulEquipHoleProto, FileName_SoulEquipHoleProto);

	// 加载元神镶嵌属性表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSoulEquipInlayProto));
	LoadResMap(m_mapSoulEquipInlayProto, FileName_SoulEquipInlayProto);


	// 加载龙魂能力表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneLongHunProto));
	LoadResMap(m_mapLongHunProto, FileName_LongHun);

	// 初始化龙魂能力辅助数组 
	InitLongHunAss();			// -- 随机龙魂能力时使用
	InitLongHunCanActivePos();	// -- 换装时判断影响部位使用

	// 加载品级装备属性参数
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitEquipQltyEffect));
	LoadResArray(m_EquipQltyEffect, 0, X_EQUIP_QUALITY_NUM - 1, FileName_EquipQltyEffect);

	// 加载时装生成相关参数
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitFashionQltyEffect));
	LoadResArray(m_FashionGen, 0, X_EQUIP_QUALITY_NUM - 1, FileName_FashionQltyEffect);

	// 加载时装生成时颜色概率
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitFashionColorPct));
	LoadResArray(m_FashionColorPct, 0, X_COLOR_NUM - 1, FileName_FashionColorPct);

	// 加载商店
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneShopProto));
	LoadResMap(m_mapShopProto, FileName_Shop, FileName_ShopRare);

	// 加载商城
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMallItemProto));
	LoadResMap(m_mapMallItemProto, FileName_MallItem);

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMallPackProto));
	LoadResMap(m_mapMallPackProto, FileName_MallPack);

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMallRecommendingItemInfo));
	LoadResMap(m_mapMallRecommendingItemInfo, FilaName_RecommendingItem);

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMallCfgProto));
	LoadResArray(m_dwMallCfg, 0, EMCA_End - 1, FilaName_MallCfg);
	
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMallFreeItemProto));
	LoadResArray(&m_MallFreeItemProto, 1, 1, FileName_MallFreeItem);

	// 加载VIP摊位
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneVIPStallProto));
	LoadResArray(m_nVIPStallRent, 0, VIP_STALL_MAX_NUM-1, FileName_VIPStall);

	// 加载驿站
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneDakProto));
	LoadResMap(m_mapDakProto, FileName_Dak);
	
	// 初始化技能
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSkillProto));
	LoadResMap(m_mapSkillProto, FileName_skill);

    // 加载状态
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneBuffProto));
	LoadResMap(m_mapBuffProto, FileName_buff);

	// 加载触发器
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneTriggerProto));
	LoadResMap(m_mapTriggerProto, FileName_trigger);

	// 加载技能盒子
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSkillBoxProto));
	LoadResMap(m_mapSkillBoxProto,FileName_SkillBoxProto);

	// 计算技能、状态、触发器之间影响
    LoadModifyMap();

	// 加载生物
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneCreatureProto));
	LoadResMap(m_mapCreatureProto, FileName_creature);

	// 加载生物AI
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneCreatureAIProto));
	LoadResMap(m_mapCreatureAI, FileName_creature_ai);

	// 加载可强化装备部位
	//SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePosyPosProto));
	//LoadResArray(m_PosyPos, 0 , EPosyAtt_End - 1, FileName_Posy_Pos);

	// 加载铭纹静态表
	//SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePosyProto));
	//LoadResMap(m_mapPosyProto, FileName_Posy_Proto);

	// 加载镌刻装备部位
	//SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePosyPosProto));
	//LoadResArray(m_EngravePos, 0, EEngraveAtt_End - 1, FileName_Engrave_Pos);

	// 加载镌刻静态表
	//SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneEngraveProto));
	//LoadResMap(m_mapEngraveProto, FileName_Engrave_Proto);

	// 加载镶嵌,烙印,龙附静态表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneConsolidateProto));
	LoadResMap(m_mapConsolidateProto, FileName_Consolidate_Proto);

	// 加载生活技能静态表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneProduceProto));
	LoadResMap(m_mapProduceProto, FileName_Produce_Proto);

	// 点化,装备分解
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneDeComposeProto));
	LoadResMap(m_mapDeComposeProto, FileName_DeCompose_Proto);

	// 萃取
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneExtractProto));
	LoadResMap(m_mapExtractProto, FileName_Extract_Proto);

	// 淬火
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneQuenchProto));
	LoadResMap(m_mapQuenchProto, FileName_Quench_Proto);

	// 称号
	// Jason 2010-3-30
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneTitleProto));
	LoadResArray(m_TitleProto, 0, MAX_TITLE_NUM_EX - 1, FileName_Title_Proto);

	// 氏族珍宝
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneClanTreasureProto));
	LoadResArray(m_ClanTreasureProto, 0, CLAN_TREASURE_NUM - 1, FileName_ClanTreasure);

	// 怪物AI进行分组
	GroupCreatureAI();

	// 加载副本中不能使用的物品
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneInsItemProto));
	LoadResMap(m_mapInstanceItem, FileName_Instance_Item);

	// 加载副本中不能使用的技能
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneInsSkillProto));
	LoadResMap(m_mapInstanceSkill, FileName_Instance_Skill);

	// 副本随机刷怪点
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSpawnPointProto));
	LoadResMap(m_mapSpawnPointProto, FileName_Spawn_Point);

	// 副本随机刷怪点等级映射表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneLevelMapping));
	LoadResMap(m_mapLevelMapping, FileName_Level_Mapping);

	// 初始化副本静态数据
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneInstanceProto));
	LoadResMap(m_mapInstanceProto, FileName_Instance_Proto);

	// 初始化非副本刷怪点静态数据
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSSpawnPointProto));
	LoadResMap(m_mapSSpawnPoint, FileName_SingleSpawnPoint_Proto);

	// 初始化宠物技能原型
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePetSkillProto));
	LoadResMap(m_mapPetSkillProto, FileName_PetSkill_Proto);  

	// 初始化宠物等级对应固定技能id
	InitPetSkillsVec();	

	// 初始化宠物原型
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePetProto));
	LoadResMap(m_mapPetProto, FileName_Pet_Proto);

	// 初始化宠物原型
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePetLvlUpProto));
	LoadResMap(m_mapPetLvlUpProto, FileName_PetLvlUpProto);  

	// 初始化宠物原型
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePetLvlUpItemProto));
	LoadResMap(m_mapPetLvlUpItemProto, FileName_PetLvlUpItemProto);  

	// 初始化宠物装备原型
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePetEquipProto));
	LoadResMap(m_mapPetEquipProto, FileName_PetEquip_Proto);  

	// 初始化宠物采集表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePetGatherProto));
	LoadResMap(m_mapPetGatherProto, FileName_PetGather_Proto);  

	// 初始化宠物五行表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePetWuXingProto));
	LoadResMap(m_mapPetWuXingProto, FileName_PetWuXing_Proto);  

	// 初始化金牌网吧礼品
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneVNBGiftProto));
	LoadResMap(m_mapVNBGiftProto, FileName_VNBGift_Proto);  

	// 初始化金牌网吧装备
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneVNBEquipProto));
	LoadResMap(m_mapVNBEquipProto, FileName_VNBEquip_Proto);  

	// 初始个性动作
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMotionProto));
	LoadResMap(m_mapMotionProto, FileName_Motion_Proto);
	
	// 初始化任务特殊道具
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitQuestUseSpeciItemProto));
	LoadResMap(m_mapQuestUseSpeciItemProto, FileName_QuestUseSpeciItem_Proto);

	// 击杀经验数值
 	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitKillObjExp));
 	LoadResMap(m_mapKillObjExp, FileName_Kill_EXP);

	// Jason 2009-12-7 初始化神龙赐福配置数据
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitLessingLoong));
	LoadResMap(m_mapConfigInfoOfLessingLoong, FileName_LessingLoong); 
	ROLE_MAX_LESSLEVEL_OF_LOONG = m_mapConfigInfoOfLessingLoong.Size  ();
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitPhilosophersStoneConfig));
	LoadResMap(m_mapPhilosophersStoneConfig, FileName_PhilosophersStoneConfig);  

	// 初始化城市系统
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneCityProto));
	LoadResMap(m_mapCityProto, FileName_City_Proto);

	// 初始化仙界技能修行
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitMiracleSkillProto));
	LoadResMap(m_mapMiracleSkillProto, FileName_MiracleSkill_Proto);

	// 初始化师徒
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMasterApprenticeProto));
	LoadResMap(m_mapMasterApprenticeProto, FileName_Master_Apprentice);

	// 精纺成功率
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSpinRateProto));
	LoadResMap(m_mapSpinRateProto, FileName_Spin_Rate_Proto);

	// 龙之试炼系统
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneTrainProto));
	LoadResMap(m_mapTrainAttProto, FileName_Train_Proto);

	// 天生技能表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneInitialSkillProto));
	LoadResMap(m_mapInitialSkill, FileName_InitialSkillProto);

	// 精纺
	InitSpinProto(m_mapSpinProto, FileName_Spin_Proto);

	// 只能放在背包的道具
	InitContainerLimit();

// 	// 初始化宠物属性默认最大最小值
// 	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitPetAttDefMinMax));
// 	LoadResArray(m_nPetAttDefMinMax, EPA_Begin, EPA_End - 1, FileName_PetDefMinMax_Proto);

	// Jason 2010-1-19 v1.3.2 玩家回归
	PlayerRegression * pPlayerRegression = TPlayerRegression::Instance  ();
	if( P_VALID(pPlayerRegression) )
	{
		LoadResToOtherObject(pPlayerRegression,TMemFun2<void,PlayerRegression,VarContainer * ,LPCTSTR >( &PlayerRegression::AddRule ),FileName_Player_Regression_Rule);
		pPlayerRegression->Initial  ();
	}

	LoadResToOtherObject(&g_OnlineRewardParam,TMemFun2<void,OnlieRewardParam,VarContainer * ,LPCTSTR >( &OnlieRewardParam::IniParam ),FileName_Online_Reward_Param);


	TObjRef<VarContainer> pVar = "VarContainer";
	m_dwChatChannelFlag	=	pVar->GetDword(_T("channelflag chat_log"));

	// Jason 2010-4-13 v2.0.0
	//if( g_world.IsFabaoOpen() )
	{
		SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFabaoLevelProto));
		LoadResMap(m_mapFabaoLevelProto, FileName_FabaoLevelProto);
		SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFabaoWuxingProto));
		LoadResMap(m_mapFabaoWuxingProto, FileName_FabaoWuxingProto);
		SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFabaoProto));
		LoadResMap(m_mapFabaoProto, FileName_FabaoProto);
	}

	if( g_world.IsSoaringOpen() )
	{
		SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneLearnSoaringSkillProto));
		LoadResMap(m_mapLearnSoaringSkill, FileName_LearnSoaringSkill);
		SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneCastingProto));
		LoadResMap(m_mapCastingProto, FileName_CastingProto);
	}

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMsgDeliverProto));
	LoadResMap(m_mapMsgDeliverProto, FileName_MsgDeliverProto);

	// 加载圣灵属性[wcy 2012-03-30]
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneHolyProto));
	LoadResMap(m_mapHolyProto, FileName_HolyProto);

	// 加载圣灵升级表[wcy 2012-04-09]
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneHolyLevelUp));
	LoadResMap(m_mapHolyLevelUp, FileName_HolyLevelUp);

	// 加载圣纹附加属性[wcy 2012-04-17]
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneHolyEquipProto));
	LoadResMap(m_mapHolyEquipProto, FileName_HolyEquipProto);

	// 加载圣纹强化属性[wcy 2012-04-26]
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneHolyEquipEnhance));
	LoadResMap(m_mapHolyEquipEnhance, FileName_HolyEquipEnhance);
	
	// Jason 2010-5-11 v2.1.0 FileName_QuestCompleteFuzhu 
	//SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneQuestUnimpededToken));
	//LoadResMap(m_mapQuestUnimpededToken, FileName_QuestCompleteFuzhu);

	// 加载任务板任务库
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneBoardQuest));
	LoadResMap(m_mapBoardQuestProto, FileName_BoardQuestProto);

	// 加载任务板任务生成规则
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneBoardQuestRandRule));
	LoadResArray(m_BoardQuestRandRule, 0, BOARD_QUEST_MAX_RULE_NUM - 1, FileName_BoardQuestRandRate);

	// Jason 2010-6-18 v1.0.0
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFConsolidateProto));
	LoadResMap(m_mapFConsolidateProto, FileName_FConsolidateProto);
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFConsolidateLuckySignPct));
	LoadResMap(m_mapFConsolidateLuckySignPct,FileName_FConsolidateLuckySignPct);
	//FileName_FFuhunStone
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFAttachSoulProto));
	LoadResMap(m_mapFAttachSoulStone,FileName_FFuhunStone);
	// FileName_GodStrengthenProto 神铸
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneGodStrengthenProto));
	LoadResMap(m_mapGodStrengthen,FileName_GodStrengthenProto);

	// 一元计划原型表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMallOfferProto));
	LoadResMap(m_mapMallOfferProto,FileName_MallOfferProto);

	LoadResToOtherObject( &ConvenientTeam::GetInstance(),TMemFun2<void,ConvenientTeam,VarContainer * ,LPCTSTR >( &ConvenientTeam::Init ),FileName_GroupScene );

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitFabaoFoodProto));
	LoadResMap(m_mapFabaoFood,FileName_FabaoFood);
	// 妖精果实FileName_FabaoFruit
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitFabaoFruitProto));
	LoadResMap(m_mapFabaoFruit,FileName_FabaoFruit);

	// 妖精特技FileName_FabaoStunt
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitFabaoStuntProto));
	LoadResMap(m_mapFabaoStunt,FileName_FabaoStunt);

	// 妖精点数提升 [3/13/2012 zhangzhihua]
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitFabaoAddPromoteTimesProto));
	LoadResMap(m_mapFabaoAddPromoteTimes,FileName_FabaoPromote);

	// 妖精融合 [3/22/2012 zhangzhihua]
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitFairyFusionQualityProto));
	LoadResMap(m_mapFairyFusionQuality,FileName_FairyFusion);
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitFairyFusionTypeProto));
	LoadResMap(m_mapFairyFusionType,FileName_FairyFusionType);

	// 守护地图奖励
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitWarRewardPointProto));
	LoadResMap(m_mapWarRewardPoint,FileName_WarRewardPoint);

	// 公会战每日奖励
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitGuildDailyRewardProto));
	LoadResMap(m_mapGuildDailyReward,FileName_GuildDailyReward);

	// 神之竞标
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitGodBidProto));
	LoadResMap(m_mapGodBidProto,FileName_GodBidProto);

	// 妖精繁殖相关
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFabaoBirthProto));
	LoadResMap(m_mapFabaoBirthProto,FileName_FabaoBirth);
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFabaoBirthItemTypeProto));
	LoadResMap(m_mapFabaoBirthItemTypeProto,FileName_FabaoBirth_Type);

	// 海外筛选器
	LoadResToOtherObject( ShieldingMgr::Create(),TMemFun2<void,ShieldingMgr,VarContainer * ,LPCTSTR >( &ShieldingMgr::Init ),FileName_Shielding );
	// 砸金蛋
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneGoldEggProto));
	LoadResMap(m_mapGoldEggProto,FileName_Gold_Egg_Proto);
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneGoldEggPctProto));
	LoadResMap(m_mapGoldEggPctProto,FileName_Gold_Egg_Pct_Proto);

	// 衣橱系统
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneWardrobeProto));
	LoadResMap(m_mapWardrobeProto,FileName_Wardrobe_Proto);
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneAdvancedComposeProto));
	LoadResMap(m_mapAdvancedComposeProto,FileName_Advanced_Compose_Proto);

	//加载神职属性表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneClergyProto));
	LoadResMap(m_mapClergyProto, FileName_Clergy);

	// 加载神格信息表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneGodHeadProto));
	LoadResMap(m_mapGodHeadProto, FileName_GodHead);

	//加载神职领域属性表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneGodAreaProto));
	LoadResMap(m_mapGodAreaProto, FileName_GodArea);

	//加载地图盒子属性表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMapBoxProto));
	LoadResMap(m_mapMapBoxProto, FileName_MapBox);

	//加载领域特效属性表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneAreaEffectProto));
	LoadResMap(m_mapAreaEffectProto, FileName_AreaEffect);

	m_mapAreaEffectProto.ResetIterator();
	tagAreaEffectProto* pTempProto = NULL;
	while (m_mapAreaEffectProto.PeekNext(pTempProto))
	{
		m_mapALLAreaBuffID.Add(pTempProto->dwBuffID, pTempProto->dwBuffID);
	}

	//加载守护妖精属性表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFamilySpriteProto));
	LoadResMap(m_mapFamilySpriteProto, FileName_Sprite);

	//加载家族守护妖精修炼属性表
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSpriteTrainProto));
	LoadResMap(m_mapSpriteTrainProto, FileName_SpriteTraining);

	// KeyCode领取奖励
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneKeyCodeRewardProto));
	LoadResMap(m_mapKeyCodeRewardProto, FileName_KeyCodeReward);

	// 家族任务完成条件属性表	
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFamilyQuestCondition));
	LoadResMap(m_mapFamilyQuestCondition, FileName_FamilyQuestCondition);

	// 家族任务属性表	
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFamilyQuestProto));
	LoadResMap(m_mapFamilyQuests, FileName_FamilyQuest);

	// 家族任务完成条件属性表	
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneFamilyRewardProto));
	LoadResMap(m_mapFamilyRewardProto, FileName_FamilyQuestRewardProto);

	// 属性
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneIdbuybackProto));
	LoadResMap(m_mapIdbuybackProto, FileName_Idbuyback);
	
	// 经脉
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOnePulseProto));
	LoadResMap(m_mapPulseProto,FileName_Pulse_Proto);
	
	// 装备成长
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneEquipGrowProto));
	LoadResMap(m_mapEquipGrowProto, FileName_EquipGrowProto);

	// 经脉
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneTigerProto));
	LoadResMap(m_mapTigerProto,FileName_Tiger_Proto);

	//离线经验
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneExpOfflineProto));
	LoadResMap(m_mapExpOfflineProto,FileName_ExpOffline_Proto);

	// 神之战灵技能
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneForceSkillProto));
	LoadResMap(m_mapForceSkillProto,FileName_ForceSkill_Proto);

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneForceLevelProto));
	LoadResMap(m_mapForceLevelProto,FileName_ForceLevel_Proto);

	// 魂晶
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSoulCrystalLvlLine));
	LoadResMap(m_mapSoulCrystalLvlLine, FileName_SoulCrystalLvlLine_Proto);

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneSoulCrystalAttData));
	LoadResMap(m_mapSoulCrystalAttData, FileName_SoulCrystalAtt_Proto);

	KillObj("AttResContainer");

	return TRUE;
}

//-----------------------------------------------------------------------------
// 释放所有资源空间
//-----------------------------------------------------------------------------
VOID AttRes::Destroy()
{
	FreeResMap(m_mapItemProto);
	FreeResMap(m_mapEquipProto);
	FreeResMap(m_mapGemProto);
	FreeResMap(m_mapEquipQltyPct);
	FreeResMap(m_mapSoulAttProto);
	FreeResMap(m_mapStarEnhanceProto);
	FreeResMap(m_mapSoulRebornProto);
	FreeResMap(m_mapSoulEquipHoleProto);
	FreeResMap(m_mapSoulEquipInlayProto);
	FreeResMap(m_mapLongHunProto);

	FreeResMap(m_mapShopProto);
	FreeResMap(m_mapDakProto);
	FreeResMap(m_mapSuitProto);
    
	FreeResMap(m_mapMallRecommendingItemInfo);
	FreeResMap(m_mapMallItemProto);
	FreeResMap(m_mapMallPackProto);

    FreeResMap(m_mapSkillProto);
	FreeResMap(m_mapTriggerProto);
	FreeResMap(m_mapBuffProto);
	FreeResMap(m_mapSkillModify);

	FreeResMap(m_mapCreatureProto);
	FreeResMap(m_mapCreatureAI);
	FreeResMap(m_mapCreatureAIGroup);
	FreeResMap(m_mapPosyProto);
	FreeResMap(m_mapEngraveProto);
	FreeResMap(m_mapConsolidateProto);
	FreeResMap(m_mapProduceProto);
	FreeResMap(m_mapQuenchProto);
	FreeResMap(m_mapDeComposeProto);

	FreeResMap(m_mapSpawnPointProto);
	FreeResMap(m_mapInstanceProto);
	FreeResMap(m_mapInstanceItem);
	FreeResMap(m_mapInstanceSkill);
	FreeResMap(m_mapLevelMapping);
	FreeResMap(m_mapSSpawnPoint);

	FreeResMap(m_mapPetProto);
	FreeResMap(m_mapPetSkillProto);
	FreeResMap(m_mapPetEquipProto);
	FreeResMap(m_mapPetGatherProto);

	FreeResMap(m_GuildUpgradeNeedInfo);
	FreeResMap(m_GuildAffairInfo);
	FreeResMap(m_GuildSkillInfo);
	//天资花费表
	FreeResMap(m_mapTalentAvailCost);
    FreeResMap(m_mapMapBuffInfo);

	// Jason 2009-12-7 神龙赐福
	FreeResMap  (m_mapConfigInfoOfLessingLoong);
	FreeResMap  (m_mapPhilosophersStoneConfig);

	FreeResMap(m_mapCityProto);
	FreeResMap(m_mapSpinRateProto);

	// 龙之试炼系统-试炼属性表
	FreeResMap(m_mapTrainAttProto);
	FreeResMap(m_mapMasterApprenticeProto);

	if(P_VALID(m_TitleProto)) delete []m_TitleProto;

	// Jason 2010-4-15 v2.0.0
	//if( g_world.IsFabaoOpen() )
	{
		FreeResMap( m_mapFabaoLevelProto );
		FreeResMap( m_mapFabaoWuxingProto );
		FreeResMap( m_mapFabaoProto );
	}

	FreeResMap(m_mapMotionProto);
	FreeResMap(m_mapKillObjExp);
	FreeResMap(m_mapQuestUseSpeciItemProto);
	FreeResMap(m_mapVNBEquipProto);
	FreeResMap(m_mapVNBGiftProto);
	FreeResMap(m_mapPetWuXingProto);
	FreeResMap(m_mapPetLvlUpItemProto);
	FreeResMap(m_mapPetLvlUpProto);
	FreeResMap(m_CofCSPProto);
	FreeResMap(m_CofCProto);
	FreeResMap(m_GuildCommodityProto);
	FreeResMap(m_GuildCommerceInfo);
	FreeResMap(m_mapMiracleSkillProto);
	FreeResMap(m_mapQuestUnimpededToken);

	std::list<tagSpinProto *> tempList;
	tagSpinProto *pResNode = NULL;

	m_mapSpinProto.ResetIterator();
	while(m_mapSpinProto.PeekNext(pResNode))
	{
		if(find(tempList.begin(), tempList.end(), pResNode) == tempList.end())
			tempList.push_back(pResNode);
	}

	m_mapSpinProto.Clear();
	for(std::list<tagSpinProto *>::iterator itor = tempList.begin(); itor!= tempList.end(); itor++)
		SAFE_DELETE(*itor);
	tempList.clear();
	//FreeResMap(m_mapQuestUnimpededToken);
	if( g_world.IsSoaringOpen() )
	{
		FreeResMap(m_mapLearnSoaringSkill);
		FreeResMap(m_mapCastingProto);
		FreeResMap(m_mapMsgDeliverProto);
	}

	FreeResMap(m_mapSkillBoxProto);

	FreeResMap(m_mapBoardQuestProto);

	FreeResMap(m_mapFConsolidateProto);
	FreeResMap(m_mapFConsolidateLuckySignPct);
	FreeResMap(m_mapFAttachSoulStone);
	FreeResMap(m_mapInitialSkill);

	FreeResMap(m_mapGodStrengthen);

	FreeResMap(m_mapFabaoFood);
	FreeResMap(m_mapFabaoFruit);
	FreeResMap(m_mapFabaoStunt);
	FreeResMap(m_mapFabaoAddPromoteTimes);
	FreeResMap(m_mapFairyFusionQuality);
	FreeResMap(m_mapFairyFusionType);
	FreeResMap(m_mapWarRewardPoint);

	FreeResMap(m_mapGuildDailyReward);

	FreeResMap(m_mapGodBidProto);

	FreeResMap(m_mapFabaoBirthProto);
	FreeResMap(m_mapFabaoBirthItemTypeProto);
	FreeResMap(m_mapWardrobeProto);
	FreeResMap(m_mapAdvancedComposeProto);
	FreeResMap(m_mapGoldEggProto);
	FreeResMap(m_mapGoldEggPctProto);

	FreeResMap(m_mapClergyProto);
	FreeResMap(m_mapGodHeadProto);
	FreeResMap(m_mapGodAreaProto);
	FreeResMap(m_mapMapBoxProto);
	FreeResMap(m_mapAreaEffectProto);

	FreeResMap(m_mapFamilySpriteProto);
	FreeResMap(m_mapSpriteTrainProto);
	FreeResMap(m_mapPulseProto);
	FreeResMap(m_mapTigerProto);

	TMap<DWORD, tagSpriteTrainData*>::TMapIterator it = m_mapSpriteTrainData.Begin();
	tagSpriteTrainData* pSpriteTrainData = NULL;
	while( m_mapSpriteTrainData.PeekNext(it, pSpriteTrainData) )
	{
		SAFE_DEL(pSpriteTrainData);
	}
	m_mapSpriteTrainData.Clear();

	FreeResMap(m_mapIdbuybackProto);

	FreeResMap(m_mapExpOfflineProto);

	//释放圣灵资源
	FreeResMap(m_mapHolyLevelUp);
	FreeResMap(m_mapHolyEquipProto);
	FreeResMap(m_mapHolyEquipEnhance);
	FreeResMap(m_mapHolyProto);
	
}

//-------------------------------------------------------------------------------------------
// 初始化过滤词表
//-------------------------------------------------------------------------------------------
BOOL AttRes::InitFilterWords(OUT std::vector<tstring>& vectFilterWords, LPCTSTR szFileName)
{
	vectFilterWords.clear();

	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	// 读入文件
	if(!m_pVar->Load(NULL, szFileName,"id", &ListField))
	{
		MAttResCaution(szFileName, _T("load"), GT_INVALID);
		return FALSE;
	}

	// 按顺序读取
	for(it = ListField.begin(); it != ListField.end(); ++it)
	{
		vectFilterWords.push_back(m_pVar->GetString(_T("name"), it->c_str()));
	}

	// 清空容器
	m_pVar->Clear();

	return TRUE;
}

BOOL AttRes::InitChatFilterWords(OUT vector<tstring>& vectFilterWords, LPCTSTR szFileName)
{
	vectFilterWords.clear();

	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	// 读入文件
	if(!m_pVar->Load(NULL, szFileName,"id", &ListField))
	{
		MAttResCaution(szFileName, _T("load"), GT_INVALID);
		return FALSE;
	}

	// 按顺序读取
	for(it = ListField.begin(); it != ListField.end(); ++it)
	{
		vectFilterWords.push_back(m_pVar->GetString(_T("item"), it->c_str()));
	}

	// 清空容器
	m_pVar->Clear();

	return TRUE;
}

//-------------------------------------------------------------------------------------------
// 从脚本管理器中获取变量长度
//-------------------------------------------------------------------------------------------
VOID AttRes::InitVarLen()
{
	// 角色名字
	g_ScriptMgr.GetGlobal("len_RoleNameMax", m_VarLen.nRoleNameMax);
	g_ScriptMgr.GetGlobal("len_RoleNameMin", m_VarLen.nRoleNameMin);
	
	// 帮派相关
	g_ScriptMgr.GetGlobal("len_GuildNameMax", m_VarLen.nGuildNameMax);
	g_ScriptMgr.GetGlobal("len_GuildNameMin", m_VarLen.nGuildNameMin);
	g_ScriptMgr.GetGlobal("len_GuildTenet", m_VarLen.nGuildTenet);

	// 摆摊
	g_ScriptMgr.GetGlobal("len_StallTitleMax", m_VarLen.nStallTitleMax);


	// 简单验证设置合法性
	if(!m_VarLen.CheckValid())
	{
		ASSERT(0);
		IMSG(_T("\n\n\tName length script maybe have problem!\n\n"));
	}
}

// 重新加载神之竞标数据
BOOL AttRes::ReloadGodBidProto()
{
	FreeResMap(m_mapGodBidProto);

	CreateObj("AttResContainer", "VarContainer");
	m_pVar = "AttResContainer";

	// 神之竞标
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitGodBidProto));
	LoadResMap(m_mapGodBidProto,FileName_GodBidProto);

	KillObj("AttResContainer");

	return TRUE;
}

// 重新加载神之竞标数据
BOOL AttRes::ReLoadGoldEggPctProto()
{
	FreeResMap(m_mapGoldEggPctProto);

	CreateObj("AttResContainer", "VarContainer");
	m_pVar = "AttResContainer";

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneGoldEggPctProto));
	LoadResMap(m_mapGoldEggPctProto,FileName_Gold_Egg_Pct_Proto);

	KillObj("AttResContainer");

	return TRUE;
}

//-----------------------------------------------------------------------------
// 重新加载商城数据
//-----------------------------------------------------------------------------
BOOL AttRes::ReloadMallProto()
{
	FreeResMap(m_mapMallItemProto);
	FreeResMap(m_mapMallPackProto);
	FreeResMap(m_mapMallRecommendingItemInfo);
	m_MallFreeItemProto.Clear();
	//m_mapMallRecommendingItemInfo.Clear();

	CreateObj("AttResContainer", "VarContainer");
	m_pVar = "AttResContainer";

	// 加载商城
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMallItemProto));
	if(!LoadResMap(m_mapMallItemProto, FileName_MallItem))
	{
		//return FALSE;
		ASSERT(0);
	}

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMallPackProto));
	if(!LoadResMap(m_mapMallPackProto, FileName_MallPack))
	{
		//return FALSE;
		ASSERT(0);
	}

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMallFreeItemProto));
	if(!LoadResArray(&m_MallFreeItemProto, 1, 1, FileName_MallFreeItem))
	{
		//return FALSE;
		ASSERT(0);
	}
    
	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMallRecommendingItemInfo));
	if(!LoadResMap(m_mapMallRecommendingItemInfo, FilaName_RecommendingItem))
	{
		//return FALSE;
		ASSERT(0);
	}

	SetInitOneRes((Fun_p)m_Trunk.sfp3(&AttRes::InitOneMallCfgProto));
	if(!LoadResArray(m_dwMallCfg, 0, EMCA_End - 1, FilaName_MallCfg))
	{
		//return FALSE;
		ASSERT(0);
	}

	KillObj("AttResContainer");

	return TRUE;
}

//-----------------------------------------------------------------------------
// 加载人物升级静态数据
//-----------------------------------------------------------------------------
VOID AttRes::InitRoleAttLevelUp(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	tagLevelUpEffect* pLevelUpEffect = (tagLevelUpEffect*)pArray;

	pLevelUpEffect[nIndex].nExpLevelUp					= m_pVar->GetInt64(_T("ExpLevelUp"), szField);
	pLevelUpEffect[nIndex].n16RoleAtt[ERA_Physique]		= (INT16)m_pVar->GetDword(_T("Physique"), szField, 0);
	pLevelUpEffect[nIndex].n16RoleAtt[ERA_Strength]		= (INT16)m_pVar->GetDword(_T("Strength"), szField, 0);
	pLevelUpEffect[nIndex].n16RoleAtt[ERA_Pneuma]		= (INT16)m_pVar->GetDword(_T("Pneuma"), szField, 0);
	pLevelUpEffect[nIndex].n16RoleAtt[ERA_InnerForce]	= (INT16)m_pVar->GetDword(_T("InnerForce"), szField, 0);
	pLevelUpEffect[nIndex].n16RoleAtt[ERA_Agility]		= (INT16)m_pVar->GetDword(_T("Agility"), szField, 0);
	pLevelUpEffect[nIndex].n16RoleAtt[ERA_Technique]	= (INT16)m_pVar->GetDword(_T("Technique"), szField, 0);
	pLevelUpEffect[nIndex].n16RoleAttAvail				= (INT16)m_pVar->GetDword(_T("RoleAttAvail_e"), szField, 0);
	pLevelUpEffect[nIndex].n16TalentAvail				= (INT16)m_pVar->GetDword(_T("TalentAvail_e"), szField, 0);
	pLevelUpEffect[nIndex].n16TalentAvail_limit			= (INT16)m_pVar->GetDword(_T("TalentAvail_limit"), szField, 0); 
	pLevelUpEffect[nIndex].n16HP						= (INT16)m_pVar->GetDword(_T("HP"), szField);
	pLevelUpEffect[nIndex].n16MP						= (INT16)m_pVar->GetDword(_T("MP"), szField);
	pLevelUpEffect[nIndex].n16Vitality					= (INT16)m_pVar->GetDword(_T("Vitality"), szField);
	pLevelUpEffect[nIndex].n16Durance					= (INT16)m_pVar->GetDword(_T("durance"),szField);
}

//-------------------------------------------------------------------------------------------
// 加载武勋属性表数据
//-------------------------------------------------------------------------------------------
VOID AttRes::IniWuXunProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
    tagWuXunProto* pWuXunProto   =  (tagWuXunProto*)pArray;
	pWuXunProto[nIndex].n16WuXunLevel				= (INT16)m_pVar->GetDword(_T("Level"), szField,0);
	pWuXunProto[nIndex].unWuXunVal		            = (UINT)m_pVar->GetDword(_T("WuXun"), szField, 0);
	pWuXunProto[nIndex].unMaxWuJiVal		        = (UINT)m_pVar->GetDword(_T("WuJi_Max"), szField, 0);
}

VOID AttRes::IniServerFunctionSwitch(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	BitSetServerFunctionSwitch* pBs = (BitSetServerFunctionSwitch*)pArray;
	BYTE byId		= (BYTE)m_pVar->GetDword(_T("id"), szField, 0);

	pBs->set(byId, (BOOL)m_pVar->GetDword(_T("switch"), szField, FALSE) ? true : false);
}

//-------------------------------------------------------------------------------------
// 加载属性默认值、最小及最大值
//-------------------------------------------------------------------------------------
VOID AttRes::InitAttDefMinMax(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	tagAttDefMinMax* pAttDefMinMax = (tagAttDefMinMax*)pArray;

	pAttDefMinMax[nIndex].nDefRole			=	m_pVar->GetInt(_T("def_role"), szField, 0);
	pAttDefMinMax[nIndex].nDefCreature		=	m_pVar->GetInt(_T("def_creature"), szField, 0);
	pAttDefMinMax[nIndex].nMin				=	m_pVar->GetInt(_T("min"), szField, INT_MIN);
	pAttDefMinMax[nIndex].nMax				=	m_pVar->GetInt(_T("max"), szField, INT_MAX);
}

//-------------------------------------------------------------------------------------------
// 加载购买天资的相应花费数据
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitTalentAvailCost(OUT LPVOID pData, IN LPCTSTR szField, INT32 nDummy)
{
	tagTalentAvailCost* pTalentAvailCost = (tagTalentAvailCost*)pData;

	pTalentAvailCost->nTalentSequenceNum       =  (INT)m_pVar->GetInt(_T("id"), szField, 0);
	pTalentAvailCost->dwCostSilver             = (DWORD)m_pVar->GetDword(_T("cost"), szField, 0);
	return &pTalentAvailCost->nTalentSequenceNum;
}

//-------------------------------------------------------------------------------------------
// 加载能进指定地图的buff数据
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitMapBuffInfo(OUT LPVOID pData, IN LPCTSTR szField, INT32 nDummy)
{
      tagMapBuffInfo* pMapBuffInfo = (tagMapBuffInfo*)pData;  
      
	   
      pMapBuffInfo->dwMapID = TObjRef<Util>()->Crc32(m_pVar->GetString(_T("map_id"), szField, 0));
	  for( INT i = 0 ; i < MAPBUFF_MAXCOUNT ; i++)
	  {
		  TCHAR szStr[36] = {0};
		  memset(szStr,0,sizeof(szStr));
		  wsprintf(szStr,_T("buff_id%d"),i+1);
		  DWORD dwTempBuffID = (DWORD)m_pVar->GetDword(szStr, szField, 0);
		  if( 0 != dwTempBuffID )
		  {
             pMapBuffInfo->dwBuffID[i] = dwTempBuffID ;
		  }
		  else
		  {
		     pMapBuffInfo->dwBuffID[i] = 0 ;
		  }
		  

	  }
	  return  &pMapBuffInfo->dwMapID;
	  


}

//-------------------------------------------------------------------------------------
// 初始化物品的一条记录
//-------------------------------------------------------------------------------------
VOID* AttRes::InitOneItemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(pItem, pProtoType, tagItemProto);

	// 初始化结构信息
	pItem->dwTypeID					=	(DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	pItem->eType					=	(EItemType)m_pVar->GetInt(_T("Type"), szField, EIT_Null);
	pItem->eTypeEx					=	(EItemTypeEx)m_pVar->GetInt(_T("TypeEx"), szField, EITE_Null);
	pItem->eTypeReserved			=	(EItemTypeReserved)m_pVar->GetInt(_T("TypeReserved"), szField, EITR_Null);

	pItem->eStuffType				=	(EStuffType)m_pVar->GetInt(_T("StuffType"), szField, EST_Null);

	pItem->eSpecFunc				=	(EItemSpecFunc)m_pVar->GetInt(_T("SpecFunc"), szField, EISF_Null);
	pItem->nSpecFuncVal1			=	m_pVar->GetInt(_T("SpecFuncVal1"), szField, 0);
	pItem->nSpecFuncVal2			=	m_pVar->GetInt(_T("SpecFuncVal2"), szField, 0);

	pItem->byLevel					=	(BYTE)m_pVar->GetDword(_T("Level"), szField, 1);
	pItem->byQuality				=	(BYTE)m_pVar->GetDword(_T("Quality"), szField, EIQ_White);
	pItem->byBindType				=	(BYTE)m_pVar->GetDword(_T("BindType"), szField, EBM_Null);
	pItem->dwQuestID				=	m_pVar->GetDword(_T("QuestID"), szField, GT_INVALID);
	pItem->nBasePrice				=	m_pVar->GetInt(_T("BasePrice"), szField, 0);
	pItem->nMaxUseTimes				=	m_pVar->GetInt(_T("MaxUseTimes"), szField, 1);
	pItem->n16MaxLapNum				=	(INT16)m_pVar->GetDword(_T("MaxLapNum"), szField, 1);
	pItem->n16MaxHoldNum			=	(INT16)m_pVar->GetDword(_T("MaxHoldNum"), szField, GT_INVALID);

	pItem->dwTimeLimit				=	m_pVar->GetDword(_T("TimeLimit"), szField, GT_INVALID);

	pItem->n16Enmity				=	(INT16)m_pVar->GetDword(_T("Enmity"), szField, 0);
	pItem->bNeedBroadcast			=	((BOOL)m_pVar->GetDword(_T("NeedBroadcast"), szField, FALSE) ? true : false);
	pItem->bCanSell					=	((BOOL)m_pVar->GetDword(_T("CanSell"), szField, TRUE) ? true : false);

	pItem->byMinUseLevel			=	(BYTE)m_pVar->GetDword(_T("MinUseLevel"), szField, 1);
	pItem->byMaxUseLevel			=	(BYTE)m_pVar->GetDword(_T("MaxUseLevel"), szField, 255);
	pItem->eSexLimit				=	(ESexLimit)m_pVar->GetInt(_T("SexLimit"), szField, ESL_Woman);

	// 职业限制//??
	pItem->dwVocationLimit			=	m_pVar->GetDword(_T("VocationLimit"), szField, GT_INVALID);

	pItem->eClanRepute				=	(EReputationType)m_pVar->GetInt(_T("ZoneRepType"), szField, ERT_NULL);
	pItem->nClanReputeVal			=	m_pVar->GetInt(_T("ZoneRepVal"), szField, 0);
	pItem->eOtherClanRepute				=	(EReputationType)m_pVar->GetInt(_T("OtherRepType"), szField, ERT_NULL);
	pItem->nOtherClanReputeVal			=	m_pVar->GetInt(_T("OtherRepVal"), szField, 0);

	pItem->eOPType					=	(ESkillOPType)m_pVar->GetInt(_T("OpType"), szField, ESOPT_NUll);
	pItem->fOPDist					=	m_pVar->GetFloat(_T("OpDist"), szField, 0);
	pItem->fOPRadius				=	m_pVar->GetFloat(_T("OpRaduis"), szField, 0);
	pItem->bMoveable				=	(BOOL)m_pVar->GetDword(_T("moveable"), szField, FALSE);
	pItem->nInterruptSkillOrdRate	=	m_pVar->GetInt(_T("InterruptSkillOrdRate"), szField, 0);
	pItem->nPrepareTime				=	m_pVar->GetInt(_T("PrepareTime"), szField, 0);
	pItem->dwTargetLimit			=	m_pVar->GetDword(_T("TargetLimit"), szField, GT_INVALID);
	pItem->dwSpecBuffLimitID		=	m_pVar->GetDword(_T("BuffLimitID"), szField, GT_INVALID);
	pItem->dwStateLimit				=	m_pVar->GetDword(_T("StateLimit"), szField, GT_INVALID);

	pItem->bInterruptMove			=	(BOOL)m_pVar->GetDword(_T("InterruptMove"), szField, FALSE) ? true : false;
	//pItem->bFriendly				=	(BOOL)m_pVar->GetDword(_T("Friendly"), szField, FALSE) ? true : false;
	//pItem->bHostile					=	(BOOL)m_pVar->GetDword(_T("Hostile"), szField, FALSE) ? true : false;
	//pItem->bIndependent				=	(BOOL)m_pVar->GetDword(_T("Independent"), szField, FALSE) ? true : false;

	pItem->dwCooldownTime			=	m_pVar->GetDword(_T("CooldownTime"), szField, 0);

	pItem->dwTriggerID0				=	m_pVar->GetDword(_T("TriggerID0"), szField, GT_INVALID);
	pItem->dwBuffID0				=	m_pVar->GetDword(_T("BuffID0"), szField, GT_INVALID);
	
	pItem->dwTriggerID1				=	m_pVar->GetDword(_T("TriggerID1"), szField, GT_INVALID);
	pItem->dwBuffID1				=	m_pVar->GetDword(_T("BuffID1"), szField, GT_INVALID);

	pItem->dwTransTriggerID			=	m_pVar->GetDword(_T("TransTriggerID"), szField, GT_INVALID);
	pItem->dwTransID				=	m_pVar->GetDword(_T("TransID"), szField, GT_INVALID);

	pItem->bDeadLoot				=	(BOOL)m_pVar->GetDword(_T("DeadLoot"), szField, FALSE) ? true : false;
	pItem->DiscardEvent				=	(EDiscardEvent)m_pVar->GetInt(_T("DiscardEvent"), szField, EDE_DiscardDestory);

	pItem->bSoarLimit				=	m_pVar->GetDword(_T("soar_limit"),szField,FALSE);

	pItem->bCanMove2Ware			=	m_pVar->GetDword(_T("can_save"),szField,0);	// 0-可以存，1-不可以存

	// 是否为稀有，是的话记录
// 	if (pItem->eSpecFunc == EISF_GlobalLimitedItem)
// 	{
// 		m_vecLimitedItem.push_back(pItem->dwTypeID);
// 	}

	// 添加新属性
	return &pItem->dwTypeID;
}

//-----------------------------------------------------------------------------
// 初始化装备的一条记录
//-----------------------------------------------------------------------------
VOID* AttRes::InitOneEquipProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	// 先初始化和物品静态属性相同的部分
	InitOneItemProto(pProtoType, szField, nDummy);

	// 初始化装备静态属性特有部分
	MTRANS_POINTER(pEquip, pProtoType, tagEquipProto);

	ASSERT(pEquip->dwTypeID >= MIN_EQUIP_ID);

	pEquip->dwSuitID[0]				=	m_pVar->GetDword(_T("SuitID1"), szField, GT_INVALID);
	pEquip->dwSuitID[1]				=	m_pVar->GetDword(_T("SuitID2"), szField, GT_INVALID);
	pEquip->dwSuitID[2]				=	m_pVar->GetDword(_T("SuitID3"), szField, GT_INVALID);

	pEquip->bySuitMinQlty[0]		=	(BYTE)m_pVar->GetDword(_T("SuitMinQlty1"), szField, EIQ_White);
	pEquip->bySuitMinQlty[1]		=	(BYTE)m_pVar->GetDword(_T("SuitMinQlty2"), szField, EIQ_White);
	pEquip->bySuitMinQlty[2]		=	(BYTE)m_pVar->GetDword(_T("SuitMinQlty3"), szField, EIQ_White);

	pEquip->n16AttALimit[0]			=	(INT16)m_pVar->GetInt(_T("PhysiqueLim"), szField, 0);
	pEquip->n16AttALimit[1]			=	(INT16)m_pVar->GetInt(_T("StrengthLim"), szField, 0);
	pEquip->n16AttALimit[2]			=	(INT16)m_pVar->GetInt(_T("PneumaLim"), szField, 0);
	pEquip->n16AttALimit[3]			=	(INT16)m_pVar->GetInt(_T("InnerForceLim"), szField, 0);
	pEquip->n16AttALimit[4]			=	(INT16)m_pVar->GetInt(_T("TechniqueLim"), szField, 0);
	pEquip->n16AttALimit[5]			=	(INT16)m_pVar->GetInt(_T("AgilityLim"), szField, 0);

    pEquip->eEquipPos				=	(EEquipPos)m_pVar->GetInt(_T("EquipPos"), szField);
    pEquip->eWeaponPos				=	(EWeaponPos)m_pVar->GetInt(_T("WeaponPos"), szField, EWP_NULL);

	pEquip->n16WuHun				=	(INT16)m_pVar->GetDword(_T("WuHun"), szField, 0);
	pEquip->n16MinDmg				=	(INT16)m_pVar->GetDword(_T("MinDmg"), szField, 0);
	pEquip->n16MaxDmg				=	(INT16)m_pVar->GetDword(_T("MaxDmg"), szField, 0);
	pEquip->n16Armor				=	(INT16)m_pVar->GetDword(_T("Armor"), szField, 0);

	pEquip->nPotVal					=	m_pVar->GetInt(_T("Potval"), szField, 0);
	pEquip->nMaxPotVal				=	m_pVar->GetInt(_T("MaxPotval"), szField, 0);
	
	pEquip->n16Newness				=	(INT16)m_pVar->GetDword(_T("MaxNewness"), szField, 0);

	pEquip->dwVocationLimitWear		=	m_pVar->GetInt(_T("VocationLimitWear"), szField, GT_INVALID);

	// Jason 2009-12-1 装备潜力值提升次数
	pEquip->nPotIncTimes			=	m_pVar->GetInt(_T("PotIncTimes"), szField, 0);

	TCHAR szTmp[8];

	// 加工前，影响的属性(和品级无关)
	for(INT32 i=0; i<MAX_ROLEATT_BASE_EFFECT; ++i)
	{
		_stprintf(szTmp, _T("%d"), i);
		pEquip->BaseEffect[i].eRoleAtt	= (ERoleAttribute)m_pVar->GetInt(_T("RoleAttType"), szTmp, szField, ERA_Null);
		if(ERA_Null == pEquip->BaseEffect[i].eRoleAtt)
		{
			break;
		}

		pEquip->BaseEffect[i].nValue	=	m_pVar->GetInt(_T("RoleAttVal"), szTmp, szField, 0);
	}

	// 龙魂属性,即五行
	pEquip->byLongfuAtt[0]			=	(BYTE)m_pVar->GetDword(_T("Metal"), szField, 0);
	pEquip->byLongfuAtt[1]			=	(BYTE)m_pVar->GetDword(_T("Wood"), szField, 0);
	pEquip->byLongfuAtt[2]			=	(BYTE)m_pVar->GetDword(_T("Water"), szField, 0);
	pEquip->byLongfuAtt[3]			=	(BYTE)m_pVar->GetDword(_T("Fire"), szField, 0);
	pEquip->byLongfuAtt[4]			=	(BYTE)m_pVar->GetDword(_T("Soil"), szField, 0);

	pEquip->dwTriggerID2			=	m_pVar->GetDword(_T("TriggerID2"), szField, GT_INVALID);
	pEquip->dwBuffID2				=	m_pVar->GetDword(_T("BuffID2"), szField, GT_INVALID);

	pEquip->bCanDye					=	((BOOL)m_pVar->GetDword(_T("CanDye"), szField, FALSE) ? true : false);
	pEquip->dwColor					=	m_pVar->GetDword(_T("Corlor"), szField, EC_Null);
	pEquip->bCanGrow					= m_pVar->GetDword(_T("CanGrow"), szField, FALSE) ? true :false;

	pEquip->bIdentifyProduct		=	((BOOL)m_pVar->GetDword(_T("IdentifyProduct"), szField, FALSE) ? true : false);
	pEquip->bIdentifyLoot			=	((BOOL)m_pVar->GetDword(_T("IdentifyLoot"), szField, FALSE) ? true : false);

	pEquip->nInitConsolidateLevel	= m_pVar->GetInt( _T("streng_level"),szField,0 );
	pEquip->nMaxConsolidateLevel	= m_pVar->GetInt( _T("streng_level_limite"),szField,30 );

	pEquip->nBornLevelOfGodStrengthen = m_pVar->GetInt( _T("soar_bornlevel"),szField,-1 );

	pEquip->bSuckLimit				= (bool)m_pVar->GetInt(_T("suck_limit"),szField,false);

	pEquip->bOnlyScriptStrengthen				= (bool)m_pVar->GetInt(_T("OnlyScriptStrengthen"),szField,false);

	pEquip->dwCondenseLimit			= (DWORD)m_pVar->GetDword(_T("godhead_limit"), szField, 0);

	pEquip->bMapLimit				= (bool)m_pVar->GetInt(_T("MapLimit"), szField, false);

	return &pEquip->dwTypeID;
}

//-----------------------------------------------------------------------------
// 初始化宝石等的一条记录
//-----------------------------------------------------------------------------
VOID* AttRes::InitOneGemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(pGemProto, pProtoType, tagGemProto);

	pGemProto->dwTypeID			= (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));

	pGemProto->n16PotValNeed	= (INT16)m_pVar->GetDword(_T("potval_consume"), szField);
	pGemProto->n16SuccessPct	= (INT16)m_pVar->GetDword(_T("success_rate"), szField);

	pGemProto->eWuXing			= (EWuXing)m_pVar->GetInt(_T("loongsoul_att"), szField, EWX_Null);
	
	TCHAR szTmp[8];

	for(INT32 i=0; i<MAX_ROLEATT_ENHANCE_EFFECT; ++i)
	{
		_stprintf(szTmp, _T("%d"), i);
		pGemProto->BaseEffect[i].eRoleAtt	= (ERoleAttribute)m_pVar->GetInt(_T("role_att"), szTmp, szField, ERA_Null);
		if(ERA_Null == pGemProto->BaseEffect[i].eRoleAtt)
		{
			break;
		}

		pGemProto->BaseEffect[i].nValue	=	m_pVar->GetInt(_T("att_val"), szTmp, szField, 0);
	}

	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("right_hand"), szField, 0) == 1 ? EEEP_Hand : 0);
	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("head"), szField, 0) == 1 ? EEEP_Head : 0);
	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("body"), szField, 0) == 1 ? EEEP_Body : 0);
	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("leg"), szField, 0) == 1 ? EEEP_Legs : 0);
	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("wrist"), szField, 0) == 1 ? EEEP_Wrist : 0);
	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("feet"), szField, 0) == 1 ? EEEP_Feet : 0);
	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("back"), szField, 0) == 1 ? EEEP_Back : 0);
	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("neck"), szField, 0) == 1 ? EEEP_Neck : 0);
	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("finger1"), szField, 0) == 1 ? EEEP_Finger : 0);
	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("waist"), szField, 0) == 1 ? EEEP_Waist : 0);
	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("face"), szField, 0) == 1 ? EEEP_Face : 0);
	pGemProto->nEnhanceFlg		|= (m_pVar->GetInt(_T("hunjing"), szField, 0) == 1 ? EEEP_HunJing : 0);

	return &pGemProto->dwTypeID;
}

//-----------------------------------------------------------------------------
// 初始化套装的一条记录
//-----------------------------------------------------------------------------
VOID* AttRes::InitOneSuitProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(pProto, pProtoType, tagSuitProto);

	pProto->dwID			= (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));

	TCHAR szTmp[8];
	for(INT32 i=0; i<MAX_SUIT_ATT_NUM; ++i)
	{
		_stprintf(szTmp, _T("%d"), i+1);
		
		pProto->dwTriggerID[i]	= m_pVar->GetDword(_T("TriggerID"), szTmp, szField, GT_INVALID);
		pProto->dwBuffID[i]		= m_pVar->GetDword(_T("BuffID"), szTmp, szField, GT_INVALID);
		pProto->n8ActiveNum[i]	= m_pVar->GetInt(_T("ActiveNum"), szTmp, szField, MAX_SUIT_EQUIP_NUM);
	}

	pProto->n8SpecEffectNum	= m_pVar->GetInt(_T("SpecEffectNum"), szField, GT_INVALID);

	return &pProto->dwID;
}

//-----------------------------------------------------------------------------
// 初始化一类装备品级鉴定几率
//-----------------------------------------------------------------------------
VOID* AttRes::InitOneEquipQltyPct(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(pEquipQltyPct, pProtoType, tagEquipQltyPct);

	pEquipQltyPct->dwTypeID						=	(DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));	// 这里干吗的？
	ASSERT(pEquipQltyPct->dwTypeID >= MIN_EQUIP_ID);

	pEquipQltyPct->nEquipQltyPct[EIQ_White]		=	m_pVar->GetInt(_T("whi_pct"), szField);
	pEquipQltyPct->nEquipQltyPct[EIQ_Yellow]	=	m_pVar->GetInt(_T("yel_pct"), szField);
	pEquipQltyPct->nEquipQltyPct[EIQ_Green]		=	m_pVar->GetInt(_T("gre_pct"), szField);
	pEquipQltyPct->nEquipQltyPct[EIQ_Blue]		=	m_pVar->GetInt(_T("blu_pct"), szField);
	pEquipQltyPct->nEquipQltyPct[EIQ_Orange]	=	m_pVar->GetInt(_T("org_pct"), szField);
	pEquipQltyPct->nEquipQltyPct[EIQ_Purple]	=	m_pVar->GetInt(_T("pur_pct"), szField,0);
	pEquipQltyPct->nEquipQltyPct[EIQ_Pink]	=	m_pVar->GetInt(_T("pin_pct"), szField,0);

	return &pEquipQltyPct->dwTypeID;
}

//-------------------------------------------------------------------------------------------
// 初始化元神基本属性表
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitOneSoulAttProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{

	MTRANS_POINTER(p, pProtoType, tagSoulAttProto);

// 	p->dwID							    =	(DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
// 	p->n64UpLvExp						=	m_pVar->GetInt64(_T("explevelup"),szField);
// 	p->n8RebornLv						=	(INT16)m_pVar->GetInt(_T("rebornlevel"),szField);
// 	p->n8SoulLevel					    =	(INT16)m_pVar->GetInt(_T("level"), szField);
// 	p->nDamage					        =	m_pVar->GetInt(_T("demage"),szField);
// 	p->nHit					            =	m_pVar->GetInt(_T("hit"),szField);
// 	p->nCrit		                    =	m_pVar->GetInt(_T("crit"),szField);
// 	p->nCritAmount						=	m_pVar->GetInt(_T("crit_amount"),szField);
// 	p->nExattack						=	m_pVar->GetInt(_T("exattack"),szField);
// 	p->nInattack						=	m_pVar->GetInt(_T("inattack"), szField);
// 	p->nAccrate							=	m_pVar->GetInt(_T("accurate"),szField);
// 	p->nExdamage						=	m_pVar->GetInt(_T("exdamage"),szField);
// 	p->nToughness			            =	m_pVar->GetInt(_T("neglect_toughness"),szField);
// 	p->n8Morale						  	=	(INT16)m_pVar->GetInt(_T("morale"),szField);
// 	p->n8Covalue						=	(INT16)m_pVar->GetDword(_T("co_value"),szField);
// 	p->dwSoulValue						=	m_pVar->GetDword(_T("max_soul"),szField);


	return &p->dwID;
}

//-------------------------------------------------------------------------------------------
// 初始化元神装备强化属性表
//-------------------------------------------------------------------------------------------
VOID*  AttRes::InitOneStarEnhanceProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(p, pProtoType, tagStarEnhanceProto);

	p->byID							    =	(BYTE)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	p->dwCostItemId						=	m_pVar->GetDword(_T("costitemid"),szField);
	p->nCostItemNum						=	(UINT16)m_pVar->GetInt(_T("costitemnum"),szField);
	p->nRate						    =	(UINT16)m_pVar->GetInt(_T("rate"), szField);
	p->byFailLevel					    =	(BYTE)m_pVar->GetInt(_T("faillevel"),szField);
	p->dwCost					        =	m_pVar->GetDword(_T("cost"),szField);
	p->nStunt		                    =	(UINT)m_pVar->GetInt(_T("stunt"),szField);
	p->nHitRate							=	(UINT)m_pVar->GetInt(_T("hitrate"),szField);
	p->nCritRate						=	(UINT)m_pVar->GetInt(_T("critrate"),szField);
	p->nCritaMount						=	(UINT)m_pVar->GetInt(_T("critamount"), szField);
	p->nExAttack						=	(UINT)m_pVar->GetInt(_T("exattack"),szField);
	p->nInAttack						=	(UINT)m_pVar->GetInt(_T("inattack"),szField);
	p->nAccuRate			            =	(UINT)m_pVar->GetInt(_T("accurate"),szField);
	p->nExDamage						=	(UINT)m_pVar->GetInt(_T("exdamage"),szField);
	p->nNegToughness					=	(UINT)m_pVar->GetDword(_T("neglecttoughness"),szField);
	p->nMorale							=	(UINT)m_pVar->GetDword(_T("morale"),szField);


	return &p->byID;
}

//-------------------------------------------------------------------------------------------
//初始化元神转生属性表
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitOneSoulRebornProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(p, pProtoType, tagHolySoulReborn);

	if (P_VALID(p))
	{
		p->byCostNum	= (BYTE)m_pVar->GetDword(_T("costitemnum"),szField);
		p->nRate1		= (INT)m_pVar->GetDword(_T("rate1"),szField);
		p->nRate2		= (INT)m_pVar->GetDword(_T("rate2"),szField);
		return &p->byCostNum;
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------
//初始化元神凿孔属性表
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitOneSoulEquipHoleProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(p, pProtoType, tagHolySoulEquipHole);

	if (P_VALID(p))
	{
		p->byID	= (BYTE)m_pVar->GetDword(_T("id"),szField);
		p->nRate		= (INT)m_pVar->GetDword(_T("rate"),szField);
		p->nCostMoney		= (INT)m_pVar->GetDword(_T("cost"),szField);
		return &p->byID;
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------
//初始化元神镶嵌属性表
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitOneSoulEquipInlayProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(p, pProtoType, tagHolySoulEquipInlay);

	if (P_VALID(p))
	{
		p->byID	= (BYTE)m_pVar->GetDword(_T("id"),szField);
		p->nRate		= (INT)m_pVar->GetDword(_T("rate"),szField);
		p->nCostMoney		= (INT)m_pVar->GetDword(_T("cost"),szField);
		return &p->byID;
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------
// 初始化龙魂能力表
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitOneLongHunProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(p, pProtoType, tagLongHunProto);

	p->dwID							=	(DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	p->eType						=	(ELongHunType)m_pVar->GetInt(_T("Type"), szField);
	p->eTypeEx						=	(ELongHunTypeEx)m_pVar->GetInt(_T("TypeEx"), szField);
	p->dwTriggerID					=	m_pVar->GetDword(_T("TriggerID"), szField);
	p->dwBuffID1					=	m_pVar->GetDword(_T("BuffID1"), szField);
	p->dwBuffID2					=	m_pVar->GetDword(_T("BuffID2"), szField, GT_INVALID);
	p->bQltyCanGen[EIQ_White]		=	m_pVar->GetInt(_T("White"), szField) == 0 ? false : true;
	p->bQltyCanGen[EIQ_Yellow]		=	m_pVar->GetInt(_T("Yellow"), szField) == 0 ? false : true;
	p->bQltyCanGen[EIQ_Green]		=	m_pVar->GetInt(_T("Green"), szField) == 0 ? false : true;
	p->bQltyCanGen[EIQ_Blue]		=	m_pVar->GetInt(_T("Blue"), szField) == 0 ? false : true;
	p->bQltyCanGen[EIQ_Orange]		=	m_pVar->GetInt(_T("Orange"), szField) == 0 ? false : true;
	p->bQltyCanGen[EIQ_Purple]		=	m_pVar->GetInt(_T("Purple"), szField,0) == 0 ? false : true;
	p->byWuXingActive[EWX_Metal]	=	(BYTE)m_pVar->GetDword(_T("Metal"), szField);
	p->byWuXingActive[EWX_Wood]		=	(BYTE)m_pVar->GetDword(_T("Wood"), szField);
	p->byWuXingActive[EWX_Water]	=	(BYTE)m_pVar->GetDword(_T("Water"), szField);
	p->byWuXingActive[EWX_Fire]		=	(BYTE)m_pVar->GetDword(_T("Fire"), szField);
	p->byWuXingActive[EWX_Soil]		=	(BYTE)m_pVar->GetDword(_T("Soil"), szField);

	return &p->dwID;
}

//-------------------------------------------------------------------------------------------
// 初始化龙魂能力辅助数组
//-------------------------------------------------------------------------------------------
VOID AttRes::InitLongHunAss()
{
	// 判断龙魂能力表是否读入
	if(m_mapLongHunProto.Size() <= 0)
	{
		MAttResCaution(FileName_LongHun, _T("Size()<=0"), GT_INVALID);
		return;
	}

	// 遍历，并生成辅助数据
	tagLongHunProto *pProto = NULL;
	TMap<DWORD, tagLongHunProto*>::TMapIterator iter = m_mapLongHunProto.Begin();

	while(m_mapLongHunProto.PeekNext(iter, pProto))
	{
		for(INT i=0; i<X_EQUIP_QUALITY_NUM; ++i)
		{
			if(pProto->bQltyCanGen[i])
			{
				m_vectLongHun[pProto->eType][pProto->eTypeEx][i].push_back(pProto->dwID);
			}
		}	
	}
}

//-----------------------------------------------------------------------------
// 根据龙魂属性由哪些部件激活数据，生成能够激活哪些部件数据
//-----------------------------------------------------------------------------
VOID AttRes::InitLongHunCanActivePos()
{
	for(INT i=0; i<EEP_MaxEquip; ++i)
	{
		if(g_sLongHunActivePos[i].eOuter != EEP_Null)
		{
			g_sLongHunCanActivePos[g_sLongHunActivePos[i].eOuter].Set((EEquipPos)i);
		}

		for(INT j=0; j<X_LONGHUN_EFFECT_POS_NUM; ++j)
		{
			if(g_sLongHunActivePos[i].eInner[j] != EEP_Null)
			{
				g_sLongHunCanActivePos[g_sLongHunActivePos[i].eInner[j]].Set((EEquipPos)i);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 加载品级装备属性参数
//-----------------------------------------------------------------------------
VOID AttRes::InitEquipQltyEffect(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	tagEquipQltyEffect* pEquipQltyEffect = (tagEquipQltyEffect*)pArray;

	pEquipQltyEffect[nIndex].fWeaponFactor			=	m_pVar->GetFloat(_T("WeaponFactor"), szField, 1);
	pEquipQltyEffect[nIndex].fArmorFactor			=	m_pVar->GetFloat(_T("ArmorFactor"), szField, 1);

	pEquipQltyEffect[nIndex].nAttAFactor			=	m_pVar->GetInt(_T("AttAFactor1"), szField, 0);
	pEquipQltyEffect[nIndex].fAttAFactor			=	m_pVar->GetFloat(_T("AttAFactor2"), szField, 0);
	pEquipQltyEffect[nIndex].nAttANumEffect			=	m_pVar->GetInt(_T("AttANumEffect"), szField, 0);

	pEquipQltyEffect[nIndex].fPotFactor				=	m_pVar->GetFloat(_T("PotFactor"), szField, 1);

	pEquipQltyEffect[nIndex].nHoleNumPct[0]			=	m_pVar->GetInt(_T("HolePct0"), szField, 0);
	pEquipQltyEffect[nIndex].nHoleNumPct[1]			=	m_pVar->GetInt(_T("HolePct1"), szField, 0);
	pEquipQltyEffect[nIndex].nHoleNumPct[2]			=	m_pVar->GetInt(_T("HolePct2"), szField, 0);
	pEquipQltyEffect[nIndex].nHoleNumPct[3]			=	m_pVar->GetInt(_T("HolePct3"), szField, 0);
	pEquipQltyEffect[nIndex].nHoleNumPct[4]			=	m_pVar->GetInt(_T("HolePct4"), szField, 0);
	pEquipQltyEffect[nIndex].nHoleNumPct[5]			=	m_pVar->GetInt(_T("HolePct5"), szField, 0);
	pEquipQltyEffect[nIndex].nHoleNumPct[6]			=	m_pVar->GetInt(_T("HolePct6"), szField, 0);
	pEquipQltyEffect[nIndex].nHoleNumPct[7]			=	m_pVar->GetInt(_T("HolePct7"), szField, 0);

	pEquipQltyEffect[nIndex].nSpecAttPct			=	m_pVar->GetInt(_T("SpecAttPct"), szField, 0);

	pEquipQltyEffect[nIndex].nLonghunPct[ELHT_Outer]=	m_pVar->GetInt(_T("LonghunPctOuter"), szField, 0);
	pEquipQltyEffect[nIndex].nLonghunPct[ELHT_Inner]=	m_pVar->GetInt(_T("LonghunPctInner"), szField, 0);
	// 增加强化等级限制
	pEquipQltyEffect[nIndex].n8StrengLevelLimit	=	m_pVar->GetInt(_T("streng_level_limit"),szField,0);
}

//-----------------------------------------------------------------------------
// 加载时装生成参数
//-----------------------------------------------------------------------------
VOID AttRes::InitFashionQltyEffect(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	MTRANS_POINTER(p, pArray, tagFashionGen);

	p[nIndex].fAppearanceFactor = m_pVar->GetFloat(_T("AppearanceFactor"), szField);
	p[nIndex].n16ReinPct		= (INT16)m_pVar->GetFloat(_T("ReinPct"), szField);
	p[nIndex].n16SavvyPct		= (INT16)m_pVar->GetFloat(_T("SavvyPct"), szField);
	p[nIndex].n16FortunePct		= (INT16)m_pVar->GetFloat(_T("FortunePct"), szField);
	p[nIndex].n8ReinVal			= (INT8)m_pVar->GetFloat(_T("ReinVal"), szField);
	p[nIndex].n8SavvyVal		= (INT8)m_pVar->GetFloat(_T("SavvyVal"), szField);
	p[nIndex].n8FortuneVal1		= (INT8)m_pVar->GetFloat(_T("FortuneVal1"), szField);
	p[nIndex].n8FortuneVal2		= (INT8)m_pVar->GetFloat(_T("FortuneVal2"), szField);
}

//-----------------------------------------------------------------------------
// 加载时装生成时颜色概率
//-----------------------------------------------------------------------------
VOID AttRes::InitFashionColorPct(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	MTRANS_POINTER(p, pArray, tagFashionColorPct);

	if(nIndex >= X_COLOR_NUM)
	{
		ASSERT(nIndex < X_COLOR_NUM);
		IMSG(_T("\n\nCaution:\n"));
		IMSG(_T("\tThere is a critical in fashion_color_pct.xml!!!!!!!!\n\n"));
		return;
	}
	
	p[EIQ_White].n16ColorPct[nIndex] = m_pVar->GetInt(_T("WhitePct"), szField);
	p[EIQ_Yellow].n16ColorPct[nIndex] = m_pVar->GetInt(_T("YellowPct"), szField);
	p[EIQ_Green].n16ColorPct[nIndex] = m_pVar->GetInt(_T("GreenPct"), szField);
	p[EIQ_Blue].n16ColorPct[nIndex] = m_pVar->GetInt(_T("BluePct"), szField);
	p[EIQ_Orange].n16ColorPct[nIndex] = m_pVar->GetInt(_T("OrangePct"), szField);
}

//-------------------------------------------------------------------------------------------
// 初始化商店
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitOneShopProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(pShop, pProtoType, tagShopProto);

	TCHAR szTmp[64];
	
	// 读取的为稀有物品原型
	if(X_READ_FILE_2 == nDummy)
	{
		for(INT32 i=0; i<MAX_SHOP_RARE_ITEM; ++i)
		{
			_stprintf(szTmp, _T("%d"), i+101);
			pShop->RareItem[i].dwTypeID = m_pVar->GetDword(_T("RareID"), szTmp, szField, GT_INVALID);

			if(GT_INVALID == pShop->RareItem[i].dwTypeID)
			{
				break;
			}

			pShop->RareItem[i].byRepLevel	= (BYTE)m_pVar->GetDword(_T("MinRepLevel"), szTmp, szField, ERL_Hostile);
			pShop->RareItem[i].nSilver		= m_pVar->GetInt(_T("Money"), szTmp, szField, 0);
			pShop->RareItem[i].nCostNum		= m_pVar->GetInt(_T("CostVal"), szTmp, szField, 0);
			pShop->RareItem[i].byShelf		= (BYTE)m_pVar->GetDword(_T("Shelf"), szTmp, szField, 0) - 1;
			ASSERT(pShop->RareItem[i].byShelf - 1 < MAX_SHOP_SHELF_NUM);

			pShop->RareItem[i].byQuality	= (BYTE)m_pVar->GetDword(_T("Qlty"), szTmp, szField, EIQ_White);
			/*ASSERT((CHAR)pShop->RareItem[i].byQuality > EIQ_Start && (CHAR)pShop->RareItem[i].byQuality < EIQ_End);*/

			pShop->RareItem[i].byRefreshNum	= (BYTE)m_pVar->GetDword(_T("RefNum"), szTmp, szField, 0);
			pShop->RareItem[i].dwRefreshTime = m_pVar->GetDword(_T("RefTime"), szTmp, szField, GT_INVALID);

			if(pShop->RareItem[i].nSilver < 0 || pShop->RareItem[i].nCostNum < 0)
			{
				ASSERT(0);
				IMSG(_T("\n\nCritical Error In Shop Proto!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n"));
			}
		}

		return &pShop->dwID;
	}

	// 普通物品原型
	pShop->dwID			= (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));

	pShop->eCostType	= (ECurCostType)m_pVar->GetInt(_T("CostType"), szField, ECCT_Null);
	pShop->dwItemTypeID	= m_pVar->GetDword(_T("CostItemID"), szField, GT_INVALID);
	pShop->bEquip		= m_pVar->GetDword(_T("IsEquip"), szField);
	//tbc:
	pShop->bClanTreasury= m_pVar->GetDword(_T("IsClanTreasury"), szField, 0);

	//税收商店 对应的城市
	pShop->dwCityID	= m_pVar->GetDword(_T("CityID"), szField, GT_INVALID);

	for(INT32 i=0; i<MAX_SHOP_SHELF_NUM; ++i)
	{
		_stprintf(szTmp, _T("ItemNum%d"), i+1);
		pShop->n16Num[i] = (INT16)m_pVar->GetDword(szTmp, szField);
	}

	for(INT32 i=0; i<MAX_SHOP_COMMON_ITEM; ++i)
	{
		_stprintf(szTmp, _T("%d"), i+1);
		pShop->Item[i].dwTypeID = m_pVar->GetDword(_T("ItemID"), szTmp, szField, GT_INVALID);

		if(GT_INVALID == pShop->Item[i].dwTypeID)
		{
			break;
		}

		pShop->Item[i].byRepLevel	= (BYTE)m_pVar->GetDword(_T("MinRepLevel"), szTmp, szField, ERL_Hostile);
		pShop->Item[i].nSilver		= m_pVar->GetInt(_T("Money"), szTmp, szField, 0);
		pShop->Item[i].nCostNum		= m_pVar->GetInt(_T("CostVal"), szTmp, szField, 0);
		pShop->Item[i].byShelf		= (BYTE)m_pVar->GetDword(_T("Shelf"), szTmp, szField, 1) - 1;

		if(pShop->Item[i].nSilver < 0 || pShop->Item[i].nCostNum < 0
			|| pShop->Item[i].byShelf - 1 >= MAX_SHOP_SHELF_NUM)
		{
			ASSERT(0);
			ASSERT(pShop->Item[i].byShelf - 1 < MAX_SHOP_SHELF_NUM);
			MAttResCaution(_T("shop"), _T("shopid"), pShop->dwID);
		}
	}

	return &pShop->dwID;
}

//-------------------------------------------------------------------------------------------
// 初始化商场商品及礼品包
//-------------------------------------------------------------------------------------------
VOID AttRes::InitOneMallItemProtoBase(OUT LPVOID pProtoType, IN LPCTSTR szField)
{
	MTRANS_POINTER(p, pProtoType, tagMallItemProtoBase);

	p->dwID						= (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));

	p->nPrice					= m_pVar->GetInt(_T("purchase_price"), szField);
	p->nSalePrice				= m_pVar->GetInt(_T("promote_price"), szField, GT_INVALID);
	p->dwPresentID				= m_pVar->GetDword(_T("present_id"), szField, 0);
	p->nNum						= m_pVar->GetInt(_T("sale_num"), szField, GT_INVALID);
	p->nPresentNum				= m_pVar->GetInt(_T("present_num"), szField, 0);
	p->nExAssign				= m_pVar->GetInt(_T("exchange_volume_assign"), szField, 0);
	p->nExNum					= m_pVar->GetInt(_T("exchange_volume_num"), szField, GT_INVALID);
	p->bNew						= m_pVar->GetInt(_T("new_goods"), szField, 0);
	p->bHot						= m_pVar->GetInt(_T("hot_goods"), szField, 0);
	p->bySmallGroupHeadcount	= (BYTE)m_pVar->GetDword(_T("min_group_headcount"), szField, GT_INVALID);
	p->bySmallGroupDiscount		= (BYTE)m_pVar->GetDword(_T("min_group_rebate"), szField, GT_INVALID);
	p->byMediumGroupHeadcount	= (BYTE)m_pVar->GetDword(_T("mid_group_headcount"), szField, GT_INVALID);
	p->byMediumGroupDiscount	= (BYTE)m_pVar->GetDword(_T("mid_group_rebate"), szField, GT_INVALID);
	p->byBigGroupHeadcount		= (BYTE)m_pVar->GetDword(_T("max_group_headcount"), szField, GT_INVALID);
	p->byBigGroupDiscount		= (BYTE)m_pVar->GetDword(_T("max_group_rebate"), szField, GT_INVALID);
	p->dwPersistTime			= m_pVar->GetDword(_T("persist_time"), szField, GT_INVALID);


	p->dwTimeSaleEnd			= GT_INVALID;
	if(m_pVar->GetDword(_T("promote_hour"), szField) != GT_INVALID)
	{
		p->dwTimeSaleEnd		= tagDWORDTime(0, 0, (BYTE)m_pVar->GetDword(_T("promote_hour"), szField), 
												(BYTE)m_pVar->GetDword(_T("promote_day"), szField), 
												(BYTE)m_pVar->GetDword(_T("promote_month"), szField), 
												(BYTE)m_pVar->GetDword(_T("promote_year"), szField));
	}

	// 检查
	if(p->nPrice <= 0)
	{
		IMSG(_T("\n\nCaution:\n"));
		IMSG(_T("There is a critical error in mall<id: %u>!!!!!!!!!!!!!\n\n"), p->dwID);
		ASSERT(0);
	}
	
	// 额外处理
	if(GT_INVALID == p->nSalePrice)
	{
		p->nSalePrice = p->nPrice;
	}
}

VOID* AttRes::InitOneMallItemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	InitOneMallItemProtoBase(pProtoType, szField);
	
	MTRANS_POINTER(p, pProtoType, tagMallItemProto);

	p->dwTypeID	= m_pVar->GetDword(_T("typeid"), szField);
	p->n8Kind	= (INT8)m_pVar->GetInt(_T("type"), szField);
	p->n8Kind2	= (BYTE)m_pVar->GetInt(_T("type2"), szField,0);
	p->byRank	= (BYTE)m_pVar->GetDword(_T("pro_goods_sort"), szField, 0);
	p->byGroupPurchaseAmount = (BYTE)m_pVar->GetDword(_T("group_purchase_amount"), szField, 1);

	return &p->dwID;
}

VOID* AttRes::InitOneMallPackProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	InitOneMallItemProtoBase(pProtoType, szField);

	MTRANS_POINTER(p, pProtoType, tagMallPackProto);

	TCHAR szTmp[32];
	INT i = 0;
	for(; i<MALL_PACK_ITEM_NUM; ++i)
	{
		_stprintf(szTmp, _T("%d"), i+1);
		p->dwTypeID[i]	= m_pVar->GetDword(_T("item_id"), szTmp, szField, GT_INVALID);
		if(GT_INVALID == p->dwTypeID[i])
		{
			break;
		}

		p->byItemNum[i]		= m_pVar->GetDword(_T("item_num"), szTmp, szField, 0);
		p->nItemPrice[i]	= m_pVar->GetInt(_T("item_price"), szTmp, szField, 0);

	}

	p->n8ItemKind = i;
	ZeroMemory(p->tszPackName, sizeof(p->tszPackName));
	_tcsncpy(p->tszPackName, m_pVar->GetString(_T("name"), szField, _T("")), Max_PackName_Length-1);
	p->byDummy[0] = (BYTE)m_pVar->GetInt(_T("index"), szField, 0);

	// 检查
	if(p->n8ItemKind <= 0)
	{
		IMSG(_T("\n\nCaution:\n"));
		IMSG(_T("There is a critical error in mall<id: %u>!!!!!!!!!!!!!\n\n"), p->dwID);
		ASSERT(p->n8ItemKind > 0);
	}

	return &p->dwID;
}
//-------------------------------------------------------------------------------------------
// 初始化推荐物品
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitOneMallRecommendingItemInfo(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
     MTRANS_POINTER(p, pProtoType, tagRecommendingItemInfo);
     p->dwID           = (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	 p->n16MinLevel    = (UINT16)m_pVar->GetInt(_T("MinLevel"), szField,0);
     p->n16MaxLevel    = (UINT16)m_pVar->GetInt(_T("MaxLevel"), szField,0);
  
     p->dwItem1TypeID  = m_pVar->GetDword(_T("Item1"), szField, GT_INVALID);
	 p->dwItem2TypeID  = m_pVar->GetDword(_T("Item2"), szField, GT_INVALID);
	 p->dwItem3TypeID  = m_pVar->GetDword(_T("Item3"), szField, GT_INVALID);
	 p->dwItem4TypeID  = m_pVar->GetDword(_T("Item4"), szField, GT_INVALID);
	 p->dwItem5TypeID  = m_pVar->GetDword(_T("Item5"), szField, GT_INVALID);

	 p->dwItem6TypeID  = m_pVar->GetDword(_T("Item6"), szField, GT_INVALID);
	 p->dwItem7TypeID  = m_pVar->GetDword(_T("Item7"), szField, GT_INVALID);
	 p->dwItem8TypeID  = m_pVar->GetDword(_T("Item8"), szField, GT_INVALID);
	 p->dwItem9TypeID  = m_pVar->GetDword(_T("Item9"), szField, GT_INVALID);
	 p->dwItem10TypeID = m_pVar->GetDword(_T("Item10"), szField,GT_INVALID); 
	 return &p->dwID;
}

//-------------------------------------------------------------------------------------------
// 初始化驿站
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitOneDakProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(pDak, pProtoType, tagDakProto);

	pDak->dwID			= (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));

	TCHAR szTmp[8];
	for(INT32 i=0; i<MAX_DAK_SITE_NUM; ++i)
	{
		_stprintf(szTmp, _T("%d"), i+1);
		
		LPCTSTR szMapName = m_pVar->GetString(_T("map"), szTmp, szField, NULL);
		if(NULL == szMapName)
		{
			pDak->dakSite[i].dwMapID = GT_INVALID;
			break;
		}

		pDak->dakSite[i].dwMapID	= m_pUtil->Crc32(szMapName);

		pDak->dakSite[i].fX			= m_pVar->GetFloat(_T("x_map"), szTmp, szField, 0.0f);
		pDak->dakSite[i].fY			= m_pVar->GetFloat(_T("y_map"), szTmp, szField, 0.0f);
		pDak->dakSite[i].fZ			= m_pVar->GetFloat(_T("z_map"), szTmp, szField, 0.0f);
		pDak->dakSite[i].eCostType	= (ECurCostType)m_pVar->GetInt(_T("cost_type_map"), szTmp, szField, ECCT_Null);
		pDak->dakSite[i].nCostVal	= m_pVar->GetInt(_T("cost_num_map"), szTmp, szField, 0);
	}

	return &pDak->dwID;
}

//-----------------------------------------------------------------------------
// 根据TypeID重新设置是否记录log
//-----------------------------------------------------------------------------
VOID AttRes::ResetItemLog(DWORD dwTypeID[], INT32 nNum)
{
	ASSERT(nNum >= 0);
	ASSERT(m_mapItemProto.Size() != 0);
	ASSERT(m_mapEquipProto.Size() != 0);

	tagItemProto	*pItem;
	tagEquipProto	*pEquip;
	if(0 == nNum)
	{
		m_mapItemProto.ResetIterator();
		while(m_mapItemProto.PeekNext(pItem))
		{
			pItem->bNeedLog = TRUE;
		}

		m_mapEquipProto.ResetIterator();
		while(m_mapEquipProto.PeekNext(pEquip))
		{
			pEquip->bNeedLog = TRUE;
		}
	}

	if(nNum > 0)
	{
		for(INT32 i = 0; i < nNum; ++i)
		{
			if(MIsEquipment(dwTypeID[i]))
			{
				tagFabaoProto* pFabaoProto = m_mapFabaoProto.Peek(dwTypeID[i]);
				tagHolyProto* pHolyProto =  m_mapHolyProto.Peek(dwTypeID[i]);
				tagHolyEquipProto* pHolyEquipProto =  m_mapHolyEquipProto.Peek(dwTypeID[i]);
				if( P_VALID(pFabaoProto) )
					pItem = pFabaoProto;
				else if (P_VALID(pHolyProto) )
				{
					pItem = pHolyProto;
				}
				else if (P_VALID(pHolyEquipProto))
				{
					pItem = pHolyEquipProto;
				}					 
				else
					pItem = m_mapEquipProto.Peek(dwTypeID[i]);
			}
			else
			{
				pItem = m_mapItemProto.Peek(dwTypeID[i]);
			}

			if(!P_VALID(pItem))
			{
				ASSERT(P_VALID(pItem));
				IMSG(_T("Can not find item prototype in m_mapItemProto!!!!!\nPlease check <typeid: %u>!!!!\n"), dwTypeID[i]);
				continue;
			}

			pItem->bNeedLog = TRUE;
		}
	}
}


//-----------------------------------------------------------------------------
// 处理Skill中人物属性中的一条属性
//-----------------------------------------------------------------------------
VOID AddSkillRoleAtt(tagSkillProto* pSkill, ERoleAttribute eType, INT nValue)
{
	ASSERT(eType > ERA_Null && eType < ERA_End);

	if(0 == nValue)
		return;

	if(MIsValuePct(nValue))
	{
		pSkill->mapRoleAttModPct.Add(eType, MValuePctTrans(nValue));
	}
	else
	{
		pSkill->mapRoleAttMod.Add(eType, nValue);
	}
}
//-----------------------------------------------------------------------------
// 处理Buff中人物属性中的一条属性
//-----------------------------------------------------------------------------
VOID AddBuffRoleAtt(tagBuffProto* pSkill, ERoleAttribute eType, INT nValue)
{
	ASSERT(eType > ERA_Null && eType < ERA_End);

	if(0 == nValue)
		return;

	if(MIsValuePct(nValue))
	{
		pSkill->mapRoleAttModPct.Add(eType, MValuePctTrans(nValue));
	}
	else
	{
		pSkill->mapRoleAttMod.Add(eType, nValue);
	}
}

//-----------------------------------------------------------------------------
// 初始化一条Skill记录
//-----------------------------------------------------------------------------
VOID *AttRes::InitOneSkillProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(pSkill, pProtoType, tagSkillProto);

	// 初始化结构信息
	pSkill->dwID								=	(DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	pSkill->eType								=	(enum ESkillType)m_pVar->GetInt(_T("type"), szField);
	pSkill->nType2								=	m_pVar->GetInt(_T("type2"), szField);
	pSkill->nType3								=	m_pVar->GetInt(_T("type3"), szField);
	pSkill->dwMainSkillID						=	m_pVar->GetDword(_T("main_skill_id"), szField, GT_INVALID);

	// 目标类型
	pSkill->eTalentType							=	(enum ETalentType)m_pVar->GetInt(_T("talent_type"), szField);
	pSkill->eTargetType							=	(enum ESkillTargetType)m_pVar->GetInt(_T("target_type"), szField);
	pSkill->dwTargetSkillID						=	m_pVar->GetDword(_T("target_type_1"), szField, GT_INVALID);
	pSkill->dwTargetBuffID						=	m_pVar->GetDword(_T("target_type_2"), szField, GT_INVALID);
	pSkill->dwTargetTriggerID					=	m_pVar->GetDword(_T("target_type_3"), szField, GT_INVALID);

	// 攻击判定
	pSkill->eDistType							=	(enum ESkillDistType)m_pVar->GetInt(_T("dist_type"), szField);
	pSkill->eUseType							=	(enum ESkillUseType)m_pVar->GetInt(_T("use_type"), szField);
	pSkill->eOPType								=	(enum ESkillOPType)m_pVar->GetInt(_T("op_type"), szField);
	pSkill->fOPDist								=	m_pVar->GetFloat(_T("op_distance"),	szField, 0.0f);
	pSkill->fOPRadius							=	m_pVar->GetFloat(_T("op_raduis"), szField, 0.0f);
	pSkill->bInterruptMove						=	(BOOL)m_pVar->GetDword(_T("interrupt_move"), szField);
	pSkill->nInterruptSkillOrdRate				=	m_pVar->GetInt(_T("interrupt_skill_ord_rate"), szField, 0);
	pSkill->nInterruptSkillSpecRate				=	m_pVar->GetInt(_T("interrupt_skill_spec_rate"), szField, 0);
	pSkill->nPrepareTime						=	m_pVar->GetInt(_T("prepare_time"), szField);
	pSkill->nCoolDown							=	m_pVar->GetInt(_T("cooldown"), szField);
	pSkill->eDmgType							=	(enum ESkillDmgType)m_pVar->GetInt(_T("dmg_type"), szField);
	pSkill->nEnmity								=	m_pVar->GetInt(_T("enmity"), szField, 0);
	pSkill->nHit								=	m_pVar->GetInt(_T("hit_add"), szField, 0);
	pSkill->nCrit								=	m_pVar->GetInt(_T("crit_add"), szField, 0);

	// 管道
	pSkill->nChannelDmg[0]						=	m_pVar->GetInt(_T("channel_dmg1"), szField, 0);
	pSkill->nChannelDmg[1]						=	m_pVar->GetInt(_T("channel_dmg2"), szField, 0);
	pSkill->nChannelDmg[2]						=	m_pVar->GetInt(_T("channel_dmg3"), szField, 0);
	pSkill->nChannelDmg[3]						=	m_pVar->GetInt(_T("channel_dmg4"), szField, 0);
	pSkill->nChannelDmg[4]						=	m_pVar->GetInt(_T("channel_dmg5"), szField, 0);
	pSkill->nChannelTime[0]						=	m_pVar->GetInt(_T("channel_time1"), szField, 0);
	pSkill->nChannelTime[1]						=	m_pVar->GetInt(_T("channel_time2"), szField, 0);
	pSkill->nChannelTime[2]						=	m_pVar->GetInt(_T("channel_time3"), szField, 0);
	pSkill->nChannelTime[3]						=	m_pVar->GetInt(_T("channel_time4"), szField, 0);
	pSkill->nChannelTime[4]						=	m_pVar->GetInt(_T("channel_time5"), szField, 0);

	// 状态
	pSkill->dwBuffID[0]							=	m_pVar->GetDword(_T("buff1_id"), szField, GT_INVALID);
	pSkill->dwBuffID[1]							=	m_pVar->GetDword(_T("buff2_id"), szField, GT_INVALID);
	pSkill->dwBuffID[2]							=	m_pVar->GetDword(_T("buff3_id"), szField, GT_INVALID);
	pSkill->dwTriggerID[0]						=	m_pVar->GetDword(_T("buff1_trigger"), szField, GT_INVALID);
	pSkill->dwTriggerID[1]						=	m_pVar->GetDword(_T("buff2_trigger"), szField, GT_INVALID);
	pSkill->dwTriggerID[2]						=	m_pVar->GetDword(_T("buff3_trigger"), szField, GT_INVALID);

	// 消耗
	pSkill->dwCostItemID						=	m_pVar->GetDword(_T("cost_item_id"), szField, GT_INVALID);
	pSkill->nCostItemNum						=	m_pVar->GetInt(_T("cost_item_num"), szField, 0);
	pSkill->nSkillCost[ESCT_HP]					=	m_pVar->GetInt(_T("cost_hp"), szField, 0);
	pSkill->nSkillCost[ESCT_MP]					=	m_pVar->GetInt(_T("cost_mp"), szField, 0);
	pSkill->nSkillCost[ESCT_Rage]				=	m_pVar->GetInt(_T("cost_rage"), szField, 0);
	pSkill->nSkillCost[ESCT_Endurance]			=	m_pVar->GetInt(_T("cost_durance"), szField, 0);
	pSkill->nSkillCost[ESCT_Valicity]			=	m_pVar->GetInt(_T("cost_valicity"), szField, 0);

	// 升级
	pSkill->nLevel								=	m_pVar->GetInt(_T("level"), szField);
	pSkill->eLevelUpType						=	(enum ESkillLevelUpType)m_pVar->GetInt(_T("levelup_type"), szField);
	pSkill->nLevelUpExp							=	m_pVar->GetInt(_T("exp"), szField, 0);
	pSkill->dwPreLevelSkillID					=	m_pVar->GetDword(_T("pre_skill_id"), szField, GT_INVALID);
	pSkill->nMaxLevel							=	m_pVar->GetInt(_T("max_level"), szField);
	pSkill->nMaxLearnLevel						=	m_pVar->GetInt(_T("max_learn_level"), szField);
	pSkill->nNeedRoleLevel						=	m_pVar->GetInt(_T("need_role_level"), szField, 0);
	pSkill->nNeedTalentPoint					=	m_pVar->GetInt(_T("need_talent_point"), szField, 0);
	//pSkill->eNeedClassType						=	(enum EClassType)m_pVar->GetInt(_T("career_type"), szField);

	// 学习限制
	pSkill->dwLearnVocationLimit				=	m_pVar->GetDword(_T("vocation_limit_learn"), szField, GT_INVALID);

	// 使用限制
	pSkill->nUseHPPctLimit						=	m_pVar->GetInt(_T("hp_limit"), szField, 0);
	pSkill->nUseMPPctLimit						=	m_pVar->GetInt(_T("mp_limit"), szField, 0);
	pSkill->eSexLimit							=	(enum ESkillSexLimit)m_pVar->GetInt(_T("sex_limit"), szField);
//	pSkill->eClassLimit							=	(enum EClassType)m_pVar->GetInt(_T("career_limit"), szField);
	pSkill->nWeaponLimit						=	m_pVar->GetInt(_T("weapon_limit"), szField);
	pSkill->ePosType							=	(enum ESkillPosType)m_pVar->GetInt(_T("pos_limit"), szField);
	pSkill->bMoveable							=	(BOOL)m_pVar->GetDword(_T("moveable"), szField, FALSE);
	pSkill->fActionTime							=   (FLOAT)m_pVar->GetFloat(_T("action_time"), szField, GT_INVALID);
	pSkill->dwTargetLimit						=	m_pVar->GetDword(_T("target_limit"), szField);
	pSkill->dwSelfStateLimit					=	m_pVar->GetDword(_T("self_state_limit"), szField, GT_INVALID);
	pSkill->dwTargetStateLimit					=	m_pVar->GetDword(_T("target_state_limit"), szField, GT_INVALID);
	pSkill->bInterCombat						=	(BOOL)m_pVar->GetDword(_T("inter_combat"), szField);
	pSkill->dwBuffLimitID						=	m_pVar->GetDword(_T("buff_limit_id"), szField, GT_INVALID);
	pSkill->dwTargetBuffLimitID					=	m_pVar->GetDword(_T("target_buff_limit_id"), szField, GT_INVALID);
	pSkill->dwVocationLimit						=	m_pVar->GetDword(_T("career_limit"), szField, GT_INVALID);
	// Jason 2010-5-18 v2.1.2
	pSkill->bSoarLimitLearn						=	m_pVar->GetDword(_T("soar_limit_learn"),szField,FALSE);

	pSkill->bFriendly							=	(BOOL)m_pVar->GetDword(_T("friendly"), szField, FALSE);
	pSkill->bHostile							=	(BOOL)m_pVar->GetDword(_T("hostile"), szField, FALSE);
	pSkill->bIndependent						=	(BOOL)m_pVar->GetDword(_T("independent"), szField, FALSE);

	// 触发器加成
	pSkill->nTriggerEventMisc1Add				=	m_pVar->GetInt(_T("trigger_event_misc1_add"), szField, 0);
	pSkill->nTriggerEventMisc2Add				=	m_pVar->GetInt(_T("trigger_event_misc2_add"), szField, 0);
	pSkill->nTriggerStateMisc1Add				=	m_pVar->GetInt(_T("trigger_state_misc1_add"), szField, 0);
	pSkill->nTriggerStateMisc2Add				=	m_pVar->GetInt(_T("trigger_state_misc2_add"), szField, 0);
	pSkill->nTriggerPropAdd						=	m_pVar->GetInt(_T("trigger_prop_add"), szField, 0);

	// 状态加成
	pSkill->nBuffPersistTimeAdd					=	m_pVar->GetInt(_T("buff_time_add"), szField, 0);
	pSkill->nBuffWarpTimesAdd					=	m_pVar->GetInt(_T("buff_wrap_times_add"), szField, 0);
	pSkill->nBuffInterruptResistAdd				=	m_pVar->GetInt(_T("buff_interupt_resist_add"), szField, 0);
	pSkill->eModBuffMode						=	(enum EBuffEffectMode)m_pVar->GetInt(_T("buff_effect_mode"), szField, EBEM_Null);
	pSkill->nBuffMisc1Add						=	m_pVar->GetInt(_T("misc1_add"), szField, 0);
	pSkill->nBuffMisc2Add						=	m_pVar->GetInt(_T("misc2_add"), szField, 0);
	pSkill->nBuffEffectAttMod[EBEA_HP]			=	m_pVar->GetInt(_T("buff_hp"), szField, 0);
	pSkill->nBuffEffectAttMod[EBEA_MP]			=	m_pVar->GetInt(_T("buff_mp"), szField, 0);
	pSkill->nBuffEffectAttMod[EBEA_Rage]		=	m_pVar->GetInt(_T("buff_rage"), szField, 0);
	pSkill->nBuffEffectAttMod[EBEA_Vitality]	=	m_pVar->GetInt(_T("buff_vitality"), szField, 0);
	pSkill->nBuffEffectAttMod[EBEA_Endurance]	=	m_pVar->GetInt(_T("buff_endurance"), szField, 0);
	pSkill->nBuffEffectAttMod[EBEA_Morale]		=	m_pVar->GetInt(_T("buff_morale"), szField, 0);
	pSkill->nBuffEffectAttMod[EBEA_Injury]		=	m_pVar->GetInt(_T("buff_injury"), szField, 0);

	pSkill->eSpecFunc							=   (ESkillSpecFunc)m_pVar->GetInt(_T("spec_func"), szField, 0);
	pSkill->nSpecFuncVal1						=   m_pVar->GetInt(_T("SpecFuncVal1"), szField, 0);
	pSkill->nSpecFuncVal2						=	m_pVar->GetInt(_T("SpecFuncVal2"), szField, 0);

	// 人物属性加成
	INT nERAPhysique							=	m_pVar->GetInt(_T("ERA_Physique"), szField, 0);
	INT nERAStrength							=	m_pVar->GetInt(_T("ERA_Strength"), szField, 0);
	INT nERAPneuma								=	m_pVar->GetInt(_T("ERA_Pneuma"), szField, 0);
	INT nERAInnerForce							=	m_pVar->GetInt(_T("ERA_InnerForce"), szField, 0);
	INT nERATechnique							=	m_pVar->GetInt(_T("ERA_Technique"), szField, 0);
	INT nERAAgility								=	m_pVar->GetInt(_T("ERA_Agility"), szField, 0);
	INT nERAMaxHP								=	m_pVar->GetInt(_T("ERA_MaxHP"), szField, 0);
	INT nERAMaxMP								=	m_pVar->GetInt(_T("ERA_MaxMP"), szField, 0);
	INT nERAExAttack							=	m_pVar->GetInt(_T("ERA_ExAttack"), szField, 0);
	INT nERAExDefense							=	m_pVar->GetInt(_T("ERA_ExDefense"), szField, 0);
	INT nERAInAttack							=	m_pVar->GetInt(_T("ERA_InAttack"), szField, 0);
	INT nERAInDefense							=	m_pVar->GetInt(_T("ERA_InDefense"), szField, 0);
	INT nERAAttackTec							=	m_pVar->GetInt(_T("ERA_AttackTec"), szField, 0);
	INT nERADefenseTec							=	m_pVar->GetInt(_T("ERA_DefenseTec"), szField, 0);
	INT nERAHitRate								=	m_pVar->GetInt(_T("ERA_HitRate"), szField, 0);
	INT nERADodge								=	m_pVar->GetInt(_T("ERA_Dodge"), szField, 0);
	INT nERAMaxEndurance						=	m_pVar->GetInt(_T("ERA_MaxEndurance"), szField, 0);
	INT nERAHPRegainRate						=	m_pVar->GetInt(_T("ERA_HPRegainRate"), szField, 0);
	INT nERAMPRegainRate						=	m_pVar->GetInt(_T("ERA_MPRegainRate"), szField, 0);
	INT nERAMaxVitality							=	m_pVar->GetInt(_T("ERA_MaxVitality"), szField, 0);
	INT nERAVitalityRegainRate					=	m_pVar->GetInt(_T("ERA_VitalityRegainRate"), szField, 0);
	INT nERASpeed_XZ							=	m_pVar->GetInt(_T("ERA_Speed_XZ"), szField, 0);
	INT nERASpeed_Y								=	m_pVar->GetInt(_T("ERA_Speed_Y"), szField, 0);
	INT nERASpeedSwim							=	m_pVar->GetInt(_T("ERA_Speed_Swin"), szField, 0);
	INT nERASpeedMount							=	m_pVar->GetInt(_T("ERA_Speed_Mount"), szField, 0);
	INT nERAWeaponDmgMin						=	m_pVar->GetInt(_T("ERA_WeaponDmgMin"), szField, 0);
	INT nERAWeaponDmgMax						=	m_pVar->GetInt(_T("ERA_WeaponDmgMax"), szField, 0);
	INT nERAWeaponSoul							=	m_pVar->GetInt(_T("ERA_WeaponSoul"), szField, 0);
	INT nERAArmor								=	m_pVar->GetInt(_T("ERA_Armor"), szField, 0);
	INT nERADerate_Ordinary						=	m_pVar->GetInt(_T("ERA_Derate_Ordinary"), szField, 0);
	INT nERADerate_Bleeding						=	m_pVar->GetInt(_T("ERA_Derate_Bleeding"), szField, 0);
	INT nERADerate_Brunt						=	m_pVar->GetInt(_T("ERA_Derate_Brunt"), szField, 0);
	INT nERADerate_Bang							=	m_pVar->GetInt(_T("ERA_Derate_Bang"), szField, 0);
	INT nERADerate_Poison						=	m_pVar->GetInt(_T("ERA_Derate_Poison"), szField, 0);
	INT nERADerate_Thinker						=	m_pVar->GetInt(_T("ERA_Derate_Thinker"), szField, 0);
	INT nERADerate_Injury						=	m_pVar->GetInt(_T("ERA_Derate_Injury"), szField, 0);
	INT nERADerate_Stunt						=	m_pVar->GetInt(_T("ERA_Derate_Stunt"), szField, 0);
	INT nERADerate_ExAttack						=	m_pVar->GetInt(_T("ERA_Derate_ExAttack"), szField, 0);
	INT nERADerate_InAttack						=	m_pVar->GetInt(_T("ERA_Derate_InAttack"), szField, 0);
	INT nERADerate_All							=	m_pVar->GetInt(_T("ERA_Derate_All"), szField, 0);
	INT nERAExDamage							=	m_pVar->GetInt(_T("ERA_ExDamage"), szField, 0);
	INT nERAExDamage_Absorb						=	m_pVar->GetInt(_T("ERA_ExDamage_Absorb"), szField, 0);
	INT nERAResist_Bleeding						=	m_pVar->GetInt(_T("ERA_Resist_Bleeding"), szField, 0);
	INT nERAResist_Weak							=	m_pVar->GetInt(_T("ERA_Resist_Weak"), szField, 0);
	INT nERAResist_Choas						=	m_pVar->GetInt(_T("ERA_Resist_Choas"), szField, 0);
	INT nERAResist_Special						=	m_pVar->GetInt(_T("ERA_Resist_Special"), szField, 0);
	INT nERARegain_Addtion						=	m_pVar->GetInt(_T("ERA_Regain_Addtion"), szField, 0);
	INT nERAAttack_MissRate						=	m_pVar->GetInt(_T("ERA_Attack_MissRate"), szField, 0);
	INT nERACloseAttack_DodgeRate				=	m_pVar->GetInt(_T("ERA_CloseAttack_DodgeRate"), szField, 0);
	INT nERARemoteAttack_DodgeRate				=	m_pVar->GetInt(_T("ERA_RemoteAttack_DodgeRate"), szField, 0);
	INT nERACrit_Rate							=	m_pVar->GetInt(_T("ERA_Crit_Rate"), szField, 0);
	INT nERACrit_Amount							=	m_pVar->GetInt(_T("ERA_Crit_Amount"), szField, 0);
	INT nERABlock_Rate							=	m_pVar->GetInt(_T("ERA_Block_Rate"), szField, 0);
	INT nERAInspiration							=	m_pVar->GetInt(_T("ERA_Inspiration"), szField, 0);
	INT nERALurk								=	m_pVar->GetInt(_T("ERA_Lurk"), szField, 0);
	INT nERASavvy								=	m_pVar->GetInt(_T("ERA_Savvy"), szField, 0);
	INT nERAEnmity_Degree						=	m_pVar->GetInt(_T("ERA_Enmity_Degree"), szField, 0);
	INT nERAShape								=	m_pVar->GetInt(_T("ERA_Shape"), szField, 0);
	INT nERAExp_Add_Rate						=	m_pVar->GetInt(_T("ERA_Exp_Add_Rate"), szField, 0);
	INT nERAMoney_Add_Rate						=	m_pVar->GetInt(_T("ERA_Money_Add_Rate"), szField, 0);
	INT nERALoot_Add_Rate						=	m_pVar->GetInt(_T("ERA_Loot_Add_Rate"), szField, 0);
	INT nERAFortune								=	m_pVar->GetInt(_T("ERA_Fortune"), szField, 0);
	INT nERAAppearance							=	m_pVar->GetInt(_T("ERA_Appearance"), szField, 0);
	INT nERARein								=	m_pVar->GetInt(_T("ERA_Rein"), szField, 0);
	INT nToughness								=	m_pVar->GetInt(_T("ERA_Toughness"), szField, 0);
	INT nERAExAt								=	m_pVar->GetInt(_T("ERA_transform_ExAttack"), szField, 0);
	INT nERAInAt								=	m_pVar->GetInt(_T("ERA_transform_InAttack"), szField, 0);
	INT nERAAll									=	m_pVar->GetInt(_T("ERA_transform_ALL"), szField, 0);
	INT nInstanceWearyEffect				= m_pVar->GetInt( _T("ERA_InstanceWearyEffect"),szField,0 );
	INT nERAMana_combustion_resistance				= m_pVar->GetInt(_T("ERA_Mana_combustion_resistance"),szField,0 );
	INT nERAToughness_god									= m_pVar->GetInt(_T("ERA_Toughness_god"),szField,0 );
	INT nERAToughness_strength								= m_pVar->GetInt(_T("ERA_Toughness_strength"),szField,0 );
	INT nERAContrel_effect_deepen						= m_pVar->GetInt(_T("ERA_Contrel_effect_deepen"),szField,0 );
	INT nERAFixed_damage_aggrandizement			= m_pVar->GetInt(_T("ERA_Fixed_damage_aggrandizement"),szField,0 );
	INT nERAAging_effect_strengthening				= m_pVar->GetInt(_T("ERA_Aging_effect_strengthening"),szField,0 );
	INT nERASlowing_effect_strengthening				= m_pVar->GetInt(_T("ERA_Slowing_effect_strengthening"),szField,0 );
	INT nERAPhysical_damage_rebound_num 		= m_pVar->GetInt(_T("ERA_Physical_damage_rebound_num"),szField,0 );
	INT nERAPhysical_damage_rebound_ratio 		= m_pVar->GetInt(_T("ERA_Physical_damage_rebound_ratio"),szField,0 );
	INT nERAMagic_damage_rebound_num			= m_pVar->GetInt(_T("ERA_Magic_damage_rebound_num"),szField,0 );
	INT nERAMagic_damage_rebound_ratio			= m_pVar->GetInt(_T("ERA_Magic_damage_rebound_ratio"),szField,0 );
	INT nERAPhysical_damage_rebound_immune	= m_pVar->GetInt(_T("ERA_Physical_damage_rebound_immune"),szField,0 );
	INT nERAMagic_damage_rebound_immune		= m_pVar->GetInt(_T("ERA_Magic_damage_rebound_immune"),szField,0 );
	INT nERAMana_combustion								= m_pVar->GetInt(_T("ERA_Mana_combustion"),szField,0 );

	INT nERANoseeattack							=	m_pVar->GetInt(_T("ERA_Nosee_attack"), szField, 0);
	INT nERANoseedefend							=	m_pVar->GetInt(_T("ERA_Nosee_defend"), szField, 0);

	INT nERATransformBang						= m_pVar->GetInt(_T("ERA_transform_Bang"), szField, 0);
	INT nERATransformBrunt						= m_pVar->GetInt(_T("ERA_transform_Brunt"), szField, 0);
	INT nERATransformInjury						= m_pVar->GetInt(_T("ERA_transform_Injury"), szField, 0);
	INT nERATransformThinker					= m_pVar->GetInt(_T("ERA_transform_Thinker"), szField, 0);

	INT nElement_penetrating					= m_pVar->GetInt(_T("Element_penetrating"), szField, 0);
	INT nResistance_elements					= m_pVar->GetInt(_T("Resistance_elements"), szField, 0);
	INT nERAHitRateAdd							= m_pVar->GetInt(_T("ERA_HitRateAdd"), szField, 0);

	AddSkillRoleAtt(pSkill, ERA_Physique, nERAPhysique);
	AddSkillRoleAtt(pSkill, ERA_Strength, nERAStrength);
	AddSkillRoleAtt(pSkill, ERA_Pneuma, nERAPneuma);
	AddSkillRoleAtt(pSkill, ERA_InnerForce, nERAInnerForce);
	AddSkillRoleAtt(pSkill, ERA_Technique, nERATechnique);
	AddSkillRoleAtt(pSkill, ERA_Agility, nERAAgility);
	AddSkillRoleAtt(pSkill, ERA_MaxHP, nERAMaxHP);
	AddSkillRoleAtt(pSkill, ERA_MaxMP, nERAMaxMP);
	AddSkillRoleAtt(pSkill, ERA_ExAttack, nERAExAttack);
	AddSkillRoleAtt(pSkill, ERA_ExDefense, nERAExDefense);
	AddSkillRoleAtt(pSkill, ERA_InAttack, nERAInAttack);
	AddSkillRoleAtt(pSkill, ERA_InDefense, nERAInDefense);
	AddSkillRoleAtt(pSkill, ERA_AttackTec, nERAAttackTec);
	AddSkillRoleAtt(pSkill, ERA_DefenseTec, nERADefenseTec);
	AddSkillRoleAtt(pSkill, ERA_HitRate, nERAHitRate);
	AddSkillRoleAtt(pSkill, ERA_Dodge, nERADodge);
	AddSkillRoleAtt(pSkill, ERA_MaxEndurance, nERAMaxEndurance);
	AddSkillRoleAtt(pSkill, ERA_HPRegainRate, nERAHPRegainRate);
	AddSkillRoleAtt(pSkill, ERA_MPRegainRate, nERAMPRegainRate);
	AddSkillRoleAtt(pSkill, ERA_MaxVitality, nERAMaxVitality);
	AddSkillRoleAtt(pSkill, ERA_VitalityRegainRate, nERAVitalityRegainRate);
	AddSkillRoleAtt(pSkill, ERA_Speed_XZ, nERASpeed_XZ);
	AddSkillRoleAtt(pSkill, ERA_Speed_Y, nERASpeed_Y);
	AddSkillRoleAtt(pSkill, ERA_Speed_Swim, nERASpeedSwim);
	AddSkillRoleAtt(pSkill, ERA_Speed_Mount, nERASpeedMount);
	AddSkillRoleAtt(pSkill, ERA_WeaponDmgMin, nERAWeaponDmgMin);
	AddSkillRoleAtt(pSkill, ERA_WeaponDmgMax, nERAWeaponDmgMax);
	AddSkillRoleAtt(pSkill, ERA_WeaponSoul, nERAWeaponSoul);
	AddSkillRoleAtt(pSkill, ERA_Armor, nERAArmor);
	AddSkillRoleAtt(pSkill, ERA_Derate_Ordinary, nERADerate_Ordinary);
	AddSkillRoleAtt(pSkill, ERA_Derate_Bleeding, nERADerate_Bleeding);
	AddSkillRoleAtt(pSkill, ERA_Derate_Brunt, nERADerate_Brunt);
	AddSkillRoleAtt(pSkill, ERA_Derate_Bang, nERADerate_Bang);
	AddSkillRoleAtt(pSkill, ERA_Derate_Poison, nERADerate_Poison);
	AddSkillRoleAtt(pSkill, ERA_Derate_Thinker, nERADerate_Thinker);
	AddSkillRoleAtt(pSkill, ERA_Derate_Injury, nERADerate_Injury);
	AddSkillRoleAtt(pSkill, ERA_Derate_Stunt, nERADerate_Stunt);
	AddSkillRoleAtt(pSkill, ERA_Derate_ExAttack, nERADerate_ExAttack);
	AddSkillRoleAtt(pSkill, ERA_Derate_InAttack, nERADerate_InAttack);
	AddSkillRoleAtt(pSkill, ERA_Derate_ALL, nERADerate_All);
	AddSkillRoleAtt(pSkill, ERA_ExDamage, nERAExDamage);
	AddSkillRoleAtt(pSkill, ERA_ExDamage_Absorb, nERAExDamage_Absorb);	
	AddSkillRoleAtt(pSkill, ERA_Resist_Bleeding, nERAResist_Bleeding);	
	AddSkillRoleAtt(pSkill, ERA_Resist_Weak, nERAResist_Weak);	
	AddSkillRoleAtt(pSkill, ERA_Resist_Choas, nERAResist_Choas);	
	AddSkillRoleAtt(pSkill, ERA_Resist_Special, nERAResist_Special);	
	AddSkillRoleAtt(pSkill, ERA_Regain_Addtion, nERARegain_Addtion);	
	AddSkillRoleAtt(pSkill, ERA_Attack_MissRate, nERAAttack_MissRate);	
	AddSkillRoleAtt(pSkill, ERA_CloseAttack_DodgeRate, nERACloseAttack_DodgeRate);	
	AddSkillRoleAtt(pSkill, ERA_RemoteAttack_DodgeRate, nERARemoteAttack_DodgeRate);	
	AddSkillRoleAtt(pSkill, ERA_Crit_Rate, nERACrit_Rate);	
	AddSkillRoleAtt(pSkill, ERA_Crit_Amount, nERACrit_Amount);	
	AddSkillRoleAtt(pSkill, ERA_Block_Rate, nERABlock_Rate);	
	AddSkillRoleAtt(pSkill, ERA_Inspiration, nERAInspiration);	
	AddSkillRoleAtt(pSkill, ERA_Lurk, nERALurk);	
	AddSkillRoleAtt(pSkill, ERA_Savvy, nERASavvy);	
	AddSkillRoleAtt(pSkill, ERA_Enmity_Degree, nERAEnmity_Degree);
	AddSkillRoleAtt(pSkill, ERA_Shape, nERAShape);
	AddSkillRoleAtt(pSkill, ERA_Exp_Add_Rate, nERAExp_Add_Rate);
	AddSkillRoleAtt(pSkill, ERA_Money_Add_Rate, nERAMoney_Add_Rate);
	AddSkillRoleAtt(pSkill, ERA_Loot_Add_Rate, nERALoot_Add_Rate);
	AddSkillRoleAtt(pSkill, ERA_Fortune, nERAFortune);
	AddSkillRoleAtt(pSkill, ERA_Appearance, nERAAppearance);
	AddSkillRoleAtt(pSkill, ERA_Rein, nERARein);
	AddSkillRoleAtt(pSkill, ERA_Toughness, nToughness);
	AddSkillRoleAtt(pSkill, ERA_transform_ExAttack, nERAExAt);
	AddSkillRoleAtt(pSkill, ERA_transform_InAttack, nERAInAt);
	AddSkillRoleAtt(pSkill, ERA_transform_ALL, nERAAll);
	AddSkillRoleAtt(pSkill, ERA_InstanceWearyEffect, nInstanceWearyEffect);
	AddSkillRoleAtt(pSkill, ERA_Mana_combustion_resistance, nERAMana_combustion_resistance);
	AddSkillRoleAtt(pSkill, ERA_Toughness_god, nERAToughness_god);
	AddSkillRoleAtt(pSkill, ERA_Toughness_strength, nERAToughness_strength);
	AddSkillRoleAtt(pSkill, ERA_Contrel_effect_deepen, nERAContrel_effect_deepen);
	AddSkillRoleAtt(pSkill, ERA_Fixed_damage_aggrandizement, nERAFixed_damage_aggrandizement);
	AddSkillRoleAtt(pSkill, ERA_Aging_effect_strengthening, nERAAging_effect_strengthening);
	AddSkillRoleAtt(pSkill, ERA_Slowing_effect_strengthening, nERASlowing_effect_strengthening);
	AddSkillRoleAtt(pSkill, ERA_Physical_damage_rebound_num, nERAPhysical_damage_rebound_num);
	AddSkillRoleAtt(pSkill, ERA_Physical_damage_rebound_ratio, nERAPhysical_damage_rebound_ratio);
	AddSkillRoleAtt(pSkill, ERA_Magic_damage_rebound_num, nERAMagic_damage_rebound_num);
	AddSkillRoleAtt(pSkill, ERA_Magic_damage_rebound_ratio, nERAMagic_damage_rebound_ratio);
	AddSkillRoleAtt(pSkill, ERA_Physical_damage_rebound_immune, nERAPhysical_damage_rebound_immune);
	AddSkillRoleAtt(pSkill, ERA_Magic_damage_rebound_immune, nERAMagic_damage_rebound_immune);
	AddSkillRoleAtt(pSkill, ERA_Mana_combustion, nERAMana_combustion);

	AddSkillRoleAtt(pSkill, ERA_Nosee_attack, nERANoseeattack);
	AddSkillRoleAtt(pSkill, ERA_Nosee_defend, nERANoseedefend);

	AddSkillRoleAtt(pSkill, ERA_transform_Bang, nERATransformBang);
	AddSkillRoleAtt(pSkill, ERA_transform_Brunt, nERATransformBrunt);
	AddSkillRoleAtt(pSkill, ERA_transform_Injury, nERATransformInjury);
	AddSkillRoleAtt(pSkill, ERA_transform_Thinker, nERATransformThinker);

	AddSkillRoleAtt(pSkill, ERA_EEP_VALUE, nElement_penetrating);
	AddSkillRoleAtt(pSkill, ERA_EER_ALL, nResistance_elements);
	AddSkillRoleAtt(pSkill, ERA_HitRateAdd, nERAHitRateAdd);

	// 计算总伤害次数
	pSkill->nDmgTimes = 0;
	for(INT n = 0; n < MAX_CHANNEL_TIMES; n++)
	{
		if( pSkill->nChannelDmg[n] > 0 )
			++pSkill->nDmgTimes;
		else
			break;
	}

	pSkill->iMaxAttackNum = m_pVar->GetInt(_T("atk_num"), szField, 0);
	pSkill->iModBufAttackNum = m_pVar->GetInt(_T("buff_atk_num_add"), szField, 0);
	pSkill->iTopHit = m_pVar->GetInt(_T("top_hit"), szField, GT_INVALID);
	pSkill->fDamageAddFactor = m_pVar->GetFloat(_T("damage_add_factor"), szField, 0);
	// 返回
	return &pSkill->dwID;
}

//-----------------------------------------------------------------------------
// 初始化一条Buff记录
//-----------------------------------------------------------------------------
VOID *AttRes::InitOneBuffProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(pBuff, pProtoType, tagBuffProto);

	// 初始化结构信息
	pBuff->dwID								=	(DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	pBuff->eType							=	(enum EBuffType)m_pVar->GetInt(_T("type"), szField);
	pBuff->nType2							=	m_pVar->GetInt(_T("type2"), szField);
	pBuff->nType3							=	m_pVar->GetInt(_T("type3"), szField);
	pBuff->eResistType						=	(enum EBuffResistType)m_pVar->GetInt(_T("resist_type"), szField);
	pBuff->bBenifit							=	(BOOL)m_pVar->GetDword(_T("benefit"), szField);
	pBuff->eFriendly						=	(enum EBuffFriendEnemy)m_pVar->GetDword(_T("friendly"), szField);
	pBuff->bInstant							=	(BOOL)m_pVar->GetDword(_T("instant"), szField);
	pBuff->bEffectExist						= (BOOL)m_pVar->GetDword(_T("effect_exist"), szField, 0);
	pBuff->dwGroupFlag						=	m_pVar->GetDword(_T("group_flag"), szField, GT_INVALID);
	pBuff->dwTargetAddLimit					=	m_pVar->GetDword(_T("target_add_limit"), szField);
	pBuff->dwTargetLimit					=	m_pVar->GetDword(_T("target_type"), szField);// 作用对象限制
	pBuff->dwTargetAddStateLimit			=	m_pVar->GetDword(_T("target_add_state_limit"), szField, GT_INVALID);
	pBuff->dwTargetStateLimit				=	m_pVar->GetDword(_T("target_state_limit"), szField, GT_INVALID);
	pBuff->nLevel							=	m_pVar->GetInt(_T("level"), szField);
	pBuff->eOPType							=	(enum EBuffOPType)m_pVar->GetInt(_T("op_type"), szField);
	pBuff->fOPDistance						=	m_pVar->GetFloat(_T("op_distance"), szField);
	pBuff->fOPRadius						=	m_pVar->GetFloat(_T("op_raduis"), szField);
	pBuff->nPersistTick						=	m_pVar->GetInt(_T("persist_time"), szField);
	pBuff->nInterOPTick						=	m_pVar->GetInt(_T("inter_op_time"), szField);
	pBuff->nWarpTimes						=	m_pVar->GetInt(_T("wrap_times"), szField);
	pBuff->bOfflineConsume					=	(BOOL)m_pVar->GetDword(_T("offline_consume"), szField, FALSE);
	pBuff->iMaxAttackNum					=	m_pVar->GetInt(_T("atk_num"), szField, 0);

	// 打断
	BOOL bInterruptMove						=	(BOOL)m_pVar->GetDword(_T("interrupt_move"), szField, FALSE);
	BOOL bInterruptInterCombat				=	(BOOL)m_pVar->GetDword(_T("interrupt_intercombat"), szField, FALSE);
	BOOL bInterruptManual					=	(BOOL)m_pVar->GetDword(_T("interrupt_manual"), szField, FALSE);
	BOOL bInterruptDead						=	(BOOL)m_pVar->GetDword(_T("interrupt_dead"), szField, FALSE);
	BOOL bInterruptChangeMap				=	(BOOL)m_pVar->GetDword(_T("interrupt_changemap"), szField, FALSE);
	BOOL bInterruptBuffFull					=	(BOOL)m_pVar->GetDword(_T("interrupt_bufffull"), szField, FALSE);
	BOOL bInterruptOffLine					=	(BOOL)m_pVar->GetDword(_T("interrupt_offline"), szField, FALSE);
	pBuff->nAttackInterruptRate				=	m_pVar->GetInt(_T("interrupt_attack_prop"), szField, GT_INVALID);
	pBuff->nHPInterruptLimit				=	m_pVar->GetInt(_T("hp_interrupt_limit"), szField, GT_INVALID);
	pBuff->nMPInterruptLimit				=	m_pVar->GetInt(_T("mp_interrupt_limit"), szField, GT_INVALID);
	pBuff->nRageInterruptLimit				=	m_pVar->GetInt(_T("rage_interrupt_limit"), szField, GT_INVALID);
	pBuff->nEnduranceInterruptLimit			=	m_pVar->GetInt(_T("durance_interrupt_limit"), szField, GT_INVALID);
	pBuff->nVitalityInterruptLimit			=	m_pVar->GetInt(_T("valicity_interrupt_limit"), szField, GT_INVALID);
	pBuff->dwBuffInterruptID				=	m_pVar->GetDword(_T("interrupt_buff_id"), szField, GT_INVALID);

	pBuff->dwInterruptFlag = 0;
	pBuff->dwInterruptFlag	|=	(bInterruptMove			?	EBIF_Move			: 0);
	pBuff->dwInterruptFlag	|=	(bInterruptInterCombat	?	EBIF_InterCombat	: 0);
	pBuff->dwInterruptFlag	|=	(bInterruptManual		?	EBIF_Manual			: 0);
	pBuff->dwInterruptFlag	|=	(bInterruptDead			?	EBIF_Die			: 0);
	pBuff->dwInterruptFlag	|=	(bInterruptChangeMap	?	EBIF_ChangeMap		: 0);
	pBuff->dwInterruptFlag	|=	(bInterruptBuffFull		?	EBIF_BuffFull		: 0);
	pBuff->dwInterruptFlag	|=	(bInterruptOffLine		?	EBIF_OffLine		: 0);

	pBuff->dwInterruptFlag	|=	((pBuff->nAttackInterruptRate > 0)		?	EBIF_BeAttacked		: 0);
	pBuff->dwInterruptFlag	|=	((pBuff->nHPInterruptLimit > 0)			?	EBIF_HPLower		: 0);
	pBuff->dwInterruptFlag	|=	((pBuff->nMPInterruptLimit > 0)			?	EBIF_MPLower		: 0);
	pBuff->dwInterruptFlag	|=	((pBuff->nRageInterruptLimit > 0)		?	EBIF_RageLower		: 0);
	pBuff->dwInterruptFlag	|=	((pBuff->nVitalityInterruptLimit > 0)	?	EBIF_VitalityLower	: 0);
	pBuff->dwInterruptFlag	|=	((pBuff->nEnduranceInterruptLimit > 0)	?	EBIF_EnduranceLower	: 0);

	// 效果
	pBuff->dwOPTrigger						=	m_pVar->GetDword(_T("trigger_id"), szField, GT_INVALID);

	pBuff->eEffect[EBEM_Persist]			=	(enum EBuffEffectType)m_pVar->GetInt(_T("effect_type"), szField, EBET_Null);
	pBuff->eEffect[EBEM_Instant]			=	(enum EBuffEffectType)m_pVar->GetInt(_T("instant_effect_type"), szField, EBET_Null);
	pBuff->eEffect[EBEM_Inter]				=	(enum EBuffEffectType)m_pVar->GetInt(_T("inter_effect_type"), szField, EBET_Null);
	pBuff->eEffect[EBEM_Finish]				=	(enum EBuffEffectType)m_pVar->GetInt(_T("end_effect_type"), szField, EBET_Null);

	pBuff->dwEffectMisc1[EBEM_Persist]		=	m_pVar->GetDword(_T("misc1"), szField, 0);
	pBuff->dwEffectMisc2[EBEM_Persist]		=	m_pVar->GetDword(_T("misc2"), szField, 0);
	pBuff->dwEffectMisc1[EBEM_Instant]		=	m_pVar->GetDword(_T("instant_misc1"), szField, 0);
	pBuff->dwEffectMisc2[EBEM_Instant]		=	m_pVar->GetDword(_T("instant_misc2"), szField, 0);
	pBuff->dwEffectMisc1[EBEM_Inter]		=	m_pVar->GetDword(_T("inter_misc1"), szField, 0);
	pBuff->dwEffectMisc2[EBEM_Inter]		=	m_pVar->GetDword(_T("inter_misc2"), szField, 0);
	pBuff->dwEffectMisc1[EBEM_Finish]		=	m_pVar->GetDword(_T("end_misc1"), szField, 0);
	pBuff->dwEffectMisc2[EBEM_Finish]		=	m_pVar->GetDword(_T("end_misc2"), szField, 0);

	// 当前属性改变
	pBuff->nInstantAttMod[EBEA_HP]			=	m_pVar->GetInt(_T("instant_hp"), szField, 0);
	pBuff->nInstantAttMod[EBEA_MP]			=	m_pVar->GetInt(_T("instant_mp"), szField, 0);
	pBuff->nInstantAttMod[EBEA_Rage]		=	m_pVar->GetInt(_T("instant_rage"), szField, 0);
	pBuff->nInstantAttMod[EBEA_Vitality]	=	m_pVar->GetInt(_T("instant_vitality"), szField, 0);
	pBuff->nInstantAttMod[EBEA_Endurance]	=	m_pVar->GetInt(_T("instant_endurance"), szField, 0);
	pBuff->nInstantAttMod[EBEA_Morale]		=	m_pVar->GetInt(_T("instant_morale"), szField, 0);
	pBuff->nInstantAttMod[EBEA_Injury]		=	m_pVar->GetInt(_T("instant_injury"), szField, 0);

	pBuff->nInterAttMod[EBEA_HP]			=	m_pVar->GetInt(_T("inter_hp"), szField, 0);
	pBuff->nInterAttMod[EBEA_MP]			=	m_pVar->GetInt(_T("inter_mp"), szField, 0);
	pBuff->nInterAttMod[EBEA_Rage]			=	m_pVar->GetInt(_T("inter_rage"), szField, 0);
	pBuff->nInterAttMod[EBEA_Vitality]		=	m_pVar->GetInt(_T("inter_vitality"), szField, 0);
	pBuff->nInterAttMod[EBEA_Endurance]		=	m_pVar->GetInt(_T("inter_endurance"), szField, 0);
	pBuff->nInterAttMod[EBEA_Morale]		=	m_pVar->GetInt(_T("inter_morale"), szField, 0);
	pBuff->nInterAttMod[EBEA_Injury]		=	m_pVar->GetInt(_T("inter_injury"), szField, 0);

	pBuff->nFinishAttMod[EBEA_HP]			=	m_pVar->GetInt(_T("end_hp"), szField, 0);
	pBuff->nFinishAttMod[EBEA_MP]			=	m_pVar->GetInt(_T("end_mp"), szField, 0);
	pBuff->nFinishAttMod[EBEA_Rage]			=	m_pVar->GetInt(_T("end_rage"), szField, 0);
	pBuff->nFinishAttMod[EBEA_Vitality]		=	m_pVar->GetInt(_T("end_vitality"), szField, 0);
	pBuff->nFinishAttMod[EBEA_Endurance]	=	m_pVar->GetInt(_T("end_endurance"), szField, 0);
	pBuff->nFinishAttMod[EBEA_Morale]		=	m_pVar->GetInt(_T("end_morale"), szField, 0);
	pBuff->nFinishAttMod[EBEA_Injury]		=	m_pVar->GetInt(_T("end_injury"), szField, 0);

	// 属性加成
	INT nERAPhysique							=	m_pVar->GetInt(_T("ERA_Physique"), szField, 0);
	INT nERAStrength							=	m_pVar->GetInt(_T("ERA_Strength"), szField, 0);
	INT nERAPneuma								=	m_pVar->GetInt(_T("ERA_Pneuma"), szField, 0);
	INT nERAInnerForce							=	m_pVar->GetInt(_T("ERA_InnerForce"), szField, 0);
	INT nERATechnique							=	m_pVar->GetInt(_T("ERA_Technique"), szField, 0);
	INT nERAAgility								=	m_pVar->GetInt(_T("ERA_Agility"), szField, 0);
	INT nERAMaxHP								=	m_pVar->GetInt(_T("ERA_MaxHP"), szField, 0);
	INT nERAMaxMP								=	m_pVar->GetInt(_T("ERA_MaxMP"), szField, 0);
	INT nERAExAttack							=	m_pVar->GetInt(_T("ERA_ExAttack"), szField, 0);
	INT nERAExDefense							=	m_pVar->GetInt(_T("ERA_ExDefense"), szField, 0);
	INT nERAInAttack							=	m_pVar->GetInt(_T("ERA_InAttack"), szField, 0);
	INT nERAInDefense							=	m_pVar->GetInt(_T("ERA_InDefense"), szField, 0);
	INT nERAAttackTec							=	m_pVar->GetInt(_T("ERA_AttackTec"), szField, 0);
	INT nERADefenseTec							=	m_pVar->GetInt(_T("ERA_DefenseTec"), szField, 0);
	INT nERAHitRate								=	m_pVar->GetInt(_T("ERA_HitRate"), szField, 0);
	INT nERADodge								=	m_pVar->GetInt(_T("ERA_Dodge"), szField, 0);
	INT nERAMaxEndurance						=	m_pVar->GetInt(_T("ERA_MaxEndurance"), szField, 0);
	INT nERAHPRegainRate						=	m_pVar->GetInt(_T("ERA_HPRegainRate"), szField, 0);
	INT nERAMPRegainRate						=	m_pVar->GetInt(_T("ERA_MPRegainRate"), szField, 0);
	INT nERAMaxVitality							=	m_pVar->GetInt(_T("ERA_MaxVitality"), szField, 0);
	INT nERAVitalityRegainRate					=	m_pVar->GetInt(_T("ERA_VitalityRegainRate"), szField, 0);
	INT nERASpeed_XZ							=	m_pVar->GetInt(_T("ERA_Speed_XZ"), szField, 0);
	INT nERASpeed_Y								=	m_pVar->GetInt(_T("ERA_Speed_Y"), szField, 0);
	INT nERASpeedSwim							=	m_pVar->GetInt(_T("ERA_Speed_Swin"), szField, 0);
	INT nERASpeedMount							=	m_pVar->GetInt(_T("ERA_Speed_Mount"), szField, 0);
	INT nERAWeaponDmgMin						=	m_pVar->GetInt(_T("ERA_WeaponDmgMin"), szField, 0);
	INT nERAWeaponDmgMax						=	m_pVar->GetInt(_T("ERA_WeaponDmgMax"), szField, 0);
	INT nERAWeaponSoul							=	m_pVar->GetInt(_T("ERA_WeaponSoul"), szField, 0);
	INT nERAArmor								=	m_pVar->GetInt(_T("ERA_Armor"), szField, 0);
	INT nERADerate_Ordinary						=	m_pVar->GetInt(_T("ERA_Derate_Ordinary"), szField, 0);
	INT nERADerate_Bleeding						=	m_pVar->GetInt(_T("ERA_Derate_Bleeding"), szField, 0);
	INT nERADerate_Brunt						=	m_pVar->GetInt(_T("ERA_Derate_Brunt"), szField, 0);
	INT nERADerate_Bang							=	m_pVar->GetInt(_T("ERA_Derate_Bang"), szField, 0);
	INT nERADerate_Poison						=	m_pVar->GetInt(_T("ERA_Derate_Poison"), szField, 0);
	INT nERADerate_Thinker						=	m_pVar->GetInt(_T("ERA_Derate_Thinker"), szField, 0);
	INT nERADerate_Injury						=	m_pVar->GetInt(_T("ERA_Derate_Injury"), szField, 0);
	INT nERADerate_Stunt						=	m_pVar->GetInt(_T("ERA_Derate_Stunt"), szField, 0);
	INT nERADerate_ExAttack						=	m_pVar->GetInt(_T("ERA_Derate_ExAttack"), szField, 0);
	INT nERADerate_InAttack						=	m_pVar->GetInt(_T("ERA_Derate_InAttack"), szField, 0);
	INT nERADerate_All							=	m_pVar->GetInt(_T("ERA_Derate_All"), szField, 0);
	INT nERAExDamage							=	m_pVar->GetInt(_T("ERA_ExDamage"), szField, 0);
	INT nERAExDamage_Absorb						=	m_pVar->GetInt(_T("ERA_ExDamage_Absorb"), szField, 0);
	INT nERAResist_Bleeding						=	m_pVar->GetInt(_T("ERA_Resist_Bleeding"), szField, 0);
	INT nERAResist_Weak							=	m_pVar->GetInt(_T("ERA_Resist_Weak"), szField, 0);
	INT nERAResist_Choas						=	m_pVar->GetInt(_T("ERA_Resist_Choas"), szField, 0);
	INT nERAResist_Special						=	m_pVar->GetInt(_T("ERA_Resist_Special"), szField, 0);
	INT nERARegain_Addtion						=	m_pVar->GetInt(_T("ERA_Regain_Addtion"), szField, 0);
	INT nERAAttack_MissRate						=	m_pVar->GetInt(_T("ERA_Attack_MissRate"), szField, 0);
	INT nERACloseAttack_DodgeRate				=	m_pVar->GetInt(_T("ERA_CloseAttack_DodgeRate"), szField, 0);
	INT nERARemoteAttack_DodgeRate				=	m_pVar->GetInt(_T("ERA_RemoteAttack_DodgeRate"), szField, 0);
	INT nERACrit_Rate							=	m_pVar->GetInt(_T("ERA_Crit_Rate"), szField, 0);
	INT nERACrit_Amount							=	m_pVar->GetInt(_T("ERA_Crit_Amount"), szField, 0);
	INT nERABlock_Rate							=	m_pVar->GetInt(_T("ERA_Block_Rate"), szField, 0);
	INT nERAInspiration							=	m_pVar->GetInt(_T("ERA_Inspiration"), szField, 0);
	INT nERALurk								=	m_pVar->GetInt(_T("ERA_Lurk"), szField, 0);
	INT nERASavvy								=	m_pVar->GetInt(_T("ERA_Savvy"), szField, 0);
	INT nERAEnmity_Degree						=	m_pVar->GetInt(_T("ERA_Enmity_Degree"), szField, 0);
	INT nERAShape								=	m_pVar->GetInt(_T("ERA_Shape"), szField, 0);
	INT nERAExp_Add_Rate						=	m_pVar->GetInt(_T("ERA_Exp_Add_Rate"), szField, 0);
	INT nERAMoney_Add_Rate						=	m_pVar->GetInt(_T("ERA_Money_Add_Rate"), szField, 0);
	INT nERALoot_Add_Rate						=	m_pVar->GetInt(_T("ERA_Loot_Add_Rate"), szField, 0);
	INT nERAFortune								=	m_pVar->GetInt(_T("ERA_Fortune"), szField, 0);
	INT nERAAppearance							=	m_pVar->GetInt(_T("ERA_Appearance"), szField, 0);
	INT nERARein								=	m_pVar->GetInt(_T("ERA_Rein"), szField, 0);
	INT nToughness								=	m_pVar->GetInt(_T("ERA_Toughness"),szField,0);
	INT nERAMana_combustion_resistance				= m_pVar->GetInt(_T("ERA_Mana_combustion_resistance"),szField,0 );
	INT nERAToughness_god									= m_pVar->GetInt(_T("ERA_Toughness_god"),szField,0 );
	INT nERAToughness_strength								= m_pVar->GetInt(_T("ERA_Toughness_strength"),szField,0 );
	INT nERAContrel_effect_deepen						= m_pVar->GetInt(_T("ERA_Contrel_effect_deepen"),szField,0 );
	INT nERAFixed_damage_aggrandizement			= m_pVar->GetInt(_T("ERA_Fixed_damage_aggrandizement"),szField,0 );
	INT nERAAging_effect_strengthening				= m_pVar->GetInt(_T("ERA_Aging_effect_strengthening"),szField,0 );
	INT nERASlowing_effect_strengthening				= m_pVar->GetInt(_T("ERA_Slowing_effect_strengthening"),szField,0 );
	INT nERAPhysical_damage_rebound_num 		= m_pVar->GetInt(_T("ERA_Physical_damage_rebound_num"),szField,0 );
	INT nERAPhysical_damage_rebound_ratio 		= m_pVar->GetInt(_T("ERA_Physical_damage_rebound_ratio"),szField,0 );
	INT nERAMagic_damage_rebound_num			= m_pVar->GetInt(_T("ERA_Magic_damage_rebound_num"),szField,0 );
	INT nERAMagic_damage_rebound_ratio			= m_pVar->GetInt(_T("ERA_Magic_damage_rebound_ratio"),szField,0 );
	INT nERAPhysical_damage_rebound_immune	= m_pVar->GetInt(_T("ERA_Physical_damage_rebound_immune"),szField,0 );
	INT nERAMagic_damage_rebound_immune		= m_pVar->GetInt(_T("ERA_Magic_damage_rebound_immune"),szField,0 );
	INT nERAMana_combustion								= m_pVar->GetInt(_T("ERA_Mana_combustion"),szField,0 );

	// Jason 2010-8-24
	// 加深状态属性影响
	INT nTransformBang					= m_pVar->GetInt(_T("ERA_transform_Bang"),szField,0 );
	INT nTransformBrunt					= m_pVar->GetInt( _T("ERA_transform_Brunt"),szField,0 );
	INT nTransformInjury					= m_pVar->GetInt( _T("ERA_transform_Injury"),szField,0 );
	INT nTransformThinker					= m_pVar->GetInt( _T("ERA_transform_Thinker"),szField,0 );
	INT nTransformExAttack				= m_pVar->GetInt( _T("ERA_transform_ExAttack"),szField,0 );
	INT nTransformInAttack				= m_pVar->GetInt( _T("ERA_transform_InAttack"),szField,0 );
	INT nTransformAll						= m_pVar->GetInt( _T("ERA_transform_ALL"),szField,0 );

	// Jason 2010-11-23 v1.2.2.X台湾用金牌网吧buff
	INT nInstanceWearyEffect				= m_pVar->GetInt( _T("ERA_InstanceWearyEffect"),szField,0 );

	INT nERANoseeattack							=	m_pVar->GetInt(_T("ERA_Nosee_attack"), szField, 0);
	INT nERANoseedefend							=	m_pVar->GetInt(_T("ERA_Nosee_defend"), szField, 0);

	// 新增加元素妖精相关属性
	INT nElement_penetrating					=   m_pVar->GetInt(_T("Element_penetrating"), szField, 0);
	INT nResistance_elements					=	m_pVar->GetInt(_T("Resistance_elements"), szField, 0);

	INT nERAHitRateAdd							=	m_pVar->GetInt(_T("ERA_HitRateAdd"), szField, 0);

	AddBuffRoleAtt(pBuff, ERA_Physique, nERAPhysique);
	AddBuffRoleAtt(pBuff, ERA_Strength, nERAStrength);
	AddBuffRoleAtt(pBuff, ERA_Pneuma, nERAPneuma);
	AddBuffRoleAtt(pBuff, ERA_InnerForce, nERAInnerForce);
	AddBuffRoleAtt(pBuff, ERA_Technique, nERATechnique);
	AddBuffRoleAtt(pBuff, ERA_Agility, nERAAgility);
	AddBuffRoleAtt(pBuff, ERA_MaxHP, nERAMaxHP);
	AddBuffRoleAtt(pBuff, ERA_MaxMP, nERAMaxMP);
	AddBuffRoleAtt(pBuff, ERA_ExAttack, nERAExAttack);
	AddBuffRoleAtt(pBuff, ERA_ExDefense, nERAExDefense);
	AddBuffRoleAtt(pBuff, ERA_InAttack, nERAInAttack);
	AddBuffRoleAtt(pBuff, ERA_InDefense, nERAInDefense);
	AddBuffRoleAtt(pBuff, ERA_AttackTec, nERAAttackTec);
	AddBuffRoleAtt(pBuff, ERA_DefenseTec, nERADefenseTec);
	AddBuffRoleAtt(pBuff, ERA_HitRate, nERAHitRate);
	AddBuffRoleAtt(pBuff, ERA_Dodge, nERADodge);
	AddBuffRoleAtt(pBuff, ERA_MaxEndurance, nERAMaxEndurance);
	AddBuffRoleAtt(pBuff, ERA_HPRegainRate, nERAHPRegainRate);
	AddBuffRoleAtt(pBuff, ERA_MPRegainRate, nERAMPRegainRate);
	AddBuffRoleAtt(pBuff, ERA_MaxVitality, nERAMaxVitality);
	AddBuffRoleAtt(pBuff, ERA_VitalityRegainRate, nERAVitalityRegainRate);
	AddBuffRoleAtt(pBuff, ERA_Speed_XZ, nERASpeed_XZ);
	AddBuffRoleAtt(pBuff, ERA_Speed_Y, nERASpeed_Y);
	AddBuffRoleAtt(pBuff, ERA_Speed_Swim, nERASpeedSwim);
	AddBuffRoleAtt(pBuff, ERA_Speed_Mount, nERASpeedMount);
	AddBuffRoleAtt(pBuff, ERA_WeaponDmgMin, nERAWeaponDmgMin);
	AddBuffRoleAtt(pBuff, ERA_WeaponDmgMax, nERAWeaponDmgMax);
	AddBuffRoleAtt(pBuff, ERA_WeaponSoul, nERAWeaponSoul);
	AddBuffRoleAtt(pBuff, ERA_Armor, nERAArmor);
	AddBuffRoleAtt(pBuff, ERA_Derate_Ordinary, nERADerate_Ordinary);
	AddBuffRoleAtt(pBuff, ERA_Derate_Bleeding, nERADerate_Bleeding);
	AddBuffRoleAtt(pBuff, ERA_Derate_Brunt, nERADerate_Brunt);
	AddBuffRoleAtt(pBuff, ERA_Derate_Bang, nERADerate_Bang);
	AddBuffRoleAtt(pBuff, ERA_Derate_Poison, nERADerate_Poison);
	AddBuffRoleAtt(pBuff, ERA_Derate_Thinker, nERADerate_Thinker);
	AddBuffRoleAtt(pBuff, ERA_Derate_Injury, nERADerate_Injury);
	AddBuffRoleAtt(pBuff, ERA_Derate_Stunt, nERADerate_Stunt);
	AddBuffRoleAtt(pBuff, ERA_Derate_ExAttack, nERADerate_ExAttack);
	AddBuffRoleAtt(pBuff, ERA_Derate_InAttack, nERADerate_InAttack);
	AddBuffRoleAtt(pBuff, ERA_Derate_ALL, nERADerate_All);
	AddBuffRoleAtt(pBuff, ERA_ExDamage, nERAExDamage);
	AddBuffRoleAtt(pBuff, ERA_ExDamage_Absorb, nERAExDamage_Absorb);	
	AddBuffRoleAtt(pBuff, ERA_Resist_Bleeding, nERAResist_Bleeding);	
	AddBuffRoleAtt(pBuff, ERA_Resist_Weak, nERAResist_Weak);	
	AddBuffRoleAtt(pBuff, ERA_Resist_Choas, nERAResist_Choas);	
	AddBuffRoleAtt(pBuff, ERA_Resist_Special, nERAResist_Special);	
	AddBuffRoleAtt(pBuff, ERA_Regain_Addtion, nERARegain_Addtion);	
	AddBuffRoleAtt(pBuff, ERA_Attack_MissRate, nERAAttack_MissRate);	
	AddBuffRoleAtt(pBuff, ERA_CloseAttack_DodgeRate, nERACloseAttack_DodgeRate);	
	AddBuffRoleAtt(pBuff, ERA_RemoteAttack_DodgeRate, nERARemoteAttack_DodgeRate);	
	AddBuffRoleAtt(pBuff, ERA_Crit_Rate, nERACrit_Rate);	
	AddBuffRoleAtt(pBuff, ERA_Crit_Amount, nERACrit_Amount);	
	AddBuffRoleAtt(pBuff, ERA_Block_Rate, nERABlock_Rate);	
	AddBuffRoleAtt(pBuff, ERA_Inspiration, nERAInspiration);	
	AddBuffRoleAtt(pBuff, ERA_Lurk, nERALurk);	
	AddBuffRoleAtt(pBuff, ERA_Savvy, nERASavvy);	
	AddBuffRoleAtt(pBuff, ERA_Enmity_Degree, nERAEnmity_Degree);
	AddBuffRoleAtt(pBuff, ERA_Shape, nERAShape);
	AddBuffRoleAtt(pBuff, ERA_Exp_Add_Rate, nERAExp_Add_Rate);
	AddBuffRoleAtt(pBuff, ERA_Money_Add_Rate, nERAMoney_Add_Rate);
	AddBuffRoleAtt(pBuff, ERA_Loot_Add_Rate, nERALoot_Add_Rate);
	AddBuffRoleAtt(pBuff, ERA_Fortune, nERAFortune);
	AddBuffRoleAtt(pBuff, ERA_Appearance, nERAAppearance);
	AddBuffRoleAtt(pBuff, ERA_Rein, nERARein);
	AddBuffRoleAtt(pBuff, ERA_Toughness,nToughness);

	// Jason 2010-8-24
	AddBuffRoleAtt(pBuff, ERA_transform_Bang, nTransformBang);
	AddBuffRoleAtt(pBuff, ERA_transform_Brunt, nTransformBrunt);
	AddBuffRoleAtt(pBuff, ERA_transform_Injury, nTransformInjury);
	AddBuffRoleAtt(pBuff, ERA_transform_Thinker, nTransformThinker);
	AddBuffRoleAtt(pBuff, ERA_transform_ExAttack, nTransformExAttack);
	AddBuffRoleAtt(pBuff, ERA_transform_InAttack, nTransformInAttack);
	AddBuffRoleAtt(pBuff, ERA_transform_ALL, nTransformAll);

	// Jason 2010-11-23 1.2.2.X台湾金牌网吧修改
	AddBuffRoleAtt(pBuff, ERA_InstanceWearyEffect, nInstanceWearyEffect);

	AddBuffRoleAtt(pBuff, ERA_Mana_combustion_resistance, nERAMana_combustion_resistance);
	AddBuffRoleAtt(pBuff, ERA_Toughness_god, nERAToughness_god);
	AddBuffRoleAtt(pBuff, ERA_Toughness_strength, nERAToughness_strength);
	AddBuffRoleAtt(pBuff, ERA_Contrel_effect_deepen, nERAContrel_effect_deepen);
	AddBuffRoleAtt(pBuff, ERA_Fixed_damage_aggrandizement, nERAFixed_damage_aggrandizement);
	AddBuffRoleAtt(pBuff, ERA_Aging_effect_strengthening, nERAAging_effect_strengthening);
	AddBuffRoleAtt(pBuff, ERA_Slowing_effect_strengthening, nERASlowing_effect_strengthening);
	AddBuffRoleAtt(pBuff, ERA_Physical_damage_rebound_num, nERAPhysical_damage_rebound_num);
	AddBuffRoleAtt(pBuff, ERA_Physical_damage_rebound_ratio, nERAPhysical_damage_rebound_ratio);
	AddBuffRoleAtt(pBuff, ERA_Magic_damage_rebound_num, nERAMagic_damage_rebound_num);
	AddBuffRoleAtt(pBuff, ERA_Magic_damage_rebound_ratio, nERAMagic_damage_rebound_ratio);
	AddBuffRoleAtt(pBuff, ERA_Physical_damage_rebound_immune, nERAPhysical_damage_rebound_immune);
	AddBuffRoleAtt(pBuff, ERA_Magic_damage_rebound_immune, nERAMagic_damage_rebound_immune);
	AddBuffRoleAtt(pBuff, ERA_Mana_combustion, nERAMana_combustion);

	AddBuffRoleAtt(pBuff, ERA_Nosee_attack, nERANoseeattack);
	AddBuffRoleAtt(pBuff, ERA_Nosee_defend, nERANoseedefend);
	//添加元素穿透和元素抵抗
	AddBuffRoleAtt(pBuff, ERA_EEP_VALUE, nElement_penetrating);
	AddBuffRoleAtt(pBuff, ERA_EER_ALL, nResistance_elements);
	//添加命中
	AddBuffRoleAtt(pBuff, ERA_HitRateAdd, nERAHitRateAdd);

	// 修正两个时间属性
	if( P_VALID(pBuff->nPersistTick) )
		pBuff->nPersistTick /= TICK_TIME;

	if( P_VALID(pBuff->nInterOPTick) )
		pBuff->nInterOPTick /= TICK_TIME;

	return &pBuff->dwID;

}

//-----------------------------------------------------------------------------
// 初始化一条trigger记录
//-----------------------------------------------------------------------------
VOID *AttRes::InitOneTriggerProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	MTRANS_POINTER(pTrigger, pProtoType, tagTriggerProto);

	// 初始化结构信息
	pTrigger->dwID				=	(DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	pTrigger->eEventType		=	(enum ETriggerEventType)m_pVar->GetInt(_T("event_type"), szField);
	pTrigger->dwEventMisc1		=	m_pVar->GetDword(_T("event_misc1"), szField, 0);
	pTrigger->dwEventMisc2		=	m_pVar->GetDword(_T("event_misc2"), szField, 0);
	pTrigger->nEventProp		=	m_pVar->GetInt(_T("prop"), szField);
	pTrigger->eStateType		=	(enum ETriggerStateType)m_pVar->GetInt(_T("state_type"), szField);
	pTrigger->dwStateMisc1		=	m_pVar->GetDword(_T("state_misc1"), szField, 0);
	pTrigger->dwStateMisc2		=	m_pVar->GetDword(_T("state_misc2"), szField, 0);

	return &pTrigger->dwID;
}

//----------------------------------------------------------------------------------------
// 创建技能被动影响表
//----------------------------------------------------------------------------------------
VOID AttRes::LoadModifyMap()
{
	m_mapSkillProto.ResetIterator();
	DWORD dwSkillTypeID = GT_INVALID;
	tagSkillProto* pProto = (tagSkillProto*)GT_INVALID;

	while( m_mapSkillProto.PeekNext(dwSkillTypeID, pProto) )
	{
		DWORD dwSkillID = dwSkillTypeID / 100;

		// 如果该技能的作用对象不是技能，则直接返回
		if( ESTT_Skill != pProto->eTargetType ) continue;
		
		// 如果该技能影响的技能不存在，则直接返回
		if( !P_VALID(pProto->dwTargetSkillID) ) continue;

		// 检查被影响的技能是否已经存在，如果没有就创建
		// F计划--技能ID小于10000，为技能盒子，需要
		if (pProto->dwTargetSkillID > 0 && pProto->dwTargetSkillID < 10000)
		{
			const tagSkillBoxProto * pSkillBoxProto = g_attRes.GetOneSkillProto(pProto->dwTargetSkillID);
			if( !P_VALID(pSkillBoxProto) )
				continue;

			for (int nCnt = 0; nCnt < tagSkillBoxProto::E_SBP_Size; nCnt++)
			{
				tagSkillModify* pSkillModify = m_mapSkillModify.Peek(pSkillBoxProto->dwSkillIDs[nCnt]);

				if( !P_VALID(pSkillModify) )
				{
					pSkillModify = new tagSkillModify;
					m_mapSkillModify.Add(pSkillBoxProto->dwSkillIDs[nCnt], pSkillModify);
				}
				if( FALSE == pSkillModify->listModify.IsExist(dwSkillID) )
					pSkillModify->listModify.PushBack(dwSkillID);
			}
		}
		else
		{
			tagSkillModify* pSkillModify = m_mapSkillModify.Peek(pProto->dwTargetSkillID);

			if( !P_VALID(pSkillModify) )
			{
				pSkillModify = new tagSkillModify;
				m_mapSkillModify.Add(pProto->dwTargetSkillID, pSkillModify);
			}
			if( FALSE == pSkillModify->listModify.IsExist(dwSkillID) )
				pSkillModify->listModify.PushBack(dwSkillID);
		}
	}
}

//--------------------------------------------------------------------------------------------
// 初始化一条Creature记录（必须在加载完默认之后进行加载）
//--------------------------------------------------------------------------------------------
VOID *AttRes::InitOneCreatureProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy)
{
	// 从配置文件world.ini中读取速度
	TObjRef<VarContainer> pVar = "VarContainer";

	MTRANS_POINTER(pCreature, pProtoType, tagCreatureProto);
	
	// 初始化结构信息
	pCreature->dwTypeID								=	(DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	pCreature->eType								=	(ECreatureType)m_pVar->GetInt(_T("type"), szField);
	pCreature->nType2								=	m_pVar->GetInt(_T("type2"), szField);
	pCreature->nType3								=	m_pVar->GetInt(_T("type3"), szField);
	pCreature->bySex								=	(BYTE)m_pVar->GetDword(_T("sex"), szField);
	pCreature->eLite								=	(ERank)m_pVar->GetInt(_T("elite"), szField, ER_Null);
	pCreature->nLevel								=	m_pVar->GetInt(_T("level"), szField);
	pCreature->vSize.x								=	X_DEF_CREATURE_SIZE_X;
	pCreature->vSize.y								=	X_DEF_CREATURE_SIZE_Y;
	pCreature->vSize.z								=	X_DEF_CREATURE_SIZE_Z;
	pCreature->ePatrolType							=	(ECreaturePatrolType)m_pVar->GetInt(_T("patrol_type"), szField);
	pCreature->nPatrolRadius						=	m_pVar->GetInt(_T("patrol_radius"), szField);
	pCreature->ePursueType							=	(ECreaturePursueType)m_pVar->GetInt(_T("pursue_type"), szField);
	pCreature->dwNormalSkillIDMelee					=	m_pVar->GetDword(_T("normal_skill_id"), szField, GT_INVALID);
	pCreature->dwNormalSkillIDRanged				=	m_pVar->GetDword(_T("normal_skill_id_2"), szField, GT_INVALID);
	pCreature->nAttackInterTick						=	m_pVar->GetInt(_T("attack_inter"), szField) / TICK_TIME;
	pCreature->nMaxPursueTick						=	m_pVar->GetInt(_T("pursue_time"), szField, CREATURE_RETURN_TICK * TICK_TIME) / TICK_TIME;
	pCreature->eRespawnType							=	(ECreatureRespawnType)m_pVar->GetInt(_T("respawn_mode"), szField);
	pCreature->nRespawnTick							=	m_pVar->GetInt(_T("respawn_time"), szField) / (INT)TICK_TIME;
	pCreature->eTaggedType							=	(ECreatureTaggedType)m_pVar->GetInt(_T("tagged_type"), szField);
	pCreature->nLiveTick							=	m_pVar->GetInt(_T("alive_time"), szField, 0)/ TICK_TIME;
	pCreature->nExpGive								=	m_pVar->GetInt(_T("exp_give"), szField);
	pCreature->eRepType1							=	(EReputationType)m_pVar->GetInt(_T("rep_type1"), szField, ERT_NULL);
	pCreature->nRepNum1								=	m_pVar->GetInt(_T("rep_num1"), szField, 0);
	pCreature->eRepType2							=	(EReputationType)m_pVar->GetInt(_T("rep_type2"), szField, ERT_NULL);
	pCreature->nRepNum2								=	m_pVar->GetInt(_T("rep_num2"), szField, 0);
	pCreature->dwLootID								=	m_pVar->GetDword(_T("loot_id"), szField, GT_INVALID);
	pCreature->dwLootID2							=	m_pVar->GetDword(_T("loot_id2"), szField, GT_INVALID);
	pCreature->dwLootID3							=	m_pVar->GetDword(_T("loot_id3"), szField, GT_INVALID);
	pCreature->bCanAttack							=	(BOOL)m_pVar->GetDword(_T("can_attack"), szField);
	pCreature->bCanBeAttack							=	(BOOL)m_pVar->GetDword(_T("can_be_attack"), szField);
	pCreature->bVisble								=	(BOOL)m_pVar->GetDword(_T("visible"), szField);
	pCreature->bCanHitFly							=	(BOOL)m_pVar->GetDword(_T("can_hitfly"), szField, FALSE);
	pCreature->eFunctionType						=	(EFunctionNPCType)m_pVar->GetInt(_T("function_type"), szField, EFNPCT_Null);
	pCreature->eAICreateType						=	(EAICreateType)m_pVar->GetInt(_T("ai_create_type"), szField, EACT_Null);
	pCreature->dwAIID								=	m_pVar->GetDword(_T("ai_id"), szField, GT_INVALID);
	pCreature->eAIAction							=	(EAIACTION)m_pVar->GetDword(_T("ai_action"), szField, 0);
	pCreature->u16QuestID							=	(UINT16)m_pVar->GetInt(_T("quest_id"), szField, 0);
	pCreature->bActive								=	(BOOL)m_pVar->GetDword(_T("bActive"), szField, 0);
	pCreature->byCampType							=	m_pVar->GetDword(_T("camps"), szField, ECamp_Null);
	if (pCreature->byCampType >= ECamp_End)
	{
		pCreature->byCampType = ECamp_Null;
	}

	// Jason 2009-11-26 物品掉落衰减类型
	pCreature->eItemLootAttenuType					=	(ECreatureItemLootAttenuationType)m_pVar->GetInt(_T("item_loot_attenuation_type"), szField, ECILAT_Normal);
	
	// 怪物属性
	memset(pCreature->nBaseAtt,0,sizeof(pCreature->nBaseAtt));
	pCreature->nBaseAtt[ERA_Physique]				=	m_pVar->GetInt(_T("physique"), szField, GetAttDefCreature(ERA_Physique));
	pCreature->nBaseAtt[ERA_Strength]				=	m_pVar->GetInt(_T("strength"), szField, GetAttDefCreature(ERA_Strength));
	pCreature->nBaseAtt[ERA_Pneuma]					=	m_pVar->GetInt(_T("pneuma"), szField, GetAttDefCreature(ERA_Pneuma));
	pCreature->nBaseAtt[ERA_InnerForce]				=	m_pVar->GetInt(_T("innerforce"), szField, GetAttDefCreature(ERA_InnerForce));
	pCreature->nBaseAtt[ERA_Agility]				=	m_pVar->GetInt(_T("technique"), szField, GetAttDefCreature(ERA_Agility));
	
	pCreature->nBaseAtt[ERA_MaxHP]					=	m_pVar->GetInt(_T("max_hp"), szField, GetAttDefCreature(ERA_MaxHP));
	pCreature->nBaseAtt[ERA_MaxMP]					=	m_pVar->GetInt(_T("max_mp"), szField, GetAttDefCreature(ERA_MaxMP));
	pCreature->nBaseAtt[ERA_ExAttack]				=	m_pVar->GetInt(_T("ex_attack"), szField, GetAttDefCreature(ERA_ExAttack));
	pCreature->nBaseAtt[ERA_ExDefense]				=	m_pVar->GetInt(_T("ex_defense"), szField, GetAttDefCreature(ERA_ExDefense));
	pCreature->nBaseAtt[ERA_InAttack]				=	m_pVar->GetInt(_T("in_attack"), szField, GetAttDefCreature(ERA_InAttack));
	pCreature->nBaseAtt[ERA_InDefense]				=	m_pVar->GetInt(_T("in_defense"), szField, GetAttDefCreature(ERA_InDefense));
	pCreature->nBaseAtt[ERA_AttackTec]				=	m_pVar->GetInt(_T("attack_tec"), szField, GetAttDefCreature(ERA_AttackTec));
	pCreature->nBaseAtt[ERA_DefenseTec]				=	m_pVar->GetInt(_T("defense_tec"), szField, GetAttDefCreature(ERA_DefenseTec));
	pCreature->nBaseAtt[ERA_HitRate]				=	m_pVar->GetInt(_T("hit_rate"), szField, GetAttDefCreature(ERA_HitRate));
	pCreature->nBaseAtt[ERA_Dodge]					=	m_pVar->GetInt(_T("dodge"), szField, GetAttDefCreature(ERA_Dodge));
	pCreature->nBaseAtt[ERA_MaxEndurance]			=	m_pVar->GetInt(_T("endurance"), szField, GetAttDefCreature(ERA_Endurance));

	pCreature->nBaseAtt[ERA_HPRegainRate]			=	m_pVar->GetInt(_T("hp_regain_rate"), szField, GetAttDefCreature(ERA_HPRegainRate));
	pCreature->nBaseAtt[ERA_MPRegainRate]			=	m_pVar->GetInt(_T("mp_regain_rate"), szField, GetAttDefCreature(ERA_MPRegainRate));
	pCreature->nBaseAtt[ERA_Speed_XZ]				=	m_pVar->GetInt(_T("speed_xz"), szField, GetAttDefCreature(ERA_Speed_XZ));
	pCreature->nBaseAtt[ERA_Speed_Y]				=	m_pVar->GetInt(_T("speed_y"), szField, GetAttDefCreature(ERA_Speed_Y));
	pCreature->nBaseAtt[ERA_WeaponDmgMin]			=	m_pVar->GetInt(_T("weapon_dmg_min"), szField, GetAttDefCreature(ERA_WeaponDmgMin));
	pCreature->nBaseAtt[ERA_WeaponDmgMax]			=	m_pVar->GetInt(_T("weapon_dmg_max"), szField, GetAttDefCreature(ERA_WeaponDmgMax));
	pCreature->nBaseAtt[ERA_WeaponSoul]				=	m_pVar->GetInt(_T("weapon_soul"), szField, GetAttDefCreature(ERA_WeaponSoul));
	pCreature->nBaseAtt[ERA_Armor]					=	m_pVar->GetInt(_T("armor"), szField, GetAttDefCreature(ERA_Armor));
	
	pCreature->nBaseAtt[ERA_Derate_Ordinary]		=	m_pVar->GetInt(_T("derate_ordinary"), szField, GetAttDefCreature(ERA_Derate_Ordinary));
	pCreature->nBaseAtt[ERA_Derate_Bleeding]		=	m_pVar->GetInt(_T("derate_bleeding"), szField, GetAttDefCreature(ERA_Derate_Bleeding));
	pCreature->nBaseAtt[ERA_Derate_Brunt]			=	m_pVar->GetInt(_T("derate_brunt"), szField, GetAttDefCreature(ERA_Derate_Brunt));
	pCreature->nBaseAtt[ERA_Derate_Bang]			=	m_pVar->GetInt(_T("derate_bang"), szField, GetAttDefCreature(ERA_Derate_Bang));
	pCreature->nBaseAtt[ERA_Derate_Poison]			=	m_pVar->GetInt(_T("derate_poison"), szField, GetAttDefCreature(ERA_Derate_Poison));
	pCreature->nBaseAtt[ERA_Derate_Thinker]			=	m_pVar->GetInt(_T("derate_thinker"), szField, GetAttDefCreature(ERA_Derate_Thinker));
	pCreature->nBaseAtt[ERA_Derate_Injury]			=	m_pVar->GetInt(_T("derate_injury"), szField, GetAttDefCreature(ERA_Derate_Injury));
	pCreature->nBaseAtt[ERA_Derate_Stunt]			=	m_pVar->GetInt(_T("derate_stunt"), szField, GetAttDefCreature(ERA_Derate_Stunt));
	pCreature->nBaseAtt[ERA_Derate_ExAttack]		=	m_pVar->GetInt(_T("derate_exattack"), szField, GetAttDefCreature(ERA_Derate_ExAttack));
	pCreature->nBaseAtt[ERA_Derate_InAttack]		=	m_pVar->GetInt(_T("derate_inattack"), szField, GetAttDefCreature(ERA_Derate_InAttack));
	pCreature->nBaseAtt[ERA_Derate_ALL]				=	m_pVar->GetInt(_T("derate_all"), szField, GetAttDefCreature(ERA_Derate_ALL));
	
	pCreature->nBaseAtt[ERA_ExDamage]				=	m_pVar->GetInt(_T("ex_damage"), szField, GetAttDefCreature(ERA_ExDamage));
	pCreature->nBaseAtt[ERA_ExDamage_Absorb]		=	m_pVar->GetInt(_T("ex_damage_absorb"), szField, GetAttDefCreature(ERA_ExDamage_Absorb));
	pCreature->nBaseAtt[ERA_Resist_Bleeding]		=	m_pVar->GetInt(_T("resist_bleeding"), szField, GetAttDefCreature(ERA_Resist_Bleeding));
	pCreature->nBaseAtt[ERA_Resist_Weak]			=	m_pVar->GetInt(_T("resist_weak"), szField, GetAttDefCreature(ERA_Resist_Weak));
	pCreature->nBaseAtt[ERA_Resist_Choas]			=	m_pVar->GetInt(_T("resist_choas"), szField, GetAttDefCreature(ERA_Resist_Choas));
	pCreature->nBaseAtt[ERA_Resist_Special]			=	m_pVar->GetInt(_T("resist_special"), szField, GetAttDefCreature(ERA_Resist_Special));
	pCreature->nBaseAtt[ERA_Regain_Addtion]			=	m_pVar->GetInt(_T("regain_addtion"), szField, GetAttDefCreature(ERA_Regain_Addtion));
	pCreature->nBaseAtt[ERA_Attack_MissRate]		=	m_pVar->GetInt(_T("attack_missrate"), szField, GetAttDefCreature(ERA_Attack_MissRate));
	pCreature->nBaseAtt[ERA_CloseAttack_DodgeRate]	=	m_pVar->GetInt(_T("closeattack_dodgerate"), szField, GetAttDefCreature(ERA_CloseAttack_DodgeRate));
	pCreature->nBaseAtt[ERA_RemoteAttack_DodgeRate] =	m_pVar->GetInt(_T("remoteattack_dodgerate"), szField, GetAttDefCreature(ERA_RemoteAttack_DodgeRate));
	pCreature->nBaseAtt[ERA_Crit_Rate]				=	m_pVar->GetInt(_T("crit_rate"), szField, GetAttDefCreature(ERA_Crit_Rate));
	pCreature->nBaseAtt[ERA_Crit_Amount]			=	m_pVar->GetInt(_T("crit_amount"), szField, GetAttDefCreature(ERA_Crit_Amount));
	pCreature->nBaseAtt[ERA_Block_Rate]				=	m_pVar->GetInt(_T("block_rate"), szField, GetAttDefCreature(ERA_Block_Rate));
	
	pCreature->nBaseAtt[ERA_Inspiration]			=	m_pVar->GetInt(_T("inspiration"), szField, GetAttDefCreature(ERA_Inspiration));
	pCreature->nBaseAtt[ERA_Lurk]					=	m_pVar->GetInt(_T("lurk"), szField, GetAttDefCreature(ERA_Lurk));
	pCreature->nBaseAtt[ERA_Savvy]					=	m_pVar->GetInt(_T("savvy"), szField, GetAttDefCreature(ERA_Savvy));
	pCreature->nBaseAtt[ERA_Morale]					=	m_pVar->GetInt(_T("morale"), szField, GetAttDefCreature(ERA_Morale));
	pCreature->nBaseAtt[ERA_Injury]					=	m_pVar->GetInt(_T("injury"), szField, GetAttDefCreature(ERA_Injury));

	pCreature->nBaseAtt[ERA_Enmity_Degree]			=	m_pVar->GetInt(_T("enmity"), szField, GetAttDefCreature(ERA_Enmity_Degree));
	pCreature->nBaseAtt[ERA_Shape]					=	m_pVar->GetInt(_T("shape"), szField, GetAttDefCreature(ERA_Shape));
	pCreature->nBaseAtt[ERA_Exp_Add_Rate]			=	m_pVar->GetInt(_T("exp_add_rate"), szField, GetAttDefCreature(ERA_Exp_Add_Rate));
	pCreature->nBaseAtt[ERA_Money_Add_Rate]			=	m_pVar->GetInt(_T("money_add_rate"), szField, GetAttDefCreature(ERA_Money_Add_Rate));
	pCreature->nBaseAtt[ERA_Loot_Add_Rate]			=	m_pVar->GetInt(_T("loot_add_rate"), szField, GetAttDefCreature(ERA_Loot_Add_Rate));


	// 社会属性全部清零
	pCreature->nBaseAtt[ERA_Fortune]				=	0;
	pCreature->nBaseAtt[ERA_Appearance]				=	0;
	pCreature->nBaseAtt[ERA_Rein]					=	0;
	pCreature->nBaseAtt[ERA_Knowledge]				=	0;
	pCreature->nBaseAtt[ERA_Morality]				=	0;
	pCreature->nBaseAtt[ERA_Culture]				=	0;

	//for( INT nEEI = 0; nEEI <= ERA_EEI_End - ERA_EEI_START; ++nEEI )
	//	pCreature->nBaseAtt[nEEI] = 0;
	INT nEEIType = m_pVar->GetInt(_T("default_wuxing"),szField,-1);
	if( nEEIType >= EEI_START && nEEIType < EEI_End )
		pCreature->nBaseAtt[ERA_EEI_START + nEEIType]			= m_pVar->GetInt(_T("default_value"),szField,0);

	pCreature->nBaseAtt[ERA_EER_Fire]	= m_pVar->GetInt(_T("fire_def"),szField,0);
	pCreature->nBaseAtt[ERA_EER_Water]	= m_pVar->GetInt(_T("water_def"),szField,0);
	pCreature->nBaseAtt[ERA_EER_Earth]	= m_pVar->GetInt(_T("soil_def"),szField,0);
	pCreature->nBaseAtt[ERA_EER_Wind]	= m_pVar->GetInt(_T("gas_def"),szField,0);
	pCreature->nBaseAtt[ERA_EER_Bright]	= m_pVar->GetInt(_T("light_def"),szField,0);
	pCreature->nBaseAtt[ERA_EER_Dark]	= m_pVar->GetInt(_T("dark_def"),szField,0);

	// 设置职能ID
	pCreature->uFunctionID.dwCommonID				=	m_pVar->GetDword(_T("function_id"), szField, GT_INVALID);

	// 巢穴相关
	if( ECT_Monster == pCreature->eType && EMTT_Nest == pCreature->nType2)
	{
		pCreature->pNest = new tagNestProto;
		pCreature->pNest->nSpawnRadius = m_pVar->GetInt(_T("spawn_raduis"), szField);

		DWORD dwCreatureID = GT_INVALID;
		TCHAR szTemp[X_LONG_STRING];
		INT n = 0;
		for(; n < MAX_SPAWN_CREATURE_NUM; n++)
		{
			_stprintf(szTemp, _T("spawn_id_%d"), n+1);
			dwCreatureID = m_pVar->GetDword(szTemp, szField, GT_INVALID);

			if( GT_INVALID != dwCreatureID )
			{
				pCreature->pNest->dwSpawnID[n] = dwCreatureID;
				_stprintf(szTemp, _T("spawn_maxnum_%d"), n+1);
				pCreature->pNest->nSpawnMax[n] = m_pVar->GetInt(szTemp, szField);
			}
			else
			{
				break;
			}
		}
		pCreature->pNest->nCreatureNum = n;
		
	}

	// 怪物小队相关
	if( ECT_Monster == pCreature->eType && EMTT_Team == pCreature->nType2)
	{
		pCreature->pNest = new tagNestProto;
		pCreature->pNest->eOrderType = (ENPCTeamOrder)m_pVar->GetInt(_T("order_type"), szField);
		pCreature->pNest->fSpace = m_pVar->GetFloat(_T("team_space"), szField);

		DWORD dwCreatureID = GT_INVALID;
		TCHAR szTemp[X_LONG_STRING];
		INT n = 0;
		for(; n < MAX_SPAWN_CREATURE_NUM; n++)
		{
			_stprintf(szTemp, _T("spawn_id_%d"), n+1);
			dwCreatureID = m_pVar->GetDword(szTemp, szField, GT_INVALID);

			if( GT_INVALID != dwCreatureID )
			{
				pCreature->pNest->dwSpawnID[n] = dwCreatureID;
				_stprintf(szTemp, _T("spawn_maxnum_%d"), n+1);
				pCreature->pNest->nSpawnMax[n] = m_pVar->GetInt(szTemp, szField);
			}
			else
			{
				break;
			}
		}
		pCreature->pNest->nCreatureNum = n;

	}
	
	return &pCreature->dwTypeID;
}

//--------------------------------------------------------------------------------------------
// 初始化一条CreatureAI记录
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneCreatureAIProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pAI, pProtoType, tagCreatureAI);

	pAI->dwID				=	(DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	pAI->dwGroupID			=	m_pVar->GetDword(_T("group_id"), szField, 0);

	pAI->dwBuffTypeID[0]	=	m_pVar->GetDword(_T("buff_1"), szField, GT_INVALID);
	pAI->dwBuffTypeID[1]	=	m_pVar->GetDword(_T("buff_2"), szField, GT_INVALID);
	pAI->dwBuffTypeID[2]	=	m_pVar->GetDword(_T("buff_3"), szField, GT_INVALID);

	pAI->dwTriggerID[0]		=	m_pVar->GetDword(_T("event1_id"), szField, GT_INVALID);
	pAI->dwTriggerID[1]		=	m_pVar->GetDword(_T("event2_id"), szField, GT_INVALID);
	pAI->dwTriggerID[2]		=	m_pVar->GetDword(_T("event3_id"), szField, GT_INVALID);
	pAI->dwTriggerID[3]		=	m_pVar->GetDword(_T("event4_id"), szField, GT_INVALID);
	pAI->dwTriggerID[4]		=	m_pVar->GetDword(_T("event5_id"), szField, GT_INVALID);

	pAI->dwSkillTypeID[0]	=	m_pVar->GetDword(_T("skill1_id"), szField, GT_INVALID);
	pAI->dwSkillTypeID[1]	=	m_pVar->GetDword(_T("skill2_id"), szField, GT_INVALID);
	pAI->dwSkillTypeID[2]	=	m_pVar->GetDword(_T("skill3_id"), szField, GT_INVALID);
	pAI->dwSkillTypeID[3]	=	m_pVar->GetDword(_T("skill4_id"), szField, GT_INVALID);
	pAI->dwSkillTypeID[4]	=	m_pVar->GetDword(_T("skill5_id"), szField, GT_INVALID);

	pAI->nTargetType[0]		=	m_pVar->GetInt(_T("target1_type"), szField, 0);
	pAI->nTargetType[1]		=	m_pVar->GetInt(_T("target2_type"), szField, 0);
	pAI->nTargetType[2]		=	m_pVar->GetInt(_T("target3_type"), szField, 0);
	pAI->nTargetType[3]		=	m_pVar->GetInt(_T("target4_type"), szField, 0);
	pAI->nTargetType[4]		=	m_pVar->GetInt(_T("target5_type"), szField, 0);

	pAI->dwPatralTime		=	m_pVar->GetDword(_T("PatralTime"), szField, GT_INVALID);
	
	return &pAI->dwID;
}

//---------------------------------------------------------------------------------------------
// 怪物AI进行分组
//---------------------------------------------------------------------------------------------
VOID AttRes::GroupCreatureAI()
{
	tagCreatureAI* pAI = NULL;

	m_mapCreatureAI.ResetIterator();
	while( m_mapCreatureAI.PeekNext(pAI) )
	{
		if(0==pAI->dwGroupID)
			continue;

		// 根据AI的groupid确定其是否已经分组
		TList<DWORD>* pList = m_mapCreatureAIGroup.Peek(pAI->dwGroupID);

		if( P_VALID(pList) )
		{
			pList->PushBack(pAI->dwID);
		}
		else
		{
			pList = new TList<DWORD>;
			pList->PushBack(pAI->dwID);
			m_mapCreatureAIGroup.Add(pAI->dwGroupID, pList);
		}
	}
}

//-------------------------------------------------------------------------------------------
// 可铭纹装备部位的一条记录
//-------------------------------------------------------------------------------------------
VOID AttRes::InitOnePosyPosProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	tagConsolidatePos *pConsolidatePos = (tagConsolidatePos*)pArray + nIndex * MAX_CONSOLIDATE_POS_QUANTITY;

	pConsolidatePos->byConsolidate = (BYTE)m_pVar->GetDword(_T("right_hand"), szField, 0);
	pConsolidatePos->ePos = EEP_RightHand;
	pConsolidatePos++;
	pConsolidatePos->byConsolidate = (BYTE)m_pVar->GetDword(_T("head"), szField, 0);
	pConsolidatePos->ePos = EEP_Head;
	pConsolidatePos++;
	pConsolidatePos->byConsolidate = (BYTE)m_pVar->GetDword(_T("body"), szField, 0);
	pConsolidatePos->ePos = EEP_Body;
	pConsolidatePos++;
	pConsolidatePos->byConsolidate = (BYTE)m_pVar->GetDword(_T("leg"), szField, 0);
	pConsolidatePos->ePos = EEP_Legs;
	pConsolidatePos++;
	pConsolidatePos->byConsolidate = (BYTE)m_pVar->GetDword(_T("wrist"), szField, 0);
	pConsolidatePos->ePos = EEP_Wrist;
	pConsolidatePos++;
	pConsolidatePos->byConsolidate = (BYTE)m_pVar->GetDword(_T("feet"), szField, 0);
	pConsolidatePos->ePos = EEP_Feet;
	pConsolidatePos++;
	pConsolidatePos->byConsolidate = (BYTE)m_pVar->GetDword(_T("back"), szField, 0);
	pConsolidatePos->ePos = EEP_Back;
	pConsolidatePos++;
	pConsolidatePos->byConsolidate = (BYTE)m_pVar->GetDword(_T("face"), szField, 0);
	pConsolidatePos->ePos = EEP_Face;
	pConsolidatePos++;
	pConsolidatePos->byConsolidate = (BYTE)m_pVar->GetDword(_T("neck"), szField, 0);
	pConsolidatePos->ePos = EEP_Neck;
	pConsolidatePos++;
	pConsolidatePos->byConsolidate = (BYTE)m_pVar->GetDword(_T("finger1"), szField, 0);
	pConsolidatePos->ePos = EEP_Finger1;
	pConsolidatePos++;
	pConsolidatePos->byConsolidate = (BYTE)m_pVar->GetDword(_T("waist"), szField, 0);
	pConsolidatePos->ePos = EEP_Waist;
}

//-------------------------------------------------------------------------------------------
// 称号的一条记录
//-------------------------------------------------------------------------------------------
VOID AttRes::InitOneTitleProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	tagTitleProto *pTitleProto = static_cast<tagTitleProto *>( pArray ) + nIndex;
	
	pTitleProto->m_u16ID = (UINT16)m_pVar->GetDword(_T("id"), szField, 0);

	UINT16 u16Index = (UINT16)nIndex;
	UINT16 u16ID	= pTitleProto->m_u16ID;
	ASSERT(u16ID == u16Index);
	if(u16ID != u16Index)
	{
		MAttResCaution(FileName_Title_Proto, _T("TitleProto"), pTitleProto->m_u16ID);
	}

//	pTitleProto->m_u16TypeID = (UINT16)m_pVar->GetDword(_T("typeid"), szField, GT_INVALID);		//类型ID
	pTitleProto->m_dwTimeLimit = (DWORD)m_pVar->GetDword(_T("timelimit"), szField, GT_INVALID);	//有效期

	pTitleProto->m_dwBuffID = (DWORD)m_pVar->GetDword(_T("buffid"), szField, GT_INVALID);
	pTitleProto->m_CondType = (EnumConditionType)m_pVar->GetDword(_T("condid"), szField, 0);
	pTitleProto->m_dwPara1 = (DWORD)m_pVar->GetDword(_T("para1"), szField, GT_INVALID);
	pTitleProto->m_dwPara2 = (DWORD)m_pVar->GetDword(_T("para2"), szField, GT_INVALID);
	
	EnumTitleEvent eteEvent = (EnumTitleEvent)m_pVar->GetDword(_T("event1id"), szField, 0);
	if ( !GT_VALID(pTitleProto->m_Events[0] = eteEvent) ) return;
	return;
	eteEvent = (EnumTitleEvent)m_pVar->GetDword(_T("event2id"), szField, GT_INVALID);
	if ( !GT_VALID(pTitleProto->m_Events[1] = eteEvent) ) return;
	eteEvent = (EnumTitleEvent)m_pVar->GetDword(_T("event3id"), szField, GT_INVALID);
	if ( !GT_VALID(pTitleProto->m_Events[2] = eteEvent) ) return;
	eteEvent = (EnumTitleEvent)m_pVar->GetDword(_T("event4id"), szField, GT_INVALID);
	if ( !GT_VALID(pTitleProto->m_Events[3] = eteEvent) ) return;
	eteEvent = (EnumTitleEvent)m_pVar->GetDword(_T("event5id"), szField, GT_INVALID);
	if ( !GT_VALID(pTitleProto->m_Events[4] = eteEvent) ) return;
}

//-------------------------------------------------------------------------------------------
// 氏族珍宝的一条记录
//-------------------------------------------------------------------------------------------
VOID AttRes::InitOneClanTreasureProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	tagClanTreasureProto* pTreasureProto = static_cast<tagClanTreasureProto*>(pArray) + nIndex;

	UINT16 dwID							= (UINT16)		m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pTreasureProto->dwTypeID			= (UINT32)		m_pVar->GetDword(_T("itemid"), szField, GT_INVALID);
	pTreasureProto->dwNpcID				= (DWORD)		m_pVar->GetDword(_T("npcid"), szField, GT_INVALID);
	LPCTSTR	tszMapName					= m_pVar->GetString(_T("mapid"), szField, _T(""));
	pTreasureProto->dwMapID				= m_pUtil->Crc32(tszMapName);
	pTreasureProto->eClanType			= (ECLanType)	m_pVar->GetDword(_T("clantype"), szField, GT_INVALID);
	pTreasureProto->nActClanConNeed		= (INT32)		m_pVar->GetDword(_T("act_clancon_need"), szField, GT_INVALID);

// 	if (!GT_VALID(pTreasureProto->dwTypeID) ||
// 		!GT_VALID(pTreasureProto->dwNpcID)	||
// 		!GT_VALID(pTreasureProto->dwMapID)	||
// 		!GT_VALID(pTreasureProto->eClanType)||
// 		!GT_VALID(pTreasureProto->nActClanConNeed) ||
// 		dwID != nIndex)
// 	{
// 		MAttResCaution(FileName_ClanTreasure, _T("treasureProtoID"), dwID);
// 	}
}


//-------------------------------------------------------------------------------------------
// 初始化商城免费物品(仅有一个)
//-------------------------------------------------------------------------------------------
VOID AttRes::InitOneMallFreeItemProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	tagMallFreeItem *p = (tagMallFreeItem*)pArray + nIndex - 1;
	p->dwTypeID		= m_pVar->GetDword(_T("free_item_id"), szField, GT_INVALID);
	p->byNum		= (BYTE)m_pVar->GetDword(_T("free_num"), szField, 0);
	p->nUnitPrice	= m_pVar->GetDword(_T("free_item_price"), szField, 0);
}

//-------------------------------------------------------------------------------------------
// 初始化商城配置文件
//-------------------------------------------------------------------------------------------
VOID AttRes::InitOneMallCfgProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	INT32* pVal = (INT32*)pArray;

	pVal[nIndex] = m_pVar->GetInt(_T("value"), szField, 0);
}

//-------------------------------------------------------------------------------------------
// 得到强化属性对应的装备是否可强化
//-------------------------------------------------------------------------------------------
BOOL AttRes::IsPosyPos(EPosyAtt ePosyAtt, EEquipPos eEquipPos)
{
	for( INT n = 0; n < MAX_CONSOLIDATE_POS_QUANTITY; ++n)
	{
		if(m_PosyPos[ePosyAtt][n].ePos == eEquipPos)
			return (BOOL)m_PosyPos[ePosyAtt][n].byConsolidate;
	}
	
	return FALSE;
}

//--------------------------------------------------------------------------------------------
// 初始化铭纹的一条记录
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOnePosyProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pPosyProto, pProtoType, tagPosyProtoSer);

	pPosyProto->dwID = (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));

	_tcscpy( pPosyProto->szName, m_pVar->GetString(_T("name"), szField, _T("")) );
	pPosyProto->ePosyAtt = (EPosyAtt)m_pVar->GetDword(_T("att"), szField, GT_INVALID);
	pPosyProto->byPosyTimes = (BYTE)m_pVar->GetDword(_T("times"), szField, GT_INVALID);
	pPosyProto->nSuccessRate = m_pVar->GetDword(_T("success_rate"), szField, GT_INVALID);
	pPosyProto->nPotValConsume = m_pVar->GetDword(_T("potval_consume"), szField, 0);
	pPosyProto->dwMoneyConsume = m_pVar->GetDword(_T("money_consume"), szField, 0);
	pPosyProto->fcoefficientA = (FLOAT)m_pVar->GetFloat(_T("coefficientA"), szField, GT_INVALID);
	pPosyProto->fcoefficientB = (FLOAT)m_pVar->GetFloat(_T("coefficientB"), szField, GT_INVALID);
	pPosyProto->fcoefficientC = (FLOAT)m_pVar->GetFloat(_T("coefficientC"), szField, GT_INVALID);

	for( INT n = 1; n <= MAX_CONSOLIDATE_STUFF_QUANTITY; ++n )
	{
		TCHAR szTemp[X_LONG_STRING];
		wsprintf(szTemp, _T("stuff%d_id"), n);
		pPosyProto->ConsolidateStuff[n - 1].dwStuffID = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("stuff%d_type"), n);
		pPosyProto->ConsolidateStuff[n - 1].eStuffType = (EStuffType)m_pVar->GetDword(szTemp, szField, EST_Null);
		wsprintf(szTemp, _T("stuff%d_num"), n);
		pPosyProto->ConsolidateStuff[n - 1].dwStuffNum = m_pVar->GetDword(szTemp, szField, GT_INVALID);

		if(pPosyProto->ConsolidateStuff[n - 1].dwStuffNum != GT_INVALID)
			pPosyProto->nTotleStuff += pPosyProto->ConsolidateStuff[n - 1].dwStuffNum;
	}

	return &pPosyProto->dwID;
}


//-------------------------------------------------------------------------------------------
// 初始化镌刻的一条记录
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitOneEngraveProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pEngraveProto, pProtoType, tagEngraveProtoSer);

	pEngraveProto->dwID = (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));

	_tcscpy( pEngraveProto->szName, m_pVar->GetString(_T("name"), szField, _T("")) );
	pEngraveProto->eEngraveAtt = (EEngraveAtt)m_pVar->GetDword(_T("att"), szField, GT_INVALID);
	pEngraveProto->byEngraveTimes = (BYTE)m_pVar->GetDword(_T("times"), szField, GT_INVALID);
	pEngraveProto->nSuccessRate = m_pVar->GetDword(_T("success_rate"), szField, GT_INVALID);
	pEngraveProto->nPotValConsume = m_pVar->GetDword(_T("potval_consume"), szField, 0);
	pEngraveProto->dwMoneyConsume = m_pVar->GetDword(_T("money_consume"), szField, 0);
	pEngraveProto->fcoefficientA = (FLOAT)m_pVar->GetFloat(_T("coefficientA"), szField, GT_INVALID);
	pEngraveProto->fcoefficientB = (FLOAT)m_pVar->GetFloat(_T("coefficientB"), szField, GT_INVALID);
	pEngraveProto->fcoefficientC = (FLOAT)m_pVar->GetFloat(_T("coefficientC"), szField, GT_INVALID);

	for( INT n = 1; n <= MAX_CONSOLIDATE_STUFF_QUANTITY; ++n )
	{
		TCHAR szTemp[X_LONG_STRING];
		wsprintf(szTemp, _T("stuff%d_id"), n);
		pEngraveProto->ConsolidateStuff[n - 1].dwStuffID = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("stuff%d_type"), n);
		pEngraveProto->ConsolidateStuff[n - 1].eStuffType = (EStuffType)m_pVar->GetDword(szTemp, szField, EST_Null);
		wsprintf(szTemp, _T("stuff%d_num"), n);
		pEngraveProto->ConsolidateStuff[n - 1].dwStuffNum = m_pVar->GetDword(szTemp, szField, GT_INVALID);

		if(pEngraveProto->ConsolidateStuff[n - 1].dwStuffNum != GT_INVALID)
			pEngraveProto->nTotleStuff += pEngraveProto->ConsolidateStuff[n - 1].dwStuffNum;
	}

	return &pEngraveProto->dwID;
}

//-------------------------------------------------------------------------------------------
// 得到强化属性对应的装备是否可镌刻
//-------------------------------------------------------------------------------------------
BOOL AttRes::IsEngravePos(EEngraveAtt ePosyAtt, EEquipPos eEquipPos)
{
	for( INT n = 0; n < MAX_CONSOLIDATE_POS_QUANTITY; ++n)
	{
		if(m_EngravePos[ePosyAtt][n].ePos == eEquipPos)
			return (BOOL)m_EngravePos[ePosyAtt][n].byConsolidate;
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------
// 初始化镶嵌,烙印,龙附的一条记录
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneConsolidateProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pConsolidateProto, pProtoType, tagConsolidateItem);

	pConsolidateProto->dwID = (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	
	pConsolidateProto->dwPotValConsume = m_pVar->GetDword(_T("potval_consume"), szField, GT_INVALID);
	pConsolidateProto->nSuccessRate = m_pVar->GetDword(_T("success_rate"), szField, GT_INVALID);
	pConsolidateProto->eWuXing = (EWuXing)m_pVar->GetDword(_T("loongsoul_att"), szField, GT_INVALID);
	
	for( INT n = 0; n < MAX_CONSOLIDATE_ROLEATT; ++n )
	{
		TCHAR szTemp[X_LONG_STRING];
		wsprintf(szTemp, _T("role_att%d"), n);
		pConsolidateProto->tagRoleAtt[n].eRoleAtt = (ERoleAttribute)m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("att_val%d"), n);
		pConsolidateProto->tagRoleAtt[n].nAttVal = m_pVar->GetInt(szTemp, szField, 0);
	}
	
	pConsolidateProto->ConsolidatePos[0].byConsolidate = (BYTE)m_pVar->GetDword(_T("right_hand"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[0].ePos = EEP_RightHand;
	pConsolidateProto->ConsolidatePos[1].byConsolidate = (BYTE)m_pVar->GetDword(_T("head"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[1].ePos = EEP_Head;
	pConsolidateProto->ConsolidatePos[2].byConsolidate = (BYTE)m_pVar->GetDword(_T("body"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[2].ePos = EEP_Body;
	pConsolidateProto->ConsolidatePos[3].byConsolidate = (BYTE)m_pVar->GetDword(_T("leg"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[3].ePos = EEP_Legs;
	pConsolidateProto->ConsolidatePos[4].byConsolidate = (BYTE)m_pVar->GetDword(_T("wrist"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[4].ePos = EEP_Wrist;
	pConsolidateProto->ConsolidatePos[5].byConsolidate = (BYTE)m_pVar->GetDword(_T("feet"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[5].ePos = EEP_Feet;
	pConsolidateProto->ConsolidatePos[6].byConsolidate = (BYTE)m_pVar->GetDword(_T("back"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[6].ePos = EEP_Back;
	pConsolidateProto->ConsolidatePos[7].byConsolidate = (BYTE)m_pVar->GetDword(_T("face"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[7].ePos = EEP_Face;
	pConsolidateProto->ConsolidatePos[8].byConsolidate = (BYTE)m_pVar->GetDword(_T("neck"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[8].ePos = EEP_Neck;
	pConsolidateProto->ConsolidatePos[9].byConsolidate = (BYTE)m_pVar->GetDword(_T("finger1"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[9].ePos = EEP_Finger1;
	pConsolidateProto->ConsolidatePos[10].byConsolidate = (BYTE)m_pVar->GetDword(_T("waist"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[10].ePos = EEP_Waist;
	pConsolidateProto->ConsolidatePos[11].byConsolidate = (BYTE)m_pVar->GetDword(_T("hunjing"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[11].ePos = EEP_HunJing;
	//v3.6.2 元神装备强化属性，只读取可强化的TypeEx
	pConsolidateProto->ConsolidatePos[12].byConsolidate = (BYTE)m_pVar->GetDword(_T("soulequip1"), szField, GT_INVALID);	
	pConsolidateProto->ConsolidatePos[13].byConsolidate = (BYTE)m_pVar->GetDword(_T("soulequip2"), szField, GT_INVALID);
	pConsolidateProto->ConsolidatePos[14].byConsolidate = (BYTE)m_pVar->GetDword(_T("soulequip3"), szField, GT_INVALID);

	return &pConsolidateProto->dwID;
}

//--------------------------------------------------------------------------------------------
// 初始化生产技能的一条记录
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneProduceProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pProduceProto, nProtoType, tagProduceProtoSer);

	pProduceProto->dwID = (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));

	pProduceProto->eProType = (EProduceType)m_pVar->GetDword(_T("produce_type"), szField, GT_INVALID);
	pProduceProto->eComType = (EComposeType)m_pVar->GetDword(_T("compose_type"), szField, GT_INVALID);
	pProduceProto->eFormFrom = (EFormulaFrom)m_pVar->GetDword(_T("formula_from"), szField, GT_INVALID);
	pProduceProto->nFormLvl= m_pVar->GetDword(_T("formula_level"), szField, GT_INVALID);
	pProduceProto->dwProItemTypeID = m_pVar->GetDword(_T("compose_item"), szField, GT_INVALID);
	pProduceProto->dwProQuantity = m_pVar->GetDword(_T("compose_num"), szField, GT_INVALID);
	pProduceProto->nSuccessRate = m_pVar->GetDword(_T("success_rate"), szField, GT_INVALID);
	pProduceProto->dwVitalityConsume = m_pVar->GetDword(_T("vitality_consume"), szField, 0);
	pProduceProto->dwMasterIncre = m_pVar->GetDword(_T("master_incre"), szField, 0);
	pProduceProto->dwMoneyConsume = m_pVar->GetDword(_T("money_consume"), szField, 0);
	pProduceProto->dwQltyFixLimit = m_pVar->GetDword(_T("qlty_fix_limit"), szField, 0);

	for( INT n = 1; n <= MAX_PRODUCE_STUFF_QUANTITY; ++n )
	{
		TCHAR szTemp[X_LONG_STRING];
		wsprintf(szTemp, _T("stuff%d_id"), n);
		pProduceProto->ProduceStuff[n - 1].dwStuffID = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("stuff%d_type"), n);
		pProduceProto->ProduceStuff[n - 1].eStuffType = (EStuffType)m_pVar->GetDword(szTemp, szField, EST_Null);
		wsprintf(szTemp, _T("stuff%d_num"), n);
		pProduceProto->ProduceStuff[n - 1].dwStuffNum = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("fail_dep%d"), n);
		pProduceProto->ProduceStuff[n - 1].dwFailDeductionNum = m_pVar->GetDword(szTemp, szField, GT_INVALID);
	}

	return &pProduceProto->dwID;
}

//--------------------------------------------------------------------------------------------
// 点化,通用分解
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneDeComposeProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pDeComposeProto, nProtoType, tagDeComposeProtoSer);

	pDeComposeProto->dwID = (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));

	pDeComposeProto->eComType = (EComposeType)m_pVar->GetDword(_T("compose_type"), szField, GT_INVALID);
	pDeComposeProto->byLevel = (BYTE)m_pVar->GetDword(_T("level"), szField, GT_INVALID);
	pDeComposeProto->byQuality = (BYTE)m_pVar->GetDword(_T("quality"), szField, GT_INVALID);
	pDeComposeProto->eType = (EItemType)m_pVar->GetDword(_T("type"), szField, GT_INVALID);
	pDeComposeProto->eTypeEx = (EItemTypeEx)m_pVar->GetDword(_T("type_ex"), szField, GT_INVALID);
	pDeComposeProto->ePos = (EEquipPos)m_pVar->GetDword(_T("equip_pos"), szField, GT_INVALID);
	pDeComposeProto->nFormLvl = m_pVar->GetDword(_T("formula_level"), szField, 0);
	pDeComposeProto->eFormFrom = (EFormulaFrom)m_pVar->GetDword(_T("formula_from"), szField, GT_INVALID);
	pDeComposeProto->dwVitalityConsume = m_pVar->GetDword(_T("vitality_consume"), szField, 0);
	pDeComposeProto->dwMasterIncre = m_pVar->GetDword(_T("master_incre"), szField, 0);
	pDeComposeProto->dwMoneyConsume = m_pVar->GetDword(_T("money_consume"), szField, 0);

	for( INT n = 1; n <= MAX_OUTPUT_QUANTITY; ++n )
	{
		TCHAR szTemp[X_LONG_STRING];
		wsprintf(szTemp, _T("item%d_id"), n);
		pDeComposeProto->OutputStuff[n - 1].dwStuffTypeID = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("per_min%d_val"), n);
		pDeComposeProto->OutputStuff[n - 1].nPerMinVal = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("per_max%d_val"), n);
		pDeComposeProto->OutputStuff[n - 1].nPerMaxVal = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("suc_min%d_val"), n);
		pDeComposeProto->OutputStuff[n - 1].nSucMinVal = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("suc_max%d_val"), n);
		pDeComposeProto->OutputStuff[n - 1].nSucMaxVal = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("rate%d"), n);
		pDeComposeProto->OutputStuff[n - 1].nRate = m_pVar->GetDword(szTemp, szField, GT_INVALID);
	}

	return &pDeComposeProto->dwID;
}

//--------------------------------------------------------------------------------------------
// 萃取
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneExtractProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pProto, nProtoType, tagExtractProto);

	pProto->dwEquipID = (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));

	pProto->dwProduceTime = m_pVar->GetDword(_T("produce_time"), szField, 0);
	pProto->dwVitalityConsume = m_pVar->GetDword(_T("vitality_consume"), szField, 0);
	pProto->dwMoneyConsume = m_pVar->GetDword(_T("money_consume"), szField, 0);
	pProto->byQuality = (BYTE)m_pVar->GetInt(_T("quality"), szField, 0);

	for( INT n = 1; n <= MAX_EXTRACT_OUTPUT; ++n )
	{
		TCHAR szTemp[X_LONG_STRING];
		wsprintf(szTemp, _T("item%d_id"), n);
		pProto->dwItemID[n-1] = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("suc_min%d_val"), n);
		pProto->nItemMaxNum[n-1] = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("suc_max%d_val"), n);
		pProto->nItemMinNum[n-1] = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("rate%d"), n);
		pProto->dwRate[n-1] = m_pVar->GetDword(szTemp, szField, GT_INVALID);
	}

	return &pProto->dwEquipID;
}

//--------------------------------------------------------------------------------------------
// 淬火
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneQuenchProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pQuenchProto, nProtoType, tagQuenchProtoSer);

	pQuenchProto->dwID = (DWORD)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	pQuenchProto->srcQuenchAtt.eWuXing = (EWuXing)m_pVar->GetDword(_T("src_att"), szField, GT_INVALID);
	pQuenchProto->srcQuenchAtt.nWuXingValue = m_pVar->GetDword(_T("src_att_value"), szField, 0);
	pQuenchProto->dstQuenchAtt.eWuXing = (EWuXing)m_pVar->GetDword(_T("dst_att"), szField, GT_INVALID);
	pQuenchProto->dstQuenchAtt.nWuXingValue = m_pVar->GetDword(_T("dst_att_value"), szField, 0);
	pQuenchProto->dwPotValConsume = m_pVar->GetDword(_T("potval_consume"), szField, 0);
	pQuenchProto->dwMoneyConsume = m_pVar->GetDword(_T("money_consume"), szField, 0);
	pQuenchProto->nSuccessRate = m_pVar->GetDword(_T("success_rate"), szField, GT_INVALID);

	for( INT n = 1; n <= MAX_CONSOLIDATE_STUFF_QUANTITY; ++n )
	{
		TCHAR szTemp[X_LONG_STRING];
		wsprintf(szTemp, _T("stuff%d_id"), n);
		pQuenchProto->ConsolidateStuff[n - 1].dwStuffID = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		wsprintf(szTemp, _T("stuff%d_type"), n);
		pQuenchProto->ConsolidateStuff[n - 1].eStuffType = (EStuffType)m_pVar->GetDword(szTemp, szField, EST_Null);
		wsprintf(szTemp, _T("stuff%d_num"), n);
		pQuenchProto->ConsolidateStuff[n - 1].dwStuffNum = m_pVar->GetDword(szTemp, szField, 0);
	}

	return &pQuenchProto->dwID;
}

//--------------------------------------------------------------------------------------------
// 副本中不能使用的物品
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneInsItemProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pInsItem, nProtoType, tagInstanceItem);

	LPCTSTR szMapName = (LPCTSTR)m_pVar->GetString(_T("id"), szField);
	pInsItem->dwMapID = m_pUtil->Crc32(szMapName);

	INT nItemNum = m_pVar->GetDword(_T("ItemNum"), szField, 0);

	for( INT n = 1; n <= nItemNum; ++n)
	{
		TCHAR szTemp[X_LONG_STRING];
		wsprintf(szTemp, _T("ItemID%d"), n);
		DWORD	dwItemID = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		pInsItem->mapInstanceItem.Add(dwItemID, dwItemID);
	}

	return &pInsItem->dwMapID;
}

//--------------------------------------------------------------------------------------------
// 副本中不能使用的技能
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneInsSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pInsSkill, nProtoType, tagInstanceSkill);

	LPCTSTR szMapName = (LPCTSTR)m_pVar->GetString(_T("id"), szField);
	pInsSkill->dwMapID = m_pUtil->Crc32(szMapName);
	INT nItemNum = m_pVar->GetDword(_T("SkillNum"), szField, 0);

	for( INT n = 1; n <= nItemNum; ++n)
	{
		TCHAR szTemp[X_LONG_STRING];
		wsprintf(szTemp, _T("SkillID%d"), n);
		DWORD	dwSkillID = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		pInsSkill->mapInstanceSkill.Add(dwSkillID, dwSkillID);
	}

	return &pInsSkill->dwMapID;
}

//--------------------------------------------------------------------------------------------
// 副本随机刷怪点
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneSpawnPointProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pSpawnPoint, nProtoType, tagRandSpawnPointInfo);
	
	DWORD		dwSpawnPointID = m_pVar->GetDword(_T("SpawnPoint"), szField, GT_INVALID);
	INT			nLevel = m_pVar->GetDword(_T("Level"), szField, 0);

	pSpawnPoint->dwSpawnPointID = dwSpawnPointID + (DWORD)nLevel;
	pSpawnPoint->nLevel = nLevel;
	pSpawnPoint->dwNormalID[0] = m_pVar->GetDword(_T("NormalID1"), szField, GT_INVALID);
	pSpawnPoint->dwNormalID[1] = m_pVar->GetDword(_T("NormalID2"), szField, GT_INVALID);
	pSpawnPoint->dwNormalID[2] = m_pVar->GetDword(_T("NormalID3"), szField, GT_INVALID);
	pSpawnPoint->dwEliteID[0] = m_pVar->GetDword(_T("EliteID1"), szField, GT_INVALID);
	pSpawnPoint->dwEliteID[1] = m_pVar->GetDword(_T("EliteID2"), szField, GT_INVALID);
	pSpawnPoint->dwEliteID[2] = m_pVar->GetDword(_T("EliteID3"), szField, GT_INVALID);
	pSpawnPoint->dwDevilID[0] = m_pVar->GetDword(_T("DevilIDl"), szField, GT_INVALID);
	pSpawnPoint->dwDevilID[1] = m_pVar->GetDword(_T("DevilID2"), szField, GT_INVALID);
	pSpawnPoint->dwDevilID[2] = m_pVar->GetDword(_T("DevilID3"), szField, GT_INVALID);
	
	return &pSpawnPoint->dwSpawnPointID;
}

//--------------------------------------------------------------------------------------------
// 副本动态刷怪点等级映射表
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneLevelMapping(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pLevelMapping, nProtoType, tagLevelMapping);

	pLevelMapping->nLevel = (INT32)_atoi64(m_pUtil->UnicodeToUnicode8(szField));
	pLevelMapping->nTransmitLevel = m_pVar->GetDword(_T("Level"), szField, 0);

	return &pLevelMapping->nLevel;	
}

//--------------------------------------------------------------------------------------------
// 初始化副本静态数据
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneInstanceProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pInstance, nProtoType, tagInstance);

	LPCTSTR szMapName = (LPCTSTR)m_pVar->GetString(_T("id"), szField);
	pInstance->dwMapID = m_pUtil->Crc32(szMapName);
	pInstance->dwEndTime = m_pVar->GetDword(_T("EndTime"), szField, GT_INVALID);
	pInstance->dwTimeLimit = m_pVar->GetDword(_T("TimeLimit"), szField, GT_INVALID);
	pInstance->eInstanceMapType = (EInstanceMapType)m_pVar->GetDword(_T("InstanceMode"), szField, 0);
	pInstance->eInstanceCreateMode = (EInstanceCreateMode)m_pVar->GetDword(_T("CreateMode"), szField, 0);
	pInstance->bAskEnter = ((BOOL)m_pVar->GetDword(_T("AskEnter"), szField) ? true : false);
	pInstance->bSelectHard = ((BOOL)m_pVar->GetDword(_T("SelectHard"), szField) ? true : false);
	pInstance->bSelectNormal = ((BOOL)m_pVar->GetDword(_T("SelectNormal"), szField) ? true : false);
	pInstance->bSelectElite = ((BOOL)m_pVar->GetDword(_T("SelectElite"), szField) ? true : false);
	pInstance->bSelectDevil = ((BOOL)m_pVar->GetDword(_T("SelectDevil"), szField) ? true : false);
	pInstance->bNoticeTeamate = ((BOOL)m_pVar->GetDword(_T("NoticeTeamate"), szField) ? true : false);
	pInstance->nNumDownLimit = m_pVar->GetDword(_T("NumDownLimit"), szField, 0);
	pInstance->nNumUpLimit = m_pVar->GetDword(_T("NumUpLimit"), szField, GT_INVALID);
	pInstance->nLevelDownLimit = m_pVar->GetDword(_T("LevelDownLimit"), szField, 0);
	pInstance->nLevelUpLimit = m_pVar->GetDword(_T("LevelUpLimit"), szField, 0);
	pInstance->eExportMode = (EExportMode)m_pVar->GetDword(_T("ExportMode"), szField, 0);

	LPCTSTR	szExportMapName = (LPCTSTR)m_pVar->GetString(_T("ExportMapID"), szField);
	pInstance->dwExportMapID = m_pUtil->Crc32(szExportMapName);
	pInstance->vExportPos.x= m_pVar->GetFloat(_T("x"), szField, 0);
	pInstance->vExportPos.y = m_pVar->GetFloat(_T("y"), szField, 0);
	pInstance->vExportPos.z = m_pVar->GetFloat(_T("z"), szField, 0);
	pInstance->vEnterPos.x = m_pVar->GetFloat(_T("EnterX"), szField, 0);
	pInstance->vEnterPos.y = m_pVar->GetFloat(_T("EnterY"), szField, 0);
	pInstance->vEnterPos.z = m_pVar->GetFloat(_T("EnterZ"), szField, 0);
	pInstance->dwTargetLimit = m_pVar->GetDword(_T("TargetLimit"), szField, 0);
	pInstance->bCombat = ((BOOL)m_pVar->GetDword(_T("IsCombat"), szField) ? true : false);
	pInstance->bPK = ((BOOL)m_pVar->GetDword(_T("IsPK"), szField) ? true : false);
	pInstance->bLoseSafeguard = ((BOOL)m_pVar->GetDword(_T("IsloseSafeGuard"), szField) ? true : false);
	pInstance->bPKPenalty = ((BOOL)m_pVar->GetDword(_T("IsPKPenalty"), szField) ? true : false);
	pInstance->bDeadPenalty = ((BOOL)m_pVar->GetDword(_T("IsDeadPenalty"), szField) ? true : false);
	pInstance->bMount = ((BOOL)m_pVar->GetDword(_T("IsMount"), szField) ? true : false);
	pInstance->bTransmit = ((BOOL)m_pVar->GetDword(_T("IsTransmit"), szField) ? true : false);
	pInstance->eRebornMode = (ERebornMode)m_pVar->GetDword(_T("RebornMode"), szField, 0);
	pInstance->eCompleteNor = (ECompleteConditionNor)m_pVar->GetDword(_T("CompleteConditionNor"), szField, 0);
	pInstance->dwCompleteNorVal1 = m_pVar->GetDword(_T("CompleteValNor1"), szField, GT_INVALID);
	pInstance->dwCompleteNorVal2 = m_pVar->GetDword(_T("CompleteValNor2"), szField, GT_INVALID);
	pInstance->eCompleteEli = (ECompleteConditionEli)m_pVar->GetDword(_T("CompleteConditionEli"), szField, 0);
	pInstance->dwCompleteEliVal1 = m_pVar->GetDword(_T("CompleteValEli1"), szField, GT_INVALID);
	pInstance->dwCompleteEliVal2 = m_pVar->GetDword(_T("CompleteValEli2"), szField, GT_INVALID);
	pInstance->eCompleteDev = (ECompleteConditionDev)m_pVar->GetDword(_T("CompleteConditionDev"), szField, 0);
	pInstance->dwCompleteDevVal1 = m_pVar->GetDword(_T("CompleteValDev1"), szField, GT_INVALID);
	pInstance->dwCompleteDevVal2 = m_pVar->GetDword(_T("CompleteValDev2"), szField, GT_INVALID);
	pInstance->eCompleteEvent = (ECompleteEvent)m_pVar->GetDword(_T("CompleteEvent"), szField, 0);
	pInstance->nWearyValue = m_pVar->GetDword(_T("weary_value"), szField, 0);	//读取疲劳积累值

	return &pInstance->dwMapID;
}

//--------------------------------------------------------------------------------------------
// 初始化非副本刷怪点原型
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneSSpawnPointProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pSSpawnPoint, nProtoType, tagSSpawnPointProto);

	pSSpawnPoint->dwSpawnPointID = (DWORD)m_pVar->GetDword(_T("id"), szField);

	for (INT nCreatureIndex = 0; nCreatureIndex < MAX_CREATURE_PER_SSPAWNPOINT; ++nCreatureIndex)
	{
		TCHAR pTchTmp[X_LONG_STRING];
		wsprintf(pTchTmp, _T("CreatureID%d"), nCreatureIndex + 1);
		pSSpawnPoint->dwTypeIDs[nCreatureIndex] = (DWORD)m_pVar->GetDword(pTchTmp, szField, GT_INVALID);
	}

	return &pSSpawnPoint->dwSpawnPointID;
}

//--------------------------------------------------------------------------------------------
// 初始化宠物技能原型
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOnePetSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pPetSkillProto, nProtoType, tagPetSkillProto);

	pPetSkillProto->dwTypeID		= (DWORD)			m_pVar->GetDword(_T("id"), szField);
	pPetSkillProto->eType			= (EPetskillType)	m_pVar->GetInt(_T("type1"), szField, EPT2_Null);
	pPetSkillProto->eType2			= (EPetskillType2)	m_pVar->GetInt(_T("type2"), szField, EPT_Null);
	pPetSkillProto->nType3			= m_pVar->GetInt(_T("type3"), szField, 0);
	pPetSkillProto->eCastType		= (EPetskillCastType)	m_pVar->GetInt(_T("cast_type"), szField, 0);
	pPetSkillProto->nSkillLevel		= m_pVar->GetInt(_T("level"), szField, 0);
	pPetSkillProto->byCast_condition	= (BYTE)	m_pVar->GetInt(_T("cast_condition"), szField, GT_INVALID);
	pPetSkillProto->nCooldownTick	= (INT)(m_pVar->GetInt(_T("cooldown_time"), szField, 0) / TICK_TIME);
	pPetSkillProto->nWorkTimeTick	= (INT)(m_pVar->GetInt(_T("work_time"), szField, 0) / TICK_TIME);
	pPetSkillProto->nWuxing_cost	= m_pVar->GetInt(_T("wuxing_cost"), szField, 0);
	pPetSkillProto->nSpirit_cost	= m_pVar->GetInt(_T("spirit_cost"), szField, 0);
	pPetSkillProto->nWuxing_add		= m_pVar->GetInt(_T("wuxing_add"), szField, 0);
	pPetSkillProto->nBuffid			= m_pVar->GetInt(_T("buffid"), szField, GT_INVALID);
	pPetSkillProto->bLearn_condition= (BOOL)m_pVar->GetInt(_T("learn_condition"), szField, 0);
	pPetSkillProto->nLearn_prob		= m_pVar->GetInt(_T("learn_prob"), szField, 0);
	pPetSkillProto->nLearn_step		= m_pVar->GetInt(_T("learn_step"), szField, 0);
	pPetSkillProto->nLearn_grade	= m_pVar->GetInt(_T("learn_grade"), szField, 0);
	pPetSkillProto->nLearn_PontentialCost	= m_pVar->GetInt(_T("potential_cost"), szField, 0);

	pPetSkillProto->nPetLvlLim		= m_pVar->GetInt(_T("learn_rolelvl"), szField, GT_INVALID);

	pPetSkillProto->AttIndexs[0]	= (INT)m_pVar->GetInt(_T("att1"), szField, GT_INVALID);
	pPetSkillProto->AttIndexs[1]	= (INT)m_pVar->GetInt(_T("att2"), szField, GT_INVALID);
	pPetSkillProto->AttIndexs[2]	= (INT)m_pVar->GetInt(_T("att3"), szField, GT_INVALID);
	pPetSkillProto->AttIndexs[3]	= (INT)m_pVar->GetInt(_T("att4"), szField, GT_INVALID);
	pPetSkillProto->AttIndexs[4]	= (INT)m_pVar->GetInt(_T("att5"), szField, GT_INVALID);
	pPetSkillProto->AttIndexs[5]	= (INT)m_pVar->GetInt(_T("att6"), szField, GT_INVALID);

	pPetSkillProto->AttMods[0]		= (INT)m_pVar->GetInt(_T("mod1"), szField, 0);
	pPetSkillProto->AttMods[1]		= (INT)m_pVar->GetInt(_T("mod2"), szField, 0);
	pPetSkillProto->AttMods[2]		= (INT)m_pVar->GetInt(_T("mod3"), szField, 0);
	pPetSkillProto->AttMods[3]		= (INT)m_pVar->GetInt(_T("mod4"), szField, 0);
	pPetSkillProto->AttMods[4]		= (INT)m_pVar->GetInt(_T("mod5"), szField, 0);
	pPetSkillProto->AttMods[5]		= (INT)m_pVar->GetInt(_T("mod6"), szField, 0);

	pPetSkillProto->nPetAttIndex	= (INT)m_pVar->GetInt(_T("pet_att"), szField, GT_INVALID);
	pPetSkillProto->nPetAttMod		= (INT)m_pVar->GetInt(_T("pet_mod"), szField, 0);

	return &pPetSkillProto->dwTypeID;

}

//--------------------------------------------------------------------------------------------
// 初始化宠物技能原型
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOnePetEquipProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pPetEquipProto, nProtoType, tagPetEquipProto);

	pPetEquipProto->dwTypeID	= (DWORD)m_pVar->GetDword(_T("id"), szField, GT_INVALID);

	pPetEquipProto->nType			= (INT)m_pVar->GetInt(_T("type"), szField, GT_INVALID);
	pPetEquipProto->nStep			= (INT)m_pVar->GetInt(_T("step"), szField, GT_INVALID);
	pPetEquipProto->nGrade			= (INT)m_pVar->GetInt(_T("grade"), szField, GT_INVALID);

	pPetEquipProto->bTypeUnique		= (BOOL)m_pVar->GetDword(_T("unique_same_type"), szField, FALSE);
	pPetEquipProto->bUnique			= (BOOL)m_pVar->GetDword(_T("unique"), szField, FALSE);

	pPetEquipProto->nPetAtt[0]		= (INT)m_pVar->GetInt(_T("att1"), szField, GT_INVALID);
	pPetEquipProto->nPetAtt[1]		= (INT)m_pVar->GetInt(_T("att2"), szField, GT_INVALID);
	pPetEquipProto->nPetAtt[2]		= (INT)m_pVar->GetInt(_T("att3"), szField, GT_INVALID);

	pPetEquipProto->nPetAttMod[0]	= (INT)m_pVar->GetInt(_T("mod1"), szField, 0);
	pPetEquipProto->nPetAttMod[1]	= (INT)m_pVar->GetInt(_T("mod2"), szField, 0);
	pPetEquipProto->nPetAttMod[2]	= (INT)m_pVar->GetInt(_T("mod3"), szField, 0);

	return &pPetEquipProto->dwTypeID;

}

//--------------------------------------------------------------------------------------------
// 初始化宠物技能原型
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOnePetProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pPetProto, nProtoType, tagPetProto);

	pPetProto->dwTypeID		= (DWORD)m_pVar->GetDword(_T("id"), szField, GT_INVALID);

	if (!GT_VALID(pPetProto->dwTypeID))
	{
		ASSERT(0);
		MAttResCaution(FileName_Pet_Proto, _T("invalide pet protoid"), pPetProto->dwTypeID);
	}

	pPetProto->nType3		= (INT)m_pVar->GetInt(_T("type3"), szField, 0);
	pPetProto->nRoleLvlLim	= (INT)m_pVar->GetInt(_T("rolelvl"), szField, 0);
	pPetProto->nMountable	= (INT)m_pVar->GetInt(_T("mountable"), szField, 0);
	pPetProto->nMountSpeed	= (INT)m_pVar->GetInt(_T("mountspeed"), szField, 0);
	pPetProto->bFlyable		= (BOOL)m_pVar->GetInt(_T("flyable"), szField, 0);

	pPetProto->fScale		= (FLOAT)m_pVar->GetFloat(_T("scale"), szField, 10000) / 10000.0f;

	pPetProto->vSize.x	= m_pVar->GetFloat(_T("box_x"), szField, X_DEF_ROLE_SIZE_X);
	pPetProto->vSize.y	= m_pVar->GetFloat(_T("box_y"), szField, X_DEF_ROLE_SIZE_Y);
	pPetProto->vSize.z	= m_pVar->GetFloat(_T("box_z"), szField, X_DEF_ROLE_SIZE_Z);

	pPetProto->bBind	= m_pVar->GetDword(_T("bind"), szField, FALSE);

	pPetProto->dwLifeTime = m_pVar->GetDword(_T("life"), szField, GT_INVALID);

	//aptitude1_min
	for (INT nIndex = EIQ_White; nIndex < EIQ_End; ++nIndex)
	{
		TCHAR pTchTmp[X_LONG_STRING];

		wsprintf(pTchTmp, _T("aptitude%d_min"), nIndex + 1);
		pPetProto->nAptitudeMin[nIndex]		= (DWORD)m_pVar->GetDword(pTchTmp, szField, 0);

		wsprintf(pTchTmp, _T("aptitude%d_max"), nIndex + 1);
		pPetProto->nAptitudeMax[nIndex]		= (DWORD)m_pVar->GetDword(pTchTmp, szField, 0);
	}

	pPetProto->dwAddBuffID = m_pVar->GetDword(_T("addbuff"), szField, GT_INVALID);
	pPetProto->dwItemID = m_pVar->GetDword(_T("itemID"), szField, GT_INVALID);

	return &pPetProto->dwTypeID;

}

//-------------------------------------------------------------------------------------------
// 学习等级比较函数对象
//-------------------------------------------------------------------------------------------
class PSidComp
{
public:
	PSidComp(AttRes* pRes):m_pRes(pRes){}
	bool operator()(const DWORD& left, const DWORD& right)
	{
		return m_pRes->GetPetSkillProto(left)->nLearn_prob < m_pRes->GetPetSkillProto(right)->nLearn_prob;
	}
private:
	AttRes*	m_pRes;
};

//-------------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------------
VOID AttRes::InitPetSkillsVec()
{
	TMap<DWORD, tagPetSkillProto*>::TMapIterator itr =  m_mapPetSkillProto.Begin();
	
	tagPetSkillProto* pProto = NULL;

	while(m_mapPetSkillProto.PeekNext(itr, pProto))
	{
		if (!P_VALID(pProto)) break;
		INT nVLvl = 1;
		TransStepGrade2VLevel(pProto->nLearn_step, pProto->nLearn_grade, nVLvl);
		m_PetLevelSkillVec[nVLvl - 1].push_back(pProto->dwTypeID);
	}

	for (INT i = 0; i < NUM_PET_VLEVEL; ++i)
	{
		m_PetLevelSkillVec[i].sort(PSidComp(this));
	//	std::sort(m_PetLevelSkillVec[i].begin(), m_PetLevelSkillVec[i].end(), );
	}

}

//-------------------------------------------------------------------------------------------
// 初始化帮派某个职位操作权限
//-------------------------------------------------------------------------------------------
VOID AttRes::InitOnePosGuildPower(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	if(!::IsGuildPosValid(EGuildMemberPos(nIndex))
		|| _atoi64(m_pUtil->UnicodeToUnicode8(szField)) != nIndex)
	{
		MAttResCaution(_T("guild_power.xml"), _T("invalid pos"), nIndex);
		return;
	}

	MTRANS_POINTER(p, pArray, tagGuildPower);

	p[nIndex].bDismissGuild		= m_pVar->GetInt(_T("DismissGuild"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bInviteJoin		= m_pVar->GetInt(_T("InviteJoin"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bTurnoverLeader	= m_pVar->GetInt(_T("TurnoverLeader"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bLeaveGuild		= m_pVar->GetInt(_T("LeaveGuild"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bDemissPostion	= m_pVar->GetInt(_T("DemissPostion"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bModifyTenet		= m_pVar->GetInt(_T("ModifyTenet"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bModifySgin		= m_pVar->GetInt(_T("ModifySgin"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bSetWareRights	= m_pVar->GetInt(_T("SetWareRights"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bUpgrade			= m_pVar->GetInt(_T("Upgrade"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bCommerce			= m_pVar->GetInt(_T("Commerce"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bSetCommend		= m_pVar->GetInt(_T("SetCommend"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bAffair			= m_pVar->GetInt(_T("Affair"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bSetSkill			= m_pVar->GetInt(_T("SetSkill"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bAdvSkill			= m_pVar->GetInt(_T("AdvSkill"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bSetTaxRate		= m_pVar->GetInt(_T("SetTaxRate"), szField, 0) == 0 ? 0 : 1;
	p[nIndex].bTransformTax		= m_pVar->GetInt(_T("TransformTax"), szField, 0) == 0 ? 0 : 1;
}

//-------------------------------------------------------------------------------------------
// 初始化帮派某个职位开革权限
//-------------------------------------------------------------------------------------------
VOID AttRes::InitOnePosGuildKick(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	if(!::IsGuildPosValid(EGuildMemberPos(nIndex))
		|| _atoi64(m_pUtil->UnicodeToUnicode8(szField)) != nIndex)
	{
		MAttResCaution(_T("guild_power.xml"), _T("invalid pos"), nIndex);
		return;
	}

	MTRANS_POINTER(p, pArray, tagGuildKick);

#if 0
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_BangZhong]		= m_pVar->GetInt(_T("BangZhong"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_JingYing]		= m_pVar->GetInt(_T("JingYing"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_BangZhu]		= m_pVar->GetInt(_T("BangZhu"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_FuBangZhu]		= m_pVar->GetInt(_T("FuBangZhu"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_HongHuFa]		= m_pVar->GetInt(_T("HongHuFa"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_ZiHuFa]			= m_pVar->GetInt(_T("ZiHuFa"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_QingLongTang]	= m_pVar->GetInt(_T("QingLongTang"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_QingLongXiang]	= m_pVar->GetInt(_T("QingLongXiang"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_QingLongZhong]	= m_pVar->GetInt(_T("QingLongZhong"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_ZhuQueTang]		= m_pVar->GetInt(_T("ZhuQueTang"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_ZhuQueXiang]	= m_pVar->GetInt(_T("ZhuQueXiang"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_ZhuQueZhong]	= m_pVar->GetInt(_T("ZhuQueZhong"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_BaiHuTang]		= m_pVar->GetInt(_T("BaiHuTang"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_BaiHuXiang]		= m_pVar->GetInt(_T("BaiHuXiang"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_BaiHuZhong]		= m_pVar->GetInt(_T("BaiHuZhong"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_XuanWuTang]		= m_pVar->GetInt(_T("XuanWuTang"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_XuanWuXiang]	= m_pVar->GetInt(_T("XuanWuXiang"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_XuanWuZhong]	= m_pVar->GetInt(_T("XuanWuZhong"), szField, 0) == 0 ? 0 : 1;
#else
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_HuiYuan]	= m_pVar->GetInt(_T("HuiYuan"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_JingYing]		= m_pVar->GetInt(_T("JingYing"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_HuiZhang]		= m_pVar->GetInt(_T("HuiZhang"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_FuHuiZhang]	= m_pVar->GetInt(_T("FuHuiZhang"), szField, 0) == 0 ? 0 : 1;
	m_GuildKickDefault[nIndex].BitSetGuildKick[EGMP_GuGan]		= m_pVar->GetInt(_T("GuGan"), szField, 0) == 0 ? 0 : 1;
#endif
}

//-------------------------------------------------------------------------------------------
// 初始化帮派某个职位任职权限
//-------------------------------------------------------------------------------------------
VOID AttRes::InitOnePosGuildAppoint(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	if(!::IsGuildPosValid(EGuildMemberPos(nIndex))
		|| _atoi64(m_pUtil->UnicodeToUnicode8(szField)) != nIndex)
	{
		MAttResCaution(_T("guild_power.xml"), _T("invalid pos"), nIndex);
		return;
	}

	MTRANS_POINTER(p, pArray, tagGuildAppoint);

#if 0
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_BangZhong]	= m_pVar->GetInt(_T("BangZhong"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_JingYing]		= m_pVar->GetInt(_T("JingYing"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_BangZhu]		= m_pVar->GetInt(_T("BangZhu"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_FuBangZhu]	= m_pVar->GetInt(_T("FuBangZhu"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_HongHuFa]		= m_pVar->GetInt(_T("HongHuFa"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_ZiHuFa]		= m_pVar->GetInt(_T("ZiHuFa"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_QingLongTang]	= m_pVar->GetInt(_T("QingLongTang"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_QingLongXiang]= m_pVar->GetInt(_T("QingLongXiang"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_QingLongZhong]= m_pVar->GetInt(_T("QingLongZhong"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_ZhuQueTang]	= m_pVar->GetInt(_T("ZhuQueTang"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_ZhuQueXiang]	= m_pVar->GetInt(_T("ZhuQueXiang"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_ZhuQueZhong]	= m_pVar->GetInt(_T("ZhuQueZhong"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_BaiHuTang]	= m_pVar->GetInt(_T("BaiHuTang"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_BaiHuXiang]	= m_pVar->GetInt(_T("BaiHuXiang"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_BaiHuZhong]	= m_pVar->GetInt(_T("BaiHuZhong"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_XuanWuTang]	= m_pVar->GetInt(_T("XuanWuTang"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_XuanWuXiang]	= m_pVar->GetInt(_T("XuanWuXiang"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_XuanWuZhong]	= m_pVar->GetInt(_T("XuanWuZhong"), szField, 0) == 0 ? 0 : 1;
#else
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_HuiYuan]	= m_pVar->GetInt(_T("HuiYuan"), szField, 0) == 0 ? 0 : 1;
		m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_JingYing]		= m_pVar->GetInt(_T("JingYing"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_HuiZhang]		= m_pVar->GetInt(_T("HuiZhang"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_FuHuiZhang]	= m_pVar->GetInt(_T("FuHuiZhang"), szField, 0) == 0 ? 0 : 1;
	m_GuildAppointDefault[nIndex].BitSetGuildAppoint[EGMP_GuGan]		= m_pVar->GetInt(_T("GuGan"), szField, 0) == 0 ? 0 : 1;
#endif
}

//-------------------------------------------------------------------------------------
// 加载宠物属性默认值、最小及最大值
//-------------------------------------------------------------------------------------
// VOID AttRes::InitPetAttDefMinMax(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
// {
// 	tagPetDefMinMax* pAttDefMinMax = (tagPetDefMinMax*)pArray;
// 
// 	pAttDefMinMax[nIndex].nDef		=	m_pVar->GetInt(_T("def"), szField, 0);
// 	pAttDefMinMax[nIndex].nMin		=	m_pVar->GetInt(_T("min"), szField, INT_MIN);
// 	pAttDefMinMax[nIndex].nMax		=	m_pVar->GetInt(_T("max"), szField, INT_MAX);
// }


VOID* AttRes::InitOnePetLvlUpProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pPetLvlUp, nProtoType, tagPetLvlUpProto);

	pPetLvlUp->dwVLevel	= (DWORD)m_pVar->GetDword(_T("id"), szField, GT_INVALID);

	pPetLvlUp->nStep		= (INT)m_pVar->GetDword(_T("step"), szField, 0);
	pPetLvlUp->nGrade		= (INT)m_pVar->GetDword(_T("grade"), szField, 0);

	pPetLvlUp->nExpLvlUpNeed			= (INT)m_pVar->GetDword(_T("exp"), szField, 0);
	pPetLvlUp->nMoneyRatePourExpNeed	= (INT)m_pVar->GetDword(_T("money"), szField, 0);

	INT nVLevel = 0;
	TransStepGrade2VLevel(pPetLvlUp->nStep, pPetLvlUp->nGrade, nVLevel);

	ASSERT(nVLevel == pPetLvlUp->dwVLevel);

	return &pPetLvlUp->dwVLevel;
}

VOID* AttRes::InitOnePetGatherProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pPetGather, nProtoType, tagPetGatherProto);

	pPetGather->dwTypeID	= (DWORD)m_pVar->GetDword(_T("id"), szField, GT_INVALID);

	for (INT nIndex = 0; nIndex < NUM_ITEMS_PET_GATHER; ++nIndex)
	{
		TCHAR pTchTmp[X_LONG_STRING];

		wsprintf(pTchTmp, _T("item_id%d"), nIndex + 1);
		pPetGather->dwItemTypeID[nIndex]	= (DWORD)m_pVar->GetDword(pTchTmp, szField, GT_INVALID);

		wsprintf(pTchTmp, _T("min%d"), nIndex + 1);
		pPetGather->nMin[nIndex]		= (DWORD)m_pVar->GetInt(pTchTmp, szField, 0);

		wsprintf(pTchTmp, _T("max%d"), nIndex + 1);
		pPetGather->nMax[nIndex]		= (DWORD)m_pVar->GetInt(pTchTmp, szField, 0);
	}

	for (INT nIndex = 0; nIndex < NUM_RARE_ITEMS_PET_GATHER; ++nIndex)
	{
		TCHAR pTchTmp[X_LONG_STRING];

		wsprintf(pTchTmp, _T("rare_item_id%d"), nIndex + 1);
		pPetGather->dwRareItemID[nIndex]	= (DWORD)m_pVar->GetDword(pTchTmp, szField, GT_INVALID);

		wsprintf(pTchTmp, _T("prob%d"), nIndex + 1);
		pPetGather->nProb[nIndex]		= (DWORD)m_pVar->GetInt(pTchTmp, szField, 0);
	}


	return &pPetGather->dwTypeID;
}

//-------------------------------------------------------------------------------------------
// 初始化VIP摊位租金
//-------------------------------------------------------------------------------------------
VOID AttRes::InitOneVIPStallProto( OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex )
{
	INT32* pRent = (INT32*)pArray;

	pRent[nIndex] = m_pVar->GetInt(_T("rent"), szField, 0);
}

VOID* AttRes::InitOnePetWuXingProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pPetWuXing, nProtoType, tagPetWuXingProto);

	pPetWuXing->dwTypeID	= (DWORD)m_pVar->GetDword(_T("id"), szField, GT_INVALID);

	for (INT nIndex = 0; nIndex < MAX_WUXING_ITEM_NUM; ++nIndex)
	{
		TCHAR pTchTmp[X_LONG_STRING];

		wsprintf(pTchTmp, _T("item%d"), nIndex + 1);
		pPetWuXing->dwItemTypeID[nIndex]	= (DWORD)m_pVar->GetDword(pTchTmp, szField, GT_INVALID);

		wsprintf(pTchTmp, _T("num%d"), nIndex + 1);
		pPetWuXing->nNum[nIndex]		= m_pVar->GetInt(pTchTmp, szField, 0);

		wsprintf(pTchTmp, _T("prob%d"), nIndex + 1);
		pPetWuXing->nProb[nIndex]		= m_pVar->GetInt(pTchTmp, szField, 0);

		wsprintf(pTchTmp, _T("notice%d"), nIndex + 1);
		pPetWuXing->bNotice[nIndex]		= (BOOL)m_pVar->GetDword(pTchTmp, szField, 0);
	}

	return &pPetWuXing->dwTypeID;
}

VOID* AttRes::InitOneVNBGiftProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pVNBGiftProto, nProtoType, tagVNBGiftProto);

	pVNBGiftProto->dwID			= (DWORD)m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pVNBGiftProto->dwItemTypeID	= (DWORD)m_pVar->GetDword(_T("typeid"), szField, GT_INVALID);
	pVNBGiftProto->nNum			= (INT)m_pVar->GetDword(_T("num"), szField, 0);

	if (!GT_VALID(pVNBGiftProto->dwID))
	{
		MAttResCaution(_T("vip_netbar_gift_proto.xml"), _T("invalid id"), GT_INVALID);
	}
	
	return &pVNBGiftProto->dwID;
}

VOID* AttRes::InitOneVNBEquipProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pVNBEquipProto, nProtoType, tagVNBEquipProto);

	pVNBEquipProto->dwID			= (DWORD)m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pVNBEquipProto->dwEquipTypeID	= (DWORD)m_pVar->GetDword(_T("typeid"), szField, GT_INVALID);
	pVNBEquipProto->nQuality		= (INT)m_pVar->GetDword(_T("quality"), szField, 0);

	if (!GT_VALID(pVNBEquipProto->dwID))
	{
		MAttResCaution(_T("vip_netbar_equip_proto.xml"), _T("invalid id"), GT_INVALID);
	}

	return &pVNBEquipProto->dwID;
}

//--------------------------------------------------------------------------------------------
// 初始化帮派设施升级需求信息
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneGuildUpgradeProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	BYTE byType		= (BYTE)m_pVar->GetDword(_T("type"), szField, GT_INVALID);
	BYTE byLevel	= (BYTE)m_pVar->GetDword(_T("level"), szField, 0);

	MTRANS_POINTER(pUpgradeInfo, nProtoType, tagGuildUpgradeNeed);
	pUpgradeInfo->dwKey			= byType - 1;
	pUpgradeInfo->dwKey			= (pUpgradeInfo->dwKey << 16) | byLevel;

	pUpgradeInfo->n16FullFill	= (INT16)m_pVar->GetDword(_T("fullfill"), szField, GT_INVALID);
	pUpgradeInfo->n16Step		= (INT16)m_pVar->GetDword(_T("step"), szField, GT_INVALID);
	pUpgradeInfo->nFund			= m_pVar->GetInt(_T("fund"), szField, GT_INVALID);
	pUpgradeInfo->nMaterial		= m_pVar->GetInt(_T("material"), szField, GT_INVALID);
	pUpgradeInfo->nBaseExploit	= m_pVar->GetInt(_T("exploit"), szField, GT_INVALID);

	for (int n=1; n<=20; n++)
	{
		tstringstream tss_type;
		tss_type << _T("item_type_") << n;
		tstringstream tss_num;
		tss_num << _T("item_num_") << n;

		tagItemNeedInfo sItemInfo;
		sItemInfo.dwItemTypeID	= m_pVar->GetDword(tss_type.str().c_str(), szField, GT_INVALID);
		sItemInfo.nItemNeedNum	= m_pVar->GetInt(tss_num.str().c_str(), szField, GT_INVALID);
		if (sItemInfo.IsValid())
		{
			pUpgradeInfo->listItemInfo.PushBack(sItemInfo);
		}
	}

	return &pUpgradeInfo->dwKey;
}

//-------------------------------------------------------------------------------
// 获取帮派设施升级需求信息
//-------------------------------------------------------------------------------
BOOL AttRes::GetGuildUpgradeItemInfo( BYTE eType, BYTE byLevel, OUT tagGuildFacilities& sInfo )
{
	DWORD dwKey = eType;
	dwKey = (dwKey << 16) | (byLevel + 1);

	tagGuildUpgradeNeed* pUpgradeProto = m_GuildUpgradeNeedInfo.Peek(dwKey);
	if (!P_VALID(pUpgradeProto))
	{
		sInfo.eType			= (EFacilitiesType)eType;
		sInfo.nNeedFund		= 0;
		sInfo.nMaterial		= 0;
		return FALSE;
	}

	sInfo.eType			= (EFacilitiesType)eType;
	sInfo.nFulfill		= pUpgradeProto->n16FullFill;
	sInfo.nStep			= pUpgradeProto->n16Step;
	sInfo.nBaseExploit	= pUpgradeProto->nBaseExploit;
	sInfo.nNeedFund		= pUpgradeProto->nFund;
	sInfo.nMaterial		= pUpgradeProto->nMaterial;

	// 随机获得4种物品
	TList<tagItemNeedInfo> listTemp = pUpgradeProto->listItemInfo;

	for (int n=0; n<MAX_UPGRADE_NEED_ITEM_TYPE; n++)
	{
		tagItemNeedInfo sItemInfo;
		if (!listTemp.RandPeek(sItemInfo, TRUE))
		{
			return FALSE;
		}

		sInfo.dwItemID[n]	= sItemInfo.dwItemTypeID;
		sInfo.nNeedNum[n]	= sItemInfo.nItemNeedNum;
	}

	return TRUE;
}

bool AttRes::GetServerFunctionSwitch(EServerFunctionSwitch eVal)
{
	if (eVal >= ESFS_End || eVal <= ESFS_Null)
	{
		return false;
	}

	return m_bsServerFunctionSwitch.at(eVal);
}

DWORD AttRes::GetServerFunctionSwitch()
{
	return m_bsServerFunctionSwitch.to_ulong();
}

BOOL AttRes::GetGuildUpgradeBaseInfo( BYTE eType, BYTE byLevel, OUT tagGuildFacilities& sInfo )
{
	DWORD dwKey = eType;
	dwKey = (dwKey << 16) | (byLevel + 1);

	tagGuildUpgradeNeed* pUpgradeProto = m_GuildUpgradeNeedInfo.Peek(dwKey);
	if (!P_VALID(pUpgradeProto))
	{
		return FALSE;
	}

	sInfo.eType			= (EFacilitiesType)eType;
	sInfo.nFulfill		= pUpgradeProto->n16FullFill;
	sInfo.nStep			= pUpgradeProto->n16Step;
	sInfo.nBaseExploit	= pUpgradeProto->nBaseExploit;
	sInfo.nNeedFund		= pUpgradeProto->nFund;
	sInfo.nMaterial		= pUpgradeProto->nMaterial;

	return TRUE;
}

//--------------------------------------------------------------------------------------------
// 初始化帮派事务信息
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneGuildAffairProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pAffairInfo, nProtoType, tagGuildAffairInfo);

	pAffairInfo->dwBuffID		= m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pAffairInfo->nFund			= m_pVar->GetInt(_T("fund"), szField, 0);
	pAffairInfo->nMaterial		= m_pVar->GetInt(_T("material"), szField, 0);
	pAffairInfo->byGuildLevel	= (BYTE)m_pVar->GetDword(_T("guild_level"), szField, 1);
	pAffairInfo->byHoldCity		= (BYTE)m_pVar->GetDword(_T("hold_city"), szField, 0);

	return &pAffairInfo->dwBuffID;
}

//--------------------------------------------------------------------------------------------
// 初始化帮派技能信息
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneGuildSkillProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pSkillInfo, nProtoType, tagGuildSkill);

	pSkillInfo->dwSkillID				= m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pSkillInfo->n16LearnRestrictLvl		= static_cast<INT16>(m_pVar->GetInt(_T("learn_restrictlevel"), szField, GT_INVALID));
	pSkillInfo->n16ResearchRestrictLvl	= static_cast<INT16>(m_pVar->GetInt(_T("research_restrictlevel"), szField, GT_INVALID));
	pSkillInfo->dwActiveItemID			= m_pVar->GetDword(_T("active_itemid"), szField, GT_INVALID);
	pSkillInfo->n16Fulfill				= static_cast<INT16>(m_pVar->GetInt(_T("fullfill"), szField, GT_INVALID));
	pSkillInfo->nLearnContribution		= m_pVar->GetInt(_T("learn_contribution"), szField, 0);
	pSkillInfo->nLearnFund				= m_pVar->GetInt(_T("learn_fund"), szField, 0);
	pSkillInfo->nLearnMaterial			= m_pVar->GetInt(_T("learn_material"), szField, 0);
	pSkillInfo->nLearnSilver			= m_pVar->GetInt(_T("learn_silver"), szField, 0);
	pSkillInfo->nResearchFund			= m_pVar->GetInt(_T("research_fund"), szField, 0);
	pSkillInfo->nResearchMaterial		= m_pVar->GetInt(_T("research_material"), szField, 0);

	return &pSkillInfo->dwSkillID;
}

// 初始化公会技能上限map
VOID AttRes::InitGuildSkillMaxLvlInfo()
{
	m_mapSkillMaxLvl.Clear();

	tagGuildSkill* pGuildSkill = NULL;
	TMap<DWORD, tagGuildSkill*>::TMapIterator iter = m_GuildSkillInfo.Begin();
	while (m_GuildSkillInfo.PeekNext(iter, pGuildSkill))
	{
		if (!P_VALID(pGuildSkill))
		{
			continue;
		}

		DWORD	dwSkillID	= pGuildSkill->dwSkillID / 100;
		INT		nLevel		= pGuildSkill->dwSkillID % 100;

		INT nPeekLvl = m_mapSkillMaxLvl.Peek(dwSkillID);
		if (nPeekLvl == GT_INVALID)
		{
			m_mapSkillMaxLvl.Add(dwSkillID, nLevel);
		}
		else if (nPeekLvl < nLevel)
		{
			m_mapSkillMaxLvl.ChangeValue(dwSkillID, nLevel);
		}
	}
}

INT AttRes::GetGuildSkillMaxLvl(DWORD dwSkillID)	// 取最大公会技能最大等级，没有返回-1
{
	return m_mapSkillMaxLvl.Peek(dwSkillID);
}

//--------------------------------------------------------------------------------------------
// 初始化帮派技能信息(创建帮派时)
//--------------------------------------------------------------------------------------------
BOOL AttRes::LoadGuildSkillInfo( TMap<DWORD, tagGuildSkill*>& mapGuildSkill )
{
	mapGuildSkill.Clear();

	tagGuildSkill* pGuildSkill = NULL;
	TMap<DWORD, tagGuildSkill*>::TMapIterator iter = m_GuildSkillInfo.Begin();
	while (m_GuildSkillInfo.PeekNext(iter, pGuildSkill))
	{
		if (!P_VALID(pGuildSkill))
		{
			continue;
		}

		DWORD	dwSkillID	= pGuildSkill->dwSkillID / 100;
		INT		nLevel		= pGuildSkill->dwSkillID % 100;
		if (nLevel == 1)
		{
			// 写入初始属性
			tagGuildSkill* pSkillInfo	= new tagGuildSkill(*pGuildSkill);
			pSkillInfo->dwSkillID		= dwSkillID;
			pSkillInfo->nLevel			= 1;
			pSkillInfo->bActive = (pSkillInfo->dwActiveItemID == 0);
			mapGuildSkill.Add(dwSkillID, pSkillInfo);
		}
	}

	return TRUE;
}

//--------------------------------------------------------------------------------------------
// 读取帮派技能静态属性
//--------------------------------------------------------------------------------------------
BOOL AttRes::GetGuildSkillInfo( DWORD dwSkillID, INT nLevel, tagGuildSkill& sGuildSkill )
{
	DWORD dwGuildSkillID = dwSkillID * 100 + nLevel;

	sGuildSkill.dwSkillID				= dwSkillID;
	sGuildSkill.nLevel					= nLevel;

	tagGuildSkill* pSkillInfo = m_GuildSkillInfo.Peek(dwGuildSkillID);
	if (!P_VALID(pSkillInfo))
	{
		return FALSE;
	}

	sGuildSkill.n16LearnRestrictLvl		= pSkillInfo->n16LearnRestrictLvl;
	sGuildSkill.n16ResearchRestrictLvl	= pSkillInfo->n16ResearchRestrictLvl;
	sGuildSkill.dwActiveItemID			= pSkillInfo->dwActiveItemID;
	sGuildSkill.n16Fulfill				= pSkillInfo->n16Fulfill;
	sGuildSkill.nLearnContribution		= pSkillInfo->nLearnContribution;
	sGuildSkill.nLearnFund				= pSkillInfo->nLearnFund;
	sGuildSkill.nLearnMaterial			= pSkillInfo->nLearnMaterial;
	sGuildSkill.nLearnSilver			= pSkillInfo->nLearnSilver;
	sGuildSkill.nResearchFund			= pSkillInfo->nResearchFund;
	sGuildSkill.nResearchMaterial		= pSkillInfo->nResearchMaterial;

	return TRUE;
}

//--------------------------------------------------------------------------------------------
// 初始化帮派跑商信息
//--------------------------------------------------------------------------------------------
VOID* AttRes::InitOneGuildCommerceProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pCommerceInfo, nProtoType, tagCommerceInfo);

	pCommerceInfo->dwID							= m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pCommerceInfo->sTaelInfo.nDeposit			= m_pVar->GetInt(_T("deposit"), szField, 0);
	pCommerceInfo->sTaelInfo.nBeginningTael		= m_pVar->GetInt(_T("beginning_tael"), szField, 0);
	pCommerceInfo->sTaelInfo.nPurposeTael		= m_pVar->GetInt(_T("purpose_tael"), szField, 0);
	pCommerceInfo->sTaelInfo.nMaxTael			= m_pVar->GetInt(_T("max_tael"), szField, 0);
	pCommerceInfo->sRedoundInfo.nExp			= m_pVar->GetInt(_T("exp"), szField, 0);
	pCommerceInfo->sRedoundInfo.nContribution	= m_pVar->GetInt(_T("contribution"), szField, 0);
	pCommerceInfo->sRedoundInfo.nExploit		= m_pVar->GetInt(_T("exploit"), szField, 0);

	return &pCommerceInfo->dwID;
}

//-------------------------------------------------------------------------------
// 获取帮派跑商信息
//-------------------------------------------------------------------------------
const tagCommerceInfo* AttRes::GetGuildCommerceInfo( INT nLevel )
{
	DWORD dwID				= GT_INVALID;
	tagCommerceInfo* pInfo	= NULL;

	TMap<DWORD, tagCommerceInfo*>::TMapIterator iter = m_GuildCommerceInfo.Begin();
	while (m_GuildCommerceInfo.PeekNext(iter, dwID, pInfo))
	{
		if (!GT_VALID(dwID) || !P_VALID(pInfo))
		{
			continue;
		}

		// 计算等级范围
		if ((nLevel >= (INT)(dwID / 1000)) && (nLevel <= (INT)(dwID % 1000)))
		{
			return pInfo;
		}
	}

	// 没找到
	return NULL;
}

//-------------------------------------------------------------------------------
// 初始化商货信息
//-------------------------------------------------------------------------------
VOID* AttRes::InitOneCommodityProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pProto, nProtoType, tagCommodityProto);

	pProto->dwGoodID			= m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pProto->byHolderID			= (BYTE)m_pVar->GetDword(_T("holder"), szField, 0);
	pProto->nLowPrice			= m_pVar->GetInt(_T("low_price"), szField, 0);
	pProto->nHighPrice			= m_pVar->GetInt(_T("high_price"), szField, 0);
	pProto->byRefreshNum		= (BYTE)m_pVar->GetDword(_T("refresh_num"), szField, 1);

	return &pProto->dwGoodID;
}

//-------------------------------------------------------------------------------
// 初始化商会信息
//-------------------------------------------------------------------------------
VOID* AttRes::InitOneCofCProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pProto, nProtoType, tagCofCProto);

	pProto->n64Key		= m_pVar->GetDword(_T("purchase_CofC"), szField, GT_INVALID);
	DWORD dwID			= m_pVar->GetDword(_T("origin_CofC"), szField, GT_INVALID);
	pProto->n64Key		= (pProto->n64Key << 32) | dwID;
	pProto->fProfit		= m_pVar->GetFloat(_T("profit"), szField, 0);

	return &pProto->n64Key;
}

//-------------------------------------------------------------------------------
// 初始化特产商信息
//-------------------------------------------------------------------------------
VOID* AttRes::InitOneCofCSPProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pProto, nProtoType, tagCofCSPProto);

	pProto->dwCofCID	= m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pProto->byHolderID	= (BYTE)m_pVar->GetDword(_T("city"), szField, 0);//特产商会(0表示一般商会)

	return &pProto->dwCofCID;
}

//-------------------------------------------------------------------------------
// 读取卖出商货收益率
//-------------------------------------------------------------------------------
const FLOAT AttRes::GetCofCProfit( DWORD dwDstID, DWORD dwSrcID )
{
	INT64 n64Key = dwDstID;
	n64Key = (n64Key << 32) | dwSrcID;

	tagCofCProto* pProto = m_CofCProto.Peek(n64Key);
	if (!P_VALID(pProto))
	{
		return GT_INVALID;
	}

	return pProto->fProfit;
}

//-------------------------------------------------------------------------------
// 载入商会商货信息
//-------------------------------------------------------------------------------
BOOL AttRes::LoadCofCGoodInfo( DWORD dwCofCID, TMap<DWORD, tagCofCSellGood*>& mapGoodSell,
							  TMap<DWORD, tagCofCBuyGood*>& mapGoodBuy )
{
	if (!GT_VALID(dwCofCID))
	{
		return FALSE;
	}
	mapGoodSell.Clear();
	mapGoodBuy.Clear();

	DWORD dwGoodID				= GT_INVALID;
	tagCommodityProto* pProto	= NULL;
	TMap<DWORD, tagCommodityProto*>::TMapIterator iter = m_GuildCommodityProto.Begin();
	while (m_GuildCommodityProto.PeekNext(iter, dwGoodID, pProto))
	{
		if (!GT_VALID(dwGoodID) || !P_VALID(pProto))
		{
			continue;
		}

		// 售卖商货信息
		if (pProto->byHolderID == dwCofCID)
		{
			tagCofCSellGood* pSellGood	= new tagCofCSellGood;
			pSellGood->dwGoodID			= dwGoodID;
			pSellGood->byRemainNum		= pProto->byRefreshNum;
			pSellGood->nCost			= m_pUtil->RandomInRange(pProto->nLowPrice, pProto->nHighPrice);
			mapGoodSell.Add(dwGoodID, pSellGood);
		}
		
		// 收购商货信息
		tagCofCBuyGood* pBuyGood	= new tagCofCBuyGood;
		pBuyGood->dwGoodID			= dwGoodID;
		pBuyGood->nCost				= m_pUtil->RandomInRange(pProto->nLowPrice, pProto->nHighPrice);
		mapGoodBuy.Add(dwGoodID, pBuyGood);
	}

	return TRUE;
}

VOID* AttRes::InitOnePetLvlUpItemProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pPetLvlUp, nProtoType, tagPetLvlUpItemProto);

	pPetLvlUp->dwTypeID	= (DWORD)m_pVar->GetDword(_T("id"), szField, GT_INVALID);


	for (INT nIndex = 0; nIndex < MAX_PET_STEP_UPGRADE_SKILL_NUM; ++nIndex)
	{
		TCHAR pTchTmp[X_LONG_STRING];

		wsprintf(pTchTmp, _T("skill%d"), nIndex + 1);
		pPetLvlUp->dwSkillIDs[nIndex]	= (DWORD)m_pVar->GetDword(pTchTmp, szField, GT_INVALID);
	}

	return &pPetLvlUp->dwTypeID;
}

VOID* AttRes::InitOneMotionProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pMotionProto, nProtoType, tagMotionProto);

	pMotionProto->dwTypeID		= (DWORD)m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pMotionProto->eMotionType	= (EMotionType1)m_pVar->GetDword(_T("type"), szField, GT_INVALID);
	pMotionProto->dwFriendVal	= (DWORD)m_pVar->GetDword(_T("friendval"), szField, 0);

	return &pMotionProto->dwTypeID;
}



VOID* AttRes::InitQuestUseSpeciItemProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pQusetProto, nProtoType, tagQuestUseSpeciItem);
	
	pQusetProto->n16QuestID		= (INT16)m_pVar->GetInt(_T("id"), szField, 0);
	pQusetProto->dwItemID		= (DWORD)m_pVar->GetDword(_T("item_id"), szField, 0);
	pQusetProto->n16ItemNum		= (INT16)m_pVar->GetInt(_T("item_number"), szField, 0);
	
	return&pQusetProto->n16QuestID;
}

VOID* AttRes::InitKillObjExp(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pKillExp, nProtoType, tagKillerExp);
	pKillExp->n16Level	= (INT16) m_pVar->GetInt(_T("id"), szField, 0);
	pKillExp->dwExp		= (DWORD) m_pVar->GetDword(_T("Exp"), szField, 0);
	pKillExp->dwBaseRwdExp		= (DWORD) m_pVar->GetDword(_T("BaseRwdExp"), szField, 0);
	pKillExp->dwBaseRwdSilver		= (DWORD) m_pVar->GetDword(_T("BaseRwdSilver"), szField, 0);
	return &pKillExp->n16Level; 
}

tagConfigLessingOfLoong* AttRes::GetConfigInfoOfLessingLoong(const int & ndx)
{
	return m_mapConfigInfoOfLessingLoong.Peek(ndx);
}
VOID* AttRes::InitLessingLoong(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(p, nProtoType, tagConfigLessingOfLoong);

	p->nID					= m_pVar->GetInt(_T("id"), szField,0);
	p->nExpireTime			= m_pVar->GetInt(_T("ExpireTime"), szField, 0) * 60;
	p->nExperience			= m_pVar->GetInt64(_T("Experience"), szField, 0);
	p->dwRewardItemTypeID	= m_pVar->GetDword( _T("RewardItemID"), szField, (DWORD)GT_INVALID );
	p->nItemCount			= m_pVar->GetInt  ( _T("ItemCount"), szField, 0 );
	p->nQlty				= m_pVar->GetInt  (_T("Quality"),szField,EIQ_Null);

	return &p->nID;
}

VOID* AttRes::InitPhilosophersStoneConfig(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(p, nProtoType, tagPhilosophersStoneConfig);

	p->uniKey.m_Pair.nStoneType			= m_pVar->GetInt(_T("GoldStoneType"), szField, 0) ;
	//p->dwDestItemTypeID	= m_pVar->GetDword( _T("DestItemID"), szField, (DWORD)GT_INVALID );
	tagGoldStone temStone ;
	TCHAR attName[20] = {0};
	for( INT i = 1; i < 21 ; ++i)
	{
		_stprintf(attName,_T("DestItemID%d"),i);
		temStone.dwTypeID = m_pVar->GetDword( attName, szField, (DWORD)GT_INVALID );
		_stprintf(attName,_T("DestItemPCT%d"),i);
		temStone.nPct = m_pVar->GetInt( attName, szField, 0 );
		if( temStone.dwTypeID != GT_INVALID && temStone.nPct > 0 )
		{
			p->vecGoldStone.push_back(temStone);
		}
	}
	p->uniKey.m_Pair.dwSrcItemTypeID	= m_pVar->GetDword( _T("ScrItemID"), szField, (DWORD)GT_INVALID );

	return &p->uniKey.m_n64Key;
}
tagPhilosophersStoneConfig* AttRes::GetConfigInfoOfPhilosophersStone(INT nStoneType,DWORD srcItemTypeID)
{
	tagPhilosophersStoneConfig::unionKey key;
	key.m_Pair.nStoneType = nStoneType;
	key.m_Pair.dwSrcItemTypeID = srcItemTypeID;
	return m_mapPhilosophersStoneConfig.Peek(key.m_n64Key);
}
//-------------------------------------------------------------------------------
// 初始化仙界技能
//-------------------------------------------------------------------------------
VOID* AttRes::InitMiracleSkillProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pProto, nProtoType, tagMiracleSkill);

	pProto->dwSkillTypeID	=	m_pVar->GetDword(_T("id"), szField, GT_INVALID);

	pProto->dwSilverCost	= m_pVar->GetDword(_T("learn_silver"), szField, 0);
	pProto->n16PowerCost	= (INT16)m_pVar->GetInt(_T("learn_power"), szField, 0);
	pProto->dwItemTypeID	= m_pVar->GetDword(_T("learn_item_typeid"), szField, GT_INVALID);
	pProto->n16ItemNumCost	= (INT16)m_pVar->GetInt(_T("learn_item_num"), szField, 0);

	return &pProto->dwSkillTypeID;
}

//-------------------------------------------------------------------------------
// 初始化城市属性
//-------------------------------------------------------------------------------
VOID* AttRes::InitOneCityProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pProto, nProtoType, tagCityProto);

	pProto->dwCityID		= m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pProto->eClanType		= (ECLanType)m_pVar->GetInt(_T("clan_type"), szField, ECLT_NULL);
	pProto->nFundOutput		= m_pVar->GetInt(_T("fund_output"), szField, 0);
	pProto->nMaterialOutput	= m_pVar->GetInt(_T("material_output"), szField, 0);

	LPCTSTR szShellMapName	= m_pVar->GetString(_T("shell_city"), szField, _T(""));
	LPCTSTR szKernelMapName	= m_pVar->GetString(_T("kernel_city"), szField, _T(""));
	pProto->dwShellID		= m_pUtil->Crc32(szShellMapName);
	pProto->dwKernelID		= m_pUtil->Crc32(szKernelMapName);
	pProto->dwMinSigupFare	= m_pVar->GetDword(_T("min_sigfare"), szField, GT_INVALID); //最低报名费用

	pProto->dwDefaultDefence	= m_pVar->GetDword(_T("default_defence"), szField, 0); //默认防守值

	pProto->dwActivityID = m_pVar->GetDword(_T("activityid"), szField, GT_INVALID);


	for (int n=1; n<=MAX_CITY_ITEM_OUTPUT; n++)
	{
		tstringstream tss_type;
		tss_type << _T("item_type_") << n;
		tstringstream tss_num;
		tss_num << _T("item_num_") << n;
		tstringstream tss_limit;
		tss_limit << _T("item_limit_") << n;

		pProto->sItemOutput[n-1].dwTypeID	= m_pVar->GetDword(tss_type.str().c_str(), szField, GT_INVALID);
		pProto->sItemOutput[n-1].nItemNum	= m_pVar->GetInt(tss_num.str().c_str(), szField, 0);
		pProto->sItemOutput[n-1].nLimit	= m_pVar->GetInt(tss_limit.str().c_str(), szField, 0);
	}
	return &pProto->dwCityID;
}

//-------------------------------------------------------------------------------
// 取得城市ID的最大最小值
//-------------------------------------------------------------------------------
VOID AttRes::GetCityIDRange( DWORD& dwMinID, DWORD& dwMaxID )
{
	dwMinID	= GT_INVALID;
	dwMaxID	= GT_INVALID;

	tagCityProto* pProto = NULL;
	DWORD dwCityID = GT_INVALID;
	TMap<DWORD, tagCityProto*>::TMapIterator iter = m_mapCityProto.Begin();
	while (m_mapCityProto.PeekNext(iter, dwCityID, pProto))
	{
		if (!GT_VALID(dwMinID) || !GT_VALID(dwMaxID))
		{
			dwMaxID = dwMinID = dwCityID;
		}
		else if (dwMaxID < dwCityID)
		{
			dwMaxID = dwCityID;
		}
		else if (dwMinID > dwCityID)
		{
			dwMinID = dwCityID;
		}
	}
}

VOID AttRes::InitSpiritSystem()
{
	//精力系统
	TObjRef<VarContainer> pVar	= "VarContainer";
	m_bSpiSysOn					=	(BOOL)pVar->GetDword(_T("on spirit"));
	m_dwSpiStartLevel			=	pVar->GetDword(_T("level spirit"));
	m_dwSpiRecoverySpeed		=	pVar->GetDword(_T("pointperminute spirit"));
	m_dwSpiReward				=	pVar->GetDword(_T("reward spirit"));
	DWORD levelCount			=	pVar->GetDword(_T("count spirit"));
	for(int i=0; i<levelCount; i++)
	{
		tstringstream tss_range;
		tss_range << _T("upper_bound_") << i << _T(" spirit");
		tstringstream tss_rate;
		tss_rate << _T("rate_") << i << _T(" spirit");

		DWORD range = pVar->GetDword(tss_range.str().c_str());
		if( range>=GetAttMin(ERA_Spirit) && range <= GetAttMax(ERA_Spirit))
			m_mapSpiRate[range] = float(pVar->GetDword(tss_rate.str().c_str())) / 100;
	}
	DWORD rewardCount			=	pVar->GetDword(_T("rewardcount spirit"));
	for(int i=0; i<rewardCount; i++)
	{
		tstringstream tss_reward;
		tss_reward << _T("reward_") << i << _T(" spirit");
		tstringstream reward_num;
		reward_num << _T("reward_num_") << i << _T(" spirit");
		tstringstream reward_quality;
		reward_quality << _T("reward_quality_") <<i << _T(" spirit");

		DWORD itemid = pVar->GetDword(tss_reward.str().c_str());		
		int num = (int)pVar->GetDword(reward_num.str().c_str());		
		EItemQuality quality = (EItemQuality)pVar->GetDword(reward_quality.str().c_str());

		m_mapSpiReward[itemid] = std::pair<int, EItemQuality>(num, quality);
	}
	rewardCount			=	pVar->GetDword(_T("strengthcount spirit"));
	for(int i=0; i<rewardCount; i++)
	{
		tstringstream tss_cost;
		tss_cost<< _T("strength_") << i << _T(" spirit");
		tstringstream tss_item;
		tss_item << _T("strength_item_") << i << _T(" spirit");

		DWORD itemid = pVar->GetDword(tss_item.str().c_str());		
		int cost = (int)pVar->GetDword(tss_cost.str().c_str());		

		m_mapStrengthReward[itemid] = cost;
	}
}

//-------------------------------------------------------------------------------
// 初始化师徒精武属性
//-------------------------------------------------------------------------------
VOID* AttRes::InitOneMasterApprenticeProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pProto, nProtoType, tagMasterApprenticeProto);

	pProto->dwLevelID				=	m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pProto->nLevelUpAddPoint		=	m_pVar->GetInt(_T("level_up_add_point"), szField, 0);
	pProto->nLevelUpNeedPoint		=	m_pVar->GetInt(_T("level_up_need_point"), szField, 0);
	pProto->nLevelUpNeedYuanBao		=	m_pVar->GetInt(_T("level_up_need_yuanbao"), szField, 0);
	pProto->nLevelUpSelfPoint		=	m_pVar->GetInt(_T("add_point_self"), szField, 0);
	pProto->nLevelUpBonus			=	m_pVar->GetInt(_T("apprentice_lvlup_point"), szField, 0);

	return &pProto->dwLevelID;
}

//-------------------------------------------------------------------------------
// 初始化精纺成功率
//-------------------------------------------------------------------------------
VOID* AttRes::InitOneSpinRateProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
{
	MTRANS_POINTER(pProto, nProtoType, tagSpinSuccessRateProto);

	pProto->dwSpinItemNum = m_pVar->GetInt(_T("item_num"), szField, 0);

	pProto->dwSuccessRate[ESpinStep_Basic] = m_pVar->GetDword(_T("basic_rate"), szField, GT_INVALID);

	pProto->dwSuccessRate[ESpinStep_Middle] = m_pVar->GetDword(_T("middle_rate"), szField, GT_INVALID);

	pProto->dwSuccessRate[ESpinStep_Senior] = m_pVar->GetDword(_T("senior_rate"), szField, GT_INVALID);

	return &pProto->dwSpinItemNum;
}

//-------------------------------------------------------------------------------
// 初始化试炼属性表
//-------------------------------------------------------------------------------
 VOID* AttRes::InitOneTrainProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
 {
 	MTRANS_POINTER(pProto, nProtoType, tagTrainAttProto);
 
 	pProto->dwTrainID	= m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	pProto->dwType1 = m_pVar->GetDword(_T("type1"), szField, GT_INVALID);
	pProto->dwType2 = m_pVar->GetDword(_T("type2"), szField, GT_INVALID);
	pProto->dwMaxNum = m_pVar->GetDword(_T("maxnum"), szField, GT_INVALID);
  	pProto->TrainReward.nStrengthReward = m_pVar->GetInt(_T("capability"), szField, 0);
	pProto->TrainReward.dwMoneyReward = m_pVar->GetDword(_T("money"), szField, GT_INVALID);	
	pProto->TrainReward.dwExpReward = m_pVar->GetDword(_T("exp"), szField, GT_INVALID);
	pProto->TrainReward.pItemReward[0].nItemTypeID	= m_pVar->GetDword(_T("Item1ID"),		szField, GT_INVALID);
	pProto->TrainReward.pItemReward[0].nItemCount	= m_pVar->GetInt(_T("Item1Num"),		szField, 0);
	pProto->TrainReward.pItemReward[0].nItemQuality = m_pVar->GetInt(_T("Item1Quality"),	szField, 0);
	pProto->TrainReward.pItemReward[1].nItemTypeID	= m_pVar->GetDword(_T("Item2ID"),		szField, GT_INVALID);
	pProto->TrainReward.pItemReward[1].nItemCount	= m_pVar->GetInt(_T("Item2Num"),		szField, 0);
	pProto->TrainReward.pItemReward[1].nItemQuality = m_pVar->GetInt(_T("Item2Quality"),	szField, 0);
 	//todo 添加其他字段读取

	return &pProto->dwTrainID;
 }

 //-------------------------------------------------------------------------------
 // 天生技能原型
 //-------------------------------------------------------------------------------
 VOID* AttRes::InitOneInitialSkillProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy )
 {
	 MTRANS_POINTER(pProto, nProtoType, tagInitialSkill);

	 pProto->dwID = m_pVar->GetDword(_T("id"), szField, GT_INVALID);
	 pProto->dwSkillID = m_pVar->GetDword(_T("skill_id"), szField, GT_INVALID);

	 return &pProto->dwID;
 }

//-------------------------------------------------------------------------------
// 得到对应的精纺成功率 升序的
//-------------------------------------------------------------------------------
const tagSpinSuccessRateProto* AttRes::GetSpinRateProto(DWORD dwItemNum)
{ 
	if(m_mapSpinRateProto.Size() == 0)
		return NULL;

	std::map <DWORD, tagSpinSuccessRateProto*>::iterator it = m_mapSpinRateProto.LowerBound(dwItemNum);

	if(it != m_mapSpinRateProto.End() && it->first <= dwItemNum)
	{
		return it->second;
	}
	else if(it == m_mapSpinRateProto.End())
	{ 
		//返回最后一个 key最大的成功率记录
		return m_mapSpinRateProto.RBegin()->second;
	}

	return NULL;
}


//-------------------------------------------------------------------------------
// 初始化精纺原型
//-------------------------------------------------------------------------------
BOOL AttRes::InitSpinProto(OUT TMap<tagSpinKey ,tagSpinProto*>	&mapSpinProto, LPCTSTR szFileName)
{
	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	// 读入文件
	if(!m_pVar->Load(NULL, szFileName,"id", &ListField))
	{
		MAttResCaution(szFileName, _T("load"), GT_INVALID);
		return FALSE;
	}

	// 按顺序读取
	for(it = ListField.begin(); it != ListField.end(); ++it)
	{
		tagSpinProto *pProto = new tagSpinProto;

		pProto->dwSpinFashionTypeID[ESpinStep_Null] = m_pVar->GetDword(_T("raw_fashion"), it->c_str(), GT_INVALID);

		pProto->dwSpinFashionTypeID[ESpinStep_Basic] = m_pVar->GetDword(_T("basic_spin"), it->c_str(), GT_INVALID);

		pProto->dwSpinFashionTypeID[ESpinStep_Middle] = m_pVar->GetDword(_T("middle_spin"), it->c_str(), GT_INVALID);

		pProto->dwSpinFashionTypeID[ESpinStep_Senior] = m_pVar->GetDword(_T("senior_spin"), it->c_str(), GT_INVALID);

		pProto->eAttType = (ESuitAttType)m_pVar->GetInt(_T("suit_att_type"), it->c_str(), ESSAT_Null);

		ASSERT(pProto->eAttType != ESSAT_Null);

		for( INT32 n32Loop = ESpinStep_Null; n32Loop<=ESpinStep_Senior; ++n32Loop )
		{
			tagSpinKey key(pProto->dwSpinFashionTypeID[n32Loop],pProto->eAttType);

			if(mapSpinProto.IsExist(key))
				continue;

			mapSpinProto.Add(key,pProto);
		}
	}

	// 清空容器
	m_pVar->Clear();

	return TRUE;
}
// Jason 2010-4-13 v2.0.0
VOID* AttRes::InitOneFabaoLevelProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pFabaoLevelProto, pProtoType, tagFabaoLevelProto);
	pFabaoLevelProto->nID			= (INT)m_pVar->GetDword(_T("id"),szField);
	pFabaoLevelProto->nMinUseLevel	= (INT)m_pVar->GetDword(_T("ues_level_limit"),szField);
	pFabaoLevelProto->nLevelUpWhated	= (INT)m_pVar->GetDword(_T("exp_next_level"),szField);
	pFabaoLevelProto->nMinValue			= (INT)m_pVar->GetDword(_T("exp_total"),szField);
	pFabaoLevelProto->fCoefficientOfGrowing	= m_pVar->GetFloat(_T("coefficient_of_growing"),szField);
	if( m_nFabaoMaxStage < pFabaoLevelProto->nID )
		m_nFabaoMaxStage = pFabaoLevelProto->nID;
	return &pFabaoLevelProto->nID;
}
VOID* AttRes::InitOneFabaoWuxingProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pFabaoWuxingProto, pProtoType, tagFabaoWuxingProto);
	pFabaoWuxingProto->nID			= (INT)m_pVar->GetDword(_T("id"),szField);
	pFabaoWuxingProto->dwTriggerID	= (INT)m_pVar->GetDword(_T("triggerid"),szField);
	pFabaoWuxingProto->dwBuffID	= (INT)m_pVar->GetDword(_T("buffid"),szField);
	pFabaoWuxingProto->nDifficulty			= (INT)m_pVar->GetDword(_T("difficulty"),szField);
	pFabaoWuxingProto->n16NativeIntelligenceLim	= (INT)m_pVar->GetDword(_T("qualification_limit"),szField,GT_INVALID);
	pFabaoWuxingProto->eFabaoTypeLim	= (ENUM_FabaoType)m_pVar->GetDword(_T("type_limit"),szField,EFB_NULL);
	pFabaoWuxingProto->nXSRewardTimes	= (EEquipPos)m_pVar->GetDword(_T("pos_limit"),szField,-1);
	pFabaoWuxingProto->nRare	= (INT)m_pVar->GetDword(_T("rare"),szField);
	return &pFabaoWuxingProto->nID;
}
// 该函数要在装备原型初始化后再调用，否则肯定出错
VOID* AttRes::InitOneFabaoProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pFabaoProto, pProtoType, tagFabaoProto);
	DWORD dwTypeID = m_pVar->GetDword(_T("id"),szField);
	tagEquipProto* pEquipProto = GetEquipProto(dwTypeID);
	if( P_VALID(pEquipProto) )
	{
		memcpy(pFabaoProto,pEquipProto,sizeof(tagEquipProto));
		pFabaoProto->eFarbaoType	= (ENUM_FabaoType)m_pVar->GetDword(_T("type"),szField);
		pFabaoProto->eAttType[0]	= (ERoleAttribute)m_pVar->GetDword(_T("att_type1"),szField,ERA_Null);
		pFabaoProto->nAttGrowing[0]			= (INT)m_pVar->GetDword(_T("att_growing1"),szField,0);
		pFabaoProto->eAttType[1]	= (ERoleAttribute)m_pVar->GetDword(_T("att_type2"),szField,ERA_Null);
		pFabaoProto->nAttGrowing[1]			= (INT)m_pVar->GetDword(_T("att_growing2"),szField,0);
		pFabaoProto->eAttType[2]	= (ERoleAttribute)m_pVar->GetDword(_T("att_type3"),szField,ERA_Null);
		pFabaoProto->nAttGrowing[2]			= (INT)m_pVar->GetDword(_T("att_growing3"),szField,0);

		pFabaoProto->eSlotAttType[0]	= (ERoleAttribute)m_pVar->GetDword(_T("slot_att_type1"),szField);
		pFabaoProto->nSlotAttValue[0]	= (INT)m_pVar->GetDword(_T("slot_attval1"),szField);
		pFabaoProto->eSlotAttType[1]	= (ERoleAttribute)m_pVar->GetDword(_T("slot_att_type2"),szField);
		pFabaoProto->nSlotAttValue[1]	= (INT)m_pVar->GetDword(_T("slot_attval2"),szField);
		pFabaoProto->eSlotAttType[2]	= (ERoleAttribute)m_pVar->GetDword(_T("slot_att_type3"),szField);
		pFabaoProto->nSlotAttValue[2]	= (INT)m_pVar->GetDword(_T("slot_attval3"),szField);
		pFabaoProto->eSlotAttType[3]	= (ERoleAttribute)m_pVar->GetDword(_T("slot_att_type4"),szField);
		pFabaoProto->nSlotAttValue[3]	= (INT)m_pVar->GetDword(_T("slot_attval4"),szField);
		pFabaoProto->eSlotAttType[4]	= (ERoleAttribute)m_pVar->GetDword(_T("slot_att_type5"),szField);
		pFabaoProto->nSlotAttValue[4]	= (INT)m_pVar->GetDword(_T("slot_attval5"),szField);

		pFabaoProto->bCanReset	= (BOOL)m_pVar->GetDword(_T("can_reset"),szField);
		pFabaoProto->bCanUse	= (BOOL)m_pVar->GetDword(_T("can_use"),szField);
		pFabaoProto->bHasDefaultValue	= (BOOL)m_pVar->GetDword(_T("is_have_defaultval"),szField);

		pFabaoProto->n16DefaultFabaoQulity	= (INT16)m_pVar->GetDword(_T("defaultqua"),szField,0);
		pFabaoProto->n16DefaultWuxing	= EWX_Null;//(INT16)m_pVar->GetDword(_T("default_wuxing"),szField,EWX_Null);
		//pFabaoProto->nWuxingMatchReward[0]	= (INT)m_pVar->GetDword(_T("wuxing_id1"),szField,0);
		//pFabaoProto->nWuxingMatchReward[1]	= (INT)m_pVar->GetDword(_T("wuxing_id2"),szField,0);
		//pFabaoProto->nWuxingMatchReward[2]	= (INT)m_pVar->GetDword(_T("wuxing_id3"),szField,0);

		pFabaoProto->eDefaultSlotType[0]	= (EWuXing)m_pVar->GetDword(_T("slot_wuxing1"),szField,EWX_Null);
		pFabaoProto->eDefaultSlotType[1]	= (EWuXing)m_pVar->GetDword(_T("slot_wuxing2"),szField,EWX_Null);
		pFabaoProto->eDefaultSlotType[2]	= (EWuXing)m_pVar->GetDword(_T("slot_wuxing3"),szField,EWX_Null);
		pFabaoProto->eDefaultSlotType[3]	= (EWuXing)m_pVar->GetDword(_T("slot_wuxing4"),szField,EWX_Null);
		pFabaoProto->eDefaultSlotType[4]	= (EWuXing)m_pVar->GetDword(_T("slot_wuxing5"),szField,EWX_Null);

		pFabaoProto->nIdfQltyPct[EIQ_White]	= (INT)m_pVar->GetDword(_T("whi_pct"),szField,0);
		pFabaoProto->nIdfQltyPct[EIQ_Yellow]	= (INT)m_pVar->GetDword(_T("yel_pct"),szField,0);
		pFabaoProto->nIdfQltyPct[EIQ_Green]	= (INT)m_pVar->GetDword(_T("gre_pct"),szField,0);
		pFabaoProto->nIdfQltyPct[EIQ_Blue]	= (INT)m_pVar->GetDword(_T("blu_pct"),szField,0);
		pFabaoProto->nIdfQltyPct[EIQ_Orange]	= (INT)m_pVar->GetDword(_T("org_pct"),szField,0);
		pFabaoProto->nIdfQltyPct[EIQ_Purple]	= (INT)m_pVar->GetDword(_T("pur_pct"),szField,0);
		pFabaoProto->nIdfQltyPct[EIQ_Pink]	= (INT)m_pVar->GetDword(_T("pin_pct"),szField,0);

		// 妖精元素伤害以及抗性
		pFabaoProto->eEleInjury = (EElementInjury)m_pVar->GetDword(_T("default_wuxing"),szField,EEI_Null);
		pFabaoProto->nEleInjury = m_pVar->GetDword(_T("default_value"),szField);
		pFabaoProto->nElePierce = m_pVar->GetDword(_T("default_value2"),szField);
		pFabaoProto->nEleResistance[EER_Fire]	= m_pVar->GetDword(_T("fire_def"),szField,0);
		pFabaoProto->nEleResistance[EER_Water]	= m_pVar->GetDword(_T("water_def"),szField,0);
		pFabaoProto->nEleResistance[EER_Earth]	= m_pVar->GetDword(_T("soil_def"),szField,0);
		pFabaoProto->nEleResistance[EER_Wind]	= m_pVar->GetDword(_T("gas_def"),szField,0);
		pFabaoProto->nEleResistance[EER_Bright]	= m_pVar->GetDword(_T("light_def"),szField,0);
		pFabaoProto->nEleResistance[EER_Dark]	= m_pVar->GetDword(_T("dark_def"),szField,0);

		pFabaoProto->dwDef60StuntID = m_pVar->GetDword(_T("stunt_id"),szField,0);

		pFabaoProto->bCanBirth		= m_pVar->GetDword(_T("can_birth"),szField,GT_INVALID);
		pFabaoProto->n16CanFusion	= m_pVar->GetInt(_T("Fairy_smelting"),szField,0);

		return &pFabaoProto->dwTypeID;
	}
	return NULL;
}

// 加载圣灵附加属性[wcy 2012-03-30]
VOID* AttRes::InitOneHolyProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pHolyProto, pProtoType, tagHolyProto);
	DWORD dwTypeID = m_pVar->GetDword(_T("id"),szField);
	tagEquipProto* pEquipProto = GetEquipProto(dwTypeID);
	if( P_VALID(pEquipProto) )
	{
		memcpy(pHolyProto,pEquipProto,sizeof(tagEquipProto));
		pHolyProto->nDevourNum = (INT)m_pVar->GetDword(_T("devour_Num"),szField,0);
		pHolyProto->n16EquipmentNumber = (INT16)m_pVar->GetDword(_T("equipment_number"),szField,0);
		pHolyProto->nHolyMaxvalue = (INT)m_pVar->GetDword(_T("holy_maxvalue"),szField,0);
		pHolyProto->n16CallConsumption = (INT16)m_pVar->GetDword(_T("call_consumption"),szField,0);
		pHolyProto->nHolyDmg = (INT)m_pVar->GetDword(_T("holy_dmg"),szField,0);
		pHolyProto->nHolyDef = (INT)m_pVar->GetDword(_T("holy_def"),szField,0);
		pHolyProto->nHolyValue = (INT)m_pVar->GetDword(_T("holy_value"),szField,0);
		pHolyProto->nCrit = (INT)m_pVar->GetDword(_T("crit"),szField,0);
		pHolyProto->n16HolyCritRate = (INT16)m_pVar->GetDword(_T("Holy_Crit_Rate"),szField,0);
		pHolyProto->nExDamage = (INT)m_pVar->GetDword(_T("ExDamage"),szField,0);
		pHolyProto->nAttackTec = (INT)m_pVar->GetDword(_T("AttackTec"),szField,0);
		pHolyProto->nNeglectToughness = (INT)m_pVar->GetDword(_T("neglect_toughness"),szField,0);
		
		return &pHolyProto->dwTypeID;
	}
	return NULL;
}

// 加载圣灵升级表[wcy 2012-04-09]
VOID*  AttRes::InitOneHolyLevelUp(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pHolyLevelUp, pProtoType, tagHolyLevelUpProto);
	pHolyLevelUp->dwID = m_pVar->GetDword(_T("id"),szField);
	pHolyLevelUp->dwDmgMin		= (INT)m_pVar->GetDword(_T("dm_min"), szField, 0);
	pHolyLevelUp->dwDmgMax		= (INT)m_pVar->GetDword(_T("dm_max"), szField, 0);
	pHolyLevelUp->dwDefMin		= (INT)m_pVar->GetDword(_T("df_min"), szField, 0);
	pHolyLevelUp->dwDefMax		= (INT)m_pVar->GetDword(_T("df_max"), szField, 0);
	pHolyLevelUp->dwLingNengMin		= (INT)m_pVar->GetDword(_T("holy_min"), szField, 0);
	pHolyLevelUp->dwLingNengMax		= (INT)m_pVar->GetDword(_T("holy_max"), szField, 0);
	pHolyLevelUp->dwCritMin		= (INT)m_pVar->GetDword(_T("Crit_min"), szField, 0);
	pHolyLevelUp->dwCritMax		= (INT)m_pVar->GetDword(_T("Crit_max"), szField, 0);
	pHolyLevelUp->dwCritValueMin		= (INT)m_pVar->GetDword(_T("Crit_Rate_min"), szField, 0);
	pHolyLevelUp->dwCritValueMax		= (INT)m_pVar->GetDword(_T("Crit_Rate_max"), szField, 0);
	pHolyLevelUp->dwDmgAddMin		= (INT)m_pVar->GetDword(_T("ExDamage_min"), szField, 0);
	pHolyLevelUp->dwDmgAddMax		= (INT)m_pVar->GetDword(_T("ExDamage_max"), szField, 0);
	pHolyLevelUp->dwAttackTecMin		= (INT)m_pVar->GetDword(_T("AttackTec_min"), szField, 0);
	pHolyLevelUp->dwAttackTecMax		= (INT)m_pVar->GetDword(_T("AttackTec_max"), szField, 0);
	pHolyLevelUp->dwNeglectMin		= (INT)m_pVar->GetDword(_T("neglect_min"), szField, 0);
	pHolyLevelUp->dwNeglectMax		= (INT)m_pVar->GetDword(_T("neglect_max"), szField, 0);

	return &pHolyLevelUp->dwID;
}

// 加载圣纹附加属性[wcy 2012-04-17]
VOID* AttRes::InitOneHolyEquipProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy)
{
	// 初始化装备静态属性特有部分
	MTRANS_POINTER(pHolyEquipProto, pProtoType, tagHolyEquipProto);
	DWORD dwTypeID = m_pVar->GetDword(_T("id"),szField);
	tagEquipProto* pEquipProto = GetEquipProto(dwTypeID);
	if( P_VALID(pEquipProto) )
	{
		memcpy(pHolyEquipProto,pEquipProto,sizeof(tagEquipProto));
		pHolyEquipProto->nCostHolyValue = (INT)m_pVar->GetDword(_T("holy_value"), szField, 0);
		pHolyEquipProto->nCostMinHolyValue = (INT)m_pVar->GetDword(_T("holy_minvalue"), szField, 0);
		pHolyEquipProto->n16MaxLapNum = (INT16)m_pVar->GetDword(_T("max_lap_num"),szField,0);
		pHolyEquipProto->n16WearLapNum = (INT16)m_pVar->GetDword(_T("max_wear_num"),szField,0);
		pHolyEquipProto->nHolyLevel = (INT)m_pVar->GetDword(_T("holy_level"), szField, 0);

		TCHAR szTmp[8];
		// 充能前，装备到圣灵上 影响的属性(和品级无关)
		for(INT32 i=1; i<=MAX_EQUIPATT_BASE_EFFECT; ++i)
		{
			_stprintf(szTmp, _T("%d"), i);
			pHolyEquipProto->BaseEffect[i-1].eHolyAtt	= (EHolyAttType)m_pVar->GetInt(_T("holy_att_"), szTmp, szField, ESAT_NULL);
			if(ESAT_NULL == pHolyEquipProto->BaseEffect[i-1].eHolyAtt)
			{
				break;
			}

			pHolyEquipProto->BaseEffect[i-1].nValue	=	m_pVar->GetInt(_T("holy_val_"), szTmp, szField, 0);
		}

		return &pHolyEquipProto->dwTypeID;
	}
	return NULL;
}

// 加载圣纹强化属性[wcy 2012-04-26]
VOID* AttRes::InitOneHolyEquipEnhance(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pHolyEquipEnhance, pProtoType, tagHolyEquipEnhance);
	pHolyEquipEnhance->dwID = m_pVar->GetDword(_T("id"),szField);
	pHolyEquipEnhance->dwPowMin[ESAT_Demage]	= (INT)m_pVar->GetDword(_T("DmPowMin"), szField, 0);
	pHolyEquipEnhance->dwPowMax[ESAT_Demage]		= (INT)m_pVar->GetDword(_T("DmPowMax"), szField, 0);
	pHolyEquipEnhance->dwPowMin[ESAT_HolyDef]		= (INT)m_pVar->GetDword(_T("DfPowMin"), szField, 0);
	pHolyEquipEnhance->dwPowMax[ESAT_HolyDef]		= (INT)m_pVar->GetDword(_T("DfPowMax"), szField, 0);
	pHolyEquipEnhance->dwPowMin[ESAT_Crit]		= (INT)m_pVar->GetDword(_T("CritPowMin"), szField, 0);
	pHolyEquipEnhance->dwPowMax[ESAT_Crit]	= (INT)m_pVar->GetDword(_T("CritPowMax"), szField, 0);
	pHolyEquipEnhance->dwPowMin[ESAT_CritRate]		= (INT)m_pVar->GetDword(_T("CritRatePowMin"), szField, 0);
	pHolyEquipEnhance->dwPowMax[ESAT_CritRate]		= (INT)m_pVar->GetDword(_T("CritRatePowMax"), szField, 0);
	pHolyEquipEnhance->dwPowMin[ESAT_ExDmg]		= (INT)m_pVar->GetDword(_T("ExDmPowMin"), szField, 0);
	pHolyEquipEnhance->dwPowMax[ESAT_ExDmg]		= (INT)m_pVar->GetDword(_T("ExDmPowMax"), szField, 0);
	pHolyEquipEnhance->dwPowMin[ESAT_AttackTec]		= (INT)m_pVar->GetDword(_T("AttTecPowMin"), szField, 0);
	pHolyEquipEnhance->dwPowMax[ESAT_AttackTec]	= (INT)m_pVar->GetDword(_T("AttTecPowMax"), szField, 0);
	pHolyEquipEnhance->dwPowMin[ESAT_NeglectToughness]		= (INT)m_pVar->GetDword(_T("NegPowMin"), szField, 0);
	pHolyEquipEnhance->dwPowMax[ESAT_NeglectToughness]		= (INT)m_pVar->GetDword(_T("NegPowMax"), szField, 0);

	pHolyEquipEnhance->dwLingNengPow		= (INT)m_pVar->GetDword(_T("HolyPointPow"), szField, 0);	

	return &pHolyEquipEnhance->dwID;
}

const tagFabaoProto * AttRes::GetFabaoProto(DWORD dwTypeID)
{
	tagFabaoProto * re = NULL;
	re = m_mapFabaoProto.Peek(dwTypeID);
	return re;
}

const tagFabaoLevelProto * AttRes::GetFabaoLevelProto(INT ID)
{
	tagFabaoLevelProto * re = NULL;
	re = m_mapFabaoLevelProto.Peek(ID);
	return re;
}
const tagFabaoWuxingProto * AttRes::GetFabaoWuxingProto(INT ID)
{
	tagFabaoWuxingProto * re = NULL;
	re = m_mapFabaoWuxingProto.Peek(ID);
	return re;
}

const tagHolyProto * AttRes::GetHolyProto(DWORD dwTypeID)
{
	tagHolyProto * re = NULL;
	re = m_mapHolyProto.Peek(dwTypeID);
	return re;
}

const tagHolyEquipProto * AttRes::GetHolyEquipProto(DWORD dwTypeID)
{
	tagHolyEquipProto * re = NULL;
	re = m_mapHolyEquipProto.Peek(dwTypeID);
	return re;
}

VOID AttRes::InitContainerLimit()
{
	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	m_pVar->Clear();

	// 读入文件
	if(!m_pVar->Load(NULL, FileName_ContainerLimit, "id", &ListField))
	{
		return;
	}

	for(it = ListField.begin(); it != ListField.end(); ++it)
	{
		m_dwContainerLimitedItems.push_back(m_pVar->GetDword(_T("id"), it->c_str(), GT_INVALID));
	}
	
	// 清空容器
	m_pVar->Clear();

	m_dwContainerLimitedItems.sort();
}
VOID * AttRes::InitOneQuestUnimpededToken(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pUnimpededToken, nProtoType, tagQuestUnimpededToken);
	if( P_VALID(pUnimpededToken) )
	{
		pUnimpededToken->dwQuestID				= m_pVar->GetDword(_T("id"),szField);
		pUnimpededToken->dwUnimpededTokenTypeID = m_pVar->GetDword(_T("item_id"),szField);
		pUnimpededToken->nTokenNum				= m_pVar->GetDword(_T("item_number"),szField);

		return &pUnimpededToken->dwQuestID;
	}
	return NULL;
}
const tagQuestUnimpededToken * AttRes::GetOneUnimpededToken(DWORD dwQuestID) 
{
	return m_mapQuestUnimpededToken.Peek(dwQuestID);
}

VOID * AttRes::InitOneLearnSoaringSkillProto(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pLearnSoaringSkillProto, nProtoType, tagLearnSoaringSkillProto);
	if( P_VALID(pLearnSoaringSkillProto) )
	{
		pLearnSoaringSkillProto->dwID				= m_pVar->GetDword(_T("id"),szField);
		pLearnSoaringSkillProto->nLearnSilverCosted = m_pVar->GetDword(_T("learn_silver"),szField);
		pLearnSoaringSkillProto->nExpCosted				= m_pVar->GetDword(_T("learn_exp"),szField);
		pLearnSoaringSkillProto->dwItemNeeded				= m_pVar->GetDword(_T("learn_item_typeid"),szField);
		pLearnSoaringSkillProto->nItemCount				= m_pVar->GetDword(_T("learn_item_num"),szField);

		return &pLearnSoaringSkillProto->dwID;
	}
	return NULL;
}

const tagLearnSoaringSkillProto * AttRes::GetOneLearnSoaringSkillProto(DWORD dwID)
{
	return m_mapLearnSoaringSkill.Peek(dwID);
}

const tagCastingProto * AttRes::GetOneCastingProto(DWORD dwID)
{
	return m_mapCastingProto.Peek(dwID);
}

VOID * AttRes::InitOneCastingProto(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pCastingProto, nProtoType, tagCastingProto);
	if( P_VALID(pCastingProto) )
	{
		pCastingProto->dwID				= m_pVar->GetDword(_T("id"),szField);
		pCastingProto->byType			= m_pVar->GetDword(_T("type"),szField);
		pCastingProto->u16Level				= m_pVar->GetDword(_T("level"),szField);
		pCastingProto->u16PotvalConsume		= m_pVar->GetDword(_T("potval_consume"),szField);
		pCastingProto->nProbability			= m_pVar->GetDword(_T("probability"),szField);

		TCHAR buf[50] = {0};
		for( int i = 0; i < 5; ++i)
		{
			_stprintf(buf,_T("att%d"),i+1);
			pCastingProto->eCastingRoleAtt[i] = (ERoleAttribute)m_pVar->GetDword(buf,szField,ERA_Null);
			_stprintf(buf,_T("val%d"),i+1);
			pCastingProto->nRoleAttValue[i] = m_pVar->GetDword(buf,szField,0);
			_stprintf(buf,_T("material%d"),i+1);
			pCastingProto->dwStuffTypeID[i] = m_pVar->GetDword(buf,szField,GT_INVALID);
			_stprintf(buf,_T("num%d"),i+1);
			pCastingProto->nStuffNum[i] = m_pVar->GetDword(buf,szField,0);
		}

		return &pCastingProto->dwID;
	}
	return NULL;
}
VOID * AttRes::InitOneMsgDeliverProto(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pMsgDeliver, nProtoType, tagMsgDeliver);
	if( P_VALID(pMsgDeliver) )
	{
		pMsgDeliver->dwID		= m_pVar->GetDword(_T("id"),szField);
		pMsgDeliver->dwIDMsgID		= m_pVar->GetDword(_T("msgid"),szField);
		LPCTSTR pStr = m_pVar->GetString(_T("mapid"),szField);
		if( P_VALID(pStr) )
		{
			pMsgDeliver->dwMapID = _tcstoul(pStr,NULL,10);
		}
		else
			pMsgDeliver->dwMapID = GT_INVALID;
		//pMsgDeliver->dwMapID		= m_pVar->GetDword(_T("mapid"),szField);
		pMsgDeliver->X		= m_pVar->GetDword(_T("pos_x"),szField,GT_INVALID);
		pMsgDeliver->Y		= m_pVar->GetDword(_T("pos_y"),szField,GT_INVALID);
		pMsgDeliver->Z		= m_pVar->GetDword(_T("pos_z"),szField,GT_INVALID);
		return &pMsgDeliver->dwID;
	}
	return NULL;
}
const tagMsgDeliver * AttRes::GetOneMsgDeliverProto(DWORD dwID)
{
	return m_mapMsgDeliverProto.Peek(dwID);
}
VOID * AttRes::InitOneSkillBoxProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pSkillBoxProto, nProtoType, tagSkillBoxProto);
	if( P_VALID(pSkillBoxProto) )
	{
		pSkillBoxProto->dwID	= m_pVar->GetDword(_T("id"),szField);
		pSkillBoxProto->bCanBeLearned = m_pVar->GetDword(_T("learn_limit"), szField, 0)!=0?TRUE:FALSE;
		const TCHAR * pStrFormator = _T("skill_id%d");
		TCHAR buf[20] = {0};
		for(INT i = 0; i < tagSkillBoxProto::E_SBP_Size; ++i)
		{
			_stprintf(buf,pStrFormator,i+1);
			pSkillBoxProto->dwSkillIDs[i] = m_pVar->GetDword(buf,szField,GT_INVALID);
		}
		return &pSkillBoxProto->dwID;
	}
	return NULL;
}
const tagSkillBoxProto * AttRes::GetOneSkillProto(DWORD dwID)
{
	return m_mapSkillBoxProto.Peek(dwID);
}

VOID* AttRes::InitOneBoardQuest(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pQuestProto, pProtoType, tagBoardQuestProto);

	pQuestProto->u16ID					= (UINT16)m_pVar->GetInt(_T("id"),szField);
	pQuestProto->byType					= (BYTE)m_pVar->GetInt(_T("QuestType"),szField);
	pQuestProto->eQuality				= (EBoardQuestQuality)m_pVar->GetInt(_T("grade"),szField);
	pQuestProto->byLevel				= (BYTE)m_pVar->GetInt(_T("ApproachLevel"), szField);
	pQuestProto->eTarget				= (EQuestTargetType)m_pVar->GetInt(_T("CompleteType"),szField);

	pQuestProto->dwInvestItem[0]		= (DWORD)m_pVar->GetDword(_T("InvestigateObjectID1"),szField, 0);
	pQuestProto->dwInvestItem[1]		= (DWORD)m_pVar->GetDword(_T("InvestigateObjectID2"),szField, 0);
	pQuestProto->dwInvestItem[2]		= (DWORD)m_pVar->GetDword(_T("InvestigateObjectID3"),szField, 0);
	pQuestProto->dwInvestItem[3]		= (DWORD)m_pVar->GetDword(_T("InvestigateObjectID4"),szField, 0);

	pQuestProto->TargetMonster[0].dwTargetMonsterID_A		= (DWORD)m_pVar->GetDword(_T("CompleteQuestRequiredCreatureID1_id1"),szField, 0);
	pQuestProto->TargetMonster[0].dwTargetMonsterID_B		= (DWORD)m_pVar->GetDword(_T("CompleteQuestRequiredCreatureID1_id2"),szField, 0);
	pQuestProto->TargetMonster[0].byTargetMonsterNum		= (INT)m_pVar->GetInt(_T("CompleteQuestRequiredCreatureID1_value"),szField, 0);
	pQuestProto->TargetMonster[1].dwTargetMonsterID_A		= (DWORD)m_pVar->GetDword(_T("CompleteQuestRequiredCreatureID2_id1"),szField, 0);
	pQuestProto->TargetMonster[1].dwTargetMonsterID_B		= (DWORD)m_pVar->GetDword(_T("CompleteQuestRequiredCreatureID2_id2"),szField, 0);
	pQuestProto->TargetMonster[1].byTargetMonsterNum		= (INT)m_pVar->GetInt(_T("CompleteQuestRequiredCreatureID2_value"),szField, 0);
	pQuestProto->TargetMonster[2].dwTargetMonsterID_A		= (DWORD)m_pVar->GetDword(_T("CompleteQuestRequiredCreatureID3_id1"),szField, 0);
	pQuestProto->TargetMonster[2].dwTargetMonsterID_B		= (DWORD)m_pVar->GetDword(_T("CompleteQuestRequiredCreatureID3_id2"),szField, 0);
	pQuestProto->TargetMonster[2].byTargetMonsterNum		= (INT)m_pVar->GetInt(_T("CompleteQuestRequiredCreatureID3_value"),szField, 0);
	pQuestProto->TargetMonster[3].dwTargetMonsterID_A		= (DWORD)m_pVar->GetDword(_T("CompleteQuestRequiredCreatureID4_id1"),szField, 0);
	pQuestProto->TargetMonster[3].dwTargetMonsterID_B		= (DWORD)m_pVar->GetDword(_T("CompleteQuestRequiredCreatureID4_id2"),szField, 0);
	pQuestProto->TargetMonster[3].byTargetMonsterNum		= (INT)m_pVar->GetInt(_T("CompleteQuestRequiredCreatureID4_value"),szField, 0);

	
	pQuestProto->dwTalkToNPC[0]			= (DWORD)m_pVar->GetDword(_T("CompleteQuestRequiredNPC1"),szField, 0);
	pQuestProto->dwTalkToNPC[1]			= (DWORD)m_pVar->GetDword(_T("CompleteQuestRequiredNPC2"),szField, 0);

	pQuestProto->GatherItem[0].dwID		= (DWORD)m_pVar->GetInt(_T("CompleteQuestRequiredItemID1"), szField, 0);
	pQuestProto->GatherItem[0].byNum	= (INT)m_pVar->GetDword(_T("CompleteQuestRequiredItemID1_value"), szField, 0);	
	pQuestProto->GatherItem[1].dwID		= (DWORD)m_pVar->GetInt(_T("CompleteQuestRequiredItemID2"), szField, 0);
	pQuestProto->GatherItem[1].byNum	= (INT)m_pVar->GetDword(_T("CompleteQuestRequiredItemID2_value"), szField, 0);
	pQuestProto->GatherItem[2].dwID		= (DWORD)m_pVar->GetInt(_T("CompleteQuestRequiredItemID3"), szField, 0);
	pQuestProto->GatherItem[2].byNum	= (INT)m_pVar->GetDword(_T("CompleteQuestRequiredItemID3_value"), szField, 0);
	//pQuestProto->GatherItem[3].dwID		= (DWORD)m_pVar->GetInt(_T("CompleteQuestRequiredItemID4"), szField, 0);
	//pQuestProto->GatherItem[3].byNum	= (BYTE)m_pVar->GetDword(_T("CompleteQuestRequiredItemID4_value"), szField, 0);


	pQuestProto->RewardItem[0].dwID		= (DWORD)m_pVar->GetDword(_T("RewChoicesItemID1"), szField, 0);
	pQuestProto->RewardItem[0].byNum		= (BYTE)m_pVar->GetInt(_T("RewChoicesItemID1_value"), szField, 0);
	pQuestProto->RewardItem[0].byQuality	= (BYTE)m_pVar->GetInt(_T("RewChoicesItemID1_quality"), szField, 0);
	pQuestProto->RewardItem[1].dwID		= (DWORD)m_pVar->GetDword(_T("RewChoicesItemID2"), szField, 0);
	pQuestProto->RewardItem[1].byNum		= (BYTE)m_pVar->GetInt(_T("RewChoicesItemID2_value"), szField, 0);
	pQuestProto->RewardItem[1].byQuality	= (BYTE)m_pVar->GetInt(_T("RewChoicesItemID2_quality"), szField, 0);
	pQuestProto->RewardItem[2].dwID		= (DWORD)m_pVar->GetDword(_T("RewChoicesItemID3"), szField, 0);
	pQuestProto->RewardItem[2].byNum		= (BYTE)m_pVar->GetInt(_T("RewChoicesItemID3_value"), szField, 0);
	pQuestProto->RewardItem[2].byQuality	= (BYTE)m_pVar->GetInt(_T("RewChoicesItemID3_quality"), szField, 0);
	pQuestProto->RewardItem[3].dwID		= (DWORD)m_pVar->GetDword(_T("RewChoicesItemID4"), szField, 0);
	pQuestProto->RewardItem[3].byNum		= (BYTE)m_pVar->GetInt(_T("RewChoicesItemID4_value"), szField, 0);
	pQuestProto->RewardItem[3].byQuality	= (BYTE)m_pVar->GetInt(_T("RewChoicesItemID4_quality"), szField, 0);
	pQuestProto->RewardItem[4].dwID		= (DWORD)m_pVar->GetDword(_T("RewChoicesItemID5"), szField, 0);
	pQuestProto->RewardItem[4].byNum		= (BYTE)m_pVar->GetInt(_T("RewChoicesItemID5_value"), szField, 0);
	pQuestProto->RewardItem[4].byQuality	= (BYTE)m_pVar->GetInt(_T("RewChoicesItemID5_quality"), szField, 0);
	
	pQuestProto->nRewardExp = m_pVar->GetInt(_T("RewXP"), szField, 0);
	pQuestProto->nRewardMoney = m_pVar->GetInt(_T("RewMoney"), szField, 0);

	return &pQuestProto->u16ID;
}

VOID AttRes::InitOneBoardQuestRandRule(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex)
{
	tagBoardQuestRandomRate *p = static_cast<tagBoardQuestRandomRate *>( pArray ) + nIndex;

	p->n16WhiteRate		= (INT16)m_pVar->GetInt(_T("white"), szField);
	p->n16YellowRate	= (INT16)m_pVar->GetInt(_T("yellow"),szField);
	p->n16GreenRate		= (INT16)m_pVar->GetInt(_T("green"), szField);
	p->n16BlueRate		= (INT16)m_pVar->GetInt(_T("blue"),  szField);
	p->n16OrangeRate	= (INT16)m_pVar->GetInt(_T("orange"),szField);
	p->n16PurpleRate	= (INT16)m_pVar->GetInt(_T("purple"),szField);
}

VOID * AttRes::InitOneFConsolidateProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pFConsolidateProto, nProtoType, tagFConsolidateProto);
	if( P_VALID(pFConsolidateProto) )
	{
		pFConsolidateProto->dwID = (DWORD)m_pVar->GetDword(_T("id"), szField);
		pFConsolidateProto->eItemTypeEx = (EItemTypeEx)m_pVar->GetInt(_T("type_ex"), szField);
		pFConsolidateProto->n16Times = (BYTE)m_pVar->GetDword(_T("time"), szField, GT_INVALID);
		pFConsolidateProto->nSuccessRate = m_pVar->GetDword(_T("success_rate"), szField);
		pFConsolidateProto->dwMoneyConsume = m_pVar->GetDword(_T("money_consume"), szField, 0);
		pFConsolidateProto->fcoefficientA = (FLOAT)m_pVar->GetFloat(_T("coefficientA"), szField, GT_INVALID);
		pFConsolidateProto->fcoefficientB = (FLOAT)m_pVar->GetFloat(_T("coefficientB"), szField, GT_INVALID);
		pFConsolidateProto->fcoefficientC = (FLOAT)m_pVar->GetFloat(_T("coefficientC"), szField, GT_INVALID);
		pFConsolidateProto->fcoefficientD = (FLOAT)m_pVar->GetFloat(_T("coefficientD"), szField, GT_INVALID);
		pFConsolidateProto->nConsTimesPct = m_pVar->GetDword(_T("add_pct"),szField,0);

		TCHAR szTemp[X_LONG_STRING];
		for( INT i = 0; i < MAX_FCONSOLIDATE_NUM; ++i )
		{
			_stprintf(szTemp,_T("att%d"),i+1);
			pFConsolidateProto->RoleAttEffect[i].eRoleAtt = (ERoleAttribute)m_pVar->GetInt(szTemp, szField, GT_INVALID);
			_stprintf(szTemp,_T("val%d"),i+1);
			pFConsolidateProto->RoleAttEffect[i].nValue = m_pVar->GetDword(szTemp, szField, GT_INVALID);
		}

		for( INT n = 1; n <= MAX_CONSOLIDATE_STUFF_QUANTITY; ++n )
		{
			_stprintf(szTemp, _T("stuff%d_id"), n);
			pFConsolidateProto->ConsolidateStuff[n - 1].dwStuffID = m_pVar->GetDword(szTemp, szField, GT_INVALID);
			_stprintf(szTemp, _T("stuff%d_type"), n);
			pFConsolidateProto->ConsolidateStuff[n - 1].eStuffType = (EStuffType)m_pVar->GetDword(szTemp, szField, EST_Null);
			_stprintf(szTemp, _T("stuff%d_num"), n);
			pFConsolidateProto->ConsolidateStuff[n - 1].dwStuffNum = m_pVar->GetDword(szTemp, szField, GT_INVALID);

			if(pFConsolidateProto->ConsolidateStuff[n - 1].dwStuffNum != GT_INVALID)
				pFConsolidateProto->nTotleStuff += pFConsolidateProto->ConsolidateStuff[n - 1].dwStuffNum;
		}
		return &pFConsolidateProto->dwID;
	}
	return NULL;
}

const tagFConsolidateProto * AttRes::GetOneFConsolidateProto(DWORD dwConsID)
{
	return m_mapFConsolidateProto.Peek(dwConsID);
}

VOID * AttRes::InitOneFConsolidateLuckySignPct(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pFConsolidateLuckySignPct, nProtoType, tagFConsolidateLuckySignPct);
	if( P_VALID(pFConsolidateLuckySignPct) )
	{
		pFConsolidateLuckySignPct->dwID = m_pVar->GetDword(_T("id"),szField);
		pFConsolidateLuckySignPct->nPct = m_pVar->GetDword(_T("pct"),szField);
		return &pFConsolidateLuckySignPct->dwID;
	}
	return NULL;
}
const tagFConsolidateLuckySignPct* AttRes::GetOneFConsolidateLuckySignPct(DWORD dwID)
{
	return m_mapFConsolidateLuckySignPct.Peek(dwID);
}

VOID* AttRes::InitOneFAttachSoulProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pSoulStone, nProtoType, tagFAttachSoulProto);
	if( P_VALID(pSoulStone) )
	{
		pSoulStone->dwConLevel = m_pVar->GetDword(_T("id"),szField);
		pSoulStone->dwStoneTypeID = m_pVar->GetDword(_T("item_id"),szField);
		pSoulStone->dwBindStoneTypeID = m_pVar->GetDword(_T("item_id2"),szField);
		return &pSoulStone->dwConLevel;
	}
	return NULL;
}

const tagFAttachSoulProto* AttRes::GetOneFuhunStone(DWORD dwConLevelID)
{
	return m_mapFAttachSoulStone.Peek(dwConLevelID);
}
VOID* AttRes::InitOneGodStrengthenProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pGodStrengthen, nProtoType, tagGodStrengthen);
	if( P_VALID(pGodStrengthen) )
	{
		pGodStrengthen->dwID			= m_pVar->GetDword(_T("id"),szField);
		pGodStrengthen->dwExp			= m_pVar->GetDword(_T("soar_levelupexp"),szField);
		pGodStrengthen->dwGreenExp	= m_pVar->GetDword(_T("green_num"),szField);
		pGodStrengthen->dwBlueExp		= m_pVar->GetDword(_T("blue_num"),szField);
		pGodStrengthen->dwOrangeExp	= m_pVar->GetDword(_T("orange_num"),szField);
		pGodStrengthen->dwPurpleExp	= m_pVar->GetDword(_T("purple_num"),szField);
		pGodStrengthen->dwPinkExp		= m_pVar->GetDword(_T("pink_num"),szField);
		pGodStrengthen->dwCost		= m_pVar->GetDword(_T("money_cost"),szField);
		return &pGodStrengthen->dwID;
	}
	return NULL;
}
const tagGodStrengthen * AttRes::GetGodStrengthenProto(DWORD dwLevel)
{
	return m_mapGodStrengthen.Peek(dwLevel);
}


const tagFabaoFoodProto* AttRes::GetFabaoFoodProto(DWORD dwTypeID)
{
	return m_mapFabaoFood.Peek(dwTypeID);
}
BOOL AttRes::IsFabaoFood(DWORD dwItemTypeID)
{
	return m_mapFabaoFood.IsExist(dwItemTypeID);
}
VOID * AttRes::InitFabaoFoodProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pFabaoFood, nProtoType, tagFabaoFoodProto);
	if( P_VALID(pFabaoFood) )
	{
		pFabaoFood->dwItemTypeID		= m_pVar->GetDword(_T("item_id"),szField);
		pFabaoFood->nLevel		= m_pVar->GetInt(_T("level"),szField,0);
		pFabaoFood->nExp		= m_pVar->GetDword(_T("exp"),szField,0);
		pFabaoFood->dwBuffID		= m_pVar->GetDword(_T("buff_id"),szField,GT_INVALID);
		pFabaoFood->dwTriggerID		= m_pVar->GetDword(_T("buff_trigger"),szField,GT_INVALID);

		TCHAR buf[50] = {0};
		for( int i = 1; i <= 3; ++i )
		{
			_stprintf(buf,_T("min_level%d"),i);
			pFabaoFood->param[i-1].nMiniLevel = m_pVar->GetDword(buf,szField,0);
			_stprintf(buf,_T("max_level%d"),i);
			pFabaoFood->param[i-1].nMaxLevel = m_pVar->GetDword(buf,szField,0);
			_stprintf(buf,_T("recovery_value%d"),i);
			pFabaoFood->param[i-1].nRecoveryValue = m_pVar->GetDword(buf,szField,0);
		}
		return &pFabaoFood->dwItemTypeID;
	}
	return NULL;
}

VOID * AttRes::InitOneMallOfferProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pMallOfferProto, nProtoType, tagMallOfferProto);
	if( P_VALID(pMallOfferProto) )
	{
		pMallOfferProto->dwID					= m_pVar->GetDword(_T("id"),szField);
		pMallOfferProto->MallOfferProto2Client.dwTypeID			= m_pVar->GetDword(_T("typeid"),szField);
		pMallOfferProto->MallOfferProto2Client.nLevel				= m_pVar->GetDword(_T("level"),szField);
		pMallOfferProto->dwPrice				= m_pVar->GetDword(_T("price"),szField);
		return &pMallOfferProto->dwID;
	}
	return NULL;
}
VOID * AttRes::InitFabaoFruitProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pFabaoFruit, nProtoType, tagFabaoFruitProto);
	if( P_VALID(pFabaoFruit) )
	{
		pFabaoFruit->dwTypeID	= m_pVar->GetDword(_T("id"),szField);
		pFabaoFruit->nLower		= m_pVar->GetInt(_T("lower_limit"),szField);
		pFabaoFruit->nHight		= m_pVar->GetInt(_T("higher_limit"),szField);
		pFabaoFruit->nExtType	= m_pVar->GetInt(_T("add_type"),szField,GT_INVALID);
		pFabaoFruit->nExtAdding	= m_pVar->GetInt(_T("increment"),szField,0);

		return &pFabaoFruit->dwTypeID;
	}
	return NULL;
}

const tagFabaoFruitProto * AttRes::GetFabaoFruitProto(DWORD dwTypeID)
{
	return m_mapFabaoFruit.Peek(dwTypeID);
}

VOID * AttRes::InitFabaoStuntProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pFabaoStunt, nProtoType, tagFabaoStuntProto);
	if( P_VALID(pFabaoStunt) )
	{
		pFabaoStunt->dwID				= m_pVar->GetDword(_T("id"),szField);
		pFabaoStunt->nStuntType		= m_pVar->GetInt(_T("type"),szField);
		pFabaoStunt->dwTriggerID		= m_pVar->GetDword(_T("trigger_id"),szField);
		pFabaoStunt->dwBuffID			= m_pVar->GetDword(_T("buff_id"),szField,GT_INVALID);
		pFabaoStunt->bCanRealized		= m_pVar->GetInt(_T("be_compre"),szField,0);
		pFabaoStunt->nIntelligenceLimit	= m_pVar->GetInt(_T("compre_quali_limit"),szField,GT_INVALID);
		pFabaoStunt->nStageLimit			= m_pVar->GetInt(_T("compre_level_limit"),szField,GT_INVALID);
		pFabaoStunt->bBinding				= m_pVar->GetDword(_T("be_binding"),szField,0);

		return &pFabaoStunt->dwID;
	}
	return NULL;
}

VOID * AttRes::InitFabaoAddPromoteTimesProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pFabaoAddPromoteTimes, nProtoType, tagFabaoAddPromoteTimesProto);
	if( P_VALID(pFabaoAddPromoteTimes) )
	{
		pFabaoAddPromoteTimes->dwID			= m_pVar->GetDword(_T("id"),szField);
		pFabaoAddPromoteTimes->dwMoney		= m_pVar->GetInt(_T("money"),szField);

		return &pFabaoAddPromoteTimes->dwID;
	}
	return NULL;
}

VOID * AttRes::InitFairyFusionQualityProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pFairyFusionQuality, nProtoType, tagFairyFusionQualityProto);
	if( P_VALID(pFairyFusionQuality) )
	{
		pFairyFusionQuality->dwID									= m_pVar->GetDword(_T("id"),szField);
		pFairyFusionQuality->n16LowScore							= m_pVar->GetDword(_T("lower_limit"),szField);
		pFairyFusionQuality->n16HighScore							= m_pVar->GetDword(_T("higher_limit"),szField);
		pFairyFusionQuality->n16QualityRate[EFairyFusion_Orange]	= m_pVar->GetDword(_T("type_pro3"),szField);
		pFairyFusionQuality->n16QualityRate[EFairyFusion_Purple]	= m_pVar->GetDword(_T("type_pro2"),szField) +
																	pFairyFusionQuality->n16QualityRate[EFairyFusion_Orange];
		pFairyFusionQuality->n16QualityRate[EFairyFusion_Pink]		= m_pVar->GetDword(_T("type_pro1"),szField) +
																	pFairyFusionQuality->n16QualityRate[EFairyFusion_Purple];

		return &pFairyFusionQuality->dwID;
	}
	return NULL;
}

VOID * AttRes::InitFairyFusionTypeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pFairyFusiontType, nProtoType, tagFairyFusionTypeProto);
	if( P_VALID(pFairyFusiontType) )
	{
		pFairyFusiontType->dwID									= m_pVar->GetDword(_T("id"),szField);

		TCHAR szBuf[128] = {0};
		INT nLastRate = 0;
		for (INT i = 0; i < MAX_FAIRY_FUSION_TYPE; i++)
		{
			_stprintf(szBuf, _T("item_pro%d"), i);
			pFairyFusiontType->dwTypeID[i]	= m_pVar->GetDword(szBuf,szField);
			_stprintf(szBuf, _T("item%d"), i);
			pFairyFusiontType->n16Rate[i]	= m_pVar->GetDword(szBuf,szField) + nLastRate;
			nLastRate = pFairyFusiontType->n16Rate[i];
		}

		return &pFairyFusiontType->dwID;
	}
	return NULL;
}

VOID * AttRes::InitWarRewardPointProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pWarRewardPoint, nProtoType, tagWarRewardPointProto);
	if( P_VALID(pWarRewardPoint) )
	{
		pWarRewardPoint->dwID									= m_pVar->GetDword(_T("id"),szField);
		pWarRewardPoint->dwExp									= m_pVar->GetDword(_T("exp_point"),szField);
		pWarRewardPoint->dwMoney								= m_pVar->GetDword(_T("money_point"),szField);

		return &pWarRewardPoint->dwID;
	}
	return NULL;
}

VOID * AttRes::InitGodBidProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pGodBid, nProtoType, tagGodBidProto);
	if( P_VALID(pGodBid) )
	{
		pGodBid->dwProtoID				= m_pVar->GetDword(_T("id"),szField);
		pGodBid->dwItemID				= m_pVar->GetDword(_T("item_id"),szField);
		pGodBid->dwItemCnt			= m_pVar->GetDword(_T("bid_quantity"),szField);
		pGodBid->eGodBidType		= static_cast<EGodBidType>(m_pVar->GetDword(_T("bid_form"),szField));
		pGodBid->dwBasePrice			= m_pVar->GetDword(_T("bid_end"),szField);
		pGodBid->dwBidLowerLimit	= m_pVar->GetDword(_T("bid_min"),szField);

		return &pGodBid->dwProtoID;
	}
	return NULL;
}

TMap<DWORD,tagFabaoStuntProto *> & AttRes::GetFabaoStuntProtos(VOID)
{
	return m_mapFabaoStunt;
}
const tagFabaoStuntProto * AttRes::GetFabaoStuntProto(DWORD dwID)
{
	return m_mapFabaoStunt.Peek(dwID);
}

TMap<DWORD,tagFabaoAddPromoteTimesProto *> & AttRes::GetFabaoAddPromoteTimesProtos(VOID)
{
	return m_mapFabaoAddPromoteTimes;
}

const tagFabaoAddPromoteTimesProto * AttRes::GetFabaoAddPromoteTimesProto(DWORD dwID)
{
	return m_mapFabaoAddPromoteTimes.Peek(dwID);
}

TMap<DWORD,tagFairyFusionQualityProto *> & AttRes::GetFairyFusionQualityProtos(VOID)
{
	return m_mapFairyFusionQuality;
}

const tagFairyFusionQualityProto * AttRes::GetFairyFusionQualityProto(DWORD dwID)
{
	return m_mapFairyFusionQuality.Peek(dwID);
}

TMap<DWORD,tagFairyFusionTypeProto *> & AttRes::GetFairyFusionTypeProtos(VOID)
{
	return m_mapFairyFusionType;
}

const tagFairyFusionTypeProto * AttRes::GetFairyFusionTypeProto(DWORD dwID)
{
	return m_mapFairyFusionType.Peek(dwID);
}

const tagWarRewardPointProto * AttRes::GetWarRewardPointProto(DWORD dwID)
{
	return m_mapWarRewardPoint.Peek(dwID);
}

VOID * AttRes::InitGuildDailyRewardProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pGuildDailyReward, nProtoType, tagGuildDailyRewardProto);
	if( P_VALID(pGuildDailyReward) )
	{
		pGuildDailyReward->dwID			= m_pVar->GetDword(_T("id"),szField);
		pGuildDailyReward->nGuildPos	= m_pVar->GetInt(_T("identity"),szField);
		pGuildDailyReward->nWinNum		= m_pVar->GetDword(_T("num"),szField);
		pGuildDailyReward->dwItemTypeID	= m_pVar->GetDword(_T("item_id"),szField,GT_INVALID);
		pGuildDailyReward->dwBuffID		= m_pVar->GetInt(_T("buff_id"),szField,0);

		return &pGuildDailyReward->dwID;
	}
	return NULL;
}
const tagGuildDailyRewardProto * AttRes::GetGuildDailyRewardProto(INT nGuildPos, INT nWinNum)
{
	TMap<DWORD,tagGuildDailyRewardProto *>::TMapIterator it = m_mapGuildDailyReward.Begin();
	tagGuildDailyRewardProto * pProto = NULL;
	while( m_mapGuildDailyReward.PeekNext(it,pProto) )
	{
		if( !P_VALID(pProto) )
			continue;
		if( pProto->nGuildPos == nGuildPos && pProto->nWinNum == nWinNum )
			return pProto;
	}
	return NULL;
}

const tagGuildDailyRewardProto * AttRes::GetGuildDailyRewardProto(DWORD dwID)
{
	return m_mapGuildDailyReward.Peek(dwID);
}

// 根据妖精得分获取修改圣灵属性的范围
const tagHolyLevelUpProto * AttRes::GetHolyLevelUpProto(INT nScore)
{
	TMap<DWORD,tagHolyLevelUpProto*>::TMapIterator it = m_mapHolyLevelUp.Begin();
	tagHolyLevelUpProto * pProto = NULL;
	while( m_mapHolyLevelUp.PeekNext(it,pProto) )
	{
		if( !P_VALID(pProto) )
			continue;
		if( pProto->dwID >= nScore )
			return pProto;
	}
	return NULL;
}

//  获取圣纹强化道具对应的强化属性
const tagHolyEquipEnhance * AttRes::GetEquipEnhanceProto(INT dwTypeId)
{
	TMap<DWORD,tagHolyEquipEnhance*>::TMapIterator it = m_mapHolyEquipEnhance.Begin();
	tagHolyEquipEnhance * pProto = NULL;
	while( m_mapHolyEquipEnhance.PeekNext(it,pProto) )
	{
		if( !P_VALID(pProto) )
			continue;
		if( pProto->dwID >= dwTypeId )
			return pProto;
	}
	return NULL;
}

// 取出妖精繁殖出的妖精typeid
DWORD	AttRes::RandomPeekFabaoBirthItem(INT nFabaoBirthValue,DWORD dwMainFabaoTypeID,DWORD * pFariyBirthType)
{
	TMap<DWORD,TFabaoBirthProto *>::TMapIterator it = m_mapFabaoBirthProto.Begin();
	DWORD dwID;
	TFabaoBirthProto * pFabaoBirthProto = NULL;
	INT nItemTypeNdx = GT_INVALID;

	while( m_mapFabaoBirthProto.PeekNext(it,dwID,pFabaoBirthProto) )
	{
		if( P_VALID(pFabaoBirthProto) )
		{
			if( pFabaoBirthProto->nLowLimit <= nFabaoBirthValue && pFabaoBirthProto->nHigherLimit >= nFabaoBirthValue )
			{
				nItemTypeNdx = (INT)pFabaoBirthProto->GetItemByPct(IUTIL->RandomInRange(1,10000));
				break;
			}
		}
	}
	if( nItemTypeNdx == GT_INVALID )
		return GT_INVALID;

	if( P_VALID(pFariyBirthType) )
		*pFariyBirthType = nItemTypeNdx;

	TMap<DWORD,tagFabaoBirthItemTypeProto *>::TMapIterator i = m_mapFabaoBirthItemTypeProto.Begin();
	tagFabaoBirthItemTypeProto * pBirthItem = NULL;
	while( m_mapFabaoBirthItemTypeProto.PeekNext(i,dwID,pBirthItem) )
	{
		if( !P_VALID(pBirthItem) )
			continue;
		if( pBirthItem->nItemType == nItemTypeNdx )
		{
			if( P_VALID(dwMainFabaoTypeID) )
			{
				map<DWORD,TFabaoBirthItem>::iterator it0 = pBirthItem->Items.find(dwMainFabaoTypeID);
				if( it0 != pBirthItem->Items.end() )
					return it0->second.GetItemByPct(IUTIL->RandomInRange(1,10000));
				else
				{
					it0 = pBirthItem->Items.begin();
					if( it0 != pBirthItem->Items.end() && it0->second.dwMainFabaoTypeID == GT_INVALID )
						return it0->second.GetItemByPct(IUTIL->RandomInRange(1,10000));
					return GT_INVALID;
				}
			}
			else
			{
				if( pBirthItem->Items.size() > 0 )
					return pBirthItem->Items.begin()->second.GetItemByPct(IUTIL->RandomInRange(1,10000));
				return GT_INVALID;
			}
		}
	}

	return GT_INVALID;
}

VOID * AttRes::InitOneFabaoBirthProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pProto, nProtoType, TFabaoBirthProto);
	if( P_VALID(pProto) )
	{
		pProto->dwID			= m_pVar->GetDword(_T("id"),szField);
		pProto->nLowLimit		= m_pVar->GetDword(_T("lower_limit"),szField);
		pProto->nHigherLimit		= m_pVar->GetDword(_T("higher_limit"),szField);
		TCHAR buf[20] = {0};
		for( int i = 0 ; i < MFabaoBirthItemsCount; ++i )
		{
			_stprintf(buf,_T("item_type%d"),i+1);
			pProto->Items[i].dwType	= m_pVar->GetDword(buf,szField,GT_INVALID);
			_stprintf(buf,_T("type_pro%d"),i+1);
			pProto->Items[i].nPct	= m_pVar->GetInt(buf,szField,0);
		}
		return &pProto->dwID;
	}
	return NULL;
}
VOID * AttRes::InitOneFabaoBirthItemTypeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pProto, nProtoType, tagFabaoBirthItemTypeProto);
	if( P_VALID(pProto) )
	{
		pProto->dwID		= m_pVar->GetDword(_T("id"),szField);
		pProto->nItemType	= m_pVar->GetInt(_T("item_type"),szField);
		DWORD dwMainFabaoTypeID = m_pVar->GetDword(_T("type_id"),szField);
		;
		tagFabaoBirthItemTypeProto * pTypeProto = m_mapFabaoBirthItemTypeProto.Peek(pProto->nItemType) ;
		TFabaoBirthItem tem;
		BOOL bFinded = FALSE;
		if( P_VALID(pTypeProto) )
		{
			bFinded = TRUE; // 为了让LoadResMap删除多余的记录
		}
		else
		{
			pTypeProto = pProto;
			tem.dwMainFabaoTypeID = dwMainFabaoTypeID;
		}

		TCHAR buf[20] = {0};
		for( int i = 0; i < MFabaoBirthItemTypes; ++i )
		{
			_stprintf(buf,_T("item%d"),i+1);
			tem.Items[i].dwType = m_pVar->GetDword(buf,szField,GT_INVALID);
			_stprintf(buf,_T("item_pro%d"),i+1);
			tem.Items[i].nPct = m_pVar->GetInt(buf,szField,0);
		}

		pTypeProto->Items[dwMainFabaoTypeID] = tem;
		if( bFinded )// 为了让LoadResMap删除多余的记录
			return NULL;
		
		return &pProto->nItemType;
	}
	return NULL;
}

VOID * AttRes::InitOneGoldEggPctProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pProto, nProtoType, tagGoldEggPctProto);
	if( P_VALID(pProto) )
	{
		pProto->dwLayer				= (BYTE)(m_pVar->GetDword(_T("id"),szField));
		pProto->n32EmptyPct		= m_pVar->GetInt(_T("empty_pct"),szField);
		for (INT nCnt = 0; nCnt < GOLD_EGG_MAX_CNT; nCnt++)
		{
			TCHAR szItemPct[16] = {0};
			_stprintf(szItemPct, _T("item_pct%d"), nCnt+1);
			pProto->n32ItemPct[nCnt] = m_pVar->GetDword(szItemPct,szField, 0);
		}
		return &pProto->dwLayer;
	}
	return NULL;
}

VOID * AttRes::InitOneGoldEggProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pProto, nProtoType, tagGoldEggProto);
	if( P_VALID(pProto) )
	{
		pProto->dwLayer				= (BYTE)(m_pVar->GetDword(_T("id"),szField));
		pProto->nGenConsum	= m_pVar->GetDword(_T("general_consum"),szField);
		pProto->nSpecConsum	= m_pVar->GetDword(_T("special_consum"),szField);
		for (INT nCnt = 0; nCnt < GOLD_EGG_MAX_CNT; nCnt++)
		{
			TCHAR szItemID[16] = {0};
			_stprintf(szItemID, _T("egg_item%d"), nCnt+1);
			pProto->dwItemID[nCnt] = m_pVar->GetDword(szItemID,szField, GT_INVALID);
		}
		return &pProto->dwLayer;
	}
	return NULL;
}

VOID * AttRes::InitOnePulseProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pProto, nProtoType, tagPulseProto);
	if( P_VALID(pProto) )
	{
		pProto->dwId = m_pVar->GetDword(_T("id"),							szField, GT_INVALID);
		pProto->dwLevel = m_pVar->GetDword(_T("level"),							szField, GT_INVALID);
		pProto->dwGodLock =  m_pVar->GetDword(_T("godlock"),							szField, GT_INVALID);
		pProto->dwLevelLock =  m_pVar->GetDword(_T("levellock"),							szField, GT_INVALID);
		pProto->dwMaxLevel =  m_pVar->GetDword(_T("maxlevel"),							szField, GT_INVALID);

		pProto->dwSkillID =  m_pVar->GetDword(_T("skillid"),							szField, GT_INVALID);
		pProto->bIsAciveSkill =  (bool)m_pVar->GetDword(_T("skilltype"),							szField, 0);
		pProto->dwSuccessRate =  m_pVar->GetDword(_T("success_rate"),							szField, GT_INVALID);

		pProto->dwItemID =  m_pVar->GetDword(_T("delete_item"),							szField, GT_INVALID);
		pProto->dwItemNum =  m_pVar->GetDword(_T("delete_item_num"),							szField, GT_INVALID);

		pProto->dwItemID2 =  m_pVar->GetDword(_T("delete_item2"),							szField, GT_INVALID);
		pProto->dwItemNum2 =  m_pVar->GetDword(_T("delete_item_num2"),							szField, GT_INVALID);

		pProto->dwMoney =  m_pVar->GetDword(_T("delete_money"),							szField, GT_INVALID);

		pProto->dwPerSkill1 =  m_pVar->GetDword(_T("pre1"),							szField, GT_INVALID);
		pProto->dwPerSkill2 =  m_pVar->GetDword(_T("pre2"),							szField, GT_INVALID);
		pProto->dwPerSkill3 =  m_pVar->GetDword(_T("pre3"),							szField, GT_INVALID);

		pProto->dwNextSkill1 =  m_pVar->GetDword(_T("next1"),							szField, GT_INVALID);
		pProto->dwNextSkill2 =  m_pVar->GetDword(_T("next2"),							szField, GT_INVALID);
		pProto->dwNextSkill3 =  m_pVar->GetDword(_T("next3"),							szField, GT_INVALID);

		return &pProto->dwId;
	}
	return NULL;
}

VOID * AttRes::InitOneTigerProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pProto, nProtoType, tagTigerProto);
	if( P_VALID(pProto) )
	{
		pProto->dwId = m_pVar->GetDword(_T("id"),							szField, GT_INVALID);
		pProto->dwRate[0] = m_pVar->GetDword(_T("times_1"),							szField, 0);
		pProto->dwRate[1] = m_pVar->GetDword(_T("times_2"),							szField, 0);
		pProto->dwRate[2] = m_pVar->GetDword(_T("times_3"),							szField, 0);
		pProto->dwRate[3] = m_pVar->GetDword(_T("times_4"),							szField, 0);
		pProto->dwRate[4] = m_pVar->GetDword(_T("times_5"),							szField, 0);
		pProto->dwRate[5] = m_pVar->GetDword(_T("times_6"),							szField, 0);
		pProto->dwRate[6] = m_pVar->GetDword(_T("times_7"),							szField, 0);

		pProto->dwNum[0] = m_pVar->GetDword(_T("num_1"),							szField, 0);
		pProto->dwNum[1] = m_pVar->GetDword(_T("num_2"),							szField, 0);
		pProto->dwNum[2] = m_pVar->GetDword(_T("num_3"),							szField, 0);
		pProto->dwNum[3] = m_pVar->GetDword(_T("num_4"),							szField, 0);
		pProto->dwNum[4] = m_pVar->GetDword(_T("num_5"),							szField, 0);
		pProto->dwNum[5] = m_pVar->GetDword(_T("num_6"),							szField, 0);
		pProto->dwNum[6] = m_pVar->GetDword(_T("num_7"),							szField, 0);

		return &pProto->dwId;
	}
	return NULL;
}

VOID * AttRes::InitOneWardrobeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pProto, nProtoType, tagWardrobeProto);
	if( P_VALID(pProto) )
	{
		pProto->dwLayerID = m_pVar->GetDword(_T("id"), szField);
		EWardrobeLayerType eLayerType = (EWardrobeLayerType)m_pVar->GetDword(_T("wardrobe_type"), szField);
		EWardrobeLayerSex eLayerSex = (EWardrobeLayerSex)m_pVar->GetDword(_T("wardrobe_sex"), szField);
		if (!IsWardrobeLayerTypeValid(eLayerType) || !IsWardrobeLayerSexValid(eLayerType,eLayerSex))
			return NULL;
		pProto->eLayerType = eLayerType;
		pProto->eLayerSex = eLayerSex;
		for (INT nFashionCnt = 0; nFashionCnt < MAX_FASHION_PERLAYER; nFashionCnt++)
		{
			TCHAR szItemID[32] = {0};
			_stprintf(szItemID, _T("wardrobe_field%d"), nFashionCnt+1);
			pProto->dwFashionID[nFashionCnt] = m_pVar->GetDword(szItemID,szField, 0);
		}
		for (INT nBuffIDCnt = 0; nBuffIDCnt < MAX_LEVEL_PERLAYER; nBuffIDCnt++)
		{
			TCHAR szBuffID[32] = {0};
			_stprintf(szBuffID, _T("buff_id%d"), nBuffIDCnt+1);
			pProto->dwBuffIDByLvl[nBuffIDCnt] = m_pVar->GetDword(szBuffID,szField, GT_INVALID);
		}
		for (INT nOtherBuffIDCnt = 0; nOtherBuffIDCnt < MAX_LEVEL_PERLAYER; nOtherBuffIDCnt++)
		{
			TCHAR szBuffID[32] = {0};
			_stprintf(szBuffID, _T("other_buff_id%d"), nOtherBuffIDCnt+1);
			pProto->dwOtherBuffIDByLvl[nOtherBuffIDCnt] = m_pVar->GetDword(szBuffID,szField, GT_INVALID);
		}

		// 衣橱层数计数器自增
		IncWardrobeLayerCnt();

		return &pProto->dwLayerID;
	}
	return NULL;
}
VOID * AttRes::InitOneAdvancedComposeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pProto, nProtoType, tagAdvancedComposeProto);
	if( P_VALID(pProto) )
	{
		pProto->dwComposeID = m_pVar->GetDword(_T("id"), szField);
		EAdvanceComposeType eAdvCompType = (EAdvanceComposeType)m_pVar->GetDword(_T("type"), szField);
		if (!IsAdvanceComposeTypeValid(eAdvCompType))
			return NULL;

		pProto->eAdvCompType = eAdvCompType;
		pProto->dwDesignID = m_pVar->GetDword(_T("design"), szField, GT_INVALID);
		if (pProto->dwDesignID == GT_INVALID)
			return NULL;

		DWORD dwGold = m_pVar->GetDword(_T("money"), szField);
		pProto->n64SilverCost = dwGold*GOLD2SILVER;
		pProto->nVitalityCost = m_pVar->GetInt(_T("vitality"), szField);
		for (INT nMaterialCnt = 0; nMaterialCnt < MAX_ADVANCED_COMPOSE_MATERIAL; nMaterialCnt++)
		{
			TCHAR szTypeID[32] = {0};
			TCHAR szCount[32] = {0};
			_stprintf(szTypeID, _T("material%d"), nMaterialCnt+1);
			_stprintf(szCount, _T("num%d"), nMaterialCnt+1);
			pProto->Material[nMaterialCnt].dwTypeID = m_pVar->GetDword(szTypeID,szField, GT_INVALID);
			pProto->Material[nMaterialCnt].dwCount = m_pVar->GetDword(szCount,szField, GT_INVALID);
		}

		for (INT nFashionComp = 0; nFashionComp < MAX_FASHION_CAN_COMPOSE; nFashionComp++)
		{
			TCHAR szTypeID[32] = {0};
			TCHAR szPct[32] = {0};
			_stprintf(szTypeID, _T("item%d"), nFashionComp+1);
			_stprintf(szPct, _T("pct%d"), nFashionComp+1);
			pProto->dwFashionComp[nFashionComp].dwTypeID = m_pVar->GetDword(szTypeID,szField, GT_INVALID);
			pProto->dwFashionComp[nFashionComp].dwPct = m_pVar->GetDword(szPct,szField, GT_INVALID);
		}
		return &pProto->dwDesignID;
	}
	return NULL;
}

//--------------------------------------------------------------------------------------------
// 初始化神职原型
VOID* AttRes::InitOneClergyProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pClergyProto, nProtoType, tagClergyProto);

	if( P_VALID(pClergyProto) )
	{
		pClergyProto->dwID				= (DWORD)m_pVar->GetDword(_T("id"), szField);									//神职ID
		pClergyProto->n8ClergyLevel		= m_pVar->GetInt(_T("clergyLevel"), szField, 0);								//神职数值等级
		pClergyProto->nNum				= m_pVar->GetInt(_T("num"), szField, 0);										//名额限制
		pClergyProto->ePromotionType	= (EPromotionType)m_pVar->GetInt(_T("promotionType"), szField, EPTT_NULL);		//晋升模式
		pClergyProto->dwClassLimit		= (EClassType)m_pVar->GetInt(_T("classLimit"), szField, EPTT_NULL);				//职业限制
		pClergyProto->nGodheadLimit		= m_pVar->GetInt(_T("godheadLimit"), szField, 0);								//神格限制
		pClergyProto->nLevelLimit		= m_pVar->GetInt(_T("levelLimit"), szField, 0);									//等级限制
		pClergyProto->eSexLimit			= (ESex)m_pVar->GetInt(_T("sexLimit"), szField, ES_Null);						//性别限制
		pClergyProto->eAttType			= (ERoleAttribute)m_pVar->GetInt(_T("attType"), szField, ERA_Null);				//属性类型
		pClergyProto->dwAttValue		= (DWORD)m_pVar->GetDword(_T("attValue"), szField, 0);							//属性数值
		pClergyProto->n8FieldNum		= m_pVar->GetInt(_T("fieldNum"), szField, 0);									//领域数量

		EGodType eType = (EGodType)m_pVar->GetInt(_T("god_type"), szField);
		pClergyProto->eType				= eType;
		if(EGT_Null != eType)
		{
			switch(eType)
			{
			case EGT_WarBoss:
				if (ES_Woman == pClergyProto->eSexLimit)
				{
					m_mapFemaleClergyBoss.Add(ECGT_War, pClergyProto->dwID);
				} 
				else
				{
					m_mapMaleClergyBoss.Add(ECGT_War, pClergyProto->dwID);
				}
				break;
			case EGT_War:
				m_mapClergyBase.Add(ECGT_War, pClergyProto->dwID);
				break;
			case EGT_SunBoss:
				if (ES_Woman == pClergyProto->eSexLimit)
				{
					m_mapFemaleClergyBoss.Add(ECGT_Sun, pClergyProto->dwID);
				} 
				else
				{
					m_mapMaleClergyBoss.Add(ECGT_Sun, pClergyProto->dwID);
				}
				break;
			case EGT_Sun:
				m_mapClergyBase.Add(ECGT_Sun, pClergyProto->dwID);
				break;
			case EGT_ManaBoss:
				if (ES_Woman == pClergyProto->eSexLimit)
				{
					m_mapFemaleClergyBoss.Add(ECGT_Mana, pClergyProto->dwID);
				} 
				else
				{
					m_mapMaleClergyBoss.Add(ECGT_Mana, pClergyProto->dwID);
				}
				break;
			case EGT_Mana:
				m_mapClergyBase.Add(ECGT_Mana, pClergyProto->dwID);
				break;
			case EGT_PeaceBoss:
				if (ES_Woman == pClergyProto->eSexLimit)
				{
					m_mapFemaleClergyBoss.Add(ECGT_Peace, pClergyProto->dwID);
				} 
				else
				{
					m_mapMaleClergyBoss.Add(ECGT_Peace, pClergyProto->dwID);
				}
				break;
			case EGT_Peace:
				m_mapClergyBase.Add(ECGT_Peace, pClergyProto->dwID);
				break;
			}
		}

		TCHAR szTemp[20] = {0};
		for( int i = 0; i < MAX_FieldNum; ++i )
		{
			wsprintf(szTemp, _T("feileID%d"), i);
			pClergyProto->dwFieldID[i] 			= (DWORD)m_pVar->GetDword(szTemp, szField, 0);					//地图ID
		}

		return &pClergyProto->dwID;
	}
	return NULL;

}

//-------------------------------------------------------------------------------------------
// 查找神职属性原型
const tagClergyProto *AttRes::GetClergyProto(DWORD dwClergyID)
{
	return m_mapClergyProto.Peek(dwClergyID);
}

const tagClergyProto *AttRes::GetClergyBase(ECampaignGodType eType)
{
	DWORD dwID = m_mapClergyBase.Peek(eType);
	return m_mapClergyProto.Peek(dwID);
}

const tagClergyProto *AttRes::GetClergyBoss(ECampaignGodType eType, ESex eSex)
{
	if(ES_Woman == eSex)
	{
		DWORD dwID = m_mapFemaleClergyBoss.Peek(eType);
		return m_mapClergyProto.Peek(dwID);
	}
	else
	{
		DWORD dwID = m_mapMaleClergyBoss.Peek(eType);
		return m_mapClergyProto.Peek(dwID);
	}
}

//-------------------------------------------------------------------------------------------
VOID * AttRes::InitOneGodHeadProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pProto, nProtoType, tagGodHeadProto);

	if( P_VALID(pProto) )
	{
		pProto->dwID				= (DWORD)m_pVar->GetDword(_T("id"), szField);
		pProto->nMinLevel			= m_pVar->GetInt(_T("lower_level"), szField);
		pProto->nMaxLevel			= m_pVar->GetInt(_T("upper_level"), szField);
		pProto->nCondenseValue		= m_pVar->GetInt(_T("agglomerate_val"), szField);
		pProto->nCanActNum			= m_pVar->GetInt(_T("can_act_num"), szField);
		
		return &pProto->dwID;
	}

	return NULL;
}

const tagGodHeadProto * AttRes::GetOneGodHeadProto(DWORD dwID)
{
	return m_mapGodHeadProto.Peek(dwID);
}

//--------------------------------------------------------------------------------------------
// 初始化神职领域原型
VOID* AttRes::InitOneGodAreaProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pGodAreaProto, nProtoType, tagGodAreaProto);

	if( P_VALID(pGodAreaProto) )
	{
		pGodAreaProto->dwID				= (DWORD)m_pVar->GetDword(_T("id"), szField);									//领域ID
		pGodAreaProto->eType			= (EGodAreaType)m_pVar->GetInt(_T("Type"), szField, EGAT_NULL);					//能力类型
		pGodAreaProto->eNatureType		= (EGodAreaNatureType)m_pVar->GetInt(_T("nature_type"), szField, EGANT_NULL);	//表现形式
		pGodAreaProto->eUseType			= (EGodAreaUseType)m_pVar->GetInt(_T("use_type"), szField, EGANUT_NULL);		//主被动
		pGodAreaProto->nLevel			= m_pVar->GetInt(_T("level"), szField, 0);										//能力等级
		pGodAreaProto->nLevelLimit		= m_pVar->GetInt(_T("level_limit"), szField, 0);								//激活等级要求
		pGodAreaProto->nGodheadLimit	= m_pVar->GetInt(_T("godhead_limit"), szField, 0);								//激活神格要求
		pGodAreaProto->nMaxNum 			= m_pVar->GetInt(_T("max_num"), szField, 0);									//存在数量
		pGodAreaProto->dwPreID 			= (DWORD)m_pVar->GetDword(_T("pre_godarea_id"), szField, 0);					//学习时领域前提
		pGodAreaProto->nFaithValue		= m_pVar->GetInt(_T("faith_val"), szField, 0);									//学习时消耗信仰
		pGodAreaProto->dwSkillID		= (DWORD)m_pVar->GetDword(_T("skill_id"), szField, 0);							//关联技能
		pGodAreaProto->dwBuffID			= (DWORD)m_pVar->GetDword(_T("buff_id"), szField, 0);							//关联状态
		pGodAreaProto->eFunctionType	= (EGodAreaFuncType)m_pVar->GetInt(_T("function_type"), szField, EGAFT_NULL);	//特殊功能	
		pGodAreaProto->nFunctionValue	= m_pVar->GetInt(_T("function_val"), szField, 0);								//特殊功能参数
		pGodAreaProto->nTime			= m_pVar->GetInt(_T("skill_timecontrol"), szField, 0);							//稀有领域时间控制
		pGodAreaProto->bContest			= m_pVar->GetInt(_T("contest"), szField, 0);									//稀有领域是否可以争夺
		

		return &pGodAreaProto->dwID;
	}
	return NULL;

}

//-------------------------------------------------------------------------------------------
// 查找神职领域属性原型
const tagGodAreaProto *AttRes::GetGodAreaProto(DWORD dwGodAreaID)
{
	return m_mapGodAreaProto.Peek(dwGodAreaID);
}
//-------------------------------------------------------------------------------------------
//地图盒子载入
VOID* AttRes::InitOneMapBoxProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pMapBoxProto, nProtoType, tagMapBoxProto);

	if( P_VALID(pMapBoxProto) )
	{
		pMapBoxProto->dwID				= (DWORD)m_pVar->GetDword(_T("id"), szField);							//盒子ID

		TCHAR szTemp[20] = {0};
		for( int i = 0; i < MAX_SKILL_MAPBOX; ++i )
		{
			wsprintf(szTemp, _T("map_id%d"), i + 1);
			pMapBoxProto->dwMapID[i] 			= (DWORD)m_pVar->GetDword(szTemp, szField, 0);					//地图ID
		}

		return &pMapBoxProto->dwID;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------------
// 查找地图盒子属性原型
const tagMapBoxProto *AttRes::GetMapBoxProto(DWORD dwID)
{
	return m_mapMapBoxProto.Peek(dwID);
}
//-------------------------------------------------------------------------------------------
//领域特效载入
VOID* AttRes::InitOneAreaEffectProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pAreaEffectProto, nProtoType, tagAreaEffectProto);

	if( P_VALID(pAreaEffectProto) )
	{
		pAreaEffectProto->dwID				= (DWORD)m_pVar->GetDword(_T("id"), szField, 0);
		pAreaEffectProto->dwBuffID			= (DWORD)m_pVar->GetDword(_T("buff_id"), szField, 0);
		
		return &pAreaEffectProto->dwID;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------------
// 查找领域特效属性原型
const tagAreaEffectProto *AttRes::GetAreaEffectProto(DWORD dwID)
{
	return m_mapAreaEffectProto.Peek(dwID);
}
//-------------------------------------------------------------------------------------------

VOID* AttRes::InitOneFamilyQuestCondition(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(pCondition, nProtoType, tagFamilyQuestAttProto);

	if( P_VALID(pCondition) )
	{
		pCondition->dwID				= (DWORD)m_pVar->GetDword(_T("id"), szField, 0);
	
		pCondition->eFinishType			= (EFamilyQuestFinishType)m_pVar->GetInt(_T("questType"), szField);
		pCondition->eJudgeType			= (EJudgeType)m_pVar->GetInt(_T("monsterType"), szField, EJT_Null);
		pCondition->nLowLevel			= m_pVar->GetInt(_T("lowLevel"), szField, 0);
		pCondition->nHighLevel			= m_pVar->GetInt(_T("highLevel"), szField, 0);
		pCondition->dwMapcrc			= m_pVar->GetDword(_T("mapCrc"), szField, 0);
		pCondition->nMonsterRank		= m_pVar->GetInt(_T("monsterLevel"), szField, ER_Null);
		
		for (int i=0; i<MAX_ItemOrMonsterID; ++i)
		{
			TCHAR szTemp[X_SHORT_NAME] = {0};
			_sntprintf(szTemp, X_SHORT_NAME, _T("id%d"), i);
			pCondition->dwItemOrMonsterID[i] = (DWORD)m_pVar->GetDword(szTemp, szField, 0);
		}

		return &pCondition->dwID;
	}

	return NULL;
}

VOID* AttRes::InitOneFamilyQuestProto(OUT LPVOID nProtoType, LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(p, nProtoType, tagFamilyQuestProto);

	if( P_VALID(p) )
	{
		p->dwID				= (DWORD)m_pVar->GetDword(_T("id"), szField, 0);

		p->eType			= (EFamilyQuestType)m_pVar->GetInt(_T("type"), szField);
		p->dwConditionID	= m_pVar->GetDword(_T("questid"), szField);
		p->nLowLevel			= m_pVar->GetInt(_T("lowLevel"), szField, 0);
		p->nHighLevel		= m_pVar->GetInt(_T("highLevel"), szField, 0);
		p->nQuestNum		= m_pVar->GetInt(_T("num"), szField, 0);

		return &p->dwID;
	}

	return NULL;
}

VOID* AttRes::InitOneFamilyRewardProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy)
{
	MTRANS_POINTER(p, nProtoType, tagFamilyQuestRewardProto);

	if( P_VALID(p) )
	{
		p->dwID	= (DWORD)m_pVar->GetDword(_T("id"), szField, 0);

		p->dwExp = m_pVar->GetDword(_T("exp"), szField, 0);
		p->dwMoney	= m_pVar->GetDword(_T("money"), szField,  0);
	
		for (int i=0; i<MAX_FQ_RewardNum; ++i)
		{
			TCHAR szTemp[X_SHORT_NAME] = {0};
			_sntprintf(szTemp, X_SHORT_NAME, _T("item%d"), i);
			p->dwItemID[i] = (DWORD)m_pVar->GetDword(szTemp, szField, 0);
		}

		for (int i=0; i<MAX_FQ_RewardNum; ++i)
		{
			TCHAR szTemp[X_SHORT_NAME] = {0};
			_sntprintf(szTemp, X_SHORT_NAME, _T("num%d"), i);
			p->dwItemNum[i] = (DWORD)m_pVar->GetDword(szTemp, szField, 0);
		}

		for (int i=0; i<MAX_FQ_RewardNum; ++i)
		{
			TCHAR szTemp[X_SHORT_NAME] = {0};
			_sntprintf(szTemp, X_SHORT_NAME, _T("itemEx%d"), i);
			p->dwItemIDEx[i] = (DWORD)m_pVar->GetDword(szTemp, szField, 0);
		}

		for (int i=0; i<MAX_FQ_RewardNum; ++i)
		{
			TCHAR szTemp[X_SHORT_NAME] = {0};
			_sntprintf(szTemp, X_SHORT_NAME, _T("numEx%d"), i);
			p->dwItemNumEx[i] = (DWORD)m_pVar->GetDword(szTemp, szField, 0);
		}

		return &p->dwID;
	}

	return NULL;
}

VOID* AttRes::InitOneKeyCodeRewardProto(OUT LPVOID nProtoType, LPCTSTR szField,  INT nDummy)
{
	MTRANS_POINTER(pProto, nProtoType, tagKeyCodeRewardProto);

	if( P_VALID(pProto) )
	{
		pProto->dwID = (DWORD)m_pVar->GetDword(_T("id"), szField, 0);
		pProto->bOnlyOnce	= (bool)m_pVar->GetInt(_T("only_once"), szField);
		for (int i=0; i<M_MaxKeyCodeRewardNum; ++i)
		{
			TCHAR szTmpStr[X_SHORT_NAME] = {0};
			_sntprintf(szTmpStr, X_SHORT_NAME, _T("item_id%d"), i+1);
			pProto->dwItemTypeID[i]	= m_pVar->GetDword(szTmpStr, szField);

			ZeroMemory(szTmpStr, X_SHORT_NAME);
			_sntprintf(szTmpStr, X_SHORT_NAME, _T("item_num%d"), i+1);
			pProto->nItemNum[i]	= m_pVar->GetInt(szTmpStr, szField);
		}

		return &pProto->dwID;
	}

	return NULL;
}
//-------------------------------------------------------------------------------------------
//加载守护妖精属性表
VOID* AttRes::InitOneFamilySpriteProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pFamilySpriteProto, nProtoType, tagFamilySpriteProto);


	if( P_VALID(pFamilySpriteProto) )
	{
		pFamilySpriteProto->nID													= (DWORD)m_pVar->GetInt(_T("id"), szField, 0);								//能力等级	作为id便于查找
		//pFamilySpriteProto->nLevel											= m_pVar->GetInt(_T("level"), szField, 0);									// 等级
		pFamilySpriteProto->dwExp												= (DWORD)m_pVar->GetDword(_T("exp"), szField, 0);							// 成长度
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_HP]						= (DWORD)m_pVar->GetDword(_T("hp"), szField, -1);							// 体力上限
		//pFamilySpriteProto->dwTrainNum										= (DWORD)m_pVar->GetDword(_T("train_num"), szField, 0);						// 修炼需求发布个数
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_EXAttack]					= (DWORD)m_pVar->GetDword(_T("ex_attack"), szField, -1);					// 物理攻击能力上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_InAttack]					= (DWORD)m_pVar->GetDword(_T("in_attack"), szField, -1);					// 法术攻击能力上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_EXDefense]					= (DWORD)m_pVar->GetDword(_T("ex_defense"), szField, -1);					// 物理防御能力上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_InDefense]					= (DWORD)m_pVar->GetDword(_T("in_defense"), szField, -1);					// 法术防御能力上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_EXAttackDeeper]			= (DWORD)m_pVar->GetDword(_T("ex_attack_add"), szField, -1);				// 物理伤害加深上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_InAttackDeeper]			= (DWORD)m_pVar->GetDword(_T("in_attack_add"), szField, -1);				// 法术伤害加深上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_EXAttackResistance]		= (DWORD)m_pVar->GetDword(_T("ex_defense_des"), szField, -1);				// 物理减免加深上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_InAttackResistance]		= (DWORD)m_pVar->GetDword(_T("in_defense_des"), szField, -1);				// 法术减免加深上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_Toughness]					= (DWORD)m_pVar->GetDword(_T("toughness"), szField, -1);					// 韧性上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_CritDes]					= (DWORD)m_pVar->GetDword(_T("crit_des"), szField, -1);						// 暴击抵消上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_ControleffectDeepen]		= (DWORD)m_pVar->GetDword(_T("control_effect_deepen"), szField, -1);		// 控制效果加深上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_ControleffectResistance]	= (DWORD)m_pVar->GetDword(_T("control_effect_resistance"), szField, -1);	// 控制抵抗加深上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_SlowingeffectDeepen]		= (DWORD)m_pVar->GetDword(_T("slowing_effect_deepen"), szField, -1);		// 减速效果加深上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_SlowingeffectResistance]	= (DWORD)m_pVar->GetDword(_T("slowing_effect_resistance"), szField, -1);	// 减速抵抗加深上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_FixedeffectDeepen]			= (DWORD)m_pVar->GetDword(_T("fixed_effect_deepen"), szField, -1);			// 固定效果加深上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_FixedeffectResistance]		= (DWORD)m_pVar->GetDword(_T("fixed_effect_resistance"), szField,-1);		// 固定抵抗加深上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_AgingeffectDeepen]			= (DWORD)m_pVar->GetDword(_T("aging_effect_deepen"), szField, -1);			// 衰弱效果加深上限
		pFamilySpriteProto->dwAbilityUpperLimit[EFSA_AgingeffectResistance]		= (DWORD)m_pVar->GetDword(_T("aging_effect_resistance"), szField, -1);		// 衰弱抵抗加深上限
		pFamilySpriteProto->dwPickTypeNum[0]									= (DWORD)m_pVar->GetDword(_T("pick_type_1"), szField, 0);					// 抽取类型1数量
		pFamilySpriteProto->dwPickTypeNum[1]									= (DWORD)m_pVar->GetDword(_T("pick_type_2"), szField, 0);					// 抽取类型2数量
		pFamilySpriteProto->dwShedingzhi									= (DWORD)m_pVar->GetDword(_T("she_ding_zhi"), szField, 0);						// 策划设定值
		
		
		return &pFamilySpriteProto->nID;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------------
// 查找守护妖精属性原型
const tagFamilySpriteProto *AttRes::GetFamilySpriteProto(DWORD dwID)
{
	return m_mapFamilySpriteProto.Peek(dwID);
}
//-------------------------------------------------------------------------------------------
//加载家族守护妖精修炼属性表
VOID* AttRes::InitOneSpriteTrainProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pSpriteTrainProto, nProtoType, tagSpriteTrainProto);


	if( P_VALID(pSpriteTrainProto) )
	{
		pSpriteTrainProto->nID						= (DWORD)m_pVar->GetInt(_T("id"), szField, 0);										
		pSpriteTrainProto->nPageType				= (DWORD)m_pVar->GetInt(_T("page_type"), szField, 0);				// 分页类型
		pSpriteTrainProto->nLevelLimit				= (DWORD)m_pVar->GetInt(_T("level_limit"), szField, 0);				// 等级限制
		pSpriteTrainProto->dwAbilityType			= (DWORD)m_pVar->GetDword(_T("ability_type"), szField, 0);			// 能力类型
		pSpriteTrainProto->dwPowerUpLimit			= (DWORD)m_pVar->GetDword(_T("power_up_limit"), szField, 0);		// 能力提升上限
		pSpriteTrainProto->dwPowerLowLimit			= (DWORD)m_pVar->GetDword(_T("power_low_limit"), szField, 0);		// 能力提升下线
		pSpriteTrainProto->dwItemID					= (DWORD)m_pVar->GetDword(_T("itemid"), szField, 0);				// 需求物品
		pSpriteTrainProto->dwItemNum				= (DWORD)m_pVar->GetDword(_T("item_num"), szField, 0);				// 需求物品数量
		pSpriteTrainProto->dwItemQuality			= (DWORD)m_pVar->GetDword(_T("item_quality"), szField, 0);			// 需求物品品级(是装备的时候才读)
		pSpriteTrainProto->dwShowUpLimit			= (DWORD)m_pVar->GetDword(_T("show_up_limit"), szField, 0);			// 出现等级上线
		pSpriteTrainProto->dwShowLowLimit			= (DWORD)m_pVar->GetDword(_T("show_low_limit"), szField, 0);		// 出现等级下线
		pSpriteTrainProto->dwPickType				= (DWORD)m_pVar->GetDword(_T("pick_type"), szField, 0);				// 抽取类型
		pSpriteTrainProto->dwSpriteExpReword		= (DWORD)m_pVar->GetDword(_T("sprite_exp_reword"), szField, 0);		// 妖精成长度奖励
		pSpriteTrainProto->dwExpReword				= (DWORD)m_pVar->GetDword(_T("exp_reword"), szField, 0);			// 经验奖励
		pSpriteTrainProto->dwItemReword				= (DWORD)m_pVar->GetDword(_T("item_reword"), szField, 0);			// 物品奖励
		pSpriteTrainProto->dwItemRewordNum			= (DWORD)m_pVar->GetDword(_T("item_reword_num"), szField, 0);		// 物品奖励数量
		pSpriteTrainProto->dwItemRewordQuality		= (DWORD)m_pVar->GetDword(_T("item_reword_quality"), szField, 0);	// 物品奖励品级(是装备的时候才读)


		//事先进行筛选
		for( int i = pSpriteTrainProto->dwShowLowLimit; i <= pSpriteTrainProto->dwShowUpLimit; ++i )
		{
			tagSpriteTrainData *pSpriteTrainData = m_mapSpriteTrainData.Peek(i);
			if( !P_VALID(pSpriteTrainData) )
			{
				pSpriteTrainData = new tagSpriteTrainData;
				if( !P_VALID(pSpriteTrainData) )
					break;

				m_mapSpriteTrainData.Add(i,pSpriteTrainData);
			}
			
			if( (DWORD)EFSPT_1 == pSpriteTrainProto->dwPickType )
			{
				pSpriteTrainData->m_vecTrainID1.push_back(pSpriteTrainProto->nID);
			}

			if( (DWORD)EFSPT_2 == pSpriteTrainProto->dwPickType )
			{
				pSpriteTrainData->m_vecTrainID2.push_back(pSpriteTrainProto->nID);
			}
		}

		return &pSpriteTrainProto->nID;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------------
// 查找家族守护妖精修炼属性原型
const tagSpriteTrainProto *AttRes::GetSpriteTrainProto(DWORD dwID)
{
	return m_mapSpriteTrainProto.Peek(dwID);
}
//-------------------------------------------------------------------------------------------
// 随机获得家族守护妖精修炼属性原型
const tagSpriteTrainProto *AttRes::RandGetSpriteTrainProto(INT nLevel,EFamilySpritePickType eType)
{
	tagSpriteTrainProto *pSpriteTrainProto = NULL;

	tagSpriteTrainData *pSpriteTrainData = m_mapSpriteTrainData.Peek(nLevel);
	if( !P_VALID(pSpriteTrainData) )
	{
		return NULL;
	}

	if(EFSPT_1 == eType)
	{
		if(pSpriteTrainData->m_vecTrainID1.size() > 0)
		{
			INT nIndex = IUTIL->RandomInRange(0,pSpriteTrainData->m_vecTrainID1.size()-1);
			return m_mapSpriteTrainProto.Peek(pSpriteTrainData->m_vecTrainID1[nIndex]);
		}
	}

	if(EFSPT_2 == eType)
	{
		if(pSpriteTrainData->m_vecTrainID2.size() > 0)
		{
			INT nIndex = IUTIL->RandomInRange(0,pSpriteTrainData->m_vecTrainID2.size()-1);
			return m_mapSpriteTrainProto.Peek(pSpriteTrainData->m_vecTrainID2[nIndex]);
		}
	}

	return NULL;
}
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitOneIdbuybackProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pIDBuyBack, nProtoType, tagIDBuyBack);


	if( P_VALID(pIDBuyBack) )
	{
		pIDBuyBack->dwID				= (DWORD)m_pVar->GetDword(_T("id"), szField, 0);										
		pIDBuyBack->dwExp				= (DWORD)m_pVar->GetDword(_T("exp"), szField, 0);				
		pIDBuyBack->dwGiftID			= (DWORD)m_pVar->GetDword(_T("gift"), szField, 0);			
		
		TCHAR tszTemp[32] = {0};
		for( int i = 0; i < MAX_GIFT_ITEM_NUM; ++i )
		{
			wsprintf( tszTemp, _T("itemID%d"), i+1 );
			pIDBuyBack->dwItemID[i]			= (DWORD)m_pVar->GetDword(tszTemp, szField, 0);
		}

		return &pIDBuyBack->dwID;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------------
VOID* AttRes::InitOneEquipGrowProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(p, nProtoType, tagEquipGrowProto);
	if (!P_VALID(p))
	{
		return NULL;
	}

	p->dwID = (DWORD)m_pVar->GetDword(_T("id"), szField);
	
	for (int i=0; i<EQUIP_GROW_MAX_LEVEL; ++i)
	{
		TCHAR szTmp[X_SHORT_STRING] = {0};
		_sntprintf(szTmp, X_SHORT_STRING, _T("exp_%d"), i+1);
		p->dwExp[i] = (DWORD)m_pVar->GetDword(szTmp, szField);
	}

	for (int j=0; j<EQUIP_GROW_ADD_ATT_MAX_NUM; ++j)
	{
		TCHAR szTmp[X_SHORT_STRING] = {0};
		_sntprintf(szTmp, X_SHORT_STRING, _T("att%d_type"), j+1);
		p->eAtt[j] = (ERoleAttribute)m_pVar->GetDword(szTmp, szField, ERA_Null);

		ZeroMemory(szTmp, X_SHORT_STRING);
		_sntprintf(szTmp, X_SHORT_STRING, _T("att%d_open_level"), j+1);
		p->byAttOpenLevel[j] = (BYTE)m_pVar->GetDword(szTmp, szField, 255);

		for (int i=0; i<EQUIP_GROW_MAX_LEVEL; ++i)
		{
			TCHAR szTmp[X_SHORT_STRING] = {0};
			_sntprintf(szTmp, X_SHORT_STRING, _T("att%d_value%d"), j+1, i+1);
			p->dwAttValue[j][i] = (DWORD)m_pVar->GetDword(szTmp, szField, 0);
		}
	}

	return &p->dwID;
}


VOID* AttRes::InitOneExpOfflineProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pExpOfflineProto, nProtoType, tagExpOfflineProto);

	if( P_VALID(pExpOfflineProto) )
	{
		pExpOfflineProto->dwID				= (DWORD)m_pVar->GetDword(_T("id"), szField, 0);										
		pExpOfflineProto->dwExpbase			= (DWORD)m_pVar->GetDword(_T("exp_base"), szField, 0);				
		pExpOfflineProto->dwExpAdditional	= (DWORD)m_pVar->GetDword(_T("exp_additional"), szField, 0);		
		pExpOfflineProto->dwGold			= (DWORD)m_pVar->GetDword(_T("gold"), szField, 0);		

		return &pExpOfflineProto->dwID;
	}
	return NULL;
}

VOID* AttRes::InitOneSoulCrystalLvlLine(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(p, nProtoType, tagSoulCrystalLvlLine);

	if( P_VALID(p) )
	{
		p->byLvl	= (BYTE)m_pVar->GetInt(_T("id"), szField, 0);										
		p->dwAttSoul	= (DWORD)m_pVar->GetDword(_T("soulrequire"), szField, 0);				
		p->n64Money	= (DWORD)m_pVar->GetInt64(_T("goldrequire"), szField, 0);		
		
		return &p->byLvl;
	}
	return NULL;
}

VOID* AttRes::InitOneSoulCrystalAttData(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(p, nProtoType, tagSoulCrystalAttData);

	if( P_VALID(p) )
	{
		p->nID	= (INT)m_pVar->GetInt(_T("id"), szField, 0);										
		p->byLevel	= (BYTE)m_pVar->GetInt(_T("level"), szField, 0);				
		p->eAttType	= (ERoleAttribute)m_pVar->GetInt(_T("atttype"), szField, 0);		
		p->dwAttValue	= (DWORD)m_pVar->GetDword(_T("value"), szField, 0);		

		return &p->nID;
	}
	return NULL;
}


VOID* AttRes::InitOneForceSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pForceSkillProto, nProtoType, tagForceSkillProto);

	if( P_VALID(pForceSkillProto) )
	{
		pForceSkillProto->dwID = (DWORD)m_pVar->GetDword(_T("id"), szField, 0);
		pForceSkillProto->bySide = (DWORD)m_pVar->GetDword(_T("side"), szField, 0);		
		pForceSkillProto->dwJob = (DWORD)m_pVar->GetDword(_T("job"), szField, 0);
		pForceSkillProto->dwLevel = (DWORD)m_pVar->GetDword(_T("level"), szField, 0);
		pForceSkillProto->dwNeedSkill = (DWORD)m_pVar->GetDword(_T("needskill"), szField, 0);
		pForceSkillProto->bForerver = (DWORD)m_pVar->GetDword(_T("forever"), szField, 0);
		pForceSkillProto->dwOpenSoul = (DWORD)m_pVar->GetDword(_T("open_soul"), szField, 0);
		pForceSkillProto->dwOpenMoney = (DWORD)m_pVar->GetDword(_T("open_money"), szField, 0);
		pForceSkillProto->dwContinueSoul = (DWORD)m_pVar->GetDword(_T("continue_soul"), szField, 0);
		pForceSkillProto->dwContinueMoney = (DWORD)m_pVar->GetDword(_T("continue_money"), szField, 0);
		pForceSkillProto->bDelete = (DWORD)m_pVar->GetDword(_T("delete"), szField, 0);

		return &pForceSkillProto->dwID;
	}
	return NULL;
}


VOID* AttRes::InitOneForceLevelProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy)
{
	MTRANS_POINTER(pForceLevelProto, nProtoType, tagForceLevelProto);

	if( P_VALID(pForceLevelProto) )
	{
		pForceLevelProto->dwID				= (DWORD)m_pVar->GetDword(_T("id"), szField, 0);										
		pForceLevelProto->dwPoint			= (DWORD)m_pVar->GetDword(_T("point"), szField, 0);				

		return &pForceLevelProto->dwID;
	}
	return NULL;
}

const tagForceSkillProto* AttRes::GetForceSkillProtoEx(DWORD dwID)
{
	const tagForceSkillProto* pForceSkillProto = g_attRes.GetForceSkillProto(dwID);
	if(!P_VALID(pForceSkillProto))
	{
		//return GT_INVALID;
		//如果没有找到
		DWORD dwLevelTemp = Skill::GetLevelFromTypeID(dwID);
		if(dwLevelTemp > 1)
		{
			DWORD dwSkillIDTemp = Skill::GetIDFromTypeID(dwID);

			pForceSkillProto = g_attRes.GetForceSkillProto(Skill::CreateTypeID(dwSkillIDTemp,1));
			if(!P_VALID(pForceSkillProto))
			{
				return NULL;
			}
			else
			{
				return pForceSkillProto;
			}
		}
		else
		{
			return NULL;
		}
	}

	return pForceSkillProto;
}


