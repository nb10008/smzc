#include "StdAfx.h"
#include "PetFrame.h"
#include "RoleMgr.h"
#include "PetManager.h"
#include "PetDef.h"
#include "PetEvent.h"
#include "PetShowWindow.h"
#include "PetBagFrame.h"
#include "CombatSysUtil.h"
#include "FilterData.h"
#include "Item.h"
#include "ItemMgr.h"
#include "Container.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "DragBtn.h"
#include "PetInformationFrame.h"
#include "PetTradeUIFrame.h"
#include "PetProtoData.h"
#include "EasyBuyFrame.h"
#include "../WorldDefine/msg_pet.h"
#include "PetTradeUIFrame.h"
#include "PetPreviewCamera.h"

PetFrame::PetFrame(void) :
    m_Trunk(this),
    m_dwCurrSelectPetID(GT_INVALID),
    m_pPetBagFrame(0),
    m_nSelectRow(0),
    m_dwStartTime(0),
    m_nCurrPage(0),
	m_bCurrPetDead(FALSE),
	m_dwCurrSelectPetID1(GT_INVALID),
	m_nSelectRow1(0),
	m_bCurrPetDead1(FALSE),
    m_nCurrPage1(0),
	m_eCurClickPage(Click_Pet_Page),
	m_bCurExtendMutex(Click_Pet_Page),
	m_iCurPetNum(0),
	m_iCurRiderNum1(0),
	m_iCurPetGridNum(MAX_PET_NUM),
	m_iCurRiderGridNum(MAX_PET_NUM)
{
}

PetFrame::~PetFrame(void)
{
}

BOOL PetFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    //! 注册网络事件
    m_pCmdMgr->Register("NS_PetRename", (NETMSGPROC)m_Trunk.sfp2(&PetFrame::OnNS_PetRename),  _T("NS_PetRename"));

    //! 注册游戏事件
    m_pFrameMgr->RegisterEventHandle(_T("Open_Pet"),            (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnOpen_Pet));
	m_pFrameMgr->RegisterEventHandle(_T("Close_Pet"),            (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnClose_Pet));
    m_pFrameMgr->RegisterEventHandle(_T("tagPetAttrChange"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnPetAttrChange));
    //m_pFrameMgr->RegisterEventHandle(_T("tagUsePetEquip"),      (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnUsePetEquipHandler));
    m_pFrameMgr->RegisterEventHandle(_T("BoxDeletePet"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnCommitDeletePet));
	m_pFrameMgr->RegisterEventHandle(_T("BoxCallBackPet"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnCommitCallBackPet));
    //m_pFrameMgr->RegisterEventHandle(_T("BoxPourPetExp"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnCommitPourExp));
    m_pFrameMgr->RegisterEventHandle(_T("ChangePetName"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnCommitPetName));
    m_pFrameMgr->RegisterEventHandle(_T("tagOpenPetbyID"),       (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnOpen_PetByID));
	m_pFrameMgr->RegisterEventHandle(_T("tagMaxPetNumChanged"),  (FRAMEEVENTPROC)m_Trunk.sfp0(&PetFrame::RefreshPetList));
    m_pFrameMgr->RegisterEventHandle(_T("Refresh_PetUI"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnEvent_Refresh_PetUI));
	m_pFrameMgr->RegisterEventHandle(_T("tagPetDispChange"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnPetDispInfo));
	m_pFrameMgr->RegisterEventHandle(_T("ExtendMsgBoxClickEvent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnMsgBoxClickEvent));
    //! create pet bag frame
    PetBagFrame *pFrame = (PetBagFrame*)m_pFrameMgr->GetFrame(_T("PetBag"));
    if (!P_VALID(pFrame))
        m_pPetBagFrame = static_cast<PetBagFrame*>(m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("PetBag"), _T("PetBagFrame"), 0));

    return bRet;
}

BOOL PetFrame::Destroy()
{
    BOOL bRet = GameFrame::Destroy();

	PetManager::Inst()->SetTradeGainPets(FALSE);
    //! 销毁窗体
    if( P_VALID(m_pWnd) )
    {
        m_pGUI->AddToDestroyList(m_pWnd);
        m_pWnd = NULL;
    }

    // 删除对话框
    if (P_VALID(GetObj("ChangePetName")))
        KillObj("ChangePetName");
	if (P_VALID(GetObj("BoxDeletePet")))
		KillObj("BoxDeletePet");
	if (P_VALID(GetObj("BoxCallBackPet")))
		KillObj("BoxCallBackPet");
	if (P_VALID(GetObj("ExtendMsgBoxClickEvent")))
		KillObj("ExtendMsgBoxClickEvent");

    //! 注销网络事件
    m_pCmdMgr->UnRegister("NS_PetRename", (NETMSGPROC)m_Trunk.sfp2(&PetFrame::OnNS_PetRename));

    //! 注销游戏事件
    m_pFrameMgr->UnRegisterEventHandler(_T("Open_Pet"),         (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnOpen_Pet));
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_Pet"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnClose_Pet));
    m_pFrameMgr->UnRegisterEventHandler(_T("tagPetAttrChange"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnPetAttrChange));
    //m_pFrameMgr->UnRegisterEventHandler(_T("tagUsePetEquip"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnUsePetEquipHandler));
    m_pFrameMgr->UnRegisterEventHandler(_T("BoxDeletePet"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnCommitDeletePet));
	m_pFrameMgr->UnRegisterEventHandler(_T("BoxCallBackPet"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnCommitCallBackPet));
    //m_pFrameMgr->UnRegisterEventHandler(_T("BoxPourPetExp"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnCommitPourExp));
    m_pFrameMgr->UnRegisterEventHandler(_T("ChangePetName"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnCommitPetName));
    m_pFrameMgr->UnRegisterEventHandler(_T("tagOpenPetbyID"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnOpen_PetByID));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagMaxPetNumChanged"),  (FRAMEEVENTPROC)m_Trunk.sfp0(&PetFrame::RefreshPetList));
	m_pFrameMgr->UnRegisterEventHandler( _T("Refresh_PetUI"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnEvent_Refresh_PetUI));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagPetDispChange"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnPetDispInfo));
	m_pFrameMgr->UnRegisterEventHandler(_T("ExtendMsgBoxClickEvent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&PetFrame::OnMsgBoxClickEvent));

	ClearPetInfo();

    return bRet;
}

BOOL PetFrame::ReloadUI()
{
    XmlElement ele;
    tstring strPath = g_strLocalPath + _T("\\ui\\pet.xml");
    m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

    // 更改控件
    char chbuff[128];
    for (int i = 0; i < MAX_PET_SKILL; ++i)
    {
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back0\\pet_ability_back\\pic_back%d\\pic", i);
		m_pGUI->ChangeXml(&ele, chbuff, "ClassName", "PetSkillButton");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back1\\pet_ability_back\\pic_back%d\\pic", i);
		m_pGUI->ChangeXml(&ele, chbuff, "ClassName", "PetSkillButton");
    }
	m_pGUI->ChangeXml(&ele, "pet_win\\pet_pic\\back0\\pet_show\\pic", "ClassName", "PetShowWindow");
	m_pGUI->ChangeXml(&ele, "pet_win\\pet_pic\\back1\\pet_show\\pic", "ClassName", "PetShowWindow");
    for (int i = 0; i < MAX_PET_NUM; ++i)
    {
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back0\\basic%d\\pic_back", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back1\\basic%d\\pic_back", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back0\\basic%d\\pic_back\\pic", i);
		m_pGUI->ChangeXml(&ele, chbuff, "ClassName", "ItemButton");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back1\\basic%d\\pic_back\\pic", i);
		m_pGUI->ChangeXml(&ele, chbuff, "ClassName", "ItemButton");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back0\\basic%d\\choose", i);
		m_pGUI->ChangeXml(&ele, chbuff, "ClassName", "StaticEx");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back1\\basic%d\\choose", i);
		m_pGUI->ChangeXml(&ele, chbuff, "ClassName", "StaticEx");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back0\\basic%d\\button", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back1\\basic%d\\button", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back0\\basic%d\\word", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back1\\basic%d\\word", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back0\\basic%d\\time", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back1\\basic%d\\time", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back0\\basic%d\\state", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back1\\basic%d\\state", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back0\\basic%d\\name", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back1\\basic%d\\name", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back0\\basic%d\\select", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		_snprintf(chbuff, sizeof(chbuff) / sizeof(char), "pet_win\\pet_pic\\back1\\basic%d\\select", i);
		m_pGUI->ChangeXml(&ele, chbuff, "TopMost", "yes");
		
    }

    // 创建
    m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

    // 注册事件处理函数
    m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&PetFrame::EventHandler));

    // 获得控件
    TCHAR tbuff[128];

    for (int index = 0; index != MAX_PET_SKILL; ++index)
    {
		_sntprintf(tbuff, sizeof(tbuff) / sizeof(TCHAR), _T("pet_pic\\back0\\pet_ability_back\\pic_back%d\\pic"), index);
		m_pBtSkillA[index]  =   static_cast<PetSkillButton*>(m_pWnd->GetChild(tbuff));         // 宠物技能
		_sntprintf(tbuff, sizeof(tbuff) / sizeof(TCHAR), _T("pet_pic\\back1\\pet_ability_back\\pic_back%d\\pic"), index);
		m_pBtSkillA1[index]  =   static_cast<PetSkillButton*>(m_pWnd->GetChild(tbuff));         // 乘骑技能
    }
	m_pPchPetPage		=	static_cast<GUIPatch*>(m_pWnd->GetChild(_T("pet_pic\\back0")));
	m_pPchRiderPage		=	static_cast<GUIPatch*>(m_pWnd->GetChild(_T("pet_pic\\back1")));
	m_pBtPetPage		=	static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("pet_pic\\winopt\\optbutt0")));
	m_pBtPetPage->SetEnable(TRUE);
	m_pBtPetPage->SetState(EGUIBS_PushDown, FALSE);
	m_pBtRiderPage		=	static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("pet_pic\\winopt\\optbutt1")));
	m_pBtRiderPage->SetEnable(TRUE);
	m_pStcPetShow       =   static_cast<PetShowWindow*>(m_pWnd->GetChild(_T("pet_pic\\back0\\pet_show\\pic")));                // 宠物Avata
	m_pStcPetShow1      =   static_cast<PetShowWindow*>(m_pWnd->GetChild(_T("pet_pic\\back1\\pet_show\\pic")));                // 乘骑
	m_pBtPrevPage       =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\back0\\butt1")));
	m_pBtNextPage       =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\back0\\butt2")));
	m_pStcPage          =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\back0\\pagenum")));
	m_pBtPrevPage1      =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\back1\\butt1")));
	m_pBtNextPage1      =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\back1\\butt2")));
	m_pStcPage1         =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\back1\\pagenum")));
    m_pBtChangeName     =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\rename")));
    m_pBtSetFree        =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\offbutt")));         // 放生
    m_pBtTrade          =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\exchange")));        // 交易
	m_pBtExtend			 =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\roombutt")));        // 扩充
	m_pBtClose          =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\closebutt")));   // 关闭
	m_pBtCallBack          =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("pet_pic\\bindrelease")));   // 回收
	//宠物分页
    for (int i = 0; i != MAX_PET_NUM; ++i)
    {
		_sntprintf(tbuff, sizeof(tbuff) / sizeof(char), _T("pet_pic\\back0\\basic%d"), i);
		m_pPchBack[i]       = static_cast<GUIPatch*>(m_pWnd->GetChild(tbuff));
		m_pStcIcon[i]       = static_cast<StaticEx*>(m_pPchBack[i]->GetChild(_T("pic_back\\pic"))); //头像
		m_pStcNick[i]       = static_cast<GUIStatic*>(m_pPchBack[i]->GetChild(_T("name")));  //名字
		m_pStcIsCalled[i]   = static_cast<GUIStatic*>(m_pPchBack[i]->GetChild(_T("state"))); //宠_准备
		m_pBtCall[i]        =   static_cast<GUIButton*>(m_pPchBack[i]->GetChild(_T("button")));   // 召唤
		m_pBtCall[i]->FlipToTop();
		m_pStcIsBinding[i]   =   static_cast<GUIButton*>(m_pPchBack[i]->GetChild(_T("word")));  // 是否绑定
		m_pStcIsBinding[i]->SetInvisible(TRUE);
		m_pStcIsBinding[i]->SetText(g_StrTable[_T("PetBinding")]);
		m_pStcTime[i]		 =   static_cast<GUIStatic*>(m_pPchBack[i]->GetChild(_T("time")));  // 宠物寿命
		m_pStcOption[i]		 =   static_cast<StaticEx*>(m_pPchBack[i]->GetChild(_T("choose"))); 
		m_pBtBack[i]        =   static_cast<GUIButton*>(m_pPchBack[i]->GetChild(_T("select")));  // 获取点击的宠物
	}
	//乘骑分页
	for (int i = 0; i != MAX_PET_NUM; ++i)
	{
		_sntprintf(tbuff, sizeof(tbuff) / sizeof(char), _T("pet_pic\\back1\\basic%d"), i);
		m_pPchBack1[i]       = static_cast<GUIPatch*>(m_pWnd->GetChild(tbuff));
		m_pStcIcon1[i]       = static_cast<StaticEx*>(m_pPchBack1[i]->GetChild(_T("pic_back\\pic"))); //头像
		m_pStcNick1[i]       = static_cast<GUIStatic*>(m_pPchBack1[i]->GetChild(_T("name")));  //名字
		m_pStcIsPreparing1[i] = static_cast<GUIStatic*>(m_pPchBack1[i]->GetChild(_T("state"))); //骑_驾驭
		m_pBtRide1[i]        =   static_cast<GUIButton*>(m_pPchBack1[i]->GetChild(_T("button")));  // 骑乘
		m_pBtRide1[i]->FlipToTop();
		m_pStcIsBinding1[i]   =   static_cast<GUIButton*>(m_pPchBack1[i]->GetChild(_T("word")));        // 是否绑定
		m_pStcIsBinding1[i]->SetInvisible(TRUE);
		m_pStcIsBinding1[i]->SetText(g_StrTable[_T("PetBinding")]);
		m_pStcTime1[i]		  =   static_cast<GUIStatic*>(m_pPchBack1[i]->GetChild(_T("time")));        // 宠物寿命
		m_pStcOption1[i]	  =   static_cast<StaticEx*>(m_pPchBack1[i]->GetChild(_T("choose"))); 
		m_pBtBack1[i]        =   static_cast<GUIButton*>(m_pPchBack1[i]->GetChild(_T("select")));  // 获取点击的乘骑
	}
    
	//for (INT count = 0;count < MAX_PET_SKILL;count++)
	//{
	//	m_pBtSkillB[count]->SetPicFileName(GREY_PIC);
	//}

    // 初始窗体关闭
    ShowWnd(FALSE);

    // 清空ui
    ClearUI();

    return TRUE;
}

VOID PetFrame::Update()
{
    GameFrame::Update();

    // 每1秒刷新下界面
    if (m_pWnd->IsInvisible())
        return;

    if (Kernel::Inst()->GetAccumTimeDW() - m_dwStartTime > 1000)
    {
        m_dwStartTime = Kernel::Inst()->GetAccumTimeDW();

		// 刷新乘骑图标
		if ( Click_Rider_Page == m_eCurClickPage )
		{
			for (map<INT, DWORD>::iterator it = m_mapRow2Pet1.begin();
				it != m_mapRow2Pet1.end(); ++it)
			{
				const tagPet* pPetSingle = PetManager::Inst()->GetPet(it->second);
				if (NULL == pPetSingle)
				{
					continue;
				}
				const tagPetProto* pPetSingleProto = PetProtoData::Inst()->FindPetProto(pPetSingle->dwTypeID);
				if (!P_VALID(pPetSingleProto))
				{
					continue;
				}

				if (PetManager::Inst()->IsPetRiding(pPetSingle->dwID))
				{
					m_pStcIcon1[it->first]->SetPicFile(pPetSingleProto->strIconPath[EPMT_Mount]);
				}
				else
				{
					m_pStcIcon1[it->first]->SetPicFile(pPetSingleProto->strIconPath[EPMT_Follow]);
				}
			}
		}
    }

	if ( Click_Pet_Page == m_eCurClickPage )
	{
		if ( m_dwCurrSelectPetID  == GT_INVALID 
			|| m_mapRow2Pet.empty() )
		{
			m_pBtTrade->SetEnable(FALSE);
			m_pBtChangeName->SetEnable(FALSE);
			m_pBtSetFree->SetEnable(FALSE);
			m_pBtCallBack->SetEnable(FALSE);
		} 
		else
		{
			m_pBtChangeName->SetEnable(TRUE);
			m_pBtSetFree->SetEnable(TRUE);
			m_pBtCallBack->SetEnable(TRUE);
			if (P_VALID(m_pMgr->GetFrame(_T("PetTradeUIFrame"))))
			{
				PetTradeUIFrame* pPetTradeFrame = (PetTradeUIFrame*)m_pMgr->GetFrame(_T("PetTradeUIFrame"));
				if (pPetTradeFrame->IsPetInTradeList(m_dwCurrSelectPetID))
				{
					m_pBtChangeName->SetEnable(FALSE);
					m_pBtSetFree->SetEnable(FALSE);
					m_pBtCallBack->SetEnable(FALSE);
				}
				map<INT, DWORD>::const_iterator  cIter = m_mapRow2Pet.begin();
				for ( ; cIter != m_mapRow2Pet.end(); cIter++ )
				{
					if ( pPetTradeFrame->IsPetInTradeList(cIter->second) )
						m_pBtCall[cIter->first]->SetEnable(FALSE);
					else
						m_pBtCall[cIter->first]->SetEnable(TRUE);
				}
			}
			if ( PetManager::Inst()->IsPetWorking(m_dwCurrSelectPetID) 
			  || PetManager::Inst()->IsPetCalled(m_dwCurrSelectPetID) )
			{
				m_pBtTrade->SetEnable(FALSE);
			}
			else
			{
				m_pBtTrade->SetEnable(TRUE);
			}
		}
	}
	else if ( Click_Rider_Page == m_eCurClickPage )
	{
		if ( m_dwCurrSelectPetID1 == GT_INVALID 
			|| m_mapRow2Pet1.empty() )
		{
			m_pBtTrade->SetEnable(FALSE);
			m_pBtChangeName->SetEnable(FALSE);
			m_pBtSetFree->SetEnable(FALSE);
			m_pBtCallBack->SetEnable(FALSE);
			
		} 
		else
		{
			m_pBtChangeName->SetEnable(TRUE);
			if ( PetManager::Inst()->IsPetRiding(m_dwCurrSelectPetID1) )
			{
				m_pBtTrade->SetEnable(FALSE);
				m_pBtSetFree->SetEnable(FALSE);
				m_pBtCallBack->SetEnable(FALSE);
			}
			else
			{
				m_pBtTrade->SetEnable(TRUE);
				m_pBtSetFree->SetEnable(TRUE);
				m_pBtCallBack->SetEnable(TRUE);
			}
			if (P_VALID(m_pMgr->GetFrame(_T("PetTradeUIFrame"))))
			{
				PetTradeUIFrame* pPetTradeFrame = (PetTradeUIFrame*)m_pMgr->GetFrame(_T("PetTradeUIFrame"));
				if (pPetTradeFrame->IsPetInTradeList(m_dwCurrSelectPetID1))
				{
					m_pBtChangeName->SetEnable(FALSE);
					m_pBtSetFree->SetEnable(FALSE);
					m_pBtCallBack->SetEnable(FALSE);
				}
				map<INT, DWORD>::const_iterator  cIter = m_mapRow2Pet1.begin();
				for ( ; cIter != m_mapRow2Pet1.end(); cIter++ )
				{
					if ( pPetTradeFrame->IsPetInTradeList(cIter->second) )
						m_pBtRide1[cIter->first]->SetEnable(FALSE);
					else
						m_pBtRide1[cIter->first]->SetEnable(TRUE);
				}
			}
		}
	}
}

BOOL PetFrame::EscCancel()
{
	if( m_pWnd->IsInvisible() )
		return FALSE;

	m_pWnd->SetInvisible(TRUE);
	m_pMgr->SendEvent( &tagGameEvent( _T("Close_Pet_UI"), this ) );
	return TRUE;
}

DWORD PetFrame::OnOpen_Pet( tagGameEvent* pMsg )
{
	ShowWnd(TRUE);
	return 0;
}

DWORD PetFrame::OnClose_Pet( tagGameEvent* pMsg )
{
	ShowWnd(FALSE);

    return 0;
}

VOID PetFrame::ShowWnd( BOOL bVal )
{
    m_pWnd->SetInvisible(!bVal);
    if (bVal)
    {
		m_pWnd->FlipToTop();
		// 刷新宠物图标
		if ( Click_Pet_Page == m_eCurClickPage )
		{
			// 选择宠物
			map<INT, DWORD>::const_iterator it = m_mapRow2Pet.find(m_nSelectRow);
			if (it != m_mapRow2Pet.end())
				m_dwCurrSelectPetID = it->second;
			else
				m_dwCurrSelectPetID = GT_INVALID;
			m_pPchPetPage->SetInvisible(FALSE);
			m_pPchRiderPage->SetInvisible(TRUE);
		} 
		// 刷新乘骑图标
		else if ( Click_Rider_Page == m_eCurClickPage )
		{
			// 选择宠物
			map<INT, DWORD>::const_iterator it = m_mapRow2Pet1.find(m_nSelectRow1);
			if (it != m_mapRow2Pet1.end())
				m_dwCurrSelectPetID1 = it->second;
			else
				m_dwCurrSelectPetID1 = GT_INVALID;
			m_pPchPetPage->SetInvisible(TRUE);
			m_pPchRiderPage->SetInvisible(FALSE);
		}
		RefreshPetList();
    }
    else
    {
        m_pMgr->SendEvent( &tagGameEvent( _T("Close_Pet_UI"), this ) );
    }
}

DWORD PetFrame::EventHandler( tagGUIEvent* pEvent )
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
	case EGUIE_PushButtonDown:
		 {
			 if (pEvent->dwParam1 != 1)//不是按下不处理
				 return 0;
			 // 宠物
			 if (pWnd == m_pBtPetPage)
			 {
				 m_pPchPetPage->SetInvisible(FALSE);
				 m_pPchPetPage->SetUnClickable(FALSE);
				 m_pPchRiderPage->SetInvisible(TRUE);
				 m_pPchRiderPage->SetUnClickable(TRUE);
				 m_eCurClickPage = Click_Pet_Page;
				 RefreshPetList();
				 break;
			 }
			 // 坐骑
			 else if (pWnd == m_pBtRiderPage)
			 {
				 m_pPchPetPage->SetInvisible(TRUE);
				 m_pPchPetPage->SetUnClickable(TRUE);
				 m_pPchRiderPage->SetInvisible(FALSE);
				 m_pPchRiderPage->SetUnClickable(FALSE);
				 m_eCurClickPage = Click_Rider_Page;
				 RefreshPetList();
				 break;
			 }
		 }
		 break;
    case EGUIE_Click:
        {
            if (pWnd == m_pBtClose)
            {
                // 关闭
                ShowWnd(FALSE);
				 m_pMgr->SendEvent( &tagGameEvent( _T("Close_Pet_UI"), this ) );
				 break;
            }
			 else if (pWnd->GetName().substr(0, 6) == _T("select"))
			 {
				// 点击宠物乘骑列表某一列
				 if (Click_Pet_Page == m_eCurClickPage)
				 {
					 for (int i = 0; i < MAX_PET_NUM; ++i)
					 {

						 if(m_nCurrPage*MAX_PET_NUM + i >= m_iCurPetNum ) 
							 break;
						 if (pWnd == m_pBtBack[i])
						 {
							 m_pStcPetShow->SetModelYaw(0.0f);
							 // 选择宠物
							 m_nSelectRow = i;			
							 map<INT, DWORD>::const_iterator it = m_mapRow2Pet.find(i);
							 if (it != m_mapRow2Pet.end())
								 m_dwCurrSelectPetID = it->second;
							 else
								 m_dwCurrSelectPetID = GT_INVALID;
							 // 刷新数据到界面
							 RefreshPetList();
							 break;
						 }
					 }
				 }
				 else if (Click_Rider_Page == m_eCurClickPage)
				 {
					 for (int i = 0; i < MAX_PET_NUM; ++i)
					 {
						 if(m_nCurrPage1*MAX_PET_NUM + i >= m_iCurRiderNum1 ) 
							 break;
						 if (pWnd == m_pBtBack1[i])
						 {
							 m_pStcPetShow1->SetModelYaw(0.0f);
							 // 选择宠物
							 m_nSelectRow1 = i;			
							 map<INT, DWORD>::const_iterator it = m_mapRow2Pet1.find(i);
							 if (it != m_mapRow2Pet1.end())
								 m_dwCurrSelectPetID1 = it->second;
							 else
								 m_dwCurrSelectPetID1 = GT_INVALID;
						     // 刷新数据到界面
							 RefreshPetList();
							 break;
						 }
					 }
				 }
			 }
            else if (pWnd == m_pBtPrevPage)
            {
                // 宠物上一页
                if (m_nCurrPage > 0)
                {
                    --m_nCurrPage;
					  PageChangeRefresh();
                    RefreshPetList();
                }
				 break;
            }
            else if (pWnd == m_pBtNextPage)
            {
                // 宠物下一页
                int nPageCount = PetManager::Inst()->GetMaxPetsCount() % MAX_PET_NUM == 0 ?
                    PetManager::Inst()->GetMaxPetsCount() / MAX_PET_NUM :
                    PetManager::Inst()->GetMaxPetsCount() / MAX_PET_NUM + 1;
                if (m_nCurrPage < nPageCount - 1)
                {
                    ++m_nCurrPage;
					  PageChangeRefresh();
                    RefreshPetList();
                }
				 break;
            }
			else if (pWnd == m_pBtPrevPage1)
			{
				// 乘骑上一页
				if (m_nCurrPage1 > 0)
				{
					--m_nCurrPage1;
					PageChangeRefresh();
					RefreshPetList();
				}
				break;
			}
			else if (pWnd == m_pBtNextPage1)
			{
				// 乘骑下一页
				int nPageCount = PetManager::Inst()->GetMaxRiderCount() % MAX_PET_NUM == 0 ?
					PetManager::Inst()->GetMaxRiderCount() / MAX_PET_NUM :
					PetManager::Inst()->GetMaxRiderCount() / MAX_PET_NUM + 1;
				if (m_nCurrPage1 < nPageCount - 1)
				{
					++m_nCurrPage1;
					PageChangeRefresh();
					RefreshPetList();
					
				}
				break;
			}
			else if (pWnd == m_pBtChangeName)
			{
				// 改名
				if (P_VALID(GetObj("ChangePetName")))
					KillObj("ChangePetName");
				CreateObj("ChangePetName", "MsgInputBox");
				MsgInputBox* pMsgBox = TObjRef<MsgInputBox>("ChangePetName");

				pMsgBox->Init(_T(""), g_StrTable[_T("PetInfo01")], _T("ChangePetName"), -1, MBIT_Name);
				break;
			}
			else if (pWnd->GetName().substr(0, 6) == _T("button"))
			{
				//点击召唤或者预备乘骑按钮
				for (int i = 0; i < MAX_PET_NUM; ++i)
				{
					if (m_pBtCall[i] == pWnd)
					{
						m_pStcPetShow->SetModelYaw(0.0f);
						// 选择宠物
						m_nSelectRow = i;			
						map<INT, DWORD>::const_iterator it = m_mapRow2Pet.find(i);
						if (it != m_mapRow2Pet.end())
							m_dwCurrSelectPetID = it->second;
						else
							m_dwCurrSelectPetID = GT_INVALID;
						// 召唤宠物
						if (m_dwCurrSelectPetID != GT_INVALID)
						{
							//加上死亡宠物召唤判断
							const tagPet* pPet = PetManager::Inst()->GetPet(m_dwCurrSelectPetID);
							if (NULL == pPet)
							{
								break;
							}
							const tagPetProto* pPetProto = PetProtoData::Inst()->FindPetProto(pPet->dwTypeID);
							if (!P_VALID(pPetProto))
							{
								break;
							}
							//因为程序中宠物死亡的参数飘忽不定（不知道什么BUG引起的），所以只能用次麻烦方法来代替宠物死亡检测
							//nLifeTime == -1说明为不死宠物
							//m_mapPetLifeEndMap中存的是时限制宠物中还没有死亡的宠物
							if (pPetProto->nLifeTime != -1 )
							{
								map<DWORD,tagDWORDTime>::const_iterator ite = PetManager::Inst()->GetPetLifeMap().find(m_dwCurrSelectPetID);
								if (ite == PetManager::Inst()->GetPetLifeMap().end())
								{
									//没有找到说明宠物已死亡
									CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_PetLifeTime_CanNotCall")]);
									break;
								}
							}	
							if (m_dwCurrSelectPetID != GT_INVALID)
							{
								PetManager::Inst()->BeginSetPetState(m_dwCurrSelectPetID, EPS_Called, !PetManager::Inst()->IsPetCalled(m_dwCurrSelectPetID));
								break;
							}
						}
					}
					else if ( m_pBtRide1[i] == pWnd)
					{
						m_pStcPetShow1->SetModelYaw(0.0f);
						// 选择宠物
						m_nSelectRow1 = i;	
						map<INT, DWORD>::const_iterator it = m_mapRow2Pet1.find(i);
						if (it != m_mapRow2Pet1.end())
							m_dwCurrSelectPetID1 = it->second;
						else
							m_dwCurrSelectPetID1 = GT_INVALID;
						// 准备乘骑
						if (m_dwCurrSelectPetID1 != GT_INVALID)
						{
							//加上死亡宠物召唤判断
							const tagPet* pPet = PetManager::Inst()->GetPet(m_dwCurrSelectPetID1);
							if (NULL == pPet)
							{
								break;
							}
							const tagPetProto* pPetProto = PetProtoData::Inst()->FindPetProto(pPet->dwTypeID);
							if (!P_VALID(pPetProto))
							{
								break;
							}
							if (pPetProto->nLifeTime != -1 )
							{
								map<DWORD,tagDWORDTime>::const_iterator ite = PetManager::Inst()->GetPetLifeMap().find(m_dwCurrSelectPetID1);
								if (ite == PetManager::Inst()->GetPetLifeMap().end())
								{
									//没有找到说明宠物已死亡
									CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_PetLifeTime_CanNotCall")]);
									break;
								}
							}	
							if (m_dwCurrSelectPetID1 != GT_INVALID)
							{
								PetManager::Inst()->BeginSetPetState(m_dwCurrSelectPetID1, EPS_Preparing, !PetManager::Inst()->IsPetPreparing(m_dwCurrSelectPetID1));
								break;
							}
						}
					}
				}//for
				RefreshPetList();
				break;
			}
			else if (pWnd == m_pBtSetFree)
			{
				// 放生
				if ( Click_Pet_Page == m_eCurClickPage )
				{
					if (m_dwCurrSelectPetID != GT_INVALID)
					{
						if (P_VALID(GetObj("BoxDeletePet")))
							KillObj("BoxDeletePet");
						CreateObj("BoxDeletePet", "MsgBox");
						TObjRef<MsgBox>("BoxDeletePet")->Init(_T(""), g_StrTable[_T("PetDelete")], _T("BoxDeletePet"), (MsgBoxFlag)(MBF_OK | MBF_Cancel), TRUE);
						break;
					}
				}
				else if ( Click_Rider_Page == m_eCurClickPage )
				{
					if (m_dwCurrSelectPetID1 != GT_INVALID)
					{
						if (P_VALID(GetObj("BoxDeletePet")))
							KillObj("BoxDeletePet");
						CreateObj("BoxDeletePet", "MsgBox");
						TObjRef<MsgBox>("BoxDeletePet")->Init(_T(""), g_StrTable[_T("PetDelete")], _T("BoxDeletePet"), (MsgBoxFlag)(MBF_OK | MBF_Cancel), TRUE);
						break;
					}
				}
			}
			else if (pWnd == m_pBtCallBack)
			{
				// 回收
				if ( Click_Pet_Page == m_eCurClickPage )
				{
					if (m_dwCurrSelectPetID != GT_INVALID)
					{
						if (P_VALID(GetObj("BoxCallBackPet")))
							KillObj("BoxCallBackPet");
						CreateObj("BoxCallBackPet", "MsgBox");
						TObjRef<MsgBox>("BoxCallBackPet")->Init(_T(""), g_StrTable[_T("PetCallBack")], _T("BoxCallBackPet"), (MsgBoxFlag)(MBF_OK | MBF_Cancel), TRUE);
						break;
					}
				}
				else if ( Click_Rider_Page == m_eCurClickPage )
				{
					if (m_dwCurrSelectPetID1 != GT_INVALID)
					{
						if (P_VALID(GetObj("BoxCallBackPet")))
							KillObj("BoxCallBackPet");
						CreateObj("BoxCallBackPet", "MsgBox");
						TObjRef<MsgBox>("BoxCallBackPet")->Init(_T(""), g_StrTable[_T("PetCallBack")], _T("BoxCallBackPet"), (MsgBoxFlag)(MBF_OK | MBF_Cancel), TRUE);
						break;
					}
				}
			}
			else if (pWnd == m_pBtExtend)
			{
				tstringstream tss;
				if ( Click_Pet_Page == m_eCurClickPage )
					tss<<g_StrTable[_T("PetExtendMsgBoxClickEvent")];
				else
					tss<<g_StrTable[_T("RiderExtendMsgBoxClickEvent")];
				 // 扩充
				if (P_VALID(GetObj("ExtendMsgBoxClickEvent")))
					KillObj("ExtendMsgBoxClickEvent");
				CreateObj("ExtendMsgBoxClickEvent", "MsgBox");
				TObjRef<MsgBox>("ExtendMsgBoxClickEvent")->Init( 
					_T(""), tss.str().c_str(),       
					_T("ExtendMsgBoxClickEvent"),(MsgBoxFlag)(MBF_OK|MBF_Cancel), FALSE );
				m_pWnd->SetUnClickable(TRUE);
			}
			else if (pWnd == m_pBtTrade)
			{
				// 交易
				if (Click_Pet_Page == m_eCurClickPage)
				{
					if (m_dwCurrSelectPetID != GT_INVALID)
					{
						PetTradeUIFrame* pFrame = (PetTradeUIFrame*)m_pMgr->GetFrame(_T("PetTradeUIFrame"));
						if (P_VALID(pFrame))
						{
							pFrame->BeginNC_PetExchangeAdd(m_dwCurrSelectPetID);
						}
					}
				}
				else if (Click_Rider_Page == m_eCurClickPage)
				{
					if (m_dwCurrSelectPetID1 != GT_INVALID)
					{
						PetTradeUIFrame* pFrame = (PetTradeUIFrame*)m_pMgr->GetFrame(_T("PetTradeUIFrame"));
						if (P_VALID(pFrame))
						{
							pFrame->BeginNC_PetExchangeAdd(m_dwCurrSelectPetID1);
						}
					}
				}
			}
			//使用技能
			//else if (pWnd->GetName().substr(0, 3) == _T("pic"))
			//{
			//	if (Click_Pet_Page == m_eCurClickPage)
			//	{
			//		// 点击技能
			//		for (int i = 0; i < MAX_PET_SKILL; ++i)
			//		{
			//			if (m_pBtSkillA[i] == pWnd)
			//			{
			//				const tagPetSkillData* pSkillData = m_pBtSkillA[i]->GetSkillData();
			//				if (P_VALID(pSkillData))
			//				{
			//					if(m_pBtSkillA[i]->IsEnable())
			//						PetManager::Inst()->TryToUsePetSkill(m_dwCurrSelectPetID, pSkillData->dwPetSkillID);
			//				}
			//				break;
			//			}
			//		}
			//	}
			//	else if (Click_Rider_Page == m_eCurClickPage)
			//	{
			//		// 点击技能
			//		for (int i = 0; i < MAX_PET_SKILL; ++i)
			//		{
			//			if (m_pBtSkillA1[i] == pWnd)
			//			{
			//				const tagPetSkillData* pSkillData = m_pBtSkillA1[i]->GetSkillData();
			//				if (P_VALID(pSkillData))
			//				{
			//				    if(m_pBtSkillA1[i]->IsEnable())
			//					    PetManager::Inst()->TryToUsePetSkill(m_dwCurrSelectPetID1, pSkillData->dwPetSkillID);
			//				}
			//				break;
			//			}
			//		}
			//	}
			//}
        }
        break;
    case EGUIE_MoveIntoCtrl:
        {
			if (pWnd->GetName().substr(0, 3) == _T("pic"))
			{
				if (Click_Pet_Page == m_eCurClickPage)
				{
					for (int i = 0; i < MAX_PET_SKILL; ++i)
					{
						if (m_pBtSkillA[i] == pWnd)
						{
							const tagPetSkillData* pSkillData = m_pBtSkillA[i]->GetSkillData();
							if (P_VALID(pSkillData))
							{
							  ShowTip(pWnd, ToolTipCreator::Inst()->GetPetSkillTips(pSkillData->dwPetSkillID).c_str());
							}
							break;
						}
					}
				}
				else if (Click_Rider_Page == m_eCurClickPage)
				{
					for (int i = 0; i < MAX_PET_SKILL; ++i)
					{
						if (m_pBtSkillA1[i] == pWnd)
						{
							const tagPetSkillData* pSkillData = m_pBtSkillA1[i]->GetSkillData();
							if (P_VALID(pSkillData))
							{
								ShowTip(pWnd, ToolTipCreator::Inst()->GetPetSkillTips(pSkillData->dwPetSkillID).c_str());
							}
							break;
						}
					}
				}
			}
        }
        break;
    case EGUIE_MoveOutOfCtrl:
        {
            ShowTip(pWnd, NULL);
        }
        break;
    }

    return 0;
}

VOID PetFrame::RefreshToUI()
{
	DWORD dwCurID = GT_INVALID;
	//宠物分页
	if (Click_Pet_Page == m_eCurClickPage)
	{
		// 高亮当前选择
		if (m_dwCurrSelectPetID != GT_INVALID)
		{
			m_pStcOption[m_nSelectRow]->SetPicFile(SELECT_ICON_PATH);
		}
		for ( int i = 0; i < MAX_PET_NUM; i++)
		{
			if( m_nCurrPage*MAX_PET_NUM + i >= m_iCurPetNum ) 
				break;
			//找到对应宠物
			map<INT, DWORD>::const_iterator it = m_mapRow2Pet.find(i);
			if (it != m_mapRow2Pet.end())
				dwCurID = it->second;
			// 填充数据
			const tagPet* pPet = PetManager::Inst()->GetPet(dwCurID);
			if (NULL == pPet)
			{
				ClearUIData();
				continue;
			}
			const tagPetProto* pPetProto = PetProtoData::Inst()->FindPetProto(pPet->dwTypeID);
			if (!P_VALID(pPetProto))
			{
				ClearUIData();
				continue;
			}
			m_pStcIcon[i]->SetPicFile(pPetProto->strIconPath[EPMT_Follow]);			
			m_pStcNick[i]->SetText(pPet->strName.c_str());
			m_pStcIsBinding[i]->SetInvisible(!pPetProto->bBinding);
			if ( pPetProto->nLifeTime == -1 )
			{
				m_pStcTime[i]->SetText(g_StrTable[_T("Pet_UnDead")]);
				m_pStcTime[i]->SetTextColor(0xFF00FF00);
			}
			else
			{
				tstring tstrLifeTime;
				BOOL bdeath = ID2RestTime(dwCurID, &tstrLifeTime);
				m_pStcTime[i]->SetText(tstrLifeTime.c_str());
				if (bdeath == FALSE)
				{
					m_pStcTime[i]->SetTextColor(0xFF00FF00);
				}
				else
				{
					m_pStcTime[i]->SetTextColor(0xFFFF0000);
					m_pBtCall[i]->SetText(g_StrTable[_T("PetButtonCall")]);
					m_pBtCall[i]->SetEnable(false);
				}	
			}
			m_pStcTime[i]->SetRefresh(TRUE);
			// 设置按钮
			if (pPet->state.bisCalled)
			{
				m_pBtCall[i]->SetText(g_StrTable[_T("PetButtonRest")]);
				m_pStcIsCalled[i]->SetText(g_StrTable[_T("Pet_State_Calling")]);
			}
			else
			{
				m_pBtCall[i]->SetText(g_StrTable[_T("PetButtonCall")]);
				m_pStcIsCalled[i]->SetText(g_StrTable[_T("Pet_State_Resting")]);
			}
			m_pBtCall[i]->SetEnable(true);
			m_pBtCall[i]->FlipToTop();
		}//for
		for(int i=0; i<MAX_PET_NUM; i++)
		{
			if(m_nCurrPage*MAX_PET_NUM + i >= PetManager::Inst()->GetMaxPetsCount())
			{
				m_pStcNick[i]->SetText(g_StrTable[_T("PetBarUnusable")]);
				m_pStcNick[i]->SetRefresh(TRUE);
			}
		}
		//页码
		tstringstream tss;
		tss << m_nCurrPage + 1;
		m_pStcPage->SetText(tss.str().c_str());
		m_pStcPage->SetRefresh(TRUE);
	} 
	//乘骑分页
	else if (Click_Rider_Page == m_eCurClickPage)
	{
		// 高亮当前选择
		if (m_dwCurrSelectPetID1 != GT_INVALID)
		{
			m_pStcOption1[m_nSelectRow1]->SetPicFile(SELECT_ICON_PATH);

		}
		for ( int i = 0; i < MAX_PET_NUM; i++)
		{
			if(m_nCurrPage1*MAX_PET_NUM + i >= m_iCurRiderNum1) 
				break;
			//找到对应宠物
			map<INT, DWORD>::const_iterator it = m_mapRow2Pet1.find(i);
			dwCurID = GT_INVALID;
			if (it != m_mapRow2Pet1.end())
				dwCurID = it->second;
			// 填充数据
			const tagPet* pPet = PetManager::Inst()->GetPet(dwCurID);
			if (NULL == pPet)
			{
				ClearUIData();
				continue;
			}
			const tagPetProto* pPetProto = PetProtoData::Inst()->FindPetProto(pPet->dwTypeID);
			if (!P_VALID(pPetProto))
			{
				ClearUIData();
				continue;
			}
			if (PetManager::Inst()->IsPetRiding(pPet->dwID))
			{
				m_pStcIcon1[it->first]->SetPicFile(pPetProto->strIconPath[EPMT_Mount]);
			}
			else
			{
				m_pStcIcon1[it->first]->SetPicFile(pPetProto->strIconPath[EPMT_Follow]);
			}
			//m_pStcIcon1[i]->SetPicFile(pPetProto->strIconPath[EPMT_Mount]);			
			m_pStcNick1[i]->SetText(pPet->strName.c_str());
			m_pStcIsBinding1[i]->SetInvisible(!pPetProto->bBinding);
			if ( pPetProto->nLifeTime == -1 )
			{
				m_pStcTime1[i]->SetText(g_StrTable[_T("Pet_UnDead")]);
				m_pStcTime1[i]->SetTextColor(0xFF00FF00);
			}
			else
			{
				tstring tstrLifeTime;
				BOOL bdeath = ID2RestTime(dwCurID, &tstrLifeTime);
				m_pStcTime1[i]->SetText(tstrLifeTime.c_str());
				if (bdeath == FALSE)
				{
					m_pStcTime1[i]->SetTextColor(0xFF00FF00);
				}
				else
				{
					m_pStcTime1[i]->SetTextColor(0xFFFF0000);
					m_pBtRide1[i]->SetText(g_StrTable[_T("Pet_State_Prepare")]);
					m_pBtRide1[i]->SetEnable(false);
				}
			}
			m_pStcTime1[i]->SetRefresh(TRUE);
			// 设置按钮
			if (pPet->state.bisPreparing)
			{
				m_pBtRide1[i]->SetText(g_StrTable[_T("Pet_State_Cancel")]);
				m_pStcIsPreparing1[i]->SetText(g_StrTable[_T("Pet_State_Preparing")]);
			}
			else
			{
				m_pBtRide1[i]->SetText(g_StrTable[_T("Pet_State_Prepare")]);
				m_pStcIsPreparing1[i]->SetText(g_StrTable[_T("Pet_State_Resting")]);
			}
			m_pBtRide1[i]->SetEnable(true);
			m_pBtRide1[i]->FlipToTop();
		}//for
		for(int i=0; i<MAX_PET_NUM; i++)
		{
			if(m_nCurrPage1*MAX_PET_NUM + i >= PetManager::Inst()->GetMaxRiderCount())
			{
				m_pStcNick1[i]->SetText(g_StrTable[_T("PetBarUnusable")]);
				m_pStcNick1[i]->SetRefresh(TRUE);
			}
		}
		/*const tagPet* pRidingPet = PetManager::Inst()->GetRidingPet();
		if (pRidingPet != NULL)
		{
			m_pBtRide1[i]->SetEnable(false);
		}*/
		//页码
		tstringstream tss;
		tss << m_nCurrPage1 + 1;
		m_pStcPage1->SetText(tss.str().c_str());
		m_pStcPage1->SetRefresh(TRUE);
	}

    // 公共按钮设置
	DWORD dwCurSelectID = GT_INVALID;
	if ( Click_Pet_Page == m_eCurClickPage )  	//找到对应宠物或乘骑		
	{
		dwCurSelectID = m_dwCurrSelectPetID;
	} 
	else if (Click_Rider_Page == m_eCurClickPage)
	{
		dwCurSelectID = m_dwCurrSelectPetID1;
	}
	if (dwCurSelectID == GT_INVALID)
	{
		ClearUIData();
	}
	else
	{
		const tagPet* pPet = PetManager::Inst()->GetPet(dwCurSelectID);
		if (NULL == pPet)
		{
			ClearUIData();
			return;
		}
		const tagPetProto* pPetProto = PetProtoData::Inst()->FindPetProto(pPet->dwTypeID);
		if (!P_VALID(pPetProto))
		{
			ClearUIData();
			return;
		}
		if (pPet->nAttrA[ECSPA_bLocked] || pPetProto->bBinding)
		{
			m_pBtTrade->SetEnable(false);
		}
		else
		{
			m_pBtTrade->SetEnable(true);
		}

		if (pPet->nAttrA[ECSPA_bLocked]||PetManager::Inst()->IsPetRiding(dwCurSelectID))
		{
			m_pBtSetFree->SetEnable(false);
		}
		else
		{
			m_pBtSetFree->SetEnable(true);
		}
	}
}

BOOL PetFrame::ID2RestTime(DWORD dwID,tstring *refTstr)
{	
	m_bCurrPetDead  = FALSE;
	m_bCurrPetDead1 = FALSE;
	map<DWORD,tagDWORDTime>::const_iterator ite = PetManager::Inst()->GetPetLifeMap().find(dwID);
	if (ite != PetManager::Inst()->GetPetLifeMap().end())
	{
		tagDWORDTime time = GetCurrentDWORDTime();
		tagDWORDTime tagTime = ite->second;
		//时间还没有到
		if( 1 == CompareDwordTime(tagTime,time))
		{
			DWORD LifeLastTime= CalcTimeDiff(ite->second,time);
			LifeLastTime = LifeLastTime/3600;//转换为小时数
			INT day		=	LifeLastTime/24;
			INT hour	=	LifeLastTime%24;
			tstringstream LifeTime;
			LifeTime << day << g_StrTable[_T("HanziZhuanhuan_tian")] << hour << g_StrTable[_T("HanziZhuanhuan_xiaoshi")];
			(*refTstr) = LifeTime.str();
		}
		//时间已经到达，但是还没有收到服务器通知
		else
		{
			tstringstream LifeTime;
			LifeTime << g_StrTable[_T("HanziZhuanhuan_1xiaoshi")];
			(*refTstr) = LifeTime.str();
		}
		return FALSE;
	}
	else//死亡
	{
		tstringstream LifeTime;
		LifeTime << g_StrTable[_T("HanziZhuanhuan_0xiaoshi")];
		(*refTstr) = LifeTime.str();
		//休息

		DWORD dwPetTypeID = PetManager::Inst()->GetPet(dwID)->dwTypeID;
		const tagPetProto* pPetSingleProto = PetProtoData::Inst()->FindPetProto( dwPetTypeID );
		if (!P_VALID(pPetSingleProto))
		{
			return FALSE;
		}
		//宠物（type3=6）
		if ( pPetSingleProto->type3 == 6 )  
		{
			 PetManager::Inst()->BeginSetPetState(dwID, EPS_Called, !PetManager::Inst()->IsPetCalled(dwID));
			 m_bCurrPetDead  = TRUE;
			 return m_bCurrPetDead;
		} 
		//乘骑
		else
		{
			 PetManager::Inst()->BeginSetPetState(dwID, EPS_Called,	   !PetManager::Inst()->IsPetCalled(dwID));
			 PetManager::Inst()->BeginSetPetState(dwID, EPS_Preparing, !PetManager::Inst()->IsPetPreparing(dwID));
			 m_bCurrPetDead1 = TRUE;
			 return m_bCurrPetDead1;
		}
	}	
	return FALSE;
}

VOID PetFrame::RefreshPetList()
{
  
	UpdateData();
	ClearUI();
	RefreshPetShow();
	RefreshToUI();
	RefreshSkill();
}

VOID PetFrame::UpdateData()
{
	// 清除
	m_mapRow2Pet.clear();
	m_mapRow2Pet1.clear();
	// 填充
	const PetManager::_id2pet* pPetList = PetManager::Inst()->GetPetList();
	if ( !P_VALID(pPetList) )
	{
		return;
	}
	int i = 0, j = 0;
	m_iCurPetNum = 0 , m_iCurRiderNum1 = 0;
	for (PetManager::_id2pet::const_iterator it = pPetList->begin();
		it != pPetList->end(); ++it)
	{
		DWORD dwPetTypeID = it->second->dwTypeID;
		const tagPetProto* pPetSingleProto = PetProtoData::Inst()->FindPetProto( dwPetTypeID );
		if (!P_VALID(pPetSingleProto))
		{
			continue;
		}
		//宠物（type3=6）
		if ( pPetSingleProto->type3 == 6 )  
		{
			if (m_iCurPetNum >= m_nCurrPage * MAX_PET_NUM && m_iCurPetNum <(1+m_nCurrPage) * MAX_PET_NUM)
			{
				if (i < MAX_PET_NUM)
				{
					m_mapRow2Pet.insert(make_pair(i, it->second->dwID));
					++i;
				}
			}
			++m_iCurPetNum;
		} 
		//乘骑
		else
		{
			if (m_iCurRiderNum1 >= m_nCurrPage1 * MAX_PET_NUM && m_iCurRiderNum1 <(1+m_nCurrPage1) * MAX_PET_NUM)
			{
				if (j < MAX_PET_NUM)
				{
					m_mapRow2Pet1.insert(make_pair(j, it->second->dwID));
					++j;
				}
			}
			++m_iCurRiderNum1;
		}
	}
}

VOID PetFrame::ClearUI()
{
    ClearUIData();

    for (int i = 0; i != MAX_PET_NUM; ++i)
    {
		m_pStcOption[i]->SetPicFile(SELECT_DIBAN);
		m_pStcIcon[i]->SetPicFile(SELECT_DIBAN);
		m_pStcNick[i]->SetText(_T(""));
		m_pStcIsCalled[i]->SetText(_T(""));
		m_pBtCall[i]->SetText(_T(""));
		m_pStcIsBinding[i]->SetInvisible(TRUE);
		m_pStcTime[i]->SetText(_T(""));
	}
	for (int i = 0; i != MAX_PET_NUM; ++i)
	{
		m_pStcOption1[i]->SetPicFile(SELECT_DIBAN);
		m_pStcIcon1[i]->SetPicFile(SELECT_DIBAN);
		m_pStcNick1[i]->SetText(_T(""));
		m_pStcIsPreparing1[i]->SetText(_T(""));
		m_pBtRide1[i]->SetText(_T(""));
		m_pStcIsBinding1[i]->SetInvisible(TRUE);
		m_pStcTime1[i]->SetText(_T(""));
	}
}

VOID PetFrame::ClearUIData()
{
	if ( Click_Pet_Page == m_eCurClickPage )
	{
		// 技能
		for (int i = 0; i != MAX_PET_SKILL; ++i)
		{
			m_pBtSkillA[i]->RefrashSkill(GT_INVALID, GT_INVALID);
		}
		// 文本框
		m_pStcPetShow->SetPetShow(GT_INVALID);
		tagPetCameraUI event(_T("PetCameraUI"), NULL);
		event.dwPetID = GT_INVALID;
		m_pFrameMgr->SendEvent(&event);
		//公共按钮
		if ( m_dwCurrSelectPetID == GT_INVALID )
		{
			m_pBtSetFree->SetEnable(false);
			m_pBtTrade->SetEnable(false);
			m_pBtChangeName->SetEnable(false);
			m_pBtCallBack->SetEnable(false);
		}
		else
		{
			m_pBtSetFree->SetEnable(true);
			m_pBtTrade->SetEnable(true);
			m_pBtChangeName->SetEnable(true);
			m_pBtCallBack->SetEnable(true);
		}
	}
	else if ( Click_Rider_Page == m_eCurClickPage )
	{
		// 技能
		for (int i = 0; i != MAX_PET_SKILL; ++i)
		{
			m_pBtSkillA1[i]->RefrashSkill(GT_INVALID, GT_INVALID);
		}
		// 文本框
		m_pStcPetShow1->SetPetShow(GT_INVALID);
		//公共按钮
		if ( m_dwCurrSelectPetID1 == GT_INVALID )
		{
			m_pBtSetFree->SetEnable(false);
			m_pBtTrade->SetEnable(false);
			m_pBtChangeName->SetEnable(false);
			m_pBtCallBack->SetEnable(false);
		}
		else
		{
			m_pBtSetFree->SetEnable(true);
			m_pBtTrade->SetEnable(true);
			m_pBtChangeName->SetEnable(true);
			m_pBtCallBack->SetEnable(true);
		}
	}
}

VOID PetFrame::ClearPetInfo()
{
	//宠物分页(pet)
	m_dwCurrSelectPetID = GT_INVALID;   
	m_nSelectRow = 0;          
	m_mapRow2Pet.clear();            
	m_nCurrPage = 0;             
	m_bCurrPetDead = FALSE;			 
	m_iCurPetNum = 0;				 
	m_iCurPetGridNum = MAX_PET_NUM;			 
	//乘骑分页(pet1)
	m_dwCurrSelectPetID1 = GT_INVALID;      
	m_nSelectRow1 = 0;           
	m_mapRow2Pet1.clear();           
	m_nCurrPage1 = 0;            
	m_bCurrPetDead1 = FALSE;			  
	m_iCurRiderNum1 = 0;			 
	m_iCurRiderGridNum = MAX_PET_NUM;	
}

DWORD PetFrame::OnUsePetEquipHandler( tagUsePetEquip* pEvent )
{
    DWORD CurrPetID = PetManager::Inst()->GetCurrPetID();

    // is there an active pet ?
    if (GT_INVALID == CurrPetID)
    {
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PetErrorMsg_14")]);
        return 0;
    }

    // try to equip
    PetManager::Inst()->BeginNC_PetEquip(CurrPetID, GT_INVALID, pEvent->n64ItemID);

    return 0;
}

DWORD PetFrame::OnPetAttrChange( tagPetAttrChange* pEvent )
{
    switch (pEvent->eType)
    {
    case EPACT_Init:
        RefreshPetList();
    	break;
    case EPACT_Remove:
		 {
			if ( Click_Pet_Page == m_eCurClickPage )
			{
				UpdateData();
				m_nSelectRow  = 0;
				map<INT, DWORD>::const_iterator it = m_mapRow2Pet.find(0);
				if (it != m_mapRow2Pet.end())
					m_dwCurrSelectPetID = it->second;
				else
					m_dwCurrSelectPetID = GT_INVALID;
			} 
			else if ( Click_Rider_Page == m_eCurClickPage )
			{
				UpdateData();
				m_nSelectRow1 = 0;
				map<INT, DWORD>::const_iterator it = m_mapRow2Pet1.find(0);
				if (it != m_mapRow2Pet1.end())
					m_dwCurrSelectPetID1 = it->second;
				else
					m_dwCurrSelectPetID1 = GT_INVALID;
			}
			RefreshPetList();
		 }
        break;
    /*case EPACT_PetValue:
        RefreshPetList();
        RefreshToUI();
        break;*/
    case EPACT_PetState:
        RefreshPetList();
        break;
    //case EPACT_PetDisp:
    //    RefreshPetList();
    //    RefreshToUI();
    //    break;
    //case EPACT_PetEquip:
    //    RefreshToUI();
    //    //RefreshEquip();
    //    break;
    //case EPACT_PetUnEquip:
    //    //RefreshEquip();
    //    RefreshPetList();
    //    break;
    case EPACT_PetSkill:
        RefreshSkill();
        break;
    }

    return 0;
}
//
//VOID PetFrame::RefreshEquip()
//{
//    // no pet selected
//    if (GT_INVALID == m_dwCurrSelectPetID)
//    {
//        ClearUIData();
//        return;
//    }
//
//    // fill equipment
//    for (int i = 0; i < MAX_PET_EQUIP; ++i)
//    {
//        const tagPetEquip* petEquip = PetManager::Inst()->FindPetEquip(m_dwCurrSelectPetID, (INT8)i);
//        if (P_VALID(petEquip))
//        {
//            m_pBtEquipA[i]->RefreshItem(petEquip->dwItemProtoID);
//        }
//        else
//        {
//            m_pBtEquipA[i]->RefreshItem();
//        }
//    }
//}

VOID PetFrame::Render3D()
{
	if ( Click_Pet_Page == m_eCurClickPage )
	{
		m_pStcPetShow->Render3D();
	}
	else if ( Click_Rider_Page == m_eCurClickPage )
	{
		m_pStcPetShow1->Render3D();
	}
    GameFrame::Render3D();
}

VOID PetFrame::RefreshPetShow()
{
	if ( Click_Pet_Page == m_eCurClickPage )
	{
		if (m_dwCurrSelectPetID != GT_INVALID)
		{
			const tagPet* pet = PetManager::Inst()->GetPet(m_dwCurrSelectPetID);
			if (P_VALID(pet))
			{
				m_pStcPetShow->SetPetShow(pet->dwTypeID,EPMT_Follow);
				tagPetCameraUI event(_T("PetCameraUI"), NULL);
				event.dwPetID = pet->dwTypeID;
				m_pFrameMgr->SendEvent(&event);
				return;
			}
			else
			{
				m_pStcPetShow->SetPetShow(GT_INVALID);
			}
		}
		else
		{
			m_pStcPetShow->SetPetShow(GT_INVALID);
		}
	}
	else if ( Click_Rider_Page == m_eCurClickPage )
	{
		if (m_dwCurrSelectPetID1 != GT_INVALID)
		{
			const tagPet* pet = PetManager::Inst()->GetPet(m_dwCurrSelectPetID1);
			if (P_VALID(pet))
			{
				m_pStcPetShow1->SetPetShow(pet->dwTypeID,EPMT_Mount);
				tagPetCameraUI event(_T("PetCameraUI"), NULL);
				event.dwPetID = pet->dwTypeID;
				m_pFrameMgr->SendEvent(&event);
				return;
			}
			else
			{
				m_pStcPetShow1->SetPetShow(GT_INVALID);
			}
		}
		else
		{
			m_pStcPetShow1->SetPetShow(GT_INVALID);
		}
	}
	tagPetCameraUI event(_T("PetCameraUI"), NULL);
	event.dwPetID = GT_INVALID;
	m_pFrameMgr->SendEvent(&event);
}

VOID PetFrame::RefreshSkill()
{
	if ( Click_Pet_Page == m_eCurClickPage )
	{
		// no pet selected
		if (GT_INVALID == m_dwCurrSelectPetID)
		{
			ClearUIData();
			return;
		}
		// clear skill container
		for (int i = 0; i != MAX_PET_SKILL; ++i)
			m_pBtSkillA[i]->RefrashSkill(GT_INVALID, GT_INVALID);
		// fill skill
		int i = 0;
		const tagPet* pPet = PetManager::Inst()->GetPet(m_dwCurrSelectPetID);
		if (P_VALID(pPet))
		{
			for (tagPet::_id2skillInfo::const_iterator it = pPet->Skill.begin();
				it != pPet->Skill.end(); ++it)
			{
				m_pBtSkillA[i]->RefrashSkill(m_dwCurrSelectPetID, it->second->dwTypeID);
				//if (m_bCurrPetDead)
				//{
				//	//m_pBtSkillB[i]->SetPicColor(0xBBFFFFFF);
				//	m_pBtSkillA[i]->SetEnable(false);
				//}else{
				//	//m_pBtSkillB[i]->SetPicColor(0x00FFFFFF);
				//	m_pBtSkillA[i]->SetEnable(true);
				//}
				++i;
			}
		}
	}
	else if ( Click_Rider_Page == m_eCurClickPage )
	{
		// no pet selected
		if (GT_INVALID == m_dwCurrSelectPetID1)
		{
			ClearUIData();
			return;
		}
		// clear skill container
		for (int i = 0; i != MAX_PET_SKILL; ++i)
			m_pBtSkillA1[i]->RefrashSkill(GT_INVALID, GT_INVALID);
		// fill skill
		int i = 0;
		const tagPet* pPet = PetManager::Inst()->GetPet(m_dwCurrSelectPetID1);
		if (P_VALID(pPet))
		{
			for (tagPet::_id2skillInfo::const_iterator it = pPet->Skill.begin();
				it != pPet->Skill.end(); ++it)
			{
				m_pBtSkillA1[i]->RefrashSkill(m_dwCurrSelectPetID1, it->second->dwTypeID);
				/*if (m_bCurrPetDead1)
				{
					m_pBtSkillA1[i]->SetEnable(false);
				}else{
					m_pBtSkillA1[i]->SetEnable(true);
				}*/
				++i;
			}
		}
	}
}

DWORD PetFrame::OnCommitDeletePet( tagMsgBoxEvent* pEvent )
{
    if (MBF_OK == pEvent->eResult)
    {
		if (Click_Pet_Page == m_eCurClickPage)
		{
			PetManager::Inst()->BeginNC_DeletePet(m_dwCurrSelectPetID);
		}
		else if (Click_Rider_Page == m_eCurClickPage)
		{
			PetManager::Inst()->BeginNC_DeletePet(m_dwCurrSelectPetID1);
		}
    }

    return 0;
}

DWORD PetFrame::OnCommitCallBackPet( tagMsgBoxEvent* pEvent )
{
	if (MBF_OK == pEvent->eResult)
	{
		if (Click_Pet_Page == m_eCurClickPage)
		{
			ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
			if(pContainer->GetFirstItemByTypeID(PET_RECALL_ITEM_ID)!=NULL)
			{
				PetManager::Inst()->BeginNC_CallBackPet(m_dwCurrSelectPetID);
			}
			else
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PetCallBackItemLack")]);
				return 0;

			}
			
		}
		else if (Click_Rider_Page == m_eCurClickPage)
		{
			ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
			if(pContainer->GetFirstItemByTypeID(PET_RECALL_ITEM_ID)!=NULL)
			{
				PetManager::Inst()->BeginNC_CallBackPet(m_dwCurrSelectPetID1);
			}
			else
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PetCallBackItemLack")]);
				return 0;

			}
		}
	}

	return 0;
}

bool PetFrame::IsWindowVisible()
{
    if (P_VALID(m_pWnd) && !m_pWnd->IsInvisible())
    {
        return true;
    }
    else
    {
        return false;
    }
}
//*************************F计划中废除的功能接口*************************
tstring PetFrame::GetLevelText( int nLevel )
{
    // 获得等级
    int step, grade;
    TransVLevel2StepGrade(nLevel, step, grade);

    TCHAR szTxt[256];
    _sntprintf(szTxt, sizeof(szTxt) / sizeof(TCHAR), g_StrTable[_T("PetLevel")], step, grade);

    return szTxt;
}

DWORD PetFrame::OnCommitPourExp( tagMsgBoxEvent* pEvent )
{
    if (MBF_OK == pEvent->eResult)
    {
        // 判断等级
        const tagPet* pPet = PetManager::Inst()->GetPet(m_dwCurrSelectPetID);
        if (P_VALID(pPet))
        {
            int nStep, nGrade;
            TransVLevel2StepGrade(pPet->nAttrA[ECSPA_nLevel], nStep, nGrade);
            if (pPet->nAttrA[ECSPA_nExpCurrent] == pPet->nAttrA[ECSPA_nExpLevelUp])
            {
                CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PetLveelFull")]);
            }
            else
            {
                PetManager::Inst()->BeginPourExp(m_dwCurrSelectPetID);
            }
        }
    }

    return 0;
}

LPCTSTR PetFrame::GetQualityText( int nQuality )
{
    if (nQuality < 0 || nQuality >= 5)
        return _T("");
    tstringstream sstream;
    sstream << _T("PetQuality_") << nQuality;
    return g_StrTable[sstream.str().c_str()];
}
//************************************************************************
DWORD PetFrame::OnCommitPetName( tagMsgInputBoxEvent* pEvent )
{
    if (MBF_OK == pEvent->eResult && MBIT_Name == pEvent->eInputType)
    {
		DWORD dwCurrPetID = GT_INVALID;
		if (Click_Pet_Page == m_eCurClickPage)
		{
			dwCurrPetID = m_dwCurrSelectPetID;
		}
		else if(Click_Rider_Page == m_eCurClickPage)
		{
			dwCurrPetID = m_dwCurrSelectPetID1;
		}
		  
		if (GT_INVALID == dwCurrPetID)
			return 0;

		if( _tcslen(pEvent->strInputName.c_str()) == 0 )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Common_Error31")]);
		}
		else if (FilterData::Inst()->IsValidName(pEvent->strInputName.c_str(), X_SHORT_NAME) != E_Success)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PetInfo02")]);
		}
		else if (pEvent->strInputName.length() > X_PET_NAME)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PetInfo05")]);
		}
		else
		{
			tagNC_PetRename     msg;
			msg.dwPetID     =   dwCurrPetID;
			_tcscpy_s(msg.szPetName, sizeof(msg.szPetName) / sizeof(TCHAR), pEvent->strInputName.c_str());
			m_pSession->Send(&msg);
		}
    }

    return 0;
}

DWORD PetFrame::OnOpen_PetByID( tagOpenPetbyID* pMsg )
{
	DWORD dwPetID = pMsg->dwPetID;
	const tagPet* pPet = PetManager::Inst()->GetPet(dwPetID);
	if (!P_VALID(pPet))
		return 0;

	//计算新宠所在页数
	const PetManager::_id2pet* pPetList = PetManager::Inst()->GetPetList();
	if ( !P_VALID(pPetList) )
	{
		return 0;
	}
	INT i=0; //宠物计数
	INT j=0; //骑乘计数
	PetManager::_id2pet::const_iterator it = pPetList->begin();
	for ( ; it != pPetList->end(); ++it )
	{
		DWORD dwTypeID = it->second->dwTypeID;
		DWORD dwID = it->second->dwID;
		const tagPetProto* pPetSingleProto = PetProtoData::Inst()->FindPetProto( dwTypeID );
		if (!P_VALID(pPetSingleProto))
			continue;
		if ( pPetSingleProto->type3 == 6 )  
		{	
			if ( dwID == dwPetID )
			{
				m_nCurrPage = (INT)i/MAX_PET_NUM;
				m_nSelectRow = (INT)i%MAX_PET_NUM;
				m_dwCurrSelectPetID = dwPetID;
				m_eCurClickPage = Click_Pet_Page;
				m_pBtPetPage->SetState(EGUIBS_PushDown,TRUE);
				RefreshPetList();
				ShowWnd(TRUE);
				break;
			}
			++i;
		} 
		//乘骑
		else
		{	
			if ( dwID == dwPetID )
			{
				m_nCurrPage1 = (INT)j/MAX_PET_NUM;
				m_nSelectRow1 = (INT)j%MAX_PET_NUM;
				m_dwCurrSelectPetID1 = dwPetID;
				m_eCurClickPage = Click_Rider_Page;
				m_pBtRiderPage->SetState(EGUIBS_PushDown,TRUE);
				RefreshPetList();
				ShowWnd(TRUE);
				break;
			}
			++j;
		}
	}//for
    return 0;
}

DWORD PetFrame::OnNS_PetRename( tagNS_PetRename* pMsg, DWORD )
{
	if (E_Success != pMsg->dwErrCode)
		PetManager::Inst()->ShowErrorMessage(pMsg->dwErrCode);
	RefreshPetList();
	return 0;
}

DWORD PetFrame::OnEvent_Refresh_PetUI(tagGameEvent* pEvent)
{
	RefreshToUI();
	return 0;
}

DWORD PetFrame::OnMaxNumChanged()
{
	
	if ( Click_Pet_Page == m_bCurExtendMutex && m_iCurPetNum < MAX_PET_RIDER_TOTAL_NUM )
	{
		m_iCurPetGridNum++;
	} 
	else if ( Click_Rider_Page == m_bCurExtendMutex && m_iCurRiderNum1 < MAX_PET_RIDER_TOTAL_NUM )
	{
		m_iCurRiderGridNum++;
	}
	return 0;
}

DWORD PetFrame::OnPetDispInfo(tagPetDispChange* pEvent )
{
	RefreshPetList();
	return 0;
}

DWORD  PetFrame::OnMsgBoxClickEvent(tagMsgBoxEvent* pGameEvent)
{
	if( MBF_OK == pGameEvent->eResult )
	{
		ItemContainer *pPocket = ItemMgr::Inst()->GetPocket();
		INT64 dw64ID  = GT_INVALID;
		if (P_VALID(pPocket))
		{
			dw64ID = pPocket->GetSpecFuncItemID(EISF_PetPocketExtItem);
		}
		if (Click_Pet_Page == m_eCurClickPage)
		{
			tagNC_ExtendPetPocket msg;
			msg.n64ItemID = dw64ID;
			msg.ePetType = PET_FOLLOWING;
			m_pSession->Send(&msg);
		}
		else if (Click_Rider_Page == m_eCurClickPage)
		{
			tagNC_ExtendPetPocket msg;
			msg.n64ItemID = dw64ID;
			msg.ePetType = PET_RIDING;
			m_pSession->Send(&msg);
		}	
		m_pWnd->SetUnClickable(FALSE);
	}
	else if( MBF_Cancel == pGameEvent->eResult )
	{
		m_pWnd->SetUnClickable(FALSE);
	}
	return 0;
}

VOID PetFrame::PageChangeRefresh()
{
	if (Click_Pet_Page == m_eCurClickPage)
	{
		 m_dwCurrSelectPetID = GT_INVALID;
		 m_nSelectRow = 0;
	}
	else if (Click_Rider_Page == m_eCurClickPage)
	{
		 m_dwCurrSelectPetID1 = GT_INVALID;
		 m_nSelectRow1 = 0;
	}	
}