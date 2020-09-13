#include "StdAfx.h"

#include "time.h"
#include "loongRank.h"
#include "../WorldDefine/script_data.h"

CLoongRank::CLoongRank()
{
	m_pDBLoongRank = new Beton::DataBase;
	m_bOn = FALSE;
}

CLoongRank::~CLoongRank()
{
	SAFE_DEL(m_pDBLoongRank);
}

BOOL CLoongRank::Init(LPCTSTR szIniFileName,BOOL bReload)
{
	// 创建容器,并载入配置文件
	if( !bReload )
	{
		CreateObj("LoongDBVar", "VarContainer");
		TObjRef<VarContainer> pVar = "LoongDBVar";
		if(!P_VALID(pVar))
		{
			ERR(_T("Create LoongDBVar(VarContainer) obj failed!"));
			return FALSE;
		}

		pVar->Load("VirtualFileSys", szIniFileName);


		// 创建并初始化游戏数据库引擎
		/*if(!m_pDBLoong->Init(_T("172.17.1.201"), _T("root"), _T("kungfu"), _T("loong_base"), 3306))*/

		if(!m_pDBLoongRank->Init(pVar->GetString(_T("host_name db_game")),
			pVar->GetString(_T("user_name db_game")), 
			pVar->GetString(_T("password db_game")), 
			pVar->GetString(_T("db_name db_game")), 
			(INT)pVar->GetDword(_T("port db_game"))))
		{
			ERR(_T("Game Database initialize failed! Please check profile..."));
			SAFE_DEL(m_pDBLoongRank);
			return FALSE;
		}

		// 清空容器
		pVar->Clear();

		// 销毁容器
		KillObj("LoongDBVar");
	}
	else
	{
		if( P_VALID(m_pDBLoongRank) && m_pDBLoongRank->Reconnect(FALSE) == FALSE )
		{
			SAFE_DEL(m_pDBLoongRank);
			return FALSE;
		}
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// 更新实力排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoongRank::UpdateStrengthRankings()
{
	m_pDBLoongRank->Execute("delete from strength_rankings_pre");
	m_pDBLoongRank->Execute("insert into strength_rankings_pre select * from strength_rankings");
	m_pDBLoongRank->Execute("delete from Strength_rankings");
// 	m_pDBLoongRank->Execute("insert into Strength_rankings select 0, 0, Strength, RoleName from roledata where removeflag=0 and CanRankFlag=1 order by Strength desc,roleid asc limit 500");
// 	m_pDBLoongRank->Execute("update Strength_rankings a set a.num = (select count(0)+1 from (select * from Strength_rankings) b where (b.Strength>a.Strength or (b.Strength=a.Strength and b.RoleName<a.RoleName)))");
// 	m_pDBLoongRank->Execute("update strength_rankings left join strength_rankings_pre on strength_rankings_pre.rolename = strength_rankings.rolename set strength_rankings.prenum = strength_rankings_pre.num");
	m_pDBLoongRank->Execute("insert into Strength_rankings select 0, 0, RoleName, YaoJingValue, RoleID from roledata where removeflag=0 and CanRankFlag=1 order by YaoJingValue desc,roleid asc limit 500");
	m_pDBLoongRank->Execute("update Strength_rankings a set a.num = (select count(0)+1 from (select * from Strength_rankings) b where (b.YaoJingValue>a.YaoJingValue or (b.YaoJingValue=a.YaoJingValue and b.RoleID<a.RoleID)))");
	m_pDBLoongRank->Execute("update strength_rankings left join strength_rankings_pre on strength_rankings_pre.rolename = strength_rankings.rolename set strength_rankings.prenum = strength_rankings_pre.num");
	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 更新声望排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoongRank::UpdateFameRankings()
{
	m_pDBLoongRank->Execute("delete from fame_rankings_pre");
	m_pDBLoongRank->Execute("insert into fame_rankings_pre select * from fame_rankings");
	m_pDBLoongRank->Execute("delete from fame_rankings");
	m_pDBLoongRank->Execute("insert into fame_rankings select 0, 0, TotalFame, RoleID from clan_data where removeflag=0 order by TotalFame desc limit 500");
	m_pDBLoongRank->Execute("update fame_rankings a set a.num = (select count(0)+1 from (select * from fame_rankings) b where (b.fame>a.fame or (b.fame=a.fame and b.roleid<a.roleid)) )");
	m_pDBLoongRank->Execute("update fame_rankings left join fame_rankings_pre on fame_rankings_pre.roleid = fame_rankings.roleid set fame_rankings.prenum = fame_rankings_pre.num");
	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 更新等级排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoongRank::UpdateLevelRankings()
{
	m_pDBLoongRank->Execute("delete from level_rankings_pre");
	m_pDBLoongRank->Execute("insert into level_rankings_pre select * from level_rankings");
	m_pDBLoongRank->Execute("delete from level_rankings");
	m_pDBLoongRank->Execute("insert into level_rankings select 0, 0, WorkedLevel, RoleName, RoleID from roledata where removeflag=0 and CanRankFlag=1 order by WorkedLevel desc,roleid asc limit 500");
	m_pDBLoongRank->Execute("update level_rankings a set a.num = (select count(0)+1 from (select * from level_rankings) b where (b.level>a.level or (b.level=a.level and b.RoleID<a.RoleID)))");
	m_pDBLoongRank->Execute("update level_rankings left join level_rankings_pre on level_rankings_pre.rolename = level_rankings.rolename set level_rankings.prenum = level_rankings_pre.num");
	return E_Success;
} 

//-------------------------------------------------------------------------------------------------------
// 更新宠物排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoongRank::UpdatePetRankings()
{
	m_pDBLoongRank->Execute("delete from pet_rankings_pre");
	m_pDBLoongRank->Execute("insert into pet_rankings_pre select * from pet_rankings");
	m_pDBLoongRank->Execute("delete from pet_rankings");
	m_pDBLoongRank->Execute("insert into pet_rankings select 0, 0, pet_value, pet_id, pet_name, master_id from pet_data where RemoveFlag=0 order by pet_value desc,pet_id asc limit 500");
	m_pDBLoongRank->Execute("update pet_rankings a set a.num = (select count(0)+1 from (select * from pet_rankings) b where (b.Pet>a.Pet or (b.Pet=a.Pet and b.PetID<a.PetID)))");
	m_pDBLoongRank->Execute("update pet_rankings left join pet_rankings_pre on pet_rankings_pre.petid = pet_rankings.petid set pet_rankings.prenum = pet_rankings_pre.num");
	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 更新装备排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoongRank::UpdateEquipRankings()
{
	m_pDBLoongRank->Execute("delete from equip_rankings_pre");
	m_pDBLoongRank->Execute("insert into equip_rankings_pre select * from equip_rankings");
	m_pDBLoongRank->Execute("delete from equip_rankings");
	m_pDBLoongRank->Execute("insert into equip_rankings select 0, 0, EquipValue, RoleID from roledata where RemoveFlag=0 and CanRankFlag=1 order by EquipValue desc,RoleID asc limit 500");
	m_pDBLoongRank->Execute("update equip_rankings a set a.num = (select count(0)+1 from (select * from equip_rankings) b where (b.Equip>a.Equip or (b.Equip=a.Equip and b.RoleID<a.RoleID)))");
	m_pDBLoongRank->Execute("update equip_rankings left join equip_rankings_pre on equip_rankings_pre.roleid = equip_rankings.roleid set equip_rankings.prenum = equip_rankings_pre.num");
	return E_Success;
}


//-------------------------------------------------------------------------------------------------------
// 更新武勋排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoongRank::UpdateWuXunRankings()
{
	m_pDBLoongRank->Execute("delete from wuxun_rankings_pre");
	m_pDBLoongRank->Execute("insert into wuxun_rankings_pre select * from wuxun_rankings");
	m_pDBLoongRank->Execute("delete from wuxun_rankings");
	m_pDBLoongRank->Execute("insert into wuxun_rankings select 0, 0, wuxun, RoleName, RoleID from roledata where removeflag=0 and CanRankFlag=1 order by wuxun desc,roleid asc limit 500");
	m_pDBLoongRank->Execute("update wuxun_rankings a set a.num = (select count(0)+1 from (select * from wuxun_rankings) b where (b.wuxun>a.wuxun or (b.wuxun=a.wuxun and b.RoleID<a.RoleID)))");
	m_pDBLoongRank->Execute("update wuxun_rankings left join wuxun_rankings_pre on wuxun_rankings_pre.rolename = wuxun_rankings.rolename set wuxun_rankings.prenum = wuxun_rankings_pre.num");
	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 更新鲜花排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoongRank::UpdateLikeRankings()
{
	m_pDBLoongRank->Execute("delete from like_rankings_pre");
	m_pDBLoongRank->Execute("insert into like_rankings_pre select * from like_rankings");
	m_pDBLoongRank->Execute("delete from like_rankings");
	m_pDBLoongRank->Execute("insert into like_rankings select 0, 0, flower_num, RoleName, RoleID from roledata where removeflag=0 and CanRankFlag=1 and Sex=0 order by flower_num desc,roleid asc limit 60");
	m_pDBLoongRank->Execute("update like_rankings a set a.num = (select count(0)+1 from (select * from like_rankings) b where (b.flowerNum>a.flowerNum or (b.flowerNum=a.flowerNum and b.RoleID<a.RoleID)))");
	m_pDBLoongRank->Execute("update like_rankings left join like_rankings_pre on like_rankings_pre.rolename = like_rankings.rolename set like_rankings.prenum = like_rankings_pre.num");
	return E_Success;
} 

//-------------------------------------------------------------------------------------------------------
// 更新鸡蛋排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoongRank::UpdateHateRankings()
{
	m_pDBLoongRank->Execute("delete from hate_rankings_pre");
	m_pDBLoongRank->Execute("insert into hate_rankings_pre select * from hate_rankings");
	m_pDBLoongRank->Execute("delete from hate_rankings");
	m_pDBLoongRank->Execute("insert into hate_rankings select 0, 0, egg_num, RoleName, RoleID from roledata where removeflag=0 and CanRankFlag=1 and Sex=0 order by egg_num desc,roleid asc limit 60");
	m_pDBLoongRank->Execute("update hate_rankings a set a.num = (select count(0)+1 from (select * from hate_rankings) b where (b.EggNum>a.EggNum or (b.EggNum=a.EggNum and b.RoleID<a.RoleID)))");
	m_pDBLoongRank->Execute("update hate_rankings left join hate_rankings_pre on hate_rankings_pre.rolename = hate_rankings.rolename set hate_rankings.prenum = hate_rankings_pre.num");
	return E_Success;
} 

DWORD CLoongRank::UpdateClergyMars()
{
	m_pDBLoongRank->Execute("delete from Clergy_Mars");
	m_pDBLoongRank->Execute("insert into Clergy_Mars select 0, RoleID, Sex, 0, ClergyMarsPoint, Clergy4SeniorOrJunior, 0 from roledata where removeflag=0 and ClergyCandidateType=0 and Clergy4SeniorOrJunior != 0 order by ClergyMarsPoint desc,RoleID asc");
	m_pDBLoongRank->Execute("update Clergy_Mars a set a.Num = (select count(0)+1 from (select * from Clergy_Mars) b where (b.ClergyPoint>a.ClergyPoint or (b.ClergyPoint=a.ClergyPoint and b.RoleID<a.RoleID)))");
	return E_Success;
}

DWORD CLoongRank::UpdateClergyApollo()
{
	m_pDBLoongRank->Execute("delete from Clergy_Apollo");
	m_pDBLoongRank->Execute("insert into Clergy_Apollo select 0, RoleID, Sex, 0, ClergyApolloPoint, Clergy4SeniorOrJunior, 0 from roledata where removeflag=0 and ClergyCandidateType=1 and Clergy4SeniorOrJunior != 0 order by ClergyApolloPoint desc,RoleID asc");
	m_pDBLoongRank->Execute("update Clergy_Apollo a set a.Num = (select count(0)+1 from (select * from Clergy_Apollo) b where (b.ClergyPoint>a.ClergyPoint or (b.ClergyPoint=a.ClergyPoint and b.RoleID<a.RoleID)))");
	return E_Success;
}

DWORD CLoongRank::UpdateClergyRabbi()
{
	m_pDBLoongRank->Execute("delete from Clergy_Rabbi");
	m_pDBLoongRank->Execute("insert into Clergy_Rabbi select 0, RoleID, Sex, 0, ClergyRabbiPoint, Clergy4SeniorOrJunior, 0 from roledata where removeflag=0 and ClergyCandidateType=2 and Clergy4SeniorOrJunior != 0 order by ClergyRabbiPoint desc,RoleID asc");
	m_pDBLoongRank->Execute("update Clergy_Rabbi a set a.Num = (select count(0)+1 from (select * from Clergy_Rabbi) b where (b.ClergyPoint>a.ClergyPoint or (b.ClergyPoint=a.ClergyPoint and b.RoleID<a.RoleID)))");
	return E_Success;
}

DWORD CLoongRank::UpdateClergyPeace()
{
	m_pDBLoongRank->Execute("delete from Clergy_Peace");
	m_pDBLoongRank->Execute("insert into Clergy_Peace select 0, RoleID, Sex, 0, ClergyPeacePoint, Clergy4SeniorOrJunior, 0 from roledata where removeflag=0 and ClergyCandidateType=3 and Clergy4SeniorOrJunior != 0 order by ClergyPeacePoint desc,RoleID asc");
	m_pDBLoongRank->Execute("update Clergy_Peace a set a.Num = (select count(0)+1 from (select * from Clergy_Peace) b where (b.ClergyPoint>a.ClergyPoint or (b.ClergyPoint=a.ClergyPoint and b.RoleID<a.RoleID)))");
	return E_Success;
}

VOID CLoongRank::RefreshClergyCandidateType()
{
	m_pDBLoongRank->Execute("update roledata set ClergyCandidateType=-1");
}
