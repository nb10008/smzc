#include "stdafx.h"
#include "CityFrame.h"
#include "GuildMgr.h"
#include "CityMgr.h"
#include "..\WorldDefine\city_define.h"
#include "CityEvent.h"
#include "ItemProtoData.h"
#include "ToolTipFrame.h"
#include "ToolTipCreator.h"
#include "RoleMgr.h"
#include "QuestMgr.h"


CityFrame::CityFrame()
 : m_Trunk(this)
 , m_pWnd(NULL)
 , m_pBtnClose(NULL)
 , m_pIstCityIcon(NULL)
 , m_pStcCityName(NULL)
 , m_pIstGuildIcon(NULL)
 , m_pStcGuildName(NULL)
 , m_pStcDoorType(NULL)
 , m_pStcPatronGod(NULL)
 , m_pStcTaxRate(NULL)
 , m_pStcBaseFundProduction(NULL)
 , m_pStcCurrentFundProduction(NULL)
 , m_pStcDoorDefence(NULL)
 , m_pStcGodPrivity(NULL)
 , m_pStcProductivity(NULL)
 , m_pStcBaseMaterialProduction(NULL)
 , m_pStcCurrentMaterialProduction(NULL)
{
	ZeroMemory(m_pIstItems, sizeof(m_pIstItems));
	ZeroMemory(m_pStcProText, sizeof(m_pStcProText));
	ZeroMemory(m_Items, sizeof(m_Items));
	m_dwCount = 0;
	m_dwTime = 0;
}

CityFrame::~CityFrame()
{
	
}

BOOL CityFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;
	
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), m_Trunk.sfp1(&CityFrame::EventHandler));

	m_pFrameMgr->RegisterEventHandle(_T("Refresh_CityAtt"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityFrame::OnEventRefreshCityAtt)));
	
	return TRUE;
}

BOOL CityFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;
	
	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName(), m_Trunk.sfp1(&CityFrame::EventHandler));

	m_pFrameMgr->UnRegisterEventHandler(_T("Refresh_CityAtt"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityFrame::OnEventRefreshCityAtt)));

	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	return TRUE;

}

BOOL CityFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\city.xml");
	if (!m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str()))
		return FALSE;

	m_pGUI->ChangeXml(&element, "city_win\\city_pic\\back\\imgback6\\img", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&element, "city_win\\city_pic\\back\\imgback7\\img", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&element, "city_win\\city_pic\\back\\imgback1\\img", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&element, "city_win\\city_pic\\back\\imgback2\\img", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&element, "city_win\\city_pic\\back\\imgback3\\img", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&element, "city_win\\city_pic\\back\\imgback4\\img", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&element, "city_win\\city_pic\\back\\imgback5\\img", "ClassName", "IconStatic");

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	if (!P_VALID(m_pWnd))
		return FALSE;
	
	m_pBtnClose = static_cast<GUIButton*>(m_pWnd->GetChild(_T("city_pic\\close")));
	m_pIstCityIcon = static_cast<IconStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\imgback6\\img")));
	m_pStcCityName = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\word3")));
	m_pIstGuildIcon = static_cast<IconStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\imgback7\\img")));
	m_pStcGuildName = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\word4")));
	m_pStcDoorType = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\back\\num1")));
	m_pStcPatronGod = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\back\\num2")));
	m_pStcTaxRate = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\back\\num3")));
	m_pStcBaseFundProduction = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\back\\num4")));
	m_pStcCurrentFundProduction = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\back\\num5")));
	m_pStcDoorDefence = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\back\\num6")));
	m_pStcGodPrivity = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\back\\num7")));
	m_pStcProductivity = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\back\\num8")));
	m_pStcBaseMaterialProduction = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\back\\num9")));
	m_pStcCurrentMaterialProduction = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\back\\num10")));

	m_pIstItems[0] = static_cast<IconStaticEx*>(m_pWnd->GetChild(_T("city_pic\\back\\imgback1\\img")));
	m_pIstItems[1] = static_cast<IconStaticEx*>(m_pWnd->GetChild(_T("city_pic\\back\\imgback2\\img")));
	m_pIstItems[2] = static_cast<IconStaticEx*>(m_pWnd->GetChild(_T("city_pic\\back\\imgback3\\img")));
	m_pIstItems[3] = static_cast<IconStaticEx*>(m_pWnd->GetChild(_T("city_pic\\back\\imgback4\\img")));
	m_pIstItems[4] = static_cast<IconStaticEx*>(m_pWnd->GetChild(_T("city_pic\\back\\imgback5\\img")));
	
	m_pStcProText[0] = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\num0")));
	m_pStcProText[1] = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\num1")));
	m_pStcProText[2] = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\num2")));
	m_pStcProText[3] = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\num3")));
	m_pStcProText[4] = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("city_pic\\back\\num4")));

	m_pIstItems[0]->SetInvisible(TRUE);
	m_pIstItems[1]->SetInvisible(TRUE);
	m_pIstItems[2]->SetInvisible(TRUE);
	m_pIstItems[3]->SetInvisible(TRUE);
	m_pIstItems[4]->SetInvisible(TRUE);

	m_pStcProText[0]->SetInvisible(TRUE);
	m_pStcProText[1]->SetInvisible(TRUE);
	m_pStcProText[2]->SetInvisible(TRUE);
	m_pStcProText[3]->SetInvisible(TRUE);
	m_pStcProText[4]->SetInvisible(TRUE);

	Show(FALSE);

	return TRUE;
}




DWORD CityFrame::EventHandler(tagGUIEvent *pEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		if (pWnd == m_pBtnClose)
		{
			m_pFrameMgr->AddToDestroyList(this);
		} break;
	case EGUIE_MoveIntoCtrl:
		{
			for (DWORD i = 0; i < m_dwCount; ++i)
			{
				if (pWnd == m_pIstItems[i])
				{
					tstring strRealTip;
					tstring strTip = ToolTipCreator::Inst()->GetItemTips(m_Items[i]);
					tstring strTipNail = strTip.substr(strTip.length() - 6, 6);
					if (strTipNail == tstring(g_StrTable[_T("Trick_Tip")]))
						strRealTip = strTip.substr(0, strTip.length() - 6);
					else
						strRealTip = strTip;
					ShowTip(pWnd, strRealTip.c_str());
				}
			}
		} break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		} break;
	default: break;
	}

	return 0;
}

DWORD CityFrame::OnEventRefreshCityAtt( tagCityAttEvent *pEvent )
{
	tagCityAtt2Client *pCityAtt = pEvent->pCityAtt;
	// 取得城市名称
	tstring CityName = CityMgr::Inst()->GetCityName(pCityAtt->dwCityID);
	LoadDefaultProductIcon(pCityAtt->dwCityID);
	m_pStcCityName->SetText(CityName.c_str());
	
	m_pIstCityIcon->SetPic(CityMgr::Inst()->GetCityIcon(pCityAtt->dwCityID));
	
	tstring strGuildName = GuildMgr::Inst()->GetGuildName(pCityAtt->dwGuildID);
	m_pStcGuildName->SetText(strGuildName.c_str());
	
	m_pIstGuildIcon->SetPic(_T("data\\ui\\guild\\guild_icon.bmp"));

	TCHAR szBuffer[256];
	tstring strDoorType = _T("City_DoorType%d");
	_sntprintf(szBuffer, 256, strDoorType.c_str(), static_cast<int>(pCityAtt->eCityGateType));
	m_pStcDoorType->SetText(g_StrTable[szBuffer]);
	_sntprintf(szBuffer, 256, _T("%d"), pCityAtt->nDefence);
	m_pStcDoorDefence->SetText(szBuffer);

	// 守护神名称
	m_pStcPatronGod->SetText(CityMgr::Inst()->GetCityGod(pCityAtt->dwCityID).c_str());
 
	_sntprintf(szBuffer, 256, _T("%d"), pCityAtt->nEudemonTally);
	m_pStcGodPrivity->SetText(szBuffer);
	_sntprintf(szBuffer, 256, _T("%d"), pCityAtt->nTaxRate);
	m_pStcTaxRate->SetText(szBuffer);
	_sntprintf(szBuffer, 256, _T("%d"), pCityAtt->nProlificacy);
	m_pStcProductivity->SetText(szBuffer);
	_sntprintf(szBuffer, 256, _T("%d"), pCityAtt->nFundOutputBase);
	m_pStcBaseFundProduction->SetText(szBuffer);
	_sntprintf(szBuffer, 256, _T("%d"), pCityAtt->nMaterialOutputBase);
	m_pStcBaseMaterialProduction->SetText(szBuffer);
	_sntprintf(szBuffer, 256, _T("%d"), pCityAtt->nFundOutputCur);
	m_pStcCurrentFundProduction->SetText(szBuffer);
	_sntprintf(szBuffer, 256, _T("%d"), pCityAtt->nMaterialOutputCur);
	m_pStcCurrentMaterialProduction->SetText(szBuffer);


	INT nItem = pCityAtt->nItemOutputType;

	for (INT i = 0; i < nItem; ++i)
	{
		for (DWORD n = 0; n < m_dwCount; ++n)
		{
			if (m_Items[n] == pCityAtt->sItemData[i].dwTypeID)
			{
				INT32 nLimit = CityMgr::Inst()->GetProductMinLimit(pCityAtt->dwCityID, pCityAtt->sItemData[i].dwTypeID);
				if (pEvent->pCityAtt->nProlificacy >= nLimit)
				{
					m_pIstItems[n]->SetPicColor(0xFFFFFFFF);
					_sntprintf(szBuffer, 256, _T("%d"), pCityAtt->sItemData[i].nItemNum);
					m_pIstItems[n]->DrawText(szBuffer);
				}
			}
		}		
	}
	return 0;
}

VOID CityFrame::LoadDefaultProductIcon(DWORD dwCityID)
{
	std::vector<tagCityProduct> out;
	CityMgr::Inst()->GetCityDefaultProduct(dwCityID, out);
	int n = 0;
	m_dwCount = out.size();
	for (size_t i = 0; i < out.size(); ++i, ++n)
	{
		if (n >= 5)
			return;
		tstring strIconPath = _T("data\\ui\\Icon\\");
		DWORD dwTypeID = out[i].dwTypeID;
		m_Items[i] = out[i].dwTypeID;
		if (!P_VALID(dwTypeID))
			continue;
		const tagItemDisplayInfo* pDisplay = ItemProtoData::Inst()->FindItemDisplay(dwTypeID);
		if (P_VALID(pDisplay))
		{
			strIconPath += pDisplay->szMaleIcon;
			strIconPath += _T(".tga");
			m_pIstItems[i]->SetInvisible(FALSE);
			m_pIstItems[i]->SetPic(strIconPath);
			m_pIstItems[i]->SetPicColor(0xFF5B5B5B);

			INT32 nLimit = CityMgr::Inst()->GetProductMinLimit(dwCityID, out[i].dwTypeID);
			TCHAR szBuffer[X_SHORT_NAME];
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nLimit);
			m_pStcProText[i]->SetInvisible(FALSE);
			m_pStcProText[i]->SetText(szBuffer);
		}
	}
}


VOID CityFrame::Show( BOOL bShow )
{
	m_pWnd->SetInvisible(!bShow);
}

BOOL CityFrame::EscCancel()
{
	if (m_pWnd->IsInvisible())
		return FALSE;

	m_pFrameMgr->AddToDestroyList(this);

	return TRUE;
}

