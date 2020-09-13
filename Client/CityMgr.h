#pragma once

#include "..\WorldDefine\city_define.h"


struct tagNS_GetCityInfo;
struct tagNS_GetAppointedCityAtt;
struct tagNS_SetTaxRate;
struct tagNS_TransformTaxation;
struct tagNS_GetSigupInfo;
struct tagNS_GetCityWarCity;
struct tagNS_UpgradeProlificacy;
struct tagNS_GetRoleInitState_City;
struct tagOpenCityEvent;
struct tagNS_ConfirmDefenceForce;
struct tagNS_CityStruggleInfo;
struct tagNS_CityStruggleInfoChange;

struct tagCityNameAndIcon
{
	tstring strCityName;
	tstring strIconPath;
	tstring strGodName;
};

struct tagCityProduct
{
	DWORD dwTypeID;
	INT32 nNum;
	INT32 nLimit;
};

class CityMgr
{
public:
	static CityMgr *Inst();
	~CityMgr();

	void Init();
	void Destroy();
	static VOID	RegisterFrames(GameFrameMgr* pMgr);
	
	VOID SendGetCityInfo(DWORD dwNPCID);
	VOID SendGetAppointedCityAtt(DWORD dwNPCID, ECityAttType eType);
	VOID SendSetTaxRate(DWORD dwNPCID, INT32 nTaxRate);
	VOID SendTransformTax(DWORD dwNPCID, ETaxTransformType eType, INT32 nTax);
	VOID SendGetApplyInfo(DWORD dwNPCID);
	VOID SendGetCityWarCity();
	DWORD GetCityID(const tstring &strMapName) const;
	tstring GetCityName(DWORD dwCityID) const;
	tstring GetCityIcon(DWORD dwCityID) const;
	tstring GetCityGod(DWORD dwCityID) const;
	INT GetMinSigfare(DWORD dwCityID) const;
	INT32 GetTaxRate(DWORD dwCityID) const;
	BOOL IsCityWarMap(DWORD dwMapID) const;
	BOOL IsDragonBeDestoryed(DWORD dwCityID) const;
	BOOL IsColumnBeDestoryed(DWORD dwCityID, INT nIndex) const;
	BOOL IsDoorBeDestoryed(DWORD dwCityID, INT nIndex) const;

	// 查询产出物品最低限制
	INT32 GetProductMinLimit(DWORD dwCityID, DWORD dwItemTypeID);
	VOID GetCityDefaultProduct(DWORD dwCityID, std::vector<tagCityProduct>& out);

public:
	typedef std::map<tstring, DWORD> MapNameToCityIDMap;
	typedef std::map<DWORD, tagCityNameAndIcon> CityIDToNameMap;
	typedef std::map<DWORD, INT> CityIDToMinSigFareMap;
	typedef std::map<DWORD, INT32> CityIDToTaxRateMap;
	typedef std::map<DWORD, std::vector<tagCityProduct> > CityIDToProductMap;
	typedef std::map<DWORD, BOOL> CityIDToStateMap;	// TRUE表示被攻下


	static const int NUM_OF_COLUMN = 5;
	static const int NUM_OF_DOOR = 3;
private:
	CityMgr();
	TSFPTrunk<CityMgr> m_Trunk;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;
	TObjRef<NetSession> m_pSession;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	
	DWORD OnEventOpenCity(tagGameEvent *pEvent);
	DWORD OnEventOpenCityByID(tagOpenCityEvent *pEvent);
	DWORD OnEventOpenCitySetRate(tagGameEvent *pEvent);
	DWORD OnEventOpenCityPickUpTax(tagGameEvent *pEvent);
	DWORD OnEventOpenCityWarApply(tagGameEvent *pEvent);
	DWORD OnMsgBoxEventViewCityInfo(tagMsgBoxEvent *pEvent);

	DWORD OnNetGetCityInfo(tagNS_GetCityInfo *pNetCmd, DWORD);
	DWORD OnNetGetAppointedCityAtt(tagNS_GetAppointedCityAtt *pNetCmd, DWORD);
	DWORD OnNetSetTaxRate(tagNS_SetTaxRate *pNetCmd, DWORD);
	DWORD OnNetGetApplyInfo(tagNS_GetSigupInfo *pNetCmd, DWORD);
	DWORD OnNetUpgradeProlificacy(tagNS_UpgradeProlificacy *pNetCmd, DWORD);
	DWORD OnNetGetRoleInitStateCity(tagNS_GetRoleInitState_City *pNetCmd, DWORD);
	DWORD OnNetConfirmDefenceForce(tagNS_ConfirmDefenceForce *pNetCmd, DWORD);
	DWORD OnNetCityStruggleInfo(tagNS_CityStruggleInfo*, DWORD);
	DWORD OnNetCityStruggleInfoChange(tagNS_CityStruggleInfoChange*, DWORD);
	
	MapNameToCityIDMap m_mapName2CityID;
	CityIDToNameMap m_mapCityID2Name;
	CityIDToMinSigFareMap m_mapCityID2MinSigfare;
	CityIDToTaxRateMap m_mapCityID2TaxRate;
	CityIDToProductMap m_mapProduct;
	CityIDToStateMap m_mapDragonState;	// 龙珠状态
	CityIDToStateMap m_mapColumnState[NUM_OF_COLUMN];  // 阴阳柱状态
	CityIDToStateMap m_mapDoorState[NUM_OF_DOOR];	// 城门状态

	bool m_bSendGetCityInfo;
	bool m_bSendGetLastRateSetTime;
	bool m_bSendGetTax;
	bool m_bSendSetTaxRate;
};

inline BOOL CityMgr::IsDragonBeDestoryed(DWORD dwCityID) const
{
	CityIDToStateMap::const_iterator itor = m_mapDragonState.find(dwCityID);
	if(itor != m_mapDragonState.end()) return itor->second;
	return FALSE;
}

inline BOOL CityMgr::IsColumnBeDestoryed(DWORD dwCityID, INT nIndex) const
{
	if(nIndex<0 || nIndex>= NUM_OF_COLUMN) return FALSE;
	CityIDToStateMap::const_iterator itor = m_mapColumnState[nIndex].find(dwCityID);
	if(itor != m_mapColumnState[nIndex].end()) return itor->second;
	return FALSE;
}

inline BOOL CityMgr::IsDoorBeDestoryed(DWORD dwCityID, INT nIndex) const
{
	if(nIndex < 0 || nIndex >= NUM_OF_DOOR) return FALSE;
	CityIDToStateMap::const_iterator itor = m_mapDoorState[nIndex].find(dwCityID);
	if(itor != m_mapDoorState[nIndex].end()) return itor->second;
	return FALSE;
}

inline BOOL CityMgr::IsCityWarMap(DWORD dwMapID) const
{
	return dwMapID == TObjRef<Util>()->Crc32(_T("c03"));
	//return dwMapID!= GT_INVALID && m_mapDragonState.find(dwMapID) != m_mapDragonState.end();
}
