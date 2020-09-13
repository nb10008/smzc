#include "StdAfx.h"
#include "wx/wx.h"

#include "Loong_LoginDBCtrl.h"
#include "Loong_MergeLogicCtrl.h"
#include "servermergetoolapp.h"
#include "Loong_OutputCtrl.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/base_define.h"

//降序
BOOL Compare_Loong_GuildLess(tagTWrap_Loong_Guild* pFir,tagTWrap_Loong_Guild* pSec)
{
	if( pFir->stOthers.nReputation<pSec->stOthers.nReputation)
		return false;
	else if(pFir->stOthers.nReputation>pSec->stOthers.nReputation)
		return true;

	return false;
}
//排序
BOOL Compare_Loong_RoleDataAcs(tagTWrap_Loong_RoleData* pFir,tagTWrap_Loong_RoleData* pSec)
{
	//降序
	if(pFir->stOthers.RoleDataSave.nLevel>pSec->stOthers.RoleDataSave.nLevel)
		return true;
	else if(pFir->stOthers.RoleDataSave.nLevel<pSec->stOthers.RoleDataSave.nLevel)
		return false;

	//降序
	if(pFir->stOthers.RoleDataSave.nCurExp>pSec->stOthers.RoleDataSave.nCurExp)
		return true;
	else if(pFir->stOthers.RoleDataSave.nCurExp<pSec->stOthers.RoleDataSave.nCurExp)
		return false;

	//升序
	if(pFir->stOthers.RoleDataConst.CreateTime<pSec->stOthers.RoleDataConst.CreateTime)
		return true;
	else if(pFir->stOthers.RoleDataConst.CreateTime>pSec->stOthers.RoleDataConst.CreateTime)
		return false;

	return false;
}
//排序
BOOL Compare_Loong_ItemAcs(tagTWrap_Loong_Item* pFir,tagTWrap_Loong_Item* pSec)
{
	//升序
	if( pFir->stOthers.dwAccountID<pSec->stOthers.dwAccountID)
		return true;
	else if(pFir->stOthers.dwAccountID>pSec->stOthers.dwAccountID)
		return false;

	//升序 为了确定保存源1的道具
	if( pFir->stOthers.dwOwnerID<pSec->stOthers.dwOwnerID)
		return true;
	else if(pFir->stOthers.dwOwnerID>pSec->stOthers.dwOwnerID)
		return false;

	//按容器类型分一下 升序
	if(pFir->stOthers.eConType<pSec->stOthers.eConType)
		return true;
	else if(pFir->stOthers.eConType>pSec->stOthers.eConType)
		return false;

	//按位置信息排一下
	if( pFir->stOthers.n16Index<pSec->stOthers.n16Index)
		return true;
	else if(pFir->stOthers.n16Index>pSec->stOthers.n16Index)
		return false;


	return false;
}

CLoongMergeLogicCtrl::CLoongMergeLogicCtrl()
:m_eMergeType(EM_MERGE_NULL),m_strSrc2WorldName(_T("")),m_n64Src1ItemMaxSerialInternal(MIN_ITEM_SERIAL_INTERNAL+1)
,m_n64Src1ItemMinSerialOutter(MIN_ITEM_SERIAL_OUTTER),m_n64Src1ItemMinSerialBill(MAX_ITEM_SERIAL_BILL)
,m_dwSrc1RoleMaxNum(0),m_dwSrc1LoginMaxNum(0),m_dwSrc1WorldNameCrc(0),m_dwSrc1PetMaxNum(0),m_bDataClean(FALSE)
,m_nItemDelEquipDelSurviveDay(30)
{

	//考虑一些划拨物品和百宝袋物品
	m_vecOldSrc1AccountID.push_back(GT_INVALID);
	m_mpOldSrc1VsSrc2.insert(make_pair(GT_INVALID, GT_INVALID));
	m_mpOldSrc2VsSrc1.insert(make_pair(GT_INVALID, GT_INVALID));

	m_n64ItemCurSerialInternal = MIN_ITEM_SERIAL_INTERNAL;
	m_n64ItemCurSerialOutter = MAX_ITEM_SERIAL_OUTTER;
	m_n64ItemCurSerialBill = MAX_ITEM_SERIAL_BILL;

	InitializeCriticalSection(&m_ContainerLock);
}

CLoongMergeLogicCtrl::~CLoongMergeLogicCtrl(void)
{
	std::vector<tagFamilyNeedChangeName*>::iterator it;
	for( it = m_vecNeedChangeFamilyName.begin(); it != m_vecNeedChangeFamilyName.end(); ++it )
	{
		SAFE_DEL((*it));
	}
	m_vecNeedChangeFamilyName.clear();

	DeleteCriticalSection(&m_ContainerLock);
}
//干掉所有中间数据
VOID CLoongMergeLogicCtrl::ClearMemory()
{
	m_n64Src1ItemMaxSerialInternal = MIN_ITEM_SERIAL_INTERNAL+1;
	m_n64Src1ItemMinSerialOutter = MIN_ITEM_SERIAL_OUTTER;
	m_n64Src1ItemMinSerialBill = MAX_ITEM_SERIAL_BILL;

	m_n64ItemCurSerialInternal = MIN_ITEM_SERIAL_INTERNAL;
	m_n64ItemCurSerialOutter = MAX_ITEM_SERIAL_OUTTER;
	m_n64ItemCurSerialBill = MAX_ITEM_SERIAL_BILL;

	m_dwSrc1LoginMaxNum = 0;
	m_dwSrc1RoleMaxNum = 0;
	m_dwSrc1PetMaxNum = 0;
//	m_dwSrc1VipStallMaxNum = 0;

	m_dwSrc1WorldNameCrc = 0;
	//m_mpOldItemNumvsNewItemNum.clear();
	m_mpOldGuildIDvsNewGuildID.clear();
	m_mpOldFamilyIDvsNewFamilyID.clear();
	m_mpAccountIDvsWareSize.clear();
	m_mpRoleIDvsRemoveType.clear();
	m_vecOverItem.clear();
	m_setOverEquipSerialNum.clear();
	m_vecOverEquip.clear();
	m_setOldGuildID.clear();
	m_mpAccountIDvsRoleIDforCB.clear();
	m_vecOldSrc1AccountID.clear();
	m_mpOldSrc1VsSrc2.clear();
	m_mpOldSrc2VsSrc1.clear();
	m_vecOldSrc2OnlyAccountID.clear();
	m_vecRoleCompensation.clear();
	m_setCAccountID.clear(); //曾经充值过的账号id
	m_mapGuildLeader.clear(); //所有帮主 新的roleid
	m_setDirtyItem.clear();	//脏的物品

	m_setSrc1RoleName.clear();
	m_setSrc1RoleNameCrc.clear();

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//更新roleid 供源2使用
VOID CLoongMergeLogicCtrl::Update_Loong_RoleID(DWORD &dwRoleID)
{
	if(IS_PLAYER(dwRoleID))
		dwRoleID += m_dwSrc1RoleMaxNum;
	else if(IS_PET(dwRoleID))
		dwRoleID += (m_dwSrc1PetMaxNum - MIN_PET_ID);

}
//转换成小写后的crc32
DWORD CLoongMergeLogicCtrl::LowerCrc32(LPCTSTR str,DWORD strLen)
{
	TCHAR *szTemp = new TCHAR[strLen];

	_tcsncpy(szTemp,str,strLen);

	_tcslwr(szTemp);

	DWORD dwCrc = IUTIL->Crc32(szTemp);

	SAFE_DEL_ARRAY(szTemp);

	return dwCrc;
}

//更新item 64位编号
VOID CLoongMergeLogicCtrl::Update_Loong_Item64SerialNum(INT64 &n64Serial)
{
	if(n64Serial != GT_INVALID && n64Serial != 0)
	{
		if(n64Serial>MIN_ITEM_SERIAL_INTERNAL)
		{	
			n64Serial += (m_n64Src1ItemMaxSerialInternal - MIN_ITEM_SERIAL_INTERNAL);

			if(n64Serial > m_n64ItemCurSerialInternal)
			{
				m_n64ItemCurSerialInternal = n64Serial;
			}
		}
		else if(n64Serial>=MIN_ITEM_SERIAL_OUTTER && n64Serial<=MAX_ITEM_SERIAL_OUTTER)
		{
			n64Serial -= (MAX_ITEM_SERIAL_OUTTER - m_n64Src1ItemMinSerialOutter + 1);

			if(n64Serial < m_n64ItemCurSerialOutter)
			{
				m_n64ItemCurSerialOutter = n64Serial;
			}
		}
		else if(n64Serial<=MAX_ITEM_SERIAL_BILL)
		{
			n64Serial -= (MAX_ITEM_SERIAL_BILL - m_n64Src1ItemMinSerialBill);

			if(n64Serial < m_n64ItemCurSerialBill)
			{
				m_n64ItemCurSerialBill = n64Serial;
			}
		}
		else
		{
			ASSERT(0);
		}
	}

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//login account表
BOOL CLoongMergeLogicCtrl::Loong_LoginMerge(std::vector <tagTWrap_Longin_Account*> &vecSrc1,
							  std::vector <tagTWrap_Longin_Account*> &vecSrc2,
							  std::vector <tagTWrap_Longin_Account*> &vecTar)
{
	//跨大区 src1的accountid不变 src2的从新排 重名的不追加
	std::vector <tagTWrap_Longin_Account*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Longin_Account*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;
	std::map<DWORD ,DWORD>::iterator itAccountFind;

	std::map<string ,DWORD>::iterator itANameVsAID;
	std::map<string ,DWORD> mapANameVsAID; //处理重名

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	//源1
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		mapANameVsAID.insert( make_pair( (*itSrc1)->szName ,(*itSrc1)->stOthers.dwAccountID ) );
		vecTar.push_back(*itSrc1);
	}

	//找一个源1的世界名crc 
	//用
	for(int iLoop = 0;iLoop<vecTar.size();iLoop++)
	{
		//利用源1的背包表 看该account是否属于源1世界
		itAccountFind = m_mpAccountIDvsWareSize.find(vecTar[iLoop]->stOthers.dwAccountID);
		if(itAccountFind != m_mpAccountIDvsWareSize.end())
		{
			//如果找到了 把crc赋上
			m_dwSrc1WorldNameCrc = vecTar[iLoop]->stOthers.dwWorldNameCrc;
			break;
		}
	}

	//src1 login account表 id最大数
	DWORD dwSrc1LoginMaxNum = m_dwSrc1LoginMaxNum + 1;

	//源2
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		(*itSrc2)->stOthers.dwWorldNameCrc = m_dwSrc1WorldNameCrc;

		//如果能找到重名的 用源1的id
		itANameVsAID = mapANameVsAID.find((*itSrc2)->szName);
		if(itANameVsAID != mapANameVsAID.end())
		{
			m_mpOldAccountVsNewAccount[tagAccountIDKey((*itSrc2)->stOthers.dwAccountID,EM_DB_SRC_TWO)] \
				= itANameVsAID->second;

			continue;
		}
		
		//旧新id对照表
		m_mpOldAccountVsNewAccount[tagAccountIDKey((*itSrc2)->stOthers.dwAccountID,EM_DB_SRC_TWO)] \
			= dwSrc1LoginMaxNum;

		//追加进去的数据给一个新的id
		(*itSrc2)->stOthers.dwAccountID = dwSrc1LoginMaxNum;

		++dwSrc1LoginMaxNum;

		vecTar.push_back(*itSrc2);
	}

	return TRUE;
}
//login worldforbid表
BOOL CLoongMergeLogicCtrl::Loong_LoginMerge(std::vector <tagTWrap_Longin_WorldForbid*> &vecSrc1,
											std::vector <tagTWrap_Longin_WorldForbid*> &vecSrc2,
											std::vector <tagTWrap_Longin_WorldForbid*> &vecTar)
{
	std::vector <tagTWrap_Longin_WorldForbid*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Longin_WorldForbid*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;
	//源1的不变 源2的根据源2世界名 算出crc 
	DWORD dwSrc2WorldCrc = IUTIL->Crc32(m_strSrc2WorldName.c_str());

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	if(m_eMergeType != EM_MERGE_CROSS_SECTION)
	{
		return TRUE;
	}

	//如果跨大区
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
		//如果找到
		if(itSrcFind != m_mpOldAccountVsNewAccount.end())
		{
			//accountid
			(*itSrc2)->dwAccountID = (*itSrcFind).second;				
		}
		
		//如果找不到
		if(itSrcFind == m_mpOldAccountVsNewAccount.end())
		{
			continue;
		}	
		//如果能找到 看其id是否在src1范围内 
		if( (itSrcFind->second) <=  m_dwSrc1LoginMaxNum )
		{
			continue;
		}		
		//在确定其为源2游戏世界时 将crc改成源1的世界名crc
		if((*itSrc2)->dwWorldNameCrc == dwSrc2WorldCrc)
		{
			(*itSrc2)->dwWorldNameCrc = m_dwSrc1WorldNameCrc;
			vecTar.push_back(*itSrc2);	
		}

	}
	return true;
}
//login 黑名单
BOOL CLoongMergeLogicCtrl::Loong_LoginMerge(std::vector <tagTWrap_Longin_BlackList*> &vecSrc1,
											std::vector <tagTWrap_Longin_BlackList*> &vecSrc2,
											std::vector <tagTWrap_Longin_BlackList*> &vecTar)
{
	std::vector <tagTWrap_Longin_BlackList*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Longin_BlackList*>::iterator itSrc2 = vecSrc2.begin();
	std::set<string>::iterator	itIP;
	std::set<string> setIP; //处理重复ip	
	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	//源1
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setIP.insert((*itSrc1)->szIP);
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//在源2里找
		itIP = setIP.find((*itSrc2)->szIP);
		//如果找到
		if(itIP != setIP.end())
		{
			continue;
		}
		vecTar.push_back(*itSrc2);
	}
	return true;
}
//login 反沉迷
BOOL CLoongMergeLogicCtrl::Loong_LoginMerge(std::vector <tagTWrap_Longin_FatigueTime*> &vecSrc1,
											std::vector <tagTWrap_Longin_FatigueTime*> &vecSrc2,
											std::vector <tagTWrap_Longin_FatigueTime*> &vecTar)
{
	std::vector <tagTWrap_Longin_FatigueTime*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Longin_FatigueTime*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	std::set<DWORD> setAccountNameCrc;
	std::set<DWORD>::iterator itFind;
	//源1
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setAccountNameCrc.insert((*itSrc1)->dwAccountNameCrc);
		vecTar.push_back(*itSrc1);
	}

	if(m_eMergeType != EM_MERGE_CROSS_SECTION)
	{
		return TRUE;
	}

	//如果跨大区
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		itFind = setAccountNameCrc.find((*itSrc2)->dwAccountNameCrc);
		if(itFind != setAccountNameCrc.end())
		{
			continue;
		}

		vecTar.push_back(*itSrc2);
	}
	return true;
}


//login 全局角色名
BOOL CLoongMergeLogicCtrl::Loong_LoginMerge(std::vector <tagTWrap_Longin_RoleName*> &vecSrc1,
											std::vector <tagTWrap_Longin_RoleName*> &vecSrc2,
											std::vector <tagTWrap_Longin_RoleName*> &vecTar)
{
	std::vector <tagTWrap_Longin_RoleName*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Longin_RoleName*>::iterator itSrc2 = vecSrc2.begin();

	if(m_eMergeType != EM_MERGE_CROSS_SECTION)
	{
		return TRUE;
	}
	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());


	TCHAR szRoleName[X_SHORT_NAME] = {0};
	std::set<tstring> setSrc1RoleName;
	//源1
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		_tcsncpy(szRoleName, (*itSrc1)->szName, X_SHORT_NAME);
		szRoleName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(szRoleName);

		setSrc1RoleName.insert(szRoleName);

		vecTar.push_back(*itSrc1);
	}

	// 直接插入
	BOOL bRet = sLoginBeton.LoginDB_Accout_Insert(vecTar,EM_DB_TAR);
	if(bRet == FALSE)
	{
		return bRet;
	}
	// 清空所有src1的
	vecTar.clear();

	std::set<tstring>::iterator itFind;
	//如果跨大区
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		_tcsncpy(szRoleName, (*itSrc2)->szName, X_SHORT_NAME);
		szRoleName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(szRoleName);

		itFind = setSrc1RoleName.find(szRoleName);
		if(itFind != setSrc1RoleName.end())
		{
			tstring AddName;
			AddName += _T("_");
			AddName += m_strSrc2WorldName;

			//role名字超长？
			StrCat((*itSrc2)->szName,AddName.c_str());
		}

		vecTar.push_back(*itSrc2);
		
		// 试一次
		sLoginBeton.LoginDB_Accout_Insert(vecTar,EM_DB_TAR);

		// 清空这一条记录
		vecTar.clear();
	}

	// 清空所有tar的记录
	vecTar.clear();

	return true;
}


//login 全局角色名
BOOL CLoongMergeLogicCtrl::Loong_LoginMerge(std::vector <tagTWrap_Longin_GuildName*> &vecSrc1,
											std::vector <tagTWrap_Longin_GuildName*> &vecSrc2,
											std::vector <tagTWrap_Longin_GuildName*> &vecTar)
{
	std::vector <tagTWrap_Longin_GuildName*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Longin_GuildName*>::iterator itSrc2 = vecSrc2.begin();

	if(m_eMergeType != EM_MERGE_CROSS_SECTION)
	{
		return TRUE;
	}
	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	TCHAR tszGuildName[X_SHORT_NAME] = {0};
	std::set<tstring> setSrc1GuildName;
	//源1
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		_tcsncpy(tszGuildName, (*itSrc1)->szName, X_SHORT_NAME);
		tszGuildName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(tszGuildName);

		setSrc1GuildName.insert(tszGuildName);

		vecTar.push_back(*itSrc1);
	}

	std::set<tstring>::iterator itFind;
	//如果跨大区
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		_tcsncpy(tszGuildName, (*itSrc2)->szName, X_SHORT_NAME);
		tszGuildName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(tszGuildName);

		itFind = setSrc1GuildName.find(tszGuildName);
		if(itFind != setSrc1GuildName.end())
		{
			tstring AddName;
			AddName += _T("_");
			AddName += m_strSrc2WorldName;
			StrCat((*itSrc2)->szName, AddName.c_str());
		}

		vecTar.push_back(*itSrc2);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//对账号名进行纠错
string Loong_CheckAccountName(CHAR szAccountName[X_SHORT_NAME])
{
	ASSERT(szAccountName);

	string strTemp = szAccountName;

	//返回结果
	string strReturn = "";

	for(string::size_type iLoop = 0; iLoop != strTemp.size(); ++iLoop)
	{
		//数字
		if(isdigit(strTemp[iLoop]) != 0)
		{
			strReturn += strTemp[iLoop];
			continue;
		}
		//字母
		if(isalpha(strTemp[iLoop]) != 0)
		{
			strReturn += tolower(strTemp[iLoop]);
			continue;
		}
		if(strTemp[iLoop] == '-')
		{
			strReturn += strTemp[iLoop];
			continue;
		}
		if(strTemp[iLoop] == '+')
		{
			strReturn += strTemp[iLoop];
			continue;
		}
		if(strTemp[iLoop] == '_')
		{
			strReturn += strTemp[iLoop];
			continue;
		}
	}


	return strReturn;
}

//loong 需要合并仓库内金钱
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_AccountCommon*> &vecSrc1,
									std::vector <tagTWrap_Loong_AccountCommon*> &vecSrc2,
									std::vector <tagTWrap_Loong_AccountCommon*> &vecTar)
{

	std::vector <tagTWrap_Loong_AccountCommon*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_AccountCommon*>::iterator itSrc2 = vecSrc2.begin();

	std::map<string,tagTWrap_Loong_AccountCommon*>::iterator itSrcFind;
	std::map<string,tagTWrap_Loong_AccountCommon*> mpAccountNameVsAccountCom;//用于处理相同的account

	//存储纠错后的账号名
	string strAccountName = "";

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		m_mpAccountIDvsWareSize[(*itSrc1)->dwAccountID] = (*itSrc1)->stOthers.n16WareSize;
		strAccountName = Loong_CheckAccountName((*itSrc1)->szAccountName);
		mpAccountNameVsAccountCom[ strAccountName ] = (*itSrc1);

		//检查是否充值过
		if(m_setCAName.end() != m_setCAName.find(strAccountName))
			m_setCAccountID.insert((*itSrc1)->dwAccountID);

		m_vecOldSrc1AccountID.push_back((*itSrc1)->dwAccountID);
		//推进去
		vecTar.push_back(*itSrc1);	
	}

	//往m_mpOldAccountVsNewAccount里存
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		strAccountName = Loong_CheckAccountName((*itSrc2)->szAccountName);
		//用源2的name在 源1里找
		itSrcFind = mpAccountNameVsAccountCom.find(strAccountName);
		//如果找到 重名的 把仓库合并
		if(itSrcFind != mpAccountNameVsAccountCom.end())
		{
			//加一下源1内仓库的 金 银 元宝 size？bag psd 以及重置时间 
			(itSrcFind->second)->stOthers.nBaiBaoYuanBao += (*itSrc2)->stOthers.nBaiBaoYuanBao;
			(itSrcFind->second)->stOthers.n64WareSilver += (*itSrc2)->stOthers.n64WareSilver;
	
			//size取大的
			if((itSrcFind->second)->stOthers.n16WareSize < (*itSrc2)->stOthers.n16WareSize)
			{
				//修改目标数据
				(itSrcFind->second)->stOthers.n16WareSize = (*itSrc2)->stOthers.n16WareSize;
				//修改对照表
				m_mpAccountIDvsWareSize[(itSrcFind->second)->dwAccountID] = (*itSrc2)->stOthers.n16WareSize;
			}

			m_mpOldSrc1VsSrc2[(itSrcFind->second)->dwAccountID] = (*itSrc2)->dwAccountID;
			m_mpOldSrc2VsSrc1[(*itSrc2)->dwAccountID] = (itSrcFind->second)->dwAccountID;

			continue;
		}

		//没找到重名
		//如果跨大区
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			m_vecOldSrc2OnlyAccountID.push_back((*itSrc2)->dwAccountID);

			(*itSrc2)->dwAccountID = m_mpOldAccountVsNewAccount[tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO)];

			//检查是否充值过
			if(m_setCAName.end() != m_setCAName.find(strAccountName))
				m_setCAccountID.insert((*itSrc2)->dwAccountID);

			//accountid与背包的对照表
			m_mpAccountIDvsWareSize[(*itSrc2)->dwAccountID] = (*itSrc2)->stOthers.n16WareSize;

		}
		//同一个大区下的添加
		else
		{
			m_vecOldSrc2OnlyAccountID.push_back((*itSrc2)->dwAccountID);

			//检查是否充值过
			if(m_setCAName.end() != m_setCAName.find(strAccountName))
				m_setCAccountID.insert((*itSrc2)->dwAccountID);

			//容错
			std::map<DWORD ,DWORD>::iterator itCheck;
			itCheck = m_mpAccountIDvsWareSize.find((*itSrc2)->dwAccountID);
			if(itCheck != m_mpAccountIDvsWareSize.end())
			{
				continue;
			}
			//accountid与背包的对照表
			m_mpAccountIDvsWareSize[(*itSrc2)->dwAccountID] = (*itSrc2)->stOthers.n16WareSize;
		}

		vecTar.push_back(*itSrc2);	
	}
	
	return TRUE;
	
}

//检查该role是否为脏的
BOOL CLoongMergeLogicCtrl::IsDirtyRoleData(DWORD dwRoleID)
{
	//数据清理开关是否打开
	if(!m_bDataClean)
		return FALSE;

	stdext::hash_map<DWORD ,ETBRoleRemoveType>::iterator itFinder = m_mpRoleIDvsRemoveType.find(dwRoleID);
	if(itFinder != m_mpRoleIDvsRemoveType.end())
	{
		if(itFinder->second == EM_R_ROLEDATA_CLEAN)
			return TRUE;
	}

	return FALSE;
}

//清楚roledata表中的脏数据
ETBRoleRemoveType CLoongMergeLogicCtrl::Loong_RoleDataDoHouseKeeping(std::vector <tagTWrap_Loong_RoleData*>::iterator itTar,
																	tagDWORDTime dwCurTm,
																	BOOL bFirstRole)
{	
	//自删除账号
	if((*itTar)->bRemoveFlag == TRUE)
	{
		return EM_R_ONESELF;
	}



	//看是否要做数据清理
	if(!m_bDataClean)
		return EM_R_NULL;

	//是否为安全账号
	static BOOL bSafeAccount = FALSE;

	//如果是该账号的第一个角色 重置安全状态
	if(bFirstRole)
	{
		bSafeAccount = FALSE;
	}

	//是否为安全账号
	if(bSafeAccount)
		return EM_R_NULL;

	//有超过50级的角色 (因为之前按角色降序排序了)
	if((*itTar)->n64WorkedLevel > 50)
	{	
		bSafeAccount = TRUE;
		return EM_R_NULL;
	}

	//有任意角色30天内登陆过账号，则不删除任何角色	(因为之前按时间降序排序了)
	if(CalcTimeDiff(dwCurTm,(*itTar)->stOthers.RoleDataSave.LoginTime) <= 60*60*24*30)
	{
		bSafeAccount = TRUE;
		return EM_R_NULL;
	}

//////////////////////////////////////////////////////////////////////////

	//是帮主
	if(m_mapGuildLeader.end() != m_mapGuildLeader.find((*itTar)->dwRoleID))
	{
		return EM_R_NULL;
	}

	//如果小于等于4级 且 30天没登陆 干掉
	if((*itTar)->n64WorkedLevel <= 4)
	{
		if(CalcTimeDiff(dwCurTm,(*itTar)->stOthers.RoleDataSave.LoginTime) >= 60*60*24*30)
		{
			(*itTar)->bRemoveFlag = TRUE;
			return EM_R_ROLEDATA_CLEAN;
		}
		else
		{
			return EM_R_NULL;
		}
	}

	//如果小于25级 且 45天没登陆 干掉
	if((*itTar)->n64WorkedLevel <= 25)
	{
		if(CalcTimeDiff(dwCurTm,(*itTar)->stOthers.RoleDataSave.LoginTime) >= 60*60*24*45)
		{
			(*itTar)->bRemoveFlag = TRUE;
			return EM_R_ROLEDATA_CLEAN;
		}
		else
		{
			return EM_R_NULL;
		}
	}

	//如果小于40级 且 60天没登陆
	if((*itTar)->n64WorkedLevel <= 40)
	{
		if(CalcTimeDiff(dwCurTm,(*itTar)->stOthers.RoleDataSave.LoginTime) >= 60*60*24*60)
		{
			(*itTar)->bRemoveFlag = TRUE;
			return EM_R_ROLEDATA_CLEAN;
		}
		else
		{
			return EM_R_NULL;
		}
	}

	return EM_R_NULL;
}


//role 数据过滤
VOID CLoongMergeLogicCtrl::Loong_RoleDataFilter(std::vector<tagTWrap_Loong_RoleData*> & vecTar)
{
	if(vecTar.size() == 0)
		return;

	//按account id升序  level降序
	std::sort(vecTar.begin(),vecTar.end(),Compare_Loong_RoleDataAcs);

	//处理一下超过5个role的情况
	std::map<DWORD ,DWORD>::iterator itAccountFinder;
	std::vector <tagTWrap_Loong_RoleData*>::iterator itTar = vecTar.begin();
	WORD wRoleNum = 0;
	DWORD dwAccountIDPre = GT_INVALID;
	ETBRoleRemoveType eRemoveType = EM_R_NULL;
	tagDWORDTime dwCurTm = GetCurrentDWORDTime();

	for(;itTar != vecTar.end(); ++itTar)
	{	
		if((*itTar)->dwAccountID == dwAccountIDPre)
		{
			//清理roldata表数据
			eRemoveType = Loong_RoleDataDoHouseKeeping(itTar, dwCurTm, FALSE);

			if(wRoleNum >= ROLE_MAX_LIMIT && eRemoveType == EM_R_NULL)
			{
				//表示该role为合并时角色超出删除
				eRemoveType = EM_R_MERGE;

				(*itTar)->bRemoveFlag = TRUE;
			}
			//累计账号人数
			if((*itTar)->bRemoveFlag == FALSE)
				++wRoleNum;

			//保存一个账号中“尚未被删除的”“等级最高的“roleid用于改绑定
			if((*itTar)->bRemoveFlag == FALSE)
			{
				itAccountFinder = m_mpAccountIDvsRoleIDforCB.find((*itTar)->dwAccountID);
				if(itAccountFinder == m_mpAccountIDvsRoleIDforCB.end())
				{
					m_mpAccountIDvsRoleIDforCB.insert(make_pair((*itTar)->dwAccountID,(*itTar)->dwRoleID));

				}
			}
		}
		else
		{	
			//清理roldata表数据
			eRemoveType = Loong_RoleDataDoHouseKeeping(itTar, dwCurTm, TRUE);

			//当切换account时，如果第一个角色没有被删除 rolenum计数为1 并保存该role到m_mpAccountIDvsRoleIDforCB表
			if((*itTar)->bRemoveFlag == FALSE)
			{
				wRoleNum = 1;
				m_mpAccountIDvsRoleIDforCB.insert(make_pair((*itTar)->dwAccountID,(*itTar)->dwRoleID));
			}
			else
			{
				wRoleNum = 0;
			}

			dwAccountIDPre = (*itTar)->dwAccountID;
		}

		//处于删除状态
		if(EM_R_NULL != eRemoveType)
		{
			//新的roleid和其removetype的对照表
			m_mpRoleIDvsRemoveType[(*itTar)->dwRoleID] = eRemoveType;
		}

	}
}

//role data
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_RoleData*> &vecSrc1,
											std::vector <tagTWrap_Loong_RoleData*> &vecSrc2,
											std::vector <tagTWrap_Loong_RoleData*> &vecTar)
{
	if(vecSrc1.size() == 0 && vecSrc2.size() == 0)
		return TRUE;

	//重名的role name加  _世界名
	//一个Account用户多余5个role时，取等级最高的5个role~其他的role置removeflag为1
	//guildid更新
	//其他不变
	std::vector <tagTWrap_Loong_RoleData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_RoleData*>::iterator itSrc2 = vecSrc2.begin();

	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//由于目前数据库存在错误，两个不同的RoleName可能会对应同一个RoleNameCrc
	//为解决此问题，在处理重名时，同时要比对名字和crc
	std::set<tstring>::iterator itNameFind;


	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		// 竞选神职角色 恢复竞选前神职
		if(m_setSrc1ClergyRoleID.find((*itSrc1)->dwRoleID) != m_setSrc1ClergyRoleID.end())
		{
			(*itSrc1)->stOthers.RoleDataSave.dwClergy = (*itSrc1)->stOthers.RoleDataSave.dwPreClergy;
			(*itSrc1)->stOthers.RoleDataSave.dwPreClergy = 0;
		}

		vecTar.push_back(*itSrc1);
	}

	//Account name change compensation already 太挫了
	BOOL bANCCAlready =FALSE;
	TCHAR szRoleName[X_SHORT_NAME] = {0};
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		// 竞选神职角色 恢复竞选前神职
		if(m_setSrc2ClergyRoleID.find((*itSrc2)->dwRoleID) != m_setSrc2ClergyRoleID.end())
		{
			(*itSrc2)->stOthers.RoleDataSave.dwClergy = (*itSrc2)->stOthers.RoleDataSave.dwPreClergy;
			(*itSrc2)->stOthers.RoleDataSave.dwPreClergy = 0;
		}

		//解决名字重复问题
		_tcsncpy(szRoleName, (*itSrc2)->stOthers.RoleDataConst.szRoleName, X_SHORT_NAME);
		szRoleName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(szRoleName);

		//是否改名
		BOOL bChangeName = FALSE;

		itNameFind = m_setSrc1RoleName.find(szRoleName);
		//如果找到重名的 在名字后面加世界名 现在不考虑重复合服后玩家名字带多个AddName的情况
		if(itNameFind != m_setSrc1RoleName.end())
		{
			tstring AddName;
			AddName += _T("_");
			AddName += m_strSrc2WorldName;

			//role名字超长？
			StrCat((*itSrc2)->stOthers.RoleDataConst.szRoleName,AddName.c_str());

			//重新算crc
			(*itSrc2)->stOthers.RoleDataConst.dwRoleNameCrc = \
				LowerCrc32((*itSrc2)->stOthers.RoleDataConst.szRoleName,X_SHORT_NAME);
		
			bChangeName = TRUE;
		}
		
		//解决crc重复问题
		while(m_setSrc1RoleNameCrc.end() !=  m_setSrc1RoleNameCrc.find((*itSrc2)->stOthers.RoleDataConst.dwRoleNameCrc))
		{
			//名字后面加个下划线重新计算
			tstring AddName;
			AddName += _T("_");
			StrCat((*itSrc2)->stOthers.RoleDataConst.szRoleName,AddName.c_str());

			//重新算crc
			DWORD dwSrc2 = LowerCrc32((*itSrc2)->stOthers.RoleDataConst.szRoleName,X_SHORT_NAME);
			(*itSrc2)->stOthers.RoleDataConst.dwRoleNameCrc = dwSrc2;
			
			bChangeName = TRUE;
		}
		m_setSrc1RoleNameCrc.insert((*itSrc2)->stOthers.RoleDataConst.dwRoleNameCrc);

		//直接加上源1的玩家数 不删除role
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		//爱人的id
		if((*itSrc2)->stOthers.RoleDataSave.dwLoverID != GT_INVALID)
			Update_Loong_RoleID((*itSrc2)->stOthers.RoleDataSave.dwLoverID);

		//师傅的id
		if((*itSrc2)->stOthers.RoleDataSave.dwMasterID != GT_INVALID)
			Update_Loong_RoleID((*itSrc2)->stOthers.RoleDataSave.dwMasterID);

		//更新帮派id
		if((*itSrc2)->stOthers.RoleDataSave.dwGuildID != GT_INVALID)
		{
			(*itSrc2)->stOthers.RoleDataSave.dwGuildID = \
				m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.RoleDataSave.dwGuildID,EM_DB_SRC_TWO)];
		}
		//如果是跨大区的
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}

			
		}

		// 更新角色的家族ID
		Update_Loong_FamilyNameCRC((*itSrc2)->stOthers.RoleDataSave.dwFamilyID);

		vecTar.push_back(*itSrc2);

		//记下该玩家
		if(bChangeName == TRUE && bANCCAlready == FALSE)
		{
			bANCCAlready = TRUE;
			m_vecRoleCompensation.push_back(tagRoleCompensation(ERC_CHANGE_ROLE_NAME,(*itSrc2)->dwAccountID,(*itSrc2)->dwRoleID));
			m_setChangeNameRoleID.insert((*itSrc2)->dwRoleID);
		}
	}
	
	//过滤roledata数据
	Loong_RoleDataFilter(vecTar);

	return TRUE;
}
//itembaibao
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Item*> &vecSrc1,
											std::vector <tagTWrap_Loong_Item*> &vecSrc2,
											std::vector <tagTWrap_Loong_Item*> &vecTar)
{
	return Loong_LoongMerge((std::vector <tagTWrap_Loong_Item*> &)vecSrc1,
							(std::vector <tagTWrap_Loong_Item*> &)vecSrc2,
							(std::vector <tagTWrap_Loong_Item*> &)vecTar,
							EM_TB_ITEM);
}
//itembaibao
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemBaiBao*> &vecSrc1,
											std::vector <tagTWrap_Loong_ItemBaiBao*> &vecSrc2,
											std::vector <tagTWrap_Loong_ItemBaiBao*> &vecTar)
{
	return Loong_LoongMerge((std::vector <tagTWrap_Loong_Item*> &)vecSrc1,
							(std::vector <tagTWrap_Loong_Item*> &)vecSrc2,
							(std::vector <tagTWrap_Loong_Item*> &)vecTar,
							EM_TB_ITEM_BAIBAO);
}
//itemDel
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemDel*> &vecSrc1,
											std::vector <tagTWrap_Loong_ItemDel*> &vecSrc2,
											std::vector <tagTWrap_Loong_ItemDel*> &vecTar)
{
	return Loong_LoongMerge((std::vector <tagTWrap_Loong_Item*> &)vecSrc1,
							(std::vector <tagTWrap_Loong_Item*> &)vecSrc2,
							(std::vector <tagTWrap_Loong_Item*> &)vecTar,
							EM_TB_ITEM_DEL);
}

//item
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Item*> &vecSrc1,
											std::vector <tagTWrap_Loong_Item*> &vecSrc2,
											std::vector <tagTWrap_Loong_Item*> &vecTar,
											ETBItemType emTBType)
{
	//item的serial num重排
	//如果跨大区合并 需要更新OwnerID(RoleID绑定物品) AccountID

	if(vecSrc1.size() == 0 && vecSrc2.size() == 0)
		return TRUE;

	//ContainerTypeID不变
	std::vector <tagTWrap_Loong_Item*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Item*>::iterator itSrc2 = vecSrc2.begin();

	// 当前时间
	tagDWORDTime stCurTm = GetCurrentDWORDTime();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;
	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	//全部重新排 建对照表
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		//
		if(emTBType == EM_TB_ITEM_DEL)
		{
			if(CalcTimeDiff(stCurTm, (*itSrc1)->stOthers.dwCreateTime) 
				<= ITEM_DEL_EQUIP_DEL_SURVIVE(m_nItemDelEquipDelSurviveDay))
			{
				m_setItemDelEquipDelSurvive.insert((*itSrc1)->stOthers.n64Serial);
			}
			else
			{
				continue;
			}
		}

		if(IsDirtyRoleData((*itSrc1)->stOthers.dwOwnerID))
		{
			EnterCriticalSection(&m_ContainerLock);
			m_setDirtyItem.insert((*itSrc1)->stOthers.n64Serial);
			LeaveCriticalSection(&m_ContainerLock);
			continue;
		}

		vecTar.push_back(*itSrc1);
	}

	// 处理大区内合并 同名账号在account_common中accountID不同的情况	
	const std::map <DWORD ,DWORD> &mpOldSrc2VsSrc1 = GetOldSrc2VsSrc1();
	std::map <DWORD ,DWORD>::const_iterator itOldSrc2VsSrc1 = mpOldSrc2VsSrc1.begin();

	//源2 更新ownerid和accountid
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		//ownerid
		Update_Loong_RoleID((*itSrc2)->stOthers.dwOwnerID);

		Update_Loong_Item64SerialNum((*itSrc2)->stOthers.n64Serial);

		if(emTBType == EM_TB_ITEM_DEL)
		{
			if(CalcTimeDiff(stCurTm, (*itSrc2)->stOthers.dwCreateTime) 
				<= ITEM_DEL_EQUIP_DEL_SURVIVE(m_nItemDelEquipDelSurviveDay))
			{
				m_setItemDelEquipDelSurvive.insert((*itSrc2)->stOthers.n64Serial);
			}
			else
			{
				continue;
			}
		}

		if(IsDirtyRoleData((*itSrc2)->stOthers.dwOwnerID))
		{
			EnterCriticalSection(&m_ContainerLock);
			m_setDirtyItem.insert((*itSrc2)->stOthers.n64Serial);
			LeaveCriticalSection(&m_ContainerLock);
			continue;
		}

		//如果是gm命令 或 玩家制作
		if((*itSrc2)->stOthers.eCreateMode == EICM_GM
			|| (*itSrc2)->stOthers.eCreateMode == EICM_Product)
			Update_Loong_RoleID((*itSrc2)->stOthers.dwCreateID);

		//
		if((*itSrc2)->stOthers.eCreateMode == EICM_Product)
			Update_Loong_RoleID((*itSrc2)->stOthers.dwCreatorID);

		//如果是帮派仓库物品
		if((*itSrc2)->stOthers.eConType == EICT_GuildWare)
		{
			//更新帮派id
			(*itSrc2)->stOthers.dwAccountID = \
				m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.dwAccountID,EM_DB_SRC_TWO)];
		}

		// 处理大区内合并 同名账号在account_common中accountID不同的情况		
		if (m_eMergeType == EM_MERGE_INSIDE_SECTION && (*itSrc2)->stOthers.eConType != EICT_GuildWare)
		{
			itOldSrc2VsSrc1 = mpOldSrc2VsSrc1.find((*itSrc2)->stOthers.dwAccountID);
			if ( itOldSrc2VsSrc1 != mpOldSrc2VsSrc1.end())
			{
				(*itSrc2)->stOthers.dwAccountID = (*itOldSrc2VsSrc1).second;
			}
		}


		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{

			if((*itSrc2)->stOthers.eConType != EICT_GuildWare)
			{
				itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->stOthers.dwAccountID,EM_DB_SRC_TWO));
				//如果找到
				if(itSrcFind != m_mpOldAccountVsNewAccount.end())
				{
					//accountid
					(*itSrc2)->stOthers.dwAccountID = (*itSrcFind).second;				
				}
			}	
		}
		vecTar.push_back(*itSrc2);
	}
	//如果是item_baibao需把m_vecOverItem里的东西放到vectar中 一起写入db
	if(emTBType == EM_TB_ITEM_BAIBAO)
	{
		std::vector <tagTWrap_Loong_Item>::iterator itOver = m_vecOverItem.begin();
		for(;itOver != m_vecOverItem.end();++itOver)
			vecTar.push_back(&(*itOver));
	}
	//////////////////////////////////////////////////////////////////////////
	//处理EICT_RoleWare EICT_Baibao  仓库 百宝物品超多的情况

	//如果不是item表 无需处理物品超多问题
	if(emTBType != EM_TB_ITEM)
		return TRUE;

	if(vecTar.empty())
		return TRUE;

	//vecTar 按AccountID RoleID排序
	std::sort(vecTar.begin(),vecTar.end(),Compare_Loong_ItemAcs);
	//检查 EICT_RoleWare EICT_Baibao 类型的item 如果超过24往 item_baibao里放
	std::vector <tagTWrap_Loong_Item*>::iterator itTar = vecTar.begin();
	stdext::hash_map <DWORD, ETBRoleRemoveType>::iterator itRoleIDvsRemoveType = m_mpRoleIDvsRemoveType.begin();	
	std::map <DWORD, DWORD>::iterator itAccountIDvsWareSize;

	DWORD dwWareItem = 0;
	DWORD dwBaibaoItem = 0;

	DWORD dwWarePos = 0; //位置
	DWORD dwBaibaoPos = 0;

	DWORD dwAccountPre = (*itTar)->stOthers.dwAccountID; //初始化为第一个account
	for(;itTar != vecTar.end();)
	{
		if((*itTar)->stOthers.dwAccountID == dwAccountPre)
		{
			if((*itTar)->stOthers.eConType != EICT_RoleWare
				&&(*itTar)->stOthers.eConType != EICT_Baibao)
			{
				++itTar;
				continue;
			}
			itRoleIDvsRemoveType = m_mpRoleIDvsRemoveType.find((*itTar)->stOthers.dwOwnerID);
			if(itRoleIDvsRemoveType != m_mpRoleIDvsRemoveType.end())
			{
				//如果该role是合并时被干掉
				if((*itRoleIDvsRemoveType).second == EM_R_MERGE)
				{
					//需要处理物品 目前绑定物品全部做删除处理　
					//非绑定的　如果仓库够位就放仓库　否则扔到item_baibao里
					//绑定的	尝试进行改绑定
					if((*itTar)->stOthers.byBind == EBS_Bind)
					{
						std::map <DWORD,DWORD>::iterator itFinder = m_mpAccountIDvsRoleIDforCB.find((*itTar)->stOthers.dwAccountID);
						if(itFinder != m_mpAccountIDvsRoleIDforCB.end())
						{
							(*itTar)->stOthers.dwOwnerID = itFinder->second;
						}
						//根本找不到该account中存在未删除账号
						else
						{
							//itTar = vecTar.erase(itTar);
							//continue;
						}
					}
				}
				//如果就是自删除role 还能剩下点儿未绑定的物品 也正常处理
				//绑定道具的话目前不处理
				else if((*itRoleIDvsRemoveType).second == EM_R_ONESELF)
				{
					if((*itTar)->stOthers.byBind == EBS_Bind)
					{
						//itTar = vecTar.erase(itTar);					
						//continue;
					}
				}
			}
			//该item拥有者未被删除
			else
			{}


			//////////////////////////////////////////////////////////////////////////
			if((*itTar)->stOthers.eConType == EICT_RoleWare)
			{
				itAccountIDvsWareSize = m_mpAccountIDvsWareSize.find( (*itTar)->stOthers.dwAccountID);
				if(itAccountIDvsWareSize == m_mpAccountIDvsWareSize.end())
				{
					tstringstream tszErr;
					tszErr << _T("accountid:") << (*itTar)->stOthers.dwAccountID << _T("拥有仓库物品但在account_common表中找不到对应记录！\r\n");
					g_clsOutput.Output(tszErr.str().c_str());
					itTar = vecTar.erase(itTar);
					continue;
				}
				//如果已经达到最大量
				if( dwWareItem >= (*itAccountIDvsWareSize).second )
				{
					//把自己扔到m_vecOverItem里
					m_vecOverItem.push_back(*(*itTar));

					//如果是武器
					if(MIsEquipment((*itTar)->stOthers.dwTypeID))
						m_setOverEquipSerialNum.insert((*itTar)->stOthers.n64Serial);

					//从tar表中删除该item 由于存的是指针 速度影响不大
					itTar = vecTar.erase(itTar);					
					continue;
				}
				else
				{
					//给这个item一个位置
					(*itTar)->stOthers.n16Index = dwWarePos;
					++dwWarePos;
					++dwWareItem;
				}
			}
			if((*itTar)->stOthers.eConType == EICT_Baibao)
			{
				//如果已经达到最大量
				if(dwBaibaoItem >= SPACE_BAIBAO_BAG)
				{
					//把自己扔到m_vecOverItem里
					m_vecOverItem.push_back(*(*itTar));

					//如果是武器
					if(MIsEquipment((*itTar)->stOthers.dwTypeID))
						m_setOverEquipSerialNum.insert((*itTar)->stOthers.n64Serial);

					//从tar表中删除该item
					itTar = vecTar.erase(itTar);					
					continue;
				}
				//给这个item一个位置
				else
				{
					//给这个item一个位置
					(*itTar)->stOthers.n16Index = dwBaibaoPos;
					++dwBaibaoItem;
					++dwBaibaoPos;
				}
			}

		}
		//切换了account
		else
		{	
			dwWarePos = 0; //位置
			dwBaibaoPos = 0;
			dwWareItem = 0;	
			dwBaibaoItem = 0;
			dwAccountPre = (*itTar)->stOthers.dwAccountID;

			//相关计数变量处理
			if((*itTar)->stOthers.eConType == EICT_RoleWare)
			{	
				(*itTar)->stOthers.n16Index = 0;
				++dwWareItem;
				++dwWarePos;
			}	
			if((*itTar)->stOthers.eConType == EICT_Baibao)
			{
				(*itTar)->stOthers.n16Index = 0;
				++dwBaibaoItem;
				++dwBaibaoPos;
			}
		}
		//下一个物品
		++itTar;
	}
	return TRUE;
}
//buff
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Buff*> &vecSrc1,
											std::vector <tagTWrap_Loong_Buff*> &vecSrc2,
											std::vector <tagTWrap_Loong_Buff*> &vecTar)
{
	//roleid srcunitid ItemSerialID
	std::vector <tagTWrap_Loong_Buff*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Buff*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc1)->stOthers.dwSrcUnitID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_Item64SerialNum((*itSrc2)->stOthers.n64Serial);
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->stOthers.dwSrcUnitID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc2)->stOthers.dwSrcUnitID))
			continue;

		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}
//帮派
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Guild*> &vecSrc1,
											std::vector <tagTWrap_Loong_Guild*> &vecSrc2,
											std::vector <tagTWrap_Loong_Guild*> &vecTar)
{
	//重排guild id 帮派名称小写的32位crc
	//如果帮派重名 加世界名当后缀处理
	//更新创始人nameid 现在就是roleid
	//其他不变
	std::vector <tagTWrap_Loong_Guild*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Guild*>::iterator itSrc2 = vecSrc2.begin();

	//
	std::set <tstring> setGuildName;
	std::set <tstring>::iterator itFind;
	std::set<DWORD> setAllNewGuildID; //临时 为了去重复

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	TCHAR tszGuildName[X_SHORT_NAME] ={0};
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		_tcsncpy(tszGuildName, (*itSrc1)->stOthers.szName, X_SHORT_NAME);
		tszGuildName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(tszGuildName);
		//插进去
		setGuildName.insert(tszGuildName);
		m_setOldGuildID.insert((*itSrc1)->stOthers.dwID);
		setAllNewGuildID.insert((*itSrc1)->stOthers.dwID);

		//帮主
		m_mapGuildLeader.insert((*itSrc1)->stOthers.dwLeaderRoleID);

		vecTar.push_back(*itSrc1);
	}
	DWORD dwNewGuildID;
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		m_setOldGuildID.insert((*itSrc2)->stOthers.dwID);
		//原来的id
		dwNewGuildID = (*itSrc2)->stOthers.dwID;

		//如果找到重名的 在名字后面加下划线
		_tcsncpy(tszGuildName, (*itSrc2)->stOthers.szName, X_SHORT_NAME);
		tszGuildName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(tszGuildName);

		itFind = setGuildName.find(tszGuildName);
		if(itFind != setGuildName.end())
		{
			tstring AddName;
			AddName += _T("_");
			AddName += m_strSrc2WorldName;
			StrCat((*itSrc2)->stOthers.szName, AddName.c_str());

			//重新算crc
			dwNewGuildID = LowerCrc32((*itSrc2)->stOthers.szName,MAX_GUILD_NAME_LEN);

		}

		//为了保证crc的绝对不重复 需要重新计算
		while(setAllNewGuildID.end() != setAllNewGuildID.find(dwNewGuildID))
		{
			//名字后面加个下划线重新计算
			tstring AddName;
			AddName += _T("_");
			StrCat((*itSrc2)->stOthers.szName,AddName.c_str());

			//重新算crc
			dwNewGuildID = LowerCrc32((*itSrc2)->stOthers.szName,MAX_GUILD_NAME_LEN);
		}
		

		//重新排源2id 目前不删除role
		Update_Loong_RoleID((*itSrc2)->stOthers.dwFounderNameID);
		Update_Loong_RoleID((*itSrc2)->stOthers.dwLeaderRoleID);
		//新旧id对照表
		m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.dwID,EM_DB_SRC_TWO)] = dwNewGuildID;
		(*itSrc2)->stOthers.dwID = dwNewGuildID;
		setAllNewGuildID.insert(dwNewGuildID);

		//帮主
		m_mapGuildLeader.insert((*itSrc2)->stOthers.dwLeaderRoleID);

		vecTar.push_back(*itSrc2);
	}
	//帮派排名
	std::sort(vecTar.begin(),vecTar.end(),Compare_Loong_GuildLess);

	std::vector<tagTWrap_Loong_Guild*>::iterator itTar = vecTar.begin();

	DWORD dwRank = 1;
	for(;itTar != vecTar.end();++itTar)
	{
		//把之前已经排名的 全部按威望重排
		if((*itTar)->stOthers.n16Rank != 0)
		{
			(*itTar)->stOthers.n16Rank = dwRank;
			++dwRank;
		}
	}


	return TRUE;
}
//BlackList
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_BlackList*> &vecSrc1,
											std::vector <tagTWrap_Loong_BlackList*> &vecSrc2,
											std::vector <tagTWrap_Loong_BlackList*> &vecTar)
{
	//roleid blackid
	std::vector <tagTWrap_Loong_BlackList*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_BlackList*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc1)->dwBlackID))
			continue;

		vecTar.push_back(*itSrc1);
	}
	
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->dwBlackID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc2)->dwBlackID))
			continue;

		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}
//enemy
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Enemy*> &vecSrc1,
											std::vector <tagTWrap_Loong_Enemy*> &vecSrc2,
											std::vector <tagTWrap_Loong_Enemy*> &vecTar)
{
	//roleid blackid
	std::vector <tagTWrap_Loong_Enemy*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Enemy*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc1)->dwEnemyID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->dwEnemyID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc2)->dwEnemyID))
			continue;

		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}
//equip
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Equip*> &vecSrc1,
											std::vector <tagTWrap_Loong_Equip*> &vecSrc2,
											std::vector <tagTWrap_Loong_Equip*> &vecTar,
											ETBEquipType eTBType)
{
	//更新序号
	std::vector <tagTWrap_Loong_Equip*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Equip*>::iterator itSrc2 = vecSrc2.begin();
	std::set<INT64>::iterator itFind;

	//std::map<tagItemNumKey ,INT64>::iterator itFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		// 数据过滤
		if(eTBType == EM_TB_EQUIP_DEL)
		{
			if(m_setItemDelEquipDelSurvive.find((*itSrc1)->n64SerialNum) == m_setItemDelEquipDelSurvive.end())
			{
				continue;
			}
		}

		if(m_setDirtyItem.end() != m_setDirtyItem.find((*itSrc1)->n64SerialNum))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_Item64SerialNum((*itSrc2)->n64SerialNum);

		// 数据过滤
		if(eTBType == EM_TB_EQUIP_DEL)
		{
			if(m_setItemDelEquipDelSurvive.find((*itSrc2)->n64SerialNum) == m_setItemDelEquipDelSurvive.end())
			{
				continue;
			}
		}
		
		if(m_setDirtyItem.end() != m_setDirtyItem.find((*itSrc2)->n64SerialNum))
			continue;

		if((*itSrc2)->stOthers.equipSpec.dwSignatruePlayerID != GT_INVALID)
			Update_Loong_RoleID((*itSrc2)->stOthers.equipSpec.dwSignatruePlayerID);

		//装备删除表 数据库有错误 必须这样处理
		//由于equip表和equip_del中有同serialnum的数据
		if(eTBType == EM_TB_EQUIP_DEL)
		{
			vecTar.push_back(*itSrc2);
		}
		else
		{
			//如果该武器属于超出物品
			itFind = m_setOverEquipSerialNum.find((*itSrc2)->n64SerialNum);
			if(itFind != m_setOverEquipSerialNum.end())
			{
				m_vecOverEquip.push_back(*(*itSrc2));
			}
			else
			{
				vecTar.push_back(*itSrc2);
			}

		}

	}
	//如果是equip_baibao表
	if(eTBType == EM_TB_EQUIP_BAIBAO)
	{
		std::vector <tagTWrap_Loong_Equip>::iterator itOver = m_vecOverEquip.begin();
		for(;itOver != m_vecOverEquip.end();++itOver)
			vecTar.push_back(&(*itOver));
	}

	return TRUE;
}
//Equip
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Equip*> &vecSrc1,
											std::vector <tagTWrap_Loong_Equip*> &vecSrc2,
											std::vector <tagTWrap_Loong_Equip*> &vecTar)
{
	return Loong_LoongMerge((std::vector <tagTWrap_Loong_Equip*> &)vecSrc1,
							(std::vector <tagTWrap_Loong_Equip*> &)vecSrc2,
							(std::vector <tagTWrap_Loong_Equip*> &)vecTar,
							EM_TB_EQUIP);
}
//EquipDel
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_EquipDel*> &vecSrc1,
											std::vector <tagTWrap_Loong_EquipDel*> &vecSrc2,
											std::vector <tagTWrap_Loong_EquipDel*> &vecTar)
{
	return Loong_LoongMerge((std::vector <tagTWrap_Loong_Equip*> &)vecSrc1,
							(std::vector <tagTWrap_Loong_Equip*> &)vecSrc2,
							(std::vector <tagTWrap_Loong_Equip*> &)vecTar,
							EM_TB_EQUIP_DEL);
}
//EquipBaiBao
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_EquipBaiBao*> &vecSrc1,
											std::vector <tagTWrap_Loong_EquipBaiBao*> &vecSrc2,
											std::vector <tagTWrap_Loong_EquipBaiBao*> &vecTar)
{
	return Loong_LoongMerge((std::vector <tagTWrap_Loong_Equip*> &)vecSrc1,
							(std::vector <tagTWrap_Loong_Equip*> &)vecSrc2,
							(std::vector <tagTWrap_Loong_Equip*> &)vecTar,
							EM_TB_EQUIP_BAIBAO);
}
//氏族
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ClanData*> &vecSrc1,
											std::vector <tagTWrap_Loong_ClanData*> &vecSrc2,
											std::vector <tagTWrap_Loong_ClanData*> &vecTar)
{
	//更新序号
	std::vector <tagTWrap_Loong_ClanData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ClanData*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}

//珍宝激活
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ActClanTreasure*> &vecSrc1,
											std::vector <tagTWrap_Loong_ActClanTreasure*> &vecSrc2,
											std::vector <tagTWrap_Loong_ActClanTreasure*> &vecTar)
{

	//更新激活人id
	std::vector <tagTWrap_Loong_ActClanTreasure*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ActClanTreasure*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	std::set<tagActClanTreasureKey> setPrimaryKey;
	std::set<tagActClanTreasureKey>::iterator itFind;

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc1)->dwActNameID))
			continue;

		//联合主键
		setPrimaryKey.insert(tagActClanTreasureKey((*itSrc1)->byClanType,(*itSrc1)->u16TreasureID));

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//要保证这个珍宝在源1中没有被激活过
		itFind = setPrimaryKey.find(tagActClanTreasureKey((*itSrc1)->byClanType,(*itSrc1)->u16TreasureID));
		if(itFind != setPrimaryKey.end())
		{
			Update_Loong_RoleID((*itSrc2)->dwRoleID);
			Update_Loong_RoleID((*itSrc2)->dwActNameID);

			if(IsDirtyRoleData((*itSrc2)->dwRoleID))
				continue;

			if(IsDirtyRoleData((*itSrc2)->dwActNameID))
				continue;

			vecTar.push_back(*itSrc2);
		}
	}
	return TRUE;
}
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecSrc1,
											 std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecSrc2,
											 std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecTar)
{
	//更新人id
	std::vector <tagTWrap_Loong_FameHallEnterSnap*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_FameHallEnterSnap*>::iterator itSrc2 = vecSrc2.begin();

	std::set<DWORD> setTime;
	std::set<DWORD>::iterator itFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{

		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc1)->dwEnterNameID))
			continue;

		setTime.insert((*itSrc1)->dwEnterTime);
		vecTar.push_back(*itSrc1);
	}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->dwEnterNameID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc2)->dwEnterNameID))
			continue;

		//处理相同时间的冲突
		while (setTime.find((*itSrc2)->dwEnterTime) != setTime.end())
		{
			(*itSrc2)->dwEnterTime = IncreaseTime((*itSrc2)->dwEnterTime,1);
		}
		setTime.insert((*itSrc2)->dwEnterTime);

		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_RepRstTime*> &vecSrc1,
											 std::vector <tagTWrap_Loong_RepRstTime*> &vecSrc2,
											 std::vector <tagTWrap_Loong_RepRstTime*> &vecTar)
{
	//取离当前时间最近的数据
	std::vector <tagTWrap_Loong_RepRstTime*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_RepRstTime*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	//比时间
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		for(;itSrc2!= vecSrc2.end();++itSrc2)
		{
			
			DWORD dwCur = GetCurrentDWORDTime();
			if((*itSrc2)->byClanType == (*itSrc1)->byClanType)
			{
				if(   CalcTimeDiff(dwCur,(*itSrc1)->dwRepRstTimeStamp) 
					> CalcTimeDiff(dwCur,(*itSrc2)->dwRepRstTimeStamp) )
				{
						vecTar.push_back(*itSrc2);
				}
				else
				{
						vecTar.push_back(*itSrc1);
				}
			}
			

		}
	}

	return TRUE;
}
//friend
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Friend*> &vecSrc1,
											std::vector <tagTWrap_Loong_Friend*> &vecSrc2,
											std::vector <tagTWrap_Loong_Friend*> &vecTar)
{

	//更新序号
	std::vector <tagTWrap_Loong_Friend*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Friend*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc1)->stOthers.dwFriendID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->stOthers.dwFriendID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc2)->stOthers.dwFriendID))
			continue;

		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}
//friendship
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_FriendshipSave*> &vecSrc1,
											std::vector <tagTWrap_Loong_FriendshipSave*> &vecSrc2,
											std::vector <tagTWrap_Loong_FriendshipSave*> &vecTar)
{

	//更新序号
	std::vector <tagTWrap_Loong_FriendshipSave*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_FriendshipSave*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc1)->stOthers.dwFriendID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->stOthers.dwFriendID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc2)->stOthers.dwFriendID))
			continue;

		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}
//GroupPurchase
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_GroupPurchase*> &vecSrc1,
											std::vector <tagTWrap_Loong_GroupPurchase*> &vecSrc2,
											std::vector <tagTWrap_Loong_GroupPurchase*> &vecTar)
{

	//更新序号
	std::vector <tagTWrap_Loong_GroupPurchase*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_GroupPurchase*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->stOthers.dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		Update_Loong_RoleID((*itSrc2)->stOthers.dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->stOthers.dwRoleID))
			continue;

		(*itSrc2)->stOthers.dwGuildID \
			= m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.dwGuildID,EM_DB_SRC_TWO)];

		//更新列表
		if((*itSrc2)->stOthers.nParticipatorNum > 0)
		{
			DWORD dwTemp;
			for(int iLoop = 0; iLoop<(*itSrc2)->stOthers.nParticipatorNum;iLoop++)
			{
				dwTemp = (DWORD)((*itSrc2)->pcBlob + sizeof(DWORD)*iLoop);
				Update_Loong_RoleID(dwTemp);
				memcpy((*itSrc2)->pcBlob + sizeof(DWORD)*iLoop,&dwTemp,sizeof(DWORD));
			}
		}	
		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}
//GuildMem
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildMem*> &vecSrc1,
											 std::vector <tagTWrap_Loong_GuildMem*> &vecSrc2,
											 std::vector <tagTWrap_Loong_GuildMem*> &vecTar)
{

	//更新roleid guildid
	std::vector <tagTWrap_Loong_GuildMem*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_GuildMem*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->stOthers.sGuildMember.dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	

		Update_Loong_RoleID((*itSrc2)->stOthers.sGuildMember.dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->stOthers.sGuildMember.dwRoleID))
			continue;

		(*itSrc2)->stOthers.dwGuildID = \
				m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.dwGuildID,EM_DB_SRC_TWO)];

		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}
//item_cdtime
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemCDTime*> &vecSrc1,
											 std::vector <tagTWrap_Loong_ItemCDTime*> &vecSrc2,
											 std::vector <tagTWrap_Loong_ItemCDTime*> &vecTar)
{
	std::vector <tagTWrap_Loong_ItemCDTime*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ItemCDTime*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{

		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}
//item_needlog
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemNeedLog*> &vecSrc1,
											 std::vector <tagTWrap_Loong_ItemNeedLog*> &vecSrc2,
											 std::vector <tagTWrap_Loong_ItemNeedLog*> &vecTar)
{

	//需要去重复
	std::vector <tagTWrap_Loong_ItemNeedLog*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ItemNeedLog*>::iterator itSrc2 = vecSrc2.begin();

	std::set<DWORD> setTypeID;
	std::set<DWORD>::iterator itFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setTypeID.insert((*itSrc1)->dwTypeID);
		vecTar.push_back(*itSrc1);
	}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		itFind = setTypeID.find((*itSrc2)->dwTypeID);
		//如果没找到
		if(itFind == setTypeID.end())
		{
			vecTar.push_back(*itSrc2);
		}
	}
	return TRUE;
}
//log_baibao
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_LogBaiBao*> &vecSrc1,
											 std::vector <tagTWrap_Loong_LogBaiBao*> &vecSrc2,
											 std::vector <tagTWrap_Loong_LogBaiBao*> &vecTar)
{
	//需要去重复
	std::vector <tagTWrap_Loong_LogBaiBao*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_LogBaiBao*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->stOthers.dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{

		Update_Loong_RoleID((*itSrc2)->stOthers.dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->stOthers.dwRoleID))
			continue;

		//如果跨大区
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);	
	}
	return TRUE;
}
//RoleDel
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_RoleDel*> &vecSrc1,
											 std::vector <tagTWrap_Loong_RoleDel*> &vecSrc2,
											 std::vector <tagTWrap_Loong_RoleDel*> &vecTar)
{
	//需要去重复
	std::vector <tagTWrap_Loong_RoleDel*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_RoleDel*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		//如果跨大区
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);	
	}
	return TRUE;
}
//pet_data
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_PetData*> &vecSrc1,
											 std::vector <tagTWrap_Loong_PetData*> &vecSrc2,
											 std::vector <tagTWrap_Loong_PetData*> &vecTar)
{
	//需要去重复
	std::vector <tagTWrap_Loong_PetData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_PetData*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->stOthers.dwMasterID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->stOthers.dwMasterID);

		if(IsDirtyRoleData((*itSrc2)->stOthers.dwMasterID))
			continue;

		(*itSrc2)->stOthers.dwPetID += (m_dwSrc1PetMaxNum - MIN_PET_ID);

		vecTar.push_back(*itSrc2);	
	}
	return TRUE;
}
//pet skill
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_PetSkill*> &vecSrc1,
											 std::vector <tagTWrap_Loong_PetSkill*> &vecSrc2,
											 std::vector <tagTWrap_Loong_PetSkill*> &vecTar)
{
	std::vector <tagTWrap_Loong_PetSkill*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_PetSkill*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		(*itSrc2)->dwPetID += (m_dwSrc1PetMaxNum - MIN_PET_ID);
		vecTar.push_back(*itSrc2);	
	}
	return TRUE;
}
//skill
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Skill*> &vecSrc1,
											 std::vector <tagTWrap_Loong_Skill*> &vecSrc2,
											 std::vector <tagTWrap_Loong_Skill*> &vecTar)
{
	//需要去重复
	std::vector <tagTWrap_Loong_Skill*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Skill*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);	
	}
	return TRUE;
}
//task
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Task*> &vecSrc1,
											 std::vector <tagTWrap_Loong_Task*> &vecSrc2,
											 std::vector <tagTWrap_Loong_Task*> &vecTar)
{
	//需要去重复
	std::vector <tagTWrap_Loong_Task*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Task*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);	
	}
	return TRUE;
}
//taskdone
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_TaskDone*> &vecSrc1,
											 std::vector <tagTWrap_Loong_TaskDone*> &vecSrc2,
											 std::vector <tagTWrap_Loong_TaskDone*> &vecTar)
{
	//需要去重复
	std::vector <tagTWrap_Loong_TaskDone*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_TaskDone*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);	
	}
	return TRUE;
}
//title
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Title*> &vecSrc1,
											 std::vector <tagTWrap_Loong_Title*> &vecSrc2,
											 std::vector <tagTWrap_Loong_Title*> &vecTar)
{
	//需要去重复
	std::vector <tagTWrap_Loong_Title*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Title*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);	
	}
	return TRUE;
}
//vip_stall
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_VipStall*> &vecSrc1,
											 std::vector <tagTWrap_Loong_VipStall*> &vecSrc2,
											 std::vector <tagTWrap_Loong_VipStall*> &vecTar)
{
	//需要去重复
	std::vector <tagTWrap_Loong_VipStall*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_VipStall*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	
	DWORD dwVipStallMaxNum = 0;

	//byStallIndex 从0开始
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{	
		if(IsDirtyRoleData((*itSrc1)->dwOwnerID))
			continue;

		if((*itSrc1)->nRemainTime > 0)
		{		
			//这个号是从0开始编的
			(*itSrc1)->byStallIndex = dwVipStallMaxNum++;
			vecTar.push_back(*itSrc1);
		}
	}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		if((*itSrc2)->nRemainTime > 0)
		{	
			//如果已经大于最大vip摊位数
			if(dwVipStallMaxNum >=  VIP_STALL_MAX_NUM )
				break;

			Update_Loong_RoleID((*itSrc2)->dwOwnerID);	

			if(IsDirtyRoleData((*itSrc2)->dwOwnerID))
				continue;

			//这个号是从0开始编的
			(*itSrc2)->byStallIndex = dwVipStallMaxNum++;
			vecTar.push_back(*itSrc2);
		}	
	}

	//补齐个数
	for(;dwVipStallMaxNum<VIP_STALL_MAX_NUM;dwVipStallMaxNum++)
	{
		tagTWrap_Loong_VipStall *pAdd = new tagTWrap_Loong_VipStall;
		pAdd->byStallIndex = dwVipStallMaxNum;
		pAdd->dwOwnerID = GT_INVALID;
		pAdd->nRemainTime = GT_INVALID;

		vecTar.push_back(pAdd);
	}


	return TRUE;
}
//VisitingCard
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_VisitingCard*> &vecSrc1,
											 std::vector <tagTWrap_Loong_VisitingCard*> &vecSrc2,
											 std::vector <tagTWrap_Loong_VisitingCard*> &vecTar)
{
	std::vector <tagTWrap_Loong_VisitingCard*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_VisitingCard*>::iterator itSrc2 = vecSrc2.begin();
	
	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->stOthers.dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc1)->stOthers.dwMateRoleID))
			(*itSrc1)->stOthers.dwMateRoleID = GT_INVALID;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->stOthers.dwRoleID);
		//配偶
		if((*itSrc2)->stOthers.dwMateRoleID != GT_INVALID)
			Update_Loong_RoleID((*itSrc2)->stOthers.dwMateRoleID);

		if(IsDirtyRoleData((*itSrc2)->stOthers.dwRoleID))
			continue;

		if(IsDirtyRoleData((*itSrc2)->stOthers.dwMateRoleID))
			(*itSrc2)->stOthers.dwMateRoleID = GT_INVALID;

		//帮派
		if((*itSrc2)->stOthers.dwFactionID != GT_INVALID)
			(*itSrc2)->stOthers.dwFactionID = \
				m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.dwFactionID,EM_DB_SRC_TWO)];		

		vecTar.push_back(*itSrc2);	
	}
	return TRUE;
}
//ybaccount
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_YBAccount*> &vecSrc1,
											 std::vector <tagTWrap_Loong_YBAccount*> &vecSrc2,
											 std::vector <tagTWrap_Loong_YBAccount*> &vecTar)
{
	std::vector <tagTWrap_Loong_YBAccount*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_YBAccount*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}
	
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);	
	}
	return TRUE;
}
//yuanbaoorder
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecSrc1,
											 std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecSrc2,
											 std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecTar)
{
	
	std::vector <tagTWrap_Loong_YuanBaoOrder*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_YuanBaoOrder*>::iterator itSrc2 = vecSrc2.begin();
	
	DWORD dwMaxID = 0;

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{	
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);

		if((*itSrc1)->dwID > dwMaxID)
		{
			dwMaxID = (*itSrc1)->dwID;
		}
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		//roleid
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		++dwMaxID;
		//索引
		(*itSrc2)->dwID = dwMaxID;

		vecTar.push_back(*itSrc2);	
	}
	
	return TRUE;
}
//bill item
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_BillItem*> &vecSrc1,
											 std::vector <tagTWrap_Loong_BillItem*> &vecSrc2,
											 std::vector <tagTWrap_Loong_BillItem*> &vecTar)
{
	
	std::vector <tagTWrap_Loong_BillItem*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_BillItem*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	//for(;itSrc1!= vecSrc1.end();++itSrc1)
	//{
	//	vecTar.push_back(*itSrc1);
	//}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//如果跨大区
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);	
	}
	
	return TRUE;
}
//bill yuanbao log
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecSrc1,
											 std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecSrc2,
											 std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecTar)
{

	std::vector <tagTWrap_Loong_BillYuanBaoLog*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_BillYuanBaoLog*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	//for(;itSrc1!= vecSrc1.end();++itSrc1)
	//{
	//	vecTar.push_back(*itSrc1);
	//}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//如果跨大区
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);	
	}

	return TRUE;
}
//bill yuanbao
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_BillYuanBao*> &vecSrc1,
											 std::vector <tagTWrap_Loong_BillYuanBao*> &vecSrc2,
											 std::vector <tagTWrap_Loong_BillYuanBao*> &vecTar)
{
	
	std::vector <tagTWrap_Loong_BillYuanBao*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_BillYuanBao*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	//for(;itSrc1!= vecSrc1.end();++itSrc1)
	//{
	//	vecTar.push_back(*itSrc1);
	//}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//如果跨大区
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);	
	}
	
	return TRUE;
}
//guild_commodity
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildCommodity*> &vecSrc1,
					  std::vector <tagTWrap_Loong_GuildCommodity*> &vecSrc2,
					  std::vector <tagTWrap_Loong_GuildCommodity*> &vecTar)
{
	std::vector <tagTWrap_Loong_GuildCommodity*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_GuildCommodity*>::iterator itSrc2 = vecSrc2.begin();
	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->stOthers.dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->stOthers.dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->stOthers.dwRoleID))
			continue;

		(*itSrc2)->dwGuildID = m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->dwGuildID,EM_DB_SRC_TWO)];

		vecTar.push_back(*itSrc2);	
	}

	return TRUE;
}
//guild_skill
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildSkill*> &vecSrc1,
					  std::vector <tagTWrap_Loong_GuildSkill*> &vecSrc2,
					  std::vector <tagTWrap_Loong_GuildSkill*> &vecTar)
{
	std::vector <tagTWrap_Loong_GuildSkill*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_GuildSkill*>::iterator itSrc2 = vecSrc2.begin();
	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		(*itSrc2)->dwGuildID = m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->dwGuildID,EM_DB_SRC_TWO)];

		vecTar.push_back(*itSrc2);	
	}

	return TRUE;

}
//vip_netbar
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_VipNetBar*> &vecSrc1,
					  std::vector <tagTWrap_Loong_VipNetBar*> &vecSrc2,
					  std::vector <tagTWrap_Loong_VipNetBar*> &vecTar)
{
	std::vector <tagTWrap_Loong_VipNetBar*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_VipNetBar*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	std::set<DWORD> setAccountID;
	std::set<DWORD>::iterator itAccountFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setAccountID.insert((*itSrc1)->dwAccountID);
		vecTar.push_back(*itSrc1);	
	}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//如果是跨大区的
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{		
				//更新一下id
				(*itSrc2)->dwAccountID = itSrcFind->second;			
			}
		}

		//如果找到重复的号
		itAccountFind = setAccountID.find((*itSrc2)->dwAccountID);
		if(itAccountFind != setAccountID.end())
		{
			continue;
		}


		vecTar.push_back(*itSrc2);	
	}

	return TRUE;
}
//guild_upgrade
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildFacilities*> &vecSrc1,
					  std::vector <tagTWrap_Loong_GuildFacilities*> &vecSrc2,
					  std::vector <tagTWrap_Loong_GuildFacilities*> &vecTar)
{
	std::vector <tagTWrap_Loong_GuildFacilities*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_GuildFacilities*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		(*itSrc2)->dwGuildID = m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->dwGuildID,EM_DB_SRC_TWO)];
		vecTar.push_back(*itSrc2);	
	}

	return TRUE;
}
//commerce_rank
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_CommerceRank*> &vecSrc1,
					  std::vector <tagTWrap_Loong_CommerceRank*> &vecSrc2,
					  std::vector <tagTWrap_Loong_CommerceRank*> &vecTar)
{
	std::vector <tagTWrap_Loong_CommerceRank*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_CommerceRank*>::iterator itSrc2 = vecSrc2.begin();
	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->stOthers.dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		Update_Loong_RoleID((*itSrc2)->stOthers.dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->stOthers.dwRoleID))
			continue;

		(*itSrc2)->dwGuildID = m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->dwGuildID,EM_DB_SRC_TWO)];
		vecTar.push_back(*itSrc2);	
	}

	return TRUE;
}
//过滤活动数据
VOID CLoongMergeLogicCtrl::FilterActivityData(std::vector <tagTWrap_Loong_ActivityData*> &vecData)
{
	std::vector <tagTWrap_Loong_ActivityData*>::iterator itData = vecData.begin();
	if(vecData.size() == 0)
		return;

	// 如果第一个字段有效 (第一次合并操作 在这里特殊处理为了兼容重复的合并)
	if(vecData[0]->bHaveData[0])
	{
		//移动数据
		for(;itData!= vecData.end();++itData)
		{
			// 所有数据有较标志位置false
			memset((*itData)->bHaveData, 0, sizeof((*itData)->bHaveData));

			// 把scriptdata移动到scriptdata1
			memcpy((*itData)->stAData[1].Data.dwData,
				(*itData)->stAData[0].Data.dwData, sizeof(DWORD)*ESD_Activity);
			
			// 标识第1个脚本数据有效
			(*itData)->bHaveData[1] = TRUE;
		}
	}
}

// 移动活动数据
VOID CLoongMergeLogicCtrl::MoveActivityData(DWORD dwActID, std::vector<tagTWrap_Loong_ActivityData*>::iterator itSrc,
					  std::vector<tagTWrap_Loong_ActivityData*>::iterator itTar, INT nValidPos, BOOL bSameContainer)
{
	// 无需移动
	if(nValidPos <= 1)
		return;

	// 移动有效的脚本数据
	for(INT nLoop = ACTIVITY_DATA_NUM - 1; nLoop > 0; --nLoop)
	{
		// 已经到头了
		if((nValidPos + nLoop - 1) >= ACTIVITY_DATA_NUM)
			continue;

		if(!(*itSrc)->bHaveData[nLoop])
			continue;

		// 更新数据
		this->UpdateSpecialActivityData(dwActID, (*itSrc)->stAData[nLoop]);

		// 移动数据
		memcpy((*itTar)->stAData[nValidPos + nLoop - 1].Data.dwData,
			(*itSrc)->stAData[nLoop].Data.dwData, sizeof(DWORD)*ESD_Activity);

		if(bSameContainer)
		{
			(*itTar)->bHaveData[nLoop] = FALSE;
			(*itTar)->bHaveData[nValidPos + nLoop - 1] = TRUE;
		}
		else
		{
			(*itTar)->bHaveData[nValidPos + nLoop - 1] = TRUE;
		}
	}
}


// 脚本数据特殊更新
VOID CLoongMergeLogicCtrl::UpdateSpecialActivityData(DWORD dwActID, tagActivityData& stAData)
{
	// 神迹争夺
	if(dwActID == 25)
	{
		//更新帮派id
		if(stAData.Data.dwData[0] != GT_INVALID
			&& stAData.Data.dwData[0] != 0)
		{
			stAData.Data.dwData[0] = \
				m_mpOldGuildIDvsNewGuildID[tagGuildIDKey(stAData.Data.dwData[0],EM_DB_SRC_TWO)];
		}

		//更新帮派id
		if(stAData.Data.dwData[2] != GT_INVALID
			&& stAData.Data.dwData[2] != 0)
		{
			stAData.Data.dwData[2] = \
				m_mpOldGuildIDvsNewGuildID[tagGuildIDKey(stAData.Data.dwData[2],EM_DB_SRC_TWO)];
		}

		//更新帮派id
		if(stAData.Data.dwData[4] != GT_INVALID
			&& stAData.Data.dwData[4] != 0)
		{
			stAData.Data.dwData[4] = \
				m_mpOldGuildIDvsNewGuildID[tagGuildIDKey(stAData.Data.dwData[4],EM_DB_SRC_TWO)];
		}
	}
}


//activity
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ActivityData*> &vecSrc1,
					  std::vector <tagTWrap_Loong_ActivityData*> &vecSrc2,
					  std::vector <tagTWrap_Loong_ActivityData*> &vecTar)
{
	std::vector <tagTWrap_Loong_ActivityData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ActivityData*>::iterator itSrc2 = vecSrc2.begin();
	std::vector <tagTWrap_Loong_ActivityData*>::iterator itTar = vecTar.begin();

	// 预留空间
	vecTar.reserve( max(vecSrc1.size() , vecSrc2.size()) );

	// 数据过滤
	this->FilterActivityData(vecSrc1);
	this->FilterActivityData(vecSrc2);

	// 先在src1中寻找一个有效的位置
	INT nValidPos = 1; //默认为1
	if(vecSrc1.size() != 0)
	{
		// 寻找一个有效的空位 用来填入src2的数据
		for(INT nLoop = 0; nLoop<ACTIVITY_DATA_NUM; ++nLoop)
		{
			if(vecSrc1[0]->bHaveData[nLoop])
			{
				nValidPos = nLoop + 1;
			}
		}
	}

	//把src1的数据压到tar中
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		// 设置src的nValidPos为有效 不管有没有有效数据
		if(vecSrc2.size() != 0)
			(*itSrc1)->bHaveData[nValidPos] = TRUE;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//在tar中寻找活动id相同的数据进行合并
		for(itTar = vecTar.begin(); itTar != vecTar.end(); ++itTar)
		{
			// 寻找对应的活动
			if((*itTar)->dwID != (*itSrc2)->dwID)
				continue;

			// 移动有效的脚本数据到tar中
			this->MoveActivityData((*itSrc2)->dwID, itSrc2, itTar, nValidPos, FALSE);

			break;
		}

		// 在tar中未找到对应的活动 直接将src2加入到tar(需要移动数据)
		if(itTar == vecTar.end())
		{
			// 移动有效的脚本数据
			this->MoveActivityData((*itSrc2)->dwID, itSrc2, itSrc2, nValidPos, TRUE);

			vecTar.push_back(*itSrc2);
		}
	}

	return TRUE;
}

//left msg
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_LeftMsg*> &vecSrc1,
											std::vector <tagTWrap_Loong_LeftMsg*> &vecSrc2,
											std::vector <tagTWrap_Loong_LeftMsg*> &vecTar)
{
	std::vector <tagTWrap_Loong_LeftMsg*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_LeftMsg*>::iterator itSrc2 = vecSrc2.begin();

	std::set<DWORD> setTime;
	std::set<DWORD>::iterator itFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		setTime.insert((*itSrc1)->dwMsgID);
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		//处理相同时间的冲突
		while (setTime.find((*itSrc2)->dwMsgID) != setTime.end())
		{
			(*itSrc2)->dwMsgID = IncreaseTime((*itSrc2)->dwMsgID,1);
		}
		setTime.insert((*itSrc2)->dwMsgID);

		vecTar.push_back(*itSrc2);	
	}

	return TRUE;
}


//外部链接
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ExternalLink*> &vecSrc1,
											std::vector <tagTWrap_Loong_ExternalLink*> &vecSrc2,
											std::vector <tagTWrap_Loong_ExternalLink*> &vecTar)
{
	std::vector <tagTWrap_Loong_ExternalLink*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ExternalLink*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	std::set<tstring> setlinkName;
	
	//最大链接数
	INT32 nExternalLinkNum = 0;
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		++nExternalLinkNum;
		setlinkName.insert((*itSrc1)->linkName);
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		if(setlinkName.find((*itSrc2)->linkName) != setlinkName.end())
		{
			continue;
		}

		if(nExternalLinkNum < EXTERNAL_LINKS_NUM)
		{
			++nExternalLinkNum;
		}
		else
		{
			break;
		}
		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}

//name history
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_NameHistory*> &vecSrc1,
											std::vector <tagTWrap_Loong_NameHistory*> &vecSrc2,
											std::vector <tagTWrap_Loong_NameHistory*> &vecTar)
{
	std::vector <tagTWrap_Loong_NameHistory*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_NameHistory*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		//如果是跨大区的
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);
	}

	return TRUE;
}

//city
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_City*> &vecSrc1,
											std::vector <tagTWrap_Loong_City*> &vecSrc2,
											std::vector <tagTWrap_Loong_City*> &vecTar)
{
	std::vector <tagTWrap_Loong_City*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_City*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	//保证src1数据库中有这个城市的信息才往目标里插
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		bool bFinded = false;
		for(;itSrc2!= vecSrc2.end();++itSrc2)
		{
			//城市id一样
			if((*itSrc1)->stOthers.sCityAtt.dwCityID == (*itSrc2)->stOthers.sCityAtt.dwCityID)
			{
				bFinded = true;
				if((*itSrc1)->stOthers.sCityAtt.dwGuildID != GT_INVALID || (*itSrc2)->stOthers.sCityAtt.dwGuildID == GT_INVALID)
				{
					vecTar.push_back(*itSrc1);
					break;
				}
				else if((*itSrc2)->stOthers.sCityAtt.dwGuildID != GT_INVALID)
				{
					//
					(*itSrc2)->stOthers.sCityAtt.dwGuildID = \
						m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.sCityAtt.dwGuildID,EM_DB_SRC_TWO)];

					vecTar.push_back(*itSrc2);
					break;
				}
			}
		}
		//没找到重号的
		if(bFinded == false)
		{
			vecTar.push_back(*itSrc1);
		}
	}
	return true;
}

//Master
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Master*> &vecSrc1,
											std::vector <tagTWrap_Loong_Master*> &vecSrc2,
											std::vector <tagTWrap_Loong_Master*> &vecTar)
{
	std::vector <tagTWrap_Loong_Master*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Master*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwID);

		if(IsDirtyRoleData((*itSrc2)->dwID))
			continue;

		for(int iLoop = 0; iLoop <MAX_APPRENTICE_NUM; iLoop++)
		{
			if((*itSrc2)->sApprentice.dwData[iLoop] != 0)
			{
				Update_Loong_RoleID((*itSrc2)->sApprentice.dwData[iLoop]);
			}
		}	
		vecTar.push_back(*itSrc2);
	}

	return true;
}

//快捷栏保存
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ShortCutBar*> &vecSrc1,
											std::vector <tagTWrap_Loong_ShortCutBar*> &vecSrc2,
											std::vector <tagTWrap_Loong_ShortCutBar*> &vecTar)
{
	std::vector <tagTWrap_Loong_ShortCutBar*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ShortCutBar*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;
	
		vecTar.push_back(*itSrc2);
	}

	return true;
}


//试炼状态
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_TrainState*> &vecSrc1,
											std::vector <tagTWrap_Loong_TrainState*> &vecSrc2,
											std::vector <tagTWrap_Loong_TrainState*> &vecTar)
{
	std::vector <tagTWrap_Loong_TrainState*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_TrainState*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);
	}

	return true;
}


//ActiveRankData
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ActiveRankData*> &vecSrc1,
											std::vector <tagTWrap_Loong_ActiveRankData*> &vecSrc2,
											std::vector <tagTWrap_Loong_ActiveRankData*> &vecTar)
{
	std::vector <tagTWrap_Loong_ActiveRankData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ActiveRankData*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleId))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleId);

		if(IsDirtyRoleData((*itSrc2)->dwRoleId))
			continue;

		if(m_setChangeNameRoleID.find((*itSrc2)->dwRoleId) != m_setChangeNameRoleID.end())
		{
			//需要改名字
			tstring AddName;
			AddName += _T("_");
			AddName += m_strSrc2WorldName;

			//role名字超长？
			StrCat((*itSrc2)->ucRoleName,AddName.c_str());
		}

		vecTar.push_back(*itSrc2);
	}

	return true;
}

//fabao
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Fabao*> &vecSrc1,
					  std::vector <tagTWrap_Loong_Fabao*> &vecSrc2,
					  std::vector <tagTWrap_Loong_Fabao*> &vecTar)
{
	std::vector <tagTWrap_Loong_Fabao*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Fabao*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(m_setDirtyItem.end() != m_setDirtyItem.find((*itSrc1)->n64Serial))
			continue;


		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_Item64SerialNum((*itSrc2)->n64Serial);

		if(m_setDirtyItem.end() != m_setDirtyItem.find((*itSrc2)->n64Serial))
			continue;

		vecTar.push_back(*itSrc2);
	}

	return true;
}

// F-Plan 1.5.0 SoulCrystal
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_SoulCrystal*> &vecSrc1,
											std::vector <tagTWrap_Loong_SoulCrystal*> &vecSrc2,
											std::vector <tagTWrap_Loong_SoulCrystal*> &vecTar)
{
	std::vector <tagTWrap_Loong_SoulCrystal*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_SoulCrystal*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(m_setDirtyItem.end() != m_setDirtyItem.find((*itSrc1)->n64SerialNum))
			continue;


		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_Item64SerialNum((*itSrc2)->n64SerialNum);

		if(m_setDirtyItem.end() != m_setDirtyItem.find((*itSrc2)->n64SerialNum))
			continue;

		vecTar.push_back(*itSrc2);
	}

	return true;
}

// F-Plan 1.6.0 holy
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Holy*> &vecSrc1,
					  std::vector <tagTWrap_Loong_Holy*> &vecSrc2,
					  std::vector <tagTWrap_Loong_Holy*> &vecTar)
{
	std::vector <tagTWrap_Loong_Holy*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Holy*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(m_setDirtyItem.end() != m_setDirtyItem.find((*itSrc1)->n64Serial))
			continue;


		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_Item64SerialNum((*itSrc2)->n64Serial);

		if(m_setDirtyItem.end() != m_setDirtyItem.find((*itSrc2)->n64Serial))
			continue;

		// 如果是源2，需修改圣灵的n64EquipSerialIDs中存放的圣纹64位ID
		for(int i=0; i<(*itSrc2)->stOthers.n16EquipmentNumber; i++)
		{
			if (i >= MaxHolyEquipNum)
			{
				break;
			}
			if ((*itSrc2)->stOthers.n64EquipSerialIDs[i] != 0)
			{
				Update_Loong_Item64SerialNum((*itSrc2)->stOthers.n64EquipSerialIDs[i]);
				if(m_setDirtyItem.end() != m_setDirtyItem.find((*itSrc2)->stOthers.n64EquipSerialIDs[i]))
					(*itSrc2)->stOthers.n64EquipSerialIDs[i] = 0;						
			}
		}

		vecTar.push_back(*itSrc2);
	}

	return true;
}

BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_HolyEquip*> &vecSrc1,
					  std::vector <tagTWrap_Loong_HolyEquip*> &vecSrc2,
					  std::vector <tagTWrap_Loong_HolyEquip*> &vecTar)
{
	std::vector <tagTWrap_Loong_HolyEquip*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_HolyEquip*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(m_setDirtyItem.end() != m_setDirtyItem.find((*itSrc1)->n64Serial))
			continue;


		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_Item64SerialNum((*itSrc2)->n64Serial);

		if(m_setDirtyItem.end() != m_setDirtyItem.find((*itSrc2)->n64Serial))
			continue;

		vecTar.push_back(*itSrc2);
	}

	return true;
}

// F-Plan 1.5.0 account_reactive, account_reactiver
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_AccountReactive*> &vecSrc1,
											std::vector <tagTWrap_Loong_AccountReactive*> &vecSrc2,
											std::vector <tagTWrap_Loong_AccountReactive*> &vecTar)
{
	std::vector <tagTWrap_Loong_AccountReactive*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_AccountReactive*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	std::set<DWORD> setAccountID;
	std::set<DWORD>::iterator itAccountFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setAccountID.insert((*itSrc1)->dwAccountID);
		vecTar.push_back(*itSrc1);	
	}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//如果是跨大区的
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{		
				//更新一下id
				(*itSrc2)->dwAccountID = itSrcFind->second;			
			}
		}

		//如果找到重复的号
		itAccountFind = setAccountID.find((*itSrc2)->dwAccountID);
		if(itAccountFind != setAccountID.end())
		{
			continue;
		}


		vecTar.push_back(*itSrc2);	
	}

	return TRUE;

}

BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_AccountReactiver*> &vecSrc1,
											std::vector <tagTWrap_Loong_AccountReactiver*> &vecSrc2,
											std::vector <tagTWrap_Loong_AccountReactiver*> &vecTar)
{
	std::vector <tagTWrap_Loong_AccountReactiver*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_AccountReactiver*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	std::set<DWORD> setAccountID;
	std::set<DWORD>::iterator itAccountFind;

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setAccountID.insert((*itSrc1)->dwAccountID);
		vecTar.push_back(*itSrc1);	
	}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//如果是跨大区的
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{		
				//更新一下id
				(*itSrc2)->dwAccountID = itSrcFind->second;			
			}
		}

		//如果找到重复的号
		itAccountFind = setAccountID.find((*itSrc2)->dwAccountID);
		if(itAccountFind != setAccountID.end())
		{
			continue;
		}


		vecTar.push_back(*itSrc2);	
	}

	return TRUE;

}

//  F-Plan 1.5.12 UseKeyCode_roleID
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_UseKeyCode*> &vecSrc1,
					   std::vector <tagTWrap_Loong_UseKeyCode*> &vecSrc2,
					   std::vector <tagTWrap_Loong_UseKeyCode*> &vecTar)
{

	std::vector <tagTWrap_Loong_UseKeyCode*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_UseKeyCode*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		if(IsDirtyRoleData((*itSrc1)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);
	}

	return true;

}

//ForceBreakOutApprentice
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecSrc1,
											std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecSrc2,
											std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecTar)
{
	std::vector <tagTWrap_Loong_ForceBreakOutApprentice*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ForceBreakOutApprentice*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwApprenticeID);
		Update_Loong_RoleID((*itSrc2)->dwMasterID);

		vecTar.push_back(*itSrc2);
	}

	return true;
}

//任务板
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Sheng_BoardQuestSaveData*> &vecSrc1,
					  std::vector <tagTWrap_Sheng_BoardQuestSaveData*> &vecSrc2,
					  std::vector <tagTWrap_Sheng_BoardQuestSaveData*> &vecTar)
{
	std::vector <tagTWrap_Sheng_BoardQuestSaveData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Sheng_BoardQuestSaveData*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		vecTar.push_back(*itSrc2);
	}

	return true;
}

//帮派宣战
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecSrc1,
					  std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecSrc2,
					  std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecTar)
{
	std::vector <tagTWrap_Sheng_GuildWarDeclare*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Sheng_GuildWarDeclare*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		(*itSrc2)->dwAttackGuildID = \
			m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->dwAttackGuildID,EM_DB_SRC_TWO)];

		(*itSrc2)->dwDefenceGuildID = \
			m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->dwDefenceGuildID,EM_DB_SRC_TWO)];

		vecTar.push_back(*itSrc2);
	}

	return true;
}

//角色在线时间统计
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Sheng_CurMonthOnlineTime*> &vecSrc1,
											std::vector <tagTWrap_Sheng_CurMonthOnlineTime*> &vecSrc2,
											std::vector <tagTWrap_Sheng_CurMonthOnlineTime*> &vecTar)
{
	std::vector <tagTWrap_Sheng_CurMonthOnlineTime*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Sheng_CurMonthOnlineTime*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
	
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		//如果跨大区
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);
	}

	return true;
}


//妖精重生数据
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Sheng_FabaoNirVanaData*> &vecSrc1,
											std::vector <tagTWrap_Sheng_FabaoNirVanaData*> &vecSrc2,
											std::vector <tagTWrap_Sheng_FabaoNirVanaData*> &vecTar)
{
	std::vector <tagTWrap_Sheng_FabaoNirVanaData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Sheng_FabaoNirVanaData*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{

		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		Update_Loong_Item64SerialNum((*itSrc2)->n64ID);

		//如果跨大区
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//如果找到
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);
	}

	return true;
}


//神之竞标
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Sheng_GodBid*> &vecSrc1,
					  std::vector <tagTWrap_Sheng_GodBid*> &vecSrc2,
					  std::vector <tagTWrap_Sheng_GodBid*> &vecTar)
{
	std::vector <tagTWrap_Sheng_GodBid*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Sheng_GodBid*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{

		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);
	}

	return true;
}

//砸蛋
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Sheng_EggInfo*> &vecSrc1,
					  std::vector <tagTWrap_Sheng_EggInfo*> &vecSrc2,
					  std::vector <tagTWrap_Sheng_EggInfo*> &vecTar)
{
	std::vector <tagTWrap_Sheng_EggInfo*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Sheng_EggInfo*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{

		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);
	}

	return true;
}


//衣橱
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Sheng_Wardrobe*> &vecSrc1,
					  std::vector <tagTWrap_Sheng_Wardrobe*> &vecSrc2,
					  std::vector <tagTWrap_Sheng_Wardrobe*> &vecTar)
{
	std::vector <tagTWrap_Sheng_Wardrobe*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Sheng_Wardrobe*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->dwFriendID);

		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);
	}

	return true;
}


//领域
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Sheng_Area*> &vecSrc1,
					  std::vector <tagTWrap_Sheng_Area*> &vecSrc2,
					  std::vector <tagTWrap_Sheng_Area*> &vecTar)
{
	std::vector <tagTWrap_Sheng_Area*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Sheng_Area*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		if(IsDirtyRoleData((*itSrc2)->dwRoleID))
			continue;

		vecTar.push_back(*itSrc2);
	}

	return true;
}

//稀有领域
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Sheng_RareArea*> &vecSrc1,
					  std::vector <tagTWrap_Sheng_RareArea*> &vecSrc2,
					  std::vector <tagTWrap_Sheng_RareArea*> &vecTar)
{

	std::vector <tagTWrap_Sheng_RareArea*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Sheng_RareArea*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		if(  0 == (*itSrc2)->eType)
			Update_Loong_Item64SerialNum((*itSrc2)->n64ID);
	
		vecTar.push_back(*itSrc2);
	}

	return true;

}
//----------------------------------------------------------------------------------------------------------------
//Family
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Family*> &vecSrc1,
											 std::vector <tagTWrap_Loong_Family*> &vecSrc2,
											 std::vector <tagTWrap_Loong_Family*> &vecTar)
{
	//暂时这样写，以后有特殊处理在这里加
	std::vector <tagTWrap_Loong_Family*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Family*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	TCHAR tszFamilyName[X_SHORT_NAME] = {0};
	std::set<tstring> setSrc1FamilyName;
	//源1
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		_tcsncpy(tszFamilyName, (*itSrc1)->stFamilyAtt.strFamilyName, X_SHORT_NAME);
		tszFamilyName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(tszFamilyName);

		setSrc1FamilyName.insert(tszFamilyName);

		vecTar.push_back(*itSrc1);
	}
	
	std::set<tstring>::iterator itFind;
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		_tcsncpy(tszFamilyName, (*itSrc2)->stFamilyAtt.strFamilyName, X_SHORT_NAME);
		tszFamilyName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(tszFamilyName);

		itFind = setSrc1FamilyName.find(tszFamilyName);
		if(itFind != setSrc1FamilyName.end())
		{
			tstring AddName;
			AddName += _T("_");
			AddName += m_strSrc2WorldName;

			StrCat((*itSrc2)->stFamilyAtt.strFamilyName,AddName.c_str());
			if(_tcslen((*itSrc2)->stFamilyAtt.strFamilyName) > X_SHORT_NAME-1)
			{
				(*itSrc2)->stFamilyAtt.strFamilyName[X_SHORT_NAME-1] = '\0';
			}

			tagFamilyNeedChangeName *pFamilyNeedChangeName = new tagFamilyNeedChangeName;
			if( !P_VALID(pFamilyNeedChangeName) )
				return FALSE;

			pFamilyNeedChangeName->dwOldCRCID = (*itSrc2)->stFamilyAtt.sFamilyAtt.dwFamilyID;
			pFamilyNeedChangeName->dwNewCRCID2 = IUTIL->Crc32((*itSrc2)->stFamilyAtt.strFamilyName);
			(*itSrc2)->stFamilyAtt.sFamilyAtt.dwFamilyID = pFamilyNeedChangeName->dwNewCRCID2;

			m_vecNeedChangeFamilyName.push_back(pFamilyNeedChangeName);
		}

		Update_Loong_RoleID((*itSrc2)->stFamilyAtt.sFamilyAtt.dwFounderID);
		Update_Loong_RoleID((*itSrc2)->stFamilyAtt.sFamilyAtt.dwLeaderID);

		vecTar.push_back(*itSrc2);
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------
//FamilyMem
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilyMem*> &vecSrc1,
											 std::vector <tagTWrap_Loong_FamilyMem*> &vecSrc2,
											 std::vector <tagTWrap_Loong_FamilyMem*> &vecTar)
{
	//暂时这样写，以后有特殊处理在这里加
	std::vector <tagTWrap_Loong_FamilyMem*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_FamilyMem*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		Update_Loong_RoleID((*itSrc2)->stFamilyMem.dwRoleID);
		
		Update_Loong_FamilyNameCRC((*itSrc2)->stFamilyMem.dwFamilyID);			

		vecTar.push_back(*itSrc2);
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------
//FamilyQuset
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilyQuset*> &vecSrc1,
											 std::vector <tagTWrap_Loong_FamilyQuset*> &vecSrc2,
											 std::vector <tagTWrap_Loong_FamilyQuset*> &vecTar)
{
	//暂时这样写，以后有特殊处理在这里加
	std::vector <tagTWrap_Loong_FamilyQuset*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_FamilyQuset*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		/*if(m_eMergeType == EM_MERGE_CROSS_SECTION)*/
		Update_Loong_FamilyNameCRC((*itSrc2)->stFamilyQuest.dwFamilyID);

		vecTar.push_back(*itSrc2);
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------
//FamilyRoleSprite
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecSrc1,
											 std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecSrc2,
											 std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecTar)
{
	//暂时这样写，以后有特殊处理在这里加
	std::vector <tagTWrap_Loong_FamilyRoleSprite*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_FamilyRoleSprite*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		Update_Loong_RoleID((*itSrc2)->stFamilyRoleSprite.dwRoleID);

		Update_Loong_FamilyNameCRC((*itSrc2)->dwFamilyID);

		vecTar.push_back(*itSrc2);
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------
//FamilySprite
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilySprite*> &vecSrc1,
											 std::vector <tagTWrap_Loong_FamilySprite*> &vecSrc2,
											 std::vector <tagTWrap_Loong_FamilySprite*> &vecTar)
{
	//暂时这样写，以后有特殊处理在这里加
	std::vector <tagTWrap_Loong_FamilySprite*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_FamilySprite*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		Update_Loong_FamilyNameCRC((*itSrc2)->stFamilySprite.dwFamilyID);

		vecTar.push_back(*itSrc2);
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------
//SpriteRequirement
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_SpriteRequirement*> &vecSrc1,
											 std::vector <tagTWrap_Loong_SpriteRequirement*> &vecSrc2,
											 std::vector <tagTWrap_Loong_SpriteRequirement*> &vecTar)
{
	//暂时这样写，以后有特殊处理在这里加
	std::vector <tagTWrap_Loong_SpriteRequirement*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_SpriteRequirement*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{	
		Update_Loong_FamilyNameCRC((*itSrc2)->dwFamilyID);

		vecTar.push_back(*itSrc2);
	}

	return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------
VOID  CLoongMergeLogicCtrl::Update_Loong_FamilyNameCRC(DWORD &dwFamilyID)
{
	std::vector<tagFamilyNeedChangeName*>::iterator it;

	for( it = m_vecNeedChangeFamilyName.begin(); it != m_vecNeedChangeFamilyName.end(); ++it )
	{
		if(dwFamilyID == (*it)->dwOldCRCID)
		{
			dwFamilyID = (*it)->dwNewCRCID2;
			return;
		}
	}
}
//----------------------------------------------------------------------------------------------------------------
//login 全局家族名
BOOL CLoongMergeLogicCtrl::Loong_LoginMerge(std::vector <tagTWrap_Longin_FamilyName*> &vecSrc1,
											std::vector <tagTWrap_Longin_FamilyName*> &vecSrc2,
											std::vector <tagTWrap_Longin_FamilyName*> &vecTar)
{
	std::vector <tagTWrap_Longin_FamilyName*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Longin_FamilyName*>::iterator itSrc2 = vecSrc2.begin();

	if(m_eMergeType != EM_MERGE_CROSS_SECTION)
	{
		return TRUE;
	}
	//预留空间
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());


	TCHAR tszFamilyName[X_SHORT_NAME] = {0};
	std::set<tstring> setSrc1FamilyName;
	//源1
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		_tcsncpy(tszFamilyName, (*itSrc1)->szName, X_SHORT_NAME);
		tszFamilyName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(tszFamilyName);

		setSrc1FamilyName.insert(tszFamilyName);

		vecTar.push_back(*itSrc1);
	}

	// 直接插入
	BOOL bRet = sLoginBeton.LoginDB_Accout_Insert(vecTar,EM_DB_TAR);
	if(bRet == FALSE)
	{
		return bRet;
	}
	// 清空所有src1的
	vecTar.clear();

	std::set<tstring>::iterator itFind;
	//如果跨大区
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		_tcsncpy(tszFamilyName, (*itSrc2)->szName, X_SHORT_NAME);
		tszFamilyName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(tszFamilyName);

		itFind = setSrc1FamilyName.find(tszFamilyName);
		if(itFind != setSrc1FamilyName.end())
		{
			tstring AddName;
			AddName += _T("_");
			AddName += m_strSrc2WorldName;

			StrCat((*itSrc2)->szName,AddName.c_str());
			if(_tcslen((*itSrc2)->szName) > X_SHORT_NAME-1)
			{
				(*itSrc2)->szName[X_SHORT_NAME-1] = '\0';
			}
		}

		vecTar.push_back(*itSrc2);
		
		// 试一次
		//sLoginBeton.LoginDB_Accout_Insert(vecTar,EM_DB_TAR);

		//// 清空这一条记录
		//vecTar.clear();
	}
	sLoginBeton.LoginDB_Accout_Insert(vecTar,EM_DB_TAR);

	// 清空所有tar的记录
	vecTar.clear();

	return true;
}
//------------------------------------------------------------------------------------------------
//GmServerData
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_GmServerData*> &vecSrc1,
											 std::vector <tagTWrap_Loong_GmServerData*> &vecSrc2,
											 std::vector <tagTWrap_Loong_GmServerData*> &vecTar)
{
	//暂时这样写，以后有特殊处理在这里加
	std::vector <tagTWrap_Loong_GmServerData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_GmServerData*>::iterator itSrc2 = vecSrc2.begin();

	//预留空间
	vecTar.reserve(1);

	if(itSrc1 != vecSrc1.end() && itSrc2 != vecSrc2.end())
	{
		if((*itSrc1)->n64Value > (*itSrc2)->n64Value)
		{
			vecTar.push_back(*itSrc2);
		}
		else
		{
			vecTar.push_back(*itSrc1);
		}
	}
	else
	{
		if(itSrc1 == vecSrc1.end())
		{
			vecTar.push_back(*itSrc2);
		}
		else
		{
			vecTar.push_back(*itSrc1);
		}
	}

	return TRUE;
}

