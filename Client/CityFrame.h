#pragma once

#include "IconStaticEx.h"

struct tagCityAttEvent;

// 查看城市信息
class CityFrame : public GameFrame
{
public:
	CityFrame();
	~CityFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID Show(BOOL bShow);

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	DWORD OnEventRefreshCityAtt(tagCityAttEvent *pEvent);
	VOID LoadDefaultProductIcon(DWORD dwCityID);
private:
	TSFPTrunk<CityFrame> m_Trunk;
	TObjRef<NetSession> m_pSession;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pFrameMgr;

	GUIWnd *m_pWnd;
	GUIButton *m_pBtnClose;					// 关闭按钮
	IconStatic *m_pIstCityIcon;				// 主城图标
	GUIStatic *m_pStcCityName;				// 主城名称
	IconStatic *m_pIstGuildIcon;			// 帮派图标
	GUIStatic *m_pStcGuildName;				// 帮派名称
	
	GUIStatic *m_pStcDoorType;				// 城门类型
	GUIStatic *m_pStcPatronGod;				// 守护神
	GUIStatic *m_pStcTaxRate;				// 城市税率
	GUIStatic *m_pStcBaseFundProduction;	// 基础资金产出量
	GUIStatic *m_pStcCurrentFundProduction;	// 当前资金产出量
	GUIStatic *m_pStcDoorDefence;			// 城门防卫值
	GUIStatic *m_pStcGodPrivity;			// 守护神契合度
	GUIStatic *m_pStcProductivity;			// 生产力
	GUIStatic *m_pStcBaseMaterialProduction;// 基础资材产出量
	GUIStatic *m_pStcCurrentMaterialProduction;//当前资材产出量
	
	IconStaticEx *m_pIstItems[5];			// 城市物品产出
	GUIStatic  *m_pStcProText[5];			// 最低生产力要求
	DWORD m_Items[5];						// 物品ID
	DWORD m_dwCount;
	DWORD m_dwTime;
	
};