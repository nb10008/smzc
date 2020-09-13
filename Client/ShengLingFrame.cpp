#include "StdAfx.h"
#include "ShengLingFrame.h"
#include "CharacterFrame.h"
#include "Item.h"
#include "ItemMgr.h"
#include "ItemProtoData.h"
#include "ItemEvent.h"
#include "VirtualRole.h"
#include "RoleMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "..\WorldDefine\msg_item.h"
#include "LocalPlayer.h"
#include "..\WorldDefine\ItemDefine.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_compose.h"
#include "GUIEffectMgr.h"

ShengLingFrame::ShengLingFrame(void) : m_Trunk(this),
m_pWnd(NULL),
m_n64FairyID(GT_INVALID),
m_bIsSendMsg(FALSE),
m_byLastSelectIndex(GT_INVALID)
{
}

ShengLingFrame::~ShengLingFrame(void)
{
	
}

BOOL ShengLingFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	/*! 注册游戏事件 */
	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_UpdateItem"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnItemEvent_UpdateItem));
	m_pFrameMgr->RegisterEventHandle(_T("ShengLingEquipEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnShengLingEquipEvent));
	m_pFrameMgr->RegisterEventHandle(_T("ShengLingUnEquipEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnShengLingUnEquipEvent));
	m_pFrameMgr->RegisterEventHandle(_T("ChaiEquipConfirmEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnChaiEquipConfirmEvent));
	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_EquipChange"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnItemEvent_EquipChange));
	m_pFrameMgr->RegisterEventHandle(_T("UpdateHolyAtt"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnUpdateHolyAtt));
	m_pFrameMgr->RegisterEventHandle(_T("ShengWenKeYinConfirmEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnShengWenKeYinConfirmEvent));

	m_pCmdMgr->Register("NS_SLEatFairy",			(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_SLEatFairy),				_T("NS_SLEatFairy"));
	m_pCmdMgr->Register("NS_RoleAttChangeSingle",	(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_RoleAttChangeSingle),	_T("NS_RoleAttChangeSingle"));
	m_pCmdMgr->Register("NS_EquipHoly",				(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_EquipHoly),				_T("NS_EquipHoly"));
	m_pCmdMgr->Register("NS_UnEquipHoly",			(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_UnEquipHoly),			_T("NS_UnEquipHoly"));
	m_pCmdMgr->Register("NS_ChongNeng",				(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_ChongNeng),				_T("NS_ChongNeng"));
	m_pCmdMgr->Register("NS_LingNeng",				(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_LingNeng),				_T("NS_LingNeng"));
	m_pCmdMgr->Register("NS_HolyEquipChg",			(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_HolyEquipChg),			_T("NS_HolyEquipChg"));
	m_pCmdMgr->Register("NS_HolyStrength",			(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_HolyStrength),			_T("NS_HolyStrength"));
	m_pCmdMgr->Register("NS_HolyEquipmentNumber",			(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_HolyEquipmentNumber),			_T("NS_HolyEquipmentNumber"));

	return bRet;
}

BOOL ShengLingFrame::Destroy()
{
	//SAFE_DELETEA(m_pReputeGUICollect);

	BOOL bRet = GameFrame::Destroy();
	if(P_VALID(m_pWndTunShi))
		m_pGUI->AddToDestroyList(m_pWndTunShi);
	if(P_VALID(m_pWndCall))
		m_pGUI->AddToDestroyList(m_pWndCall);
	if(P_VALID(m_pWndKeYin))
		m_pGUI->AddToDestroyList(m_pWndKeYin);
	if(P_VALID(m_pWndChongNeng))
		m_pGUI->AddToDestroyList(m_pWndChongNeng);

	/*! 注销游戏事件 */
	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_UpdateItem"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnItemEvent_UpdateItem));
	m_pFrameMgr->UnRegisterEventHandler(_T("ShengLingEquipEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnShengLingEquipEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("ShengLingUnEquipEvent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnShengLingUnEquipEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("ChaiEquipConfirmEvent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnChaiEquipConfirmEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_EquipChange"),(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnItemEvent_EquipChange));
	m_pFrameMgr->UnRegisterEventHandler(_T("UpdateHolyAtt"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnUpdateHolyAtt));
	m_pFrameMgr->UnRegisterEventHandler(_T("ShengWenKeYinConfirmEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShengLingFrame::OnShengWenKeYinConfirmEvent));

	m_pCmdMgr->UnRegister("NS_SLEatFairy",				(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_SLEatFairy));
	m_pCmdMgr->UnRegister("NS_RoleAttChangeSingle",		(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_RoleAttChangeSingle));
	m_pCmdMgr->UnRegister("NS_EquipHoly",				(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_EquipHoly));
	m_pCmdMgr->UnRegister("NS_UnEquipHoly",				(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_UnEquipHoly));
	m_pCmdMgr->UnRegister("NS_ChongNeng",				(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_ChongNeng));
	m_pCmdMgr->UnRegister("NS_LingNeng",				(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_LingNeng));
	m_pCmdMgr->UnRegister("NS_HolyEquipChg",			(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_HolyEquipChg));
	m_pCmdMgr->UnRegister("NS_HolyStrength",			(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_HolyStrength));
	m_pCmdMgr->UnRegister("NS_HolyEquipmentNumber",			(NETMSGPROC)m_Trunk.sfp2(&ShengLingFrame::OnNS_HolyEquipmentNumber));

	return bRet;
}

BOOL ShengLingFrame::ReloadUI()
{
	TCHAR szText[128] = {0};
	// 创建
	m_pWndFather = m_pGUI->GetWnd(((CharacterFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("sl_back"));
	m_pIStcItemPic		= (IconStatic*)m_pWnd->GetChild(_T("basic\\pic_back\\pic"));		//圣灵图标
	m_pIStcItemPic->SetPic(DefaultIcon);
	m_pIStcItemPic->SetRefresh(TRUE);
	m_pStcItemName		= (GUIStatic*)m_pWnd->GetChild(_T("basic\\name"));		//圣灵名字
	m_pProTunShi		= (ActiveProgress*)m_pWnd->GetChild(_T("basic\\aptitudeprogress\\progressbar"));		//吞噬进度条
	m_pProLingNeng		= (ActiveProgress*)m_pWnd->GetChild(_T("basic\\cultivationprogress\\progressbar"));		//灵能进度条
	m_pProMoQi			= (ActiveProgress*)m_pWnd->GetChild(_T("basic\\moodprogress\\progressbar"));			//默契进度条
	m_pBtnTunShi		= (GUIButton*)m_pWnd->GetChild(_T("basic\\butt1"));		//吞噬按钮
	m_pBtnFusion		= (GUIButton*)m_pWnd->GetChild(_T("basic\\butt2"));		//融合按钮(此功能貌似先不做)
	m_pStcShengLingDmg	= (GUIStatic*)m_pWnd->GetChild(_T("basic\\num0"));	//圣灵伤害
	m_pStcShengLingDef	= (GUIStatic*)m_pWnd->GetChild(_T("basic\\num1"));	//圣灵防御
	m_pStcCrit			= (GUIStatic*)m_pWnd->GetChild(_T("basic\\num2"));			//致命
	m_pStcCritValue		= (GUIStatic*)m_pWnd->GetChild(_T("basic\\num3"));	//致命量
	m_pStcDmgAdd		= (GUIStatic*)m_pWnd->GetChild(_T("basic\\num4"));		//伤害加深
	m_pStcJingZhun		= (GUIStatic*)m_pWnd->GetChild(_T("basic\\num5"));		//精准
	m_pStcLingQiao		= (GUIStatic*)m_pWnd->GetChild(_T("basic\\num6"));		//灵巧
	m_pStcTunShi		= (GUIStatic*)m_pWnd->GetChild(_T("basic\\aptitudeprogress\\number"));	//吞噬数
	m_pStcLingNeng		= (GUIStatic*)m_pWnd->GetChild(_T("basic\\cultivationprogress\\number"));	//灵能数
	m_pStcMoQi			= (GUIStatic*)m_pWnd->GetChild(_T("basic\\moodprogress\\progressbar"));	//默契
	m_pBtnKeYin			= (GUIButton*)m_pWnd->GetChild(_T("picback\\pic\\butt"));
	m_pStcShengLingName = (GUIStatic*)m_pWnd->GetChild(_T("basic\\name"));
	for(int i = 0; i < 6; i++)
	{
		_stprintf(szText, _T("picback\\pic\\pic%d"),i);
		m_pIStcEquip[i] = (IconStatic*)m_pWnd->GetChild(szText);
		m_pIStcEquip[i]->SetPic(DefaultIcon);
		m_pIStcEquip[i]->SetRefresh(TRUE);
		_stprintf(szText, _T("picback\\pic\\butt%d"), i);
		m_pBtnChong[i] = (GUIButton*)m_pWnd->GetChild(szText);
		_stprintf(szText, _T("picback\\pic\\butt%d"), i+6);
		m_pBtnChai[i] = (GUIButton*)m_pWnd->GetChild(szText);
	}

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&ShengLingFrame::EventHandler));


	//吞噬界面
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\ts.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	m_pGUI->ChangeXml(&ele, "ts\\back\\back1\\pic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "ts\\back\\aptitudeprogress\\progressbar", "ClassName", "ActiveProgress");
	m_pGUI->ChangeXml(&ele, "ts\\back\\back2\\pic", "ClassName", "ComposeStatic");
	m_pWndTunShi = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pWndTunShi->SetInvisible(TRUE);
	m_pProgress				= (ActiveProgress*)m_pWndTunShi->GetChild(_T("back\\aptitudeprogress\\progressbar"));			//吞噬进度条
	m_pIStcShengLingIcon	= (IconStatic*)m_pWndTunShi->GetChild(_T("back\\back1\\pic"));									//圣灵图标
	m_pIStcShengLingIcon->SetPic(DefaultIcon);
	m_pIStcShengLingIcon->SetRefresh(TRUE);
	m_pCStcFairy			= (ComposeStatic*)m_pWndTunShi->GetChild(_T("back\\back2\\pic"));								//妖精图标
	m_pCStcFairy->SetPic(DefaultIcon);
	m_pCStcFairy->SetRefresh(TRUE);
	m_pBtnEat				= (GUIButton*)m_pWndTunShi->GetChild(_T("back\\butt"));											//吞噬按钮
	m_pBtnTunShiCancel		= (GUIButton*)m_pWndTunShi->GetChild(_T("back\\butt1"));										//取消
	m_pBtnTunShiClose		= (GUIButton*)m_pWndTunShi->GetChild(_T("back\\close"));										//关闭按钮
	m_pStcEatNum			= (GUIStatic*)m_pWndTunShi->GetChild(_T("back\\word3"));										//吞噬次数
	m_pStcTunShiResult		= (GUIStatic*)m_pWndTunShi->GetChild(_T("back\\word5"));										//吞噬结果（良好，一般啥的）
	m_pStcDmgNum			= (GUIStatic*)m_pWndTunShi->GetChild(_T("back\\back0\\num1"));									//圣灵伤害数值
	m_pStcDefNum			= (GUIStatic*)m_pWndTunShi->GetChild(_T("back\\back0\\num2"));									//圣灵防御数值
	m_pStcCirtNum			= (GUIStatic*)m_pWndTunShi->GetChild(_T("back\\back0\\num3"));									//致命数值
	m_pStcCritValueNum		= (GUIStatic*)m_pWndTunShi->GetChild(_T("back\\back0\\num4"));									//致命量数值
	m_pStcAttackTecNum		= (GUIStatic*)m_pWndTunShi->GetChild(_T("back\\back0\\num5"));									//精准数值
	m_pStcNeglectNum		= (GUIStatic*)m_pWndTunShi->GetChild(_T("back\\back0\\num6"));									//灵巧数值
	m_pStcDmgAddNum			= (GUIStatic*)m_pWndTunShi->GetChild(_T("back\\back0\\num7"));									//伤害加深
	m_pStcLingNengNum		= (GUIStatic*)m_pWndTunShi->GetChild(_T("back\\back0\\num8"));									//灵能
	m_pGUI->RegisterEventHandler(m_pWndTunShi->GetFullName().c_str(), m_Trunk.sfp1(&ShengLingFrame::EventHandler));

	//召唤
	XmlElement ele1;
	tstring strPath1 = g_strLocalPath + _T("\\ui\\zh.xml");
	m_pGUI->LoadXml(&ele1, "VFS_System", strPath1.c_str());
	m_pGUI->ChangeXml(&ele1, "zh\\k\\tu\\sl", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "zh\\k\\progpic\\progress", "ClassName", "ActiveProgress");

	m_pWndCall = m_pGUI->CreateWnd(_T("\\desktop"), &ele1);
	m_pIStcCall = (IconStatic*)m_pWndCall->GetChild(_T("k\\tu\\sl"));
	m_pProShengLingValue = (ActiveProgress*)m_pWndCall->GetChild(_T("k\\progpic\\progress"));
	m_pProShengLingValue->SetMaxValue(MAX_ROLE_HOLY_VALUE);
	m_pStcShengLingValue = (GUIStatic*)m_pWndCall->GetChild(_T("k\\progpic\\number"));
	m_pWndCall->SetInvisible(TRUE);
	
	m_pGUI->RegisterEventHandler(m_pWndCall->GetFullName().c_str(), m_Trunk.sfp1(&ShengLingFrame::EventHandler));

	//刻印
	XmlElement ele2;
	tstring strPath2 = g_strLocalPath + _T("\\ui\\sl_k.xml");
	m_pGUI->LoadXml(&ele2, "VFS_System", strPath2.c_str());
	m_pGUI->ChangeXml(&ele2, "sl_k\\back\\back1\\pic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele2, "sl_k\\back\\progress1\\progressbar", "ClassName", "ActiveProgress");
	m_pGUI->ChangeXml(&ele2, "sl_k\\back\\progress2\\progressbar", "ClassName", "ActiveProgress");
	m_pGUI->ChangeXml(&ele2, "sl_k\\back\\back\\back1\\pic", "ClassName", "ComposeStatic");
	char cTemp[128] = {0};
	for(int i = 0; i < MaxHolyEquipNum; i++)
	{
		_snprintf(cTemp, sizeof(cTemp)/sizeof(CHAR), "sl_k\\back\\back0\\back%d\\pic", i+1);
		m_pGUI->ChangeXml(&ele2, cTemp, "ClassName", "IconStatic");
	}

	m_pWndKeYin = m_pGUI->CreateWnd(_T("\\desktop"), &ele2);
	m_pIStcKShengLingIcon	= (IconStatic*)m_pWndKeYin->GetChild(_T("back\\back1\\pic"));							//圣灵图标
	m_pIStcKShengLingIcon->SetPic(DefaultIcon);
	m_pProKTunShi			= (ActiveProgress*)m_pWndKeYin->GetChild(_T("back\\progress1\\progressbar"));			//吞噬进度条
	m_pStcKTunShi			= (GUIStatic*)m_pWndKeYin->GetChild(_T("back\\progress1\\number"));			
	m_pProKLingNeng			= (ActiveProgress*)m_pWndKeYin->GetChild(_T("back\\progress2\\progressbar"));			//灵能进度条
	m_pStcKLingNeng			= (GUIStatic*)m_pWndKeYin->GetChild(_T("back\\progress2\\number"));
	m_pCStcNewEquipIcon		= (ComposeStatic*)m_pWndKeYin->GetChild(_T("back\\back\\back1\\pic"));					//待放装备图片
	m_pCStcNewEquipIcon->SetPic(DefaultIcon);
	m_pStcShengLingLevel	= (GUIStatic*)m_pWndKeYin->GetChild(_T("back\\back\\num0"));							//要求圣灵吃妖精数
	m_pStcLingNengCost		= (GUIStatic*)m_pWndKeYin->GetChild(_T("back\\back\\num8"));							//灵能消耗
	m_pBtnEquipClose		= (GUIButton*)m_pWndKeYin->GetChild(_T("back\\close"));									//关闭界面
	m_pBtnBeginKeYin		= (GUIButton*)m_pWndKeYin->GetChild(_T("back\\butt"));									//刻印
	m_pBtnCancelKeYin		= (GUIButton*)m_pWndKeYin->GetChild(_T("back\\butt1"));									//取消刻印
	m_pStcShengWenName		= (GUIStatic*)m_pWndKeYin->GetChild(_T("back\\word0"));
	for(int i = 0; i< MaxHolyEquipNum; i++)
	{
		_stprintf(szText, _T("back\\back0\\back%d\\pic"), i+1);
		m_pIStcOldEquipIcon[i] = (IconStatic*)m_pWndKeYin->GetChild(szText);
		m_pIStcOldEquipIcon[i]->SetPic(DefaultIcon);
		_stprintf(szText, _T("back\\back0\\word%d"), i+1);
		m_pStcEquipName[i] = (GUIStatic*)m_pWndKeYin->GetChild(szText);
	}
	for(int i = 0; i < X_ESAT_ATTA_NUM; i++)
	{
		_stprintf(szText, _T("back\\back\\word%d"), i+1);
		m_pSteEquipAttName[i] = (GUIStatic*)m_pWndKeYin->GetChild(szText);
		_stprintf(szText, _T("back\\back\\num%d"), i+1);
		m_pStcEquipAttValue[i] = (GUIStatic*)m_pWndKeYin->GetChild(szText);
	}
	m_pWndKeYin->SetInvisible(TRUE);
	m_pGUI->RegisterEventHandler(m_pWndKeYin->GetFullName().c_str(), m_Trunk.sfp1(&ShengLingFrame::EventHandler));

	//充能
	XmlElement ele3;
	tstring strPath3 = g_strLocalPath + _T("\\ui\\sl_s.xml");
	m_pGUI->LoadXml(&ele3, "VFS_System", strPath3.c_str());
	m_pGUI->ChangeXml(&ele3, "sl_s\\back\\back0\\pic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele3, "sl_s\\back\\back2\\back1\\pic", "ClassName", "ComposeStatic");
	m_pGUI->ChangeXml(&ele3, "sl_s\\back\\back1\\back1\\pic", "ClassName", "ComposeStatic");
	m_pGUI->ChangeXml(&ele3, "sl_s\\back\\back1\\back2\\pic", "ClassName", "ComposeStatic");
	m_pGUI->ChangeXml(&ele3, "sl_s\\back\\progpic\\progress", "ClassName", "ActiveProgress");
	
	m_pWndChongNeng			= m_pGUI->CreateWnd(_T("\\desktop"), &ele3);
	m_pBtnCClose			= (GUIButton*)m_pWndChongNeng->GetChild(_T("back\\close"));			//充能关闭按钮
	m_pBtnBeginChongNeng	= (GUIButton*)m_pWndChongNeng->GetChild(_T("back\\butt"));	//冲能按钮
	m_pBtnChongNengCancel	= (GUIButton*)m_pWndChongNeng->GetChild(_T("back\\butt1"));	//取消充能
	m_pStcRemainTimes		= (GUIStatic*)m_pWndChongNeng->GetChild(_T("back\\word2\\word4"));		//充能剩余次数
	m_pPBtnBaseAtt			= (GUIPushButton*)m_pWndChongNeng->GetChild(_T("back\\com_fra\\optbutt0"));			//基础属性充能
	m_pPBtnSpecAtt			= (GUIPushButton*)m_pWndChongNeng->GetChild(_T("back\\com_fra\\optbutt1"));			//灵能属性充能
	m_pPchBastAtt			= (GUIPatch*)m_pWndChongNeng->GetChild(_T("back\\back1"));
	m_pPchSpecAtt			= (GUIPatch*)m_pWndChongNeng->GetChild(_T("back\\back2"));
	m_pIStcCEquip			= (IconStatic*)m_pWndChongNeng->GetChild(_T("back\\back0\\pic"));			//充能装备图片
	m_pCStcCIMIcon			= (ComposeStatic*)m_pWndChongNeng->GetChild(_T("back\\back1\\back1\\pic"));			//充能道具
	m_pCStcCDisIMIcon		= (ComposeStatic*)m_pWndChongNeng->GetChild(_T("back\\back1\\back2\\pic"));		//偏向道具
	m_pStcAttAddValue		= (GUIStatic*)m_pWndChongNeng->GetChild(_T("back\\back1\\word4"));
	m_pCStcCLingIMIcon		= (ComposeStatic*)m_pWndChongNeng->GetChild(_T("back\\back2\\back1\\pic"));		//灵能道具
	m_pStcCurLingNeng		= (GUIStatic*)m_pWndChongNeng->GetChild(_T("back\\back2\\word5"));
	m_pStcLingCost			= (GUIStatic*)m_pWndChongNeng->GetChild(_T("back\\back2\\word2"));
	m_pComBoxSelectAtt		= (GUIComboBox*)m_pWndChongNeng->GetChild(_T("back\\back1\\constellation"));
	m_pProChongNengTime		= (ActiveProgress*)m_pWndChongNeng->GetChild(_T("back\\progpic\\progress"));
	m_pComBoxSelectAtt->GetEditBox()->SetUnClickable(TRUE);
	m_pWndChongNeng->SetInvisible(TRUE);
	
	m_pGUI->RegisterEventHandler(m_pWndChongNeng->GetFullName().c_str(), m_Trunk.sfp1(&ShengLingFrame::EventHandler));

	//充能成功
	XmlElement ele4;
	tstring strPath4 = g_strLocalPath + _T("\\ui\\sl_c.xml");
	m_pGUI->LoadXml(&ele4, "VFS_System", strPath4.c_str());
	m_pGUI->ChangeXml(&ele4, "sl_s\\back\\back0\\pic", "ClassName", "IconStatic");

	m_pWndChongNengSucc			= m_pGUI->CreateWnd(_T("\\desktop"), &ele4);
	m_pStcChongNengAtt1			= (GUIStatic*)m_pWndChongNengSucc->GetChild(_T("sl_c\\back1\\k1\\word"));	//强化属性1
	m_pStcChongNengAtt2			= (GUIStatic*)m_pWndChongNengSucc->GetChild(_T("sl_c\\back1\\k2\\word"));	//强化属性2
	m_pStcChongNengAttValue1	= (GUIStatic*)m_pWndChongNengSucc->GetChild(_T("sl_c\\back1\\k1\\word1"));//强化属性添加值1
	m_pStcChongNengAttValue2	= (GUIStatic*)m_pWndChongNengSucc->GetChild(_T("sl_c\\back1\\k2\\word1"));//强化属性添加值2
	m_pBtnConfirmChongNeng		= (GUIButton*)m_pWndChongNengSucc->GetChild(_T("sl_c\\surebutt"));	//确认充能
	m_pBtnAbortChongNeng		= (GUIButton*)m_pWndChongNengSucc->GetChild(_T("sl_c\\canclebutt"));	//放弃充能
	m_pWndChongNengSucc->SetInvisible(TRUE);

	m_pGUI->RegisterEventHandler(m_pWndChongNengSucc->GetFullName().c_str(), m_Trunk.sfp1(&ShengLingFrame::EventHandler));

	return TRUE;
}

VOID ShengLingFrame::Update()
{
	if (m_pWnd->IsInvisible())
		return;

	if(!m_pWndTunShi->IsInvisible())
		m_pWndTunShi->Align();

	if(!m_pWndKeYin->IsInvisible())
		m_pWndKeYin->Align();

	if(!m_pWndChongNeng->IsInvisible())
		m_pWndChongNeng->Align();

	if(m_bIsSendMsg)
	{
		float deltaTime = Kernel::Inst()->GetDeltaTime();
		float fcur = m_pProChongNengTime->GetCurrentValue();
		float fMax = m_pProChongNengTime->GetMaxValue();
		if(fcur < fMax)
		{
			fcur += deltaTime;
			m_pProChongNengTime->SetValue(fcur, true);
			m_pProChongNengTime->SetRefresh();
		}
		else
		{
			//发送消息
			if(!m_pPchBastAtt->IsInvisible())
			{
				Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64ChongNengItemID);
				if(!P_VALID(pItem))
				{
					m_n64ChongNengItemID = GT_INVALID;
					ChongNengEnable(TRUE);
					RefreshChongNeng();
					m_pProChongNengTime->SetValue(0, TRUE);
					return ;
				}
				pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64PianXiangItemID);
				if(!P_VALID(pItem))
					m_n64PianXiangItemID = GT_INVALID;

				tagNC_ChongNeng cmd;
				cmd.n64ChongNeng = m_n64ChongNengItemID;
				cmd.n64PianXiang = m_n64PianXiangItemID;
				cmd.byIndex = m_byChongNengIndex;
				cmd.byPianXiangIndex = m_byLastSelectIndex;
				m_pSession->Send(&cmd);
				m_bIsSendMsg = FALSE;
			}
			else if(!m_pPchSpecAtt->IsInvisible())
			{
				tagNC_LingNeng cmd;
				cmd.n64ItemId = m_n64LingNengItemID;
				cmd.byIndex = m_byChongNengIndex;
				m_pSession->Send(&cmd);
				m_bIsSendMsg = FALSE;
			}
		}
	}

}

BOOL ShengLingFrame::EscCancel()
{
	if(m_bIsSendMsg)
	{
		m_bIsSendMsg = FALSE;
		m_pProChongNengTime->SetValue(0);
		ChongNengEnable(TRUE);
		return FALSE;
	}
	if(!m_pWndChongNeng->IsInvisible())
		m_pWndChongNeng->SetInvisible(TRUE);

	if(!m_pWndKeYin->IsInvisible())
		m_pWndKeYin->SetInvisible(TRUE);

	if(!m_pWndTunShi->IsInvisible())
		m_pWndTunShi->SetInvisible(TRUE);

	return FALSE;
}

DWORD ShengLingFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		//吞噬相关
		if(pWnd == m_pBtnTunShi)
		{
			m_pWndTunShi->SetInvisible(FALSE);
			m_pWndKeYin->SetInvisible(TRUE);
			m_pWndChongNeng->SetInvisible(TRUE);
			m_n64FairyID = GT_INVALID;
			RefreshTunShi();
		}
		else if(pWnd == m_pBtnTunShiClose || pWnd == m_pBtnTunShiCancel)
		{
			CloseShengLingWnd();
			//---测试代码
//  			DWORD dwLpId = RoleMgr::Inst()->GetLocalPlayerID();
//  			VirtualRole*pVp =  RoleMgr::Inst()->GetVirtualRole(dwLpId);
//  			if ( P_VALID(pVp) )
//  			{
//  				tagVirtualRoleEvent event;
//  				event.ePlayAct = EPAT_CALL;
//  				pVp->OnGameEvent(&event);
//  
//  				tagNC_CallShengLing cmd;
//  				m_pSession->Send(&cmd);
//  			}
		}
		else if(pWnd == m_pBtnEat)
		{
			//发送吃妖精消息
			tagNC_SLEatFairy cmd;
			cmd.n64Fairy = m_n64FairyID;
			m_pSession->Send(&cmd);

			//清理数据
// 			m_n64FairyID = GT_INVALID;
// 			m_pCStcFairy->SetPic(DefaultIcon);
// 			m_pCStcFairy->SetRefresh(TRUE);
		}
		//刻印相关
		else if(pWnd == m_pBtnEquipClose|| pWnd == m_pBtnCancelKeYin)
		{
			m_pWndKeYin->SetInvisible(TRUE); 
		}
		else if(pWnd == m_pBtnKeYin)
		{
			m_pWndKeYin->SetInvisible(FALSE);
			m_pWndTunShi->SetInvisible(TRUE);
			m_pWndChongNeng->SetInvisible(TRUE);
			m_n64KeYinEquipID = GT_INVALID;
			RefreshKeYin();
		}
		else if(pWnd == m_pBtnBeginKeYin)
		{
			tagNC_EquipHoly cmd;
			cmd.n64Equip = m_n64KeYinEquipID;
			m_pSession->Send(&cmd);
		}
		//充能相关
		else if(pWnd == m_pBtnCClose )
		{
			m_pWndChongNeng->SetInvisible(TRUE);
			m_bIsSendMsg = FALSE;
			m_pProChongNengTime->SetValue(0);
			ChongNengEnable(TRUE);
		}
		else if(pWnd == m_pBtnBeginChongNeng)
		{
			Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64ChongNengItemID);
			if(!P_VALID(pItem))
			{
				pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64LingNengItemID);
			}
			
			Item* pPianXiang = ItemMgr::Inst()->GetPocketItemByID(m_n64PianXiangItemID);
			if(P_VALID(pPianXiang))
			{
				if(m_byLastSelectIndex == 255 || m_byLastSelectIndex == GT_INVALID)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_NotSelect")]);
					return 0;
				}
			}
			if(P_VALID(pItem))
			{
				m_bIsSendMsg = TRUE;
				m_pProChongNengTime->SetMaxValue(2);
				m_pProChongNengTime->SetValue(0, TRUE);
				m_pProChongNengTime->SetSpeed(100);
				ChongNengEnable(FALSE);
			}
			else
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotFoundShengLingItem")]);
			}
		}
		else if(pWnd == m_pBtnChongNengCancel)
		{
			m_bIsSendMsg = FALSE;
			m_pProChongNengTime->SetValue(0);
			ChongNengEnable(TRUE);
		}
		else if(pWnd == m_pComBoxSelectAtt->GetButton())
		{
			Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64PianXiangItemID);
			if(!P_VALID(pItem))
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotFoundPianxiangItem")]);
				m_pComBoxSelectAtt->GetListBox()->SetInvisible(TRUE);
			}
		}
		else if(pWnd == m_pBtnConfirmChongNeng)
		{
			tagNC_ChongNengResult cmd;
			cmd.isConfirm = TRUE;
			cmd.isLingNeng = m_bIsLingNengChongNeng;
			m_pSession->Send(&cmd);
			m_pWndChongNengSucc->SetInvisible(TRUE);
			m_pGUI->SetModal(NULL);
		}
		else if(pWnd == m_pBtnAbortChongNeng)
		{
			tagNC_ChongNengResult cmd;
			cmd.isConfirm = FALSE;
			cmd.isLingNeng = m_bIsLingNengChongNeng;
			m_pSession->Send(&cmd);
			m_pWndChongNengSucc->SetInvisible(TRUE);
			m_pGUI->SetModal(NULL);
		}

		for(int i = 0 ; i < MaxHolyEquipNum; i++)
		{
			if(pWnd == m_pBtnChong[i])
			{
				Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Holy);
				if(!P_VALID(pEquip))
					return 0;

				const tagHolySpec* pSpec = pEquip->GetHolySpec();
				if(!P_VALID(pSpec))
					return 0;
				Equipment* pEquipMent = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_HolyManEquip, pSpec->n64EquipSerialIDs[i]);
				if(!P_VALID(pEquipMent))
					return 0;
				m_pTempChongnengEquip = pEquipMent;
				const tagHolyEquipSpec* pEquipSpec = m_pTempChongnengEquip->GetHolyEquipSpec();
				if(!P_VALID(pEquipSpec))
					return 0;

				m_pWndKeYin->SetInvisible(TRUE);
				m_pWndTunShi->SetInvisible(TRUE);
				ClearChongNeng();
				m_pWndChongNeng->SetInvisible(FALSE);
				m_pWndChongNeng->FlipToTop();
				m_pPBtnBaseAtt->SetState(EGUIBS_PushDown);
				RefreshChongNeng();
				//填充下拉框
				m_pComBoxSelectAtt->GetListBox()->Clear();
				m_pComBoxSelectAtt->GetListBox()->SetText(0, 0, g_StrTable[_T("NULLText")]);
				int nIndex = 1;
				m_mapSelectIndex.clear();
				for(int o = 0; o < X_ESAT_ATTA_NUM; o++)
				{
					if(pEquipSpec->nHolyAttEffect[o] != 0)
					{
						TCHAR szText[128] = {0};
						_stprintf(szText, _T("HolyAtt%d"), o);
						m_pComBoxSelectAtt->GetListBox()->SetText(nIndex, 0, g_StrTable[szText]);
						m_mapSelectIndex.insert(make_pair(nIndex, o));
						nIndex++;
					}
				}
				//调整下拉框
				tagPoint ptSize = m_pComBoxSelectAtt->GetListBox()->GetSize();

				ptSize.x = m_pComBoxSelectAtt->GetEditBox()->GetSize().x;
				ptSize.y = m_pComBoxSelectAtt->GetListBox()->GetRowHeight() * nIndex;

				m_pComBoxSelectAtt->GetListBox()->SetResizable(TRUE);
				m_pComBoxSelectAtt->GetListBox()->SetColWidth(0, ptSize.x	);
				m_pComBoxSelectAtt->GetListBox()->Resize(ptSize);
				m_pComBoxSelectAtt->GetListBox()->SetResizable(FALSE);
				m_byChongNengIndex = i;
			}
			else if(pWnd == m_pBtnChai[i])
			{
				//弹个2次确认框
				if( P_VALID(GetObj("ChaiEquipConfirm")) )
					KillObj("ChaiEquipConfirm");
				CreateObj( "ChaiEquipConfirm", "MsgBox" );
				TCHAR szText[128] = {0};
				Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Holy);
				if(!P_VALID(pEquip))
					return 0;

				const tagHolySpec* pSpec = pEquip->GetHolySpec();
				if(!P_VALID(pSpec))
					return 0;
				Equipment* pShengLingEquip = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_HolyManEquip, pSpec->n64EquipSerialIDs[i]);
				if(!P_VALID(pShengLingEquip))
					return 0;	

				m_dwXiYinItemID = IsItemEnough(pShengLingEquip->GetItemQuality());
				if(m_dwXiYinItemID == GT_INVALID)
				{
					return 0;
				}

				const tagItemProto* pItemProto = ItemProtoData::Inst()->FindEquipProto(pShengLingEquip->GetItemTypeID());
				_stprintf(szText, g_StrTable[_T("ChaiEquipConfirm")], GetColorValue[pShengLingEquip->GetItemQuality()], pItemProto->pDisplayInfo->szName);
				TObjRef<MsgBox>("ChaiEquipConfirm")->Init( _T(""), szText, _T("ChaiEquipConfirmEvent"), (MsgBoxFlag)(MBF_OK|MBF_Cancel) );

				m_byXiYinIndex = i;
			}
		}
		break;
	case EOnEventSendDropGUI_Item:
		if(pWnd == m_pCStcFairy)
		{
			if(!m_pWndTunShi->IsInvisible())
			{
				INT16 nPos = INT16(pEvent->dwParam2);
				Equipment* pItem = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
				if(P_VALID(pItem) && pItem->GetEquipPos() == EEP_Face)
				{
					if(pItem->GetItemQuality() == EIQ_Pink)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EatFairyQualityLimit")]);
					}
					else if(pItem->GetFaBaoSpe()->n16Stage < 40)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EatFairyLevelLow")]);
					}
					else
					{
						m_n64FairyID = pItem->GetItemId();
						m_pCStcFairy->SetPic(ItemMgr::Inst()->GetItemIconPath(pItem->GetItemTypeID()));
						m_pCStcFairy->SetRefresh(TRUE);
						RefreshTunShi();
					}
				}
				else if(P_VALID(pItem) &&pItem->GetProto()->eSpecFunc==EISF_ReplaceFabao)//add by xp
				{
					m_n64FairyID = pItem->GetItemId();
					m_pCStcFairy->SetPic(ItemMgr::Inst()->GetItemIconPath(pItem->GetItemTypeID()));
					m_pCStcFairy->SetRefresh(TRUE);
					RefreshTunShi();
				}//add end
				else
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EatNotFairy")]);
				}
			}
		}
		else if(pWnd == m_pCStcNewEquipIcon)
		{
			if(!m_pWndKeYin->IsInvisible())
			{
				INT16 nPos = INT16(pEvent->dwParam2);
				Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
				if(P_VALID(pEquip))
				{
					if(pEquip->GetItemType() == EIT_HolyEquip)
					{
						m_n64KeYinEquipID = pEquip->GetItemId();
						RefreshKeYin();
					}
					else
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotShengWen")]);
					}
				}
			}
		}
		else if(pWnd == m_pCStcCIMIcon)
		{
			if(!m_pPchBastAtt->IsInvisible() && !m_pWndChongNeng->IsInvisible())
			{
				INT16 nPos = INT16(pEvent->dwParam2);
				Item* pItem = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
				const tagHolyEquipEnhance* pEnhanceProto = ItemProtoData::Inst()->FindHolyEquipEnhance(pItem->GetItemTypeID());//强化proto
				if(!P_VALID(pEnhanceProto))
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotChongNengItem")]);
					return 0;
				}
				const tagHolyEquipSpec* pEquipSpec = m_pTempChongnengEquip->GetHolyEquipSpec();//圣纹spec
				if(!P_VALID(pEquipSpec))
					return 0;
				BOOL IsCanChongNeng=FALSE;
				if(P_VALID(pItem))
				{
					int i=0;
					int j=0;
					while(TRUE)//判断能量石所充能的属性是否在圣纹上存在，如果不存在，就不让放进去
					{
						if(i>6||j>6)
						{
							break;
						}
						if(pEquipSpec->nHolyAttEffect[i]!=0&&pEnhanceProto->dwPowMin[j]!=0)
						{
							IsCanChongNeng=TRUE;
							break;

						}
						else
						{
							i++;
							j++;
						}

					}
					if(pItem->GetItemStuffType() == EST_ChongNeng)
					{
						if(IsCanChongNeng)
						{
							m_n64ChongNengItemID = pItem->GetItemId();
							RefreshChongNeng();
						}
						else
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotFoundShengWenAtt")]);
						}
						
					}
					else 
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotChongNengItem")]);
					}
				}
			}
		}
		else if(pWnd == m_pCStcCDisIMIcon)
		{
			if(!m_pPchBastAtt->IsInvisible())
			{
				INT16 nPos = INT16(pEvent->dwParam2);
				Item* pItem = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
				if(P_VALID(pItem))
				{
					if(pItem->GetItemStuffType() == EST_PianXiang)
					{
						m_n64PianXiangItemID = pItem->GetItemId();
						RefreshChongNeng();
					}
					else
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotPianXiangItem")]);
					}
				}
			}
		}
		else if(pWnd == m_pCStcCLingIMIcon)
		{
			if(!m_pPchSpecAtt->IsInvisible())
			{
				INT16 nPos = INT16(pEvent->dwParam2);
				Item* pItem = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
				if(P_VALID(pItem))
				{
					if(pItem->GetItemStuffType() == EST_LingNeng)
					{
						m_n64LingNengItemID = pItem->GetItemId();
						RefreshChongNeng();
					}
					else
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotLingNengItem")]);
					}
				}
			}
		}
		break;
	case EGUISCB_Drop:
		{
			for(int i = 0; i < MaxHolyEquipNum; i++)
			{
				if(pWnd == m_pIStcEquip[i])
				{
					INT16 nPos = INT16(pEvent->dwParam2);
					Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
					if(P_VALID(pEquip))
					{
						m_n64KeYinEquipID = pEquip->GetItemId();
						//二次确认框
						if( P_VALID(GetObj("ShengWenKeYin")) )
							KillObj("ShengWenKeYin");
						CreateObj( "ShengWenKeYin", "MsgBox" );

						TObjRef<MsgBox>("ShengWenKeYin")->Init( _T(""), g_StrTable[_T("KeYinConfirm")], _T("ShengWenKeYinConfirmEvent"), (MsgBoxFlag)(MBF_OK|MBF_Cancel) );

					}
				}
			}
		}
	case EGUIE_MoveIntoCtrl:
		if(pWnd == m_pIStcItemPic || pWnd == m_pIStcShengLingIcon || pWnd == m_pIStcKShengLingIcon)
		{
			Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Holy);
			if( P_VALID(pEquip) )
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
			}
		}
		else if(pWnd == m_pCStcFairy)
		{
			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64FairyID);
			if(P_VALID(pEquip))
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
			}
		}
		else if(pWnd == m_pCStcNewEquipIcon)
		{
			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64KeYinEquipID);
			if(P_VALID(pEquip))
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
			}
		}
		else if(pWnd == m_pIStcCEquip)
		{
			if(P_VALID(m_pTempChongnengEquip))
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pTempChongnengEquip).c_str());
			}
		}
		else if(pWnd == m_pCStcCIMIcon)
		{
			Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64ChongNengItemID);
			if(P_VALID(pItem))
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
			}
		}
		else if(pWnd == m_pCStcCDisIMIcon)
		{
			Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64PianXiangItemID);
			if(P_VALID(pItem))
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
			}
		}
		else if(pWnd == m_pCStcCLingIMIcon)
		{			
			Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64LingNengItemID);
			if(P_VALID(pItem))
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
			}

		}
		else
		{
			Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Holy);
			if(!P_VALID(pEquip))
				return 0;

			const tagHolySpec* pSpec = pEquip->GetHolySpec();
			if(!P_VALID(pSpec))
				return 0;

			for(int i = 0; i < MaxHolyEquipNum; i++)
			{
				if(pWnd == m_pIStcEquip[i] || pWnd == m_pIStcOldEquipIcon[i])
				{
					Equipment* pShengLingEquip = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_HolyManEquip, pSpec->n64EquipSerialIDs[i]);
					if(P_VALID(pShengLingEquip))
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pShengLingEquip).c_str());
				}
			}
		}
		break;
	case EGUIE_Drag:
		if(pWnd == m_pIStcCall)
		{
// 			LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
// 			if( (P_VALID(pLocalPlayer) && HOLY_MIN_VALUE <= pLocalPlayer->GetAttribute(ERA_Holy) || (pLocalPlayer->GetRoleState() & ERS_CALLHOLY)) && !pLocalPlayer->IsInState(ES_Dead) )
// 			{
// 				tagNC_CallShengLing cmd;
// 				m_pSession->Send(&cmd);
// 			}
// 			else if(pLocalPlayer->IsInState(ES_Dead))
// 			{
// 				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CallShengLing_Dead")]);
// 			}
// 			else
// 			{
// 				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CallShengLing_HolyNotEnough")]);
// 			}

			//改为自动召唤了，这里点击后弹出圣灵界面~
			m_pFrameMgr->SendEvent(&tagGameEvent(_T("Open_Holy"), NULL));
		}
		break;
	case EGUIE_PushButtonDown:
		if (pEvent->dwParam1 != 1)//不是按下不处理
			return 0;
		if(pWnd == m_pPBtnBaseAtt)
		{
			m_pPchBastAtt->SetInvisible(FALSE);
			m_pPchSpecAtt->SetInvisible(TRUE);
			m_n64LingNengItemID = GT_INVALID;
			RefreshChongNeng();
		}
		else if(pWnd == m_pPBtnSpecAtt)
		{
			m_pPchBastAtt->SetInvisible(TRUE);
			m_pPchSpecAtt->SetInvisible(FALSE);
			m_n64ChongNengItemID = GT_INVALID;
			m_n64PianXiangItemID = GT_INVALID;
			RefreshChongNeng();
		}
		break;
	case EGUIE_ItemClick:
		if(pWnd == m_pComBoxSelectAtt->GetListBox())
		{
			m_pStcAttAddValue->SetText(_T(""));
			m_pGUI->SetActive(m_pWndChongNeng);
			map<BYTE, BYTE>::iterator it = m_mapSelectIndex.find(pEvent->dwParam1);
			if(it == m_mapSelectIndex.end())
			{
				m_byLastSelectIndex = GT_INVALID;
				return 0;
			}
			m_byLastSelectIndex = (*it).second;
			RefreshChongNeng();
		}
		break;
	}
	return 0;
}

VOID ShengLingFrame::RefreshUI()
{
	for(int i = 0; i < MaxHolyEquipNum; i++)
	{
		m_pIStcEquip[i]->SetPic(DefaultIcon);
		m_pIStcEquip[i]->SetRefresh(TRUE);
	}
	Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Holy);
	if(!P_VALID(pEquip))
		return;

	const tagSoulAttName* pProto = ItemProtoData::Inst()->FindShenLingName(pEquip->GetItemTypeID());
	if(!P_VALID(pProto))
		return;

	const tagHolySpec* pSpec = pEquip->GetHolySpec();
	if(!P_VALID(pProto))
		return;

	const tagItemProto* pShengLignNameProto = ItemProtoData::Inst()->FindItemProto(pEquip->GetItemTypeID());
	if(!P_VALID(pProto))
		return;

	//显示名字
	m_pStcShengLingName->SetText(pShengLignNameProto->pDisplayInfo->szName);

	//设置图标
	TCHAR szText[128] = {0};
	if(pSpec->nDevourNum >= pProto->dwHighEatNum)
		_sntprintf(szText, sizeof(szText), _T("data\\ui\\Icon\\%s.tga"),pProto->szIcon_High);
	else 
		_sntprintf(szText, sizeof(szText), _T("data\\ui\\Icon\\%s.tga"),pProto->szIcon_Low);
	m_pIStcItemPic->SetPic(szText);
	//吞噬次数
	_stprintf(szText, _T("%d/%d"), pSpec->nDevourNum, pSpec->nMaxDevourNum);
	m_pStcTunShi->SetText(szText);
 	m_pProTunShi->SetMaxValue(pSpec->nMaxDevourNum);
 	m_pProTunShi->SetValue(pSpec->nDevourNum, TRUE);
	//默契
	_stprintf(szText, _T("%d/%d"), pSpec->nCoValue, MAX_MOQI);
	m_pStcMoQi->SetText(szText);
	m_pProMoQi->SetMaxValue(MAX_MOQI);
	m_pProMoQi->SetValue(pSpec->nCoValue, TRUE);
	//圣灵伤害
	_stprintf(szText, _T("%d"), pSpec->dwLevelUpAtt[0]);
	m_pStcShengLingDmg->SetText(szText);
	//圣灵防御
	_stprintf(szText, _T("%d"), pSpec->dwLevelUpAtt[1]);
	m_pStcShengLingDef->SetText(szText);
	//致命
	_stprintf(szText, _T("%d"), pSpec->dwLevelUpAtt[2]);
	m_pStcCrit->SetText(szText);	
	//致命量	
	_stprintf(szText, _T("%d"), pSpec->dwLevelUpAtt[3]);
	m_pStcCritValue->SetText(szText);
	//伤害加深
	_stprintf(szText, _T("%d"), pSpec->dwLevelUpAtt[4]);
	m_pStcDmgAdd->SetText(szText);		
	//精准
	_stprintf(szText, _T("%d"), pSpec->dwLevelUpAtt[5]);
	m_pStcJingZhun->SetText(szText);		
	//灵巧
	_stprintf(szText, _T("%d"), pSpec->dwLevelUpAtt[6]);
	m_pStcLingQiao->SetText(szText);	
	//灵能
	_stprintf(szText, _T("%d/%d"), pSpec->dwLevelUpAtt[7] - pSpec->nCostHoly, pSpec->dwLevelUpAtt[7]);
	m_pStcLingNeng->SetText(szText);
	m_pProLingNeng->SetMaxValue(pSpec->dwLevelUpAtt[7]);
	m_pProLingNeng->SetValue(pSpec->dwLevelUpAtt[7] - pSpec->nCostHoly, TRUE);

	for(int i = 0; i < MaxHolyEquipNum; i++)
	{
		if(pSpec->n64EquipSerialIDs[i] != 0)
		{
			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_HolyManEquip, pSpec->n64EquipSerialIDs[i]);
			if(P_VALID(pEquip))
			{
				m_pIStcEquip[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(pEquip->GetItemTypeID()));
				m_pIStcEquip[i]->SetRefresh(TRUE);
			}
		}

		if(i > pSpec->n16EquipmentNumber-1)
		{
			m_pIStcEquip[i]->SetPic(DisablePatch);
			m_pIStcEquip[i]->SetRefresh(TRUE);
		}
	}
}

VOID ShengLingFrame::RefreshTunShi()
{
	//清理数据先
	m_pStcTunShiResult->SetText(_T(""));		//吞噬结果（良好，一般啥的）
	m_pStcDmgNum->SetText(_T(""));				//圣灵伤害数值
	m_pStcDefNum->SetText(_T(""));				//圣灵防御数值
	m_pStcCirtNum->SetText(_T(""));				//致命数值
	m_pStcCritValueNum->SetText(_T(""));		//致命量数值
	m_pStcAttackTecNum->SetText(_T(""));		//精准数值
	m_pStcNeglectNum->SetText(_T(""));			//灵巧数值
	m_pStcDmgAddNum->SetText(_T(""));			//伤害加深
	m_pStcLingNengNum->SetText(_T(""));		

	Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Holy);
	if(!P_VALID(pEquip))
		return;

 	const tagSoulAttName* pProto = ItemProtoData::Inst()->FindShenLingName(pEquip->GetItemTypeID());
 	if(!P_VALID(pProto))
 		return;
 
 	const tagHolySpec* pSpec = pEquip->GetHolySpec();
 	if(!P_VALID(pProto))
 		return;

	//设置图标
	TCHAR szText[128] = {0};
	if(pSpec->nDevourNum >= pProto->dwHighEatNum)
		_sntprintf(szText, sizeof(szText), _T("data\\ui\\Icon\\%s.tga"),pProto->szIcon_High);
	else 
		_sntprintf(szText, sizeof(szText), _T("data\\ui\\Icon\\%s.tga"),pProto->szIcon_Low);
	m_pIStcShengLingIcon->SetPic(szText);
	m_pIStcShengLingIcon->SetRefresh(TRUE);

	_stprintf(szText, _T("%d/%d"), pSpec->nToDayEatNum, MAX_DAY_EAT);
	m_pProgress->SetMaxValue(MAX_DAY_EAT);
	m_pProgress->SetValue(pSpec->nToDayEatNum, TRUE);
	_stprintf(szText, _T("%d"), (pSpec->nMaxDevourNum - pSpec->nDevourNum));
	m_pStcEatNum->SetText(szText);


	Equipment* pFairy = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64FairyID);
	DWORD dwScore = 0;
	if(P_VALID(pFairy)&&pFairy->GetProto()->eSpecFunc==EISF_ReplaceFabao)//如果是妖精魂核
	{
		dwScore=pFairy->GetProto()->nSpecFuncVal1;
	}
	else//如果是妖精
	{
		if(!P_VALID(pFairy))
		{
			m_pCStcFairy->SetPic(NullIconPatch);
			m_pCStcFairy->SetText(_T(""));
			m_n64FairyID = GT_INVALID;
			return;
		}

		const tagFabaoSpec* pFairySpec = pFairy->GetFaBaoSpe();
		if(!P_VALID(pFairySpec))
			return;

		CallEatFairyScore(pFairy->GetItemQuality(), pFairySpec->n16Stage, dwScore);
	}
	
	const tagHolyLevelUpProto* pLevelProto = ItemProtoData::Inst()->GetHolyLevelProto(dwScore);
	if(!P_VALID(pLevelProto))
		return;

	_stprintf(szText, _T("%d-%d"), pLevelProto->dwDmgMin, pLevelProto->dwDmgMax);	//圣灵伤害
	m_pStcDmgNum->SetText(szText);
	_stprintf(szText, _T("%d-%d"), pLevelProto->dwDefMin, pLevelProto->dwDefMax);	//圣灵防御
	m_pStcDefNum->SetText(szText);
	_stprintf(szText, _T("%d-%d"), pLevelProto->dwCritMin, pLevelProto->dwCritMax);	//致命
	m_pStcCirtNum->SetText(szText);
	_stprintf(szText, _T("%d-%d"), pLevelProto->dwCritValueMin, pLevelProto->dwCritValueMax);	//致命量
	m_pStcCritValueNum->SetText(szText);
	_stprintf(szText, _T("%d-%d"), pLevelProto->dwAttackTecMin, pLevelProto->dwAttackTecMax);	//精准
	m_pStcAttackTecNum->SetText(szText);
	_stprintf(szText, _T("%d-%d"), pLevelProto->dwNeglectMin, pLevelProto->dwNeglectMax);	//灵巧
	m_pStcNeglectNum->SetText(szText);
	_stprintf(szText, _T("%d-%d"), pLevelProto->dwDmgAddMin, pLevelProto->dwDmgAddMax);	//伤害加深
	m_pStcDmgAddNum->SetText(szText);
	_stprintf(szText, _T("%d-%d"), pLevelProto->dwLingNengMin, pLevelProto->dwLingNengMax);	//灵能
	m_pStcLingNengNum->SetText(szText);

	//剩余数量
	Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64FairyID);
	DWORD nItemQuantity = pItem->GetItemQuantity();
	_stprintf(szText, _T("%d"), nItemQuantity);
	if(nItemQuantity > 0)
		m_pCStcFairy->SetText(szText);
}

VOID ShengLingFrame::RefreshKeYin()
{
	//清理数据
	for(int i = 0; i < X_ESAT_ATTA_NUM; i++)
	{
		m_pSteEquipAttName[i]->SetInvisible(TRUE);
		m_pStcEquipAttValue[i]->SetInvisible(TRUE);
	}
	for(int i = 0 ; i < MaxHolyEquipNum; i++ )
	{
		m_pIStcOldEquipIcon[i]->SetPic(DefaultIcon);
		m_pStcEquipName[i]->SetText(_T(""));
	}
	m_pStcShengLingLevel->SetText(_T(""));
	m_pStcLingNengCost->SetText(_T(""));
	Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Holy);
	if(!P_VALID(pEquip))
		return;

	const tagSoulAttName* pProto = ItemProtoData::Inst()->FindShenLingName(pEquip->GetItemTypeID());
	if(!P_VALID(pProto))
		return;

	const tagHolySpec* pSpec = pEquip->GetHolySpec();
	if(!P_VALID(pProto))
		return;

	const tagItemProto* pShengLingProto = ItemProtoData::Inst()->FindItemProto(pEquip->GetItemTypeID());
	if(!P_VALID(pShengLingProto))
		return ;

	m_pStcShengWenName->SetText(pShengLingProto->pDisplayInfo->szName);

	//设置图标
	TCHAR szText[128] = {0};
	if(pSpec->nDevourNum >= pProto->dwHighEatNum)
		_sntprintf(szText, sizeof(szText), _T("data\\ui\\Icon\\%s.tga"),pProto->szIcon_High);
	else 
		_sntprintf(szText, sizeof(szText), _T("data\\ui\\Icon\\%s.tga"),pProto->szIcon_Low);
	m_pIStcKShengLingIcon->SetPic(szText);
	m_pProKTunShi->SetMaxValue(pProto->nDevourNum);
	m_pProKTunShi->SetValue(pSpec->nDevourNum, TRUE);
	_stprintf(szText, _T("%d/%d"), pSpec->nDevourNum, pProto->nDevourNum);
	m_pStcKTunShi->SetText(szText);
	_stprintf(szText, _T("%d/%d"), pSpec->dwLevelUpAtt[7] - pSpec->nCostHoly, pSpec->dwLevelUpAtt[7]);
	m_pStcKLingNeng->SetText(szText);
	m_pProKLingNeng->SetMaxValue(pSpec->dwLevelUpAtt[7]);
	m_pProKLingNeng->SetValue(pSpec->dwLevelUpAtt[7] - pSpec->nCostHoly, TRUE);

	Equipment* pKeYinEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64KeYinEquipID);
	if(P_VALID(pKeYinEquip))
	{
		m_pCStcNewEquipIcon->SetPic(ItemMgr::Inst()->GetItemIconPath(pKeYinEquip->GetItemTypeID()));
		m_pCStcNewEquipIcon->SetRefresh(TRUE);

		const tagHolyEquipSpec* pHolyEquipSpec = pKeYinEquip->GetHolyEquipSpec();
		if(P_VALID(pHolyEquipSpec))
		{
			int index = 0;
			for(int i = 0; i < X_ESAT_ATTA_NUM; i++)
			{
				if(pHolyEquipSpec->nHolyAttEffect[i] != 0)
				{
					_stprintf(szText, _T("HolyAtt%d"), i);
					m_pSteEquipAttName[index]->SetText(g_StrTable[szText]);
					m_pSteEquipAttName[index]->SetInvisible(FALSE);
					_stprintf(szText, _T("%d"), pHolyEquipSpec->nHolyAttEffect[i]);
					m_pStcEquipAttValue[index]->SetText(szText);
					m_pStcEquipAttValue[index]->SetInvisible(FALSE);
					index++;
				}
			}
			_stprintf(szText, _T("%d"), pHolyEquipSpec->nCostHoly);
			m_pStcLingNengCost->SetText(szText);
		}
		const tagHolyEquipProto* pEquipProto = ItemProtoData::Inst()->FindHolyEquipProto(pKeYinEquip->GetItemTypeID());
		if(P_VALID(pEquipProto))
		{
			_stprintf(szText, _T("%d"), pEquipProto->nHolyLevel);
			m_pStcShengLingLevel->SetText(szText);
		}
	}
	else
	{
		m_pCStcNewEquipIcon->SetPic(DefaultIcon);
	}

	//已穿戴装备
	for(int i = 0 ; i < MaxHolyEquipNum; i++ )
	{
		if(pSpec->n64EquipSerialIDs[i] != 0)
		{
			Equipment* pShengLingEquip = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_HolyManEquip, pSpec->n64EquipSerialIDs[i]);
			if(P_VALID(pShengLingEquip))
			{
				m_pIStcOldEquipIcon[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(pShengLingEquip->GetItemTypeID()));
				const tagItemProto* pItemProto = ItemProtoData::Inst()->FindEquipProto(pShengLingEquip->GetItemTypeID());
				m_pStcEquipName[i]->SetTextColor(GetColorValue[pShengLingEquip->GetItemQuality()]);
				m_pStcEquipName[i]->SetText(pItemProto->pDisplayInfo->szName);
			}
		}

		if(i > pSpec->n16EquipmentNumber-1)
		{
			m_pIStcOldEquipIcon[i]->SetPic(DisablePatch);
			m_pIStcOldEquipIcon[i]->SetRefresh(TRUE);
		}
	}
}

VOID ShengLingFrame::RefreshChongNeng()
{
	//清理
	m_pCStcCIMIcon->SetPic(DefaultIcon);
	m_pCStcCDisIMIcon->SetPic(DefaultIcon);
	m_pCStcCLingIMIcon->SetPic(DefaultIcon);
	m_pStcLingCost->SetText(_T(""));
	m_pStcCurLingNeng->SetText(_T(""));
	m_pStcAttAddValue->SetText(_T(""));
	m_pStcLingCost->SetText(_T(""));
	m_pCStcCIMIcon->SetText(_T(""));

	if(!P_VALID(m_pTempChongnengEquip))
		return;

	const tagHolyEquipSpec* pSpec = m_pTempChongnengEquip->GetHolyEquipSpec();
	if(!P_VALID(pSpec))
		return;

	TCHAR szText[128] = {0};
	m_pIStcCEquip->SetPic(ItemMgr::Inst()->GetItemIconPath(m_pTempChongnengEquip->GetItemTypeID()));
	const tagHolyEquipProto* pProto = ItemProtoData::Inst()->FindHolyEquipProto(m_pTempChongnengEquip->GetItemTypeID());
	if(P_VALID(pProto))
	{
		_stprintf(szText, _T("%d"), pProto->n16MaxLapNum - pSpec->n16EnhanceCount);
		m_pStcRemainTimes->SetText(szText);
	}

	_stprintf(szText, _T("%d"), pSpec->nCostHoly);
	m_pStcCurLingNeng->SetText(szText);

	if(m_n64ChongNengItemID != GT_INVALID)
	{
		//刷新充能道具图标
		Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64ChongNengItemID);
		if(P_VALID(pItem))
		{
			m_pCStcCIMIcon->SetPic(ItemMgr::Inst()->GetItemIconPath(pItem->GetItemTypeID()));
			DWORD nItemQuantity = pItem->GetItemQuantity();
			_stprintf(szText, _T("%d"), nItemQuantity);
			if(nItemQuantity > 0)
				m_pCStcCIMIcon->SetText(szText);
		}
	}
	if(m_n64PianXiangItemID != GT_INVALID)
	{
		//刷新偏向道具图标
		Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64PianXiangItemID);
		if(P_VALID(pItem))
		{
			m_pCStcCDisIMIcon->SetPic(ItemMgr::Inst()->GetItemIconPath(pItem->GetItemTypeID()));
		}
	}
	if(m_n64LingNengItemID != GT_INVALID)
	{
		//刷新灵能道具图标
		Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64LingNengItemID);
		if(P_VALID(pItem))
		{
			m_pCStcCLingIMIcon->SetPic(ItemMgr::Inst()->GetItemIconPath(pItem->GetItemTypeID()));
		}
	}

	Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64ChongNengItemID);
	if(P_VALID(pItem))
	{
		const tagHolyEquipEnhance* pEnhanceProto = ItemProtoData::Inst()->FindHolyEquipEnhance(pItem->GetItemTypeID());
		if(P_VALID(pEnhanceProto) && m_byLastSelectIndex >= 0 && m_byLastSelectIndex < X_ESAT_ATTA_NUM )
		{
			DWORD dwValue1 = pEnhanceProto->dwPowMin[m_byLastSelectIndex];
			DWORD dwValue2 = pEnhanceProto->dwPowMax[m_byLastSelectIndex];

			_stprintf(szText, _T("%d～%d"), dwValue1, dwValue2);
			m_pStcAttAddValue->SetText(szText);

			if(pEnhanceProto->dwLingNengPow != 0)
			{
				_stprintf(szText, _T("%d"), pEnhanceProto->dwLingNengPow);
				m_pStcLingCost->SetText(szText);
			}
		}

	}
	else
	{
		m_pStcAttAddValue->SetText(_T(""));
		m_pComBoxSelectAtt->GetEditBox()->SetText(g_StrTable[_T("NULLText")]);
		m_byLastSelectIndex = GT_INVALID;
	}

	pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64LingNengItemID);
	if(P_VALID(pItem))
	{
		const tagHolyEquipEnhance* pEnhanceProto = ItemProtoData::Inst()->FindHolyEquipEnhance(pItem->GetItemTypeID());
		if(P_VALID(pEnhanceProto))
		{
			if(pEnhanceProto->dwLingNengPow != 0)
			{
				_stprintf(szText, _T("-%d"), pEnhanceProto->dwLingNengPow);
				m_pStcLingCost->SetText(szText);
			}
		}
	}
}

VOID ShengLingFrame::CloseShengLingWnd()
{
	m_pWndTunShi->SetInvisible(TRUE);
	m_pWndKeYin->SetInvisible(TRUE);
}

DWORD ShengLingFrame::OnItemEvent_UpdateItem(tagUpdateLPItemEvent* pGameEvent)
{
	if(pGameEvent->eType == EICT_HolyManEquip || (pGameEvent->eType == EICT_Equip && pGameEvent->n16Index == EEP_Holy))
	{
		if(!m_pWnd->IsInvisible())
			RefreshUI();
	}
	return 0;
}

DWORD ShengLingFrame::OnShengLingEquipEvent(tagGameEvent* pGameEvent)
{
	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLocalPlayer) && pLocalPlayer->GetRoleState()&ERS_CALLHOLY)
		RoleMgr::Inst()->LoadHolyManModel(FALSE);
	else
		RoleMgr::Inst()->LoadHolyManModel();
	m_pWndCall->SetInvisible(FALSE);
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetCurEquip(EEP_Holy);
	if(!P_VALID(pEquip))
		return 0;

	const tagSoulAttName* pShengLingName = ItemProtoData::Inst()->FindShenLingName(pEquip->GetItemTypeID());
	if(!P_VALID(pShengLingName))
		return 0;

	const tagHolySpec* pShengLingSpec = pEquip->GetHolySpec();
	if(!P_VALID(pShengLingSpec))
		return 0;

	TCHAR szText[128] = {0};
	if(pShengLingSpec->nDevourNum >= pShengLingName->dwHighEatNum)
		_sntprintf(szText, sizeof(szText), _T("data\\ui\\Icon\\%s.tga"),pShengLingName->szIcon_High);
	else 
		_sntprintf(szText, sizeof(szText), _T("data\\ui\\Icon\\%s.tga"),pShengLingName->szIcon_Low);

	m_pIStcCall->SetPic(szText);

	if(!P_VALID(pLocalPlayer))
		return 0;
	m_pProShengLingValue->SetMaxValue(MAX_ROLE_HOLY_VALUE);
	m_pProShengLingValue->SetValue(pLocalPlayer->GetAttribute(ERA_Holy), TRUE);
	_stprintf(szText, _T("%d/%d"), pLocalPlayer->GetAttribute(ERA_Holy), MAX_ROLE_HOLY_VALUE);
	m_pStcShengLingValue->SetText(szText);

	return 0;
}

DWORD ShengLingFrame::OnShengLingUnEquipEvent(tagGameEvent* pGameEvent)
{
	m_pWndCall->SetInvisible(TRUE);
	m_pWndKeYin->SetInvisible(TRUE);

	return 0;
}

DWORD ShengLingFrame::OnItemEvent_EquipChange(tagUpdateLPItemEvent* pGameEvent)
{
	if(pGameEvent->eType != EICT_Equip)
		return 0;

	if(!m_pWnd->IsInvisible())
		RefreshUI();

	return 0;
}

DWORD ShengLingFrame::OnNS_SLEatFairy(tagNS_SLEatFairy* pMsg, LPVOID pParam)
{
	TCHAR szText[128] = {0};
	RefreshTunShi();
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);		
		break;
	case E_SLEatFairy_EquipInValid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SLEatFairy_EquipInValid")]);
		break;
	case E_SLEatFairy_CountNotEnough:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SLEatFairy_CountNotEnough")]);
		break;
	case E_SLEatFairy_CountExceed:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SLEatFairy_CountExceed")]);
		break;
	}
	//判断是否该更换圣灵模型了
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetCurEquip(EEP_Holy);
	if(!P_VALID(pEquip))
		return 0;

	const tagSoulAttName* pShengLingName = ItemProtoData::Inst()->FindShenLingName(pEquip->GetItemTypeID());
	if(!P_VALID(pShengLingName))
		return 0;

	if(pEquip->GetHolySpec()->nDevourNum == pShengLingName->dwHighEatNum)
	{
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if(P_VALID(pLocalPlayer) && pLocalPlayer->GetRoleState()&ERS_CALLHOLY)
		{
			RoleMgr::Inst()->LoadHolyManModel(FALSE);
		}
		else
		{
			RoleMgr::Inst()->LoadHolyManModel();
		}

		if(pEquip->GetHolySpec()->nDevourNum >= pShengLingName->dwHighEatNum)
			_sntprintf(szText, sizeof(szText), _T("data\\ui\\Icon\\%s.tga"),pShengLingName->szIcon_High);
		else 
			_sntprintf(szText, sizeof(szText), _T("data\\ui\\Icon\\%s.tga"),pShengLingName->szIcon_Low);

		m_pIStcCall->SetPic(szText);
	}
	return 0;
}

DWORD ShengLingFrame::OnNS_RoleAttChangeSingle(tagNS_RoleAttChangeSingle* pMsg, LPVOID pParam)
{
	if(pMsg->eType == ERA_Holy)
	{
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if(!P_VALID(pLocalPlayer))
			return 0;
		
		m_pProShengLingValue->SetValue(pLocalPlayer->GetAttribute(ERA_Holy), TRUE);
		TCHAR szText[128] = {0};
		_stprintf(szText, _T("%d/%d"), pLocalPlayer->GetAttribute(ERA_Holy), MAX_ROLE_HOLY_VALUE);
		m_pStcShengLingValue->SetText(szText);

		if(pLocalPlayer->GetAttribute(ERA_Holy) == 100)
		{
			GUIEffectMgr::Inst()->PlayGuiEffect(_T("txui031"), m_pIStcCall, GUIEffectMgr::EET_ShengLingCall);
		}
		
		if(pLocalPlayer->GetAttribute(ERA_Holy) <= 100)
		{
			GUIEffectMgr::Inst()->StopEffectByType(GUIEffectMgr::EET_ShengLingCall);
		}
	}

	return 0;
}

DWORD ShengLingFrame::OnNS_EquipHoly(tagNS_EquipHoly* pMsg, LPVOID pParam)
{
	if(pMsg->dwErrorCode == E_Success)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
		m_n64KeYinEquipID = GT_INVALID;
		RefreshKeYin();
	}
	else
	{
		switch(pMsg->dwErrorCode)
		{
		case E_EquipHoly_EquipInValid:		// 给圣灵穿装备，客户端提供的装备类型不符合
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipHoly_EquipInValid")]);
			break;
		case E_EquipHoly_NotEmpty:			// 给圣灵穿装备，圣灵上装备已满
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipHoly_NotEmpty")]);
			break;
		case E_EquipHoly_HolyLowLevel:		// 给圣灵穿装备，放入的圣纹吞噬等级要求过高
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipHoly_HolyLowLevel")]);
			break;
		case E_EquipHoly_NotEnough:			// 给圣灵穿装备，圣灵剩余灵能不足
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipHoly_NotEnough")]);
			break;
		case E_EquipHoly_ExceedLapCount:		// 给圣灵穿装备，超过了该类型圣纹可在圣灵上重复装备的数量
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipHoly_ExceedLapCount")]);
			break;
		}
		
		
	}

	return 0;
}

DWORD ShengLingFrame::OnChaiEquipConfirmEvent(tagMsgBoxEvent* pMsgBoxEvent)
{
	if(pMsgBoxEvent->eResult == MBF_OK)
	{
		tagNC_UnEquipHoly cmd;
		cmd.byIndex = m_byXiYinIndex;
		cmd.dwTypeID = m_dwXiYinItemID;
		m_pSession->Send(&cmd);
	}

	return 0;
}

DWORD ShengLingFrame::OnShengWenKeYinConfirmEvent(tagMsgBoxEvent* pMsgBoxEvent)
{
	if(pMsgBoxEvent->eResult == MBF_OK)
	{
		tagNC_EquipHoly cmd;
		cmd.n64Equip = m_n64KeYinEquipID;
		m_pSession->Send(&cmd);
	}
	else
	{
		m_n64KeYinEquipID = GT_INVALID;
	}

	return 0;
}

DWORD ShengLingFrame::OnUpdateHolyAtt(tagGameEvent* pEvent)
{
	RefreshUI();

	return 0;
}

DWORD ShengLingFrame::IsItemEnough(BYTE byQuality)
{
	Item* pItem;
	TCHAR szText[128] = {0};
	//判断是否有道具
	if(byQuality == 3)
	{
		//蓝色：3500031（不绑定）、3500032（绑定）
		pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(3500032);
		if(P_VALID(pItem))
			return 3500032;
		pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(3500031);
		if(P_VALID(pItem))
			return 3500031;

		const tagItemDisplayInfo* pProto = ItemProtoData::Inst()->FindItemDisplay(3500031);
		if(P_VALID(pProto))
		{
			_stprintf(szText, g_StrTable[_T("ChaiEquipLimit")], pProto->szName);
			CombatSysUtil::Inst()->ShowScreenCenterMsg(szText);
		}
	}
	else if(byQuality == 4)
	{
		//橙色：3500033（不绑定）、3500034（绑定）
		pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(3500034);
		if(P_VALID(pItem))
			return 3500034;
		pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(3500033);
		if(P_VALID(pItem))
			return 3500033;

		const tagItemDisplayInfo* pProto = ItemProtoData::Inst()->FindItemDisplay(3500033);
		if(P_VALID(pProto))
		{
			_stprintf(szText, g_StrTable[_T("ChaiEquipLimit")], pProto->szName);
			CombatSysUtil::Inst()->ShowScreenCenterMsg(szText);
		}
	}
	else if(byQuality == 5)
	{
		//紫色：3500035（不绑定）、3500036（绑定）
		pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(3500036);
		if(P_VALID(pItem))
			return 3500036;
		pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(3500035);
		if(P_VALID(pItem))
			return 3500035;

		const tagItemDisplayInfo* pProto = ItemProtoData::Inst()->FindItemDisplay(3500035);
		if(P_VALID(pProto))
		{
			_stprintf(szText, g_StrTable[_T("ChaiEquipLimit")], pProto->szName);
			CombatSysUtil::Inst()->ShowScreenCenterMsg(szText);
		}
	}
	else if(byQuality == 6)
	{
		//粉色：3500037（不绑定）、3500038（绑定）
		pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(3500038);
		if(P_VALID(pItem))
			return 3500038;
		pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(3500037);
		if(P_VALID(pItem))
			return 3500037;

		const tagItemDisplayInfo* pProto = ItemProtoData::Inst()->FindItemDisplay(3500037);
		if(P_VALID(pProto))
		{
			_stprintf(szText, g_StrTable[_T("ChaiEquipLimit")], pProto->szName);
			CombatSysUtil::Inst()->ShowScreenCenterMsg(szText);
		}
	}
	return GT_INVALID;
}

DWORD ShengLingFrame::OnNS_UnEquipHoly(tagNS_UnEquipHoly* pMsg, LPVOID pParam)
{
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
		break;
	case E_EquipHoly_UnEquipInValid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipHoly_UnEquipInValid")]);
		break;
	case E_EquipHoly_QualityNotEqual:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipHoly_QualityNotEqual")]);
		break;
	case E_EquipHoly_SilverNotEnough:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipHoly_SilverNotEnough")]);
		break;
	case E_EquipHoly_SpaceNotEnough:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipHoly_SpaceNotEnough")]);
		break;
	}

	if(!m_pWndKeYin->IsInvisible())
		RefreshKeYin();

	return 0;
}

DWORD ShengLingFrame::OnNS_ChongNeng(tagNS_ChongNeng* pMsg, LPVOID pParam)
{
	ChongNengEnable(TRUE);
	RefreshChongNeng();
	m_pProChongNengTime->SetValue(0, TRUE);
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
			m_pWndChongNengSucc->SetInvisible(FALSE);
			TCHAR szText[128] = {0};
			_stprintf(szText, _T("HolyAtt%d"), pMsg->byAtt);
			m_pStcChongNengAtt1->SetText(g_StrTable[szText]);
			_stprintf(szText, _T("%d"), pMsg->dwAttValue);
			m_pStcChongNengAttValue1->SetText(szText);
			m_pStcChongNengAtt2->SetText(g_StrTable[_T("HolyLingNeng")]);
			_stprintf(szText, _T("10"));
			m_pStcChongNengAttValue2->SetText(szText);
			m_bIsLingNengChongNeng = FALSE;
			m_pWndChongNengSucc->FlipToTop();
			m_pGUI->SetModal(m_pWndChongNengSucc);
		}
		break;
	case E_EnhanceHolyEquip_SilverNotEnough:	// 给圣纹充能，所需金币不足
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_SilverNotEnough")]);
		break;
	case E_EnhanceHolyEquip_CountExceed:		// 给圣纹充能，充能次数已经达到最大
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_CountExceed")]);
		break;
	case E_EnhanceHolyEquip_ProtoNotFound:	// 给圣纹充能，找不到强化道具对应的属性
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_ProtoNotFound")]);
		break;
	case E_EnhanceHolyEquip_MinHolyExceed:	// 给圣纹充能，消耗的灵能已经达到最小灵能值
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_MinHolyExceed")]);
		break;
	case E_EnhanceHolyEquip_ItemNotFit:	// 给圣纹充能，充能道具不符合
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_ItemNotFit")]);
		break;
	case E_EnhanceHolyEquip_MaxHolyExceed:		// 给圣纹基础属性充能，圣灵的灵能消耗不可再增加10
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_MaxHolyExceed")]);
		break;
	case E_EnhanceHolyEquip_ProtoNotExist:		// 给圣纹基础属性充能，圣纹本身不具备要偏向的属性
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_ProtoNotExist")]);
		break;
	}

	return 0;
}
DWORD ShengLingFrame::OnNS_LingNeng(tagNS_LingNeng* pmsg, LPVOID pParam)
{
	ChongNengEnable(TRUE);
	RefreshChongNeng();
	m_pProChongNengTime->SetValue(0, TRUE);
	switch(pmsg->dwErrorCode)
	{
	case E_Success:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
			m_pWndChongNengSucc->SetInvisible(FALSE);
			TCHAR szText[128] = {0};
			m_pStcChongNengAtt1->SetText(g_StrTable[_T("HolyLingNeng")]);
			_stprintf(szText, _T("%d"), pmsg->dwValue);
			m_pStcChongNengAttValue1->SetText(szText);
			m_pStcChongNengAtt2->SetText(_T(""));
			m_pStcChongNengAttValue2->SetText(_T(""));
			m_bIsLingNengChongNeng = TRUE;
			m_pWndChongNengSucc->FlipToTop();
			m_pGUI->SetModal(m_pWndChongNengSucc);
		}
		break;
	case E_EnhanceHolyEquip_SilverNotEnough:	// 给圣纹充能，所需金币不足
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_SilverNotEnough")]);
		break;
	case E_EnhanceHolyEquip_CountExceed:		// 给圣纹充能，充能次数已经达到最大
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_CountExceed")]);
		break;
	case E_EnhanceHolyEquip_ProtoNotFound:	// 给圣纹充能，找不到强化道具对应的属性
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_ProtoNotFound")]);
		break;
	case E_EnhanceHolyEquip_MinHolyExceed:	// 给圣纹充能，消耗的灵能已经达到最小灵能值
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_MinHolyExceed")]);
		break;
	case E_EnhanceHolyEquip_ItemNotFit:	// 给圣纹充能，充能道具不符合
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_ItemNotFit")]);
		break;
	case E_EnhanceHolyEquip_MaxHolyExceed:		// 给圣纹基础属性充能，圣灵的灵能消耗不可再增加10
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_MaxHolyExceed")]);
		break;
	case E_EnhanceHolyEquip_ProtoNotExist:		// 给圣纹基础属性充能，圣纹本身不具备要偏向的属性
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EnhanceHolyEquip_ProtoNotExist")]);
		break;
	}
	return 0;
}

VOID ShengLingFrame::ChongNengEnable(BOOL bIsEnable)
{
	m_pPBtnBaseAtt->SetEnable(bIsEnable);
	m_pPBtnSpecAtt->SetEnable(bIsEnable);
	m_pComBoxSelectAtt->GetButton()->SetEnable(bIsEnable);
	m_pBtnBeginChongNeng->SetEnable(bIsEnable);
	m_pWndChongNeng->SetRefresh(TRUE);
}

DWORD ShengLingFrame::OnNS_HolyEquipChg(tagNS_HolyEquipChg* pMsg, LPVOID pParam)
{
	if(!m_pWndChongNeng->IsInvisible())
		RefreshChongNeng();

	return 0;
}

VOID ShengLingFrame::ClearChongNeng()
{
	m_n64ChongNengItemID = GT_INVALID;
	m_n64PianXiangItemID = GT_INVALID;
	m_byLastSelectIndex = GT_INVALID;
}

DWORD ShengLingFrame::OnNS_HolyStrength(tagNS_HolyStrength* pmsg, LPVOID pPrarm)
{
	switch(pmsg->dwErrorCode)
	{
	case E_Success:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
			break;
		}
	case E_NotHolyStrengthItem:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotHolyStrengthItem")]);
			break;
		}
	case E_NotHolyItem:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotHolyItem")]);
			break;
		}
	case E_HolyEatNumMax:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("HolyEatNumMax")]);
			break;
		}
	}

	return 0;
}

DWORD ShengLingFrame::OnNS_HolyEquipmentNumber(tagNS_HolyEquipmentNumber* pmsg, LPVOID pPrarm)
{
	switch(pmsg->dwErrorCode)
	{
	case E_Success:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
			break;
		}
	case E_NotHolyItem:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotHolyItem")]);
			break;
		}
	case E_NotHolyEquipmentNumber:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotHolyEquipmentNumber")]);
			break;
		}
	case E_HolyEquipmentNumberMax:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("HolyEquipmentNumberMax")]);
			break;
		}
	}

	return 0;
}