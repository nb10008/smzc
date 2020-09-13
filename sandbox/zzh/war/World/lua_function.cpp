//--------------------------------------------------------------------------------
//!\file lua_functions.cpp
//!\author Aslan
//!
//!\date 2009-03-13
//! last 2009-03-13
//!
//!\brief Lua脚本函数定义
//!
//!	Copyright (c) 2004 CTCRST Entertainment All rights reserved.
//--------------------------------------------------------------------------------
#include "StdAfx.h"
#include "script_mgr.h"

#include "map.h"
#include "map_mgr.h"
#include "map_creator.h"
#include "map_instance.h"
#include "unit.h"
#include "role.h"
#include "creature.h"
#include "creature_ai.h"
#include "skill.h"
#include "gm_policy.h"
#include "buff.h"
#include "activity_mgr.h"
#include "role_mgr.h"
#include "group_mgr.h"
#include "team.h"
#include "item_creator.h"
#include "MsgInfoMgr.h"
#include "guild.h"
#include "guild_mgr.h"
#include "pet_pocket.h"
#include "title_mgr.h"
#include "city.h"
#include "city_mgr.h"
#include "city_struggle.h"
#include "guild_member.h"
#include "quest_mgr.h"
#include "../WorldDefine/city_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/quest.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/msg_digbao.h"
#include "../WorldDefine/TreasureChest_define.h"
#include "../WorldDefine/msg_virgin_online.h"
#include "../WorldDefine/msg_fishing.h"
#include "../WorldDefine/msg_social.h"
#include "../WorldDefine/msg_city_struggle.h"
#include "../WorldDefine/guild_define.h"
#include "../WorldDefine/msg_frame.h"
#include "../WorldDefine/msg_master_apprentice.h"
#include "../WorldDefine/group_define.h"
#include "../WorldDefine/msg_group.h"
#include "../WorldDefine/remote_role.h"
#include "../WorldDefine/msg_script.h"
#include "../WorldDefine/msg_func_npc.h"

#include "athletics_PvP_define.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "../WorldDefine/msg_train.h"
#include "../WorldDefine/quest.h"
#include "att_res.h"
#include "TreasureChest_mgr.h"
#include "world_session.h"
#include "../ServerDefine/role_data.h"
#include "GuildBattle.h"
#include "../ServerDefine/master_apprentice.h"

#include "athletics_room_mgr.h"
#include "athletics_room.h"
#include "athletics_system.h"
#include "map_instance_stable.h"
#include "athletics_pvp.h"
#include "athletics_pvp_mgr.h"

#include "../WorldDefine/msg_VoteShow_define.h"
#include "../ServerDefine/msg_draft_show.h"
#include "../WorldDefine/msg_GuildBattle.h"

#include "draft_show.h"
#include "player_regression.h"
// Jason 2010-1-27 v1.3.2 玩家回归/离线奖励相关
#include "player_account.h"

#include "pet_att.h"
#include "pet_soul.h"

#include "../WorldDefine/msg_activity.h"
#include "fbbs_session.h"
#include "family.h"
#include "family_mgr.h"
#include "../ServerDefine/data_collect.h"
#include "data_collect.h"

extern AthleticsSystem g_AthleticsSystem;

//--------------------------------------------------------------------------------
// 脚本函数注册
//--------------------------------------------------------------------------------
static int SC_RegisterCreatureEvent(lua_State* L);
static int SC_RegisterQuestEvent(lua_State* L);
static int SC_RegisterRoleEvent(lua_State* L);
static int SC_RegisterMapEvent(lua_State* L);
static int SC_RegisterActEvent(lua_State* L);
static int SC_RegisterWorldEvent(lua_State* L);
static int SC_RegisterItemEvent(lua_State* L);
static int SC_RegisterSkillEvent(lua_State* L);
static int SC_RegisterMotionEvent(lua_State* L);
static int SC_RegisterBuffEvent(lua_State* L);

//---------------------------------------------------------------------------------
// 脚本锁函数
//---------------------------------------------------------------------------------
//static int SC_CreateScriptMutex(lua_State* L);
//static int SC_LockScriptMutex(lua_State* L);
//static int SC_UnLockScriptMutex(lua_State* L);

//---------------------------------------------------------------------------------
// 玩家和生物共有的接口函数
//---------------------------------------------------------------------------------
static int SC_ChangeHP(lua_State* L);
static int SC_ChangeMP(lua_State* L);
static int SC_AddBuff(lua_State* L);
static int SC_CancelBuff(lua_State* L); 
static int SC_GetPosition(lua_State* L);
static int SC_IsDead(lua_State* L);
static int SC_IsPlayer(lua_State* L);
static int SC_IsCreature(lua_State* L);
static int SC_StopAttack(lua_State* L);
static int SC_StopMove(lua_State* L);
static int SC_IsAttack(lua_State* L);
static int SC_FriendEnemy(lua_State* L);
static int SC_GetUnitAttValue(lua_State* L);
static int SC_GetAttValue(lua_State* L);
static int SC_ModAttValue(lua_State* L);
static int SC_SetAttValue(lua_State* L);
static int SC_GetUnitTypeID(lua_State* L);
static int SC_IsInState(lua_State* L);
static int SC_SetState(lua_State* L);
static int SC_UnsetState(lua_State* L);
static int SC_GetCamp(lua_State* L);
static int SC_SetCamp(lua_State* L);
static int SC_SetFaceTo(lua_State* L);
static int SC_GetRoleCreateTime(lua_State* L);

//---------------------------------------------------------------------------------
// 玩家独有函数
//---------------------------------------------------------------------------------
static int SC_RoleAddQuest(lua_State* L);
static int SC_RoleAddQuestFromNPC(lua_State* L);
static int SC_DeleteQuest(lua_State* L);
static int SC_RoleCompleteQuest(lua_State* L);
static int SC_QuestInit(lua_State* L);
static int SC_IsRoleHaveQuest(lua_State* L);
static int SC_IsRoleHaveBoardQuest(lua_State* L);
static int SC_IsRoleCompleteQuest(lua_State* L);
static int SC_RoleGotoNewMap(lua_State* L);
static int SC_GetRoleLevel(lua_State *L);
static int SC_GetRoleAttValue(lua_State *L);
static int SC_ModRoleAttValue(lua_State *L);
static int SC_QuestAddRoleItem(lua_State* L);
static int SC_BaibaoAddRoleItem(lua_State* L);
static int SC_RoleAddSkill(lua_State* L);
static int SC_IsRoleLearnedSkill(lua_State* L);
static int SC_GetRoleName(lua_State* L);
static int SC_IsRoleHaveTeam(lua_State* L);
static int SC_GetRoleTeamMemID(lua_State* L);
static int SC_SetQuestScriptData(lua_State* L);
static int SC_ModQuestScriptData(lua_State* L);
static int SC_GetQuestScriptData(lua_State* L);
static int SC_AddRoleItem(lua_State* L);
static int SC_RoleEquip(lua_State* L);
static int SC_RemoveFromRole(lua_State* L);
static int SC_RemoveFromEquipBar(lua_State* L);
static int SC_RemoveFromWare(lua_State* L);
static int SC_GetBagFreeSize(lua_State* L);
static int SC_GetRoleItemNum(lua_State* L);
static int SC_AddRoleExp(lua_State* L);
static int SC_AddRoleSilver(lua_State* L);
static int SC_DecRoleSilver(lua_State* L);
static int SC_GetRoleSilver(lua_State* L);
static int SC_GetRoleSex(lua_State* L);
static int SC_GetRoldDressid(lua_State* L);
static int SC_SetRoleScriptData(lua_State* L);
static int SC_ModRoleScriptData(lua_State* L);
static int SC_GetRoleScriptData(lua_State* L);
static int SC_AddRoleExVolume(lua_State* L);
static int SC_IsRoleInStatus(lua_State* L);
static int SC_CanMount(lua_State* L);
static int SC_CanGather(lua_State* L);
static int SC_GetRoleLevelUpExp(lua_State* L);
static int SC_IsRoleOnLine(lua_State* L);
static int SC_GetActiveLongHunNum(lua_State* L);
static int SC_CastActiveLongHun(lua_State* L);
static int SC_ProlifeLongHun(lua_State* L);
static int SC_IsRoleOnStall(lua_State* L);
static int SC_SetRoleState(lua_State* L);
static int SC_UnSetRoleState(lua_State* L);
static int SC_CanOpenChest(lua_State* L);
static int SC_SendChestMsg(lua_State* L);
static int SC_IncTreasureSum(lua_State* L);
static int SC_GetTreasureSum(lua_State* L);
static int SC_GetChestItem(lua_State* L);
static int SC_ItemNeedBroadcast(lua_State* L);
static int SC_StopMount(lua_State* L);
static int SC_InstanceNotify(lua_State* L);
static int SC_IsRoleMarried(lua_State* L);
static int SC_IsRoleWedded(lua_State* L);
static int SC_GetFriendShipValue(lua_State* L);
static int SC_SigTitleEvent(lua_State* L);
static int SC_SigRemoveTitleEvent(lua_State* L);
static int SC_ChangeSkillExp(lua_State* L);
static int SC_IsRoleHaveBuff(lua_State* L);
static int SC_QuestSetRebornMap(lua_State* L);
static int SC_VirginOnlineSendClient(lua_State* L);
static int SC_FishingSendClient(lua_State* L);
static int SC_RoleCancelSkill(lua_State* L);
static int SC_RoleFixSpirit(lua_State* L);
static int SC_AddFriendShipValue(lua_State* L);
static int SC_IsCouple(lua_State* L);
static int SC_GetRoleYuanbao(lua_State* L);
static int SC_RoleUseSkill(lua_State* L);
static int SC_GetMasterID(lua_State* L);
static int SC_RoleBecomeMaster(lua_State* L);
static int SC_LevelUpAddJingWuPoint(lua_State* L);
static int SC_LevelUpAddBonus(lua_State* L);
static int SC_GraduateNotice(lua_State* L);
static int SC_GetGraduateRewardNum(lua_State* L);
static int SC_SetGraduateRewardNum(lua_State* L);
static int SC_GetQinWuPoint(lua_State* L);
static int SC_SetQinWuPoint(lua_State* L);
static int SC_GetLevelUpBonus(lua_State* L);
static int SC_SetLevelUpBonus(lua_State* L);
static int SC_GetKillObjExp(lua_State* L);
static int SC_EudemonSummonCountDown(lua_State* L);
static int SC_ModRankData(lua_State* L);
static int SC_GetRoleRankData(lua_State* L);
static int SC_GetDisplayRank(lua_State* L);
static int SC_CancelRoleStallState(lua_State* L);
static int SC_ModSpecialTargetValue(lua_State* L);
static int SC_SetRoleClass(lua_State* L);
static int SC_GetRoleClass(lua_State* L);
static int SC_GetRoleCompleteBoardQuestNum(lua_State* L);
//static int SC_ModSpecialTargetValue(lua_State* L);
static int SC_NotifyFBBS(lua_State* L);
static int SC_IsFairyItem(lua_State* L);
static int SC_GetRoleQuestNum(lua_State* L);
static int SC_GetWardrobeDetailInfo(lua_State* L);
                
// Jason 2010-1-19 v1.3.2 玩家离线时间
static int SC_GetPlayerOfflineTime(lua_State* L);

// Jason 2010-1-27 v1.3.2 账号离线时间
int SC_GetPlayerAccountOfflineTime(lua_State* L);
int SC_SetRecAccountRegressionRewardFlag(lua_State* L);
int SC_CanRecAccountRegressionReward(lua_State* L);
// Jason 2010-1-29 v1.3.2 脚本控制删除一个技能
int SC_RemoveSkill(lua_State* L);
static int SC_GetConsumptiveStrength(lua_State* L);
static int SC_SetConsumptiveStrength(lua_State* L);
static int SC_GetRoleMapID(lua_State* L);
// Jason 2010-3-16 v1.5.0
static int SC_AddRoleGiftPoints(lua_State * L);
// Jason 2010-7-28 v1.0.0
static INT SC_GetRoleYaojingLevel(lua_State * L);
static INT SC_PutDownItem(lua_State * L);
static int SC_GetBuffWrapTimes(lua_State* L);

// 竞技场-服务器提供LUA调用接口
static int SC_SetForceInArena(lua_State* L);
static int SC_LetRoleLeaveTeam(lua_State* L);
static int SC_CreatANewTeam(lua_State* L);
static int SC_LetRoleJoinTeam(lua_State* L);
static int SC_ShowAlivePeopleInArena(lua_State* L);
static int SC_ShowKillerPictureInArena(lua_State* L);
static int SC_RoleGotoArena(lua_State* L);
static int SC_ShowArenaEndFrame(lua_State* L);
static int SC_ShowArenaRoomFrame(lua_State* L);
static int SC_ForcedToRevive(lua_State* L);
static int SC_DeletRoleFromArenaTable(lua_State* L);
static int SC_GetArenaType(lua_State* L);
static int SC_GetArenaRoomLimit(lua_State* L);
static int SC_JoinArena(lua_State* L);
static int SC_ShowAlivePeopleNameInArena(lua_State* L);
static int SC_CloseArenaWaitingRoom(lua_State* L);
static int SC_GetRoleSoulCrystalLvl(lua_State* L);

// 成就系统
static int SC_OpenTrain(lua_State* L);
static int SC_GetClanRepValue(lua_State* L);
static int SC_GetPetStepGrade(lua_State* L);
static int SC_GetFstBoxWpnPosyTimes(lua_State* L);
static int SC_GetFstBoxWpnEngraveTimes(lua_State* L);
static int SC_GetFstBoxWpnEquipHoleCnt(lua_State* L);
static int SC_GetGuildExploit(lua_State* L);
static int SC_GetPlayerTrainCnt(lua_State* L);
static int SC_IsRoleHaveTitle(lua_State* L);
static int SC_GodMiraclePoints(lua_State* L);
static int SC_ModifyMiraclePoints(lua_State* L);
static int SC_GetTrialCompleteNum(lua_State* L);
static int SC_SetTrialCompleteNum(lua_State* L);
static int SC_TrialComplete(lua_State* L);
static int SC_GetTrialState(lua_State* L);
static int SC_GetFriendNum(lua_State* L);
static int SC_GetEquipIDOnPos(lua_State* L);
static int SC_GetEquipLvLimit(lua_State* L);
static int SC_GetEquipQlty(lua_State* L);

// 脚本装备强化
static int SC_GetEquipStrengthLevel(lua_State* L);
static int SC_SetEquipStrengthLevel(lua_State* L);
static int SC_DigHoleOnEquip(lua_State* L);
static int SC_PutStoneOnEquip(lua_State* L);
static int SC_SetEquipAdditionalAbility(lua_State* L);
static int SC_SetGrowEquipLevel(lua_State* L);

// 给客户端发送城战个人战绩信息
static int SC_SendCityStruggleRecord2Role(lua_State* L);

static int SC_GetRoleGuildAtt(lua_State* L);
static int SC_GetSoarValue(lua_State* L);
static int SC_SetSoarValue(lua_State* L);
static int SC_GetGuildAtt(lua_State* L);
static int SC_SyncFlagAttack(lua_State* L);

// 夺宝奇兵脚本接口
static int SC_SyncOpenChaseRemainTime(lua_State* L);
static int SC_ForceLootChase(lua_State* L);
static int SC_GetCurMoveState(lua_State* L);

static int SC_UnsetStateBefAdd2Map(lua_State* L);

// 家族相关
static int SC_GetRoleFamilyID(lua_State* L);
static int SC_GetRoleLoverID(lua_State* L);
static int SC_GetFamilyMemberID(lua_State* L);
static int SC_GetFamilyLeaderID(lua_State* L);


static int SC_AddRoleAwardData(lua_State* L);
static int SC_GetRoleAwardPoint(lua_State* L);

static int SC_GetRoleSpecFlag(lua_State* L);
static int SC_SetRoleSpecFlag(lua_State* L);

// 玩家回归
static int SC_GetAccountReactiveStatus(lua_State* L);
static int SC_GetAccountReactiveKey(lua_State* L);
static int SC_GetAccountReactiveScore(lua_State* L);
static int SC_PopInputReactiveKey(lua_State* L);
static int SC_GetAccountReactiveLevel(lua_State* L);
static int SC_SetAccountReactiveLevel(lua_State* L);

static int SC_GetRoleSoul(lua_State* L);
static int SC_SetRoleSoul(lua_State* L);
static int SC_GetRoleSoulPoint(lua_State* L);
static int SC_SetRoleSoulPoint(lua_State* L);

static int SC_GetSkillActiveTime(lua_State* L);
static int SC_SetSkillActiveTime(lua_State* L);

static int SC_DotaSendMsg(lua_State* L);


//---------------------------------------------------------------------------------
// 帮派相关接口
//---------------------------------------------------------------------------------
static int SC_GetRoleGuildID(lua_State* L);						// 得到玩家帮派ID（玩家没有帮派返回GT_INVALID)
static int SC_GetGuildPos(lua_State* L);
static int SC_GetGuildFund(lua_State* L);
static int SC_GetGuildMaterial(lua_State* L);
static int SC_GetGuildPeace(lua_State* L);
static int SC_GetGuildReputation(lua_State* L);

static int SC_IsGuildInStatus(lua_State* L);
static int SC_SetGuildStatus(lua_State* L);
static int SC_UnsetGuildStatus(lua_State* L);

static int SC_ModifyGuildFund(lua_State* L);
static int SC_ModifyGuildMaterial(lua_State* L);
static int SC_ModifyGuildPeace(lua_State* L);
static int SC_ModifyGuildReputation(lua_State* L);
static int SC_ModifyContribution(lua_State* L);
static int SC_ModifyMemberExploit(lua_State* L);
static int SC_IsHaveGuild(lua_State* L);
static int SC_AddBuffToAllGuildMember(lua_State* L);
static int SC_RemoveBuffFromAllGuildMember(lua_State* L);
static int SC_UnsetStateOfAllGuildMember(lua_State* L);
static int SC_UnsetStateOfAllRoleInBattleField(lua_State* L);
static int SC_GetAllGuildMember(lua_State* L);
static int SC_InChangeForGuildMember(lua_State* L);
static int SC_GuildRoleInterGodArea(lua_State* L);
static int SC_SetRoleStartActiveTimes(lua_State* L);
static int SC_GetGodMiracleCurState(lua_State* L);
static int SC_UpdateGodMiracleMin(lua_State* L);
static int SC_UpdateGodMiracle(lua_State* L);
static int SC_SetCaptureGodGuildID(lua_State* L);
static int SC_GetCaptureGodGuildID(lua_State* L);
static int SC_SetApplyGuildID(lua_State* L);
static int SC_GetApplyGuildID(lua_State* L);
static int SC_SetGuildRoleDie(lua_State* L);
static int SC_CalCaptureContributionAndExploit(lua_State* L);
static int SC_InitGodMiracle(lua_State* L);
static int SC_SetGodCurState(lua_State* L);
static int SC_RoleLeaveArea(lua_State* L);
static int SC_SendAllCoptureGodGuildMember(lua_State* L);
static int SC_SetGodGuildPoint(lua_State* L);
static int SC_GetDefendGuildID(lua_State* L);
static int SC_GetGuildAllianceType(lua_State* L);

static int SC_SetGuildValue(lua_State* L);
static int SC_GetGuildValue(lua_State* L);

//---------------------------------------------------------------------------------
// 怪物独有函数
//---------------------------------------------------------------------------------
static int SC_GetAroundCreature(lua_State* L);
static int SC_GetAroundRole(lua_State* L);
static int SC_SetCreatureScriptData(lua_State* L);
static int SC_ModCreatureScriptData(lua_State* L);
static int SC_GetCreatureScriptData(lua_State* L);
static int SC_SetCreatureUpdateAITimer(lua_State* L);
static int SC_CreatureUseSkill(lua_State* L);
static int SC_MonsterSay(lua_State* L);
static int SC_MonsterPlayAction(lua_State* L);
static int SC_CreatureChangeAIState(lua_State* L);
static int SC_GetEnterCombatTick(lua_State* L);
static int SC_GetCreatureTypeID(lua_State* L);
static int SC_AddEnmity(lua_State* L);
static int SC_GetCreatureCurTargetID(lua_State* L);
// Jason 2010-4-13 v2.0.0
static int SC_PullCreatureToPos(lua_State* L);
static int SC_IsHaveBuff(lua_State* L);

static int SC_GetEnemyList(lua_State* L);
static int SC_SetEnemyValue(lua_State* L);
static int SC_GetCampByCreatureTid(lua_State* L);
static int SC_SetCampByCreatureTid(lua_State* L);

//---------------------------------------------------------------------------------
// 固定活动接口
//---------------------------------------------------------------------------------
static int SC_InitEventTime(lua_State* L);
static int SC_GetActIsStart(lua_State* L);
static int SC_AddAllRoleBuff(lua_State* L);
static int SC_SetActScriptData(lua_State* L);
static int SC_GetActScriptData(lua_State* L);
static int SC_SaveActScriptData(lua_State* L);
static int SC_GetGuildCaptureGodMiracleNum(lua_State* L);
static int SC_GetDuoBaoQiBingOpenState(lua_State* L);

//---------------------------------------------------------------------------------
// 地图接口
//---------------------------------------------------------------------------------
static int SC_SetMapScriptData(lua_State* L);
static int SC_GetMapScriptData(lua_State* L);
static int SC_MapCreateCreature(lua_State* L);
static int SC_MapCreateColCreature(lua_State* L);
static int SC_MapDeleteCreature(lua_State* L);
static int SC_MapPlaySceneEffectByObjID(lua_State* L);
static int SC_MapPlaySceneEffectByID(lua_State* L);
static int SC_MapPlaySceneMusic(lua_State* L);
static int SC_AddMapRoleBuff(lua_State* L);
static int SC_CreateInstance(lua_State* L);
static int SC_DeleteInstance(lua_State* L);
static int SC_IsInArea(lua_State* L);
static int SC_PlaySceneEffect(lua_State* L);
static int SC_StopSceneEffect(lua_State* L);
static int SC_OpenCloseDoor(lua_State* L);
static int SC_OpenCloseAllDoors(lua_State* L);
static int SC_GetDoorPos(lua_State* L);
static int SC_OpenCloseDoorByPos(lua_State* L);
static int SC_GetMapAllRoleID(lua_State* L);
static int SC_IsCreateMap(lua_State* L);
static int SC_PutItemNearRandCreature(lua_State* L);
static int SC_RemoveGroundItem(lua_State* L);
static int SC_MoveRole2AnotherMap(lua_State* L);
static int SC_SetMonsterGenState(lua_State* L);

//---------------------------------------------------------------------------------
// 城市系统接口
//---------------------------------------------------------------------------------
static int SC_GetConfirmGuild(lua_State* L);
static int SC_GetCityAppointedAtt(lua_State* L);
static int SC_ChangeCityHolder(lua_State* L);
static int SC_SetCityStruggleState(lua_State* L);
static int SC_SetReviveTime(lua_State* L);
static int SC_GetRoleForce(lua_State* L);
static int SC_EnterLeaveCityStruggle(lua_State* L);
static int SC_ChangeCityDoor(lua_State* L);
static int SC_GetCityAppointedAttByMapID(lua_State* L);
static int SC_ChangeCityDefence(lua_State* L);
static int SC_SendCityStruggleInfo(lua_State* L);
static int SC_SendOneCityStruggleInfo(lua_State* L);
static int SC_InitCSRoleData(lua_State* L);
static int SC_AddRoleDataInfo(lua_State* L);
static int SC_AddKillRoleNum(lua_State* L);
static int SC_AddRoleData(lua_State* L);
static int SC_GetRoleCSScore(lua_State* L);
static int SC_SortByScore(lua_State* L);
static int SC_GetRoleKillNum(lua_State* L);
static int SC_GetCSRankedRoleID(lua_State* L);
static int SC_SetCityStruggleResult(lua_State* L);
static int SC_SummonPersonChange(lua_State* L);
static int SC_SetSummonTime(lua_State* L);
static int SC_StopSummon(lua_State* L);

//---------------------------------------------------------------------------------
// 脚本消息接口
//---------------------------------------------------------------------------------
static int SC_BeginMsgEvent(lua_State* L);
static int SC_AddMsgEvent(lua_State* L);
static int SC_DispatchRoleMsgEvent(lua_State* L);
static int SC_DispatchWorldMsgEvent(lua_State* L);
static int SC_DispatchMapMsgEvent(lua_State* L); 
static int SC_DispatchGuildMsgEvent(lua_State* L); 
static int SC_DispatchAllianceGuildMsgEvent(lua_State* L); 
static int SC_DispatchBroadcast(lua_State* L);// 播放指定ID的服务器广播
static int SC_SendWorldSwitchMsg(lua_State* L);
static int SC_SendRoleSwitchMsg(lua_State* L);

//---------------------------------------------------------------------------------
// 职业系统相关接口
//---------------------------------------------------------------------------------
static int SC_IsRoleInaugural(lua_State* L);
static int SC_ChangeClass(lua_State* L); 

//---------------------------------------------------------------------------------
// 系统相关接口
//---------------------------------------------------------------------------------
static int SC_TimeGetTime(lua_State* L);
static int SC_DataTime2DWORDTime(lua_State* L);
static int SC_GetCurrentDWORDTime(lua_State* L);
static int SC_CalcTimeDiff(lua_State* L);
static int SC_IncreaseTime(lua_State* L);
static int SC_DecreaseTime(lua_State* L);
static int SC_GetGMExpRate(lua_State* L);
static int SC_GetWorldTick(lua_State* L);
static int SC_CalUnitDist(lua_State* L);
static int SC_IsOldSerNewPlayer(lua_State* L);
static int SC_MultiRateReward(lua_State* L);
static int SC_GetWorldDataTable(lua_State* L);
static int SC_SetWorldDataTable(lua_State* L);
static int SC_GetRoleDataTable(lua_State* L);
static int SC_SetRoleDataTable(lua_State* L);

//---------------------------------------------------------------------------------
// 脚本log记录
//---------------------------------------------------------------------------------
static int SC_WriteLog(lua_State* L);
static int SC_Trace(lua_State * L);

//--------------------------------------------------------------------------------
// 64位数据处理
//--------------------------------------------------------------------------------
static void		push_64bit_data(lua_State* L, INT64 n64Data);
static INT64	pop_64bit_data(lua_State* L, int nHigh, int nLow);

//---------------------------------------------------------------------------------
// 玩家小队接口
//---------------------------------------------------------------------------------
static int SC_GetTeamLeaderID(lua_State* L);
static int SC_CreateTeam(lua_State* L);
static int SC_AddTeamMember(lua_State* L);

//---------------------------------------------------------------------------------
// 弹出窗口界面
//---------------------------------------------------------------------------------
static int SC_OpenFramePage(lua_State* L);

//元神相关
static int SC_GetSoulMaxLevelExp(lua_State* L);
static int SC_AddHolySoulExp(lua_State* L);
static int SC_GetSoulCurLevel(lua_State* L);

static int SC_GetHolySoulRebornLevel(lua_State* L);

static int SC_GetCurCovalue(lua_State* L);
static int SC_AddCurCovalue(lua_State* L);
static int SC_GetCurSoulValue(lua_State* L);
static int SC_AddCurSoulValue(lua_State* L);
static int SC_GetMaxSoulValue(lua_State* L);
static int SC_GetSoulCurExp(lua_State* L);
static int SC_ReloadHolySoulAtt(lua_State* L);
static int SC_SetHunJingStrength(lua_State* L);
static int SC_SetWarGold(lua_State* L);
static int SC_GetWarGold(lua_State* L);


//神职
static int SC_GetRoleClergy(lua_State* L);

//宝石强化，刷成长id
static int SC_GetEquipGrowAttID(lua_State* L);	
static int SC_SetEquipGrowAttID(lua_State* L);	
static int SC_GetEquipStoneAddLevel(lua_State* L);
static int SC_SetEquipStoneAddLevel(lua_State* L);
//商城积分
static int SC_AddMallMark(lua_State* L);


//---------------------------------------------------------------------------------
// 辅助包
//---------------------------------------------------------------------------------
static const luaL_Reg auxLib[] =
{
	{"RegisterCreatureEvent",			SC_RegisterCreatureEvent},
	{"RegisterQuestEvent",				SC_RegisterQuestEvent},
	{"RegisterRoleEvent",				SC_RegisterRoleEvent},
	{"RegisterMapEvent",				SC_RegisterMapEvent},
	{"RegisterActEvent",				SC_RegisterActEvent},
	{"RegisterWorldEvent",				SC_RegisterWorldEvent},
	{"RegisterItemEvent",				SC_RegisterItemEvent},
	{"RegisterSkillEvent",				SC_RegisterSkillEvent},
	{"RegisterMotionEvent",             SC_RegisterMotionEvent},
	{"RegisterBuffEvent",				SC_RegisterBuffEvent},
	//{"CreateScriptMutex",				SC_CreateScriptMutex},
	//{"LockScriptMutex",					SC_LockScriptMutex},
	//{"UnLockScriptMutex",				SC_UnLockScriptMutex},
	{"TimeGetTime",						SC_TimeGetTime},
	{"GetWorldTick",					SC_GetWorldTick},
	{"WriteLog",						SC_WriteLog},
	{"DataTime2DWORDTime",				SC_DataTime2DWORDTime},
	{"GetCurrentDWORDTime",				SC_GetCurrentDWORDTime},
	{"CalcTimeDiff",					SC_CalcTimeDiff},
	{"IncreaseTime",					SC_IncreaseTime},
	{"DecreaseTime",					SC_DecreaseTime},
	{"GetGMExpRate",					SC_GetGMExpRate},
	{"Trace",							SC_Trace},
	{"CalUnitDist",						SC_CalUnitDist},
	{"IsOldSerNewPlayer",		SC_IsOldSerNewPlayer},
	{"MultiRateReward",		SC_MultiRateReward},
	{"GetWorldDataTable",	SC_GetWorldDataTable},
	{"SetWorldDataTable",		SC_SetWorldDataTable},
	{"GetRoleDataTable",		SC_GetRoleDataTable},
	{"SetRoleDataTable",		SC_SetRoleDataTable},

	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// 玩家和生物共有接口包
//----------------------------------------------------------------------------------
static const luaL_Reg unitLib[] = 
{
	{"ChangeHP",						SC_ChangeHP},
	{"ChangeMP",						SC_ChangeMP},
	{"AddBuff",							SC_AddBuff},
	{"CancelBuff",						SC_CancelBuff},
	{"GetPosition",						SC_GetPosition},
	{"IsDead",							SC_IsDead},
	{"IsPlayer",						SC_IsPlayer},
	{"IsCreature",						SC_IsCreature},
	{"StopMove",						SC_StopMove},
	{"StopAttack",						SC_StopAttack},
	{"IsAttack",						SC_IsAttack},
	{"FriendEnemy",						SC_FriendEnemy},
	{"GetUnitAttValue",					SC_GetUnitAttValue},
	{"GetAttValue",						SC_GetAttValue},
	{"ModAttValue",						SC_ModAttValue},
	{"SetAttValue",						SC_SetAttValue},
	{"GetUnitTypeID",					SC_GetUnitTypeID},
	{"IsInState",						SC_IsInState},
	{"SetState",						SC_SetState},
	{"UnsetState",						SC_UnsetState},
	{"GetCamp",							SC_GetCamp},
	{"SetCamp",							SC_SetCamp},
	{"SetFaceTo",						SC_SetFaceTo},
	
	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// 固定活动接口
//----------------------------------------------------------------------------------
static const luaL_Reg actLib[] =
{
	{"InitEventTime",					SC_InitEventTime},
	{"GetActIsStart",					SC_GetActIsStart},
	{"AddAllRoleBuff",					SC_AddAllRoleBuff},
	{"SetActScriptData",				SC_SetActScriptData},
	{"GetActScriptData",				SC_GetActScriptData},
	{"SaveActScriptData",				SC_SaveActScriptData},
	{"GetGuildCaptureGodMiracleNum",	SC_GetGuildCaptureGodMiracleNum},
	{"GetDuoBaoQiBingOpenState",	SC_GetDuoBaoQiBingOpenState},
	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// 地图事件接口
//----------------------------------------------------------------------------------
static const luaL_Reg mapLib[] = 
{
	{"SetMapScriptData",				SC_SetMapScriptData},
	{"GetMapScriptData",				SC_GetMapScriptData},
	{"MapCreateCreature",				SC_MapCreateCreature},
	{"MapCreateColCreature",			SC_MapCreateColCreature},
	{"MapDeleteCreature",				SC_MapDeleteCreature},
	{"CreateInstance",					SC_CreateInstance},
	{"DeleteInstance",					SC_DeleteInstance},
	{"AddMapRoleBuff",					SC_AddMapRoleBuff},
	{"IsInArea",						SC_IsInArea},
//	{"MapPlaySceneEffectByObjID",		SC_MapPlaySceneEffectByObjID},
//	{"MapPlaySceneEffectByID",			SC_MapPlaySceneEffectByID},
//	{"MapPlaySceneMusic",				SC_MapPlaySceneMusic},
	{"PlaySceneEffect",					SC_PlaySceneEffect},
	{"StopSceneEffect",					SC_StopSceneEffect},
	{"OpenCloseDoor",					SC_OpenCloseDoor},
	{"OpenCloseAllDoors",				SC_OpenCloseAllDoors},
	{"GetDoorPos",						SC_GetDoorPos},
	{"OpenCloseDoorByPos",				SC_OpenCloseDoorByPos},
	{"GetMapAllRoleID",					SC_GetMapAllRoleID},
	{"IsCreateMap",						SC_IsCreateMap},
	{"PutItemNearRandCreature",			SC_PutItemNearRandCreature},
	{"RemoveGroundItem",				SC_RemoveGroundItem},
	{"MoveRole2AnotherMap",				SC_MoveRole2AnotherMap},
	{"SetMonsterGenState",				SC_SetMonsterGenState},

	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// 玩家事件接口
//----------------------------------------------------------------------------------
static const luaL_Reg roleLib[] = 
{
	{"RoleGotoNewMap",					SC_RoleGotoNewMap},
	{"GetRoleLevel",					SC_GetRoleLevel},
	{"GetRoleAttValue",					SC_GetRoleAttValue},
	{"ModRoleAttValue",					SC_ModRoleAttValue},
	{"AddQuest",						SC_RoleAddQuest},
	{"AddQuestFromNPC",						SC_RoleAddQuestFromNPC},
	{"DeleteQuest",						SC_DeleteQuest},
	{"RoleCompleteQuest",				SC_RoleCompleteQuest},
	{"QuestInit",						SC_QuestInit},
	{"QuestAddRoleItem",				SC_QuestAddRoleItem},
	{"BaibaoAddRoleItem",				SC_BaibaoAddRoleItem},
	{"AddSkill",						SC_RoleAddSkill},
	{"IsLearnedSkill",					SC_IsRoleLearnedSkill},
	{"GetRoleName",						SC_GetRoleName},
	{"IsRoleHaveTeam",					SC_IsRoleHaveTeam},
	{"GetRoleTeamMemID",				SC_GetRoleTeamMemID},
	{"SetQuestScriptData",				SC_SetQuestScriptData},
	{"GetQuestScriptData",				SC_GetQuestScriptData},
	{"ModQuestScriptData",				SC_ModQuestScriptData},
	{"IsRoleHaveQuest",					SC_IsRoleHaveQuest},
	{"IsRoleHaveBoardQuest",			SC_IsRoleHaveBoardQuest},
	{"IsRoleCompleteQuest",				SC_IsRoleCompleteQuest},
	{"AddRoleItem",						SC_AddRoleItem},
	{"RoleEquip",						SC_RoleEquip},
	{"RemoveFromRole",					SC_RemoveFromRole},
	{"RemoveFromEquipBar",				SC_RemoveFromEquipBar},
	{"RemoveFromWare",					SC_RemoveFromWare},
	{"GetBagFreeSize",					SC_GetBagFreeSize},
	{"GetRoleItemNum",					SC_GetRoleItemNum},
	{"AddRoleExp",						SC_AddRoleExp},
	{"AddRoleSilver",					SC_AddRoleSilver},
	{"DecRoleSilver",					SC_DecRoleSilver},
	{"GetRoleSilver",					SC_GetRoleSilver},
	{"GetRoleSex",						SC_GetRoleSex},
	{"GetRoleDressid",					SC_GetRoldDressid},
	{"SetRoleScriptData",				SC_SetRoleScriptData},
	{"ModRoleScriptData",				SC_ModRoleScriptData},
	{"GetRoleScriptData",				SC_GetRoleScriptData},
	{"AddRoleExVolume",					SC_AddRoleExVolume},
	{"IsRoleInStatus",					SC_IsRoleInStatus},
	{"CanMount",						SC_CanMount},
	{"CanGather",						SC_CanGather},
	{"GetRoleLevelUpExp",				SC_GetRoleLevelUpExp},
	{"IsRoleOnLine",					SC_IsRoleOnLine},
	{"GetLongHunID",					SC_GetActiveLongHunNum},
	{"CastLongHun",						SC_CastActiveLongHun},
	{"ProlifeLongHun",					SC_ProlifeLongHun},
	{"IsRoleOnStall",					SC_IsRoleOnStall},	
	{"CanOpenChest",					SC_CanOpenChest},		
	{"SendChestMsg",					SC_SendChestMsg},		
	{"IncTreasureSum",					SC_IncTreasureSum},		
	{"GetTreasureSum",					SC_GetTreasureSum},		
	{"GetChestItem",					SC_GetChestItem},
	{"ItemNeedBroadcast",				SC_ItemNeedBroadcast},
	{"SetRoleState",					SC_SetRoleState},
	{"UnSetRoleState",					SC_UnSetRoleState},
	{"StopMount",						SC_StopMount},
	{"InstanceNotify",					SC_InstanceNotify},
	{"IsRoleMarried",					SC_IsRoleMarried},
	{"IsRoleWedded",					SC_IsRoleWedded},
	{"GetFriendShipValue",				SC_GetFriendShipValue},
	{"SigTitleEvent",					SC_SigTitleEvent},
	{"SigRemoveTitleEvent",				SC_SigRemoveTitleEvent},
	{"ChangeSkillExp",					SC_ChangeSkillExp},
	{"IsRoleHaveBuff",					SC_IsRoleHaveBuff},
	{"QuestSetRebornMap",				SC_QuestSetRebornMap},
	{"VirginOnlineSendClient",			SC_VirginOnlineSendClient},
	{"FishingSendClient",				SC_FishingSendClient},
	{"IsCouple",						SC_IsCouple},
	{"GetRoleYuanbao",					SC_GetRoleYuanbao},
	{"RoleCancelSkill",					SC_RoleCancelSkill},
	{"RoleUseSkill",					SC_RoleUseSkill},
	{"GetMasterID",						SC_GetMasterID},
	{"RoleBecomeMaster",				SC_RoleBecomeMaster},
	{"LevelUpAddJingWuPoint",			SC_LevelUpAddJingWuPoint},
	{"LevelUpAddBonus",					SC_LevelUpAddBonus},
	{"GraduateNotice",					SC_GraduateNotice},
	{"GetGraduateRewardNum",			SC_GetGraduateRewardNum},
	{"SetGraduateRewardNum",			SC_SetGraduateRewardNum},
	{"GetQinWuPoint",					SC_GetQinWuPoint},
	{"SetQinWuPoint",					SC_SetQinWuPoint},
	{"GetLevelUpBonus",					SC_GetLevelUpBonus},
	{"SetLevelUpBonus",					SC_SetLevelUpBonus},
	{"GetKillObjExp",					SC_GetKillObjExp},
	{"EudemonSummonCountDown",			SC_EudemonSummonCountDown},
	{"ModRankData",			            SC_ModRankData},
	{"GetRoleRankData",			        SC_GetRoleRankData},
	{"GetDisplayRank",			        SC_GetDisplayRank},
	{"CancelRoleStallState",			SC_CancelRoleStallState},
	{"GetWardrobeDetailInfo",			SC_GetWardrobeDetailInfo},
	         
	// Jason 2010-1-19 v1.3.2
	{"GetPlayerOfflineTime",			SC_GetPlayerOfflineTime},
	{"RoleFixSpirit",					SC_RoleFixSpirit},	//  设置精力值cd时间
	// Jason 2010-1-27 v.1.3.2 离线奖励/回归
	{"GetPlayerAccountOfflineTime" ,	SC_GetPlayerAccountOfflineTime},
	{"CanRecAccountRegressionReward" ,	SC_CanRecAccountRegressionReward},
	{"SetRecAccountRegressionRewardFlag" ,	SC_SetRecAccountRegressionRewardFlag},
	// Jason 2010-1-29 v1.3.2 删除玩家一个技能
	{"RemoveSkill"					,	SC_RemoveSkill},

	{"OpenFramePage",					SC_OpenFramePage},
	{"GetConsumptiveStrength",			SC_GetConsumptiveStrength},								
	{"SetConsumptiveStrength",			SC_SetConsumptiveStrength},
	{"GetRoleMapID",					SC_GetRoleMapID},
	// Jason 2010-3-16 v1.5.0
	{"AddRoleGiftPoints",				SC_AddRoleGiftPoints},

	// 竞技场-服务器提供LUA调用接口
	{"SetForceInArena",					SC_SetForceInArena},
	{"LetRoleLeaveTeam",				SC_LetRoleLeaveTeam},
	{"CreatANewTeam",					SC_CreatANewTeam},
	{"LetRoleJoinTeam",					SC_LetRoleJoinTeam},
	{"ShowAlivePeopleInArena",			SC_ShowAlivePeopleInArena},
	{"ShowKillerPictureInArena",		SC_ShowKillerPictureInArena},
	{"ShowArenaEndFrame",				SC_ShowArenaEndFrame},
	{"ShowArenaRoomFrame",				SC_ShowArenaRoomFrame},
	{"RoleGotoArena",					SC_RoleGotoArena},
	{"DeletRoleFromArenaTable",			SC_DeletRoleFromArenaTable},
	{"ForcedToRevive",					SC_ForcedToRevive},
	{"GetArenaType",					SC_GetArenaType},
	{"GetArenaRoomLimit",               SC_GetArenaRoomLimit},
	{"JoinArena",                       SC_JoinArena},
	{"ShowAlivePeopleNameInArena",      SC_ShowAlivePeopleNameInArena},
	{"CloseArenaWaitingRoom",           SC_CloseArenaWaitingRoom},
	{"GetRoleSoulCrystalLvl",           SC_GetRoleSoulCrystalLvl},
	 
	// 龙之试炼系统-服务器提供LUA调用接口
	{"OpenTrain",								SC_OpenTrain},
	{"GetClanRepValue",					SC_GetClanRepValue},
	{"GetPetStepGrade",					SC_GetPetStepGrade},
	{"GetFstBoxWpnPosyTimes",			SC_GetFstBoxWpnPosyTimes},
	{"GetFstBoxWpnEngraveTimes",		SC_GetFstBoxWpnEngraveTimes},
	{"GetFstBoxWpnEquipHoleCnt",		SC_GetFstBoxWpnEquipHoleCnt},
	{"GetGuildExploit",					SC_GetGuildExploit},
	{"GetPlayerTrainCnt",				SC_GetPlayerTrainCnt},
	{"IsRoleHaveTitle",					SC_IsRoleHaveTitle},
	{"GodMiraclePoints",				SC_GodMiraclePoints},
	{"ModifyMiraclePoints",				SC_ModifyMiraclePoints},
	{"GetTrialCompleteNum",			SC_GetTrialCompleteNum},
	{"SetTrialCompleteNum",			SC_SetTrialCompleteNum},
	{"TrialComplete",						SC_TrialComplete},
	{"GetTrialState",						SC_GetTrialState},
	{"GetFriendNum",					SC_GetFriendNum},
	{"GetEquipIDOnPos",				SC_GetEquipIDOnPos},
	{"GetEquipLvLimit",					SC_GetEquipLvLimit},
	{"GetEquipQlty",						SC_GetEquipQlty},

	// 给客户端发送城战个人战绩信息
	{"SendCityStruggleRecord2Role",		SC_SendCityStruggleRecord2Role},
	{"GetSoarValue",					SC_GetSoarValue},
	{"SetSoarValue",					SC_SetSoarValue},
	{"GetGuildAtt",						SC_GetRoleGuildAtt},

	{"ModSpecialTargetValue",			SC_ModSpecialTargetValue},
	{"SetRoleClass",					SC_SetRoleClass},
	{"GetRoleClass",					SC_GetRoleClass},
	{"GetRoleCompleteBoardQuestNum",	SC_GetRoleCompleteBoardQuestNum},
	{"GetYaojingLevel",					SC_GetRoleYaojingLevel},
	{"PutDownItem",						SC_PutDownItem},

	{"SyncOpenChaseRemainTime",			SC_SyncOpenChaseRemainTime},
	{"ForceLootChase",					SC_ForceLootChase},
	{"GetCurMoveState",				SC_GetCurMoveState},

	{"UnsetStateBefAdd2Map",		SC_UnsetStateBefAdd2Map},
	{"GetBuffLayer",				SC_GetBuffWrapTimes},
	{"NotifyFBBS",					SC_NotifyFBBS},
	{"IsFairyItem",					SC_IsFairyItem},
	{"GetRoleQuestNum",				SC_GetRoleQuestNum},
	{"GetRoleClergy",				SC_GetRoleClergy},

	
	{"GetRoleFamilyID", SC_GetRoleFamilyID},
	{"GetRoleLoverID",	SC_GetRoleLoverID},
	{"AddRoleAwardData", SC_AddRoleAwardData},
	{"GetRoleAwardPoint", SC_GetRoleAwardPoint},
	
	{"GetRoleSpecFlag",		SC_GetRoleSpecFlag},
	{"SetRoleSpecFlag",		SC_SetRoleSpecFlag},

	{"GetAccountReactiveStatus",		SC_GetAccountReactiveStatus},
	{"GetAccountReactiveKey",			SC_GetAccountReactiveKey},
	{"GetAccountReactiveScore",		SC_GetAccountReactiveScore},
	{"PopInputReactiveKey",				SC_PopInputReactiveKey},
	{"GetAccountReactiveLevel",		SC_GetAccountReactiveLevel},
	{"SetAccountReactiveLevel",		SC_SetAccountReactiveLevel},

	{"GetRoleSoul",			SC_GetRoleSoul},
	{"SetRoleSoul",			SC_SetRoleSoul},
	{"GetRoleSoulPoint",	SC_GetRoleSoulPoint},
	{"SetRoleSoulPoint",	SC_SetRoleSoulPoint},

	{"GetSkillActiveTime",	SC_GetSkillActiveTime},
	{"SetSkillActiveTime",	SC_SetSkillActiveTime},

	//天选者之战添加借口
	{"DotaSendMsg",			SC_DotaSendMsg},
	{"GetEquipStrengthLevel",	SC_GetEquipStrengthLevel},
	{"SetEquipStrengthLevel",	SC_SetEquipStrengthLevel},
	{"DigHoleOnEquip",	SC_DigHoleOnEquip},
	{"PutStoneOnEquip",	SC_PutStoneOnEquip},
	{"SetEquipAdditionalAbility",	SC_SetEquipAdditionalAbility},
	{"SetGrowEquipLevel",	SC_SetGrowEquipLevel},
	//刷成长属性，宝石强化相关接口
	{"GetEquipGrowAttID",		SC_GetEquipGrowAttID},		//获取装备成长id
	{"SetEquipGrowAttID",		SC_SetEquipGrowAttID},		//设置装备成长id
	{"GetEquipStoneAddLevel",	SC_GetEquipStoneAddLevel},	//获取宝石强化等级
	{"SetEquipStoneAddLevel",	SC_SetEquipStoneAddLevel},	//设置宝石强化等级
	//商城积分
	{"AddMallMark",				SC_AddMallMark},			//增加商城积分
	//角色创建时间
	{"GetRoleCreateTime",		SC_GetRoleCreateTime},
	//元神相关
	{"GetSoulMaxLevelExp",				SC_GetSoulMaxLevelExp},
	{"AddHolySoulExp",					SC_AddHolySoulExp},
	{"GetSoulCurLevel",					SC_GetSoulCurLevel},	
	{"GetHolySoulRebornLevel",			SC_GetHolySoulRebornLevel},
	{"GetSoulCurExp",					SC_GetSoulCurExp},
	{"GetCurCovalue",					SC_GetCurCovalue},
	{"AddCurCovalue",					SC_AddCurCovalue},
	{"GetCurSoulValue",					SC_GetCurSoulValue},
	{"AddCurSoulValue",					SC_AddCurSoulValue},
	{"GetMaxSoulValue",					SC_GetMaxSoulValue},
	{"ReloadHolySoulAtt",				SC_ReloadHolySoulAtt},

	{"SetHunJingStrength",				SC_SetHunJingStrength},
	{"SetWarGold",						SC_SetWarGold},
	{"GetWarGold",						SC_GetWarGold},

	{NULL, NULL},
};

//--------------------------------------------------------------------------------------
// 家族函数接口
//--------------------------------------------------------------------------------------
static const luaL_Reg familyLib[] = 
{
	{"GetFamilyAllMemberID",	SC_GetFamilyMemberID},
	{"GetFamilyLeaderID",	SC_GetFamilyLeaderID},
};

//--------------------------------------------------------------------------------------
// 消息事件接口
//--------------------------------------------------------------------------------------
static const luaL_Reg msgLib[] = 
{
	{"BeginMsgEvent",					SC_BeginMsgEvent},
	{"AddMsgEvent",						SC_AddMsgEvent},
	{"DispatchRoleMsgEvent",			SC_DispatchRoleMsgEvent},
	{"DispatchWorldMsgEvent",			SC_DispatchWorldMsgEvent},
	{"DispatchMapMsgEvent",				SC_DispatchMapMsgEvent},
	{"DispatchGuildMsgEvent",			SC_DispatchGuildMsgEvent},
	{"DispatchAllianceGuildMsgEvent",	SC_DispatchAllianceGuildMsgEvent},
	{"DispatchBroadcast",				SC_DispatchBroadcast},// 播放指定ID的服务器广播
	{"SendWorldSwitchMsg",				SC_SendWorldSwitchMsg},
	{"SendRoleSwitchMsg",				SC_SendRoleSwitchMsg},
	{NULL, NULL},
};

//--------------------------------------------------------------------------------------
// 怪物事件接口
//--------------------------------------------------------------------------------------
static const luaL_Reg creLib[] = 
{
	{"GetAroundCreature",				SC_GetAroundCreature},
	{"GetAroundRole",					SC_GetAroundRole},
	{"GetCreatureScriptData",			SC_GetCreatureScriptData},
	{"SetCreatureScriptData",			SC_SetCreatureScriptData},
	{"ModCreatureScriptData",			SC_ModCreatureScriptData},
	{"SetCreatureAITimer",				SC_SetCreatureUpdateAITimer},
	{"CreatureUseSkill",				SC_CreatureUseSkill},
	{"MonsterSay",						SC_MonsterSay},
	{"MonsterPlayerAction",				SC_MonsterPlayAction},
	{"CreatureChangeAIState",			SC_CreatureChangeAIState},
	{"GetEnterCombatTick",				SC_GetEnterCombatTick},
	{"GetCreatureTypeID",				SC_GetCreatureTypeID},
	{"AddEnmity",						SC_AddEnmity},
	{"GetCreatureCurTargetID",			SC_GetCreatureCurTargetID},
	// Jason 2010-4-13 v2.0.0
	{"PullCreatureToPos",				SC_PullCreatureToPos},
	{"IsHaveBuff",						SC_IsHaveBuff},
	// lib 2012年1月10日 添加怪物仇恨列表相关接口
	{"GetEnemyList",					SC_GetEnemyList},//获取仇恨列表
	{"SetEnemyValue",					SC_SetEnemyValue},	 //设置仇恨列表中第几个的仇恨为多少多少
	{"GetCampByCreatureTid",			SC_GetCampByCreatureTid},
	{"SetCampByCreatureTid",			SC_SetCampByCreatureTid},

	{NULL,	NULL},
};

static const luaL_Reg teamLib[] = 
{
	{"CreateTeam",						SC_CreateTeam},
	{"AddTeamMember",					SC_AddTeamMember},
	{"GetTeamLeaderID",					SC_GetTeamLeaderID},
	{NULL, NULL},
};

//--------------------------------------------------------------------------------------
// 帮派接口
//--------------------------------------------------------------------------------------
static const luaL_Reg guildLib[] = 
{
	{"GetRoleGuildID",					SC_GetRoleGuildID},
	{"GetRoleGuildPos",					SC_GetGuildPos},
	{"GetGuildFund",					SC_GetGuildFund},
	{"GetGuildMaterial",				SC_GetGuildMaterial},
	{"GetGuildPeace",					SC_GetGuildPeace},
	{"GetGuildReputation",				SC_GetGuildReputation},
	{"ModifyRoleContribute",			SC_ModifyContribution},
	{"ModifyGuildFund",					SC_ModifyGuildFund},
	{"ModifyGuildMaterial",				SC_ModifyGuildMaterial},
	{"ModifyGuildPeace",				SC_ModifyGuildPeace},
	{"ModifyGuildReputation",			SC_ModifyGuildReputation},
	{"IsGuildInStatus",					SC_IsGuildInStatus},
	{"SetGuildStatus",					SC_SetGuildStatus},
	{"UnsetGuildStatus",				SC_UnsetGuildStatus},
	{"ModifyMemberExploit",				SC_ModifyMemberExploit},
	{"IsHaveGuild",						SC_IsHaveGuild},
	{"AddBuffToAllGuildMember",			SC_AddBuffToAllGuildMember},
	{"RemoveBuffFromAllGuildMember",	SC_RemoveBuffFromAllGuildMember},
	{"UnsetStateOfAllGuildMember",		SC_UnsetStateOfAllGuildMember},
	{"UnsetStateOfAllRoleInBattleField",SC_UnsetStateOfAllRoleInBattleField},
	{"GetAllGuildMember",				SC_GetAllGuildMember},
	{"IsChangeForGuildMember",			SC_InChangeForGuildMember},
	{"GuildRoleEnterGodArea",			SC_GuildRoleInterGodArea},
	{"SetRoleStartActiveTimes",			SC_SetRoleStartActiveTimes},
	{"GetGodMiracleCurState",			SC_GetGodMiracleCurState},
	{"UpdateGodMiracleMin",				SC_UpdateGodMiracleMin},
	{"UpdateGodMiracle",				SC_UpdateGodMiracle},
	{"SetCaptureGodGuildID",			SC_SetCaptureGodGuildID},
	{"GetCaptureGodGuildID",			SC_GetCaptureGodGuildID},
	{"SetApplyGuildID",					SC_SetApplyGuildID},
	{"GetApplyGuildID",					SC_GetApplyGuildID},
	{"SetGuildRoleDie",					SC_SetGuildRoleDie},
	{"CalCaptureContributionAndExploit",SC_CalCaptureContributionAndExploit},
	{"InitGodMiracle",					SC_InitGodMiracle},
	{"SetGodCurState",					SC_SetGodCurState},
	{"RoleLeaveArea",					SC_RoleLeaveArea},
	{"SendAllCoptureGodGuildMember",	SC_SendAllCoptureGodGuildMember},
	{"SetGodGuildPoint",				SC_SetGodGuildPoint},
	{"GetDefendGuildID",				SC_GetDefendGuildID},
	{"GetGuildAllianceType",			SC_GetGuildAllianceType},

	{"GetGuildAtt",						SC_GetGuildAtt},

	{"SyncFlagAttack",					SC_SyncFlagAttack},
	//梁月要的两个数值的读写
	{"SetGuildValue",					SC_SetGuildValue},
	{"GetGuildValue",					SC_GetGuildValue},

	{NULL,	NULL},
};

//--------------------------------------------------------------------------------------
// 城市系统接口
//--------------------------------------------------------------------------------------
static const luaL_Reg cityLib[] = 
{
	{"GetConfirmGuild",					SC_GetConfirmGuild},
	{"GetCityAppointedAtt",				SC_GetCityAppointedAtt},
	{"ChangeCityHolder",				SC_ChangeCityHolder},
	{"SetCityStruggleState",			SC_SetCityStruggleState},
	{"SetReviveTime",					SC_SetReviveTime},
	{"GetRoleForce",					SC_GetRoleForce},
	{"EnterLeaveCityStruggle",			SC_EnterLeaveCityStruggle},
	{"ChangeCityDoor",					SC_ChangeCityDoor},
	{"GetCityAppointedAttByMapID",		SC_GetCityAppointedAttByMapID},
	{"ChangeCityDefence",				SC_ChangeCityDefence},
	{"SendCityStruggleInfo",			SC_SendCityStruggleInfo},
	{"SendOneCityStruggleInfo",			SC_SendOneCityStruggleInfo},
	{"InitCSRoleData",					SC_InitCSRoleData},
	{"AddRoleDataInfo",					SC_AddRoleDataInfo},
	{"AddKillRoleNum",					SC_AddKillRoleNum},
	{"AddRoleData",						SC_AddRoleData},
	{"GetRoleCSScore",					SC_GetRoleCSScore},
	{"SortByScore",						SC_SortByScore},
	{"GetRoleKillNum",					SC_GetRoleKillNum},
	{"GetCSRankedRoleID",				SC_GetCSRankedRoleID},
	{"SetCityStruggleResult",			SC_SetCityStruggleResult},
	{"SummonPersonChange",				SC_SummonPersonChange},
	{"SetSummonTime",					SC_SetSummonTime},
	{"StopSummon",						SC_StopSummon},
	{NULL,	NULL},
};

//----------------------------------------------------------------------------------
// 注册C函数库函数
//----------------------------------------------------------------------------------
VOID LuaOpenCommLibs(lua_State*  L)
{
	luaL_openlib(L, "aux",  auxLib,  0);
	luaL_openlib(L, "unit", unitLib, 0);
	luaL_openlib(L, "role", roleLib, 0);
	luaL_openlib(L, "act",  actLib,  0);
	luaL_openlib(L,	"map",  mapLib,  0);
	luaL_openlib(L, "msg",  msgLib,  0);
	luaL_openlib(L, "cre",  creLib,  0);
	luaL_openlib(L, "team", teamLib, 0);
	luaL_openlib(L, "guild", guildLib, 0);
	luaL_openlib(L, "city",	cityLib, 0);
	luaL_openlib(L, "family",	familyLib, 0);
}

//--------------------------------------------------------------------------------
// 64位数据处理
//--------------------------------------------------------------------------------
void push_64bit_data(lua_State* L, INT64 n64Data)
{
	const INT32 nMask	= 0xFFFFFFFF;

	DWORD n32High	= (DWORD)((n64Data >> 32) & nMask);
	DWORD n32Low	= (DWORD)(n64Data & nMask);

	lua_pushnumber(L, n32High);
	lua_pushnumber(L, n32Low);
}

INT64 pop_64bit_data(lua_State* L, int nHigh, int nLow)
{
	const INT64 n64Mask	= 0x00000000FFFFFFFF;

	DWORD n32High	= (DWORD)lua_tonumber(L, nHigh);
	DWORD n32Low	= (DWORD)lua_tonumber(L, nLow);

	INT64 n64Data	= n32High;
	n64Data			= (n64Data << 32) | (n32Low & n64Mask);

	return n64Data;
}

//-----------------------------------------------------------------------------------
// 外部的注册函数
//-----------------------------------------------------------------------------------
int SC_RegisterCreatureEvent(lua_State* L)
{
	DWORD dwTypeID = (DWORD)luaL_checknumber(L, 1);			// 生物的ID
	INT nEventType = luaL_checkint(L, 2);				// 生物的事件类型
	const CHAR* szFunction = luaL_checkstring(L, 3);	// 脚本函数

	if( !GT_VALID(dwTypeID) || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterCreatureEvent(dwTypeID, (EScriptCreatureEvent)nEventType, szFunction);
	return 0;
}

int SC_RegisterQuestEvent(lua_State* L)
{
	UINT16 u16QuestID = (UINT16)luaL_checknumber(L, 1);	// 任务ID
	INT nEventType = luaL_checkint(L, 2);				// 事件
	const CHAR* szFunction = luaL_checkstring(L, 3);	// 脚本函数

	if( !P_VALID(u16QuestID) || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterQuestEvent(u16QuestID, (EScriptQuestEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterRoleEvent(lua_State* L)
{
	INT nEventType = luaL_checkint(L, 1);				// 事件
	const CHAR* szFunction = luaL_checkstring(L, 2);	// 脚本函数

	if( !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterRoleEvent((EScriptRoleEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterMapEvent(lua_State* L)
{
	const CHAR* szMapName = luaL_checkstring(L, 1);		// 地图名字
	INT nEventType = luaL_checkint(L, 2);				// 事件
	const CHAR* szFunction = luaL_checkstring(L, 3);	// 脚本函数

	if( !P_VALID(szMapName) || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterMapEvent(szMapName, (EScriptMapEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterActEvent(lua_State* L)
{
	DWORD dwActID = luaL_checkint(L, 1);				// 活动ID
	INT nEventType = luaL_checkint(L, 2);				// 事件
	const CHAR* szFunction = luaL_checkstring(L, 3);	// 脚本函数

	if( !GT_VALID(dwActID) || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterActEvent(dwActID, (EScriptActEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterWorldEvent(lua_State* L)
{
	INT		nEventType = luaL_checkint(L, 1);			// 事件
	const CHAR* szFunction = luaL_checkstring(L,2);	// 脚本函数

	if( !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterWorldEvent((EScriptWorldEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterItemEvent(lua_State* L)
{
	DWORD dwTypeID	=	(DWORD)luaL_checknumber(L, 1);			// 物品ID
	INT	nEventType	=	luaL_checkint(L, 2);			// 事件
	const CHAR* szFunction = luaL_checkstring(L, 3);	// 脚本函数

	if(!GT_VALID(dwTypeID) || !GT_VALID(nEventType) || !P_VALID(szFunction))
		return 0;

	g_ScriptMgr.RegisterItemEvent(dwTypeID, (EScriptItemEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterSkillEvent(lua_State* L)
{
	DWORD dwSkillID	=	(DWORD)luaL_checknumber(L, 1);			// 技能ID
	INT	nEventType	=	luaL_checkint(L, 2);			// 事件
	const CHAR* szFunction = luaL_checkstring(L, 3);	// 脚本函数

	if(!GT_VALID(dwSkillID) || !GT_VALID(nEventType) || !P_VALID(szFunction))
		return 0;

	g_ScriptMgr.RegisterSkillEvent(dwSkillID, (EScriptSkillEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterMotionEvent(lua_State* L)
{
	INT nEventType =  luaL_checkint(L,1);           //事件ID
	const CHAR* szFunction = luaL_checkstring(L,2); //脚本函数
    if(!GT_VALID( nEventType ) || !P_VALID(szFunction))
		return 0;
	g_ScriptMgr.RegisterMotionEvent((EScriptMotionEvent)nEventType, szFunction);

	return 0;

}

int SC_RegisterBuffEvent(lua_State* L)
{
	INT nEventType =  luaL_checkint(L,1);           //事件ID
	const CHAR* szFunction = luaL_checkstring(L,2); //脚本函数
	if(!GT_VALID( nEventType ) || !P_VALID(szFunction))
		return 0;
	g_ScriptMgr.RegisterBuffEvent((EScriptBuffEvent)nEventType, szFunction);

	return 0;

}

////-------------------------------------------------------------------------------------
//// 锁包
////-------------------------------------------------------------------------------------
//int SC_CreateScriptMutex(lua_State* L)
//{
//	DWORD dwMutexID = g_ScriptMgr.CreateScriptMutex();
//
//	lua_pushnumber(L, 0);
//
//	return 1;
//}
//
//int SC_LockScriptMutex(lua_State* L)
//{
//	DWORD dwMutexID = lua_tonumber(L, 1);
//
//	g_ScriptMgr.LockScriptMutex(dwMutexID);
//
//	return 0;
//}
//
//int SC_UnLockScriptMutex(lua_State* L)
//{
//	DWORD dwMutexID = lua_tonumber(L, 1);
//
//	g_ScriptMgr.UnLockScriptMutex(dwMutexID);
//
//	return 0;
//}

//-------------------------------------------------------------------------------------
// 获得系统时间
//-------------------------------------------------------------------------------------
int SC_TimeGetTime(lua_State* L)
{
	DWORD dwTime = timeGetTime();

	lua_pushnumber(L, dwTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// 得到服务器当前的心跳数
//-------------------------------------------------------------------------------------
int SC_GetWorldTick(lua_State* L)
{
	DWORD dwTick = g_world.GetWorldTick();

	lua_pushnumber(L, dwTick);

	return 1;
}

//-------------------------------------------------------------------------------------
//记录脚本log
//-------------------------------------------------------------------------------------
int SC_WriteLog(lua_State* L)
{
	ILOG->Write(IUTIL->AnsiToUnicode(lua_tostring(L, 1)));
	return 0;
}

//-------------------------------------------------------------------------------------
// 转换时间为DWORD
//-------------------------------------------------------------------------------------
int SC_DataTime2DWORDTime(lua_State* L)
{
	BYTE	byYear		=	(BYTE)lua_tonumber(L, 1);
	BYTE	byMonth		=	(BYTE)lua_tonumber(L, 2);
	BYTE	byDay		=	(BYTE)lua_tonumber(L, 3);
	BYTE	byHour		=	(BYTE)lua_tonumber(L, 4);
	BYTE	byMin		=	(BYTE)lua_tonumber(L, 5);
	BYTE	bySec		=	(BYTE)lua_tonumber(L, 6);

	if( bySec < 0 || bySec > 59 || byMin < 0 || byMin > 59 || byHour < 0 || byHour > 23 ||
		byDay < 0 || byDay > 31 || byMonth <  0 || byMonth > 12 || byYear < 0 || byYear > 63 )
	{
		ASSERT(0);
		return 0;
	}

	DWORD	dwDWORDTime = tagDWORDTime(bySec, byMin, byHour, byDay, byMonth, byYear);

	lua_pushnumber(L, dwDWORDTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// 得到当前DWORD时间
//-------------------------------------------------------------------------------------
int SC_GetCurrentDWORDTime(lua_State* L)
{
	DWORD	dwCurTime = GetCurrentDWORDTime();
	lua_pushnumber(L, dwCurTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// 计算两个日期的时间差，返回秒
//-------------------------------------------------------------------------------------
int SC_CalcTimeDiff(lua_State* L)
{
	DWORD	dwDestTime		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwSrcTime		=	(DWORD)lua_tonumber(L, 2);
	
	DWORD dwSecond = CalcTimeDiff(dwDestTime, dwSrcTime);
	lua_pushnumber(L, dwSecond);

	return 1;
}

//-------------------------------------------------------------------------------------
// 计算两个Unit坐标距离(注：两个Unit对象必须在同一地图)
//-------------------------------------------------------------------------------------
static int SC_CalUnitDist(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID1		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwUnitID2		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit1 = pMap->FindUnit(dwUnitID1);
	if( !P_VALID(pUnit1) ) return 0;

	Unit* pUnit2 = pMap->FindUnit(dwUnitID2);
	if( !P_VALID(pUnit2) ) return 0;

	// 计算两个Unit坐标距离
	LONG fDistSQ = Vec3DistSq(pUnit1->GetCurPos(), pUnit2->GetCurPos());

	lua_pushnumber(L, (fDistSQ > 750*750)?0:1);

	return 1;
}

//-------------------------------------------------------------------------------------
// 让某个时间加上某个值
//-------------------------------------------------------------------------------------
int SC_IncreaseTime(lua_State* L)
{
	DWORD	dwSrcTime		=	(DWORD)lua_tonumber(L, 1);
	INT		nIncSecond		=	(INT)lua_tonumber(L, 2);

	DWORD	dwDestTime = IncreaseTime(dwSrcTime, nIncSecond);	
	lua_tonumber(L, dwDestTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// 时间上递减指定秒数
//-------------------------------------------------------------------------------------
int SC_DecreaseTime(lua_State* L)
{
	DWORD	dwSrcTime		=	(DWORD)lua_tonumber(L, 1);
	INT		nDecSecond		=	(INT)lua_tonumber(L, 2);

	DWORD	dwDestTime = DecreaseTime(dwSrcTime, nDecSecond);
	lua_tonumber(L, dwDestTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// GM收益率
//-------------------------------------------------------------------------------------
int SC_GetGMExpRate(lua_State* L)
{
	FLOAT	fGMExpRate = g_GMPolicy.GetExpRate();

	lua_pushnumber(L, fGMExpRate);

	return 1;
}

//-------------------------------------------------------------------------------------
// 玩家和怪物共有的接口函数
//-------------------------------------------------------------------------------------
int SC_ChangeHP(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nHP				=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->ChangeHP(nHP);

	return 0;
}

int SC_ChangeMP(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nMP				=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->ChangeMP(nMP);

	return 0;
}

//-------------------------------------------------------------------------------------
// 得到Unit所在地图MapCrc和坐标
//-------------------------------------------------------------------------------------
int SC_GetPosition(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	INT nX = INT(pUnit->GetCurPos().x / TILE_SCALE);
	INT nY = INT(pUnit->GetCurPos().y);
	INT nZ = INT(pUnit->GetCurPos().z / TILE_SCALE);

	lua_pushinteger(L, nX);
	lua_pushinteger(L, nY);
	lua_pushinteger(L, nZ);

	return 3;
}

int SC_IsDead(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))	return 0;

	BOOL bDead = pUnit->IsDead();
	lua_pushboolean(L, bDead);

	return 1;
}

int SC_IsPlayer(lua_State* L)
{
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 1);
	BOOL	bPlayer			=	FALSE;

	if(IS_PLAYER(dwUnitID))
		bPlayer = TRUE;

	lua_pushboolean(L, bPlayer);

	return 1;
}

int SC_IsCreature(lua_State* L)
{
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 1);
	BOOL	bCreature		=	FALSE;

	if(IS_CREATURE(dwUnitID) || IS_PET(dwUnitID))
		bCreature = TRUE;

	lua_pushboolean(L, bCreature);

	return 1;
}

//-------------------------------------------------------------------------------------
// 让Unit停止移动
//-------------------------------------------------------------------------------------
int SC_StopMove(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	BOOL	bSendMsg		=	(BOOL)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->GetMoveData().StopMove(bSendMsg);

	return 0;
}

//-------------------------------------------------------------------------------------
// 让Unit停止攻击
//-------------------------------------------------------------------------------------
int SC_StopAttack(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->GetCombatHandler().End();

	return 0;
}

//-------------------------------------------------------------------------------------
// 单位是否在战斗中，只适用于creature
//-------------------------------------------------------------------------------------
int SC_IsAttack(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) )
	{
		return 0;
	}

	// 得到creature当前状态
	INT nFight = 0;
	if (pCreature->GetAI()->GetCurrentStateType() == AIST_Idle)
	{
		nFight = 0;
	}
	else
	{
		nFight = 1;
	}

	lua_pushinteger(L, nFight);
	return 1;

}

//-------------------------------------------------------------------------------------
// 敌我判断
//-------------------------------------------------------------------------------------
int SC_FriendEnemy(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwSrcID			=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwDstID			=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pSrc = pMap->FindUnit(dwSrcID);
	if( !P_VALID(pSrc) ) return 0;

	Unit* pDest = pMap->FindUnit(dwDstID);
	if( !P_VALID(pDest) ) return 0;

	DWORD dwFriendEnemy = pSrc->FriendEnemy(pDest);

	lua_pushnumber(L, dwFriendEnemy);

	return 1;
}

//-------------------------------------------------------------------------------------
// 获取Unit Att
//-------------------------------------------------------------------------------------
static int SC_GetUnitAttValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	lua_pushinteger(L, pUnit->GetAttValue(nIndex));

	return 1;
}

//-------------------------------------------------------------------------------------
// 读取单位当前属性
//-------------------------------------------------------------------------------------
int SC_GetAttValue(lua_State *L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	lua_pushinteger(L, pUnit->GetAttValue(nIndex));
	return 1;
}

//-------------------------------------------------------------------------------------
// 修改单位当前属性
//-------------------------------------------------------------------------------------
int SC_ModAttValue(lua_State *L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);
	INT		nValue			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	pUnit->ModAttValue(nIndex, nValue);

	return 0;
};

//-------------------------------------------------------------------------------------
// 设置单位当前属性
//-------------------------------------------------------------------------------------
int SC_SetAttValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);
	INT		nValue			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	pUnit->SetAttValue(nIndex, nValue);

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取生物的TYPEID
//-------------------------------------------------------------------------------------
int SC_GetUnitTypeID(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	// 找到地图和单位
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwUnitID);
	if( !P_VALID(pCreature) ) return 0;

	DWORD dwTypeID = pCreature->GetTypeID();

	lua_pushnumber(L, dwTypeID);

	return 1;
}

//-------------------------------------------------------------------------------------
// 判断单位是否处于某种状态
//-------------------------------------------------------------------------------------
int SC_IsInState(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	EState		eState			=	(EState)lua_tointeger(L, 4);

	// 找到地图和单位
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	lua_pushboolean(L, pUnit->IsInState(eState));
	return 1;
}

//-------------------------------------------------------------------------------------
// 设置单位状态
//-------------------------------------------------------------------------------------
int SC_SetState(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	EState		eState			=	(EState)lua_tointeger(L, 4);

	// 找到地图和单位
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->SetState(eState);
	return 0;
}

//-------------------------------------------------------------------------------------
// 反设置单位状态
//-------------------------------------------------------------------------------------
int SC_UnsetState(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	EState		eState			=	(EState)lua_tointeger(L, 4);

	// 找到地图和单位
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->UnsetState(eState);
	return 0;
}

// 获得阵营 [4/12/2012 zhangzhihua]
int SC_GetCamp(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	// 找到地图和单位
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))
	{
		lua_pushnumber(L, ECamp_Null);
		return 0;
	}

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))
	{
		lua_pushnumber(L, ECamp_Null);
		return 0;
	}

	ECampType eType = pUnit->GetCampType();
	lua_pushnumber(L, eType);

	return 1;
}

// 设置阵营 [4/12/2012 zhangzhihua]
int SC_SetCamp(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	ECampType	eType			=	(ECampType)lua_tointeger(L, 4);

	// 找到地图和单位
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))
	{
		return 0;
	}

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))
	{
		return 0;
	}

	pUnit->SetCampType(eType);

	return 1;
}

// 设置朝向 [4/12/2012 zhangzhihua]
int SC_SetFaceTo(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	FLOAT		fx				=	(FLOAT)lua_tonumber(L, 4);
	FLOAT		fz				=	(FLOAT)lua_tonumber(L, 5);

	// 找到地图和单位
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))
	{
		return 0;
	}

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))
	{
		return 0;
	}

	Vector3 vFaceTo;
	vFaceTo.x = fx;
	vFaceTo.z = fz;
	vFaceTo.y = 0.0f;

	pUnit->GetMoveData().SetFaceTo(vFaceTo, true);

	return 1;
}

//获取角色创建时间
int SC_GetRoleCreateTime(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	// 找到地图和单位
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Role* pRole = pMap->FindRole(dwUnitID);
	if( !P_VALID(pRole) ) return 0;

	tagDWORDTime pre = pRole->GetRoleCreateTime();

	lua_pushinteger(L, pre.year);
	lua_pushinteger(L, pre.month);
	lua_pushinteger(L, pre.day);
	lua_pushinteger(L, pre.hour);
	return 4;
}

static int SC_UnsetStateBefAdd2Map(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	EState		eState			=	(EState)lua_tointeger(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	pRole->UnsetState(eState);
	return 0;
}

//-------------------------------------------------------------------------------------
// 添加技能
//-------------------------------------------------------------------------------------
static int SC_RoleAddSkill(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwSkillTypeID	=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	const tagSkillProto* pProto = g_attRes.GetSkillProto(dwSkillTypeID);
	if( !P_VALID(pProto) ) return 0;

	DWORD	dwSkillID	= Skill::GetIDFromTypeID(dwSkillTypeID);
	INT		nLevel		= Skill::GetLevelFromTypeID(dwSkillTypeID);

	if(P_VALID(pRole->GetSkill(dwSkillID))) return 0;
	Skill*  pSkill = new Skill(dwSkillID, nLevel, 0, 0, 0);
	pRole->AddSkill(pSkill);

	const RoleScript * pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript) && P_VALID(pSkill))
		pRoleScript->OnLearnSkill(pRole, dwSkillTypeID);

	// 重新计算属性
	pRole->RoleRecalAtt();

	return 0;
}

//-------------------------------------------------------------------------------------
// 是否已经学会指定技能
//-------------------------------------------------------------------------------------
static int SC_IsRoleLearnedSkill(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwSkillID	=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	Skill* pSkill = pRole->GetSkill(dwSkillID);	
	if (!P_VALID(pSkill))	return 0;

	lua_pushinteger(L, pSkill->GetLevel());

	return 1;
}

int SC_AddBuff(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwBuffTypeID	=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwSrcUnitID		=	(DWORD)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	Unit* pSrcUnit = pMap->FindUnit(dwSrcUnitID);

	const tagBuffProto* pProto = g_attRes.GetBuffProto(dwBuffTypeID);
	if( !P_VALID(pProto) ) return 0;

	pUnit->TryAddBuff(pSrcUnit, pProto, NULL, NULL, NULL);

	return 0;
}

int SC_CancelBuff(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwBuffTypeID	=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	//pUnit->CancelBuff(dwBuffTypeID);
	pUnit->RemoveBuff(Buff::GetIDFromTypeID(dwBuffTypeID), TRUE);

	return 0;
}

//-------------------------------------------------------------------------------------
// 玩家独有函数
//-------------------------------------------------------------------------------------
int SC_RoleAddQuest(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	// 添加任务
	pRole->AcceptQuestFromNPC(u16QuestID, GT_INVALID);

	return 0;
}
static int SC_RoleAddQuestFromNPC(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);
	DWORD	dwNPCID	=	(UINT16)lua_tointeger(L, 3);

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	// 添加任务
	pRole->AcceptQuestFromNPC(u16QuestID, dwNPCID);
	return 0;
}
int SC_DeleteQuest(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	// 删除任务
	pRole->RemoveQuest(u16QuestID, FALSE);

	// 发送返回消息给客户端
	tagNS_DeleteQuest send;
	send.u16QuestID = u16QuestID;
	send.dwErrorCode = E_Success;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

int SC_RoleCompleteQuest(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);
	DWORD	dwNPCID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16NextQuestID = 0;

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	INT nRet = pRole->CompleteQuest(u16QuestID, dwNPCID, -1, EQCT_Normal, u16NextQuestID);

	// 发送返回
	tagNS_CompleteQuest send;
	send.u16QuestID = u16QuestID;
	send.dwErrorCode = nRet;
	pRole->SendMessage(&send, send.dwSize);

	// 检测是否有后续自动接取的任务
	if(E_Success == nRet && u16NextQuestID != 0)
		pRole->AcceptQuestFromNPC(u16NextQuestID, GT_INVALID);

	return 0;
}

//-------------------------------------------------------------------------------------
// 玩家独有函数
//-------------------------------------------------------------------------------------
int SC_QuestInit(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);
	
	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	tagQuestDynamicTarget* pTarget = pQuest->GetDynamicTarget();
	if(!P_VALID(pTarget)) return 0;

	pTarget->eTargetType = (EQuestTargetType)lua_tointeger(L, 3);	// 任务动态目标类型
	pTarget->dwQuestTipsID = (DWORD)lua_tonumber(L, 4);

	if(EQTT_NPCTalk	== pTarget->eTargetType || EQTT_Invest == pTarget->eTargetType)
	{
		for(INT i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			pTarget->dwTargetID[i] = (DWORD)lua_tonumber(L ,i + 5);
		}
	}
	else
	{
		for(INT n = 0; n < DYNAMIC_TARGET_COUNT; ++n)
		{
			pTarget->dwTargetID[n] = (DWORD)lua_tonumber(L, (2*n)+5);
			pTarget->nTargetNum[n] = (INT)lua_tonumber(L, (2*n)+6);
		}
	}

	pTarget->nRewMoney = (INT32)lua_tonumber(L, 13);
	pTarget->nRewExp = (INT32)lua_tonumber(L, 14);

	for(int i = 0; i<DYNAMIC_TARGET_COUNT; ++i)
	{
		pTarget->dwRewItemID[i] = (DWORD)lua_tonumber(L, 2*i+15);
		pTarget->dwRewItemNum[i] = (DWORD)lua_tonumber(L, 2*i+16);
	}

	for(int i = 0; i<DYNAMIC_TARGET_COUNT; ++i)
	{
		pTarget->dwOptionRewItemID[i] = (DWORD)lua_tonumber(L, 2*i+23);
		pTarget->dwOptionRewItemNum[i] = (DWORD)lua_tonumber(L, 2*i+24);
	}

	return 0;
}

int SC_RoleGotoNewMap(lua_State* L)		// 去一个新地图
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwDestMapID		=	(DWORD)lua_tonumber(L, 4);
	FLOAT	fx				=	(FLOAT)lua_tonumber(L, 5);
	FLOAT	fy				=	(FLOAT)lua_tonumber(L, 6);
	FLOAT	fz				=	(FLOAT)lua_tonumber(L, 7);

	fx = fx * TILE_SCALE;
	fy = fy * TILE_SCALE;
	fz = fz * TILE_SCALE;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	pRole->GotoNewMap(dwDestMapID, fx, fy, fz);

	return 0;
}

//-------------------------------------------------------------------------------------
// 得到玩家等级
//-------------------------------------------------------------------------------------
int SC_GetRoleLevel(lua_State *L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushinteger(L, pRole->GetLevel());
	return 1;
}

//-------------------------------------------------------------------------------------
// 读取玩家当前属性
//-------------------------------------------------------------------------------------
int SC_GetRoleAttValue(lua_State *L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	lua_pushinteger(L, pRole->GetAttValue(nIndex));
	return 1;
}

//-------------------------------------------------------------------------------------
// 修改玩家当前属性
//-------------------------------------------------------------------------------------
int SC_ModRoleAttValue(lua_State *L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);
	INT		nValue			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	pRole->ModAttValue(nIndex, nValue);

	return 0;
};

//-------------------------------------------------------------------------------------
// 添加物品装备到百宝袋
//-------------------------------------------------------------------------------------
int SC_BaibaoAddRoleItem(lua_State* L)
{
	DWORD			dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD			dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD			dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD			dwTypeID		=	(DWORD)lua_tonumber(L, 4);					// 物品TypeID
	INT				nNum			=	lua_tointeger(L, 5);				// 物品数量
	EItemQuality	eQuality		=	(EItemQuality)lua_tointeger(L, 6);	// 物品品质
	ELogCmdID		eLogCmdID		=	(ELogCmdID)lua_tointeger(L, 7);		// 记录log类型
	INT				nCreateMode		=	lua_tointeger(L, 8);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagItem* pNew = ItemCreator::CreateEx((EItemCreateMode)nCreateMode, dwRoleID, dwTypeID, nNum, eQuality);
	if(!P_VALID(pNew))
	{
		return FALSE;
	}

	pRole->GetItemMgr().ProcBaiBaoRecord(dwTypeID, dwRoleID, 
		pRole->GetNameID(), EBBRT_System, pNew->dw1stGainTime);
	pRole->GetItemMgr().Add2BaiBao(pNew, eLogCmdID, FALSE, dwRoleID);

	return 0;
}

//-------------------------------------------------------------------------------------
// 玩家获得任务物品装备
//-------------------------------------------------------------------------------------
int SC_QuestAddRoleItem(lua_State* L)
{
	DWORD			dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD			dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD			dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16			u16QuestID		=   (UINT16)lua_tointeger(L, 4);		// 任务ID
	DWORD			dwTypeID		=	(DWORD)lua_tonumber(L, 5);					// 物品TypeID
	INT				nNum			=	lua_tointeger(L, 6);				// 物品数量
	EItemQuality	eQuality		=	(EItemQuality)lua_tointeger(L, 7);	// 物品品质
	ELogCmdID		eLogCmdID		=	(ELogCmdID)lua_tointeger(L, 8);		// 记录log类型

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;
	
	pRole->GetItemMgr().Add2Role(EICM_Quest, u16QuestID, dwTypeID, (INT16)nNum, eQuality, eLogCmdID);

	return 0;
}

//-------------------------------------------------------------------------------------
// 玩家获得物品装备
//-------------------------------------------------------------------------------------
static int SC_AddRoleItem(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwTypeID		=	(DWORD)lua_tonumber(L, 4);
	INT16 n16Num		=	(INT16)lua_tointeger(L, 5);
	INT32 nQlty			=	lua_tointeger(L, 6);
	INT   nCreateMode	=	lua_tointeger(L, 7);
	INT	  nLogCmdID		=	lua_tointeger(L, 8);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	//判断背包是否有空位
	ItemMgr& itemMgr = pRole->GetItemMgr();
	if (itemMgr.GetBagFreeSize() < 1)
	{
		return -1;
	}

	if(n16Num <= 0)
	{
		n16Num = 1;
	}

	tagItem *pNewItem = ItemCreator::Create((EItemCreateMode)nCreateMode, dwRoleID, dwTypeID, n16Num,-1,0,0,10000,0,nQlty);
	if(!P_VALID(pNewItem))
	{
		return 0;
	}

	//if(MIsEquipment(pNewItem->dwTypeID))
	//{
	//	if(nQlty == GT_INVALID)
	//	{
	//		// 不鉴定	
	//	}
	//	else
	//	{
	//		ItemCreator::IdentifyEquip((tagEquip*)pNewItem, (EItemQuality)nQlty);
	//	}
	//}

	//pRole->GetItemMgr().Add2Bag(pNewItem, (ELogCmdID)nLogCmdID, TRUE);

	DWORD dwErrorCode = pRole->GetItemMgr().Add2Bag(pNewItem, (ELogCmdID)nLogCmdID, TRUE);
	//放入背包失败，则删除该物品
	if (E_Success != dwErrorCode)
	{
		Destroy(pNewItem);
		return 0;
	}

	//lua_pushnumber(L, pNewItem->n64Serial);
	push_64bit_data(L, pNewItem->n64Serial);
	
	if(!MIsEquipment(dwTypeID))
	{
		return 2;
	}

	lua_pushinteger(L, pNewItem->pEquipProto->eEquipPos);

	return 3;
}

//-------------------------------------------------------------------------------------
// 玩家穿上装备
//-------------------------------------------------------------------------------------
static int SC_RoleEquip(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	//INT64 n64Serial		=	lua_tonumber(L, 4);
	INT64 n64Serial		=	pop_64bit_data(L, 4, 5);
	INT16 n16EquipPos	=	(INT16)lua_tonumber(L, 6);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->Equip(n64Serial, (EEquipPos)n16EquipPos);

	return 0;
}

//-------------------------------------------------------------------------------------
// 从背包或任务栏删除物品
//-------------------------------------------------------------------------------------
int SC_RemoveFromRole(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwTypeID		=	(DWORD)lua_tonumber(L, 4);
	INT16 n16Num		=	(INT16)lua_tointeger(L, 5);
	INT	  nLogCmdID		=	lua_tointeger(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT nRet = pRole->GetItemMgr().RemoveFromRole(dwTypeID, n16Num, nLogCmdID);

	lua_pushinteger(L, nRet);

	return 1;
}

int SC_RemoveFromWare(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwTypeID		=	(DWORD)lua_tonumber(L, 4);
	INT16 n16Num		=	(INT16)lua_tointeger(L, 5);
	INT	  nLogCmdID		=	lua_tointeger(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT nRet = pRole->GetItemMgr().RemoveFromWare(dwTypeID, n16Num, nLogCmdID);

	lua_pushinteger(L, nRet);

	return 1;
}

int SC_RemoveFromEquipBar(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwTypeID		=	(DWORD)lua_tonumber(L, 4);
	INT16 n16Num		=	(INT16)lua_tointeger(L, 5);
	INT	  nLogCmdID		=	lua_tointeger(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT nRet = pRole->DeleteEquipFromEquipBar(dwTypeID, n16Num, nLogCmdID);

	lua_pushinteger(L, nRet);

	return 1;
}

//-------------------------------------------------------------------------------------
// 获得玩家背包空闲空间数量
//-------------------------------------------------------------------------------------
int SC_GetBagFreeSize(lua_State* L)
{
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	ItemMgr& itemMgr = pRole->GetItemMgr();	

	INT nFreeSize = itemMgr.GetBagFreeSize();

	lua_pushinteger(L, nFreeSize);

	return 1;
}

//-------------------------------------------------------------------------------------
// 获得玩家背包和任务背包中物品的数量
//-------------------------------------------------------------------------------------
int SC_GetRoleItemNum(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwTypeID		=	(DWORD)lua_tonumber(L, 2);

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) 
	{
		lua_pushinteger(L, 0);
		return 1;
	}

	ItemMgr& itemMgr = pRole->GetItemMgr();

	INT nTotal = itemMgr.GetBagSameItemCount(dwTypeID) + itemMgr.GetQuestBagSameItemCount(dwTypeID);

	lua_pushinteger(L, nTotal);

	return 1;
}

//-------------------------------------------------------------------------------------
// 获得玩家名字
//-------------------------------------------------------------------------------------
int SC_GetRoleName(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	TCHAR szName[X_SHORT_NAME];
	g_roleMgr.GetRoleNameByID(dwRoleID, szName);

	lua_pushstring(L, IUTIL->UnicodeToUnicode8(szName));

	return 1;
}

//-------------------------------------------------------------------------------------
// 玩家是否有小队
//-------------------------------------------------------------------------------------
int SC_IsRoleHaveTeam(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetTeamID());

	return 1;
}

//-------------------------------------------------------------------------------------
// 获得玩家小队成员ID
//-------------------------------------------------------------------------------------
int SC_GetRoleTeamMemID(lua_State* L)
{
	DWORD	dwTeamID		=	(DWORD)lua_tonumber(L, 1);
	
	const Team *pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
	if(!P_VALID(pTeam))	return 0;

	DWORD dwMemID[MAX_TEAM_NUM];
	memset(dwMemID, 0xFF, sizeof(dwMemID));

	pTeam->ExportAllMemID(dwMemID);

	for (INT i = 0; i < MAX_TEAM_NUM; ++i)
	{
		lua_pushnumber(L, dwMemID[i]);
	}

	return MAX_TEAM_NUM;
}

//-------------------------------------------------------------------------------------
// 设置任务脚本通用数据
//-------------------------------------------------------------------------------------
int SC_SetQuestScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 2);		// 任务ID
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+4);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+5);

		if( nIndex < 0 || nIndex >= ESD_Quest ) return 0;

		pQuest->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 修改任务脚本通用数据
//-------------------------------------------------------------------------------------
int SC_ModQuestScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 2);		// 任务ID
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+4);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+5);

		if( nIndex < 0 || nIndex >= ESD_Quest ) return 0;

		pQuest->ModScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 设置任务脚本通用数据
//-------------------------------------------------------------------------------------
int SC_GetQuestScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 2);		// 任务ID
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+4);		
		
		if( nIndex < 0 || nIndex >= ESD_Quest ) return 0;

		dwValue = pQuest->GetScriptData(nIndex);
		lua_pushnumber(L, dwValue);
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// 玩家是否接取了该任务
//-------------------------------------------------------------------------------------
int SC_IsRoleHaveQuest(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 4);		// 任务ID

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	BOOL bHaveQuest = pRole->IsHaveQuest(u16QuestID);
	lua_pushboolean(L, bHaveQuest);

	return 1;
}

//-------------------------------------------------------------------------------------
// 玩家是否接取了该任务板任务
//-------------------------------------------------------------------------------------
int SC_IsRoleHaveBoardQuest(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 4);		// 任务ID

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	UINT16 u16ID = 0;
	for (int i=QUEST_PANEL_START_TASK_ID; i<=QUEST_PANEL_END_TASK_ID; ++i)
	{
		if (EQBT_Null == pRole->GetQuestBoardType(i))
		{
			continue;
		}

		if (IsRewardBoardQuest(i) && u16QuestID == pRole->GetBoardQuestMgr(EQBT_Reward)->GetBoardIDFromID(i))
		{
			u16ID = i;
			break;
		}

		if (IsYellowListQuest(i) && u16QuestID == pRole->GetBoardQuestMgr(EQBT_YellowList)->GetBoardIDFromID(i))
		{
			u16ID = i;
			break;
		}
	}

	BOOL bHaveQuest = pRole->IsHaveQuest(u16ID);
	lua_pushboolean(L, bHaveQuest);

	return 1;
}

//-------------------------------------------------------------------------------------
// 玩家是否做过某任务
//-------------------------------------------------------------------------------------
int SC_IsRoleCompleteQuest(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 4);		// 任务ID

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	BOOL bCompleteQuest = pRole->IsHaveDoneQuest(u16QuestID);
	lua_pushboolean(L, bCompleteQuest);

	return 1;
}

//-------------------------------------------------------------------------------------
// 增加玩家经验
//-------------------------------------------------------------------------------------
int SC_AddRoleExp(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nValue			=	lua_tointeger(L, 4);
	//INT64	nValue			= pop_64bit_data(L,4,5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->ExpChange((INT64)nValue);

	return 0;
}

//-------------------------------------------------------------------------------------
// 增加玩家金钱
//-------------------------------------------------------------------------------------
int SC_AddRoleSilver(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nValue			=	lua_tointeger(L, 4);
	INT		nCmdID			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( nValue <= 0 ) return 0;

	pRole->GetCurMgr().IncBagSilver(nValue, (ELogCmdID)nCmdID);

	return 0;
}

//-------------------------------------------------------------------------------------
// 减少玩家金钱
//-------------------------------------------------------------------------------------
int SC_DecRoleSilver(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT64	n64Value		=	(INT64)lua_tonumber(L, 4);
	INT		nCmdID			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( n64Value <= 0 ) return 0;

	pRole->GetCurMgr().DecBagSilver(n64Value, (ELogCmdID)nCmdID);

	return 0;
}


//-------------------------------------------------------------------------------------
// 获得玩家金钱
//-------------------------------------------------------------------------------------
int SC_GetRoleSilver(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT64 n64Silver = pRole->GetCurMgr().GetBagSilver();
	lua_pushnumber(L, n64Silver);
	//lua_pushinteger(L, nSilver);
	return 1;
}

//-------------------------------------------------------------------------------------
// 获得玩家元宝数
//-------------------------------------------------------------------------------------
int SC_GetRoleYuanbao(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT32  iYuanbao = pRole->GetCurMgr().GetBagYuanBao();
	lua_pushnumber(L, iYuanbao);

	return 1;
}

//-------------------------------------------------------------------------------------
// 角色使用技能
//-------------------------------------------------------------------------------------
static int SC_RoleUseSkill(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwTargetID		=   (DWORD)lua_tonumber(L, 4);
	DWORD	dwSkillID		=   (DWORD)lua_tonumber(L, 5);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->GetCombatHandler().UseSkill(dwSkillID,dwTargetID, 0);
	
	return 0;
}

//------------------------------------------------------------------------------------
// 取消使用技能
//-------------------------------------------------------------------------------------
static int SC_RoleCancelSkill(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwSkillID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->GetCombatHandler().CancelSkillUse(dwSkillID);

	return 0;
}

//-------------------------------------------------------------------------------------
// 得到师傅ID
//-------------------------------------------------------------------------------------
static int SC_GetMasterID(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetMasterID());
	return 1;
}

//-------------------------------------------------------------------------------------
// 玩家完成师傅资格任务，成为师傅
//-------------------------------------------------------------------------------------
static int SC_RoleBecomeMaster(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// 向DB的master表中插入一行
	tagNDBC_InsertMasterApprenticeData sendDB;
	sendDB.sMasterApprentice.dwID = dwRoleID;
	sendDB.sMasterApprentice.nJingWuPoint = 0;
	sendDB.sMasterApprentice.nQinWuPoint = 0;
	sendDB.sMasterApprentice.sApprentice = tagApprenticeData();
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 玩家升级为师门增加精武点数，同时为自己增加一定的精武点数
//-------------------------------------------------------------------------------------
static int SC_LevelUpAddJingWuPoint(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	const tagMasterApprenticeProto* pProto = g_attRes.GetMasterApprenticeProto(pRole->GetLevel());
	if (!P_VALID(pProto))
	{
		return 0;
	}

	// 如果师傅在线
	Role* pMaster = g_roleMgr.GetRolePtrByID(pRole->GetMasterID());
	if (P_VALID(pMaster))
	{
		pMaster->SetJingWuPoint(pProto->nLevelUpAddPoint + pMaster->GetJingWuPoint());
	}

	// 更新至数据库
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetMasterID();
	sendDB.sOneMAData.eType = EOMAD_JingWu;
	sendDB.sOneMAData.eMode = EMAMM_Mod;		// 增量更新
	*(INT*)(sendDB.sOneMAData.byData) = pProto->nLevelUpAddPoint;

	g_dbSession.Send(&sendDB, sendDB.dwSize);

	// 为自己增加精武点数
	pRole->SetJingWuPoint(pProto->nLevelUpSelfPoint + pRole->GetJingWuPoint());

	// 更新所有同门其师尊可用的精武点数
	for (INT i=0; i!=MAX_APPRENTICE_NUM; ++i)
	{
		DWORD dwMateID = pRole->GetClassMate().dwData[i];
		Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
		if (P_VALID(pMate))
		{
			pMate->GetClassMate().nJingWuPoint += pProto->nLevelUpAddPoint;
		}
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 玩家升级为师门增加徒弟升级积分
//-------------------------------------------------------------------------------------
static int SC_LevelUpAddBonus(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	const tagMasterApprenticeProto* pProto = g_attRes.GetMasterApprenticeProto(pRole->GetLevel());
	if (!P_VALID(pProto))
	{
		return 0;
	}

	// 如果师傅在线
	Role* pMaster = g_roleMgr.GetRolePtrByID(pRole->GetMasterID());
	if (P_VALID(pMaster))
	{
		pMaster->SetLevelUpBonus(pProto->nLevelUpBonus + pMaster->GetLevelUpBonus());
	}

	// 更新至数据库
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetMasterID();
	sendDB.sOneMAData.eType = EOMAD_LevelUpBonus;
	sendDB.sOneMAData.eMode = EMAMM_Mod;		// 增量更新
	*(INT*)(sendDB.sOneMAData.byData) = pProto->nLevelUpBonus;

	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 出师通知
//-------------------------------------------------------------------------------------
static int SC_GraduateNotice(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// 给徒弟发
	tagNS_GraduateNotice sendApprentice;
	sendApprentice.dwRoleID = dwRoleID;
	pRole->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);

	// 如果师傅在线则通知师傅，增加一次师傅可领取的出师奖励次数
	Role* pMaster = g_roleMgr.GetRolePtrByID(pRole->GetMasterID());
	if (P_VALID(pMaster))
	{
		pMaster->SetRewardNum(pMaster->GetRewardNum() + 1);

		tagNS_GraduateNotice sendMaster;
		sendMaster.dwRoleID = dwRoleID;
		pMaster->GetSession()->SendMessage(&sendMaster, sendMaster.dwSize);
	}

	// 更新至数据库
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetMasterID();
	sendDB.sOneMAData.eType = EOMAD_RewardNum;
	sendDB.sOneMAData.eMode = EMAMM_Mod;		// 增量更新
	*(INT*)(sendDB.sOneMAData.byData) = 1;

	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 获得可用出师奖励次数
//-------------------------------------------------------------------------------------
static int SC_GetGraduateRewardNum(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetRewardNum());
	return 1;
}

//-------------------------------------------------------------------------------------
// 改变可用出师奖励次数
//-------------------------------------------------------------------------------------
static int SC_SetGraduateRewardNum(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nNum			=	(INT)lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->SetRewardNum(nNum);

	// 更新至数据库
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetID();
	sendDB.sOneMAData.eType = EOMAD_RewardNum;
	sendDB.sOneMAData.eMode = EMAMM_Mod;		// 增量更新
	*(INT*)(sendDB.sOneMAData.byData) = -1;

	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取师德点数
//-------------------------------------------------------------------------------------
static int SC_GetQinWuPoint(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetQinWuPoint());
	return 1;
}

//-------------------------------------------------------------------------------------
// 获取徒弟升级积分
//-------------------------------------------------------------------------------------
static int SC_GetLevelUpBonus(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetLevelUpBonus());
	return 1;
}

//-------------------------------------------------------------------------------------
// 改变徒弟升级积分
//-------------------------------------------------------------------------------------
static int SC_SetLevelUpBonus(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nPoint			=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->SetLevelUpBonus(nPoint);

	// 更新至数据库
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetID();
	sendDB.sOneMAData.eType = EOMAD_LevelUpBonus;
	sendDB.sOneMAData.eMode = EMAMM_Set;		// 替换更新
	*(INT*)(sendDB.sOneMAData.byData) = nPoint;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}


//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 设置红蓝状态
//-------------------------------------------------------------------------------------
static int SC_SetForceInArena(lua_State* L)
{
	DWORD dwMapID			= (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID		= (DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID			= (DWORD)lua_tonumber(L, 3);
	BOOL  bRedOrBlue		= (BOOL)(DWORD)lua_tonumber(L, 4);
	
	//取房间管理器
	AthleticsRoomMgr* pAthleticsRoomMgr = g_AthleticsSystem.GetRoomMgrPtr();
	if (!P_VALID(pAthleticsRoomMgr))
	{
		return 0;
	}

	tagPvPRoleInRoom* pRoleInRoom = pAthleticsRoomMgr->GetInRoomRole(dwRoleID);
	if (!P_VALID(pRoleInRoom))
	{
		return 0;
	}

	RoomBase* pRoom = pAthleticsRoomMgr->GetPvPRoom(pRoleInRoom->dwSerialID);
	if (!P_VALID(pRoom))
	{
		return 0;
	}

	if (pRoom->GetRoomType() == E_PvP_Room3v3)
	{
		Athletics3V3Room* p3v3Room		= (Athletics3V3Room*)pRoom;
		BYTE ucPos						= p3v3Room->GetRolePos(dwRoleID);
		tagPvPRoleInfo stPvPRoleInfo	= {0};
		tagPvPRoleInfo& PvPRoleInfo		= p3v3Room->GetPosRoleInfo(ucPos);

		stPvPRoleInfo.dwRoleID		= PvPRoleInfo.dwRoleID;
		stPvPRoleInfo.eRoleInPvP	= PvPRoleInfo.eRoleInPvP;
		stPvPRoleInfo.eRoleInSide	= PvPRoleInfo.eRoleInSide;
		stPvPRoleInfo.eState		= PvPRoleInfo.eState;
		stPvPRoleInfo.ucPos			= PvPRoleInfo.ucPos;

		memcpy(&stPvPRoleInfo, &PvPRoleInfo, sizeof(tagPvPRoleInfo));
		
		stPvPRoleInfo.eRoleInSide = bRedOrBlue?ERBS_DefenseSide:ERBS_Attack;

		p3v3Room->SetPosRoleInfo(ucPos, (tagPvPRoleInfo&)stPvPRoleInfo);

		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if(P_VALID(pRole))
		{
			pRole->SetState(bRedOrBlue?ES_PvPDefence:ES_PvPAttack);
			pRole->UnsetState(bRedOrBlue?ES_PvPAttack:ES_PvPDefence);
		}

		//同步敌方血量
	    p3v3Room->SendRemoteAttToPvPRole(dwRoleID,TRUE);
	
	}
	else
	{
		ASSERT(0);
		return 0;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 让指定玩家脱离原来的小队
//-------------------------------------------------------------------------------------
static int SC_LetRoleLeaveTeam(lua_State* L)
{
	DWORD dwTeamID 		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID 		= (DWORD)lua_tonumber(L, 2);



     
	DWORD dwRet  = E_Success;
	//dwRet = ((Team*)pTeam)->LeaveTeam(pRole);
     g_groupMgr.OnMemberLeaveTeam(dwRoleID,NULL);




	return 0;
}

//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 将指定玩家1和指定玩家2组成一个新的小队并且返回小队ID
//-------------------------------------------------------------------------------------
static int SC_CreatANewTeam(lua_State* L)
{
	DWORD dwRoleID1 	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID2 	= (DWORD)lua_tonumber(L, 2);

	Role* pRole1 = g_roleMgr.GetRolePtrByID(dwRoleID1);
	if(!P_VALID(pRole1) || pRole1->GetTeamID() != GT_INVALID)
	{
		// pRole1不在线或者已经有队伍
		return 0;
	}

	Role* pRole2 = g_roleMgr.GetRolePtrByID(dwRoleID2);
	if(!P_VALID(pRole2) || pRole2->GetTeamID() != GT_INVALID)
	{
		// pRole2不在线或者已经有队伍
		return 0;
	}


	//创建玩家小队
	Team * pTeam = g_groupMgr.CreateTeamWithoutNotify(pRole1, pRole2);
	if(!P_VALID(pTeam))	return 0;	
	
	lua_pushnumber(L, pTeam->GetTeamID());

	return 1;
}

//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 将指定玩家放入一个指定的小队中
//-------------------------------------------------------------------------------------
static int SC_LetRoleJoinTeam(lua_State* L)
{
	DWORD dwTeamID = (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 2);

	// 找到玩家小队
	Team* pTeam = const_cast<Team*>(g_groupMgr.GetTeamPtr(dwTeamID));
	if (!P_VALID(pTeam))
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	if (pTeam->AddMemberWithoutNotify(dwRoleID) != E_Success)
	{
		lua_pushboolean(L, 0);
		return 1;
	}	

	lua_pushboolean(L, 1);

	return 1;
}

//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 将玩家传入竞技场
//-------------------------------------------------------------------------------------
static int SC_RoleGotoArena(lua_State* L)
{
	DWORD	dwMapID				=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID		=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwDestMapID			=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwDestInstanceID	=	(DWORD)lua_tonumber(L, 5);
	FLOAT	fx					=	(FLOAT)lua_tonumber(L, 6);
	FLOAT	fy					=	(FLOAT)lua_tonumber(L, 7);
	FLOAT	fz					=	(FLOAT)lua_tonumber(L, 8);

	//取地图、角色
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// 检查副本是否被创建了
	Map* pDestMap = g_mapCreator.GetMap(dwDestMapID, dwDestInstanceID);
	if( !P_VALID(pDestMap) || EMT_System != pDestMap->GetMapType() ) return 0;

	//将玩家传送进竞技场
// 	pInstance->AddRole(pRole);
	pRole->GotoNewMap(dwDestMapID, fx, fy, fz, GT_INVALID, FALSE, dwDestInstanceID);

	return 0;
}

//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 从竞技场大厅和房间列表中删除一个玩家
//-------------------------------------------------------------------------------------
static int SC_DeletRoleFromArenaTable(lua_State* L)
{
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 1);
    

	//Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
    g_AthleticsSystem.GetRoomMgr().OnOffLine(dwRoleID,FALSE);

	//EPvPError ePvPError = E_PvP_Success;
	
	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	// 	tagNS_PvPLeaveHall send;
	// 	send.dwErrorCode = ePvPError;
	// 	pRole->SendMessage(&send,send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 强制复活的脚本接口
//-------------------------------------------------------------------------------------
static int SC_ForcedToRevive(lua_State* L)
{
	DWORD	dwMapID				=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID		=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 3);
	INT		nReviveMP			=	1;
	INT		nReviveHP			=	1;
	DWORD	dwRefMapID			=	dwMapID;
	float	x, y, z;

	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->SetAttValue(ERA_HP, nReviveHP);
	pRole->SetAttValue(ERA_MP, nReviveMP);
	pRole->UnsetState(ES_Dead);
	x = pRole->GetCurPos().x;
	y = pRole->GetCurPos().y;
	z = pRole->GetCurPos().z;

	//复活角色
	pMap->OnRevive(pRole, ERRT_Locus, nReviveHP, nReviveMP, x, y, z, dwRefMapID);
	pRole->SetAttValue(ERA_HP, nReviveHP);
	pRole->SetAttValue(ERA_MP, nReviveMP);

	tagNS_RoleRevive send;
	send.dwRoleID		= dwRoleID;
	send.dwErrorCode = E_Success;
	pRole->SendMessage(&send, send.dwSize);

	if( P_VALID(pRole->GetMap()) )
	{
		pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
	}
    


	pRole->GotoNewMap(dwRefMapID, x, y,z);

	return 0;
}
//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 获得指定房间指定玩家所在的房间类型。如果是新手房返回0，如果是普通返回1，
// 如果是没有找到则返回-1
//-------------------------------------------------------------------------------------
static int SC_GetArenaType(lua_State* L)
{
	DWORD	dwRoomID				=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 2);

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	lua_pushinteger(L, GT_INVALID);
	return 1;
	//--------------------------------------------------------------------------

	//取房间管理器
	AthleticsRoomMgr* pAthleticsRoomMgr = g_AthleticsSystem.GetRoomMgrPtr();
	if (!P_VALID(pAthleticsRoomMgr))
	{
		lua_pushinteger(L, GT_INVALID);
		return 1;
	}

	tagPvPRoleInRoom* pRoleInRoom = pAthleticsRoomMgr->GetInRoomRole(dwRoleID);
	if (!P_VALID(pRoleInRoom))
	{
		lua_pushinteger(L, GT_INVALID);
		return 1;
	}

	RoomBase* pRoom = pAthleticsRoomMgr->GetPvPRoom(pRoleInRoom->dwSerialID);
	if (!P_VALID(pRoom))
	{
		lua_pushinteger(L, GT_INVALID);
		return 1;
	}

	if (pRoom->GetRoomLevel() == E_PvP_Junior)	//初级(新手竞技) 
	{
		lua_pushinteger(L, 0);
		return 1;
	}
	else if (pRoom->GetRoomLevel() == E_PvP_Middle)	//中级(普通竞技 )
	{
		lua_pushinteger(L, 1);
		return 1;
	}
	else
	{
		lua_pushinteger(L, GT_INVALID);
		return 1;
	}
}

//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 获得竞技场房间的限制
// 要求返回三个值：房间的武勋要求，房间的最低等级要求，房间的最高等级要求。
// 如果没有找到该房间，则返回-1。

//-------------------------------------------------------------------------------------
static int SC_GetArenaRoomLimit(lua_State* L)
{
	INT	nRoomType	= lua_tointeger(L, 1);
	INT	nRoomID 	= lua_tointeger(L, 2);

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	lua_pushinteger(L,-1);
	return 1;
	//--------------------------------------------------------------------------

	tagPvPRoom*  pRoomInfo = g_AthleticsSystem.GetRoomMgr().GetHallRoomInfo(nRoomID,(E_PvP_RoomLevel)nRoomType);
    if ( P_VALID( pRoomInfo) )
    {
		INT nWuXunValLimit       = pRoomInfo->dwWuXunLimit ;
        INT nRoomLevelLowerLimit = pRoomInfo->iLevelLowerLimit;
		INT nRoomLevelUpperLimit = pRoomInfo->iLevelUpperLimit;
		lua_pushinteger(L, nWuXunValLimit);
		lua_pushinteger(L, nRoomLevelLowerLimit);
		lua_pushinteger(L, nRoomLevelUpperLimit);
		return 3 ;
    }
	else
	{
         lua_pushinteger(L,-1);
		 return 1;
	}
}

//--------------------------------------------------------------------------
//SM 竞技场的最近修改
//增加该部分功能 
//--------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 把玩家状态传给客户端
// 如果是没有找到则返回-1
//-------------------------------------------------------------------------------------
static int SC_ShowAlivePeopleNameInArena(lua_State* L)
{
	DWORD  dwRoleID        	= lua_tonumber(L, 1);
	DWORD  dwEnermyRoleID 	= lua_tonumber(L, 2); //敌方角色ID
	DWORD  dwStateVal       = lua_tonumber(L, 3);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))
	{
		return 0;
	}
	
	tagNS_PvPSinglePlayerInfo Msg;
	Msg.dwPlayerID = dwEnermyRoleID;
	Msg.eState     = (EPvPPlayerState)dwStateVal;
	pRole->SendMessage(&Msg,Msg.dwSize);
    return 0;

}

//--------------------------------------------------------------------------
//SM 竞技场的最近修改
//增加该部分功能 
//--------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 把玩家状态传给客户端.如果是新手房返回0，如果是普通返回1，
// 如果是没有找到则返回-1
//-------------------------------------------------------------------------------------
static int SC_CloseArenaWaitingRoom(lua_State* L)
{
    g_AthleticsSystem.GetRoomMgr().AthleticsActiveOver();  
	return 0;
}



//--------------------------------------------------------------------------
//SM 竞技场的最近修改
//增加该部分功能 
//--------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 把玩家加入竞技场。如果是新手房返回0，如果是普通返回1，
// 如果是没有找到则返回-1
//-------------------------------------------------------------------------------------
static int SC_JoinArena(lua_State* L)
{
	DWORD dwMapID	    = (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID 	= (DWORD)lua_tonumber(L, 2);
    DWORD dwRoleID      = (DWORD)lua_tonumber(L, 3);



	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))
	{
		return 0;
	}

	if( g_AthleticsSystem.GetPvPState() == 0)
	{
		tagNS_PvPJoinRoom msg;
		msg.dwErrorCode = E_PvP_UnOpened;
		pRole->SendMessage(&msg,msg.dwSize);
		return E_PvP_UnOpened;

	}

	if ( g_AthleticsSystem.GetRoomMgr().IsRoleExist(dwRoleID))
	{
		tagNS_PvPJoinRoom msg;
		msg.dwErrorCode = E_PvP_AlreadyInRoom;
		pRole->SendMessage(&msg,msg.dwSize);
		return E_PvP_AlreadyInRoom;         
	}
    
	DWORD dwRoleLevel = pRole->GetLevel();
    DWORD dwErrorCode = E_Success;

	if( MAX_LEVEL_LIMIT > dwRoleLevel )
	{
		tagNS_PvPJoinRoom msg;
		msg.dwErrorCode = E_PvP_InvalidLevel;
		pRole->SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}
	tagNS_PvPJoinRoom Msg;

	//DWORD dwValiantMerit = pRole->GetAttValue(ERA_WuXun); 
	//DWORD dwValiantMeritLevel = pRole->GetValiantMeritLevel(dwValiantMerit);

	// 	if ( dwRoleLevel < 30)
	// 	{
	// 		Msg.dwErrorCode = E_PvP_InvalidLevel;
	// 		Msg.eRoomLevel  = E_PvP_Junior;
	// 		Msg.nRoomID     = 0;
	// 		pRole->SendMessage(&Msg,Msg.dwSize);
	// 		return 0;
	// 	}

	TMap<INT16, tagPvPRoom*>& pvpMapRoomInfo = g_AthleticsSystem.GetRoomMgr().GetMapPvPRoomInfo(E_PvP_Junior);
	TMap<INT16, tagPvPRoom*>::TMapIterator it = pvpMapRoomInfo.Begin();
	tagPvPRoom* pRoomInfo = NULL;
	BOOL  bFindOut   = FALSE ;
	INT  nRoomID    =  0;
	std::vector<INT16> vecRoomID;
	vecRoomID.clear();
	while ( pvpMapRoomInfo.PeekNext(it,pRoomInfo))
	{
		if ( P_VALID(pRoomInfo) )
		{
			if ( (pRoomInfo->eRoomState != E_PvP_Waiting) || (pRoomInfo->ucCurrentPersonNum == 6) || pRoomInfo->ucCurrentPersonNum == 0 )
			{
				continue;
			}
			if ( (dwRoleLevel  >= pRoomInfo->iLevelLowerLimit) && (dwRoleLevel <= pRoomInfo->iLevelUpperLimit))
			{
				vecRoomID.push_back(pRoomInfo->nRoomID);
			}
		}
	}
	if (vecRoomID.size() > 0)
	{
         bFindOut  = TRUE ;
		 if ( 1 == vecRoomID.size())
		 {
              nRoomID = vecRoomID[0];
		 }
		 else
		 {
			  INT nMaxRoomID = (INT)(vecRoomID.size() - 1);
			  INT nRandomIndex  = IUTIL->RandomInRange(0,nMaxRoomID);
              nRoomID  =  vecRoomID[nRandomIndex];
		 }
	}

	if ( !bFindOut)   // 没有找到就自动创建一个适合玩家的房间
	{

		TCHAR szRoleName[X_SHORT_NAME];
		DWORD dwRoleId = pRole->GetID();
		g_roleMgr.GetRoleNameByID(dwRoleId,szRoleName);

		INT  nLevelUpperLimit =  dwRoleLevel + 8;//dwRoleLevel + (10 - ((dwRoleLevel%10)== 0 ? 10 : (dwRoleLevel%10) ));
		INT  nLevelLowerLimit =  dwRoleLevel - 8;//dwRoleLevel - ((dwRoleLevel%10)== 0 ? 10  : (dwRoleLevel%10) );
		if ( MAX_LEVEL_LIMIT == dwRoleLevel)
		{
			nLevelLowerLimit = MAX_LEVEL_LIMIT ;
			nLevelUpperLimit = MAX_LEVEL_LIMIT + 8 ;
		}

		if ( nLevelLowerLimit < MAX_LEVEL_LIMIT )
		{
			nLevelLowerLimit = MAX_LEVEL_LIMIT;
		}

		tagPvPRoom PvPRoom;
		PvPRoom.nRoomID            = (INT16)g_AthleticsSystem.GetRoomMgr().GetAvailRoomId(E_PvP_Junior);
		PvPRoom.dwManagerID        = dwRoleId;
		PvPRoom.dwWuXunLimit       = 1;
		PvPRoom.eLevel             = E_PvP_Junior;
		PvPRoom.eRoomState         = E_PvP_Waiting;
		PvPRoom.eRoomType          = E_PvP_Room3v3;
		PvPRoom.iLevelLowerLimit   = nLevelLowerLimit;
		PvPRoom.iLevelUpperLimit   = nLevelUpperLimit;
		_tcscpy_s(PvPRoom.szManagerName,X_SHORT_NAME, szRoleName);
		PvPRoom.ucCurrentPersonNum = 1;
		PvPRoom.ucTotalPersonNum   = 6;
	

		if ( pRole->IsDead())
		{
			dwErrorCode = E_PvP_CreateRoomFail;
			Msg.dwErrorCode = dwErrorCode;
			Msg.eRoomLevel  = E_PvP_RoomLevelNull;
			Msg.nRoomID     = 0;
			pRole->SendMessage(&Msg,Msg.dwSize);
			return 0;	
		}

		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
		if ( P_VALID( pRoleScript ) )
		{
			BOOL bCanCreate = FALSE;
			pRoleScript->OnRoleCreateArenaRoom(pRole,E_PvP_Junior,PvPRoom.nRoomID,bCanCreate);
			if ( !bCanCreate)
			{    
				dwErrorCode = E_PvP_CreateRoomFail;
				Msg.dwErrorCode = dwErrorCode;
				Msg.eRoomLevel  = E_PvP_RoomLevelNull;
				Msg.nRoomID     = 0;
				pRole->SendMessage(&Msg,Msg.dwSize);
				return 0;		

			}
		}
		else
		{
			return 0;
		}

		//先加入竞技场大厅 由之前修改来，以免更大的修改

		dwErrorCode =  g_AthleticsSystem.GetRoomMgr().AddToHall(E_PvP_Room3v3,dwRoleID);
		if ( dwErrorCode != E_Success)
		{
			return 0;
		}
		dwErrorCode = g_AthleticsSystem.GetRoomMgr().CreateAthleticsRoom(PvPRoom);

		// 		if (  dwErrorCode == E_PvP_Success)
		// 		{
		// 			g_AthleticsSystem.ModHallRoomNumTemp(1);
		// 	
	    //       }

        if ( E_PvP_Success != dwErrorCode)
        {
			 g_AthleticsSystem.GetRoomMgr().LeaveHall(dwRoleID);
			 return dwErrorCode;
        }
         
        
		DWORD dwTotal    = pRole->GetScriptData(24);
		DWORD dwWinCount = pRole->GetScriptData(25);	
		Msg.dwLocalPlayerRankLevel   = pRole->GetPm(ERT_WuXun) ;
		if ( dwTotal > 0)
		{
			Msg.fLocalPlayerSuccessRate  = (float)(((float)dwWinCount) / ((float)dwTotal));
		}
		else
		{
			Msg.fLocalPlayerSuccessRate  = 0.0f;
		}

		Msg.dwErrorCode = dwErrorCode;
		Msg.eRoomLevel  = PvPRoom.eLevel;
		Msg.nRoomID     = PvPRoom.nRoomID;
		pRole->SendMessage(&Msg,Msg.dwSize);
		return 0;   
	}
	else
	{

		if ( pRole->IsDead())
		{
			Msg.dwErrorCode = E_PvP_CanNotJoinRoom;
			Msg.eRoomLevel  = E_PvP_Junior;
			Msg.nRoomID     = nRoomID;
			pRole->SendMessage(&Msg,Msg.dwSize); 
			return 0;

		}

		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
		if ( P_VALID( pRoleScript ) )
		{
			BOOL bCanJoin = FALSE;
			pRoleScript->OnRoleJoinToArenaRoom(pRole,E_PvP_Junior,nRoomID,bCanJoin);
			if ( !bCanJoin)
			{    
				return 0;
			}
		}
		else
		{
			return 0;
		}
		//先加入竞技场大厅 由之前修改来，以免更大的修改
		dwErrorCode =  g_AthleticsSystem.GetRoomMgr().AddToHall(E_PvP_Room3v3,dwRoleID);

		if ( dwErrorCode != E_Success)
		{
             return dwErrorCode;
		}
		dwErrorCode = g_AthleticsSystem.GetRoomMgr().RoleJoinInRoom(pRole->GetID(),E_PvP_Junior,nRoomID); 

		if ( dwErrorCode != E_Success )
		{
           g_AthleticsSystem.GetRoomMgr().LeaveHall(dwRoleID);
		   return dwErrorCode;
		}


		DWORD dwTotal    = pRole->GetScriptData(24);
		DWORD dwWinCount = pRole->GetScriptData(25);
		Msg.dwLocalPlayerRankLevel   = pRole->GetPm(ERT_WuXun) ;
		if ( dwTotal > 0)
		{
			Msg.fLocalPlayerSuccessRate  = (float)(((float)dwWinCount) / ((float)dwTotal));
		}
		else
		{
			Msg.fLocalPlayerSuccessRate  = 0.0f;
		}
		Msg.dwErrorCode = dwErrorCode;
		Msg.eRoomLevel  = E_PvP_Junior;
		Msg.nRoomID     = nRoomID;
		pRole->SendMessage(&Msg,Msg.dwSize); 
	}

   return 0;
}

//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 对指定玩家显示存活人数界面
//-------------------------------------------------------------------------------------
static int SC_ShowAlivePeopleInArena(lua_State* L)
{
	DWORD	dwRoleID	= (DWORD)lua_tonumber(L, 1);
	INT		nAliveRed	= lua_tointeger(L, 2);
	INT		nAliveBlue	= lua_tointeger(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;


 	tagNS_PvPGetPvPState send;
 	send.dwRedCount = nAliveRed;
 	send.dwGreenCount = nAliveBlue;

 	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 显示杀人数图片
//-------------------------------------------------------------------------------------
static int SC_ShowKillerPictureInArena(lua_State* L)
{
	DWORD	dwRoleID		= (DWORD)lua_tonumber(L, 1);
	INT		nPicture_Num	= lua_tointeger(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))	return 0;

 	tagNS_PvPNotifyKill send;
 	send.dwKillCount = nPicture_Num;

 	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 向指定玩家显示分数界面
//-------------------------------------------------------------------------------------
static int SC_ShowArenaEndFrame(lua_State* L)
{
	DWORD dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	CHAR* pMsgName	=	(CHAR*)lua_tostring(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))	return 0;

 	tagNS_PvPResultInfo send;
	TObjRef<Util> pUtil = "Util"; 	
	MCREATE_MSG(pSend, sizeof(tagNS_PvPResultInfo) + strlen(pMsgName)* sizeof(TCHAR), NS_PvPResultInfo);
	_tcscpy(pSend->pData, pUtil->AnsiToUnicode(pMsgName));
 	pRole->SendMessage(pSend, pSend->dwSize);
	MDEL_MSG(pSend);
	return 0;
}

//-------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// 将指定的几个玩家放回到之前的房间中，并确定房主
//-------------------------------------------------------------------------------------
static int SC_ShowArenaRoomFrame(lua_State* L)
{
	

	DWORD dwSerialID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID1 	= (DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID2 	= (DWORD)lua_tonumber(L, 3);
	DWORD dwRoleID3 	= (DWORD)lua_tonumber(L, 4);
	DWORD dwRoleID4 	= (DWORD)lua_tonumber(L, 5);
	DWORD dwRoleID5 	= (DWORD)lua_tonumber(L, 6);
	DWORD dwRoleID6 	= (DWORD)lua_tonumber(L, 7);
	PvPMgr* pPvPMgr = g_AthleticsSystem.GetPvPMgrPtr();
	if (!P_VALID(pPvPMgr)) return 0;


	dwRoleID1 = (dwRoleID1 < 0)? 0 : dwRoleID1;
	dwRoleID2 = (dwRoleID2 < 0)? 0 : dwRoleID2;
	dwRoleID3 = (dwRoleID3 < 0)? 0 : dwRoleID3;
	dwRoleID4 = (dwRoleID4 < 0)? 0 : dwRoleID4;
	dwRoleID5 = (dwRoleID5 < 0)? 0 : dwRoleID5;
	dwRoleID6 = (dwRoleID6 < 0)? 0 : dwRoleID6;

	
 
 	AthleticsPvP* pAthleticsPvP = pPvPMgr->GetPvP(dwSerialID);
 	if (!P_VALID(pAthleticsPvP)) 
	{
		
		return 0;
	}    


 	RoomBase* pRoom = g_AthleticsSystem.GetRoomMgr().GetPvPRoom(dwSerialID);
	

 	if (!P_VALID(pRoom))
	{
		pAthleticsPvP->ClosePvP(TRUE);
		return 0;
	}   
	Athletics3V3Room* p3v3Room  = NULL;
	if ( pRoom->GetRoomType() == E_PvP_Room3v3)
	{
		p3v3Room = (Athletics3V3Room*)pRoom;
	}
	else
	{
		pAthleticsPvP->ClosePvP(TRUE);
		
		return 0; 
	}


	if( g_AthleticsSystem.GetPvPState() == 0)
	{
		pAthleticsPvP->ClosePvP(TRUE);
		p3v3Room->SetRoomState(E_PvP_RoomStateNull);
		p3v3Room->SetIsPvPRoleEnterPvP(FALSE);
		return 0;

	}
 	p3v3Room->DeleteRoleFromPvP(dwRoleID1, dwRoleID2, dwRoleID3, dwRoleID4, dwRoleID5, dwRoleID6);
    
	//-------------------------------------------------------------------------------------------------------
	// 房间列表的信息修改
	//-------------------------------------------------------------------------------------------------------
	tagPvPRoom*  pRoomlist = g_AthleticsSystem.GetRoomMgr().GetHallRoomInfo((INT16)p3v3Room->GetRoomID(),p3v3Room->GetRoomLevel());
	if ( P_VALID( pRoomlist ) )
	{
		pRoomlist->dwManagerID        = p3v3Room->GetMasterRoleID();
		pRoomlist->eRoomState         = p3v3Room->GetRoomState();
		pRoomlist->dwSerialID         = p3v3Room->GetRoomSerialID();
		_tcscpy(pRoomlist->szManagerName, p3v3Room->GetMasterName());
		pRoomlist->ucCurrentPersonNum = p3v3Room->GetRoomPersonNumber();
	}
	pAthleticsPvP->ClosePvP(TRUE);

	return 0;
}

//-------------------------------------------------------------------------------------
// 给客户端发消息，通知试炼开启
//-------------------------------------------------------------------------------------
static int SC_OpenTrain(lua_State* L)
{
	DWORD dwOwnerID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwTrainID		= (DWORD)lua_tonumber(L, 2);
	
	Role * pOwner = g_roleMgr.GetRolePtrByID(dwOwnerID);
	if (!P_VALID(pOwner))
	{
		return 0;
	}

	ETrainState eTrainState = pOwner->GetTrainState(dwTrainID);
	if (eTrainState == ETS_Close)
	{
		// 给客户端发消息，通知试炼开启
		tagNS_OpenTrain send;
		send.dwTrainID = dwTrainID;
		
		pOwner->SendMessage((LPVOID)(&send), send.dwSize);

		// 保存试炼状态
		pOwner->SaveTrainState(dwTrainID, ETS_Open);
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
// fn ：获取玩家指定氏族的声望值
// Ret：对应氏族声望类型的声望值
//-------------------------------------------------------------------------------------
static int SC_GetClanRepValue(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	ECLanType   eClanType	=	(ECLanType)lua_tointeger(L, 2);

	if (eClanType < ECLT_XuanYuan || eClanType > ECLT_GongGong)
	{
		return 0;
	}

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	ClanData& refClanData = pRole->GetClanData();
	lua_pushinteger(L, refClanData.RepGetVal(eClanType));

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ：获取玩家所拥有的灵兽中灵阶最高的灵兽的灵阶值，
// Ret：最高灵阶值，玩家没有灵兽时返回nil,例如玩家拥有灵阶分别为3，5，4的三只灵兽，则返回值为5
//-------------------------------------------------------------------------------------
static int SC_GetPetStepGrade(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	PetPocket* pPetPocket = pRole->GetPetPocket();
	if (!P_VALID(pPetPocket))
	{
		return 0;
	}

	DWORD	dwPetID[CONST_MAX_PETSOUL_NUM] = {0};
	INT		nPetNum = 0;
	INT		nPetStep = 0;

	// 取玩家所有灵兽ID
	pPetPocket->GetAllPetID(dwPetID, (INT&)nPetNum);
	
	// 玩家没有灵兽时返回nil
	if (nPetNum <= 0)
	{
		return 0;
	}

	// 取玩家所有灵兽最大灵阶值
	for (INT nCnt = 0; nCnt < nPetNum; nCnt++)
	{
		PetSoul* pPetSoul = pPetPocket->GetPetSoul(dwPetID[nCnt]);
		if (!P_VALID(pPetSoul))
		{
			return 0;
		}
		PetAtt&	refPetAtt = pPetSoul->GetPetAtt();
		INT nCurPetStep = refPetAtt.GetStep();
		nPetStep = max(nCurPetStep, nPetStep);
	}
	
	lua_pushinteger(L, nPetStep);

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ：获取玩家行囊中第一个格子中的装备的铭文等级，
// Ret：铭文等级，若第一个格子中无物品或有物品但不是装备，则返回nil
//-------------------------------------------------------------------------------------
static int SC_GetFstBoxWpnPosyTimes(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	// 物品/装备管理器
	ItemMgr& refItemMgr = pRole->GetItemMgr();

	tagItem * pItem = refItemMgr.GetBagItem((INT16)0);
	if (!P_VALID(pItem))
	{
		return 0;
	}

	tagEquipProto *pProto = g_attRes.GetEquipProto(pItem->dwTypeID);
	// 有物品但不是装备
	if(!P_VALID(pProto))
	{
		return 0;
	}
	
	tagEquip* pEquip = (tagEquip*)pItem;
	if (!P_VALID(pEquip))
	{
		return 0;
	}

	lua_pushinteger(L, pEquip->GetWpnPosyTimes());

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ：获取玩家行囊中第一个格子中的装备的镌刻等级，
// Ret：镌刻等级，若第一个格子中无物品或有物品但不是装备，则返回nil
//-------------------------------------------------------------------------------------
static int SC_GetFstBoxWpnEngraveTimes(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	// 物品/装备管理器
	ItemMgr& refItemMgr = pRole->GetItemMgr();

	tagItem * pItem = refItemMgr.GetBagItem((INT16)0);
	if (!P_VALID(pItem))
	{
		return 0;
	}

	tagEquipProto *pProto = g_attRes.GetEquipProto(pItem->dwTypeID);
	// 有物品但不是装备
	if(!P_VALID(pProto))
	{
		return 0;
	}

	tagEquip* pEquip = (tagEquip*)pItem;
	if (!P_VALID(pEquip))
	{
		return 0;
	}

	lua_pushinteger(L, pEquip->GetWpnEngraveTimes());

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ：获取玩家行囊中第一个格子中的装备的镶嵌孔数，
// Ret：装备镶嵌孔数，若第一个格子中无物品或有物品但不是装备，则返回nil
//-------------------------------------------------------------------------------------
static int SC_GetFstBoxWpnEquipHoleCnt(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	// 物品/装备管理器
	ItemMgr& refItemMgr = pRole->GetItemMgr();

	tagItem * pItem = refItemMgr.GetBagItem((INT16)0);
	if (!P_VALID(pItem))
	{
		return 0;
	}

	tagEquipProto *pProto = g_attRes.GetEquipProto(pItem->dwTypeID);
	// 有物品但不是装备
	if(!P_VALID(pProto))
	{
		return 0;
	}

	tagEquip* pEquip = (tagEquip*)pItem;
	if (!P_VALID(pEquip))
	{
		return 0;
	}

	lua_pushinteger(L, pEquip->GetWpnEquipHoleCnt());

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ：获取玩家的帮派功勋值
// Ret：玩家的帮派功勋值
//-------------------------------------------------------------------------------------
static int SC_GetGuildExploit(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	// 得到帮派
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return 0;
	}

	// 得到帮派成员
	tagGuildMember* pMember = pGuild->GetMember(dwRoleID);
	if (!P_VALID(pMember))
	{
		return 0;
	}

	lua_pushnumber(L, pMember->nExploit);

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ：获取玩家已完成的试炼数量
// Ret：已完成试炼数量
//-------------------------------------------------------------------------------------
static int SC_GetPlayerTrainCnt(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	lua_pushinteger(L, pRole->GetPassTrainCnt());

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ：判断玩家是否有某个称号
// Ret：玩家是否有某个称号
//-------------------------------------------------------------------------------------
static int SC_IsRoleHaveTitle(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16		u16TitleID	=	(UINT16)lua_tointeger(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	TitleMgr* pTitleMgr = pRole->GetTitleMgr();
	if (!P_VALID(pTitleMgr))
	{
		return 0;
	}

	int nb = pTitleMgr->IsTitleObtained(u16TitleID) ? 1 : 0; 
	lua_pushboolean(L, nb);

	return 1;
}


//-------------------------------------------------------------------------------------
// 改变师德点数
//-------------------------------------------------------------------------------------
static int SC_SetQinWuPoint(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nNum			=	(INT)lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->SetQinWuPoint(nNum);

	// 徒弟更新师尊信息
	pRole->GetApprenticeData().ResetIterator();
	DWORD dwApprenticeID = GT_INVALID;
	while (pRole->GetApprenticeData().PeekNext(dwApprenticeID))
	{
		Role* pApprentice = g_roleMgr.GetRolePtrByID(dwApprenticeID);
		if (P_VALID(pApprentice))
		{
			pApprentice->GetClassMate().nQinWuPoint = nNum;
		}
	}

	// 更新至数据库
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetID();
	sendDB.sOneMAData.eType = EOMAD_QinWu;
	sendDB.sOneMAData.eMode = EMAMM_Set;		// 替换更新
	*(INT*)(sendDB.sOneMAData.byData) = nNum;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 得到击杀玩家经验
//-------------------------------------------------------------------------------------
static int SC_GetKillObjExp(lua_State* L)
{
	INT		nLevel		=	(INT)lua_tointeger(L, 1);

	const tagKillerExp*	pProto = g_attRes.GetKillObjExpProto((INT16)nLevel);
	if (!P_VALID(pProto))
		return 0;

	lua_pushnumber(L, pProto->dwExp);
	return 1;
}

//-------------------------------------------------------------------------------------
// 守护神倒计时通知客户端
//-------------------------------------------------------------------------------------
static int SC_EudemonSummonCountDown(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT32	nCountDown		=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_BeginSummon send;
	send.nCountDown = nCountDown;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 修改活动排行数据
//-------------------------------------------------------------------------------------
static int SC_ModRankData(lua_State* L)
{ 
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 1);
	INT16	n16DataType	        =	(INT16)lua_tonumber(L, 2);
	INT	    nModValue		    =	(INT)lua_tonumber(L, 3);
	if ( 0 == dwRoleID || !GT_VALID(dwRoleID) || 0 == nModValue )
	{
        return 0;
	}
	g_WorldExpo.ModRankingsData(dwRoleID,(E_ActiveDataType)(n16DataType-1),nModValue);
    
    return 0;
}

//-------------------------------------------------------------------------------------
// 获得活动排行数据
//-------------------------------------------------------------------------------------
static int SC_GetRoleRankData(lua_State* L)
{
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 1);
	INT16	n16DataType      	=	(INT16)lua_tonumber(L, 2);
	if ( 0 == dwRoleID || !GT_VALID(dwRoleID) )
	{
		return 0;
	}
	INT32   nValue              =  g_WorldExpo.GetRoleRankingsData(dwRoleID,(E_ActiveDataType)(n16DataType-1));
    lua_pushnumber(L, nValue);
    return 1;
}

//-------------------------------------------------------------------------------------
// 获得显示排行数据
//-------------------------------------------------------------------------------------
static int SC_GetDisplayRank(lua_State* L)
{
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 1);
	INT16	n16DataType     	=	(INT16)lua_tonumber(L, 2);
	INT  	nLowRankPos  		=	(INT)lua_tonumber(L, 3);
	INT 	nHighRankPos		=	(INT)lua_tonumber(L, 4);
	if ( 0 == dwRoleID || !GT_VALID(dwRoleID) )
	{
		return 0;
	}
    g_WorldExpo.GetRoleDisplayRankings(dwRoleID,(E_ActiveDataType)(n16DataType-1),nLowRankPos,nHighRankPos);


    return 0;
}

static int SC_CancelRoleStallState(lua_State* L)
{
     DWORD   dwMapId           = (DWORD)lua_tonumber(L,1);
	 DWORD   dwMapInstanceId   = (DWORD)lua_tonumber(L,2);
	 DWORD   dwRoleId          = (DWORD)lua_tonumber(L,3);
	 DWORD   dwError   =  0 ;
     Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleId);
	 if ( !P_VALID(pRole) )
	 {
		 dwError = 1;
	 }
	 else
	 {
		 if ( !pRole->GetMap())
		 {
             dwError  = 1;
		 }
		 else
		 {
	    	  dwError =  pRole->SendCloseStall();
		 }
	 }
	 lua_pushnumber(L,dwError);
	 return 1;
}


//-------------------------------------------------------------------------------------
// 设置精力值cd时间
//-------------------------------------------------------------------------------------
static int SC_RoleFixSpirit(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwCDTime		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->ModFixSpiriteCDTime(dwCDTime);

	return 0;
}

//-------------------------------------------------------------------------------------
// 得到玩家性别
//-------------------------------------------------------------------------------------
static int SC_GetRoleSex(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	lua_pushinteger(L, pRole->GetSex());
	return 1;
}

//-------------------------------------------------------------------------------------
// 得到玩家服装编号
//-------------------------------------------------------------------------------------
int SC_GetRoldDressid(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	lua_pushinteger(L, pRole->GetAvatar()->wDressMdlID);
	return 1;
}

//-------------------------------------------------------------------------------------
// 得到玩家帮派ID
//-------------------------------------------------------------------------------------
int SC_GetRoleGuildID(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	lua_pushnumber(L, pRole->GetGuildID());

	return 1;
}

//-------------------------------------------------------------------------------------
// 获得玩家可激发龙魂数量
//-------------------------------------------------------------------------------------
int SC_GetActiveLongHunNum(lua_State* L)
{
	lua_pushinteger(L, 0);
	return 1;

	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	// 取得可激发龙魂数量
	INT nActiveNum = pRole->GetLongHun().GetActiveNum();

	lua_pushinteger(L, nActiveNum);

	return 1;
}

//-------------------------------------------------------------------------------------
// 龙魂能力激发
//-------------------------------------------------------------------------------------
int SC_CastActiveLongHun(lua_State* L)
{
	return 0;
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwSkillID	= (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	// 取得玩家技能
	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if (!P_VALID(pSkill))
	{
		return 0;
	}

	// 随机取出可爆发龙魂ID
	DWORD dwLongHunID = pRole->GetLongHun().GetRandomLongHunID();
	if (!GT_VALID(dwLongHunID))
	{
		return 0;
	}

	// 取得龙魂属性
	const tagLongHunProto* pProto = g_attRes.GetLongHunProto(dwLongHunID);
	if (!P_VALID(pProto))
	{
		return 0;
	}

	if (pProto->eTypeEx == ELHTE_PowerGather)
	{
		return 0;
	}

	if (GT_VALID(pProto->dwBuffID1))
	{
		const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(pProto->dwBuffID1);
		if (P_VALID(pBuffProto))
		{
			pRole->TryAddBuff(pRole, pBuffProto, NULL, pSkill, NULL);
		}
	}
	if (GT_VALID(pProto->dwBuffID2))
	{
		const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(pProto->dwBuffID2);
		if (P_VALID(pBuffProto))
		{
			pRole->TryAddBuff(pRole, pBuffProto, NULL, pSkill, NULL);
		}	
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 龙魂激增
//-------------------------------------------------------------------------------------
int SC_ProlifeLongHun(lua_State* L)
{
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwSkillID	= (DWORD)lua_tonumber(L, 2);
	BOOL bProlife	= (BOOL)lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	// 取得玩家技能
	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if (!P_VALID(pSkill))
	{
		return 0;
	}

	// 取得当前任务身上已经爆发的龙魂buff
	Buff* pBuff = pRole->GetRelativeSkillBuff(dwSkillID);
	if (!P_VALID(pBuff))
	{
		return 0;
	}

	// 取得下一级Buff属性
	const tagBuffProto* pProto = g_attRes.GetBuffProto(pBuff->GetTypeID()+1);
	if (!P_VALID(pProto))
	{
		return 0;
	}
	
	if (bProlife)
	{
		pRole->TryAddBuff(pRole, pProto, NULL, pSkill, NULL);
	}
	lua_pushboolean(L, true);

	return 1;
}

//-------------------------------------------------------------------------------------
// 判断玩家是否在摆摊
//-------------------------------------------------------------------------------------
int SC_IsRoleOnStall(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	int nOnStall = FALSE;

	// 判断是否已经处于摆摊或摆摊设置状态
	if(pRole->IsInRoleStateAny(ERS_Stall | ERS_StallSet))
		nOnStall = TRUE;
	else
		nOnStall = FALSE;

	lua_pushboolean(L, nOnStall);

	return 1;
}

//-------------------------------------------------------------------------------------
// 设置玩家状态
//-------------------------------------------------------------------------------------
int SC_SetRoleState(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	INT			nState		=	lua_tointeger(L, 2);
	BOOL		bSendMsg	=	(BOOL)lua_toboolean(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	if( nState <= 0 || nState >= ERS_End)	return 0;

	pRole->SetRoleState((ERoleState)nState, bSendMsg);

	if(ERS_Safeguard == nState)
	{
		// 返回给客户端
		tagNS_SafeGuard send;
		send.bOpen = TRUE;
		send.dwErrorCode = E_Success;
		pRole->SendMessage(&send, send.dwSize);
	}

	return 0;
}

int SC_UnSetRoleState(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	INT			nState		=	lua_tointeger(L, 2);
	BOOL		bSendMsg	=	(BOOL)lua_toboolean(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	if( nState <= 0 || nState >= ERS_End)	return 0;

	pRole->UnsetRoleState((ERoleState)nState, bSendMsg);

	if(ERS_Safeguard == nState)
	{
		// 返回给客户端
		tagNS_SafeGuard send;
		send.bOpen = FALSE;
		send.dwErrorCode = E_Success;
		pRole->SendMessage(&send, send.dwSize);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 能否开启宝箱
//-------------------------------------------------------------------------------------
int SC_CanOpenChest(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwChestID		=	(DWORD)lua_tonumber(L, 4);
	DWORD dwKeyID		=	(DWORD)lua_tonumber(L, 5);

// 	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
// 	if(!P_VALID(pMap))	return 0;
// 
// 	Role* pRole = pMap->FindRole(dwRoleID);
// 	if(!P_VALID(pRole))	return 0;
// 
// 	tagItemProto* pItemProto = g_attRes.GetItemProto(dwChestID);
// 	if (!P_VALID(pItemProto))
// 	{
// 		return 0;
// 	}
	// 宝箱开启去掉密码
	lua_pushboolean(L, true);

	return 1;
}

//-------------------------------------------------------------------------------------
// 向客户端发送消息
//-------------------------------------------------------------------------------------
int SC_SendChestMsg(lua_State* L)
{
	DWORD dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID		=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID			=	(DWORD)lua_tonumber(L, 3);
	DWORD dwChestID			=	(DWORD)lua_tonumber(L, 4);
	const string strMsgName =	lua_tostring(L, 5);
	BOOL bOpened			=	(BOOL)lua_toboolean(L, 6);
	BOOL bDestroy			=	(BOOL)lua_toboolean(L, 7);
	DWORD dwItemID			=	(DWORD)lua_tonumber(L, 8);
	INT nItemNum			=	lua_tointeger(L, 9);
	DWORD dwErrorCode		=	(DWORD)lua_tonumber(L, 10);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// 发送消息给客户端
	g_TreasureChestMgr.SendMsg2Client(pRole, dwChestID, strMsgName, bOpened, bDestroy, dwItemID, nItemNum, dwErrorCode);

	return 0;
}

//-------------------------------------------------------------------------------------
// 使世界宝箱开启计数和角色宝箱开启计数加1
//-------------------------------------------------------------------------------------
int SC_IncTreasureSum(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	g_worldSession.IncTreasureSum();
	pRole->IncTreasureSum();
	
	return 0;
}

//-------------------------------------------------------------------------------------
// 获得世界宝箱开启计数和角色宝箱开启计数
//-------------------------------------------------------------------------------------
int SC_GetTreasureSum(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushinteger(L, pRole->GetTreasureSum());
	lua_pushinteger(L, g_worldSession.GetTreasureSum());

	return 2;
}

//-------------------------------------------------------------------------------------
// 得到宝箱内物品
//-------------------------------------------------------------------------------------
int SC_GetChestItem(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwChestID		=	(DWORD)lua_tonumber(L, 4);
	ERateType eRate		=	(ERateType)lua_tointeger(L, 5);
	FLOAT fRand			=	(FLOAT)lua_tonumber(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagChestItem* pItem = g_TreasureChestMgr.GetRandomItem(dwChestID, eRate, fRand);
	if (!P_VALID(pItem))	return 0;

	DWORD dwItemTypeID = pItem->dwTypeID;	// 开出物品typeid
	INT nNum = pItem->nNum;					// 开出物品数量
	lua_pushnumber(L, dwItemTypeID);
	lua_pushinteger(L, nNum);

	return 2;
}

//-------------------------------------------------------------------------------------
// 物品是否需要广播
//-------------------------------------------------------------------------------------
int SC_ItemNeedBroadcast(lua_State* L)
{
	DWORD dwTypeID	=	(DWORD)lua_tonumber(L, 1);

	// 如果是装备
	if (MIsEquipment(dwTypeID))
	{
		// 找到装备
		tagEquipProto* pProto = g_attRes.GetEquipProto(dwTypeID);
		if (!P_VALID(pProto))	return 0;

		lua_pushnumber(L, pProto->bNeedBroadcast);
		return 1;
	}
	
	// 是物品
	else
	{
		// 找到物品
		tagItemProto* pProto = g_attRes.GetItemProto(dwTypeID);
		if (!P_VALID(pProto))	return 0;

		lua_pushnumber(L, pProto->bNeedBroadcast);
		return 1;
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
// 得到宝箱内物品
//-------------------------------------------------------------------------------------
int SC_StopMount(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	pRole->StopMount();

	return 0;
}

//-------------------------------------------------------------------------------------
// 通知队友进入副本
//-------------------------------------------------------------------------------------
int SC_InstanceNotify(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	BOOL	bNotify		=	(BOOL)lua_toboolean(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	if(bNotify == FALSE)	return 0;

	// 找到玩家小队
	const Team* pTeam = g_groupMgr.GetTeamPtr(pRole->GetTeamID());
	if( !P_VALID(pTeam) ) return 0;

	// 检查队伍是否创建了副本
	Map* pMap = g_mapCreator.GetMap(pTeam->GetOwnInstanceMapID(), pTeam->GetOwnInstanceID());
	if( !P_VALID(pMap) || EMT_Instance != pMap->GetMapType() ) return 0;

	MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
	if( !P_VALID(pInstance) ) return 0;

	const tagInstance* pInstanceProto = pInstance->GetInstanceProto();
	if( !P_VALID(pInstanceProto) ) return 0;	

	// 副本是否允许通知队友
	if( !pInstanceProto->bNoticeTeamate ) return 0;

	// 通知队友
	tagNS_InstanceAgree	 send;
	send.dwInviterID	=	pRole->GetID();
	send.dwMapID		=	pInstance->GetMapID();
	g_groupMgr.SendTeamInstanceNotify(pRole, pRole->GetTeamID(), &send, send.dwSize);

	return 0;
}
//-------------------------------------------------------------------------------------
// 是否已经结婚
//-------------------------------------------------------------------------------------
static int SC_IsRoleMarried(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);


	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	lua_pushboolean(L, pRole->IsRoleMarried());

	return 1;
}
//-------------------------------------------------------------------------------------
// 是否互为夫妻
//-------------------------------------------------------------------------------------
static int SC_IsCouple(lua_State* L)
{
	DWORD	dwRoleID1	=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwRoleID2	=	(DWORD)lua_tonumber(L, 2);

	Role* pRole1 = g_roleMgr.GetRolePtrByID(dwRoleID1);
	if( !P_VALID(pRole1) )	return 0;

	if(pRole1->GetLoverID() == dwRoleID2)
	{
		lua_pushboolean(L, 1);
	}
	else
	{
		lua_pushboolean(L, 0);
	}

	return 1;
}
//-------------------------------------------------------------------------------------
// 是否已经举行过婚礼
//-------------------------------------------------------------------------------------
static int SC_IsRoleWedded(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	lua_pushboolean(L, pRole->IsRoleWedded());

	return 1;
}
//-------------------------------------------------------------------------------------
// 得到友好度(亲密度)
//-------------------------------------------------------------------------------------
static int SC_GetFriendShipValue(lua_State* L)
{
	DWORD	dwRoleID1	=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwRoleID2	=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID1);
	if( !P_VALID(pRole) )	return 0;

	tagFriend* pFriRole = pRole->GetFriendPtr(dwRoleID2);
	if( !P_VALID(pFriRole) )
	{
		lua_pushinteger(L, GT_INVALID);
	}
	else
	{
		lua_pushinteger(L, pFriRole->dwFriVal);
	}
	return 1;
}
//-------------------------------------------------------------------------------------
// 增加友好度
//-------------------------------------------------------------------------------------
static int SC_AddFriendShipValue(lua_State* L)
{
	DWORD	dwRoleID1	=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwRoleID2	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwAddFriVal	=	(DWORD)lua_tonumber(L, 3);

	if(dwAddFriVal <= 0)
		return 0;

	Role* pRole1 = g_roleMgr.GetRolePtrByID(dwRoleID1);
	if( !P_VALID(pRole1) )	return 0;

	Role* pRole2 = g_roleMgr.GetRolePtrByID(dwRoleID2);
	if( !P_VALID(pRole2) )	return 0;

	//得到role1的朋友
	tagFriend* pRole1Fri = pRole1->GetFriendPtr(dwRoleID2);

	//得到role2的朋友
	tagFriend* pRole2Fri = pRole2->GetFriendPtr(dwRoleID1);


	BOOL bInsert = TRUE;
	DWORD dwFriVal = 0;
	if(P_VALID(pRole1Fri))
	{
		if(pRole1Fri->dwFriVal > 0)
			bInsert = FALSE;

		dwFriVal = pRole1Fri->dwFriVal += dwAddFriVal;

		tagNS_UpdateFriValue send;
		send.dwRoleID = pRole1Fri->dwFriendID;
		send.nFriVal = dwFriVal;
		pRole1->SendMessage(&send, send.dwSize);

		//更新称号	
		if(pRole1->GetLoverID() == pRole1Fri->dwFriendID)
			pRole1->GetTitleMgr()->SigEvent(ETE_MARRY_JOIN,  GT_INVALID, dwFriVal);

	}

	if(P_VALID(pRole2Fri))
	{
		if(pRole2Fri->dwFriVal > 0)
			bInsert = FALSE;

		dwFriVal = pRole2Fri->dwFriVal += dwAddFriVal;

		tagNS_UpdateFriValue send;
		send.dwRoleID = pRole2Fri->dwFriendID;
		send.nFriVal = dwFriVal;
		pRole2->SendMessage(&send, send.dwSize);

		
		//更新称号	
		if(pRole2->GetLoverID() == pRole2Fri->dwFriendID)
			pRole2->GetTitleMgr()->SigEvent(ETE_MARRY_JOIN,  GT_INVALID, dwFriVal);
	}

	if(bInsert)
	{
		tagNDBC_InsertFriVal sendDB;
		sendDB.dwRoleID = (dwRoleID1 < dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.dwFriendID = (dwRoleID1 > dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.nFriVal = dwFriVal;
		g_dbSession.Send(&sendDB, sendDB.dwSize);
	}
	else{
		tagNDBC_UpdateFriVal sendDB;
		sendDB.dwRoleID = (dwRoleID1 < dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.dwFriendID = (dwRoleID1 > dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.nFriVal = dwFriVal;
		g_dbSession.Send(&sendDB, sendDB.dwSize);
	}

	return 0;
}
//-------------------------------------------------------------------------------------
// 增加称号
//-------------------------------------------------------------------------------------
int SC_SigTitleEvent(lua_State* L)
{
	DWORD			dwRoleID	= (DWORD)lua_tonumber(L, 1);
	DWORD			dwArg1		= (DWORD)lua_tonumber(L, 2);


	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	BOOL bHas = pRole->GetTitleMgr()->SigEvent(ETE_SCRIPT, dwArg1, GT_INVALID);

	lua_pushboolean(L, bHas);

	return 1;
}
//-------------------------------------------------------------------------------------
// 删除称号
//-------------------------------------------------------------------------------------
int SC_SigRemoveTitleEvent(lua_State* L)
{
	DWORD			dwRoleID	= (DWORD)lua_tonumber(L, 1);
	DWORD			titleEvent	= (DWORD)lua_tonumber(L, 2);
	DWORD			dwArg1		= (DWORD)lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	pRole->GetTitleMgr()->SigRemoveEvent((EnumTitleEvent)titleEvent, dwArg1);

	return 1;
}
//-------------------------------------------------------------------------------------
// 改变技能熟练度
//-------------------------------------------------------------------------------------
int SC_ChangeSkillExp(lua_State* L)
{
	DWORD			dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	DWORD			dwSkillID	=	(DWORD)lua_tonumber(L, 2);
	DWORD			dwSkillExp	=	(DWORD)lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if( !P_VALID(pSkill) )
	{
		return 0;
	}

	pRole->ChangeSkillExp(pSkill, dwSkillExp);

	return 0;
}

//-------------------------------------------------------------------------------------
// 玩家是否有某buff
//-------------------------------------------------------------------------------------
int SC_IsRoleHaveBuff(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwBuffID		=	(DWORD)lua_tonumber(L, 4);
	BOOL	bRet			=	FALSE;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	bRet = pRole->IsHaveBuff(dwBuffID);
	lua_pushboolean(L, bRet);

	return 1;
}

//-------------------------------------------------------------------------------------
// 任务设置玩家复活点
//-------------------------------------------------------------------------------------
int SC_QuestSetRebornMap(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16QuestID		=	(UINT16)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_BindRebornMap send;
	send.dwErrorCode = pRole->QuestSetRebornMap(u16QuestID, send.dwBornMapID, send.vBornPos);
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 首次上线将物品同步给客户端
//-------------------------------------------------------------------------------------
int SC_VirginOnlineSendClient(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	INT64	n64Serial1	=	pop_64bit_data(L, 2, 3);
	INT64	n64Serial2	=	pop_64bit_data(L, 4, 5);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	// 给客户端发送初次上线消息
	tagNS_VirginOnline send;
	send.n64Serial1 = n64Serial1;
	send.n64Serial2 = n64Serial2;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 钓鱼成功消息发送给客户端
//-------------------------------------------------------------------------------------
int SC_FishingSendClient(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_Fishing send;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 设置玩家脚本通用数据
//-------------------------------------------------------------------------------------
int SC_SetRoleScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+3);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+1+3);

		if( nIndex < 0 || nIndex >= ESD_Role ) return 0;

		pRole->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 修改玩家脚本通用数据
//-------------------------------------------------------------------------------------
int SC_ModRoleScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+3);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+3);

		if( nIndex < 0 || nIndex >= ESD_Role ) return 0;

		pRole->ModScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 获得玩家脚本通用数据
//-------------------------------------------------------------------------------------
int SC_GetRoleScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+3);

		if( nIndex < 0 || nIndex >= ESD_Role ) return 0;

		dwValue = pRole->GetScriptData(nIndex);
		lua_pushnumber(L, dwValue);
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// 获得玩家脚本通用数据
//-------------------------------------------------------------------------------------
int SC_CanMount(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;
	
	INT nRt = pRole->GetPetPocket()->CanRidePet();

	lua_pushinteger(L, nRt);

	return 1;
}

//-------------------------------------------------------------------------------------
// 增加玩家赠点数量
//-------------------------------------------------------------------------------------
int SC_AddRoleExVolume(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nValue			=	lua_tointeger(L, 4);
	INT		nCmdID			=	lua_tointeger(L, 5);



	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->GetCurMgr().IncExchangeVolume(nValue, (ELogCmdID)nCmdID);

	return 0;
}


//-------------------------------------------------------------------------------------
// 判断人物状态
//-------------------------------------------------------------------------------------
int SC_IsRoleInStatus(lua_State* L)
{
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwStatus	= (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}
	
	lua_pushboolean(L, pRole->IsInRoleStateAll(dwStatus));

	return 1;
}

//-------------------------------------------------------------------------------------
// 创建脚本通用消息
//-------------------------------------------------------------------------------------
int SC_BeginMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	lua_pushnumber(L, dwMsgInfoID);

	return 1;
}

//-------------------------------------------------------------------------------------
// 向消息中添加相关事件数据
//-------------------------------------------------------------------------------------
int SC_AddMsgEvent(lua_State* L)
{
	DWORD			dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);
	EMsgUnitType	eMsgUnitType	=	(EMsgUnitType)lua_tointeger(L, 2);
	LPVOID			pData			=	NULL;

	if(EMUT_RoleName == eMsgUnitType)
	{
		LPCTSTR pData = IUTIL->AnsiToUnicode(lua_tostring(L, 3));
		g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, eMsgUnitType, (LPVOID)pData);
	}
	else
	{
		DWORD dwData = (DWORD)lua_tonumber(L, 3);
		g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, eMsgUnitType, &dwData);
	}

	return 0;
}


//-------------------------------------------------------------------------------------
// 发送脚本通用消息给玩家
//-------------------------------------------------------------------------------------
int SC_DispatchRoleMsgEvent(lua_State* L)
{	
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 2);

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	g_MsgInfoMgr.DispatchRoleMsgEvent(dwMsgInfoID, pRole);
	return 0;
}

//-------------------------------------------------------------------------------------
// 给服务器所有地图内的玩家发送脚本通用消息
//-------------------------------------------------------------------------------------
int SC_DispatchWorldMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);

	g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);

	return 0;
}

int SC_DispatchMapMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 3);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	g_MsgInfoMgr.DispatchMapMsgEvent(dwMsgInfoID, pMap);

	return 0;
}


//-------------------------------------------------------------------------------------
// 给该帮派的成员发脚本消息
//-------------------------------------------------------------------------------------
int SC_DispatchGuildMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwGuildID		=	(DWORD)lua_tonumber(L, 2);

	Guild *pGuild = g_guildMgr.GetGuild(dwGuildID);

	if(!P_VALID(pGuild))	return 0;

	g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, pGuild);

	return 0;

}

//-------------------------------------------------------------------------------------
// 给该战场参战同盟公会的成员发脚本消息
//-------------------------------------------------------------------------------------
int SC_DispatchAllianceGuildMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwAllianceType	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwGodID			=	(DWORD)lua_tonumber(L, 3);

	if (dwGodID >= EGM_MAX || dwGodID < EGM_NH)
		return 0;

	if (dwAllianceType == 1 || dwAllianceType == 2)
	{
		CGodMiracleMgr::GetInst()->GetGodMiracle(static_cast<EGodMiracle>(dwGodID))->SendAllianceGuildMsg(dwMsgInfoID, dwAllianceType);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 播放指定ID的服务器广播
//-------------------------------------------------------------------------------------
int SC_DispatchBroadcast(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwGuildID		=	(DWORD)lua_tonumber(L, 4);

	// 若该参数为-1，则该条广播为全服广播
	if (dwMapID == -1)
	{
		g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);
	}
	else if (dwMapID == -2)
	{
		// dwMapID == -2帮派广播,帮派不存在，广播不播放。
		Guild *pGuild = g_guildMgr.GetGuild(dwGuildID);
		if(!P_VALID(pGuild))	return 0;

		g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, pGuild);
	}
	else
	{
		// 若该参数不为-1时，则播放CRC值与该参数相等的场景的场景广播;若控制广播范围的参数不为-1，且找不到与该值对应的地图名CRC时，则该条广播不会播放
		Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
		if(!P_VALID(pMap))	return 0;

		g_MsgInfoMgr.DispatchMapMsgEvent(dwMsgInfoID, pMap);
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 职业系统接口
//-------------------------------------------------------------------------------------
int SC_IsRoleInaugural(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwVocation		=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	INT nVocation = (INT)(pRole->GetClass());
	DWORD dwTmpStr = 1 << (nVocation-1);
	if( dwTmpStr & dwVocation )
		lua_pushnumber(L, 1);
	else
		lua_pushnumber(L, 0);

	return 1;
}

int SC_ChangeClass(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);
	DWORD dwNPCID = (DWORD)lua_tonumber(L, 2);
	INT nVocation = (INT)lua_tonumber(L, 3);
	
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	pRole->ChangeVocation(dwNPCID, (EClassType)nVocation);
	
	return 0;
}

//-------------------------------------------------------------------------------------
// 固定活动接口
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// 初始化活动时间事件的触发时间
//-------------------------------------------------------------------------------------
int SC_InitEventTime(lua_State* L)
{
	const CHAR* szTableName		=   lua_tostring(L, 1);			// 存放时间点table的名字
	INT			nNum			=   lua_tointeger(L, 2);		// 时间点的个数
	DWORD		dwID			=   (DWORD)lua_tonumber(L, 3);			// 活动ID

	if(nNum <= 0)	return 0;

	CActivity *pActivity = g_activityMgr.GetActivity(dwID);
	if(!P_VALID(pActivity))	return 0;

	for(INT i = 1; i <= nNum; ++i)
	{
		lua_getglobal(L, szTableName);
		lua_pushnumber(L, i);
		lua_gettable(L, -2);
		DWORD	dwTime = lua_tointeger(L, -1);

		pActivity->AddEventTime(dwTime);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 活动是否开始
//-------------------------------------------------------------------------------------
int SC_GetActIsStart(lua_State* L)
{
	BOOL		bStart		=	FALSE;
	DWORD		dwID		=	(DWORD)lua_tonumber(L, 1);			//活动ID

	CActivity* pActivity = g_activityMgr.GetActivity(dwID);
	if( !P_VALID(pActivity) )	return 0;

	if( pActivity->IsStart() ) bStart = TRUE;

	lua_pushboolean(L, bStart);
	return 1;
}

//-------------------------------------------------------------------------------------
// 活动是否开始
//-------------------------------------------------------------------------------------
int SC_AddAllRoleBuff(lua_State* L)
{
	DWORD	dwBuffTypeID		=	(DWORD)lua_tonumber(L, 1);
	tagAllRoleBuff 	AllRoleBuff;
	AllRoleBuff.dwBuffTypeID = dwBuffTypeID;
	g_groupMgr.AddEvent(GT_INVALID, EVT_AddAllRoleBuff, sizeof(tagAllRoleBuff), &AllRoleBuff);

	return 0;
}

//-------------------------------------------------------------------------------------
// 设置固定活动脚本通用数据
//-------------------------------------------------------------------------------------
int SC_SetActScriptData(lua_State* L)
{
	DWORD	dwActID			=	(DWORD)lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到活动
	CActivity* pActivity = g_activityMgr.GetActivity(dwActID);
	if( !P_VALID(pActivity) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+3);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+1+3);

		pActivity->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 获得固定活动脚本通用数据
//-------------------------------------------------------------------------------------
int SC_GetActScriptData(lua_State* L)
{
	DWORD	dwActID			=	(DWORD)lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到活动
	CActivity* pActivity = g_activityMgr.GetActivity(dwActID);
	if( !P_VALID(pActivity) ) return 0;

	if( nParamNum <= 0 ) return 0;

	std::vector<INT> vec;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+3);
		vec.push_back(nIndex);
	}

	std::vector<INT>::iterator it = vec.begin();
	while (it != vec.end())
	{
		dwValue = pActivity->GetScriptData(*it);
		lua_pushnumber(L, dwValue);
		++it;
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// 保存固定活动脚本通用数据
//-------------------------------------------------------------------------------------
int SC_SaveActScriptData(lua_State* L)
{
	DWORD	dwActID			=	(DWORD)lua_tonumber(L, 1);

	CActivity* pActivity = g_activityMgr.GetActivity(dwActID);
	if( !P_VALID(pActivity) )	return 0;
		
	pActivity->SaveActivityData2DB();

	return 0;
}

//-------------------------------------------------------------------------------------
// 得到帮派占有神迹的个数 0:没占有 
//-------------------------------------------------------------------------------------
static int SC_GetGuildCaptureGodMiracleNum(lua_State* L)
{
	DWORD	dwGuild			=	(DWORD)lua_tonumber(L, 1);

	// 神迹活动写死
	CActivity* pActive = NULL;
	pActive	= g_activityMgr.GetActivity(GUILBATTLE_ACTIVITY_ID);
	if ( NULL != pActive)
	{
		lua_pushnumber(L, pActive->GetGuildCpatureGodMiracleNum(dwGuild));
	}
	else
		lua_pushnumber(L, 0);
	
	return 1;
}

//-------------------------------------------------------------------------------------
// 得到夺宝奇兵一周内每一天是否开放的状态
//-------------------------------------------------------------------------------------
static int SC_GetDuoBaoQiBingOpenState(lua_State* L)
{
	for (DWORD dwWeekDay = 0; dwWeekDay < DAYS_ONE_WEEK; dwWeekDay++)
	{
		lua_pushnumber(L, g_worldSession.GetDBQBOpenState(dwWeekDay));
	}

	return DAYS_ONE_WEEK;
}

//-------------------------------------------------------------------------------------
// 创建玩家小队
//-------------------------------------------------------------------------------------
int SC_CreateTeam(lua_State* L)
{
	DWORD	dwSrcRoleID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwDesRoleID			=	(DWORD)lua_tonumber(L, 2);

	tagCreateTeam	CreateTeam;
	CreateTeam.dwSrcRoleID = dwSrcRoleID;
	CreateTeam.dwDesRoleID = dwDesRoleID;
	g_groupMgr.AddEvent(GT_INVALID, EVT_CreateTeam, sizeof(tagCreateTeam), &CreateTeam);

	return 0;
}

//-------------------------------------------------------------------------------------
// 增加一个玩家到玩家小队
//-------------------------------------------------------------------------------------
int SC_AddTeamMember(lua_State* L)
{
	DWORD	dwTeamID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwDesRoleID		=	(DWORD)lua_tonumber(L, 2);

	tagAddTeamMember	AddTeamMem;
	AddTeamMem.dwTeamID = dwTeamID;
	AddTeamMem.dwDesRoleID = dwDesRoleID;

	g_groupMgr.AddEvent(GT_INVALID, EVT_AddMember, sizeof(tagAddTeamMember), &AddTeamMem);

	return 0;
}

//-------------------------------------------------------------------------------------
// 得到小队队长的ID
//-------------------------------------------------------------------------------------
int SC_GetTeamLeaderID(lua_State* L)
{
	DWORD	dwTeamID		=	(DWORD)lua_tonumber(L, 1);

	const Team*	pTeam		=	g_groupMgr.GetTeamPtr(dwTeamID);
	if ( !P_VALID(pTeam) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	lua_pushnumber(L, pTeam->GetMemID(0));

	return 1;
}


//-------------------------------------------------------------------------------------
// 设置地图脚本通用数据
//-------------------------------------------------------------------------------------
int SC_SetMapScriptData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	INT		dwInstanceID	=	lua_tointeger(L, 2);
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到地图
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+4);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+1+4);

		pMap->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 获得地图脚本通用数据
//-------------------------------------------------------------------------------------
int SC_GetMapScriptData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到地图
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	if( nParamNum <= 0 ) return 0;

	std::vector<INT> vec;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+4);
		vec.push_back(nIndex);
	}

	std::vector<INT>::iterator it = vec.begin();
	while (it != vec.end())
	{
		dwValue = pMap->GetScriptData(*it);
		lua_pushnumber(L, dwValue);
		++it;
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// 地图动态刷出非碰撞怪物
//-------------------------------------------------------------------------------------
int SC_MapCreateCreature(lua_State* L)
{
 	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwTypeID		=	(DWORD)lua_tonumber(L, 3);
	FLOAT	fx				=	(FLOAT)lua_tonumber(L, 4);
	FLOAT	fy				=	(FLOAT)lua_tonumber(L, 5);
	FLOAT	fz				=	(FLOAT)lua_tonumber(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	Vector3 vPos;
	vPos.x = fx * TILE_SCALE;
	vPos.y = fy;
	vPos.z = fz * TILE_SCALE;

	// 随机怪物朝向
	Vector3 vFaceTo;
	FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);
	vFaceTo.x = cosf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.z = sinf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.y = 0.0f;

	Creature *pCreature = pMap->CreateCreature(dwTypeID, vPos, vFaceTo);

	if (P_VALID(pCreature))
	{
		lua_pushnumber(L, pCreature->GetID());
		return 1;
	}

	lua_pushnumber(L, GT_INVALID);
	return 1;

}

//-------------------------------------------------------------------------------------
// 动态刷出碰撞怪物
//-------------------------------------------------------------------------------------
int SC_MapCreateColCreature(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwTypeID		=	(DWORD)lua_tonumber(L, 3);
	FLOAT	fx				=	(FLOAT)lua_tonumber(L, 4);
	FLOAT	fy				=	(FLOAT)lua_tonumber(L, 5);
	FLOAT	fz				=	(FLOAT)lua_tonumber(L, 6);
	BOOL	bCollide		=	lua_toboolean(L, 7);
	CHAR*	pPatrolListName	=   const_cast<CHAR*>(lua_tostring(L, 8));


	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	Vector3 vPos;
	vPos.x = fx * TILE_SCALE;
	vPos.y = fy;
	vPos.z = fz * TILE_SCALE;

	// 随机怪物朝向
	Vector3 vFaceTo;
	FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);
	vFaceTo.x = cosf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.z = sinf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.y = 0.0f;

	Creature *pCreature = pMap->CreateCreature(dwTypeID, vPos, vFaceTo, GT_INVALID, bCollide, pPatrolListName);

	if (P_VALID(pCreature))
	{
		lua_pushnumber(L, pCreature->GetID());
		return 1;
	}

	lua_pushnumber(L, GT_INVALID);
	return 1;
}

//-------------------------------------------------------------------------------------
// 删除怪物
//-------------------------------------------------------------------------------------
int SC_MapDeleteCreature(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwID			=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	pMap->DeleteCreature(dwID);

	return 0;
}

//-------------------------------------------------------------------------------------
// 创建副本
//-------------------------------------------------------------------------------------
int SC_CreateInstance(lua_State* L)
{
	DWORD	dwMapID 		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);

	MapMgr* pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr))		return 0;

	BOOL	bRet = pMapMgr->CreateScriptInstance(dwInstanceID);

	lua_pushboolean(L, bRet);

	return 1;	
}

//-------------------------------------------------------------------------------------
// 删除副本
//-------------------------------------------------------------------------------------
int SC_DeleteInstance(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);

	MapMgr* pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr))		return 0;

	MapInstance* pInstance = pMapMgr->GetInstance(dwInstanceID);
	if( !P_VALID(pInstance))			return 0;

	pInstance->SetDelete();
	
	return 0;
}

//-------------------------------------------------------------------------------------
// 播放场景特效
//-------------------------------------------------------------------------------------
int SC_PlaySceneEffect(lua_State* L)
{
	DWORD dwMapID			= (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID		= (DWORD)lua_tonumber(L, 2);
	ESceneEffectType eType	= (ESceneEffectType)lua_tointeger(L, 3);
	DWORD dwObjID			= (DWORD)lua_tonumber(L, 4);
	DWORD dwEffectID		= (DWORD)lua_tonumber(L, 5);
	FLOAT fPosX				= (FLOAT)lua_tonumber(L, 6);
	FLOAT fPosY				= (FLOAT)lua_tonumber(L, 7);
	FLOAT fPosZ				= (FLOAT)lua_tonumber(L, 8);

	Vector3 vPos(fPosX, fPosY, fPosZ);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->PlaySceneEffect(eType, dwObjID, vPos, dwEffectID);

	return 0;
}

//-------------------------------------------------------------------------------------
// 停止场景特效
//-------------------------------------------------------------------------------------
int SC_StopSceneEffect(lua_State* L)
{
	DWORD dwMapID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	= (DWORD)lua_tonumber(L, 2);
	DWORD dwObjID		= (DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->StopSceneEffect(dwObjID);

	return 0;
}

//-------------------------------------------------------------------------------------
// 打开/关闭地图上指定的门
//-------------------------------------------------------------------------------------
int SC_OpenCloseDoor(lua_State* L)
{
	DWORD dwMapID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	= (DWORD)lua_tonumber(L, 2);
	DWORD dwObjID		= (DWORD)lua_tonumber(L, 3);
	BOOL bOpen			= lua_toboolean(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->OpenCloseDoor(dwObjID, bOpen);

	return 0;
}

//-------------------------------------------------------------------------------------
// 打开/关闭地图上所有的门
//-------------------------------------------------------------------------------------
int SC_OpenCloseAllDoors(lua_State* L)
{
	DWORD dwMapID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	= (DWORD)lua_tonumber(L, 2);
	BOOL bOpen			= lua_toboolean(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->OpenCloseAllDoors(bOpen);

	return 0;
}

//-------------------------------------------------------------------------------------
// 得到地图中所有门的位置
//-------------------------------------------------------------------------------------
int SC_GetDoorPos(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);

	const tagMapInfo* pInfo = g_mapCreator.GetMapInfo(dwMapID);
	if (!P_VALID(pInfo))
		return 0;

	tagMapDoor* pDoor = NULL;
	pInfo->mapDoor.ResetIterator();
	INT nTableNum = 0;
	while (pInfo->mapDoor.PeekNext(pDoor))
	{
		lua_newtable(L);
		lua_pushnumber(L, pDoor->vPos.x / TILE_SCALE);
		lua_rawseti(L, -2, nTableNum*3 + 1);

		lua_pushnumber(L, pDoor->vPos.y);
		lua_rawseti(L, -2, nTableNum*3 + 2);

		lua_pushnumber(L, pDoor->vPos.z / TILE_SCALE);
		lua_rawseti(L, -2, nTableNum*3 + 3);

		++nTableNum;
	}

	return pInfo->mapDoor.Size();
}

//-------------------------------------------------------------------------------------
// 打开或关闭地图上指定位置的门
//-------------------------------------------------------------------------------------
int SC_OpenCloseDoorByPos(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	FLOAT	x				=	(FLOAT)lua_tonumber(L, 3);
	FLOAT	y				=	(FLOAT)lua_tonumber(L, 4);
	FLOAT	z				=	(FLOAT)lua_tonumber(L, 5);
	BOOL	bOpen			=	lua_toboolean(L, 6);

	const tagMapInfo* pInfo = g_mapCreator.GetMapInfo(dwMapID);
	if (!P_VALID(pInfo))
		return 0;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	tagMapDoor* pDoor = NULL;
	pInfo->mapDoor.ResetIterator();
	while (pInfo->mapDoor.PeekNext(pDoor))
	{
		FLOAT fDist = Vec3Dist( Vector3( pDoor->vPos.x / TILE_SCALE, pDoor->vPos.y, pDoor->vPos.z / TILE_SCALE ), Vector3(x, y, z));
		if (fDist < 10)
		{
			pMap->OpenCloseDoor(pDoor->dwObjID, bOpen);
		}
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取地图所有玩家ID
//-------------------------------------------------------------------------------------
int SC_GetMapAllRoleID(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	lua_newtable(L);
	Map::RoleMap::TMapIterator itRole = pMap->GetRoleMap().Begin();
	Role* pRole = NULL;
	INT n = 0;
	while (pMap->GetRoleMap().PeekNext(itRole, pRole))
	{
		lua_pushnumber(L, pRole->GetID());
		lua_rawseti(L, -2, ++n);
	}

	return 1;
}

//-------------------------------------------------------------------------------------
// 获取地图所有玩家ID
//-------------------------------------------------------------------------------------
int SC_IsCreateMap(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
		lua_pushboolean(L, 0);
	else
		lua_pushboolean(L,1);

	return 0;
}

//-------------------------------------------------------------------------------------
// 取一个随机的野外场景（非安全区）,取随机怪物并在它旁边放掉落一个物品
//-------------------------------------------------------------------------------------
static int SC_PutItemNearRandCreature(lua_State* L)
{
	DWORD dwItemID  = (DWORD)lua_tonumber(L, 1);

	DWORD dwRandMapID;
	if(rand()%2 == 0)
		dwRandMapID = TObjRef<Util>()->Crc32(_T("s03"));
	else
		dwRandMapID = TObjRef<Util>()->Crc32(_T("s04"));

	Map* pMap = g_mapCreator.GetMap(dwRandMapID, 0);
	if (!P_VALID(pMap))
		return 0;

	Vector3 vPos;
	INT64 n64Serial = 0;
	INT64 n64GroupID = 0;
	if (pMap->PutItemNearRandCreature(dwItemID, vPos, n64Serial, n64GroupID) != E_Success)
	{
		return 0;
	}
	else
	{
		lua_pushnumber(L, dwRandMapID);
		lua_pushnumber(L, vPos.x/TILE_SCALE);
		lua_pushnumber(L, vPos.z/TILE_SCALE);
		push_64bit_data(L, n64Serial);
		push_64bit_data(L, n64GroupID);
		return 7;
	}
}

static int SC_RemoveGroundItem(lua_State* L)
{
	DWORD dwMapID	  = (DWORD)lua_tonumber(L, 1);
	INT64 n64Serial   = pop_64bit_data(L, 2, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, 0);
	if (!P_VALID(pMap))
		return 0;	
	pMap->RemoveGroundItem(n64Serial);
	return 0;
}

//-------------------------------------------------------------------------------------
// 给地图所有的玩家增加一个buff
//-------------------------------------------------------------------------------------
int SC_AddMapRoleBuff(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwBuffTypeID	=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	const tagBuffProto* pProto = g_attRes.GetBuffProto(dwBuffTypeID);
	if( !P_VALID(pProto) ) return 0;

	Map::RoleMap::TMapIterator itRole = pMap->GetRoleMap().Begin();
	Role* pRole = NULL;
	while (pMap->GetRoleMap().PeekNext(itRole, pRole))
	{
		pRole->TryAddBuff(pRole, pProto, NULL, NULL, NULL);
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
// 玩家是否在某一脚本类型的区域
//-------------------------------------------------------------------------------------
int SC_IsInArea(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwObjID			=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushboolean(L, false);
		return 1;
	}

	const tagMapInfo* pMapInfo = pMap->GetMapInfo();
	if( !P_VALID(pMapInfo) )
	{
		lua_pushboolean(L, false);
		return 1;
	}

	Role* pRole = pMap->FindRole(dwRoleID);
	if( !P_VALID(pRole) )	
	{
		lua_pushboolean(L, false);
		return 1;
	}

	// 得到玩家的包裹盒和格子坐标
	AABBox roleBox = pRole->GetAABBox();
	INT nTileX = INT(pRole->GetCurPos().x / (FLOAT)TILE_SCALE);
	INT nTileZ = INT(pRole->GetCurPos().z / (FLOAT)TILE_SCALE);

	tagMapAreaInfo* pArea = NULL;

	// 检测脚本区
	pArea = pMap->IsInArea(pMapInfo->mapScriptArea, roleBox, nTileX, nTileZ);
	if( !P_VALID(pArea) )
	{
		lua_pushboolean(L, false);
		return 1;
	}

	if (pArea->dwObjID == dwObjID)
	{
		lua_pushboolean(L, true);
	}
	else
	{
		lua_pushboolean(L, false);
	}

	return 1;
}

//-------------------------------------------------------------------------------------
// 获得怪物周围的怪物
//-------------------------------------------------------------------------------------
int SC_GetAroundCreature(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	FLOAT	fOPRadius		=   (FLOAT)lua_tonumber(L, 4);
	FLOAT	fHigh			=	(FLOAT)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) )
	{
		return 0;
	}

	std::vector<DWORD> vecCreature;
	INT nCreatureNum = 0;
	INT Index = 1;
	fOPRadius = fOPRadius * TILE_SCALE;
	nCreatureNum = pCreature->GetAroundCreature(vecCreature, fOPRadius, fHigh);

	lua_createtable(L,nCreatureNum,0);

	std::vector<DWORD>::iterator it = vecCreature.begin();
	while(it != vecCreature.end())
	{
		lua_pushnumber(L, Index);
		lua_pushnumber(L, *it);
		lua_settable(L,-3);  
		++it;
		++Index;
	}

	return 1;
};

//-------------------------------------------------------------------------------------
// 获得怪物周围的玩家
//-------------------------------------------------------------------------------------
int SC_GetAroundRole(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	FLOAT	fOPRadius		=   (FLOAT)lua_tonumber(L, 4);
	FLOAT	fHigh			=	(FLOAT)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) )
	{
		return 0;
	}

	std::vector<DWORD> vecRole;
	INT nRoleNum = 0;
	INT Index = 1;
	fOPRadius = fOPRadius * TILE_SCALE;
	nRoleNum = pCreature->GetAroundRole(vecRole, fOPRadius, fHigh);

	lua_createtable(L,nRoleNum,0);

	std::vector<DWORD>::iterator it = vecRole.begin();
	while(it != vecRole.end())
	{
		lua_pushnumber(L, Index);
		lua_pushnumber(L, *it);
		lua_settable(L,-3);  
		++it;
		++Index;
	}

	return 1;
}

//-------------------------------------------------------------------------------------------
// 获得怪物的脚本数据
//-------------------------------------------------------------------------------------------
int SC_GetCreatureScriptData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	INT		nParamNum		=	lua_tointeger(L, 4);

	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// 得到参数个数
	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+5);
		if( nIndex < 0 || nIndex >= ESD_Creature ) return 0;

		dwValue = pCreature->GetScriptData(nIndex);
		lua_pushnumber(L, dwValue);
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// 设置怪物脚本通用数据
//-------------------------------------------------------------------------------------
int SC_SetCreatureScriptData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	INT		nParamNum		=	lua_tointeger(L, 4);

	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// 参数个数
	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+5);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+6);

		if( nIndex < 0 || nIndex >= ESD_Creature ) return 0;

		pCreature->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 改变怪物脚本通用数据
//-------------------------------------------------------------------------------------
int SC_ModCreatureScriptData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	INT		nParamNum		=	lua_tointeger(L, 4);

	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// 参数个数
	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+5);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+6);

		if( nIndex < 0 || nIndex >= ESD_Creature ) return 0;

		pCreature->ModScriptData(nIndex, dwValue);
	}

	return 0;
}

//------------------------------------------------------------------------------------------
// 设置怪物更新脚本AI的时间间隔，以tick为单位
//------------------------------------------------------------------------------------------
int SC_SetCreatureUpdateAITimer(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	INT		nTimer			=	lua_tointeger(L, 4);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	if( nTimer <= 0 && nTimer != GT_INVALID ) return 0;

	pCreature->GetAI()->SetScriptUpdateTimer(nTimer);

	return 0;
}

//------------------------------------------------------------------------------------------
// 怪物使用技能
//------------------------------------------------------------------------------------------
int SC_CreatureUseSkill(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwSkillID		=	(DWORD)lua_tonumber(L, 4);
	INT		nTargetType		=	lua_tointeger(L, 5);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	// 得到目标ID
	DWORD dwTargetID = pCreature->GetAI()->GetTargetIDByType((ECreatureTargetFriendEnemy)nTargetType);

	// 使用技能
	pCreature->GetAI()->AIUseSkill(dwSkillID, dwTargetID);

	return 0;
}

//---------------------------------------------------------------------------------------------
// 怪物喊话
//----------------------------------------------------------------------------------------------
int SC_MonsterSay(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwSayID			=	(DWORD)lua_tonumber(L, 4);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// 喊话
	pCreature->Say(dwSayID);

	return 0;
}

//----------------------------------------------------------------------------------------------
// 怪物播放动作
//----------------------------------------------------------------------------------------------
int SC_MonsterPlayAction(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	const CHAR*	szFourCC		=	lua_tostring(L, 4);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// 检查是否合法，fourCC只能是四个大小的字符
	if( !P_VALID(szFourCC) || strlen(szFourCC) != 4 ) return 0;

	// 转换成ID
	DWORD dwActionID = Cool3D::_MakeFourCCA(szFourCC);

	pCreature->PlayerAction(dwActionID);

	return 0;
}

//--------------------------------------------------------------------------------------------------
// 生物切换AI状态
//--------------------------------------------------------------------------------------------------
int SC_CreatureChangeAIState(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	INT			nState			=	lua_tointeger(L, 4);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	// 切换状态
	pCreature->GetAI()->ChangeState((AIStateType)nState);

	return 0;
}

//--------------------------------------------------------------------------------------------------
// 生物进入战斗后的时间
//--------------------------------------------------------------------------------------------------
int SC_GetEnterCombatTick(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	(DWORD)lua_tonumber(L, 3);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	// 得到进入战斗时间
	DWORD dwTick = pCreature->GetAI()->GetEnterCombatTick();

	lua_pushnumber(L, dwTick);

	return 1;
}

//--------------------------------------------------------------------------------------------------
// 根据怪物ID得到TypeID
//--------------------------------------------------------------------------------------------------
int SC_GetCreatureTypeID(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	(DWORD)lua_tonumber(L, 3);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	DWORD dwTypeID = pCreature->GetTypeID();

	lua_pushnumber(L, dwTypeID);

	return 1;
}

//-------------------------------------------------------------------------------------
// 增加怪物仇恨
//-------------------------------------------------------------------------------------
int SC_AddEnmity(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwTargetID		=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwValue			=	(DWORD)lua_tonumber(L, 5);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	Unit* pTarget = pMap->FindUnit(dwTargetID);
	if( !P_VALID(pTarget) ) return 0;

	pCreature->GetAI()->AddEnmity(pTarget, dwValue);

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取怪物当前的攻击目标
//-------------------------------------------------------------------------------------
int SC_GetCreatureCurTargetID(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) 
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	DWORD	dwTargetID = pCreature->GetAI()->GetTargetUnitID();

	lua_pushnumber(L, dwTargetID);

	return 1;
}

//-------------------------------------------------------------------------------------
// 获得玩家升级所需经验
//-------------------------------------------------------------------------------------
int SC_GetRoleLevelUpExp(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// 获得当前升级所需经验
	const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(pRole->GetLevel());
	if (!P_VALID(pEffect))	return 0;

	INT32 nHigh = pEffect->nExpLevelUp/1000000000;
	INT32 nLow = pEffect->nExpLevelUp % 1000000000;

	lua_pushinteger(L, nHigh);
	lua_pushinteger(L, nLow);
	return 2;
}

//-------------------------------------------------------------------------------------
// 玩家是否在线
//-------------------------------------------------------------------------------------
int SC_IsRoleOnLine(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
	{
		lua_pushinteger(L, 0);
	}
	else
	{
		lua_pushinteger(L, 1);
	}

	return 1;
}

//---------------------------------------------------------------------------------
// 获取帮派成员职位
//---------------------------------------------------------------------------------
static int SC_GetGuildPos(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 2);

	Guild* pGuild = NULL;
	if (!GT_VALID(dwGuildID))
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if (!P_VALID(pRole))
		{
			// 不在线
			return 0;
		}
		dwGuildID = pRole->GetGuildID();
		if (!GT_VALID(dwGuildID))
		{
			// 不在帮派
			return 0;
		}
	}
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		// 帮派不存在
		return 0;
	}

	tagGuildMember* pMember = pGuild->GetMember(dwRoleID);
	if (!P_VALID(pMember))
	{
		// 不在帮派中
		return 0;
	}

	lua_pushinteger(L, pMember->eGuildPos);

	return 1;
}

//---------------------------------------------------------------------------------
// 获取帮派资金
//---------------------------------------------------------------------------------
static int SC_GetGuildFund(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().nFund);

	return 1;
}

//---------------------------------------------------------------------------------
// 获取帮派资材
//---------------------------------------------------------------------------------
static int SC_GetGuildMaterial(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().nMaterial);

	return 1;
}

//---------------------------------------------------------------------------------
// 获取帮派安定度
//---------------------------------------------------------------------------------
static int SC_GetGuildPeace(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().n16Peace);

	return 1;
}

//---------------------------------------------------------------------------------
// 获取帮派声望
//---------------------------------------------------------------------------------
static int SC_GetGuildReputation(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().nReputation);

	return 1;
}
//---------------------------------------------------------------------------------
// 检查帮派状态
//---------------------------------------------------------------------------------
static int SC_IsGuildInStatus(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwStatus	= (DWORD)lua_tonumber(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushboolean(L, pGuild->IsInGuildStateAll(dwStatus));

	return 1;
}

//---------------------------------------------------------------------------------
// 设置帮派状态
//---------------------------------------------------------------------------------
static int SC_SetGuildStatus(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	INT	nStatus		= (INT)lua_tonumber(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	pGuild->SetGuildState((EGuildSpecState)nStatus);
	return 0;
}

//---------------------------------------------------------------------------------
// 取消帮派状态
//---------------------------------------------------------------------------------
static int SC_UnsetGuildStatus(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	INT nStatus		= (INT)lua_tonumber(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	pGuild->UnsetGuildState((EGuildSpecState)nStatus);
	return 0;
}

// 帮派资金变更
//---------------------------------------------------------------------------------
static int SC_ModifyGuildFund(lua_State* L)
{
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	INT32 nFund			= (INT32)lua_tonumber(L, 3);
	DWORD dwLogCmdID	= (DWORD)lua_tointeger(L, 4);

	if (nFund == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (nFund > 0)
	{
		pGuild->IncGuildFund(dwRoleID, nFund, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildFund(dwRoleID, -nFund, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// 帮派资材变更
//---------------------------------------------------------------------------------
static int SC_ModifyGuildMaterial(lua_State* L)
{
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	INT32 nMaterial		= (INT32)lua_tonumber(L, 3);
	DWORD dwLogCmdID	= (DWORD)lua_tointeger(L, 4);

	if (nMaterial == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (nMaterial > 0)
	{
		pGuild->IncGuildMaterial(dwRoleID, nMaterial, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildMaterial(dwRoleID, -nMaterial, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// 帮派安定度变更
//---------------------------------------------------------------------------------
static int SC_ModifyGuildPeace(lua_State* L)
{
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	INT16 n16Peace		= (INT16)lua_tonumber(L, 3);
	DWORD dwLogCmdID	= (DWORD)lua_tointeger(L, 4);

	if (n16Peace == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (n16Peace > 0)
	{
		pGuild->IncGuildPeace(dwRoleID, n16Peace, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildPeace(dwRoleID, -n16Peace, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// 帮派成员威望变更
//---------------------------------------------------------------------------------
static int SC_ModifyGuildReputation(lua_State* L)
{
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	INT32 nReputation	= (INT32)lua_tonumber(L, 3);
	DWORD dwLogCmdID	= (DWORD)lua_tointeger(L, 4);

	if (nReputation == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (nReputation > 0)
	{
		pGuild->IncGuildReputation(dwRoleID, nReputation, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildReputation(dwRoleID, -nReputation, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// 帮派成员贡献变更
//---------------------------------------------------------------------------------
static int SC_ModifyContribution(lua_State* L)
{
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	INT32 nContribute	= (INT32)lua_tonumber(L, 3);
	DWORD dwLogCmdID	= (DWORD)lua_tointeger(L, 4);

	Guild* pGuild = NULL;
	if (!GT_VALID(dwGuildID))
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if (!P_VALID(pRole))
		{
			// 不在线
			return 0;
		}
		dwGuildID = pRole->GetGuildID();
		if (!GT_VALID(dwGuildID))
		{
			// 不在帮派
			return 0;
		}
	}
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		// 帮派不存在
		return 0;
	}

	if (nContribute > 0)
	{
		pGuild->ChangeMemberContribution(dwRoleID, nContribute, TRUE);
	}
	else
	{
		pGuild->ChangeMemberContribution(dwRoleID, -nContribute, FALSE);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// 帮派成员功勋变更
//---------------------------------------------------------------------------------
static int SC_ModifyMemberExploit(lua_State* L)
{
	DWORD dwGuildID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 2);
	INT32 nExploit		=	lua_tointeger(L, 3);

	Guild* pGuild = NULL;
	if (!GT_VALID(dwGuildID))
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if (!P_VALID(pRole))
		{
			// 不在线
			return 0;
		}
		dwGuildID = pRole->GetGuildID();
		if (!GT_VALID(dwGuildID))
		{
			// 不在帮派
			return 0;
		}
	}
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		// 帮派不存在
		return 0;
	}

	pGuild->ChangeMemberExploit(dwRoleID, nExploit);

	return 0;
}


//---------------------------------------------------------------------------------
//检查帮派是否存在
//---------------------------------------------------------------------------------
static int SC_IsHaveGuild(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
		lua_pushboolean(L, FALSE);
	else
		lua_pushboolean(L, TRUE);

	return 1;
}

//---------------------------------------------------------------------------------
// 给所有帮派在在线成员添加BUFF
//---------------------------------------------------------------------------------
static int SC_AddBuffToAllGuildMember(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwBuffID	= (DWORD)lua_tonumber(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	pGuild->GetMemberMgr().AddBuffToAllOnLineMember(dwBuffID);

	return 0;
}

//---------------------------------------------------------------------------------
// 移除所有在线帮派成员的BUFF
//---------------------------------------------------------------------------------
static int SC_RemoveBuffFromAllGuildMember(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwBuffID	= (DWORD)lua_tonumber(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}
	pGuild->GetMemberMgr().RemoveBuffFromAllOnLineMember(dwBuffID);

	return 0;
}

//---------------------------------------------------------------------------------
// 移除所有指定公会成员的状态（EState）
//---------------------------------------------------------------------------------
static int SC_UnsetStateOfAllGuildMember(lua_State* L)
{
	DWORD	dwGuildID		= (DWORD)lua_tonumber(L, 1);
	EState	eState			= (EState)lua_tointeger(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}
	pGuild->GetMemberMgr().UnsetStateOfAllGuildMember(eState);

	return 0;
}

//---------------------------------------------------------------------------------
// 移除所有战场参战成员的状态（EState）
//---------------------------------------------------------------------------------
static int SC_UnsetStateOfAllRoleInBattleField(lua_State* L)
{
	DWORD	eGuildCapType	= (DWORD)lua_tonumber(L, 1);
	EState	eState			= (EState)lua_tointeger(L, 2);
	DWORD	eGodMiracle		= (DWORD)lua_tonumber(L, 3);

	CGodMiracleMgr* pGodMiracleMgr = CGodMiracleMgr::GetInst();
	if (!P_VALID(pGodMiracleMgr))
	{
		return 0;
	}
	
	CGodMiracle* pGodMiracle = pGodMiracleMgr->GetGodMiracle((EGodMiracle)eGodMiracle);
	if (!P_VALID(pGodMiracle))
	{
		return 0;
	}

	if (eGuildCapType == EGCT_Defend || eGuildCapType == EGCT_Capture)
	{
		pGodMiracle->UnsetStateOfAllRoleInBattleField((EGuildCapType)eGuildCapType, eState);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// 得到所有的帮派成员
//---------------------------------------------------------------------------------
static int SC_GetAllGuildMember(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}
	
	INT16 n16GuildNumber(0);
	DWORD dwRoleID[100];

	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = pGuild->GetMemberMgr().GetMemberMap().Begin();
	while(pGuild->GetMemberMgr().GetMemberMap().PeekNext(iter, pMember))
	{
		dwRoleID[n16GuildNumber] = pMember->dwRoleID;
		n16GuildNumber++;
	}

	lua_newtable(L);
	for (int n=0; n<n16GuildNumber; n++)
	{
		lua_pushnumber(L, dwRoleID[n]);
		lua_rawseti(L, -2, n+1);
	}
	return 1;
}

//---------------------------------------------------------------------------------
// 帮派的成员是否有变化
//---------------------------------------------------------------------------------
static int SC_InChangeForGuildMember(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if ( TRUE == pGuild->GetMemberMgr().GetGuildMemberChangeFlag())
		lua_pushboolean(L, TRUE);
	else
		lua_pushboolean(L, FALSE);
	
	return 1;
}

//---------------------------------------------------------------------------------
// 角色进入神迹区域
//---------------------------------------------------------------------------------
static int SC_GuildRoleInterGodArea(lua_State* L)
{
	DWORD dwGodID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 2);
	DWORD DOrCFlag	= (DWORD)lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodID);
	if (!P_VALID(pGodMiracle)) return 0;

	BOOL bRet = pGodMiracle->PushGodRoleInfo(DOrCFlag, dwRoleID);
	if (FALSE == bRet)
		return 0;
	return 0;
}

//---------------------------------------------------------------------------------
// 角色进入神迹区域
//---------------------------------------------------------------------------------
static int SC_SetRoleStartActiveTimes(lua_State* L)
{
	DWORD dwGodID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 2);
	DWORD DOrCFlag	= (DWORD)lua_tonumber(L, 3);
	DWORD IsInAreaFlag	= (DWORD)lua_tonumber(L, 4);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodID);
	if (!P_VALID(pGodMiracle)) return 0;
	
	tagGodRoleInfo* pGodRoleInfo = pGodMiracle->GetGodRoleInfo(DOrCFlag, dwRoleID);
	if (!P_VALID(pGodRoleInfo)) return 0;

	pGodRoleInfo->SetAreaFlag(IsInAreaFlag);
	
	return 0;
}

//---------------------------------------------------------------------------------
// 得到当前神迹的状态
//---------------------------------------------------------------------------------
static int SC_GetGodMiracleCurState(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;
	
	lua_pushinteger(L, pGodMiracle->GetGodMiracleState());
	return 1;
}


//---------------------------------------------------------------------------------
// 更新神迹，记录活跃时间
//---------------------------------------------------------------------------------
static int SC_UpdateGodMiracleMin(lua_State* L)
{
	 CGodMiracleMgr::GetInst()->UpdateMin();
	 return 0;
}

//---------------------------------------------------------------------------------
// 更新神迹，记录活跃时间
//---------------------------------------------------------------------------------
static int SC_UpdateGodMiracle(lua_State* L)
{
	 CGodMiracleMgr::GetInst()->Update();
	return 0;
}
//---------------------------------------------------------------------------------
// 设置占领神迹的帮派ID
//---------------------------------------------------------------------------------
static int SC_SetCaptureGodGuildID(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 2);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->SetCapGodID(dwGuildID);
	ILOG->Write(_T("SC_SetCaptureGodGuildID::SetCapGodID(dwGuildID):	%d\r\n"), dwGuildID);
	pGodMiracle->SetCapStartTime(GetCurrentDWORDTime());
	pGodMiracle->SaveCapTureInfoToActive();
	return 0;
}

//---------------------------------------------------------------------------------
// 设置占领神迹的帮派ID
//---------------------------------------------------------------------------------
static int SC_GetCaptureGodGuildID(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	lua_pushnumber(L, pGodMiracle->GetCapGodID());
	return 1;
}


//---------------------------------------------------------------------------------
// 设置申请占领神迹的帮派ID
//---------------------------------------------------------------------------------
static int SC_SetApplyGuildID(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 2);
	DWORD dwMoney		= (DWORD)lua_tonumber(L, 3);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->SetApplyID(dwGuildID);
	pGodMiracle->SetApplyMoney(dwMoney);
	
	return 0;
}

//---------------------------------------------------------------------------------
// 设置申请占领神迹的帮派ID
//---------------------------------------------------------------------------------
static int SC_GetApplyGuildID(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	// 申请帮派解散
	Guild* pGuild = g_guildMgr.GetGuild(pGodMiracle->GetApplyID());
	if ( P_VALID(pGuild))
		lua_pushnumber(L,  pGodMiracle->GetApplyID());
	else
		lua_pushnumber(L, 0);
	return 1;
}

//---------------------------------------------------------------------------------
// 设置争夺帮派的玩家死
//---------------------------------------------------------------------------------
static int SC_SetGuildRoleDie(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRole		= (DWORD)lua_tonumber(L, 2);
	DWORD dwBeKiller	= (DWORD)lua_tonumber(L, 3);
	DWORD dwDOrCFlag	= (DWORD)lua_tonumber(L, 4);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	tagGodRoleInfo* pGodRoleInfo = pGodMiracle->GetGodRoleInfo(dwDOrCFlag, dwRole);
	if (!P_VALID(pGodRoleInfo)) return 0;

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRole);
	if (!P_VALID(pRoleInfo)) return 0;

	if ( 1 == dwDOrCFlag )
	{
		pGodRoleInfo->SetBeKilledRole(dwBeKiller, pRoleInfo->dwGuildID);
	}
	else if ( 2 == dwDOrCFlag )
	{
		pGodRoleInfo->SetBeKilledRole(dwBeKiller, pRoleInfo->dwGuildID);
	}

	/// 对组队的角色特殊处理
	CGodMiracleMgr::GetInst()->ModteamInfoForkillRole(dwGodIndex, dwRole, dwBeKiller);

	return 0;
}

//---------------------------------------------------------------------------------
// 胜负计算
//---------------------------------------------------------------------------------
static int SC_CalCaptureContributionAndExploit(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwDOrCFlag	= (DWORD)lua_tonumber(L, 2);	// 那方胜利标志

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->CalAllRoleContributionAndExploit(dwDOrCFlag);

	return 0;
}

//---------------------------------------------------------------------------------
//初始化某神迹 
//---------------------------------------------------------------------------------
static int SC_InitGodMiracle(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	
	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;
	
	pGodMiracle->Init();

	return 0;
}

//---------------------------------------------------------------------------------
// 设置某神迹的当前状态 
//---------------------------------------------------------------------------------
static int SC_SetGodCurState(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwCurState	= (DWORD)lua_tonumber(L, 2);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->SetCurState((EGodMiracleState)dwCurState);
	return 0;
}

//---------------------------------------------------------------------------------
// 设置某神迹的当前状态 
//---------------------------------------------------------------------------------
static int SC_RoleLeaveArea(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	DWORD dwDOrCFlag	= (DWORD)lua_tonumber(L, 3);
	
	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->RemoveGodRoleInfo(dwDOrCFlag, dwRoleID);
	return 0;
}
//---------------------------------------------------------------------------------
// 得到所有参战的最优秀的玩家
//---------------------------------------------------------------------------------
static int SC_SendAllCoptureGodGuildMember(lua_State* L)
{
	CGodMiracleMgr::GetInst()->BroadcastAllGoodPlayer();
	return 0;
}

//---------------------------------------------------------------------------------
// 设置神迹防御和进攻方的积分
//---------------------------------------------------------------------------------
static int SC_SetGodGuildPoint(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwDOrCFlag	= (DWORD)lua_tonumber(L, 2);
	DWORD dwPoint		= (DWORD)lua_tonumber(L, 3);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->ModPoint(dwDOrCFlag, dwPoint);
	return 0;
}

//---------------------------------------------------------------------------------
// 取防守公会ID
//---------------------------------------------------------------------------------
static int SC_GetDefendGuildID(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	
	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	DWORD dwGuildID = pGodMiracle->GetCapGodID();
	lua_pushnumber(L, dwGuildID);
	return 1;
}

//---------------------------------------------------------------------------------
// 取公会阵营类型，1表示防守，2表示攻击，0表示没有参战权
//---------------------------------------------------------------------------------
static int SC_GetGuildAllianceType(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 2);
	
	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))	return 0;

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	BOOL bAlliance = FALSE;
	EGuildCapType eGuildCapType = pGodMiracle->GetCapType(dwGuildID,bAlliance);
	if (eGuildCapType != EGCT_Capture && eGuildCapType != EGCT_Defend && eGuildCapType != EGCT_NULL)
		return 0;

	if(bAlliance)
	{
		BOOL bConfirmed = pGodMiracle->GetAllianceConfirmed(dwGuildID);
		if (!bConfirmed)
			return 0;
	}

	lua_pushnumber(L, static_cast<DWORD>(eGuildCapType));
	return 1;
}

static int SC_CanGather( lua_State* L )
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 2);

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) || !P_VALID(pRole->GetMap())) return 0;

	Creature* pRes = pRole->GetMap()->FindCreature(dwCreatureID);
	if (!P_VALID(pRes)) return 0;

	INT nRt = pRole->CanGather(pRes);

	lua_pushinteger(L, nRt);

	return 1;
}

//---------------------------------------------------------------------------------
// 获取参与城战的帮派
//---------------------------------------------------------------------------------
static int SC_GetConfirmGuild( lua_State* L )
{
	DWORD dwCityID = (DWORD)lua_tonumber(L, 1);
	ECityForceType eForce = (ECityForceType)lua_tointeger(L, 2);

	// 取出城市
	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
	{
		return 0;
	}

	// 获取参与帮派
	INT nGuildNum = MAX_CITY_ATTACK_FORCE;
	DWORD dwGuildID[MAX_CITY_ATTACK_FORCE];
	memset(dwGuildID, 0xFF, sizeof(dwGuildID));

	if (!pCity->GetConfirmGuild(eForce, nGuildNum, dwGuildID))
	{
		return 0;
	}

	//lua_pushinteger(L, nGuildNum);
	if (nGuildNum <= 0)
	{
		return 0;
	}

	lua_newtable(L);
	for (int n=0; n<nGuildNum; n++)
	{
		lua_pushnumber(L, dwGuildID[n]);
		lua_rawseti(L, -2, n+1);
	}
	
	return 1;
}

//---------------------------------------------------------------------------------
// 获取城市属性 -- 仅限于获取单个数据，无法获取城市产出、报名等信息
//---------------------------------------------------------------------------------
static int SC_GetCityAppointedAtt(lua_State* L)
{
	DWORD dwCityID	=	(DWORD)lua_tonumber(L, 1);
	INT nType		=	lua_tointeger(L, 2);
	DWORD dwData	=	GT_INVALID;

	// 取出城市
	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
	{
		return 0;
	}

	INT nNum = 0;
	pCity->GetCityAppointedAtt(ECityAttType(nType), nNum, &dwData);

	lua_pushnumber(L, dwData);
	return 1;
}
//---------------------------------------------------------------------------------
// 根据地图id获取城市属性 -- 仅限于获取单个数据，无法获取城市产出、报名等信息
//---------------------------------------------------------------------------------
static int SC_GetCityAppointedAttByMapID(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	INT		nType			=	lua_tointeger(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	// 取出城市
	City* pCity = g_cityMgr.GetCity(pMap);
	if (!P_VALID(pCity))
	{
		return 0;
	}
	
	DWORD dwData = GT_INVALID;
	INT nNum = 0;
	pCity->GetCityAppointedAtt(ECityAttType(nType), nNum, &dwData);

	lua_pushnumber(L, dwData);
	return 1;

}
//---------------------------------------------------------------------------------
// 改变城市防卫值
//---------------------------------------------------------------------------------
static int SC_ChangeCityDefence(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	INT32	nChangeVal		=	lua_tointeger(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	// 取出城市
	City* pCity = g_cityMgr.GetCity(pMap);
	if (!P_VALID(pCity))
	{
		return 0;
	}
	if(nChangeVal > 0)
	{
		pCity->IncCityDefence(nChangeVal);
	}
	else if(nChangeVal < 0)
	{
		pCity->DecCityDefence(-nChangeVal);
	}

	return 1;
}
//---------------------------------------------------------------------------------
// 改变城市统治帮派
//---------------------------------------------------------------------------------
static int SC_ChangeCityHolder(lua_State* L)
{
	DWORD dwCityID	=	(DWORD)lua_tonumber(L, 1);
	DWORD dwGuildID =	(DWORD)lua_tonumber(L, 2);

	// 取出城市
	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		// 帮派不存在
		return 0;
	}

	pCity->ChangeCityHolder(dwGuildID);

	return 0;
}

//---------------------------------------------------------------------------------
// 发送城战状态
//---------------------------------------------------------------------------------
static int SC_SendCityStruggleInfo(lua_State* L)
{
	DWORD dwMapID					=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID				=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID					=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagLongZhuState sLongZhuState;
	sLongZhuState.eForce			=	(ECityForceType)lua_tointeger(L, 4);	// 当前占领龙珠的联盟
	sLongZhuState.bSealing			=	lua_toboolean(L, 5);					// 龙珠是否正在被封印
	sLongZhuState.nCountDown		=	lua_tointeger(L, 6);					// 龙珠下方的倒计时
	
	tagYinYangZhuState sYinYangState[5];
	sYinYangState[0].eForce			=	(ECityForceType)lua_tointeger(L, 7);					// 阴阳柱占领状态
	sYinYangState[1].eForce			=	(ECityForceType)lua_tointeger(L, 8);
	sYinYangState[2].eForce			=	(ECityForceType)lua_tointeger(L, 9);
	sYinYangState[3].eForce			=	(ECityForceType)lua_tointeger(L, 10);
	sYinYangState[4].eForce			=	(ECityForceType)lua_tointeger(L, 11);
	
	INT32 nWuZiKuNum				=	lua_tointeger(L, 12);					// 物资库数量

	tagCityDoorState sCityDoorState[3];											// 城门状态
	sCityDoorState[0].nDoorType		=	lua_tointeger(L, 13);
	sCityDoorState[0].bDestroied	=	lua_tointeger(L, 14);
	sCityDoorState[0].nCountDown	=	lua_tointeger(L, 15);

	sCityDoorState[1].nDoorType		=	lua_tointeger(L, 16);
	sCityDoorState[1].bDestroied	=	lua_tointeger(L, 17);
	sCityDoorState[1].nCountDown	=	lua_tointeger(L, 18);

	sCityDoorState[2].nDoorType		=	lua_tointeger(L, 19);
	sCityDoorState[2].bDestroied	=	lua_tointeger(L, 20);
	sCityDoorState[2].nCountDown	=	lua_tointeger(L, 21);

	INT32 nAtkPlayerNum				=	lua_tointeger(L, 22);					// 两方总人数
	INT32 nDefPlayerNum				=	lua_tointeger(L, 23);

	tagNS_CityStruggleInfo send;
	send.nAtkPlayerNum = nAtkPlayerNum;
	send.nDefPlayerNum = nDefPlayerNum;
	send.nWuZiKuNum = nWuZiKuNum;
	memcpy(send.sDoorState, sCityDoorState, sizeof(sCityDoorState));
	send.sLongZhuState = sLongZhuState;
	memcpy(send.sYinYangZhuState, sYinYangState, sizeof(sYinYangState));
	
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 发送一条城战信息
//-------------------------------------------------------------------------------------
int SC_SendOneCityStruggleInfo(lua_State* L)
{
	DWORD dwMapID					=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID				=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID					=	(DWORD)lua_tonumber(L, 3);
	ECityStruggleChangeType eType1	=	(ECityStruggleChangeType)lua_tointeger(L, 4);	// 类型1
	INT32 nType2					=	lua_tointeger(L, 5);							// 类型2
	INT32 nData						=	lua_tointeger(L, 6);							// 改变的数据

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_CityStruggleInfoChange send;
	send.eType1 = eType1;
	send.nType2 = nType2;
	send.nData = nData;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// 设置城战状态
//-------------------------------------------------------------------------------------
static int SC_SetCityStruggleState(lua_State* L)
{
	DWORD				dwCityID	=	(DWORD)lua_tonumber(L, 1);
	ECityStruggleState	state		=	(ECityStruggleState)lua_tointeger(L, 2);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->SetCityStruggleState(state);
	return 0;
}

//-------------------------------------------------------------------------------------
// 设置联盟复活时间
//-------------------------------------------------------------------------------------
static int SC_SetReviveTime(lua_State* L)
{
	DWORD			dwCityID	=	(DWORD)lua_tonumber(L, 1);
	ECityForceType	eForceType	=	(ECityForceType)lua_tointeger(L, 2);
	INT				nSecond		=	(INT)lua_tonumber(L, 3);

	if (nSecond < 0)
		return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->SetReviveTime(eForceType, nSecond);

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取玩家阵营
//-------------------------------------------------------------------------------------
static int SC_GetRoleForce(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	ECityForceType eForce;
	if (TRUE == pCity->GetForceType(pRole, eForce))
	{
		lua_pushnumber(L, eForce);
		return 1;
	}

	lua_pushnumber(L, 2);
	return 1;
}

//-------------------------------------------------------------------------------------
// 玩家进入离开城战场景--需给客户端发消息
//-------------------------------------------------------------------------------------
static int SC_EnterLeaveCityStruggle(lua_State* L)
{
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 2);
	BOOL	bEnter			=	(BOOL)lua_tointeger(L, 3);
	ECityForceType	eForce	=	(ECityForceType)lua_tointeger(L, 4);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))	return 0;

	// 找到玩家
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	// 给客户端发消息
	if (bEnter)
	{
		// 发送给客户端敌对势力的帮派id
		if (eForce == ECFT_Attack)
		{
			// 设置玩家为进攻阵营状态
			pRole->SetState(ES_CSAttack);
		}
		else
		{
			// 设置玩家为防守阵营状态
			pRole->SetState(ES_CSDefence);
		}
	}

	else
	{
		// 取消掉玩家的城战阵营状态
		pRole->UnsetState(ES_CSAttack);
		pRole->UnsetState(ES_CSDefence);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 改变禁城门模型
//-------------------------------------------------------------------------------------
static int SC_ChangeCityDoor(lua_State* L)
{
	DWORD	dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwTypeID	=	(DWORD)lua_tonumber(L, 2);

	const tagMapInfo* pInfo = g_mapCreator.GetMapInfo(dwMapID);
	if (!P_VALID(pInfo))	return 0;

	tagMapDoor* pMapDoor = NULL;
	pInfo->mapDoor.ResetIterator();
	while (pInfo->mapDoor.PeekNext(pMapDoor))
	{
		pMapDoor->dwTypeID = dwTypeID;
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
// 初始化城战玩家战绩
//-------------------------------------------------------------------------------------
static int SC_InitCSRoleData(lua_State* L)
{
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 1);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().InitCSRoleData();
	return 0;
}

//-------------------------------------------------------------------------------------
// 增加一条城战玩家战绩记录
//-------------------------------------------------------------------------------------
static int SC_AddRoleDataInfo(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().AddRoleDataInfo(dwRoleID);
	return 0;
}

//-------------------------------------------------------------------------------------
// 增加玩家击杀数
//-------------------------------------------------------------------------------------
static int SC_AddKillRoleNum(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwTargetID		=	(DWORD)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().AddKillRoleNum(dwRoleID, dwTargetID);
	return 0;
}

//-------------------------------------------------------------------------------------
// 改变一条玩家战绩数据
//-------------------------------------------------------------------------------------
static int SC_AddRoleData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 4);
	INT		nType			=	(INT)lua_tointeger(L, 5);
	DWORD	dwData			=	(DWORD)lua_tointeger(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().AddRoleData(dwRoleID, (ECityStruggleRoleData)nType, dwData);

	return 0;
}

//-------------------------------------------------------------------------------------
// 获得玩家战绩积分
//-------------------------------------------------------------------------------------
static int SC_GetRoleCSScore(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	tagCSRoleData* pData = pCity->GetCityStruggle().GetRoleDataPtr(dwRoleID);
	if (P_VALID(pData))
	{
		lua_pushnumber(L, pData->RoleData.nScore);
		return 1;
	}

	return 0;

}

//-------------------------------------------------------------------------------------
// 城战结束玩家战绩排序
//-------------------------------------------------------------------------------------
static int SC_SortByScore(lua_State* L)
{
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 1);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().SortByScore();

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取玩家城战击杀人数
//-------------------------------------------------------------------------------------
static int SC_GetRoleKillNum(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	tagCSRoleData* pData = pCity->GetCityStruggle().GetRoleDataPtr(dwRoleID);
	if (P_VALID(pData))
	{
		lua_pushnumber(L, (DWORD)pData->RoleData.byKills);
		return 1;
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取城战战绩排名玩家ID
//-------------------------------------------------------------------------------------
static int SC_GetCSRankedRoleID(lua_State* L)
{
	DWORD	dwCityID	=	(DWORD)lua_tonumber(L, 1);
	INT32	nRankNum	=	lua_tointeger(L, 2);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	if (nRankNum < 1)
		return 0;

	INT32 nSize = pCity->GetCityStruggle().GetRankData().size();
	lua_newtable(L);
	for (int n=0; n<nRankNum && n<nSize; ++n)
	{
		DWORD dwRoleID = (pCity->GetCityStruggle().GetRankData())[n].first;
		lua_pushnumber(L, dwRoleID);
		lua_rawseti(L, -2, n+1);
	}

	return 1;
}

//-------------------------------------------------------------------------------------
// 设置城战结果
//-------------------------------------------------------------------------------------
static int SC_SetCityStruggleResult(lua_State* L)
{
	DWORD	dwCityID			=	(DWORD)lua_tonumber(L, 1);
	ECityStruggleResult eResult	=	(ECityStruggleResult)lua_tointeger(L, 2);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().SetResult(eResult);

	return 0;
}

//-------------------------------------------------------------------------------------
// 召唤守护神人数改变
//-------------------------------------------------------------------------------------
static int SC_SummonPersonChange(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT32	nPerson			=	lua_tointeger(L, 4);
	INT32	nNeedPerson		=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_SummonPersonChange send;
	send.nPerson = nPerson;
	send.nNeedPerson = nNeedPerson;
	return 0;
}

//-------------------------------------------------------------------------------------
// 开始守护神召唤倒计时
//-------------------------------------------------------------------------------------
static int SC_SetSummonTime(lua_State* L)
{
	DWORD	dwCityID	=	(DWORD)lua_tonumber(L, 1);
	INT32	nCountDown	=	lua_tointeger(L, 2);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().SetSummonTime(nCountDown);
	return 0;
}

//-------------------------------------------------------------------------------------
// 终止召唤
//-------------------------------------------------------------------------------------
static int SC_StopSummon(lua_State* L)
{
	DWORD	dwCityID	=	(DWORD)lua_tonumber(L, 1);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().StopSummon();
	return 0;
}

// Jason 2010-1-19 v1.3.2 玩家离线时间

static int SC_GetPlayerOfflineTime(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	DWORD sec = 0;
	if( P_VALID(pRole) )
	{
		sec = pRole->GetOfflineTime  ();
	}
	lua_pushnumber(L,sec);
	return 1;
}

// Jason 2010-1-27 v1.3.2 账号离线时间
static int SC_GetPlayerAccountOfflineTime(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	DWORD sec = 0;
	if( P_VALID(pRole) )
	{
		PlayerSession * pSession = pRole->GetSession  ();
		if( P_VALID(pSession) )
		{
			sec = g_PlayerAccountMgr.GetOfflineTime  (pSession->GetSessionID());
		}
	}
	lua_pushnumber(L,sec);
	return 1/*SC_GetPlayerOfflineTime(L)*/;
}
static int SC_CanRecAccountRegressionReward(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	BOOL sec = FALSE;
	if( P_VALID(pRole) )
	{
		//sec = pRole->GetOfflineTime  ();
		PlayerRegression * p = TPlayerRegression::Instance  ();
		if( P_VALID(p) )
		{
			if( p->CanRecRegression  (pRole) )
			{
				sec = TRUE;
				lua_pushboolean(L,1);
				return 1;
			}
		}
	}
	lua_pushboolean(L,0);
	return 1;
}
static int SC_SetRecAccountRegressionRewardFlag(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	bReced			=	(DWORD)lua_tonumber(L, 2);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	DWORD sec = 0;
	if( P_VALID(pRole) )
	{
		//sec = pRole->GetOfflineTime  ();
		PlayerSession * pSession = pRole->GetSession  ();
		if( P_VALID(pSession) )
		{
			if( 1 == bReced )
				g_PlayerAccountMgr.ReceiveRegressionRewardTime(pSession->GetSessionID  (),g_world.GetWorldTime  ());
			else if( 0 == bReced )
				g_PlayerAccountMgr.ReceiveRegressionRewardTime(pSession->GetSessionID  (),0);
		}
	}
	return 0;
}

int SC_OpenFramePage(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT nType = (INT)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		tagNS_OpenFramePage send;
		send.eType = (EFrameType)nType;
		pRole->SendMessage(&send, send.dwSize);
	}

	return 0;
}
int SC_RemoveSkill(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwSkillBigID	=	(DWORD)lua_tonumber(L, 2);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	DWORD sec = 0;
	if( P_VALID(pRole) )
	{
		//char buf[20] = {0};
		//sprintf(buf,"%u",dwSkillBigID);
		//if( strlen(buf) >= 7 )
		//{
		//	buf[7] = 0;
		//	INT smallID = atoi(buf);
			pRole->RemoveSkill  (dwSkillBigID);
			lua_pushnumber(L,1);
			return 1;
		//}
	}
	lua_pushnumber(L,0);
	return 1;
}

int SC_GetConsumptiveStrength(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( P_VALID(pRole) )
	{
		INT nTmp = pRole->GetStrength().nConsumptiveStrength;
		lua_pushinteger(L, nTmp);
	}

	return 1;
}

int SC_SetConsumptiveStrength(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT nValue =	(INT)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( P_VALID(pRole) )
	{
		pRole->SetConsumptiveStrength(nValue);	
	}

	return 0;
}

int SC_GetRoleMapID(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( P_VALID(pRole) )
	{	
		Map* pMap = pRole->GetMap();
		if(!P_VALID(pMap))
		{
			return 0;
		}

		DWORD dwMapID = pRole->GetMapID();
		DWORD dwInstanceID = pMap->GetInstanceID();
	
		lua_pushnumber(L, dwMapID);
		lua_pushnumber(L, dwInstanceID);

		return 2;
	}

	return 0;
}

// Jason 2010-3-16 v1.5.0
// role lib
/**
* 增加玩家赠点接口
* \param mapid 
* \param instantid 
* \param roleid 角色id 
* \param points 赠点，正增加，负减少
* \return 成功返回true，否则返回false
*/
//BOOL AddRoleGiftPoints(DWORD mapid, DWORD instantid, DWORD roleid, int points)

int SC_AddRoleGiftPoints(lua_State * L)
{
	DWORD dwMapID			= (DWORD)lua_tonumber(L, 1);
	DWORD dwMapInstantID	= (DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID			= (DWORD)lua_tonumber(L, 3);
	INT		nGiftPoints		= (INT)	 lua_tointeger(L,4);

	BOOL re = FALSE;
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( P_VALID(pRole) && nGiftPoints != 0 )
	{	
		Map* pMap = pRole->GetMap();
		if(!P_VALID(pMap))
		{
			lua_pushnumber(L,re);
			return 1;
		}

		if( nGiftPoints > 0 )
			pRole->GetCurMgr().IncExchangeVolume(nGiftPoints,ELCLD_MALL_LUA_AddGiftPoint);
		else
			pRole->GetCurMgr().DecExchangeVolume(nGiftPoints,ELCLD_MALL_GiftPointCosted);
		re = TRUE;
	}

	lua_pushnumber(L,re);

	return 1;
}

static int SC_Trace(lua_State * L)
{
	ITRACE(lua_tostring(L, 1));
	return 0;
}


//-------------------------------------------------------------------------------------
// 获得玩家元宝数
//-------------------------------------------------------------------------------------
static int SC_GodMiraclePoints(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT32  points = pRole->GetGodMiraclePoints();
	lua_pushnumber(L, points);

	return 1;
}
static int SC_ModifyMiraclePoints(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT   points		=	(INT)lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	pRole->GodMiraclePoints(points);
	lua_pushboolean(L, 1);

	return 1;
}

// 给客户端发送城战个人战绩信息
static int SC_SendCityStruggleRecord2Role(lua_State* L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	pRole->SendCityStruggleRecord();

	return 0;
}
static int SC_PullCreatureToPos(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	FLOAT   x		=	(FLOAT)lua_tonumber(L, 4);
	FLOAT   y		=	(FLOAT)lua_tonumber(L, 5);
	FLOAT   z		=	(FLOAT)lua_tonumber(L, 6);

	x *= TILE_SCALE;
	y *= TILE_SCALE;
	z *= TILE_SCALE;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if(!P_VALID(pCreature))
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	pCreature->PulledToPos(Vector3(x,y,z));
	lua_pushboolean(L, 1);

	return 1;
};


static int SC_GetRoleGuildAtt(lua_State* L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	INT   nGuildAttID		=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}
	DWORD dwGuildID = pRole->GetGuildID();
	if( !P_VALID(dwGuildID) || EGAN_NULL >= nGuildAttID || nGuildAttID >= EGAN_END )
	{
		return 0;
	}

	Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
	if( !P_VALID(pGuild) )
	{
		return 0;
	}
	const tagGuild & att = pGuild->GetGuildAtt();

	int r = 1;

	switch(nGuildAttID)
	{
	case EGAN_LeaderID:
		lua_pushnumber(L,att.dwLeaderRoleID);
		break;
	case EGAN_Level:
		lua_pushnumber(L,att.byLevel);
		break;
	case EGAN_SpecState:
		lua_pushnumber(L,att.dwSpecState);
		break;
	case EGAN_Fund:
		lua_pushnumber(L,att.nFund);
		break;
	case EGAN_Material:
		lua_pushnumber(L,att.nMaterial);
		break;
	case EGAN_Reputation:
		lua_pushnumber(L,att.nReputation);
		break;
	case EGAN_GroupPurchase:
		lua_pushnumber(L,att.nGroupPurchase);
		break;
	case EGAN_Peace:
		lua_pushnumber(L,att.n16Peace);
		break;
	case EGAN_MemberNum:
		lua_pushnumber(L,att.n16MemberNum);
		break;
	case EGAN_Rank:
		lua_pushnumber(L,att.n16Rank);
		break;
	case EGAN_MinJoinLevel:
		lua_pushnumber(L,att.byMinJoinLevel);
		break;
	case EGAN_Commendation:
		lua_pushnumber(L,att.bCommendation);
		break;
	case EGAN_DailyCost:
		lua_pushnumber(L,att.nDailyCost);
		break;
	default:
		r = 0;
		break;
	}

	return r;
}
static int SC_GetSoarValue(lua_State* L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	lua_pushnumber(L,pRole->GetBaseAttValue(ERA_Soar));

	return 1;
}
static int SC_SetSoarValue(lua_State* L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwVal		=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;// 告诉lua返回nil
	}

	pRole->SetSoaringAtt(dwVal);
	lua_pushboolean(L,1);

	return 1;
}
static int SC_GetGuildAtt(lua_State* L)
{
	DWORD   dwGuildID		=	(DWORD)lua_tonumber(L, 1);

	if( !P_VALID(dwGuildID) )
		return 0;

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if( !P_VALID(pGuild) )
		return 0;

	const tagGuild & att = pGuild->GetGuildAtt();
	lua_pushnumber(L,att.byLevel);

	return 1;
}

static int SC_SyncFlagAttack(lua_State* L)
{
	DWORD		dwGodID						= (DWORD)lua_tonumber(L, 1);// 神迹类型
	DWORD	    dwFlagType					= (DWORD)	lua_tonumber(L, 2);// 指定旗子
	DWORD	    dwGuildCapType				= (DWORD)lua_tonumber(L, 3);// 旗子所属
	DWORD		dwMapID						= (DWORD)lua_tonumber(L, 4);
	DWORD		dwInstanceID				= (DWORD)lua_tonumber(L, 5);
	BOOL		bMapMessage					= (BOOL)lua_tonumber(L, 6);
	INT 		nFlagPosX 					= (INT)lua_tonumber(L, 7);
	INT			nFlagPosY					= (INT)lua_tonumber(L, 8);
	INT			nFlagPosZ 					= (INT)lua_tonumber(L, 9);
	EGodMiracle	eGodID						= (EGodMiracle)dwGodID;
	EFlagType	eFlagType					= (EFlagType)dwFlagType;
	EGuildCapType		eGuildCapType		= (EGuildCapType)dwGuildCapType;

	if (eGodID >= EGM_MAX || eGodID < EGM_NH)
		return 0;
	if (eFlagType < EFT_God || eFlagType > EFT_Monster)
		return 0;
	if (eGuildCapType < EGCT_NULL || eGuildCapType > EGCT_Capture)
		return 0;

	tagNS_SyncFlagAttack send;
	send.eFlagType = eFlagType;
	send.eGuildCapType = eGuildCapType;
	send.eGodID = eGodID;
	send.nFlagPosX = nFlagPosX;
	send.nFlagPosY = nFlagPosY;
	send.nFlagPosZ = nFlagPosZ;
	if (bMapMessage)
	{
		Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
		if(P_VALID(pMap))
			pMap->SendMapMessage(&send, send.dwSize);
	}
	else
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwMapID);
		if (P_VALID(pRole))
			pRole->SendMessage(&send, send.dwSize);
	}
	return 0;
}

static int SC_SendRoleSwitchMsg(lua_State* L)
{
	DWORD ndx		= (DWORD)lua_tonumber(L, 1);
	DWORD dwMapID	= (DWORD)lua_tonumber(L, 2);
	DWORD dwInsID	= (DWORD)lua_tonumber(L, 3);
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 4);
	DWORD dwAppID	= (DWORD)lua_tonumber(L,5);
	DWORD dwActID	= (DWORD)lua_tonumber(L,6);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	const tagMsgDeliver * pMsgDeliver = g_attRes.GetOneMsgDeliverProto(ndx);
	if( P_VALID(pMsgDeliver) && P_VALID(pRole) )
	{
		//DWORD dwMsgID		= pMsgDeliver->dwIDMsgID;
		//DWORD dwMsgInfoID	= (DWORD)g_MsgInfoMgr.BeginMsgEvent();
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, (EMsgUnitType)dwMsgID, NULL);
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->X));
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->Y));
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->Z));
		//g_MsgInfoMgr.DispatchRoleMsgEvent(dwMsgInfoID, pRole);
		tagNS_PullRoleMsg send;
		send.dwAppID = dwAppID;
		send.dwActID = dwActID;
		send.dwIdMsgID	= pMsgDeliver->dwIDMsgID;
		send.dwMapID	= pMsgDeliver->dwMapID;
		send.x			= pMsgDeliver->X;
		send.y			= pMsgDeliver->Y;
		send.z			= pMsgDeliver->Z;
		pRole->SendMessage(&send,send.dwSize);
	}
	return 0;
}

static int SC_SendWorldSwitchMsg(lua_State* L)
{
	struct tagComp
	{
		INT m_nLevel;
		tagComp()
			:m_nLevel(0){}
		bool operator()(Role *pRole,int minilevel,int maxlevel)
		{
			if( P_VALID(pRole) )
			{
				m_nLevel = pRole->GetLevel();
				if( ( GT_INVALID == minilevel || (m_nLevel >= minilevel) ) &&
					( GT_INVALID == maxlevel || (m_nLevel <= maxlevel) ) )
				{
					return true;
				}
			}
			return false;
		}
	};
	//struct tagSafeFree
	//{
	//	LPVOID m_p;
	//	DWORD m_id;
	//	tagSafeFree(LPVOID p,DWORD id)
	//		:m_p(p),m_id(id){}
	//	~tagSafeFree()
	//	{
	//		if( P_VALID(m_p) && P_VALID(m_id) )
	//			g_MsgInfoMgr.DiscardMsg(m_id,m_p);
	//	}
	//};
	struct SendMsg
	{
		SendMsg(PVOID pMsg, DWORD dwSize)
			:m_pMsg(pMsg), m_dwSize(dwSize){}
		VOID operator()(Role* pRole)
		{
			if (P_VALID(pRole))
			{
				pRole->SendMessage(m_pMsg, m_dwSize);
			}
		}
		PVOID m_pMsg;
		DWORD m_dwSize;
	};


	DWORD ndx		= (DWORD)lua_tonumber(L, 1);
	int minlevel	= (int)lua_tonumber(L, 2);
	int maxlevel	= (int)lua_tonumber(L, 3);
	DWORD dwAppID	= (DWORD)lua_tonumber(L, 4);
	DWORD dwActID	= (DWORD)lua_tonumber(L,5);

	const tagMsgDeliver * pMsgDeliver = g_attRes.GetOneMsgDeliverProto(ndx);
	if( P_VALID(pMsgDeliver) )
	{
		//DWORD dwMsgID		= pMsgDeliver->dwIDMsgID;
		//DWORD dwMsgInfoID	= (DWORD)g_MsgInfoMgr.BeginMsgEvent();
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, (EMsgUnitType)dwMsgID, NULL);
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->X));
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->Y));
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->Z));
		//BYTE * pData;
		//INT size ;
		//g_MsgInfoMgr.GetDispatchMsg(dwMsgInfoID,pData,size);
		tagNS_PullRoleMsg send;
		send.dwAppID = dwAppID;
		send.dwActID = dwActID;
		send.dwIdMsgID	= pMsgDeliver->dwIDMsgID;
		send.dwMapID	= pMsgDeliver->dwMapID;
		send.x			= pMsgDeliver->X;
		send.y			= pMsgDeliver->Y;
		send.z			= pMsgDeliver->Z;
		//if( P_VALID(pData) && size )
		{
			//tagNetCmd * pCmd = (tagNetCmd*)pData;
			//tagSafeFree(pData,dwMsgInfoID);
			//if(pCmd->dwSize == size)
				g_roleMgr.ForEachSpec2(tagComp(),minlevel,maxlevel,SendMsg(&send,send.dwSize));
		}
	}
	return 0;
}

//static int SC_ModSpecialTargetValue(lua_State* L)
//{
//	DWORD	dwRoleID		= (DWORD)lua_tonumber(L, 1);
//	UINT16	u16QuestID		= (UINT16)lua_tonumber(L, 2);
//	BOOL	bValue			= (BOOL)lua_tonumber(L, 3);
//
//	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
//	if (!P_VALID(pRole))
//	{
//		return 0;
//	}
//
//	Quest* pQuest = pRole->GetQuest(u16QuestID);
//	if (!P_VALID(pQuest))
//	{
//		return 0;
//	}
//
//	pQuest->SetSpecTargetFlag(bValue);
//
//	tagNS_QuestSpecComplete send;
//	send.dwRoleID = dwRoleID;
//	send.u16QuestID = u16QuestID;
//	send.bFlag = bValue;
//	pRole->SendMessage(&send, send.dwSize);
//
//	return 0;
//}

static int SC_SetRoleClass(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	EClassType   eClass		=	(EClassType)(DWORD)lua_tonumber(L, 4);

	if(EV_Null>eClass || eClass >= EV_End) return 0;
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// 就职
	pRole->SetClass(eClass);

	// 洗天资
	pRole->ClearTalent((INT64)0, FALSE);

	tagNS_RoleClassChangeBroadcast send;
	send.dwRoleID	= pRole->GetID();
	send.eClass = pRole->GetClass();
	pMap->SendBigVisTileMsg(pRole, &send, send.dwSize);
	lua_pushboolean(L,1);
	return 1;
}

static int SC_GetRoleClass(lua_State* L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	EClassType   eClass	 = pRole->GetClass();

	lua_pushnumber(L, eClass);

	return 1;
}

static int SC_GetRoleCompleteBoardQuestNum(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	BYTE		byType			= (BYTE)lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	EQuestBoardType eQuestBoardType = static_cast<EQuestBoardType>(byType);
	if (!P_VALID(pRole->GetBoardQuestMgr(eQuestBoardType)))
		return 0;

	INT nNum = pRole->GetBoardQuestMgr(eQuestBoardType)->GetCompleteReFreshTime();
	lua_pushinteger(L,nNum);
	return 1;
}

static INT SC_GetRoleYaojingLevel(lua_State * L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagRemoteRoleFabaoInfo info;

	pRole->SynRemoteBodyFabaoInfo(info);
	if( info.byEquipFabaoFlag )
	{
		lua_pushinteger(L,info.byStage);
		
	}
	else
	{
		lua_pushinteger(L,0);
	}
	
	return 1;
}

static int SC_ModSpecialTargetValue(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16QuestID		=	(UINT16)lua_tonumber(L, 4);
	BYTE	byValue			=	(BYTE)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	const tagQuestProto* pQuestProto = g_questMgr.GetQuestProto(u16QuestID);
	if(!P_VALID(pQuestProto)) return 0;

	if (!pQuestProto->spec_condition) return 0;
	
	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if (!P_VALID(pQuest)) return 0;

	tagNS_RoleSpecQuestCompleteState send;
	send.dwRoleID = dwRoleID;
	send.u16QuestID = u16QuestID;
	send.byState = byValue;
	pRole->SendMessage(&send, send.dwSize);
	return 0;
}

static INT SC_PutDownItem(lua_State * L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwItemID		=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	tagItem* pItem = pRole->GetItemMgr().GetItemByTypeID(dwItemID);
	if (!P_VALID(pItem)) return 0;

	INT64 n64GroupID = 0;
	pRole->LootItem(pItem->n64Serial, n64GroupID);

	push_64bit_data(L, n64GroupID);

	return 2;
}

// 夺宝奇兵脚本接口
static int SC_SyncOpenChaseRemainTime(lua_State* L)
{
	DWORD   dwRoleID				=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwOpenChaseRemainTime	=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	tagNS_SyncOpenChaseRemainTime send;
	send.dwOpenChaseRemainTime = dwOpenChaseRemainTime;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

static int SC_ForceLootChase(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	tagNS_ForceLootChase send;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

static int SC_GetCurMoveState(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	lua_pushnumber(L, pRole->GetMoveData().GetCurMoveState());
	return 1;
}

static int SC_MoveRole2AnotherMap(lua_State* L)
{
	DWORD   dwCurMapID = (DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID = (DWORD)lua_tonumber(L, 2);
	DWORD   dwDestMapID = (DWORD)lua_tonumber(L, 3);
	float   fX = (float)lua_tonumber(L, 4);
	float   fY = (float)lua_tonumber(L, 5);
	float   fZ = (float)lua_tonumber(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwCurMapID, dwInstanceID);
	if (!P_VALID(pMap)) return 0;


	Map::RoleMap::TMapIterator itRole = pMap->GetRoleMap().Begin();
	Role* pRole = NULL;
	while (pMap->GetRoleMap().PeekNext(itRole, pRole))
	{
		if (!P_VALID(pRole))
			continue;
		pRole->GotoNewMap(dwDestMapID, fX, fY, fZ);
	}
	return 0;
}

static int SC_SetMonsterGenState(lua_State* L)
{
	DWORD		dwCurMapID = (DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID = (DWORD)lua_tonumber(L, 2);
	DWORD		dwState = (DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwCurMapID, dwInstanceID);
	if (!P_VALID(pMap)) return 0;

	pMap->GetMonsterGen().SetState((EMonsterGenState)dwState);

	return 1;
}

static int SC_GetTrialCompleteNum(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	DWORD   dwTrainID = (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		lua_pushnumber(L, 0);
		return 1;
	}

	lua_pushnumber(L, pRole->GetTrialCompleteNum(dwTrainID));
	return 1;
}

static int SC_SetTrialCompleteNum(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	DWORD   dwTrainID = (DWORD)lua_tonumber(L, 2);
	DWORD	dwCompleteness = (DWORD)lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	pRole->SetTrainCompleteness(dwTrainID, dwCompleteness);

	return 0;
}

static int SC_TrialComplete(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	DWORD   dwTrainID = (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	pRole->CompleteTrain(dwTrainID);

	return 0;
}

static int SC_GetTrialState(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	DWORD   dwTrainID = (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	lua_pushnumber(L, pRole->GetTrainState(dwTrainID));

	return 1;
}

static int SC_GetFriendNum(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		lua_pushnumber(L, 0);
		return 1;
	}

	lua_pushnumber(L, pRole->GetFriendCount());

	return 1;
}

static int SC_GetEquipIDOnPos(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT16       nPos = (INT16)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	if (nPos < EEP_Equip_Start || nPos > EEP_End)
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(nPos);
	if (!P_VALID(pEquip))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	lua_pushnumber(L, pEquip->dwTypeID);
	return 1;
}

static int SC_GetEquipLvLimit(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}
	
	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 1;
		}
	}
	BYTE byUseLvlLimit = 0;
	if (P_VALID(pEquip->pEquipProto))
	{
		if (pEquip->IsFaBao())
		{
			tagFabao* pFabao = (tagFabao*)pEquip;
			if (P_VALID(pFabao))
				byUseLvlLimit = pFabao->nUseLevel;
		}
		else
		{
			byUseLvlLimit = pEquip->pEquipProto->byMinUseLevel;
		}
	}

	lua_pushnumber(L, byUseLvlLimit);
	return 1;
}

static int SC_GetEquipQlty(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 1;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	lua_pushnumber(L, pEquip->equipSpec.byQuality);//pEquip->pEquipProto->byQuality);
	return 1;
}

static int SC_GetEquipStrengthLevel(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 1;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	lua_pushnumber(L, pEquip->equipSpec.byConsolidateLevel);
	return 1;
}

static int SC_SetEquipStrengthLevel(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	DWORD   dwLevel = (DWORD)lua_tonumber(L, 4);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 0;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}
	if (!pEquip->pEquipProto->bOnlyScriptStrengthen)
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	ItemCreator::RecalEquipStrengthAtt(pEquip,dwLevel);
	pEquip->equipSpec.byConsolidateLevel = dwLevel;
	pRole->CalEquipFlare(pEquip);
	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 1;
}


static int SC_SetGrowEquipLevel(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	DWORD   dwGrowNum = (DWORD)lua_tonumber(L, 4);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 0;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}
	if (!pEquip->pEquipProto->bOnlyScriptStrengthen)
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}
	const tagEquipGrowProto* pGrowProto = g_attRes.GetEquipGrowProto(pEquip->equipSpec.dwGrowTypeID);
	if (!P_VALID(pGrowProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}


	// 统计各种颜色的星
	int nGray=0, nGreen=0, nBlue=0, nPurple = 0;
	pEquip->equipSpec.dwGrowValue += dwGrowNum;
	++pEquip->equipSpec.nTotalGrowTimes;

	// 检查是否可升级
	float fpct = 1.0f;
	DWORD dwValueLine = 0;
	for (int i=0; i<pEquip->equipSpec.byGrowLevel+1; ++i)
	{
		dwValueLine += pGrowProto->dwExp[i]*fpct;
		//dwValueNow = (pEquip->equipSpec.dwGrowValue > dwValueLine) ? 0 : (dwValueLine - pEquip->equipSpec.dwGrowValue);

		// 可升级
		if (pEquip->equipSpec.byGrowLevel <= i && pEquip->equipSpec.dwGrowValue >= dwValueLine)
		{
			EEquipGrowStar eNewStar = (EEquipGrowStar)pRole->OnRoleEquipGrow(pEquip->pProtoType->dwTypeID, pEquip->equipSpec.byQuality, pEquip->equipSpec.nTotalGrowTimes,
				pEquip->equipSpec.nTotalIMUseTimes, pEquip->equipSpec.byGrowLevel+1, nGray, nGreen, nBlue, nPurple, 0);

			if (!IsGrowStarValid(eNewStar))
			{
				DWORD dwError = GT_INVALID;
				lua_pushnumber(L, dwError);
				return 0;
			}

			++(pEquip->equipSpec.byGrowLevel);

			if (pEquip->equipSpec.byGrowLevel < EQUIP_GROW_MAX_LEVEL)
			{
				pEquip->equipSpec.eGrowStar[pEquip->equipSpec.byGrowLevel-1] = eNewStar;
				switch(eNewStar)
				{
				case EEGS_Gray:
					++nGray;
					break;
				case EEGS_Green:
					++nGreen;
					break;
				case EEGS_Blue:
					++nBlue;
					break;
				case EEGS_Purple:
					++nPurple;
					break;
				default:
					break;
				}
			}		
			else if(pEquip->equipSpec.byGrowLevel == EQUIP_GROW_MAX_LEVEL)
			{
				pEquip->equipSpec.dwGrowValue = dwValueLine;
			}
			
		}
	}

	// 增加成长属性
	for (int i=0; i<EQUIP_GROW_ADD_ATT_MAX_NUM; ++i)
	{
		if (P_VALID(pGrowProto->byAttOpenLevel[i]) && pEquip->equipSpec.byGrowLevel + 1 >= pGrowProto->byAttOpenLevel[i])
		{
			DWORD dwAttValue = 0;
			// 累积之前各级的属性和
			for (int j=0; j<pEquip->equipSpec.byGrowLevel; ++j)
			{
				float fpct = 1.0f;
				if(j>0) // 有星的时候
					MGetEquipGrowRatio(pEquip->equipSpec.eGrowStar[j-1], fpct);

				dwAttValue += pGrowProto->dwAttValue[i][j]*fpct;
			}
			// 累加本级的属性值
			DWORD dwExpValue = 0;
			for (int k=0; k<pEquip->equipSpec.byGrowLevel; ++k)
			{
				dwExpValue += pGrowProto->dwExp[k];
			}
			float fpct1 = 1.0f;
			if(pEquip->equipSpec.byGrowLevel>0)
				MGetEquipGrowRatio(pEquip->equipSpec.eGrowStar[pEquip->equipSpec.byGrowLevel-1], fpct1);
			dwAttValue += (pEquip->equipSpec.dwGrowValue - dwExpValue)*fpct1*(pGrowProto->dwAttValue[i][pEquip->equipSpec.byGrowLevel])/(pGrowProto->dwExp[pEquip->equipSpec.byGrowLevel]);

			pEquip->equipSpec.nGrowAttValue[i] = dwAttValue;
		}
	}

	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 1;
}
static int SC_DigHoleOnEquip(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 0;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}
	if (!pEquip->equipSpec.bCanCut || pEquip->equipSpec.byHoleNum >= MAX_EQUIPHOLE_NUM)
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	++(pEquip->equipSpec.byHoleNum);
	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);
	// 触发脚本
	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript))
		pRoleScript->OnDigHoleOnEquip(pRole, pEquip->dwTypeID, true, pEquip->equipSpec.byHoleNum);

	return 1;
}

static int SC_PutStoneOnEquip(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	DWORD   dwItemID = (DWORD)lua_tonumber(L, 4);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 0;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	// 找到强化数据
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(dwItemID);
	if(!P_VALID(pConsolidateProto))
		return 0;

	if((INT)pEquip->equipSpec.byHoleNum == 0)
		return 0;

	bool bFull = true;
	for(int i = 0; i < pEquip->equipSpec.byHoleNum;++i)
	{
		if( !P_VALID(pEquip->equipSpec.dwHoleGemID[i]) )
		{
			bFull = false;
			break;
		}
	}
	if( bFull )
		return 0;

	// 检测宝石是否能镶嵌到装备上
	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return 0;



	// 镶嵌宝石
	for(INT i = 0; i < (INT)pEquip->equipSpec.byHoleNum; ++i)
	{
		if(pEquip->equipSpec.dwHoleGemID[i] == GT_INVALID || pEquip->equipSpec.dwHoleGemID[i] == 0)
		{
			pEquip->equipSpec.dwHoleGemID[i] = dwItemID;
			break;
		}
	}

	pRole->CalEquipFlare(pEquip);
	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 1;
}


static int SC_SetEquipAdditionalAbility(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	DWORD   dwItemID = (DWORD)lua_tonumber(L, 4);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 0;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	 tagItemProto* pItemProto = g_attRes.GetItemProto(dwItemID);
	 if(!P_VALID(pItemProto))
	 {
		 DWORD dwError = GT_INVALID;
		 lua_pushnumber(L, dwError);
		 return 0;
	 }
	// 找到强化数据
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(dwItemID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	// 检测印记等级是否达到上限
	if((pEquip->equipSpec.byBrandLevel + pItemProto->byLevel) > MAX_BRAND_LEVEL)
		return E_Consolidate_Brand_TopLevel;

	// 检测装备是否能被印记
	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotBrand;

	// 检查装备潜力值是否足够
	if(pEquip->equipSpec.nPotVal < (INT)pConsolidateProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// 记录印记等级
	BYTE byLevel = pItemProto->byLevel;

	DWORD dwBrandItemTypeID = dwItemID;

	// 装备烙印等级(增加量为印记等级)
	pEquip->equipSpec.byBrandLevel += byLevel;

	// 装备增加对应强化属性
	for(INT n = 0; n < MAX_CONSOLIDATE_ROLEATT; ++n)
	{
		INT nIdx = 0;
		if (pConsolidateProto->tagRoleAtt[n].eRoleAtt >= ERA_Derate_Start &&
			pConsolidateProto->tagRoleAtt[n].eRoleAtt <= ERA_Derate_End)
		{
			nIdx = MTransERADerate2Index(pConsolidateProto->tagRoleAtt[n].eRoleAtt);
		}
		else if (pConsolidateProto->tagRoleAtt[n].eRoleAtt >= ERA_transform_Start &&
			pConsolidateProto->tagRoleAtt[n].eRoleAtt <= ERA_transform_End)
		{
			nIdx = MTransERATransform2Index(pConsolidateProto->tagRoleAtt[n].eRoleAtt);
		}
		pEquip->equipSpec.nBrandAtt[nIdx] += pConsolidateProto->tagRoleAtt[n].nAttVal;
	}


	pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume));


	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript))
		pRoleScript->OnBrandEquip(pRole, pEquip->dwTypeID, dwBrandItemTypeID, TRUE, pEquip->equipSpec.byBrandLevel);

	pRole->CalEquipFlare(pEquip);
	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 1;
}

static int SC_GetBuffWrapTimes(lua_State* L)
{
	// 添加者id
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	// buff作用者
	DWORD	dwBuffTypeID = (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	DWORD dwSrcID = GT_INVALID;
	INT times = pRole->GetBuffWrapTimes(dwBuffTypeID,dwSrcID);
	lua_pushnumber(L, times);
	lua_pushnumber(L, dwSrcID);
	return 2;
}

static int SC_IsFairyItem(lua_State* L)
{
	DWORD   dwItemTypeID = (DWORD)lua_tonumber(L, 1);
	tagItemProto* pProto = g_attRes.GetEquipProto(dwItemTypeID);
	bool bFairy = false;
	if( P_VALID(pProto) )
	{
		if( EIT_FaBao == pProto->eType )
			bFairy = true;
	}
	lua_pushboolean(L,bFairy);
	return 1;
}
static int SC_NotifyFBBS(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	int   nCmdID	= (int)lua_tonumber(L, 2);
	int   nPram		= (int)lua_tonumber(L, 3);
	g_FBBSSession.NotifyFBBS(dwRoleID,nCmdID,nPram);
	return 0;
}

static int SC_GetRoleQuestNum(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	lua_pushnumber(L, pRole->GetCurrentQuestCount());
	return 1;
}

static int SC_IsOldSerNewPlayer(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushboolean(L, false);
		return 1;
	}
	
	lua_pushboolean(L, pRole->IsOldSerNewPlayer());
	return 1;
}

//-------------------------------------------------------------------------------------
// 怪物是否有某buff
//-------------------------------------------------------------------------------------
int SC_IsHaveBuff(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwBuffID		=	(DWORD)lua_tonumber(L, 4);
	BOOL	bRet			=	FALSE;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if (!P_VALID(pCreature)) return 0;

	bRet = pCreature->IsHaveBuff(dwBuffID);
	lua_pushboolean(L, bRet);

	return 1;
}

//--------------------------------------------------------------------------------------
//	获取怪物的仇恨列表
//--------------------------------------------------------------------------------------
int SC_GetEnemyList(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwTargetID		=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwValue			=	(DWORD)lua_tonumber(L, 5);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	Unit* pTarget = pMap->FindUnit(dwTargetID);
	if( !P_VALID(pTarget) ) return 0;
	
	TMap<DWORD, DWORD> EnemyListMap;
	pCreature->GetAI()->GetEnemyList(EnemyListMap);
	if(EnemyListMap.Empty())
		return 0;

	EnemyListMap.ResetIterator();
	DWORD dwEnemyRoleID;
	DWORD dwEnemyVal;
	int nIndex = 0;
	lua_newtable(L);
	while(EnemyListMap.PeekNext(dwEnemyRoleID, dwEnemyVal))
	{
		lua_pushnumber(L, nIndex++);
		lua_pushnumber(L, dwEnemyRoleID);
		lua_settable(L, -3);
	}	

	return 1;
}

//-------------------------------------------------------------------------------------
//	设置仇恨列表中某个位置的人的仇恨值为多少多少
//-------------------------------------------------------------------------------------
int SC_SetEnemyValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwTargetID		=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwValue			=	(DWORD)lua_tonumber(L, 5);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	Unit* pTarget = pMap->FindUnit(dwTargetID);
	if( !P_VALID(pTarget) ) return 0;

	pCreature->GetAI()->SetEnmity(pTarget, dwValue);

	return 0;
}

int SC_GetCampByCreatureTid(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureTid	=	(DWORD)lua_tonumber(L, 3);
	ECampType eType = ECamp_Null;

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	TMap<DWORD, Creature*>& mapCreature = pMap->GetCreatureMap();

	TMap<DWORD, Creature*>::TMapIterator it = mapCreature.Begin();
	Creature* pCreature = NULL;
	while( mapCreature.PeekNext(it, pCreature) )
	{
		if (dwCreatureTid != pCreature->GetTypeID())
		{
			continue;
		}

		eType = pCreature->GetCampType();
		break;
	}

	lua_pushnumber(L, eType);

	return 0;
}

int SC_SetCampByCreatureTid(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureTid	=	(DWORD)lua_tonumber(L, 3);
	ECampType eType			=	(ECampType)lua_tointeger(L, 4);

	// 找到地图和怪物
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	TMap<DWORD, Creature*>& mapCreature = pMap->GetCreatureMap();

	TMap<DWORD, Creature*>::TMapIterator it = mapCreature.Begin();
	Creature* pCreature = NULL;
	while( mapCreature.PeekNext(it, pCreature) )
	{
		if (dwCreatureTid != pCreature->GetTypeID())
		{
			continue;
		}

		pCreature->SetCampType(eType);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 获得玩家神职
//-------------------------------------------------------------------------------------
int SC_GetRoleClergy(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	DWORD dwClergy = pRole->GetClergy();
	lua_pushnumber(L, dwClergy);
	
	return 1;
}
//-------------------------------------------------------------------------------------
static int SC_GetRoleFamilyID(lua_State* L)
{
	DWORD   dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	DWORD dwFamilyID = pRole->GetFamilyID();
	if (P_VALID(dwFamilyID))
	{
		lua_pushnumber(L, dwFamilyID);
		return 1;
	}
	else
	{
		return 0;
	}	
}

static int SC_GetFamilyMemberID(lua_State* L)
{
	DWORD   dwFamilyID		=	(DWORD)lua_tonumber(L, 1);

	Family* pFamily = g_FamilyMgr.GetFamily(dwFamilyID);
	if (!P_VALID(pFamily))
		return 0;

	FamilyMember& pMembers = pFamily->GetFamilyMember();
	
	DWORD dwCount = pMembers.GetFamilyMemberCnt();
	if(dwCount <= 0)
		return 0;

	DWORD MemberIDs[MAX_FAMILY_MEMBER] = {0};
	DWORD dwNum = pMembers.GetFamilyAllMember(MemberIDs);
	for (int i =0; i<dwNum; ++i)
	{
		lua_pushnumber(L, MemberIDs[i]);
	}
	
	return dwNum;
}

static int SC_GetFamilyLeaderID(lua_State* L)
{
	DWORD   dwFamilyID		=	(DWORD)lua_tonumber(L, 1);

	Family* pFamily = g_FamilyMgr.GetFamily(dwFamilyID);
	if (!P_VALID(pFamily))
		return 0;
	
	lua_pushnumber(L, pFamily->GetFamilyAtt().dwLeaderID);
	
	return 1;
}

static int SC_GetRoleLoverID(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	DWORD dwLoverID = pRole->GetLoverID();
	lua_pushnumber(L, dwLoverID);

	return 1;
}

static int SC_AddRoleAwardData(lua_State* L)
{
	DWORD   dwRoleID =	(DWORD)lua_tonumber(L, 1);
	INT		nData =	(int)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) 
		return 0;

	pRole->AddRoleAwardPoint(nData);

	return 0;
}

static int SC_GetWardrobeDetailInfo(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	INT nSum = 0; // 气质值和
	INT nSuit1Num = 0; // 集齐的时装套数
	INT nSuit2Num = 0;
	INT nSuit3Num = 0;
	INT nSuitSum = 0; // 时装总套数
	
	pRole->GetFashionTemperamentSum(nSum, nSuit1Num, nSuit2Num, nSuit3Num, nSuitSum);

	lua_pushnumber(L, nSum);
	lua_pushnumber(L, nSuit1Num);
	lua_pushnumber(L, nSuit2Num);
	lua_pushnumber(L, nSuit3Num);
	lua_pushnumber(L, nSuitSum);
	
	return 5;
}

static int SC_GetRoleAwardPoint(lua_State* L)
{
	DWORD   dwRoleID =	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) 
	{
		lua_pushnumber(L, 0);
	}
	else
	{
		lua_pushnumber( L, pRole->GetAwardPoint() );
	}

	return 1;
}

static int SC_MultiRateReward(lua_State* L)
{
	INT nType	= lua_tonumber(L, 1);
	INT nRate		= lua_tonumber(L, 2);
	DWORD dwLastSecond	=	(DWORD)lua_tonumber(L, 3);

	g_GMPolicy.SetRate(EDoubleType(nType), nRate, (DWORD)GetCurrentDWORDTime(), dwLastSecond);

	return 0;
}

static int SC_GetWorldDataTable(lua_State* L)
{
	return 0;
	INT nIndex	= lua_tonumber(L, 1);

	if (!DataCollection_Index_Valid(nIndex))
	{
		return 0;
	}

	lua_pushnumber(L,g_DataCollection.GetData((BYTE)nIndex));

	return 1;
}

static int SC_SetWorldDataTable(lua_State* L)
{
	return 0;
	INT nIndex	= lua_tonumber(L, 1);
	DWORD dwValue		= (DWORD)lua_tonumber(L, 2);

	if (!DataCollection_Index_Valid(nIndex))
	{
		return 0;
	}

	g_DataCollection.SetData((BYTE)nIndex, dwValue);

	return 0;
}

static int SC_GetRoleDataTable(lua_State* L)
{
	return 0;
	DWORD dwRoleID	= lua_tonumber(L, 1);
	INT nIndex	= lua_tonumber(L, 2);

	if (!DataCollection_Role_Index_Valid(nIndex))
	{
		return 0;
	}

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	lua_pushnumber(L, pRole->GetData((BYTE)nIndex));

	return 1;
}

static int SC_SetRoleDataTable(lua_State* L)
{
	return 0;
	DWORD dwRoleID	= lua_tonumber(L, 1);
	INT nIndex	= lua_tonumber(L, 2);
	DWORD dwValue		= (DWORD)lua_tonumber(L, 3);

	if (!DataCollection_Role_Index_Valid(nIndex))
	{
		return 0;
	}

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	pRole->SetData((BYTE)nIndex, dwValue);

	return 0;
}

static int SC_GetRoleSpecFlag(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	PlayerSession* pSession = pRole->GetSession();

	lua_pushnumber(L, pSession->GetAccountSpec());

	return 1;
}

static int SC_SetRoleSpecFlag(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	INT nValue	= lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	PlayerSession* pSession = pRole->GetSession();

	pSession->SetAccountSpec((EAccountSpec)nValue);

	return 0;
}

static int SC_GetAccountReactiveStatus(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->IsAccountReactiver());
	return 1;
}

static int SC_GetAccountReactiveKey(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetAccountReactiveKey());

	return 1;
}

static int SC_GetAccountReactiveScore(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetAccountReactiveScore());

	return 1;
}

static int SC_PopInputReactiveKey(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	PlayerSession* pSession = pRole->GetSession();
	if (P_VALID(pSession))
	{
		tagNS_PlsInputReactiveKeyCode send;
		pSession->SendMessage(&send, send.dwSize);
	}
	return 0;
}


static int SC_GetRoleSoul(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	lua_pushinteger(L, pRole->GetGod());
	lua_pushinteger(L, pRole->GetMonster());

	return 2;
}

static int SC_SetRoleSoul(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	INT nGod	= lua_tonumber(L, 4);
	INT nMonster	= lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	INT nCurGod = pRole->GetGod();
	INT nCurMonster = pRole->GetMonster();

	INT nTempGod = nGod - nCurGod;
	INT nTempMonster = nMonster - nCurMonster;

	pRole->ChangeSoul(nTempGod,nTempMonster);

	//pRole->SetGod(nGod);
	//pRole->SetMonster(nMonster);

	return 0;
}

static int SC_GetRoleSoulPoint(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	lua_pushinteger(L, pRole->GetGodPoint());
	lua_pushinteger(L, pRole->GetMonsterPoint());

	return 2;
}

static int SC_SetRoleSoulPoint(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	INT nGodPoint	= lua_tonumber(L, 4);
	INT nMonsterPoint	= lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	INT nCurGodPoint = pRole->GetGodPoint();
	INT nCurMonsterPoint = pRole->GetMonsterPoint();

	INT nTempGodPoint = nGodPoint - nCurGodPoint;
	INT nTempMonsterPoint = nMonsterPoint - nCurMonsterPoint;

	pRole->ChangeSoulPoint(nTempGodPoint,nTempMonsterPoint);
	//pRole->SetGodPoint(nGodPoint);
	//pRole->SetMonsterPoint(nMonsterPoint);

	return 0;
}

static int SC_GetAccountReactiveLevel(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);

	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	lua_pushinteger(L, pRole->GetRewardReactiveLevel());

	return 1;

}

static int SC_SetAccountReactiveLevel(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	DWORD dwLevel	= lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	pRole->SetRewardReactiveLevel((BYTE)dwLevel);

	return 0;
}

static int SC_GetRoleSoulCrystalLvl(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID = lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	DWORD dwSerialHigh = lua_tonumber(L, 4);
	DWORD dwSerialLow = lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	tagItem* pItem = pRole->GetItemMgr().GetEquipBar().GetItem((INT64)(dwSerialHigh*100000 + dwSerialLow));
	if (!P_VALID(pItem) || !MIsSoulCrystal(pItem))
	{
		return 0;
	}

	tagSoulCrystal* pSoul = (tagSoulCrystal*) pItem;
	
	lua_pushinteger(L, pSoul->SoulSpec.bySoulCrystalLvl);

	return 1;
}


static int SC_GetSkillActiveTime(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwSkillID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if(!P_VALID(pSkill))	
		return 0;

	DWORD dwTime = (DWORD)pSkill->GetActiveTime();
	lua_pushinteger(L, dwTime);

	return 1;
}

static int SC_SetSkillActiveTime(lua_State* L)
{
	DWORD dwMapID				=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID			=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID				=	(DWORD)lua_tonumber(L, 3);
	DWORD dwSkillID				=	(DWORD)lua_tonumber(L, 4);
	DWORD dwSkillActiveTime		=	(DWORD)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	//Skill* pSkill = pRole->GetSkill(dwSkillID);
	//if(!P_VALID(pSkill))	
	//	return 0;

	//pSkill->SetActiveTime(dwSkillActiveTime);

	pRole->ContinueSoulSkillByBook(dwSkillID,dwSkillActiveTime);

	return 0;
}

//-------------------------------------------------------------------------------------
// 发送dota相关消息给客户端
//-------------------------------------------------------------------------------------
static int SC_DotaSendMsg(lua_State* L)
{	
	DWORD dwMapID			= (DWORD)lua_tonumber(L, 1);
	DWORD dwMapInstanceID	= (DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID			= (DWORD)lua_tonumber(L, 3);
	DWORD dwTypeID			= (DWORD)lua_tonumber(L, 4);
	const char* pMsg		= lua_tostring(L, 5);
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwMapInstanceID);
	if(!P_VALID(pMap)) return 0;
	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole)) return 0;
	DWORD dwSize = sizeof(tagNS_DotaMsg)  + strlen(pMsg);
	MCREATE_MSG(pSend, dwSize, NS_DotaMsg);
	pSend->dwMsgType = dwTypeID;
	pSend->dwMsgSize = strlen(pMsg);
	strcpy(pSend->msg, pMsg);
	pSend->msg[pSend->dwMsgSize] = 0;
	pRole->SendMessage(pSend, pSend->dwSize);
	MDEL_MSG(pSend);
	return 0;
}

static int SC_GetEquipGrowAttID(lua_State* L)
{
	DWORD dwRoleID = lua_tonumber(L, 1);			//role ID
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);	//装备64位id

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);	//获取角色
	if(!P_VALID(pRole))
		return 0;

	tagEquip* pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(n64SerialID));
	if(!P_VALID(pEquip) || pEquip->equipSpec.dwGrowTypeID == 0 || pEquip->equipSpec.dwGrowTypeID == GT_INVALID)	//无效
	{
		return 0;
	}

	lua_pushinteger(L, pEquip->equipSpec.dwGrowTypeID);
	return 1;
}
static int SC_SetEquipGrowAttID(lua_State* L)
{

	DWORD dwRoleID = lua_tonumber(L, 1);			//role ID
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);	//装备64位id
	DWORD dwGrowID = lua_tonumber(L, 4);			//成长id

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);	//获取角色
	if(!P_VALID(pRole))
		return 0;

	tagEquip* pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(n64SerialID));
	if(!P_VALID(pEquip))	//无效
	{
		return 0;
	}

	//设置成长id并清空成长属性
	pEquip->equipSpec.dwGrowTypeID = dwGrowID;	// 成长属性ID
	pEquip->equipSpec.dwGrowValue = 0;			// 当前成长值
	pEquip->equipSpec.byGrowLevel = 0;			// 当前成长等级，0-11
	// nTotalGrowTimes;							// 总共成长过的次数		//这两个值暂不清除
	// nTotalIMUseTimes;						// 总共使用过IM道具的次数
	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
	{
		pEquip->equipSpec.eGrowStar[i] = EEGS_Null;	//星星
	}
	for(int i = 0; i < EQUIP_GROW_ADD_ATT_MAX_NUM; i++)
	{
		pEquip->equipSpec.nGrowAttValue[i] = 0;		//属性值
	}

	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 0;
}
static int SC_GetEquipStoneAddLevel(lua_State* L)	//获取宝石增幅值
{
	DWORD dwRoleID = lua_tonumber(L, 1);			//role ID
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);	//装备64位id

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);	//获取角色
	if(!P_VALID(pRole))
		return 0;

	tagEquip* pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(n64SerialID));
	if(!P_VALID(pEquip))	//无效
	{
		return 0;
	}
	lua_pushinteger(L, pEquip->equipSpec.byDiamondLevel);
	return 1;
}
static int SC_SetEquipStoneAddLevel(lua_State* L)
{
	DWORD dwRoleID		= lua_tonumber(L, 1);			//role ID
	INT64 n64SerialID	= pop_64bit_data(L, 2, 3);	//装备64位id
	BYTE byDiamondLevel = (BYTE)lua_tonumber(L, 4);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);	//获取角色
	if(!P_VALID(pRole))
		return 0;

	tagEquip* pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(n64SerialID));
	if(!P_VALID(pEquip))	//无效
	{
		return 0;
	}
	pEquip->equipSpec.byDiamondLevel = byDiamondLevel;

	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 0;
}

static int SC_AddMallMark(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nValue			=	lua_tointeger(L, 4);
	INT     nCmdID			=   lua_tointeger(L, 5);



	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	CurrencyMgr& currencyMgr = pRole->GetCurMgr();
	currencyMgr.IncVIPPoint(nValue, (ELogCmdID)nCmdID);	//1表示通过使用道具产生的积分

	return 0;
}

static int SC_SetGuildValue(lua_State* L)
{
	DWORD dwGuildID = lua_tonumber(L, 1);
	DWORD dwGuildValue1 = lua_tonumber(L, 2);
	DWORD dwGuildValue2 = lua_tonumber(L, 3);

	Guild* pGuild = NULL;
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if(!P_VALID(pGuild))
		return 0;

	pGuild->SetGuildValue1(dwGuildValue1);
	pGuild->SetGuildValue2(dwGuildValue2);

	g_guildMgr.SendAllGuild2DB();

	return 0;
}

static int SC_GetGuildValue(lua_State* L)
{
	DWORD dwGuildID = lua_tonumber(L, 1);

	Guild* pGuild = NULL;
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if(!P_VALID(pGuild))
		return 0;

	DWORD dwGuildValue1 = pGuild->GetGuildValue1();
	DWORD dwGuildValue2 = pGuild->GetGuildValue2();

	lua_pushinteger(L, dwGuildValue1);
	lua_pushinteger(L, dwGuildValue2);

	return 2;
}

//元神当前等级经验值上限的获取
static int SC_GetSoulMaxLevelExp(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	tagHolySoulInfo& holySoulInfo = holySoul.GetHolySoulInfo();
	INT64 n64MaxLevelExp = holySoulInfo.SoulAttEx.n64MaxExp;

	push_64bit_data(L, n64MaxLevelExp);
	return 2;

}

//累加元神经验
static int SC_AddHolySoulExp(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nAddendExp		=	(INT)lua_tointeger(L,4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if (nAddendExp <= 0)
	{
		return 0;
	}

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	DWORD dwErrorCode;
	dwErrorCode = holySoul.AddExp((INT64)nAddendExp);

	lua_pushnumber(L, dwErrorCode);
	return 1;
}

//获取元神的当前等级
static int SC_GetSoulCurLevel(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	tagHolySoulInfo& holySoulInfo = holySoul.GetHolySoulInfo();

// 	INT nLevel = holySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel];
// 	if (nLevel <= 0)
// 	{
// 		return 0;
// 	}
	//lua_pushnumber(L, nLevel);
	return 1;
}

//获取元神的转生等级
static int SC_GetHolySoulRebornLevel(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	tagHolySoulInfo& holySoulInfo = holySoul.GetHolySoulInfo();
// 	INT nRebornLevel = (INT)(holySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv] / 10000);
// 	if (nRebornLevel <= 0)
// 	{
// 		return 0;
// 	}
// 
// 	lua_pushnumber(L, nRebornLevel);
	return 1;
}

//获取元神的默契值
static int SC_GetCurCovalue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	INT nCurCovalue = holySoul.GetHolySoulAtt(EHEAT_Covalue);
	if (nCurCovalue < 0)
	{
		return 0;
	}

	lua_pushnumber(L, nCurCovalue);
	return 1;
}

//累加元神的默契值
static int SC_AddCurCovalue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nCoValue		=	lua_tointeger(L, 4);
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}
	holySoul.OnChangeAttVal(EHEAT_Covalue,nCoValue);
	return 0;
}

//获取元神值
static int SC_GetCurSoulValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	INT nCurSoulValue = holySoul.GetHolySoulAtt(EHEAT_SoulValue);
	if (nCurSoulValue < 0)
	{
		return 0;
	}

	lua_pushnumber(L, nCurSoulValue);
	return 1;
}

//累加元神值
static int SC_AddCurSoulValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nSoulValue		=	lua_tointeger(L, 4);
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	holySoul.OnChangeAttVal(EHEAT_SoulValue,nSoulValue);
	return 0;
}

//获取元神值的最大值
static int SC_GetMaxSoulValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	INT nMaxSoulValue = holySoul.GetHolySoulAtt(EHEAT_MaxSoulValue);
	if (nMaxSoulValue < 0)
	{
		return 0;
	}

	lua_pushnumber(L, nMaxSoulValue);
	return 1;
}

//获取元神当前的经验值
static int SC_GetSoulCurExp(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	INT64 n64CurExp = holySoul.GetCurrExp();
	if (n64CurExp < 0)
	{
		return 0;
	}

	push_64bit_data(L, n64CurExp);
	return 2;
}

static int SC_SetHunJingStrength(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT64	n64ItemID		=	pop_64bit_data(L, 4, 5);
	DWORD	dwStrength		=	(DWORD)lua_tonumber(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagEquip* pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64ItemID);
	if(P_VALID(pEquip))
	{
		if(MIsSoulCrystal(pEquip))
		{
			((tagSoulCrystal*)(pEquip))->SoulSpec.dwSoulStrength = dwStrength;
		}
	}
}

static int SC_SetWarGold(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nWarGold		=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	WarRole* pWarRole = pRole->GetWarRole();
	if(!P_VALID(pWarRole))	return 0;

	pWarRole->AddGlod(nWarGold);

	return 0;
}

static int SC_GetWarGold(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	WarRole* pWarRole = pRole->GetWarRole();
	if(!P_VALID(pWarRole))	return 0;

	pWarRole->GetGold();

	return 0;
}

//重新加载计算元神属性值,仅用于解决元神属性表改变后，玩家元神属性值不正确的bug
static int SC_ReloadHolySoulAtt(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
	{
		return 0;
	}

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
	{
		return 0;
	}

	HolySoul& holySoul = pRole->GetHolySoul();	
	if (!holySoul.IsActivated())
	{
		return 0;
	}

	//获得元神ID
	DWORD dwSoulID = holySoul.GetID();
	const tagSoulAttProto* pCurSoulAttProto = g_attRes.GetSoulAttProto(dwSoulID);
	if (!P_VALID(pCurSoulAttProto))
	{
		return 0;
	}

	//先于ReloadProcHolySoulAtt之前调用
	DWORD dwErrCode =holySoul.ReCalHolyChangeRoleAtt(pCurSoulAttProto,true);
	if (dwErrCode != E_Success)
	{
		return 0;
	}	

	dwErrCode = holySoul.ReloadProcHolySoulAtt(pCurSoulAttProto);
	if (dwErrCode != E_Success)
	{
		return 0;
	}	

	//更新到数据库
	holySoul.SaveHolySoulInfoToDB(HolySoul::ESaveType_Update);
	//同步客户端元神信息改变
	holySoul.SendHolySoulAttToClient();

	return 0;
}

