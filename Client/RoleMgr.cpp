#include "StdAfx.h"
#include "RoleMgr.h"
#include "Role.h"
#include "LocalPlayer.h"
#include "NPC.h"
#include "Door.h"
#include "Pet.h"
#include "PetManager.h"
#include "MapMgr.h"
#include "RoleEvent.h"
#include "CreatureData.h"
#include "PetProtoData.h"
#include "ShadowPool.h"
#include "SGDynamicProjector.h"
#include "..\WorldDefine\exchange_define.h"
#include "..\Cool3D\Util\CSVFile.h"
#include "OnlineTips.h"
#include "PetEvent.h"
#include "WorldPickerFrame.h"
#include "CombatSys.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_map.h"
#include "RoleLevelUpPromptFrame.h"
#include "..\WorldDefine\selectrole.h"
#include "PlayerNameTab.h"
#include "EffectMgr.h"
#include "CombatActionChecker.h"
#include "WorldPickerFrame.h"
#include "GameSet.h"
#include "ItemMgr.h"
#include "Item.h"
#include "DresserFrame.h"
#include "..\WorldDefine\msg_VoteShow_define.h"
#include "..\WorldDefine\msg_appearance_transform.h"
#include "DresserFrame.h"
#include "..\WorldDefine\msg_quest_board.h"
#include "..\WorldDefine\quest_board.h"
#include "..\WorldDefine\msg_pk.h"
#include "CombatSysUtil.h"
#include "QuestPanelFrame.h"
#include "ChatShowPop.h"
#include "NPCSayTable.h"
#include "KeyCodeFrame.h"
#include "..\WorldDefine\msg_shutdown.h"
#include "CountDownFrame.h"
#include "..\WorldDefine\msg_fashion.h"
#include "ClothespressData.h"
#include "SkillProtoData.h"
#include "..\WorldDefine\msg_god.h"
#include "SkillMgr.h"
#include "..\WorldDefine\msg_combat.h"
#include "..\WorldDefine\skill_define.h"
#include "FamilyEvent.h"
#include "..\WorldDefine\msg_family.h"
#include "TigerMachineFrame.h"
#include "ReputeMgr.h"
#include "..\WorldDefine\msg_func_npc.h"
#include "VirtualRole.h"
#include "HolyManMgr.h"
#include "FSM_RP.h"
#include "..\WorldDefine\msg_yuanshen.h"
#include "ItemProtoData.h"
#include "..\WorldDefine\msg_compose.h"
#include "SaveAccount.h"
#include "SendHttpPost.h"


const INT32 MAX_EXCHANGE_DIST = 10 * TILE_SCALE;	// 玩家交互（包括和玩家，NPC，摊位的情况）最远距离
const INT32 MAX_TEAM_DIST = 100 * TILE_SCALE;	// 组队增益的有效距离
const INT32 MAX_FACE_DIST_SQ = 400 * TILE_SCALE * TILE_SCALE;
const INT32 MIN_FACE_DIST_SQ = 1 * TILE_SCALE * TILE_SCALE;

RoleMgr::RoleMgr(void):m_trunk(this)
{
	m_pLocalPlayer=NULL;
	m_pShadowPool=NULL;
	m_bEnalbeShadow=FALSE;
	m_mapVirtualRole.clear();
	
	m_paReqRole.first = 0;
	m_paReqRole.second.clear();

	m_bHideOtherPlayers = false;
	m_viewDistLevel = 0;
	m_fViewDist = 0.0f;
	m_dwRolePointEffectID = GT_INVALID;
	m_dwRecivMsgTime = GT_INVALID;
	m_pKeyCodeFrame = NULL;
}

RoleMgr::~RoleMgr(void)
{
}

RoleMgr g_roleMgr;
RoleMgr* RoleMgr::Inst()
{
	return &g_roleMgr;
}

void RoleMgr::Init(DWORD localPlayerID)
{
	m_pCmdMgr = TObjRef<NetCmdMgr>();
	m_pSession = TObjRef<NetSession>();
    m_pFrameMgr = TObjRef<GameFrameMgr>();
	m_pUtil	= TObjRef<Util>();

	//Destroy();
	
	//--创建本地玩家角色
	m_pLocalPlayer=new LocalPlayer;
	m_pLocalPlayer->SetID(localPlayerID);
	m_pLocalPlayer->InitFSM();

	//--注册网络消息处理函数
	m_pCmdMgr->Register("NS_SyncFamilyActive",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_SyncFamilyActive),	_T("NS_SyncFamilyActive"));
	m_pCmdMgr->Register("NS_RemoteRoleSkin",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemoteRoleTransSkin),	_T("NS_RemoteRoleSkin"));
	m_pCmdMgr->Register("NS_IsDressUpAgree",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetFriendBeautyMe),		_T("NS_IsDressUpAgree"));
	m_pCmdMgr->Register("NS_UseExpPilular",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetUseExpPilular),			_T("NS_UseExpPilular"));
	m_pCmdMgr->Register("NS_WillShutdownWorld",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetShutDownServer),		_T("NS_WillShutdownWorld"));
	m_pCmdMgr->Register("NS_RoleBeAttacked",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetBeAttacked),			_T("NS_RoleBeAttacked"));
	m_pCmdMgr->Register("NS_RemoteRoleChangeFabao",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetRRoleChangeFabao),		_T("NS_RemoteRoleChangeFabao"));
	m_pCmdMgr->Register("NS_AppearanceTransform",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetChangeAvatar),			_T("NS_AppearanceTransform"));
	m_pCmdMgr->Register("NS_VoteForDrangonGirl",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetVote),					_T("NS_VoteForDrangonGirl"));
	m_pCmdMgr->Register("NS_LPRename",					(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetRename),				_T("NS_LPRename"));
	m_pCmdMgr->Register("NS_GetRoleInitState_Att",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetInitLocalPlayerAtt),		_T("NS_GetRoleInitState_Att"));
	m_pCmdMgr->Register("NS_GetRemoteRoleState",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemoteRoleState),		_T("NS_GetRemoteRoleState"));
	m_pCmdMgr->Register("NS_GetRemoteCreatureState",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemoteCreatureState),	_T("NS_GetRemoteCreatureState"));
	m_pCmdMgr->Register("NS_RemoveRemote",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoveRemote),			_T("NS_RemoveRemote"));
	m_pCmdMgr->Register("NS_RoleAttChangeSingle",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttChangeSingle),		_T("NS_RoleAttChangeSingle"));
	m_pCmdMgr->Register("NS_RoleAttChangeMutiple",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttChangeMutiple),	_T("NS_RoleAttChangeMutiple"));
	m_pCmdMgr->Register("NS_RemoteAttChangeSingle",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteAttChangeSingle),	_T("NS_RemoteAttChangeSingle"));
	m_pCmdMgr->Register("NS_RemoteAttChangeMutiple",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteAttChangeMutiple),	_T("NS_RemoteAttChangeMutiple"));
	m_pCmdMgr->Register("NS_AvatarEquipChange",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAvatarEquipChange),		_T("NS_AvatarEquipChange"));
	m_pCmdMgr->Register("NS_AvatarEquip",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAvatarEquip),				_T("NS_AvatarEquip"));
	m_pCmdMgr->Register("NS_AddRoleBuff",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAddRoleBuff),				_T("NS_AddRoleBuff"));
	m_pCmdMgr->Register("NS_RemoveRoleBuff",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoveRoleBuff),			_T("NS_RemoveRoleBuff"));
	m_pCmdMgr->Register("NS_UpdateRoleBuff",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetUpdateRoleBuff),			_T("NS_UpdateRoleBuff"));
	m_pCmdMgr->Register("NS_RoleAttPointAddChange",	    (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttPointAddChange),	_T("NS_RoleAttPointAddChange"));
	m_pCmdMgr->Register("NS_RoleBidAttPoint",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleBidAttPoint),			_T("NS_RoleBidAttPoint"));
	m_pCmdMgr->Register("NS_RoleClearAttPoint",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleClearAttPoint),		_T("NS_RoleClearAttPoint"));
	m_pCmdMgr->Register("NS_SetRoleStateEx",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSetRoleStateEx),			_T("NS_SetRoleStateEx"));
	m_pCmdMgr->Register("NS_UnSetRoleStateEx",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetUnsetRoleStateEx),		_T("NS_UnSetRoleStateEx"));
	m_pCmdMgr->Register("NS_RoleSetDisplay",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleSetDisplay),			_T("NS_RoleSetDisplay"));
	m_pCmdMgr->Register("NS_SetPersonalSet",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSetPersonalSet),			_T("NS_SetPersonalSet"));
    m_pCmdMgr->Register("NS_GetRemotePetState",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemotePetState),		_T("NS_GetRemotePetState"));
	m_pCmdMgr->Register("NS_EquipEffectChange",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetEquipEffectChange),		_T("NS_EquipEffectChange"));
	m_pCmdMgr->Register("NS_SuitEffect",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSuitEffectChange),		_T("NS_SuitEffect"));
    m_pCmdMgr->Register("NS_RoleTitleChangeBroadcast",  (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleTitleChange),		    _T("NS_RoleTitleChangeBroadcast"));
	m_pCmdMgr->Register("NS_RemoteRoleGuildInfoChange", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteGuildInfoChange),	_T("NS_RemoteRoleGuildInfoChange"));
	m_pCmdMgr->Register("NS_CreatureTaggedOwnerChange", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetCreatureTaggedOwnerChange),	_T("NS_CreatureTaggedOwnerChange"));
    m_pCmdMgr->Register("NS_SetTransportMap",           (NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_SetTransportMap),	    _T("NS_SetTransportMap"));
	m_pCmdMgr->Register("NS_RoleLevelChange",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_RoleLevelChange),		_T("NS_RoleLevelChange"));
	m_pCmdMgr->Register("NS_RoleRename",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleRenameBroadcast),		_T("NS_RoleRename") );
	m_pCmdMgr->Register("NS_SyncActiveCreaturesPos",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_ActiveMoster),			_T("NS_SyncActiveCreaturesPos") );
	m_pCmdMgr->Register("NS_ChangeClergy",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_ChangeClergy),			_T("NS_ChangeClergy"));
	m_pCmdMgr->Register("NS_PlsInputReactiveKeyCode",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_PlsInputReactiveKeyCode),			_T("NS_PlsInputReactiveKeyCode"));
	m_pCmdMgr->Register("NS_ReactiveKeyCode",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_ReactiveKeyCode),			_T("NS_ReactiveKeyCode"));
	m_pCmdMgr->Register("NS_RemoteHolySoulAttChange",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_RemoteHolySoulAttChange), _T("NS_RemoteHolySoulAttChange") );
	m_pCmdMgr->Register("NS_CallShengLing",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_CallShengLing),			  _T("NS_CallShengLing"));
	m_pCmdMgr->Register("NS_CallShengLingDisappear",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_CallShengLingDisappear),	_T("NS_CallShengLingDisappear"));
	m_pCmdMgr->Register("NS_SyncCamp",					(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_SyncCamp),					_T("NS_SyncCamp"));
	//m_pCmdMgr->Register("NS_UnSetRoleState",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_UnSetRoleState),			_T("NS_UnSetRoleState"));
	//m_pCmdMgr->Register("NS_GetRoleAttHolySoulEffect", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_GetRoleAttHolySoulEffect), _T("NS_GetRoleAttHolySoulEffect") );

    // 注册游戏事件
    m_pFrameMgr->RegisterEventHandle(_T("tagEnterMount"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnGetPetMount));
    m_pFrameMgr->RegisterEventHandle(_T("tagLeaveMount"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnLeavePetMount));
	m_pFrameMgr->RegisterEventHandle(_T("tagLPRoleStateChangeEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnLpChangeStateEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagMousePointRoleIDChangeEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnMousePointRoleIDChangeEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagRolePickEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnRolePickEvent));
	m_pFrameMgr->RegisterEventHandle(_T("Hide_Players"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnHideOtherPlayers));
	m_pFrameMgr->RegisterEventHandle(_T("Show_Players"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnShowOtherPlayers));
	m_pFrameMgr->RegisterEventHandle(_T("VoteForDragonDaughter"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnRoleVote));
	m_pFrameMgr->RegisterEventHandle(_T("tagStartNPCTalkEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagLeaveCombatEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnRoleLeaveCombatEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagEnterCombatEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnRoleEnterCombatEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagNPCTalkEvent"),		(FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnNPCTalkEvent));
	m_pFrameMgr->RegisterEventHandle(_T("OnFriendBeautyMe"),	(FRAMEEVENTPROC)(m_trunk.sfp1(&RoleMgr::OnEventFriendBeautyMe)) );

	// 注册隐藏/显示其他玩家热键响应函数
	//TObjRef<GameInputMap>()->Register(0, g_StrTable[_T("Hotkey_HidePlayer")], (INPUTMAPEVENTHANDLE)m_trunk.sfp2(&RoleMgr::OnHideOtherPlayers), DIK_F9, TRUE, TRUE, 1);

	// 初始化角色请求发送时间
	m_paReqRole.first = 0;
}

void RoleMgr::Destroy()
{
	m_setEnemy.clear();

	map<DWORD, VirtualRole*>::iterator iterVR = m_mapVirtualRole.begin();
	for ( ; iterVR!=m_mapVirtualRole.end(); ++iterVR )
	{
		VirtualRole* pVR = iterVR->second;
		SAFE_DELETE(pVR);
	}
	m_mapVirtualRole.clear();

	//--释放本地玩家角色
	SAFE_DELETE(m_pLocalPlayer);

	//--释放其它角色
	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();++iter)
	{
		Role* pRole=iter->second;
		delete pRole;
	}
	m_roleMap.clear();

	//--释放死亡生物
	for(list<Role*>::iterator iter=m_listDeadCreature.begin();
		iter!=m_listDeadCreature.end();++iter)
	{
		Role* pRole=(*iter);
		delete pRole;
	}
	m_listDeadCreature.clear();

	SAFE_DELETE( m_pShadowPool );

	//--清空请求列表
	m_paReqRole.first = 0;
	m_paReqRole.second.clear();

	m_bHideOtherPlayers = false;
	m_pKeyCodeFrame = NULL;
	//--注册网络消息处理函数
	m_pCmdMgr->UnRegister("NS_SyncFamilyActive",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_SyncFamilyActive));
	m_pCmdMgr->UnRegister("NS_RemoteRoleSkin",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemoteRoleTransSkin));
	m_pCmdMgr->UnRegister("NS_IsDressUpAgree",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetFriendBeautyMe));
	m_pCmdMgr->UnRegister("NS_UseExpPilular",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetUseExpPilular));
	m_pCmdMgr->UnRegister("NS_WillShutdownWorld",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetShutDownServer) );
	m_pCmdMgr->UnRegister("NS_RoleBeAttacked",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetBeAttacked) );
	m_pCmdMgr->UnRegister("NS_RemoteRoleChangeFabao",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetRRoleChangeFabao) );
	m_pCmdMgr->UnRegister("NS_AppearanceTransform",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetChangeAvatar) );
	m_pCmdMgr->UnRegister("NS_VoteForDrangonGirl",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetVote) );
	m_pCmdMgr->UnRegister("NS_LPRename",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetRename));
	m_pCmdMgr->UnRegister("NS_GetRoleInitState_Att",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetInitLocalPlayerAtt));
	m_pCmdMgr->UnRegister("NS_GetRemoteRoleState",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemoteRoleState));
	m_pCmdMgr->UnRegister("NS_GetRemoteCreatureState",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemoteCreatureState));
	m_pCmdMgr->UnRegister("NS_RemoveRemote",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoveRemote));
	m_pCmdMgr->UnRegister("NS_RoleAttChangeSingle",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttChangeSingle));
	m_pCmdMgr->UnRegister("NS_RoleAttChangeMutiple",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttChangeMutiple));
	m_pCmdMgr->UnRegister("NS_RemoteAttChangeSingle",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteAttChangeSingle));
	m_pCmdMgr->UnRegister("NS_RemoteAttChangeMutiple",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteAttChangeMutiple));
	m_pCmdMgr->UnRegister("NS_AvatarEquipChange",	    (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAvatarEquipChange));
	m_pCmdMgr->UnRegister("NS_AvatarEquip",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAvatarEquip));
	m_pCmdMgr->UnRegister("NS_AddRoleBuff",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAddRoleBuff));
	m_pCmdMgr->UnRegister("NS_RemoveRoleBuff",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoveRoleBuff));
	m_pCmdMgr->UnRegister("NS_UpdateRoleBuff",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetUpdateRoleBuff));
	m_pCmdMgr->UnRegister("NS_RoleAttPointAddChange",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttPointAddChange));
	m_pCmdMgr->UnRegister("NS_RoleBidAttPoint",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleBidAttPoint));
	m_pCmdMgr->UnRegister("NS_RoleClearAttPoint",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleClearAttPoint));
	m_pCmdMgr->UnRegister("NS_SetRoleStateEx",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSetRoleStateEx));
	m_pCmdMgr->UnRegister("NS_UnSetRoleStateEx",	    (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetUnsetRoleStateEx));
	m_pCmdMgr->UnRegister("NS_RoleSetDisplay",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleSetDisplay));
	m_pCmdMgr->UnRegister("NS_SetPersonalSet",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSetPersonalSet));
    m_pCmdMgr->UnRegister("NS_GetRemotePetState",	    (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemotePetState));
	m_pCmdMgr->UnRegister("NS_EquipEffectChange",	    (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetEquipEffectChange));
	m_pCmdMgr->UnRegister("NS_SuitEffect",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSuitEffectChange));
	m_pCmdMgr->UnRegister("NS_RoleTitleChangeBroadcast",  (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleTitleChange));
	m_pCmdMgr->UnRegister("NS_RemoteRoleGuildInfoChange", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteGuildInfoChange));
	m_pCmdMgr->UnRegister("NS_CreatureTaggedOwnerChange", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetCreatureTaggedOwnerChange));
    m_pCmdMgr->UnRegister("NS_SetTransportMap",           (NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_SetTransportMap));
	m_pCmdMgr->UnRegister("NS_RoleLevelChange",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_RoleLevelChange));
	m_pCmdMgr->UnRegister("NS_RoleRename",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleRenameBroadcast) );
	m_pCmdMgr->UnRegister("NS_SyncActiveCreaturesPos",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_ActiveMoster) );
	m_pCmdMgr->UnRegister("NS_ChangeClergy",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_ChangeClergy));
	m_pCmdMgr->UnRegister("NS_PlsInputReactiveKeyCode",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_PlsInputReactiveKeyCode));
	m_pCmdMgr->UnRegister("NS_ReactiveKeyCode",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_ReactiveKeyCode));
	m_pCmdMgr->UnRegister( "NS_RemoteHolySoulAttChange", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_RemoteHolySoulAttChange) );
	m_pCmdMgr->UnRegister("NS_CallShengLing",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_CallShengLing));
	m_pCmdMgr->UnRegister("NS_CallShengLingDisappear",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_CallShengLingDisappear));
	m_pCmdMgr->UnRegister("NS_SyncCamp",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_SyncCamp));
	//m_pCmdMgr->UnRegister("NS_UnSetRoleState",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_UnSetRoleState));
	//m_pCmdMgr->UnRegister( "NS_GetRoleAttHolySoulEffect", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_GetRoleAttHolySoulEffect) );

    // 注销游戏事件
    m_pFrameMgr->UnRegisterEventHandler(_T("tagEnterMount"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnGetPetMount));
    m_pFrameMgr->UnRegisterEventHandler(_T("tagLeaveMount"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnLeavePetMount));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagLPRoleStateChangeEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnLpChangeStateEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagMousePointRoleIDChangeEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnMousePointRoleIDChangeEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagRolePickEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnRolePickEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("Show_Players"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnShowOtherPlayers));
	m_pFrameMgr->UnRegisterEventHandler(_T("Hide_Players"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnHideOtherPlayers));
	m_pFrameMgr->UnRegisterEventHandler(_T("VoteForDragonDaughter"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnRoleVote));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagStartNPCTalkEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagLeaveCombatEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnRoleLeaveCombatEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagEnterCombatEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnRoleEnterCombatEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagNPCTalkEvent"),		(FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnNPCTalkEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("OnFriendBeautyMe"),		(FRAMEEVENTPROC)(m_trunk.sfp1(&RoleMgr::OnEventFriendBeautyMe)) );

	TObjRef<GameInputMap>()->SetEnable(g_StrTable[_T("Hotkey_HidePlayer")], false);
	if( P_VALID(GetObj("FriendBeautyMe")) )
		KillObj("FriendBeautyMe");
}

void RoleMgr::OnOpenMap(SceneGraph* pSG, const Vector3& localPlayerPos, const Vector3& localPlayerFaceto)
{
	if (P_VALID(m_pLocalPlayer))
	{
		//--将本地玩家SceneNode添加到SceneGraph，以渲染
		if( NULL != m_pLocalPlayer->GetSceneNode() )
		{
			m_pLocalPlayer->AttachSceneNodeToSG(pSG);	

			VirtualRole *pVirtualRole = RoleMgr::Inst()->GetVirtualRole(m_pLocalPlayer->GetID());
			if( NULL != pVirtualRole && NULL != pVirtualRole->GetSceneNode() )
			{
				pVirtualRole->AttachSceneNodeToSG(pSG);
				pVirtualRole->OnGotoMap(localPlayerPos, localPlayerFaceto);
				//m_v3FaceTo = localPlayerFaceto;
			}
		}
		//--设置角色位置、朝向和状态
		m_pLocalPlayer->OnGotoMap(localPlayerPos,localPlayerFaceto);
	}
}

void RoleMgr::OnCloseMap(SceneGraph* pSG)
{
	if ( !P_VALID(pSG) )
		return;
	//--销毁SceneGraph以前先将本地玩家SceneNode从中移除
	if( NULL != m_pLocalPlayer->GetSceneNode() )
		m_pLocalPlayer->DetachSceneNodeFromSG(pSG);

	// 销毁本地玩家VirtualRole
	map<DWORD, VirtualRole*>::iterator itVR_L = m_mapVirtualRole.find(m_pLocalPlayer->GetID());
	if ( itVR_L != m_mapVirtualRole.end() )
	{
		VirtualRole* pVR = itVR_L->second;
		if ( P_VALID(pVR) )
			pVR->DetachSceneNodeFromSG( pSG );
	}	

	//--释放所有角色
	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();++iter)
	{
		Role* pRole=iter->second;
		if( NULL != pRole->GetSceneNode() )
		{
			//释放场景之前先删掉其身上的特效
			//EffectMgr::Inst()->RemoveRoleEffect( pRole->GetID() );
			pRole->DetachSceneNodeFromSG(pSG);
		}

		//释放玩家虚拟角色
		map<DWORD, VirtualRole*>::iterator itVR = m_mapVirtualRole.find(pRole->GetID());
		if ( itVR != m_mapVirtualRole.end() )
		{
			VirtualRole* pVR = itVR->second;
			if ( P_VALID(pVR) )
			{
				pVR->DetachSceneNodeFromSG( pSG );
				pVR->UnloadTransformModel( TRUE, pRole->GetID() );
				SAFE_DELETE(pVR);
			}
			m_mapVirtualRole.erase(pRole->GetID());
		}	

		delete pRole;
	}
	m_roleMap.clear();

	//--释放死亡生物
	for(list<Role*>::iterator iter=m_listDeadCreature.begin();
		iter!=m_listDeadCreature.end();++iter)
	{
		Role* pRole=(*iter);
		if( NULL != pRole->GetSceneNode() )
			pRole->DetachSceneNodeFromSG(pSG);
		delete pRole;
	}
	m_listDeadCreature.clear();

	//--清空请求列表
	m_paReqRole.first = 0;
	m_paReqRole.second.clear();
}

Role* RoleMgr::FindRole( DWORD roleID,bool bAddReqIfNotFind/*=false*/,bool bFindDeadCreatureList/*=false*/ )
{
	if(roleID==m_pLocalPlayer->GetID())
		return m_pLocalPlayer;

	map<DWORD,Role*>::iterator iter=m_roleMap.find(roleID);
	if(iter!=m_roleMap.end())
	{
		return iter->second;
	}

	if(bAddReqIfNotFind)
	{
		Role* pRole=NULL;
		if(IS_PLAYER(roleID))
		{
			Player* pPlayer=new Player;
			pPlayer->SetID(roleID);
			pPlayer->InitFSM();	
			pRole=pPlayer;
		}
		else if (IS_CREATURE(roleID))
		{
			ASSERT(IS_CREATURE(roleID));
			if(IS_NORMAL_CREATURE(roleID))
			{
				NPC* pNPC=new NPC;
				pNPC->SetID(roleID);
				pNPC->InitFSM();
				pRole=pNPC;
			}
			else
			{
				ASSERT(IS_DOOR(roleID));
				Door* pDoor=new Door;
				pDoor->SetID(roleID);
				pRole=pDoor;
			}
		}
        else if (IS_PET(roleID))
        {
            Pet* pPet = new Pet();
            pPet->SetID(roleID);
            pPet->InitFSM();
            pRole = pPet;
        }
        else
        {
        }
        
        if (P_VALID(pRole))
            m_roleMap.insert(make_pair(roleID, pRole));
        else
            ASSERT(P_VALID(pRole));
		
		AddReqRole(roleID);
		return pRole;
	}
	
	if( bFindDeadCreatureList )
	{
		list<Role*>::iterator it = m_listDeadCreature.begin();
		for (; it != m_listDeadCreature.end(); ++it)
		{
			if (((Role*)(*it))->GetID() == roleID)
				return *it;
		}
	}

	return NULL;
}

void RoleMgr::FreeRole(DWORD roleID,SceneGraph* pSG)
{
	if(roleID == m_pLocalPlayer->GetID())
		return;

	map<DWORD,Role*>::iterator iter=m_roleMap.find(roleID);
	if(iter!=m_roleMap.end())
	{
		Role* pRole=iter->second;

		// 服务器没有对坐骑进行实体管理，所以这里需要删除坐骑
		if( P_VALID(pRole) && pRole->IS_STATIC_CLASS(Player) )
		{
			Player *pPlayer = static_cast<Player*>(pRole);
			Pet *pPet = pPlayer->GetMount();
			if( P_VALID( pPet ) )
			{
				if( pPet->GetRidersCount() <= 1 )
				{
					if( NULL != pPet->GetSceneNode() )
						pPet->DetachSceneNodeFromSG(pSG);
					m_roleMap.erase( pPet->GetID() );
					delete pPet;
				}
				else
				{
					pPet->RidersCountDec();
				}
			}

			//释放玩家虚拟角色
			map<DWORD, VirtualRole*>::iterator itVR = m_mapVirtualRole.find(pRole->GetID());
			if ( itVR != m_mapVirtualRole.end() )
			{
				VirtualRole* pVR = itVR->second;
				if ( P_VALID(pVR) )
				{
					pVR->DetachSceneNodeFromSG( pSG );
					pVR->UnloadTransformModel();
					SAFE_DELETE(pVR);
				}
				m_mapVirtualRole.erase(pRole->GetID());
			}	
		}

		//释放场景之前先删掉其身上的特效
		//EffectMgr::Inst()->RemoveRoleEffect( pRole->GetID() );

		if( NULL != pRole->GetSceneNode() )
			pRole->DetachSceneNodeFromSG(pSG);
		delete pRole;
		m_roleMap.erase(iter);
	}
}

void RoleMgr::Update( SceneGraph* pSG )
{
	// 当请求列表不为空，且待发时间大于500毫秒时，向服务器发送角色数据请求
	if( !m_paReqRole.second.empty()
		&& 2000 < Kernel::Inst()->GetAccumTimeDW() - m_paReqRole.first)
		SendReqRoleList();

	m_pLocalPlayer->Update();  
	UpdataHightLight( m_pLocalPlayer );

	map<DWORD, VirtualRole*>::iterator itVR = m_mapVirtualRole.begin();
	for ( ; itVR!=m_mapVirtualRole.end() ; ++itVR )
	{
		VirtualRole* pVR = itVR->second;
		if (P_VALID(pVR))
			pVR->Update();
	}

	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();)
	{
		Role* pRole=iter->second;
		if( P_VALID(pRole) )
		{
			// 死亡生物移动到死亡生物列表
			if( IS_CREATURE(iter->first) && pRole->IsDead() )
			{
				m_listDeadCreature.push_back(pRole);
				iter = m_roleMap.erase(iter);
				continue;
			}

			pRole->Update();

			// 隐藏/显示角色
			const bool bHide = GetRoleHide( pRole );
            pRole->Hide( bHide );
			if(!bHide){
				if (pRole->GetLastHitedTime()==0 ||  timeGetTime()-pRole->GetLastHitedTime()>=300){
				   UpdataHightLight( pRole );    
				}
			}
			// 隐藏/显示虚拟角色
			if (pRole->IS_KIND_OF(Player))
			{
				Player *pPlayer = (Player*)pRole;
				VirtualRole* pVR = GetVirtualRole(pPlayer->GetID());
				if ( P_VALID(pVR) && NULL!=pVR->GetSceneNode() ) 
				{
					if ( pPlayer->IsPlayerHolyAwake() )
						pVR->Hide(bHide);
				}
			}


		}
		++iter;
	}

	// 更新死亡角色
	for(list<Role*>::iterator iter=m_listDeadCreature.begin();iter!=m_listDeadCreature.end();)
	{
		Role* pRole=(*iter);
		if( P_VALID(pRole) )
		{
			pRole->Update();

			// 释放死亡角色
			if( pRole->IsNeedToFree() )
			{
				if( NULL != pRole->GetSceneNode() )
					pRole->DetachSceneNodeFromSG(pSG);
				delete pRole;
				iter = m_listDeadCreature.erase(iter);
				continue;
			}

			// 隐藏/显示角色
			const bool bHide = GetRoleHide( pRole );
			pRole->Hide( bHide );
			if(!bHide){
			pRole->SetMulColor(1.0f,1.0f,1.0f);  
			}
		}
		iter++;
	}

	// 角色阴影
	UpdataRoleShadow();

	if( !m_pLocalPlayer->GetRoleState(ERS_Combat) && 
		m_dwRecivMsgTime != GT_INVALID && 
		Kernel::Inst()->GetAccumTimeDW() - m_dwRecivMsgTime >= 30000 )
	{
		m_setEnemy.clear();
		m_dwRecivMsgTime = GT_INVALID;
	}
}

DWORD RoleMgr::PickRole( const Ray& mouseRay, const BOOL bIgnorePlayer )
{
	DWORD dwRoleID = GT_INVALID;
	float fMinDistSq = FLOAT_MAX;

	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();iter++)
	{
		
		if (iter->second->IS_STATIC_CLASS(Door))
		{
			// 如果是打开门的状态
			//if (((Door*)(iter->second))->GetDoorState() == true)
				continue;
		}

		SceneNode* pSGNode=iter->second->GetSceneNode();
		if(pSGNode==NULL)
			continue;

		if( !iter->second->IsAttributeInited() )
			continue;

		if( iter->second->IsHide() )
			continue;

		if( IS_PLAYER( iter->first ) && bIgnorePlayer )
			continue;

		// 本地玩家的宠物、远程玩家的宠物非骑乘状态、忽略远程玩家---忽略该宠物
		if( iter->second->IS_STATIC_CLASS(Pet) )
		{
			Pet* pPet = (Pet*)iter->second;
			if( pPet->GetOwnerID() == m_pLocalPlayer->GetID() || pPet->IsFSMEnabled() || bIgnorePlayer )
				continue;
		}

		AABBox box;
		pSGNode->GetBox(box);
		TResult tr=mouseRay.Intersect(box);
		// jay add 
		// to use obj space aabb && real skin pick
		if(tr.first && (pSGNode->IS_KIND_OF(EntityNode) || pSGNode->IS_KIND_OF(AvatarNode)))
		{
			if(pSGNode->IS_KIND_OF(AvatarNode))
			{
				((AvatarNode*)pSGNode)->GetObjBox(box);
			}
			else
			{
				((EntityNode*)pSGNode)->GetObjBox(box);
			}			

			Matrix4 objMatrix;
			if(P_VALID(D3DXMatrixInverse(&objMatrix, NULL, &pSGNode->GetWorldMat())))
			{
				Ray objSpaceRay;
				D3DXVec3TransformCoord(&objSpaceRay.origin, &mouseRay.origin, &objMatrix);
				D3DXVec3TransformNormal(&objSpaceRay.dir, &mouseRay.dir, &objMatrix);
				tr=objSpaceRay.Intersect(box);
				if(tr.first && GameSet::Inst()->Get(EGS_AccurateRolePick)==TRUE)
				{
					if(pSGNode->IS_KIND_OF(AvatarNode))
					{
						tr = ((AvatarNode*)pSGNode)->RayCollide(mouseRay);
					}
					else
					{
						tr = ((EntityNode*)pSGNode)->RayCollide(mouseRay) ;
						if(!tr.first) tr = ((EntityNode*)pSGNode)->RayCollideForDynaNode(mouseRay) ;
					}
				}
			}
		}
		if(tr.first)
		{
			const float fDistSq = Vec3DistSq( mouseRay.origin, iter->second->GetPos() );
			if( fDistSq < fMinDistSq )
			{
				fMinDistSq = fDistSq;

				// 如果是坐骑，则返回骑乘者ID
				if( iter->second->IS_STATIC_CLASS(Pet) )
				{
					Pet* pPet = (Pet*)iter->second;
					dwRoleID = pPet->GetOwnerID();
				}
				else
				{
					dwRoleID = iter->first;
				}
			}
		}
	}
	return dwRoleID;
}

DWORD RoleMgr::GetLocalPlayerID()
{
	return m_pLocalPlayer->GetID();
}

void RoleMgr::AddReqRole(DWORD dwRoleID)
{
	//--如果请求列表中不存在，则添加
	bool bFind=false;
	for(list<DWORD>::iterator iter=m_paReqRole.second.begin();
		iter!=m_paReqRole.second.end();++iter)
	{
		if(*iter==dwRoleID)
		{
			bFind=true;
			break;
		}
	}
	if(!bFind)
	{
		m_paReqRole.second.push_back(dwRoleID);
	}
}

void RoleMgr::SendReqRoleList()
{
	BYTE byTemp[sizeof(tagNC_GetRemoteRoleState)+sizeof(DWORD)*50]={0};
	tagNC_GetRemoteRoleState* pSend = (tagNC_GetRemoteRoleState*)byTemp;
	pSend->dwID = pSend->Crc32("NC_GetRemoteRoleState");
	pSend->dwExID =0;
	pSend->nRoleNum = 0;
	while( !m_paReqRole.second.empty() )
	{
		pSend->dwRoleID[pSend->nRoleNum++] = m_paReqRole.second.front();
		m_paReqRole.second.pop_front();
		
		if( pSend->nRoleNum >= 50 )	// 一次最多传50个ID
			break;
	}

	if( pSend->nRoleNum > 0 )
	{
		pSend->dwSize = sizeof(tagNC_GetRemoteRoleState)
			+ sizeof(DWORD) * (pSend->nRoleNum - 1);
		m_pSession->Send(pSend);
		m_paReqRole.first = Kernel::Inst()->GetAccumTimeDW();
	}
}

DWORD RoleMgr::NetRemoveRemote( tagNS_RemoveRemote* pNetCmd, DWORD )
{
	int num = ( pNetCmd->dwSize - sizeof(tagNS_RemoveRemote) ) / sizeof(DWORD) + 1;
	for(int i=0;i<num;i++)
	{
		FreeRole(pNetCmd->dwRoleID[i],MapMgr::Inst()->GetSceneGraph());
	}

	return 0;
}

DWORD RoleMgr::NetInitLocalPlayerAtt( tagNS_GetRoleInitState_Att* pNetCmd, DWORD )
{
	if( P_VALID(m_pLocalPlayer) && NULL == m_pLocalPlayer->GetSceneNode() )
	{
		m_pLocalPlayer->LoadAvatar( pNetCmd->Avatar, pNetCmd->AvatarEquip, pNetCmd->DisplaySet );
		m_pLocalPlayer->AttachSceneNodeToSG( MapMgr::Inst()->GetSceneGraph() );
		m_pLocalPlayer->InitAttribute( pNetCmd );
		m_pLocalPlayer->LoadEquipEffect(pNetCmd->AvatarEquip);

		//设置神职ID
		m_pLocalPlayer->SetPlayerClergy(pNetCmd->dwClergyID);
		//家族设置
		m_pLocalPlayer->SetRoleFamilyID(pNetCmd->dwFamilyID);
		m_pLocalPlayer->SetRoleFamilyLeader(pNetCmd->bFamilyLeader);
		if ( GT_INVALID != pNetCmd->dwFamilyID )
		{
			tagFamilyFrameEvent evt( _T("FamilyEvent_Frame"), NULL );
			evt.bShowFrame	= FALSE;
			m_pFrameMgr->SendEvent( &evt );
		}
		//设置声望相关
		ReputeMgr::Inst()->SetGodRepute(pNetCmd->nGodPoint);
		ReputeMgr::Inst()->SetEvilRepute(pNetCmd->nMonsterPoint);
		ReputeMgr::Inst()->SetGodSoul(pNetCmd->nGod);
		ReputeMgr::Inst()->SetEvilSoul(pNetCmd->nMonster);

		// 发送初始化消息
		tagNC_GetRoleInitState m;
		m.eType = ERIT_Skill;
		m_pSession->Send(&m);
		
		// 如果等级小于40..显示onlinetips
	//	if (GetLocalPlayer()->GetRoleLevel() < 40)
	//	{
			OnlineTips *onlinetips = static_cast<OnlineTips*>(TObjRef<GameFrameMgr>()->GetFrame(_T("OnlineTips")));
			onlinetips->ShowWnd(TRUE);
	//	}

		// 发送游戏事件让onlinetips刷新
		TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("Refresh_Onlinetips"), NULL ) );

        // 发送游戏事件刷新职业
        TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("LocalPlayerSetClass"), NULL ) );

		//----初始化元神消息
		Vector3 v3FaceTo = m_pLocalPlayer->GetFaceTo();
		//LoadHolyManModel( m_pLocalPlayer, v3FaceTo, HOLYSOUL_INITID);
		//LoadHolyManModel(m_pLocalPlayer, v3FaceTo);
// 		if ( m_pLocalPlayer->GetRoleLevel() >= MIN_HOLY_SOUL_LEVEL )
// 		{	
// 			HolyManMgr::Inst()->ActiveHolyMan();
// 			tagNC_GetRoleInitState msg;	//发送初始化元神请求
// 			msg.eType = ERIT_HolySoulInfo;
// 			m_pSession->Send(&msg);
// 		}
	}

	return 0;
}

DWORD RoleMgr::NetGetRemoteRoleState( tagNS_GetRemoteRoleState* pNetCmd, DWORD )
{
	Role* pRole = FindRole( pNetCmd->RoleData.dwID );
	if( P_VALID( pRole ) && pRole->IS_STATIC_CLASS(Player) && NULL == pRole->GetSceneNode() )
	{
		Player* pPlayer = (Player*)pRole;
		pPlayer->LoadAvatar( pNetCmd->RoleData.Avatar, pNetCmd->RoleData.AvatarEquip, pNetCmd->RoleData.sDisplaySet );
		pPlayer->AttachSceneNodeToSG( MapMgr::Inst()->GetSceneGraph() );
		pPlayer->InitAttribute( pNetCmd );
		pPlayer->LoadEquipEffect(pNetCmd->RoleData.AvatarEquip);
		pPlayer->SetPlayerClergy(pNetCmd->RoleData.dwClergyID);
		pPlayer->SetRoleCampType(pNetCmd->RoleData.eCamp);

		//元神
		Vector3 v3FaceTo(pNetCmd->RoleData.fFaceTo[0], pNetCmd->RoleData.fFaceTo[1], pNetCmd->RoleData.fFaceTo[2]);
		//LoadHolyManModel( pPlayer, v3FaceTo, pNetCmd->RoleData.dwHolySoulID);
		//LoadHolyManModel(pPlayer, v3FaceTo);
		LoadHolyManModel(pNetCmd->RoleData.dwID, pNetCmd->RoleData.dwHolySoulID, pNetCmd->RoleData.dwEatFairyNum);
		if ( pPlayer->IsPlayerHolyAwake() )
		{
			VirtualRole *pVirtualRole = GetVirtualRole(pPlayer->GetID());
			if( NULL != pVirtualRole && NULL != pVirtualRole->GetSceneNode() )
			{
				//pVirtualRole->CallHolyManInit();
				pVirtualRole->SetState(EPAT_MOVE,TRUE);
				pVirtualRole->Hide(FALSE);
			}
		}



	}

	return 0;
}

DWORD RoleMgr::NetGetRemoteCreatureState( tagNS_GetRemoteCreatureState* pNetCmd, DWORD )
{
	Role* pRole = FindRole( pNetCmd->CreatureData.dwID );
	if( (P_VALID( pRole ) && pRole->IS_STATIC_CLASS(NPC) && NULL == pRole->GetSceneNode()) || 
		(P_VALID( pRole ) && pRole->IS_STATIC_CLASS(Door) && NULL == pRole->GetSceneNode()) )
	{
		NPC* pNPC = (NPC*)pRole;
		const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt( pNetCmd->CreatureData.dwTypeID );
		if (!P_VALID(pProto))
			return 0;

		tstring  strFileName = CreatureData::Inst()->GetNpcMdlPath( pProto );
		pNPC->LoadEntity( strFileName.c_str() );
		if( pProto->eAnimationType == EAT_Skeleton )
		{
			int i=1;
			tstring szSkinPath = CreatureData::Inst()->GetNpcSkinPath(pProto,i++);
			IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
			DWORD fileSize = pIFS->GetSize(szSkinPath.c_str());
			while( 0 != fileSize && GT_INVALID != fileSize )
			{
				pNPC->LoadSkin(szSkinPath.c_str());
				szSkinPath = CreatureData::Inst()->GetNpcSkinPath(pProto,i++);
				fileSize = pIFS->GetSize(szSkinPath.c_str());
			}
			pNPC->UpdateBoundingBox();
		}

		pNPC->AttachSceneNodeToSG( MapMgr::Inst()->GetSceneGraph() );
		pNPC->InitAttribute( pNetCmd );
		pNPC->SetRoleCampType(pNetCmd->CreatureData.eCamp);
	}

	return 0;
}



DWORD RoleMgr::NetGetRemotePetState( tagNS_GetRemotePetState* pNetCmd, DWORD )
{
    Role* pRole = FindRole(pNetCmd->PetData.dwID);
	 Role* pOwner = FindRole(pNetCmd->PetData.dwRoleID);

    if (P_VALID(pRole) && pRole->IS_STATIC_CLASS(Pet) && NULL == pRole->GetSceneNode())
    {
        Pet* pPet = static_cast<Pet*>(pRole);
        pPet->InitPetAttribute(pNetCmd);
        const tagPetProto* petProto = pPet->GetProto();
        ASSERT(P_VALID(petProto));
        EPetModelType mdltype = EPMT_Follow;
        pPet->LoadEntity( petProto->strModelPath[mdltype].c_str());
        if (EAT_Skeleton == petProto->eAniType[mdltype])
        {
            for (list<tstring>::const_iterator it = petProto->lstSkinPath[mdltype].begin();
                it != petProto->lstSkinPath[mdltype].end(); ++it)
            {
                pPet->LoadSkin(it->c_str());
            }
            pPet->UpdateBoundingBox();
        }

		pPet->SetScale(petProto->scalePercent);
		if (P_VALID(pOwner) && pOwner->IS_KIND_OF(Player))
		{
			Player* pRider = static_cast<Player*>(pOwner);
			pPet->SetCurrScale(pRider->GetDesScale());
			if ( petProto->type3 != 6)
				pRider->ClearTrumpEffect();
		}

        pPet->AttachSceneNodeToSG( MapMgr::Inst()->GetSceneGraph());

		// 挂载特效
		TCHAR szFile[255];
		_stprintf( szFile, _T("data\\animation\\npc\\Effect\\%d.csv"), pPet->GetTypeID() );
		IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
		DWORD dwFileSize=pIFS->GetSize( szFile );
		if(dwFileSize>0 && dwFileSize!=GT_INVALID)
		{
			CSVFile csvFile;
			if(csvFile.OpenFile(pIFS,szFile))
			{
				//只有一行
				csvFile.BeginReadLine();
				const vector<tstring>& line = csvFile.GetNextLine();
				int numToken=line.size();
				for(int i=0;i<numToken;i+=2)
				{
					if(i+1<numToken)
					{
						DWORD dweffectid = EffectMgr::Inst()->PlayRoleEffect( pPet->GetID(), line[i+1].c_str(), line[i].c_str(), EffectMgr::EEST_RoleScale, NULL );
						if(dweffectid!=GT_INVALID)
							pPet->AddEffectID(dweffectid);
					}
				}
				csvFile.CloseFile();
			}
		}
	}
    return 0;
}

DWORD RoleMgr::NetRoleAttChangeSingle( tagNS_RoleAttChangeSingle* pNetCmd, DWORD )
{
	if(pNetCmd->eType == ERA_God_Faith)
	{
		if(pNetCmd->nValue > m_pLocalPlayer->GetAttribute(ERA_God_Faith))
		{
			tagPctNExpAchieve event(_T("UpdataBelief"),NULL);
			event.nPctMultiplied = pNetCmd->nValue - m_pLocalPlayer->GetAttribute(ERA_God_Faith);
			m_pFrameMgr->SendEvent(&event);
		}
	}

	m_pLocalPlayer->SetAttribute(pNetCmd->eType, pNetCmd->nValue, false);

	// 发送游戏事件让onlinetips刷新
	TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("Refresh_Onlinetips"), NULL ) );
	return 0;
}

DWORD RoleMgr::NetRoleAttChangeMutiple( tagNS_RoleAttChangeMutiple* pNetCmd, DWORD )
{
	for(INT i=0; i<pNetCmd->nNum; ++i)
	{
		m_pLocalPlayer->SetAttribute(pNetCmd->value[i].eType, pNetCmd->value[i].nValue, false);
	}

	// 发送游戏事件让onlinetips刷新
	TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("Refresh_Onlinetips"), NULL ) );
	return 0;
}

DWORD RoleMgr::NetRemoteAttChangeSingle( tagNS_RemoteAttChangeSingle* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole))
	{
		pRole->SetRemoteRoleAtt(pNetCmd->eType, pNetCmd->nValue, false);
	}

	return 0;
}

DWORD RoleMgr::NetRemoteAttChangeMutiple( tagNS_RemoteAttChangeMutiple* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole))
	{
		for(INT i=0; i<pNetCmd->nNum; ++i)
		{
			pRole->SetRemoteRoleAtt(pNetCmd->value[i].eType, pNetCmd->value[i].nValue, false);
		}
	}

	return 0;
}

DWORD RoleMgr::NetAvatarEquipChange( tagNS_AvatarEquipChange* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole) && pRole->IS_KIND_OF(Player) && pRole->IsAttributeInited() )
	{
		Player* pPlayer = (Player*)pRole;
		if (IsTransforming(pPlayer) && EAE_RWeapon != pNetCmd->EquipDisplay.byDisplayPos && EAE_LWeapon != pNetCmd->EquipDisplay.byDisplayPos )
			return 0;

		pPlayer->SetAvatarEquip( (EAvatarElement)pNetCmd->EquipDisplay.byDisplayPos, pNetCmd->EquipDisplay );
		if (pNetCmd->EquipDisplay.dwTypeID == GT_INVALID || pNetCmd->EquipDisplay.dwTypeID == 0)
		{
			pPlayer->RemoveSingleEquipEffect((EAvatarElement)pNetCmd->EquipDisplay.byDisplayPos);
		}
		else
		{
			pPlayer->LoadSingleEquipEffect(pNetCmd->EquipDisplay);
		}
	}

	return 0;
}

DWORD RoleMgr::NetAvatarEquip(tagNS_AvatarEquip* pNetCmd, DWORD)
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole) && pRole->IS_KIND_OF(Player) && pRole->IsAttributeInited() )
	{
		Player* pPlayer = (Player*)pRole;

		if(m_pLocalPlayer->GetID() == pPlayer->GetID())
		{
			if(pNetCmd->byTransSkinIndex != 0 && 
				pNetCmd->byTransSkinIndex != GT_INVALID)
				TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("LocalPlayerTransform"), NULL ) );
			else
				TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("LocalPlayerUnTransform"), NULL ) );
			m_pLocalPlayer->SetTransSkinIndex(pNetCmd->byTransSkinIndex);
		}

		tagDisplaySet displaySet = pPlayer->GetDisplaySet();
		displaySet.bFashionDisplay = pNetCmd->bFashion;
		pPlayer->SetDisplaySet( displaySet );
		tagAvatarEquip& sAvatarEquip = pNetCmd->sAvatarEquip;
		if(pNetCmd->byTransSkinIndex != 0 && pNetCmd->byTransSkinIndex != GT_INVALID)
		{
			for(int i = EAE_Head; i <EAE_RWeapon; i++)		// 隐藏所有的装备，除了武器
			{
				sAvatarEquip.AvatarEquip[i].dwTypeID = GT_INVALID;
			}
		}
		pPlayer->UpdateAvatar( pPlayer->GetAvatarAtt(), sAvatarEquip, MapMgr::Inst()->GetSceneGraph() );
        
		if( pPlayer->GetID() == m_pLocalPlayer->GetID() && !pNetCmd->bFashion )
		{
			tagChangeFashionEvent e( _T("tagChangeFashionEvent"), NULL );
			e.bDisFashion = pNetCmd->bFashion;
			TObjRef<GameFrameMgr>()->SendEvent(&e);
		}
	}

	return 0;
}

DWORD RoleMgr::NetEquipEffectChange(tagNS_EquipEffectChange* pNetCmd, DWORD)
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole) && pRole->IS_KIND_OF(Player) && pRole->IsAttributeInited() )
	{
		Player* pPlayer = (Player*)pRole;
		pPlayer->UpdataLongHunEffect( (EAvatarElement)pNetCmd->byDisplayPos, pNetCmd->byEquipEffect );
	}

	return 0;
}

DWORD RoleMgr::NetSuitEffectChange(tagNS_SuitEffect* pNetCmd, DWORD)
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole) && pRole->IS_KIND_OF(Player) && pRole->IsAttributeInited() )
	{
		Player* pPlayer = (Player*)pRole;
		pPlayer->UpdataSuitEffect( pNetCmd->dwSuitEffectID );
	}

	return 0;
}

DWORD RoleMgr::NetAddRoleBuff( tagNS_AddRoleBuff* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	bool bHideLWeaponBuff = false; 
	if(P_VALID(pRole))
	{
// 		if ( (pNetCmd->Buff).dwBuffTypeID != GT_INVALID )
// 			HolyManCtrl( pRole->GetID(), EPAT_CALL, (pNetCmd->Buff).dwBuffTypeID );
		pRole->AddRoleBuff(pNetCmd->Buff);
		if( P_VALID( pRole ) && pRole->IS_KIND_OF(Player) )
		{
			Player* pPlayer = (Player*)pRole;
			TList<tagRoleBuff*> playerBuffList = pPlayer->GetBuffList();
			if (!playerBuffList.Empty())
			{
				tagRoleBuff* pBuff=NULL;
				playerBuffList.ResetIterator();
				while(playerBuffList.PeekNext(pBuff))
				{
					//1隐藏武器,0显示武器（不做处理）
					if ( pBuff->pBuffProto->nIfHideLWeapon == 1 )
					{
						//只要有1个buff是要求隐藏的就要隐藏
						bHideLWeaponBuff = TRUE;
						break;
					}
				}	
			}
			pPlayer->HideWeapon(bHideLWeaponBuff);
			pPlayer->HideWeaponPertain(bHideLWeaponBuff);
		}
	}
	return 0;
}

DWORD RoleMgr::NetRemoveRoleBuff( tagNS_RemoveRoleBuff* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);

	//隐藏副手武器buff,False表示默认没有此类buff
	bool bHideLWeaponBuff = false; 
	if(P_VALID(pRole))
	{
// 		if ( pNetCmd->dwBuffTypeID != GT_INVALID )
// 			HolyManCtrl( pRole->GetID(), EPAT_DISPPEAR, pNetCmd->dwBuffTypeID );
		pRole->RemoveRoleBuff(pNetCmd->dwBuffTypeID);

		if( P_VALID( pRole ) && pRole->IS_KIND_OF(Player) )
		{
			Player* pPlayer = (Player*)pRole;
			TList<tagRoleBuff*> playerBuffList = pPlayer->GetBuffList();
			if (!playerBuffList.Empty())
			{
				tagRoleBuff* pBuff=NULL;
				playerBuffList.ResetIterator();
				while(playerBuffList.PeekNext(pBuff))
				{
					//1隐藏武器,0显示武器（不做处理）
					if ( pBuff->pBuffProto->nIfHideLWeapon == 1 )
					{
						//只要有1个buff是要求隐藏的就要隐藏
						bHideLWeaponBuff = TRUE;
						break;
					}
				}	
			}
			pPlayer->HideWeapon(bHideLWeaponBuff);
			pPlayer->HideWeaponPertain(bHideLWeaponBuff);
		}
	}
	return 0;
}

DWORD RoleMgr::NetUpdateRoleBuff( tagNS_UpdateRoleBuff* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	//隐藏副手武器buff,False表示默认没有此类buff
	bool bHideLWeaponBuff = false; 
	if(P_VALID(pRole))
	{
		pRole->UpdateRoleBuff(pNetCmd->Buff);

		if( P_VALID( pRole ) && pRole->IS_KIND_OF(Player) )
		{
			tagBuffMsgInfo buff = pNetCmd->Buff;
			DWORD buffID = buff.dwBuffTypeID;
			Player* pPlayer = (Player*)pRole;
			tagRoleBuff* pBuff = pPlayer->FindBuff(buffID);
			if (P_VALID(pBuff))
			{
				//1隐藏武器,0显示武器（不做处理）
				if ( pBuff->pBuffProto->nIfHideLWeapon == 1 )
				{
					bHideLWeaponBuff = TRUE;
				}
			}
			pPlayer->HideWeapon(bHideLWeaponBuff);
			pPlayer->HideWeaponPertain(bHideLWeaponBuff);
		}
	}

	return 0;
}

DWORD RoleMgr::NetRoleAttPointAddChange( tagNS_RoleAttPointAddChange* pNetCmd, DWORD )
{
	for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		m_pLocalPlayer->SetAttPointAdd((ERoleAttribute)i, pNetCmd->nAttPointAdd[i]);
	}
	
	return 0;
}

DWORD RoleMgr::NetRoleBidAttPoint( tagNS_RoleBidAttPoint* pNetCmd, DWORD )
{
	
	return 0;
}

DWORD RoleMgr::NetRoleClearAttPoint( tagNS_RoleClearAttPoint* pNetCmd, DWORD )
{
	
	return 0;
}

bool RoleMgr::IsOutValidDist( DWORD dwRoleID )
{
	Role* pRole = FindRole(dwRoleID);
	if (P_VALID(pRole))
	{
		if( Vec3DistSq(pRole->GetPos(), m_pLocalPlayer->GetPos()) > MAX_EXCHANGE_DIST*MAX_EXCHANGE_DIST )
			return TRUE;
		else
			return FALSE;
	}
	return TRUE;
}

bool RoleMgr::IsInTeamBufDis( DWORD dwRoleID )
{
	Role* pRole = FindRole(dwRoleID);
	if (P_VALID(pRole))
	{
		if( pRole->GetID() == m_pLocalPlayer->GetID() )
			return FALSE;
		if( Vec3DistSq(pRole->GetPos(), m_pLocalPlayer->GetPos()) < MAX_TEAM_DIST*MAX_TEAM_DIST )
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

bool RoleMgr::IsOutFaceDist( DWORD dwRoleID )
{
	Role* pRole = FindRole(dwRoleID);
	if (P_VALID(pRole))
	{
		if( Vec3DistSq(pRole->GetPos(), m_pLocalPlayer->GetPos()) > MAX_FACE_DIST_SQ
			|| Vec3DistSq(pRole->GetPos(), m_pLocalPlayer->GetPos()) < MIN_FACE_DIST_SQ )
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

bool RoleMgr::IsAABBoxCollideRole( const AABBox &box, bool bOnlyNPC /*= true*/ )
{
	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();++iter)
	{
		SceneNode* pSGNode=iter->second->GetSceneNode();
		if(pSGNode==NULL)
			continue;

		if( iter->second->IsHide() )
			continue;
		
		if( IS_PLAYER( iter->first ) && bOnlyNPC )
			continue;

        if (IS_PET(iter->first) && ((Pet*)(iter->second))->IsFSMEnabled())
            continue;

		AABBox other;
		pSGNode->GetBox(other);
		if(box.Intersects(other))
			return TRUE;
	}
	return FALSE;
}

VOID RoleMgr::UpdataRoleShadow()
{
	// 打开阴影
	if( m_bEnalbeShadow && NULL == m_pShadowPool && SGDynamicProjector::IsHWSupport() )
	{
		m_pShadowPool=new ShadowPool;
		m_pShadowPool->CreateData();
	}

	// 关闭阴影
	if( !m_bEnalbeShadow && NULL != m_pShadowPool )
	{
		m_pLocalPlayer->DetachShadow();
		for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
			iter!=m_roleMap.end();++iter)
		{
			Role* pRole=iter->second;
			if( P_VALID(pRole) )
				pRole->DetachShadow();
		}
		SAFE_DELETE( m_pShadowPool );
	}

	// 更新阴影
	if( P_VALID( m_pShadowPool ) )
		m_pShadowPool->AssignToRoles();
}

void RoleMgr::SetViewDistLevel( const int viewDistLevel )
{
	ASSERT( viewDistLevel >= 0 && viewDistLevel <= MAX_VIEW_DIST );
	m_viewDistLevel = viewDistLevel;
	const float fMaxViewDist = VIS_DIST * 2 * TILE_SCALE;
	const float fMinViewDist = 32 * TILE_SCALE;
	const float fStep = ( fMaxViewDist - fMinViewDist ) / MAX_VIEW_DIST;
	m_fViewDist = fMinViewDist + fStep * m_viewDistLevel;
}

bool RoleMgr::GetRoleHide( Role* pRole )
{
	// 角色视野之外
	if( !IsInViewDist( pRole ) )
		return true;

	// 摆摊玩家，隐藏
	if( pRole->IS_KIND_OF(Player) )
	{
		Player* pPlayer = (Player*)pRole;
		if( pPlayer->GetRoleState( ERS_Stall ) )
			return true;
	}
	else if( pRole->IS_KIND_OF(Pet) ) // 摆摊玩家的宠物，隐藏
	{
		Pet* pPet = (Pet*)pRole;
		DWORD dwOwnerID = pPet->GetOwnerID();
		Player* pPlayer = (Player*)FindRole(dwOwnerID);
		if( P_VALID(pPlayer) && pPlayer->GetRoleState( ERS_Stall ) )
			return true;
	}

	// 隐藏其他玩家（F9）
	if( IS_PLAYER( pRole->GetID() ) && m_bHideOtherPlayers )
		return true;

	// 其他玩家的宠物
	if (IS_PET(pRole->GetID()) &&
		PetManager::Inst()->GetPet(pRole->GetID()) == NULL &&
		m_bHideOtherPlayers)
		return true;

	return false;
}

bool RoleMgr::IsInViewDist( Role* pRole )
{
	if( NULL == m_pLocalPlayer )
		return false;

	const float fRoleDistSq = Vec3DistSq( m_pLocalPlayer->GetPos(), pRole->GetPos() );
	if( fRoleDistSq > m_fViewDist * m_fViewDist )
		return false;

	return true;
}

void RoleMgr::UpdataHightLight( Role* pRole )
{
	
	const DWORD dwMousePointRoleID = GetCurMousePointRoleID();
	const DWORD dwCurTargetID = GetCurTargetID();
	
	// 选中或鼠标指向玩家
	
	if( dwMousePointRoleID == pRole->GetID() || dwCurTargetID == pRole->GetID() )
	{
		pRole->SetMulColor( 2.0f, 2.0f, 2.0f );
		return;
	}
	

	// 选中或鼠标指向玩家的坐骑
	if( pRole->IS_KIND_OF(Pet) )
	{
		Pet* pPet = (Pet*)pRole;
		if( !pPet->IsFSMEnabled() && 
			( pPet->GetOwnerID() == dwCurTargetID || pPet->GetOwnerID() == dwMousePointRoleID ) )
		{
			pRole->SetMulColor( 2.0f, 2.0f, 2.0f );
			return;
		}
	}
	pRole->SetMulColor( 1.0f, 1.0f, 1.0f );
}

void RoleMgr::UpdataGameObjectQuestEffect()
{
	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();iter++)
	{
		if( IS_CREATURE( iter->first ) )
		{
			NPC* pNPC = (NPC*)iter->second;
			pNPC->UpdateGameObjectQuestEffect();
		}
	}
}

DWORD RoleMgr::NetRemoteGuildInfoChange(tagNS_RemoteRoleGuildInfoChange *pNetCmd, DWORD)
{
	Role *pRole = FindRole(pNetCmd->dwRoleID);
	if (P_VALID(pRole))
	{
		ASSERT( pRole->IS_KIND_OF(Player) );
		Player* pPlayer = (Player*)pRole;
		pPlayer->SetRoleGuildID(pNetCmd->dwGuildID);
		pPlayer->SetRoleGuildPos(pNetCmd->n8GuildPos);
	}
	return 0;
}

DWORD RoleMgr::NetSetRoleStateEx( tagNS_SetRoleStateEx* pNetCmd, DWORD )
{
	m_pLocalPlayer->SetRoleStateEx(pNetCmd->eState, true);
	
	return 0;
}

DWORD RoleMgr::NetUnsetRoleStateEx( tagNS_UnSetRoleStateEx* pNetCmd, DWORD )
{
	m_pLocalPlayer->SetRoleStateEx(pNetCmd->eState, false);

	return 0;
}

DWORD RoleMgr::NetRoleSetDisplay( tagNS_RoleSetDisplay* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if( P_VALID(pRole) && pRole->IsAttributeInited() )
	{
		ASSERT( pRole->IS_KIND_OF(Player) );
		Player* pPlayer = (Player*)pRole;
		pPlayer->SetDisplaySet( pNetCmd->sDisplaySet );
	}
	return 0;
}

DWORD RoleMgr::NetSetPersonalSet( tagNS_SetPersonalSet* pNetCmd, DWORD )
{
	if( P_VALID(m_pLocalPlayer) && m_pLocalPlayer->IsAttributeInited() )
	{
		m_pLocalPlayer->SetPersonalSet( pNetCmd->PersonalSet );
	}

	return 0;
}

DWORD RoleMgr::NetRoleTitleChange( tagNS_RoleTitleChangeBroadcast* pNetCmd, DWORD )
{
    Role* pRole = FindRole(pNetCmd->dwRoleID);
    if( P_VALID(pRole) && pRole->IsAttributeInited() )
    {
        ASSERT( pRole->IS_KIND_OF(Player) );
        Player* pPlayer = (Player*)pRole;
        pPlayer->SetPlayerTitle(pNetCmd->dwTitleID);

        // 通知
        tagRoleTitleChange e(_T("tagRoleTitleChange"), NULL);
        e.dwRoleID  = pNetCmd->dwRoleID;
        e.dwTitleID = pNetCmd->dwTitleID;
        m_pFrameMgr->SendEvent(&e);
    }
    return 0;
}

DWORD RoleMgr::NetCreatureTaggedOwnerChange(tagNS_CreatureTaggedOwnerChange* pNetCmd, DWORD)
{
	Role* pRole = FindRole(pNetCmd->dwCreatureID);
	if( P_VALID(pRole) && pRole->IS_KIND_OF(NPC) )
	{
		NPC* pNpc = (NPC*)pRole;
		pNpc->SetTaggedOwnerID(pNetCmd->dwTaggedOwner);
	}
	return 0;
}

DWORD RoleMgr::OnGetPetMount( tagEnterMount* pGameEvent )
{
    Role* pRole = FindRole(pGameEvent->dwPetID,true);
    Role* pOwner = FindRole(pGameEvent->dwOwnerID);

    if (P_VALID(pRole) && pRole->IS_STATIC_CLASS(Pet) && NULL == pRole->GetSceneNode())
    {
        Pet* pPet = static_cast<Pet*>(pRole);

		pPet->SetFSMEnabled(false);

        // 伪造网络消息
        tagNS_GetRemotePetState fakeNetMsg;
        fakeNetMsg.PetData.dwID         =   pGameEvent->dwPetID;
        fakeNetMsg.PetData.dwProtoID    =   pGameEvent->dwTypeID;
        fakeNetMsg.PetData.dwRoleID     =   pGameEvent->dwOwnerID;
        fakeNetMsg.PetData.uState.bisRiding     =   true;
        pPet->InitPetAttribute(&fakeNetMsg);

        const tagPetProto* petProto = pPet->GetProto();
        EPetModelType mdltype = EPMT_Mount;
        pPet->LoadEntity( petProto->strModelPath[mdltype].c_str());
        if (EAT_Skeleton == petProto->eAniType[mdltype])
        {
            for (list<tstring>::const_iterator it = petProto->lstSkinPath[mdltype].begin();
                it != petProto->lstSkinPath[mdltype].end(); ++it)
            {
                pPet->LoadSkin(it->c_str());
            }
            pPet->UpdateBoundingBox();
        }

        pPet->SetScale(petProto->scalePercent);
        if (P_VALID(pOwner) && pOwner->IS_KIND_OF(Player))
        {
            Player* pRider = static_cast<Player*>(pOwner);
            pPet->SetCurrScale(pRider->GetDesScale());
			pRider->ClearTrumpEffect();
        }

        pPet->AttachSceneNodeToSG( MapMgr::Inst()->GetSceneGraph());

        // 挂载特效
        TCHAR szFile[255];
        _stprintf( szFile, _T("data\\animation\\npc\\Effect\\%d.csv"), pPet->GetTypeID() );
        IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
        DWORD dwFileSize=pIFS->GetSize( szFile );
        if(dwFileSize>0 && dwFileSize!=GT_INVALID)
        {
            CSVFile csvFile;
            if(csvFile.OpenFile(pIFS,szFile))
            {
                //只有一行
                csvFile.BeginReadLine();
                const vector<tstring>& line = csvFile.GetNextLine();
                int numToken=line.size();
                for(int i=0;i<numToken;i+=2)
                {
                    if(i+1<numToken)
                    {
                        DWORD dweffectid = EffectMgr::Inst()->PlayRoleEffect( pPet->GetID(), line[i+1].c_str(), line[i].c_str(), EffectMgr::EEST_RoleScale, NULL );
                        if(dweffectid!=GT_INVALID)
                            pPet->AddEffectID(dweffectid);
                    }
                }
                csvFile.CloseFile();
            }
        }
    }
	return 0;
}

DWORD RoleMgr::OnLeavePetMount( tagLeaveMount* pGameEvent )
{
    FreeRole(pGameEvent->dwPetID, MapMgr::Inst()->GetSceneGraph());
	Role* pOwner = FindRole(pGameEvent->dwOwnerID);
	if (P_VALID(pOwner) && pOwner->IS_KIND_OF(Player))
	{
		Player* pRider = static_cast<Player*>(pOwner);
		pRider->PlayTrumpEffect();
	}

	return 0;
}

DWORD RoleMgr::OnNS_SetTransportMap( tagNS_SetTransportMap* pNetCmd, DWORD )
{
    if (E_Success == pNetCmd->dwErrorCode)
    {
        LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
        if (P_VALID(pLp))
            pLp->SetTransportPoint(pNetCmd->dwTransportMapID, pNetCmd->vTransportPoint);
    }

    ShowSetTransportErrorMsg(pNetCmd->dwErrorCode);

    return 0;
}

DWORD RoleMgr::OnNS_RoleLevelChange( tagNS_RoleLevelChange* pNetCmd, DWORD )
{
	if( pNetCmd->dwRoleID != GetLocalPlayerID() )
		return 0;

	RoleLevelUpPromptFrame* pFrame = (RoleLevelUpPromptFrame*)(m_pFrameMgr->GetFrame(_T("RoleLevelUpPromptFrame")) );
	if( !P_VALID(pFrame) )
	{
		m_pFrameMgr->CreateFrame(_T("World"), _T("RoleLevelUpPromptFrame"), _T("RoleLevelUpPromptFrame"), 0 );
		pFrame = (RoleLevelUpPromptFrame*)m_pFrameMgr->GetFrame( _T("RoleLevelUpPromptFrame") );
	}
	
	if( P_VALID( pFrame ) )
	{
		pFrame->LevelUpTo( pNetCmd->nLevel );
	}

	if(pNetCmd->nLevel >= 15 && SaveAccount::Inst()->GetYunLianOn())
	{
		//向云联发送暂停游戏消息
		SendyunLianPauthGame();
	}

	return 0;
}

void RoleMgr::ShowSetTransportErrorMsg( DWORD dwErrCode )
{
    if (dwErrCode >= EST_SuccessSet && dwErrCode < EST_ErrEnd)
    {
        tstringstream tss;
        tss << _T("SetTransportMapError_") << dwErrCode;
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[tss.str().c_str()]);
    }
}

DWORD RoleMgr::OnNetRename( tagNS_LPRename *pNetCmd, DWORD )
{
	if( pNetCmd->dwRoleID != GetLocalPlayerID() )
		return 0;

	switch(pNetCmd->dwErrorCode)
	{
	case E_Success:
		{
			m_pLocalPlayer->SetRoleName( pNetCmd->szNewName );
			PlayerNameTab::Inst()->RoleRename( pNetCmd->szNewName, pNetCmd->dwRoleID );
			tagRoleRenameEvent e( _T("tagRoleRenameEvent"), NULL );
			e.dwRoleID = pNetCmd->dwRoleID;
			e.strRoleName = pNetCmd->szNewName;
			m_pFrameMgr->SendEvent(&e);
		}
		break;
	case E_CreateRole_NameExist:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CreateRole_NameExist")]);
		break;
	case E_Filter_Name_Illegal:
	case E_Filter_Name_InFilterFile:
	case E_CreateRole_NameInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CreateRole_NameInvalid")]);
		break;
	case E_Filter_Name_TooLong:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Common_Error30")]);
		break;
	case E_Filter_Name_Null:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Common_Error31")]);
		break;
	}
	return 0;
}

DWORD RoleMgr::NetRoleRenameBroadcast( tagNS_RoleRename* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID, true);
	if( P_VALID(pRole) )
	{
		ASSERT( pRole->IS_KIND_OF(Player) );
		Player* pPlayer = (Player*)pRole;
		pPlayer->SetRoleName( pNetCmd->szNewName );
		PlayerNameTab::Inst()->RoleRename( pNetCmd->szNewName, pNetCmd->dwRoleID );
		tagRoleRenameEvent e( _T("tagRoleRenameEvent"), NULL );
		e.dwRoleID = pNetCmd->dwRoleID;
		e.strRoleName = pNetCmd->szNewName;
		m_pFrameMgr->SendEvent(&e);
	}
	return 0;
}

DWORD RoleMgr::OnLpChangeStateEvent( tagLPRoleStateChangeEvent *pEvent )
{
	bool bInSafeArea = m_pLocalPlayer->GetRoleState( ERS_SafeArea );
	DWORD dwHostility = m_pLocalPlayer->GetAttribute( ERA_Hostility );
	bool bPK = m_pLocalPlayer->GetRoleState( ERS_PK ) || m_pLocalPlayer->GetRoleState( ERS_PKEX );
	if( bInSafeArea && !( pEvent->dwOldState & ERS_SafeArea ) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Role_InSafeArea")]);
	}
	else if( !bInSafeArea && ( pEvent->dwOldState & ERS_SafeArea ) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Role_OutSafeArea")]);
	}
	return 0;
}

DWORD RoleMgr::OnMousePointRoleIDChangeEvent(tagMousePointRoleIDChangeEvent *pEvent)
{
	if (P_VALID(m_dwRolePointEffectID))
		EffectMgr::Inst()->StopEffect(m_dwRolePointEffectID);

	if (P_VALID(pEvent->dwCurPointRoleID))
	{
		// 如果当前选中的当前指向的相同就不播放特效鸟~
		if (pEvent->dwCurPointRoleID == GetCurTargetID())
			return 0;
		Role *pRole = FindRole(pEvent->dwCurPointRoleID);
		if (P_VALID(pRole))
		{
			// 如果可以攻击
			if (CombatActionChecker::Inst()->IfCanAttack(pRole->GetID()))
				m_dwRolePointEffectID = EffectMgr::Inst()->PlayRoleEffect(pRole->GetID(), _T("Txqt01_4"), NULL, EffectMgr::EEST_XZ);
			else // 不能攻击的
				m_dwRolePointEffectID = EffectMgr::Inst()->PlayRoleEffect(pRole->GetID(), _T("Txqt01_3"), NULL, EffectMgr::EEST_XZ);
		}
	}
	return 0;
}

DWORD RoleMgr::OnRolePickEvent(tagRolePickEvent *pEvent)
{
	if (pEvent->eType == EWPT_LClick)
	{
		if (P_VALID(m_dwRolePointEffectID))
			EffectMgr::Inst()->StopEffect(m_dwRolePointEffectID);
	}
	return 0;
}

DWORD RoleMgr::OnNS_ActiveMoster( tagNS_SyncActiveCreaturesPos *pNetCmd, DWORD )
{
	m_vecActiveMons.clear();
	for( int i = 0; i < pNetCmd->nCreaturesNum; i++)
	{
		const tagCreatureProto *pProto = CreatureData::Inst()->FindNpcAtt( pNetCmd->stData[i].dwTypeID );
		if( P_VALID(pProto) && pProto->bActive )
		{
			m_vecActiveMons.push_back(pNetCmd->stData[i]);
		}
	}
	return 0;
}

bool RoleMgr::IsInSameMasterGuild( Player *pPlayer )
{
	if( P_VALID(pPlayer) && P_VALID(m_pLocalPlayer) )
	{
		DWORD dwLPMasterID = m_pLocalPlayer->GetMasterID();
		DWORD dwPlayerMasterID = pPlayer->GetMasterID();
		DWORD dwLPRoleID = m_pLocalPlayer->GetID();
		DWORD dwPlayerID = pPlayer->GetID();
		if( dwLPRoleID == dwPlayerMasterID || dwPlayerID == dwLPMasterID )
			return true;
		else if( dwLPMasterID != GT_INVALID && dwLPMasterID == dwPlayerMasterID )
			return true;
	}
	return false;
}

VOID RoleMgr::PopUpVoteUI()
{
	// 投票输入框
	if(P_VALID(GetObj("VoteForDragonDaughter")))
		KillObj("VoteForDragonDaughter");

	CreateObj("VoteForDragonDaughter", "MsgInputBox");
	TObjRef<MsgInputBox>("VoteForDragonDaughter")->Init( _T(""), 
		g_StrTable[_T("VoteForDragonDaughter")],
		_T("VoteForDragonDaughter"));
}

VOID RoleMgr::DestroyVoteUI()
{
	if(P_VALID(GetObj("VoteForDragonDaughter")))
		KillObj("VoteForDragonDaughter");
}

DWORD RoleMgr::OnRoleVote( tagMsgInputBoxEvent *pEvent )
{
	if( pEvent->eResult == MBF_OK )
	{
		tagNC_VoteForDrangonGirl cmd;
		cmd.nDrangonGirlID = pEvent->nInputNum;
		m_pSession->Send(&cmd);
	}
	return 0;

}

DWORD RoleMgr::OnNetVote( tagNS_VoteForDrangonGirl *pNetCmd, DWORD )
{
	switch( pNetCmd->dwErrorCode )
	{
	case E_VoteShow_Success:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("VoteSuceess")]);
		break;
	case E_VoteShow_NoNeckLace:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotVote")]);
		break;
	case E_VoteShow_SignIdInvaild:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NoDrangonGirlID")]);
		break;
	case E_VoteShow_UnOpen:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("VoteNotOpen")]);
		break;
	}
	return 0;
}

DWORD RoleMgr::OnNetChangeAvatar( tagNS_AppearanceTransform *pNetCmd, DWORD )
{
	if( pNetCmd->dwRoleID == GetLocalPlayerID() )
	{
		switch( pNetCmd->dwErrorCode )
		{
		case EATE_Success:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ChangAvatarSucc")]);
				tagAvatarAtt att = m_pLocalPlayer->GetAvatarAtt();
				att.wHairMdlID = pNetCmd->wHairMdlID;
				att.wHairTexID = pNetCmd->wHairTexID;
				att.wFaceMdlID = pNetCmd->wFaceMdlID;
				att.wFaceDetailTexID = pNetCmd->wFaceDetailTexID;
				AvatarEquipNode* pNode = (AvatarEquipNode*)m_pLocalPlayer->GetSceneNode();
				if( NULL == pNode )
					return 0;
				pNode->UpdateAllAtt(att);
			}
			break;
		case EATE_NoTransformItem:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NoChangAvatarItem")]);
			break;
		}
	}
	else
	{
		Role* pRole = FindRole( pNetCmd->dwRoleID );
		if( P_VALID( pRole ) && pRole->IS_STATIC_CLASS(Player) )
		{
			Player* pPlayer = (Player*)pRole;
			tagAvatarAtt att = pPlayer->GetAvatarAtt();
			att.wHairMdlID = pNetCmd->wHairMdlID;
			att.wHairTexID = pNetCmd->wHairTexID;
			att.wFaceMdlID = pNetCmd->wFaceMdlID;
			att.wFaceDetailTexID = pNetCmd->wFaceDetailTexID;
			AvatarEquipNode* pNode = (AvatarEquipNode*)pPlayer->GetSceneNode();
			if( NULL == pNode )
				return 0;
			pNode->UpdateAllAtt(att);
		}
	}
	tagUpdataAvatarEvent evt( _T("tagUpdataAvatarEvent"), NULL );
	evt.dwRoleID = pNetCmd->dwRoleID;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);
	return 0;
}

DWORD RoleMgr::OnEvent( tagGameEvent* pEvent )
{
	if( _T("tagStartNPCTalkEvent") == pEvent->strEventName )	// 与NPC对话
	{
		tagStartNPCTalkEvent* pEvt = (tagStartNPCTalkEvent*)pEvent;
		const tagCreatureProto* pCreatureProto = CreatureData::Inst()->FindNpcAtt(pEvt->typeID);
		if( pCreatureProto->eFunctionType >= EFNPCT_QuestPanelStart && pCreatureProto->eFunctionType < EFNPCT_QuestPanelEnd )
		{
			INT nLevel = m_pLocalPlayer->GetRoleLevel();
			if( nLevel < BOARD_QUEST_LIMIT_LEVEL && EFNPCT_QuestPanelReward == pCreatureProto->eFunctionType)
			{
				TCHAR szTemp[X_LONG_NAME] = {0};
				_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("CanNotCommuWithNPC")], BOARD_QUEST_LIMIT_LEVEL );
				CombatSysUtil::Inst()->ShowScreenCenterMsg(szTemp);
			}
			else if( nLevel < YELLOW_QUEST_LIMIT_LEVEL && EFNPCT_QuestPanelYellowList == pCreatureProto->eFunctionType )
			{
				TCHAR szTemp[X_LONG_NAME] = {0};
				_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("CanNotCommuWithNPC")], YELLOW_QUEST_LIMIT_LEVEL );
				CombatSysUtil::Inst()->ShowScreenCenterMsg(szTemp);
			}
			else
			{
				QuestPanelFrame *pFrame = (QuestPanelFrame*)m_pFrameMgr->GetFrame(_T("QuestPanel"));
				if(P_VALID(pFrame))
					m_pFrameMgr->DestroyFrame(pFrame);
				pFrame = (QuestPanelFrame*)m_pFrameMgr->CreateFrame( _T("World"), _T("QuestPanel"), _T("QuestPanelFrame"), 0 );
				if( P_VALID(pFrame) )
				{
					pFrame->SetNPCID(pEvt->globalID, pCreatureProto->eFunctionType);
					tagNC_OpenQuestBoard m;
					m.dwNPCID = pEvt->globalID;
					m_pSession->Send(&m);
				}
			}
		}
		else if (EFNPCT_KeyCodeNPC == pCreatureProto->eFunctionType
		   || EFNPCT_NewKeyCodeNPC == pCreatureProto->eFunctionType )
		{	//key码
			m_pKeyCodeFrame = (KeyCodeFrame*) m_pFrameMgr->GetFrame(_T("KeyCode"));
			if (!P_VALID(m_pKeyCodeFrame))
			{
				m_pKeyCodeFrame = (KeyCodeFrame*) m_pFrameMgr->CreateFrame(_T("World"), _T("KeyCode"), _T("KeyCodeFrame"),0);
			}
			if (P_VALID(m_pKeyCodeFrame))
			{
				m_pKeyCodeFrame->Show(TRUE);
				m_pKeyCodeFrame->SetNPC(pEvt->globalID);
				if (EFNPCT_KeyCodeNPC == pCreatureProto->eFunctionType)
				{
					m_pKeyCodeFrame->SetType(FALSE);
				}
				else //if (EFNPCT_NewKeyCodeNPC == pCreatureProto->eFunctionType)
				{
					m_pKeyCodeFrame->SetType(TRUE);
				}
				
			}
		}
		else if (EFNPCT_TigerNPC == pCreatureProto->eFunctionType)
		{	//key码
			TigerMachineFrame *pFrame = (TigerMachineFrame*)m_pFrameMgr->GetFrame(_T("TigerMachine"));
			if(P_VALID(pFrame))
			{
				pFrame->SetNpcID(pEvt->globalID);
				pFrame->ShowFrame(TRUE);
			}
		}
	}
	return 0;
}

DWORD RoleMgr::OnNetRRoleChangeFabao( tagNS_RemoteRoleChangeFabao *pNetCmd, DWORD )
{
	Role* pRole = FindRole( pNetCmd->dwRoleID );
	if( P_VALID( pRole ) && pRole->IS_STATIC_CLASS(Player) && NULL != pRole->GetSceneNode() )
	{
		Player* pPlayer = (Player*)pRole;
		if(P_VALID(pPlayer->GetMount()))
			pPlayer->ClearTrumpEffect();
		else
			pPlayer->PlayTrumpEffect(pNetCmd->tagFaBaoIno);
	}
	return 0;
}

BOOL RoleMgr::IsEnemy( DWORD dwRoleID )
{
	set<DWORD>::iterator iter = m_setEnemy.find(dwRoleID);
	if( iter != m_setEnemy.end() )
		return TRUE;
	return FALSE;
}

DWORD RoleMgr::OnNetBeAttacked( tagNS_RoleBeAttacked *pNetCmd, DWORD )
{
	Role* pRole = FindRole( pNetCmd->dwEvilRoleID );
	if( P_VALID( pRole ) && pRole->IS_STATIC_CLASS(Player) && NULL != pRole->GetSceneNode() )
	{
		m_setEnemy.insert(pNetCmd->dwEvilRoleID);
		m_dwRecivMsgTime = Kernel::Inst()->GetAccumTimeDW();
	}
	return 0;
}

DWORD RoleMgr::OnRoleLeaveCombatEvent( tagLeaveCombatEvent *pEvent )
{
	m_setEnemy.clear();
	return 0;
}

DWORD RoleMgr::OnRoleEnterCombatEvent( tagEnterCombatEvent *pEvent )
{
	if (P_VALID(m_pLocalPlayer))
	{
		m_pLocalPlayer->HideWeapon(false);
		m_pLocalPlayer->HandUpWeapon(true);
	}
	return 0;
}

DWORD RoleMgr::OnNPCTalkEvent( tagNPCTalkEvent *pEvent )
{
	Role* pRole = RoleMgr::Inst()->FindRole( pEvent->dwNPCID );
	if( !P_VALID( pRole ) || ( !pRole->IsAttributeInited() ) )
		return 0;
	if( pRole->IS_KIND_OF( NPC ) )
	{
		NPC* pNpc = (NPC*)pRole;
		if( pNpc->IsNPC() )
		{
			tagNPCSay *pSay = NPCSayTable::Inst()->FindNPCSayTable(pNpc->GetTypeID());
			if(!P_VALID(pSay))
				return 0;
			ChatShowPop::Inst()->PushChatInfo(pEvent->dwNPCID, pSay->strSay.c_str());
		}
	}

	return 0;
}

DWORD RoleMgr::OnNetShutDownServer( tagNS_WillShutdownWorld *pNetCmd, DWORD )
{
	CountDownFrame *pFrame = (CountDownFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("CountDown") );
	if( !P_VALID(pFrame) )
	{
		pFrame = (CountDownFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("CountDown"), _T("CountDownFrame"), 0 );
		pFrame->ReloadUI( "\\desktop\\targetwin\\buff19", "OuterBottom", "ShutDownWin" );
	}
	pFrame->Show();
	pFrame->SetCDTime(pNetCmd->nSecondsDelay*1000);
	return 0;
}

DWORD RoleMgr::OnNetUseExpPilular( tagNS_UseExpPilular *pNetCmd, DWORD )
{
	switch(pNetCmd->dwErrorCode)
	{
	case E_Success:
		break;
	case E_FabaoStoreExp_NotYourExpPililar:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyExpBallIsNotYours")]);
		break;
	case E_FabaoStoreExp_ExpPililarUseTimesLimit:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("UseExpBallLimite")]);
		break;
	}
	return 0;
}

DWORD RoleMgr::OnNetFriendBeautyMe( tagNS_IsDressUpAgree *pNetCmd, DWORD )
{
	tstring strRoleName = PlayerNameTab::Inst()->FindNameByID(pNetCmd->dwSrcRoleID);
	tstring strFashionName = ClothespressData::Inst()->GetSuitName(pNetCmd->dwWardrobeLayerID);
	if( P_VALID(GetObj("FriendBeautyMe")) )
		KillObj("FriendBeautyMe");
	CreateObj( "FriendBeautyMe", "MsgBox" );
	TCHAR szTips[X_LONG_NAME];
	_sntprintf( szTips, X_LONG_NAME, g_StrTable[_T("BeautyFriendRequest")], strRoleName.c_str(), strFashionName.c_str());
	TObjRef<MsgBox>("FriendBeautyMe")->Init( _T(""), szTips, _T("OnFriendBeautyMe") );
	TObjRef<MsgBox>("FriendBeautyMe")->SetDwordUserData(pNetCmd->dwSrcRoleID);
	return 0;
}

bool RoleMgr::AutoPickRole(DWORD dwRoleID)
{
	LocalPlayer* plp = GetLocalPlayer();
	if(!P_VALID(plp))		return false;

	if(plp->GetID() == dwRoleID ) return true;  // 选择自己

	map<DWORD,Role*>::iterator iter=m_roleMap.find(dwRoleID);
	if (iter != m_roleMap.end())
	{
		if( iter->second->IS_KIND_OF(Player) || iter->second->IS_KIND_OF(NPC) )	// 只能选中NPC和玩家
		{
			if (iter->second->IsDead())	return false;

			float fMinDistSq = FLOAT_MAX;
			const float fDistSq = Vec3DistSq( plp->GetPos(), iter->second->GetPos() );
			if( fDistSq < fMinDistSq )
			{
				tagRolePickEvent evt( _T("tagRolePickEvent"), NULL );
				evt.eType = EWPT_LClick;
				evt.dwRoleID = dwRoleID;
				m_pFrameMgr->SendEvent( &evt );
				return true;
			}

			return false;
		}
		else
			return false;
	
	}
	else
		return false;

	
}

DWORD RoleMgr::OnEventFriendBeautyMe( tagMsgBoxEvent *pGameEvent )
{
	DWORD dwRoleID = TObjRef<MsgBox>("FriendBeautyMe")->GetDwordUserData();
	tagNC_IsDressUpAgree cmd;
	cmd.dwTargetRoleID = dwRoleID;
	if( pGameEvent->eResult == MBF_OK )
	{
		cmd.bAgree = true;

		m_pFrameMgr->SendEvent(&tagGameEvent(_T("OffFashionClothes"), NULL));
		tagChangeFashionEvent pEvent = tagChangeFashionEvent(_T("tagChangeFashionEventEx"), NULL);
		pEvent.bDisFashion = TRUE;
		m_pFrameMgr->SendEvent(&pEvent);
	}
	else
		cmd.bAgree = false;
	m_pSession->Send(&cmd);

	tagRehanding eve(_T("tagRehanding"), NULL);
	eve.dwError = cmd.bAgree ? E_Success : E_SystemError;
	TObjRef<GameFrameMgr>()->SendEvent(&eve);
	return 0;
}

DWORD RoleMgr::OnNS_ChangeClergy(tagNS_ChangeClergy* pNetCmd, DWORD)
{
	Role* pRole = FindRole( pNetCmd->dwRoleID );
	if(P_VALID(pRole))
	{
		((Player*)pRole)->SetPlayerClergy(pNetCmd->dwClergy);
	}
	return 0;
}

DWORD RoleMgr::OnNS_SyncFamilyActive(tagNS_SyncFamilyActive* pNetCmd, DWORD)
{
	if (P_VALID(m_pLocalPlayer))
	{
		m_pLocalPlayer->SetLPActiveDegree(pNetCmd->nActive);
	}
	return 0;
}

DWORD RoleMgr::NetGetRemoteRoleTransSkin(tagNS_RemoteRoleSkin* pNetCmd, DWORD)
{
	Role* pRole = FindRole( pNetCmd->dwRoleID );
	if( P_VALID( pRole ) && pRole->IS_STATIC_CLASS(Player) && pRole->IsAttributeInited())
	{
		Player* pPlayer = (Player*)pRole;
		pPlayer->SetDisplaySet( pNetCmd->sDisplaySet );
		pPlayer->SetTransSkinIndex(pNetCmd->Avatar.byTransSkinIndex);
		tagAvatarEquip	&sTempAvatarEquip = pNetCmd->AvatarEquip;
		if ( 0 != pNetCmd->Avatar.byTransSkinIndex  && GT_INVALID != pNetCmd->Avatar.byTransSkinIndex)
		{
			for(int i = EAE_Head; i <EAE_RWeapon; i++)		// 隐藏所有的装备，除了武器
			{
				sTempAvatarEquip.AvatarEquip[i].dwTypeID = GT_INVALID;
			}
		}
		if (NULL == pRole->GetSceneNode() )
		{
			pPlayer->LoadAvatar( pNetCmd->Avatar, sTempAvatarEquip, pNetCmd->sDisplaySet );
		}
		else
		{
			pPlayer->UpdateAvatar( pNetCmd->Avatar, sTempAvatarEquip, MapMgr::Inst()->GetSceneGraph() );
		}
	}
	return 0;
}

bool RoleMgr::IsTransforming(Player* pPlayer)
{
	if(!P_VALID(pPlayer)) return false;

	tagRoleBuff* pBuff = NULL;
	TList<tagRoleBuff*>& listBuff = pPlayer->GetBuffList();
	listBuff.ResetIterator();
	while( listBuff.PeekNext(pBuff) )
	{
		if( !P_VALID( pBuff ) )
			continue;
		
		if (EBET_Transform == pBuff->pBuffProto->eEffect[EBEM_Persist])
			return true;
	}
	return false;
}

DWORD RoleMgr::OnNS_PlsInputReactiveKeyCode(tagNS_PlsInputReactiveKeyCode* pNetCmd, DWORD)
{
	m_pKeyCodeFrame = (KeyCodeFrame*) m_pFrameMgr->GetFrame(_T("KeyCode"));
	if (!P_VALID(m_pKeyCodeFrame))
	{
		m_pKeyCodeFrame = (KeyCodeFrame*) m_pFrameMgr->CreateFrame(_T("World"), _T("KeyCode"), _T("KeyCodeFrame"),1);
	}
	if (P_VALID(m_pKeyCodeFrame))
	{
		m_pKeyCodeFrame->Show(TRUE);
	}

	return 0;
}

DWORD RoleMgr::OnNS_ReactiveKeyCode(tagNS_ReactiveKeyCode* pNetCmd, DWORD)
{
	switch (pNetCmd->dwErrorCode)
	{
	case E_AccountReactive_Error:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_AccountReactive_Error")]);
		break;
	case E_AccountReactive_AlreadyDone:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_AccountReactive_AlreadyDone")]);
		break;
	case E_AccountReactive_CodeInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_AccountReactive_CodeInvalid")]);
		break;
	}
	return 0;
}
//DWORD RoleMgr::OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam)
//{
//	if(pMsg->dwErrorCode!=E_Success)
//		return 0; 
//
//	const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pMsg->dwSkillID/100);
//	if(P_VALID(pData))
//	{
//		if(ESSF_FreeOpenFace == pData->pProto->eSpecFunc && ESSOF_AWARD	 == pData->pProto->nSpecFuncVal1)
//		{
//			const tagCreatureProto* pCreatureProto = CreatureData::Inst()->FindNpcAtt(pData->pProto->nSpecFuncVal2);
//			if(P_VALID(pCreatureProto))
//			{
//				QuestPanelFrame *pFrame = (QuestPanelFrame*)m_pFrameMgr->GetFrame(_T("QuestPanel"));
//				if(P_VALID(pFrame))
//					m_pFrameMgr->DestroyFrame(pFrame);
//
//				pFrame = (QuestPanelFrame*)m_pFrameMgr->CreateFrame( _T("World"), _T("QuestPanel"), _T("QuestPanelFrame"), 0 );
//				if( P_VALID(pFrame) )
//				{
//					//2000000030
//					pFrame->SetNPCID(0, pCreatureProto->eFunctionType);
//					tagNC_OpenQuestBoard m;
//					m.dwNPCID = 0;
//					m.dwNPCTypeID = pCreatureProto->dwTypeID;
//					m_pSession->Send(&m);
//				}
//			}
//		}
//	}
//
//	return 0;
//}

//通过角色id获取其虚拟角色(元神觉醒)
VirtualRole* RoleMgr::GetVirtualRole(DWORD dwRoleID)
{
	map<DWORD, VirtualRole*>::iterator it = m_mapVirtualRole.find(dwRoleID);
	if ( it!=m_mapVirtualRole.end() )
	{
		return it->second;
	}
	return NULL;
}

/** 通过角色id获取是否有虚拟角色(元神觉醒)*/
BOOL RoleMgr::HasVirtualRole(DWORD dwRoleID)
{
	map<DWORD, VirtualRole*>::iterator it = m_mapVirtualRole.find(dwRoleID);
	if ( it!=m_mapVirtualRole.end() )
		return TRUE;
	return FALSE;
}

/** 同步虚拟角色和人物动作(元神觉醒)*/
VOID RoleMgr::SynVRCombatAction( DWORD dwRoleID, tstring strActionName )
{
	tstring trackName = HolyManMgr::Inst()->GetMatchCombatAction(strActionName);
	if ( trackName.empty() )
		trackName = _T("std");
	map<DWORD, VirtualRole*>::iterator it = m_mapVirtualRole.find(dwRoleID);
	if ( it!=m_mapVirtualRole.end() )
	{
		VirtualRole* pVR = it->second;
		if (P_VALID(pVR))
			pVR->SynCombatAction(trackName);
	}

}

/**家在元神的VirtualRole模型*/
BOOL RoleMgr::LoadHolyManModel(Player* pPlayer, Vector3 v3FaceTo, DWORD dwModelID/*, tagAvatarAtt	Avatar, tagAvatarEquip	AvatarEquip, tagDisplaySet	DisplaySet*/ )
{
	if ( !P_VALID(pPlayer) || GT_INVALID == dwModelID )
		return FALSE;

	VirtualRole* pVirtualRole=new VirtualRole;
	pVirtualRole->InitFSM(pPlayer->GetID());
	if (P_VALID(pVirtualRole)&&(NULL==pVirtualRole->GetSceneNode()))
	{
		//pVirtualRole->LoadAvatar(Avatar, AvatarEquip, DisplaySet);
		const tagAvatarAtt lpAtt = pPlayer->GetAvatarAtt();
		tstring resName =  HolyManMgr::Inst()->GetHolyManSkelName(dwModelID,lpAtt.bySex);
		if ( !resName.empty() )
		{
			pVirtualRole->LoadVirtualAvatar( resName.c_str() );
			pVirtualRole->SetHolyManID(dwModelID);
			pVirtualRole->CallHolyManInit();
			pVirtualRole->LoadVirtualEffect( resName.c_str() );
			pVirtualRole->Hide(TRUE);
		}
		pVirtualRole->OnGotoMap(pPlayer->GetPos(), v3FaceTo);
		m_mapVirtualRole.insert(make_pair(pPlayer->GetID(), pVirtualRole));
	}
	else
	{
		SAFE_DELETE(pVirtualRole);
		return FALSE;
	}
	return TRUE;

} 

BOOL RoleMgr::LoadHolyManModel(BOOL bIsHide)
{
	Vector3 v3FaceTo = m_pLocalPlayer->GetFaceTo();
	if ( !P_VALID(m_pLocalPlayer) )
		return FALSE;

	VirtualRole* pVirtualRole=new VirtualRole;
	pVirtualRole->InitFSM(m_pLocalPlayer->GetID());
	if (P_VALID(pVirtualRole)&&(NULL==pVirtualRole->GetSceneNode()))
	{
		//pVirtualRole->LoadAvatar(Avatar, AvatarEquip, DisplaySet);
		const tagAvatarAtt lpAtt = m_pLocalPlayer->GetAvatarAtt();
		Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Holy);
		if(!P_VALID(pEquip))
			return FALSE;

		const tagSoulAttName* pProto = ItemProtoData::Inst()->FindShenLingName(pEquip->GetItemTypeID());
		if(!P_VALID(pProto))
			return FALSE;

		const tagHolySpec* pSpec = pEquip->GetHolySpec();
		if(!P_VALID(pSpec))
			return FALSE;

		pVirtualRole->SetHolyManID(pProto->dwTypeID);

		tstring resName;
		if(pSpec->nDevourNum >= pProto->dwHighEatNum)
		{
			resName = pProto->szMode_High;
		}
		else 
			resName = pProto->szMode_Low;

		if ( !resName.empty() )
		{
			pVirtualRole->LoadVirtualAvatar( resName.c_str() );
			//pVirtualRole->SetHolyManID(dwModelID);
			pVirtualRole->CallHolyManInit();
			pVirtualRole->LoadVirtualEffect( resName.c_str() );
			pVirtualRole->Hide(TRUE);
		}
		pVirtualRole->OnGotoMap(m_pLocalPlayer->GetPos(), v3FaceTo);
		map<DWORD, VirtualRole*>::iterator it = m_mapVirtualRole.find(m_pLocalPlayer->GetID());
		if(it != m_mapVirtualRole.end())
		{
			VirtualRole* pVR = (*it).second;
			pVR->DetachSceneNodeFromSG( MapMgr::Inst()->GetSceneGraph() );
			pVR->UnloadTransformModel( TRUE, m_pLocalPlayer->GetID() );
			SAFE_DELETE(pVR);
			m_mapVirtualRole.erase(it);
		}
		m_mapVirtualRole.insert(make_pair(m_pLocalPlayer->GetID(), pVirtualRole));
		pVirtualRole->Hide(bIsHide);
	}
	else
	{
		SAFE_DELETE(pVirtualRole);
		return FALSE;
	}
	return TRUE;
}

BOOL RoleMgr::LoadHolyManModel(DWORD dwRoleID, DWORD dwHolyTypeID, DWORD dwEatFairyNum)
{

	Role* pRole = FindRole( dwRoleID );
	if( P_VALID( pRole ) )
	{
		Player* pPlayer = (Player*)pRole;
		Vector3 v3FaceTo = pPlayer->GetFaceTo();
		VirtualRole* pVirtualRole=new VirtualRole;
		pVirtualRole->InitFSM(pPlayer->GetID());
		if (P_VALID(pVirtualRole)&&(NULL==pVirtualRole->GetSceneNode()))
		{
			const tagSoulAttName* pProto = ItemProtoData::Inst()->FindShenLingName(dwHolyTypeID);
			if(!P_VALID(pProto))
				return FALSE;

			pVirtualRole->SetHolyManID(dwHolyTypeID);

// 			const tagHolySpec* pSpec = pEquip->GetHolySpec();
// 			if(!P_VALID(pSpec))
// 				return FALSE;

			tstring resName;
 			if( dwEatFairyNum >= pProto->dwHighEatNum)
 			{
 				resName = pProto->szMode_High;
 			}
 			else 
				resName = pProto->szMode_Low;

			if ( !resName.empty() )
			{
				pVirtualRole->LoadVirtualAvatar( resName.c_str() );
				//pVirtualRole->SetHolyManID(dwModelID);
				pVirtualRole->CallHolyManInit();
				pVirtualRole->LoadVirtualEffect( resName.c_str() );
				pVirtualRole->Hide(TRUE);
			}
			pVirtualRole->OnGotoMap(pPlayer->GetPos(), v3FaceTo);
			map<DWORD, VirtualRole*>::iterator it = m_mapVirtualRole.find(dwRoleID);
			if(it != m_mapVirtualRole.end())
			{
				VirtualRole* pVR = (*it).second;
				pVR->DetachSceneNodeFromSG( MapMgr::Inst()->GetSceneGraph() );
				pVR->UnloadTransformModel( TRUE, m_pLocalPlayer->GetID() );
				SAFE_DELETE(pVR);
				m_mapVirtualRole.erase(it);
			}
			m_mapVirtualRole.insert(make_pair(dwRoleID, pVirtualRole));
		}
		else
		{
			SAFE_DELETE(pVirtualRole);
			return FALSE;
		}
	}
	return TRUE;
}

// VOID RoleMgr::HolyManCtrl(DWORD dwRoleID, EPLayActionType eType, DWORD dwBuffID/*=GT_INVALID*/)
// {
// 	if ( dwBuffID != GT_INVALID)
// 	{
// 		DWORD dwBuffTypeID = dwBuffID/100;
// 		if ( dwBuffTypeID != HOLYSOUL_BUFFID ) //元神召唤buff id
// 			return;
// 	}
// 
// 	VirtualRole*pVp =  RoleMgr::Inst()->GetVirtualRole(dwRoleID);
// 	if ( P_VALID(pVp) )
// 	{
// 		BOOL bAwake = ! pVp->IsHide();
// 		if (  (bAwake && eType==EPAT_CALL)							//防止重复召唤和消失
// 			|| ( !bAwake && eType==EPAT_DISPPEAR ) )
// 			return;
// 
// 		tagVirtualRoleEvent event;
// 		event.ePlayAct = eType;
// 		pVp->OnGameEvent(&event);
// 	}
// }

DWORD RoleMgr::OnNS_RemoteHolySoulAttChange( tagNS_RemoteHolySoulAttChange *pNetCmd, DWORD )
{
	ReloadHolyManModel( pNetCmd->dwRoleID, pNetCmd->dwSoulID, pNetCmd->eOptType );
	return 0;
}

//重载元神模型
BOOL RoleMgr::ReloadHolyManModel(DWORD dwRoleID, DWORD dwHolyID, ESoulAttOptType eOpt )
{
	Role* pRole = FindRole(dwRoleID);
	SceneGraph* pSG = MapMgr::Inst()->GetSceneGraph();
	//if ( !P_VALID(pSG) || !P_VALID(pRole) || !pRole->IS_STATIC_CLASS(Player) 
	//	|| 0 == dwHolyID || GT_INVALID == dwHolyID )  
	//	return FALSE;

	if ( !P_VALID(pSG) ||!P_VALID(pRole)  )  
		return FALSE;

	if ( !(pRole->IS_KIND_OF(Player)) )  
		return FALSE;
	if ( 0 == dwHolyID || GT_INVALID == dwHolyID )  
		return FALSE;

	Player* pPlayer = (Player*)pRole;
	if ( NULL==pPlayer->GetSceneNode() )
		return FALSE;
	//释放旧的虚拟角色
	BOOL bOriAwake = FALSE;		//原本是否唤醒
	map<DWORD, VirtualRole*>::iterator itVR = m_mapVirtualRole.find( dwRoleID );
	if ( itVR != m_mapVirtualRole.end() )
	{
		VirtualRole* pVR = itVR->second;
		if ( P_VALID(pVR) )
		{
			bOriAwake = ! pVR->IsHide();
			if ( pVR->GetHolyManID() != dwHolyID )
			{
				pVR->Hide(TRUE);
				pVR->DetachSceneNodeFromSG( pSG );
				pVR->UnloadTransformModel( TRUE, dwRoleID );
				SAFE_DELETE(pVR);
			}
			else	//没有变化
				return FALSE;
		}
		m_mapVirtualRole.erase( dwRoleID );
	}	
	//加载新的虚拟角色
	Vector3 v3FaceTo = pPlayer->GetFaceTo();
	//LoadHolyManModel( pPlayer, v3FaceTo, dwHolyID);
	//LoadHolyManModel(pPlayer, v3FaceTo);

	//防止重复召唤和消失
// 	if ( ESAOType_Call == eOpt && !bOriAwake )
// 		HolyManCtrl( dwRoleID, EPAT_CALL );

	map<DWORD, VirtualRole*>::iterator itNewVR = m_mapVirtualRole.find( dwRoleID );
	if ( bOriAwake &&  itNewVR != m_mapVirtualRole.end() )
	{
		VirtualRole* pVR = itNewVR->second;
		if ( P_VALID(pVR) )
			pVR->Hide(FALSE);
	}


	return TRUE;
}

// DWORD RoleMgr::OnNS_GetRoleAttHolySoulEffect( tagNS_GetRoleAttHolySoulEffect *pNetCmd, DWORD )
// {
// 	m_pLocalPlayer->SetAttribute(ERA_WeaponDmgMin,		pNetCmd->nMinWeaponDmg,		false);
// 	m_pLocalPlayer->SetAttribute(ERA_WeaponDmgMax,		pNetCmd->nMaxWeaponDmg,	false);
// 	m_pLocalPlayer->SetAttribute(ERA_WeaponSoul,				pNetCmd->nWeaponSoul,			false);
// 	m_pLocalPlayer->SetAttribute(ERA_HitRate,						pNetCmd->nHit,							false);
// 	m_pLocalPlayer->SetAttribute(ERA_Crit_Rate,					pNetCmd->nCrit,							false);
// 	m_pLocalPlayer->SetAttribute(ERA_Crit_Amount,				pNetCmd->nCritAmount,				false);
// 	m_pLocalPlayer->SetAttribute(ERA_ExAttack,					pNetCmd->nExAttack,					false);
// 	m_pLocalPlayer->SetAttribute(ERA_InAttack,						pNetCmd->nInAttack,					false);
// 	m_pLocalPlayer->SetAttribute(ERA_Accurate,					pNetCmd->nAccurate,					false);
// 	m_pLocalPlayer->SetAttribute(ERA_ExDamage,					pNetCmd->nExDamage,				false);
// 	m_pLocalPlayer->SetAttribute(ERA_Neglect_Toughness,	pNetCmd->nNeglectToughness,	false);
// 	m_pLocalPlayer->SetAttribute(ERA_Morale,						pNetCmd->nMorale,					false);
// 	return 0;
// }

DWORD RoleMgr::OnNS_CallShengLing(tagNS_CallShengLing* pCmd, DWORD)
{
	if(pCmd->dwErrorCode == E_Success)
	{
		if(pCmd->dwPlayID != m_pLocalPlayer->GetID())
		{
			Role* pRole = FindRole( pCmd->dwPlayID);//add by xp
			Player* pPlayer = (Player*)pRole;
			pPlayer->SetRoleState(ERS_CALLHOLY, TRUE);//add end
			LoadHolyManModel(pCmd->dwPlayID, pCmd->dwShengLingTypeID, pCmd->dwEatFairyNum);
		}
		else
			m_pLocalPlayer->SetRoleState(ERS_CALLHOLY, TRUE);
		VirtualRole*pVp =  RoleMgr::Inst()->GetVirtualRole(pCmd->dwPlayID);
		if ( P_VALID(pVp) )
		{
			tagVirtualRoleEvent event;
			event.ePlayAct = EPAT_CALL;
			pVp->OnGameEvent(&event);
		}
	}
	else
	{
		switch(pCmd->dwErrorCode)
		{
		case E_CallShengLing_HolyNotEnough:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CallShengLing_HolyNotEnough")]);
			break;
		case E_CallShengLing_EquipInValid:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CallShengLing_EquipInValid")]);
			break;
		case E_CallShengLing_ForMountState:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CallShengLing_ForMountState")]);
			break;
		case E_CallShengLing_UseSkillState:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CallShengLing_UseSkillState")]);
			break;
		}		
	}			

	
	return 0;
}

DWORD RoleMgr::OnNS_CallShengLingDisappear(tagNS_CallShengLingDisappear*pCmd, DWORD)
{
	if(pCmd->dwPlayerID != m_pLocalPlayer->GetID())
	{
		Role* pRole = FindRole( pCmd->dwPlayerID);//add by xp
		Player* pPlayer = (Player*)pRole;
		pPlayer->SetRoleState(ERS_CALLHOLY, FALSE);//add end
		map<DWORD, VirtualRole*>::iterator itVR = m_mapVirtualRole.find(pCmd->dwPlayerID);
		if ( itVR != m_mapVirtualRole.end() )
		{
			VirtualRole* pVR = itVR->second;
			if ( P_VALID(pVR) )
			{
				pVR->DetachSceneNodeFromSG( MapMgr::Inst()->GetSceneGraph() );
				pVR->UnloadTransformModel();
				SAFE_DELETE(pVR);
			}
			m_mapVirtualRole.erase(pCmd->dwPlayerID);
		}	
	}
	else
	{
		if(m_pLocalPlayer->GetRoleState()&ERS_CALLHOLY)
			m_pLocalPlayer->SetRoleState(ERS_CALLHOLY, FALSE);
		VirtualRole*pVp =  RoleMgr::Inst()->GetVirtualRole(pCmd->dwPlayerID);
		if ( P_VALID(pVp) )
		{
			tagVirtualRoleEvent event;
			event.ePlayAct = EPAT_DISPPEAR;
			pVp->OnGameEvent(&event);
		}
	}

	
	return 0;
}

 DWORD RoleMgr::OnNS_SyncCamp(tagNS_SyncCamp* pCmd, DWORD)
 {
 	Role* pRole = FindRole(pCmd->dwRoleID);
 	if(P_VALID(pRole))
 	{
 		pRole->SetRoleCampType(pCmd->eCamp);
 		return 0;
 	}
 
 	return 0;
 }

// DWORD RoleMgr::OnNS_UnSetRoleState(tagNS_UnSetRoleState* pCmd, DWORD)
// {
// 	if(pCmd->eState != ERS_CALLHOLY)
// 		return 0;
// 
// 	if(pCmd->dwRoleID != m_pLocalPlayer->GetID())
// 	{
// 		map<DWORD, VirtualRole*>::iterator itVR = m_mapVirtualRole.find(pCmd->dwRoleID);
// 		if ( itVR != m_mapVirtualRole.end() )
// 		{
// 			VirtualRole* pVR = itVR->second;
// 			if ( P_VALID(pVR) )
// 			{
// 				pVR->DetachSceneNodeFromSG( MapMgr::Inst()->GetSceneGraph() );
// 				pVR->UnloadTransformModel();
// 				SAFE_DELETE(pVR);
// 			}
// 			m_mapVirtualRole.erase(pCmd->dwRoleID);
// 		}	
// 	}
// 	else
// 	{
// 		if(m_pLocalPlayer->GetRoleState()&ERS_CALLHOLY)
// 			m_pLocalPlayer->SetRoleState(ERS_CALLHOLY, FALSE);
// 		VirtualRole*pVp =  RoleMgr::Inst()->GetVirtualRole(pCmd->dwRoleID);
// 		if ( P_VALID(pVp) )
// 		{
// 			tagVirtualRoleEvent event;
// 			event.ePlayAct = EPAT_DISPPEAR;
// 			pVp->OnGameEvent(&event);
// 		}
// 	}
// 
// 
// 	return 0;
// }

void RoleMgr::SendyunLianPauthGame()
{
	SendHttpPost sendHttpPost;
	stringstream strHttp;
	strHttp<< "/index.php?directive=GameClose|"<<m_pUtil->UnicodeToAnsi(SaveAccount::Inst()->GetAccount().c_str())<<"|15|";
	string strUserMacMD5;
	strUserMacMD5 += m_pUtil->UnicodeToAnsi(SaveAccount::Inst()->GetAccount().c_str());
	strUserMacMD5 += "15";
	strUserMacMD5 += "kzlieling";
	BYTE digest[16];
	m_pUtil->MD5ForString( (char*)strUserMacMD5.c_str(), digest );
	string strPswMD5;
	m_pUtil->MD5ToString( strPswMD5, digest );
	strHttp << strPswMD5;
	LPCWSTR FinalStr;
	FinalStr = m_pUtil->AnsiToUnicode(strHttp.str().c_str());
	ShellExecute(NULL, _T("open"), _T("client\\YunLianSend.exe"), FinalStr, NULL, SW_SHOW);
}