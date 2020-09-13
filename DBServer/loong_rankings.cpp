#include "StdAfx.h"
#include "LoongDB.h"
#include "Loong.h"
#include "loongRank.h"
#include "../ServerDefine/rankings_define.h"
#include "../ServerDefine/msg_rankings.h"

class CLoongRank;
//-------------------------------------------------------------------------------------------------------
// 加载各种排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InitRankings(DWORD pDBMsg, DWORD dwDummy)
{
	LoadStrengthRankings();
	LoadFameRankings();
	LoadLevelRankings();
	LoadPetRankings();
	LoadEquipRankings();
	LoadWuXunRankings();
	LoadFlowerRankings();
	LoadEggRankings();

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 加载实力排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadStrengthRankings()
{
	if (!m_pDBRank->IsNeedLoad(ERT_Strength))
	{
		return E_Success;
	}

	tagRankingsNode* pRankingsNode = new tagRankingsNode[MAX_RANKINGS_LEN];
	//tagRankingsNode* pCur = pRankingsNode; 
	INT nNum;

	if (!m_pDB->LoadStrengthRankings(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return GT_INVALID;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitRankings send;
	send.nSum = nNum;
	send.eType = ERT_Strength;
	for (INT i=0; i<nNum && i<MAX_RANKINGS_LEN; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].nPreNum = pRankingsNode[i].nPreNum;
		pRankingsNode[i].szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s(send.Rankings[i].szRoleName, X_SHORT_NAME, pRankingsNode[i].szRoleName);
		send.Rankings[i].n64Value = pRankingsNode[i].n64Value;
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;

	return E_Success;

}

//-------------------------------------------------------------------------------------------------------
// 加载声望排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadFameRankings()
{
	if (!m_pDBRank->IsNeedLoad(ERT_Fame))
	{
		return E_Success;
	}

	tagRankingsNode* pRankingsNode = new tagRankingsNode[MAX_RANKINGS_LEN];
	//tagRankingsNode* pCur = pRankingsNode;
	INT nNum;

	if (!m_pDB->LoadFameRankings(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return GT_INVALID;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitRankings send;
	send.nSum = nNum;
	send.eType = ERT_Fame;
	for (INT i=0; i<nNum && i<MAX_RANKINGS_LEN; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].nPreNum = pRankingsNode[i].nPreNum;
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
		send.Rankings[i].n64Value = pRankingsNode[i].n64Value;
		ZeroMemory(send.Rankings[i].szRoleName, X_SHORT_NAME);
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;

	return E_Success;

}

//-------------------------------------------------------------------------------------------------------
// 加载宠物排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadPetRankings()
{
	if (!m_pDBRank->IsNeedLoad(ERT_Pet))
	{
		return E_Success;
	}

	tagPetRankingsNode* pRankingsNode = new tagPetRankingsNode[MAX_RANKINGS_LEN];
	//tagPetRankingsNode* pCur = pRankingsNode;
	INT nNum;

	if (!m_pDB->LoadPetRankings(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return GT_INVALID;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitPetRankings send;
	send.nSum = nNum;
	send.eType = ERT_Pet;
	for (INT i=0; i<nNum && i<MAX_RANKINGS_LEN; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].nPreNum = pRankingsNode[i].nPreNum;
		//ZeroMemory(send.Rankings[i].szRoleName, X_SHORT_NAME);
		pRankingsNode[i].szPetName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.Rankings[i].szPetName, X_SHORT_NAME, pRankingsNode[i].szPetName );
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
		send.Rankings[i].n64Value = pRankingsNode[i].n64Value;
		send.Rankings[i].dwPetID = pRankingsNode[i].dwPetID;
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;

	return E_Success;

}

//-------------------------------------------------------------------------------------------------------
// 加载等级排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadLevelRankings()
{
	if (!m_pDBRank->IsNeedLoad(ERT_Level))
	{
		return E_Success;
	}

	tagRankingsNode* pRankingsNode = new tagRankingsNode[MAX_RANKINGS_LEN];
	//tagRankingsNode* pCur = pRankingsNode;
	INT nNum;

	if (!m_pDB->LoadLevelRankings(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return GT_INVALID;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitRankings send;
	send.nSum = nNum;
	send.eType = ERT_Level;
	for (INT i=0; i<nNum && i<MAX_RANKINGS_LEN; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].nPreNum = pRankingsNode[i].nPreNum;
		pRankingsNode[i].szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.Rankings[i].szRoleName, X_SHORT_NAME, pRankingsNode[i].szRoleName );
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
		send.Rankings[i].n64Value = pRankingsNode[i].n64Value;
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;

	return E_Success;

}

//-------------------------------------------------------------------------------------------------------
// 加载装备排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadEquipRankings()
{
	if (!m_pDBRank->IsNeedLoad(ERT_Equip))
	{
		return E_Success;
	}

	tagRankingsNode* pRankingsNode = new tagRankingsNode[MAX_RANKINGS_LEN];
	//tagRankingsNode* pCur = pRankingsNode;
	INT nNum;

	if (!m_pDB->LoadEquipRankings(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return GT_INVALID;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitRankings send;
	send.nSum = nNum;
	send.eType = ERT_Equip;
	for (INT i=0; i<nNum && i<MAX_RANKINGS_LEN; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].nPreNum = pRankingsNode[i].nPreNum;
		send.Rankings[i].n64Value = pRankingsNode[i].n64Value;
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
		ZeroMemory(send.Rankings[i].szRoleName, X_SHORT_NAME);
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;

	return E_Success;

}



//-------------------------------------------------------------------------------------------------------
// 加载武勋排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadWuXunRankings()
{
	if (!m_pDBRank->IsNeedLoad(ERT_WuXun))
	{
		return E_Success;
	}

	tagRankingsNode* pRankingsNode = new tagRankingsNode[MAX_RANKINGS_LEN];
	//tagRankingsNode* pCur = pRankingsNode;
	INT nNum;

	if (!m_pDB->LoadWuXunRankings(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return GT_INVALID;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitRankings send;
	send.nSum = nNum;
	send.eType = ERT_WuXun;
	for (INT i=0; i<nNum && i<MAX_RANKINGS_LEN; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].nPreNum = pRankingsNode[i].nPreNum;
		pRankingsNode[i].szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.Rankings[i].szRoleName, X_SHORT_NAME, pRankingsNode[i].szRoleName );
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
		send.Rankings[i].n64Value = pRankingsNode[i].n64Value;
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;

	return E_Success;

}

//-------------------------------------------------------------------------------------------------------
// 加载鲜花排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadFlowerRankings()
{
	if (!m_pDBRank->IsNeedLoad(ERT_Flower))
	{
		return E_Success;
	}

	tagRankingsNode* pRankingsNode = new tagRankingsNode[MAX_FlowerEggRankings_LEN];
	//tagRankingsNode* pCur = pRankingsNode;
	INT nNum;

	if (!m_pDB->LoadFlowerRankings(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return GT_INVALID;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitFlowerRankings send;
	send.nSum = nNum;
	send.eType = ERT_Flower;
	for (INT i=0; i<nNum && i<MAX_RANKINGS_LEN; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].nPreNum = pRankingsNode[i].nPreNum;
		pRankingsNode[i].szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.Rankings[i].szRoleName, X_SHORT_NAME, pRankingsNode[i].szRoleName );
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
		send.Rankings[i].n64Value = pRankingsNode[i].n64Value;
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;

	return E_Success;

}
//-------------------------------------------------------------------------------------------------------
// 加载鸡蛋排行榜
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadEggRankings()
{
	if (!m_pDBRank->IsNeedLoad(ERT_Egg))
	{
		return E_Success;
	}

	tagRankingsNode* pRankingsNode = new tagRankingsNode[MAX_FlowerEggRankings_LEN];
	//tagRankingsNode* pCur = pRankingsNode;
	INT nNum;

	if (!m_pDB->LoadEggRankings(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return GT_INVALID;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitFlowerRankings send;
	send.nSum = nNum;
	send.eType = ERT_Egg;
	for (INT i=0; i<nNum && i<MAX_RANKINGS_LEN; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].nPreNum = pRankingsNode[i].nPreNum;
		pRankingsNode[i].szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.Rankings[i].szRoleName, X_SHORT_NAME, pRankingsNode[i].szRoleName );
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
		send.Rankings[i].n64Value = pRankingsNode[i].n64Value;
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;

	return E_Success;

}

//-------------------------------------------------------------------------------------------------------
// 加载神职竞选结果
//-------------------------------------------------------------------------------------------------------
 DWORD CLoong::LoadClergyRank(DWORD pDBMsg, DWORD)
 {
 	LoadClergyMarsRank();
 	LoadClergyApolloRank();
 	LoadClergyRabbiRank();
 	LoadClergyPeaceRank();
 	return E_Success;
 }

VOID CLoong::LoadClergyMarsRank()
{
	tagClergyNode* pRankingsNode = new tagClergyNode[MAX_CampaignInfoNumPerType];
	INT nNum;

	if (!m_pDB->LoadClergyMarsRank(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitClergyRank send;
	send.nSum = nNum;
	send.eType = ECGT_War;
	for (INT i=0; i<nNum && i<MAX_CampaignInfoNumPerType; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
		send.Rankings[i].dwPoint = pRankingsNode[i].dwPoint;
		send.Rankings[i].eClergy = pRankingsNode[i].eClergy;
		send.Rankings[i].bRecv = pRankingsNode[i].bRecv;
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;
}

VOID CLoong::LoadClergyApolloRank()
{
	tagClergyNode* pRankingsNode = new tagClergyNode[MAX_CampaignInfoNumPerType];
	INT nNum;

	if (!m_pDB->LoadClergyApolloRank(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitClergyRank send;
	send.nSum = nNum;
	send.eType = ECGT_Sun;
	for (INT i=0; i<nNum && i<MAX_CampaignInfoNumPerType; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
		send.Rankings[i].dwPoint = pRankingsNode[i].dwPoint;
		send.Rankings[i].eClergy = pRankingsNode[i].eClergy;
		send.Rankings[i].bRecv = pRankingsNode[i].bRecv;
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;
}

VOID CLoong::LoadClergyRabbiRank()
{
	tagClergyNode* pRankingsNode = new tagClergyNode[MAX_CampaignInfoNumPerType];
	INT nNum;

	if (!m_pDB->LoadClergyRabbiRank(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitClergyRank send;
	send.nSum = nNum;
	send.eType = ECGT_Mana;
	for (INT i=0; i<nNum && i<MAX_CampaignInfoNumPerType; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
		send.Rankings[i].dwPoint = pRankingsNode[i].dwPoint;
		send.Rankings[i].eClergy = pRankingsNode[i].eClergy;
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;
}

VOID CLoong::LoadClergyPeaceRank()
{
	tagClergyNode* pRankingsNode = new tagClergyNode[MAX_CampaignInfoNumPerType];
	INT nNum;

	if (!m_pDB->LoadClergyPeaceRank(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return;
	}

	// 组织成消息，发给游戏世界
	tagNDBS_InitClergyRank send;
	send.nSum = nNum;
	send.eType = ECGT_Peace;
	for (INT i=0; i<nNum && i<MAX_CampaignInfoNumPerType; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].dwRoleID = pRankingsNode[i].dwRoleID;
		send.Rankings[i].dwPoint = pRankingsNode[i].dwPoint;
		send.Rankings[i].eClergy = pRankingsNode[i].eClergy;
	}

	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;
}

DWORD CLoong::ChangeCandidateRecvFlag(DWORD pDBMsg, DWORD)
{
	tagNDBC_CandidateRecvClergy* pRecv = (tagNDBC_CandidateRecvClergy*)pDBMsg;
	int i = 0;
	while(i<pRecv->nNum)
	{
		m_pDB->ChangeClergyCandidateRecvFlag(pRecv->eType, pRecv->dwRoleID[i]);
		i++;
	}

	return E_Success;
}

DWORD CLoong::GetClergyCandidateCount(DWORD pDBMsg, DWORD o)
{
	DWORD dwCount[ECGT_Num] = {0};
	if(E_Success != m_pDB->GetClergyCandidateCount(dwCount))
	{
		return GT_INVALID;
	}

	tagNDBS_Ask4CandidateCount send;
	memcpy(send.dwCount, dwCount, sizeof(DWORD)*ECGT_Num);

	SendMsg(&send, send.dwSize);

	return E_Success;
}

DWORD CLoong::CancelPreClergy(DWORD pDBMsg, DWORD)
{
	tagNDBC_CancelPreClergy* pRecv = (tagNDBC_CancelPreClergy*) pDBMsg;
	if(E_Success != m_pDB->CancelPreClergy(pRecv->nNum, pRecv->dwRoleID))
	{
		return GT_INVALID;
	}

	return E_Success;
}
