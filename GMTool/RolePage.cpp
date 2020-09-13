#include "stdafx.h"
#include "wx/wx.h"
#include "wx/busyinfo.h"
#include "wx/panel.h"
#include "wx/bookctrl.h"
#include "wx/listbox.h"
#include "wx/checklst.h"
#include "GMTool.h"
#include "common.h"
#include "RolePage.h"
#include "mylistctrl.h"
#include "resource.h"
#include "UserFrameMgr.h"
#include "DataMgr.h"
#include "NetSessionMgr.h"
#include "ItemProtoData.h"
#include "..\ServerDefine\msg_gm_tool_c.h"
#define ITEM_MAX ItemProtoData::Inst()->GetItemNum()

BEGIN_EVENT_TABLE(RolePage, wxPanel)
	EVT_BUTTON(RolePage_OkResume,		RolePage::OnRoleResumeOperation)
	EVT_BUTTON(RolePage_OkModiyPro,		RolePage::OnRoleProModifyOperation)
	EVT_BUTTON(RolePage_OkItemPro,		RolePage::OnRoleItemProOperation)
	EVT_BUTTON(RolePage_OkItemDel,		RolePage::OnRoleItemDelOperation)
	EVT_BUTTON(RolePage_OKItemResume,	RolePage::OnRoleItemResOperation)
	EVT_BUTTON(RolePage_OkClearBaibao,	RolePage::OnRoleClearBaibaoOperation)
	EVT_BUTTON(RolePage_OKTitleIssue,	RolePage::OnRoleTitleIssue)
	EVT_BUTTON(RolePage_OKTitleDel,		RolePage::OnRoleTitleDel)
	EVT_BUTTON(RolePage_OKMoveRole,		RolePage::OnRoleGetMove)
	EVT_BUTTON(RolePage_OKClearAttPoint,RolePage::OnClearAttPoint)
	EVT_BUTTON(RolePage_AddSpeakOff,	RolePage::OnRoleSpeakOff)
	EVT_BUTTON(RolePage_DelSpeakOff,	RolePage::OnRoleSpeakOn)
	EVT_BUTTON(RolePage_NeedPKLog,		RolePage::OnRoleAddPKLog)
END_EVENT_TABLE()

RolePage::RolePage( wxBookCtrlBase *book, const wxString& label ):m_label(label), wxPanel(book, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
																						  wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN | wxTAB_TRAVERSAL)
{
	//巨大的构造函数
	wxSizer* sizerAll = new wxBoxSizer(wxVERTICAL);
	
	wxTextValidator validator(wxFILTER_NUMERIC); 

	// 角色恢复
	wxStaticBox *box = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("RoleResume")]);
	wxSizer* sizerBox = new wxStaticBoxSizer(box, wxHORIZONTAL);

	wxControl *control1 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("RoleOldName")]);
	wxControl *control2 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("RoleNewName")]);
	m_text_delrole = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, validator);
	m_text_rolename = new wxTextCtrl(this, wxID_ANY);
	sizerBox->Add(control1, 0, wxALL, CONTROL_BORDER);
	sizerBox->Add(m_text_delrole, 0, wxALL, CONTROL_BORDER);
	sizerBox->Add(control2, 0, wxALL, CONTROL_BORDER);
	sizerBox->Add(m_text_rolename, 0, wxALL, CONTROL_BORDER);
	m_btn_okresume = new wxButton(this, RolePage_OkResume, g_StrTable[_T("OkResume")]);
	sizerBox->Add(m_btn_okresume, 0, wxALL, CONTROL_BORDER);


	// 角色修改
	wxStaticBox *boxMin = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("ProModify")]);
	wxSizer* sizerBoxMin = new wxStaticBoxSizer(boxMin, wxHORIZONTAL);

	wxControl *control3 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("RoleName")]);
	m_text_prorole =  new wxTextCtrl(this, wxID_ANY);
	wxControl *control4 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("Level")]);
	m_text_level =  new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, validator);
	wxControl *control5 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("Exp")]);
	m_text_exp = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, validator);
	wxControl *control6 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("SkillNum")]);
	m_text_skillnum = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, validator);
	wxControl *control9 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("AttPoint")]);
	m_text_attpoint = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, validator);
	wxControl *control7 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("Money")]);
	m_text_money = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, validator);
	wxControl *control8 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("Yuanbao")]);
	m_text_yuanbao = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, validator);
	sizerBoxMin->Add(control3, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(m_text_prorole, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(control4, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(m_text_level, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(control5, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(m_text_exp, 0, wxALL, CONTROL_BORDER);
	
	sizerBoxMin->Add(control6, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(m_text_skillnum, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(control7, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(m_text_money, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(control8, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(m_text_yuanbao, 0, wxALL, CONTROL_BORDER);

	sizerBoxMin->Add(control9, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(m_text_attpoint, 0, wxALL, CONTROL_BORDER);
	m_btn_okmodipro = new wxButton(this, RolePage_OkModiyPro, g_StrTable[_T("OkModify")]);
	m_btn_clearattpoint = new wxButton(this, RolePage_OKClearAttPoint, g_StrTable[_T("OkClearAtt")]);
	sizerBoxMin->Add(m_btn_okmodipro, 0, wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(m_btn_clearattpoint, 0, wxALL, CONTROL_BORDER);

	//布局-道具生成
	wxStaticBox *boxBom = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("ItemProduce")]);
	wxSizer* sizerBoxBom = new wxStaticBoxSizer(boxBom, wxHORIZONTAL);
	
	wxControl *contro9 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("RoleName")]);
	wxControl *contro10 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("ItemProduce1")]);
	wxControl *contro11 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("ItemQuality")]);
	m_text_itemrole =  new wxTextCtrl(this, wxID_ANY);
	m_text_num =  new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, validator);
	m_text_quality =  new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, validator);
	m_btn_okitem = new wxButton(this, RolePage_OkItemPro, g_StrTable[_T("OkItemProduce")]);

	int nItemNum = ItemProtoData::Inst()->GetItemNum();
	std::map<DWORD, tagItem2GM>& item = ItemProtoData::Inst()->GetItem();
	std::map<DWORD, tagItem2GM>::iterator it = item.begin();
	unsigned int i=0;
	wxString strins[10000];
	for( ; it!=item.end() && i<10000; ++it )
	{
		strins[i] = it->second.szName;
		++i;
	}
	
	m_combox_item = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, nItemNum, strins);

	sizerBoxBom->Add(contro9, 0, wxALL, CONTROL_BORDER);
	sizerBoxBom->Add(m_text_itemrole, 0, wxALL, CONTROL_BORDER);
	sizerBoxBom->Add(m_combox_item, 0, wxALL, CONTROL_BORDER);
	sizerBoxBom->Add(contro10, 0, wxALL, CONTROL_BORDER);
	sizerBoxBom->Add(m_text_num, 0, wxALL, CONTROL_BORDER);
	sizerBoxBom->Add(contro11, 0, wxALL, CONTROL_BORDER);
	sizerBoxBom->Add(m_text_quality, 0, wxALL, CONTROL_BORDER);
	sizerBoxBom->Add(m_btn_okitem, 0, wxALL, CONTROL_BORDER);

	//布局-道具删除
	wxStaticBox *boxBom1 = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("ItemDelete")]);
	wxSizer* sizerBoxBom1 = new wxStaticBoxSizer(boxBom1, wxHORIZONTAL);

	wxControl *contro1Del1 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("RoleName")]);
	m_text_itemownername =  new wxTextCtrl(this, wxID_ANY, wxT(""));
	wxControl *contro1Del2 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("ItemGlobalID")]);
	m_text_Item64IDStr =new wxTextCtrl(this, wxID_ANY, wxT(""),  wxDefaultPosition, wxSize(500,18), 0, validator);
	m_btn_okitemdel = new wxButton(this, RolePage_OkItemDel, g_StrTable[_T("OkItemDelete")]);
	
	sizerBoxBom1->Add(contro1Del1, 0, wxALL, CONTROL_BORDER);
	sizerBoxBom1->Add(m_text_itemownername, 0, wxALL, CONTROL_BORDER);
	sizerBoxBom1->Add(contro1Del2, 0, wxALL, CONTROL_BORDER);
	sizerBoxBom1->Add(m_text_Item64IDStr, 1, wxGROW|wxALL, CONTROL_BORDER);
	sizerBoxBom1->Add(m_btn_okitemdel, 0, wxALL, CONTROL_BORDER);

	// 清空百宝袋
	wxStaticBox *boxClearBaibao = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("ClearBaibao")]);
	wxSizer* sizerClearBaibao = new wxStaticBoxSizer(boxClearBaibao, wxHORIZONTAL);

	wxControl *controlDel3 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("RoleName")]);
	m_text_baibaoownername =  new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_btn_clearbaibao = new wxButton(this, RolePage_OkClearBaibao, g_StrTable[_T("ClearBaibao")]);

	sizerClearBaibao->Add(controlDel3, 0, wxALL, CONTROL_BORDER);
	sizerClearBaibao->Add(m_text_baibaoownername, 0, wxALL, CONTROL_BORDER);
	sizerClearBaibao->Add(m_btn_clearbaibao, 0, wxALL, CONTROL_BORDER);

	//布局-道具恢复
	wxStaticBox *boxItemResume = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("ItemResume")]);
	wxSizer* sizerBoxItemResume = new wxStaticBoxSizer(boxItemResume, wxHORIZONTAL);

	wxControl *controlItemResume1 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("ItemSerialNum")]);
	wxControl *controlItemResume2 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("TargrtRoleName")]);
	
	m_btn_okitemresume = new wxButton(this, RolePage_OKItemResume, g_StrTable[_T("OkItemResume")]);
	m_text_resumeitemidstr = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(500,18), 0, validator);
	m_text_targetrolename = new wxTextCtrl(this, wxID_ANY);
	
	sizerBoxItemResume->Add(controlItemResume1, 0, wxALL, CONTROL_BORDER);
	sizerBoxItemResume->Add(m_text_resumeitemidstr, 1, wxALL | wxGROW, CONTROL_BORDER);
	sizerBoxItemResume->Add(controlItemResume2, 0, wxALL, CONTROL_BORDER);
	sizerBoxItemResume->Add(m_text_targetrolename, 0, wxALL, CONTROL_BORDER);
	sizerBoxItemResume->Add(m_btn_okitemresume, 0, wxALL, CONTROL_BORDER);


	//布局-称号发放
	wxStaticBox *boxTitle = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("TitleIssue")]);
	wxSizer* sizerBoxTitle = new wxStaticBoxSizer(boxTitle, wxHORIZONTAL);

	wxControl *controltitle1 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("RoleName")]);
	wxControl *controltitle2 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("TitleID")]);
	
	m_text_title_rolename = new wxTextCtrl(this, wxID_ANY);
	m_text_title_titleid = new wxTextCtrl(this, wxID_ANY);
	m_btn_oktitle = new wxButton(this, RolePage_OKTitleIssue, g_StrTable[_T("TitleDone")]);
	m_btn_deltitle = new wxButton(this, RolePage_OKTitleDel, g_StrTable[_T("DelTitleDone")]);

	sizerBoxTitle->Add(controltitle1, 0, wxALL, CONTROL_BORDER);
	sizerBoxTitle->Add(m_text_title_rolename, 0, wxALL, CONTROL_BORDER);
	sizerBoxTitle->Add(controltitle2, 0, wxALL, CONTROL_BORDER);
	sizerBoxTitle->Add(m_text_title_titleid, 0, wxALL, CONTROL_BORDER);
	sizerBoxTitle->Add(m_btn_oktitle, 0, wxALL, CONTROL_BORDER);
	sizerBoxTitle->Add(m_btn_deltitle, 0, wxALL, CONTROL_BORDER);

	// 移动角色
 	wxStaticBox *boxMoveRole = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("MoveRole")]);
 	wxSizer* sizerBoxMoveRole = new wxStaticBoxSizer(boxMoveRole, wxHORIZONTAL);
 
 	wxControl *controlRoleName = new wxStaticText(this, wxID_ANY, g_StrTable[_T("RoleName")]);
	wxControl *controlMapName = new wxStaticText(this, wxID_ANY, g_StrTable[_T("MapName")]);
 	wxControl *controlX = new wxStaticText(this, wxID_ANY, g_StrTable[_T("X")]);
	wxControl *controlZ = new wxStaticText(this, wxID_ANY, g_StrTable[_T("Z")]);
 
 	m_text_moverole_rolename = new wxTextCtrl(this, wxID_ANY);
	m_text_moverole_map = new wxTextCtrl(this, wxID_ANY);
 	m_text_moverole_x = new wxTextCtrl(this, wxID_ANY);
 	m_text_moverole_z = new wxTextCtrl(this, wxID_ANY);
 	m_btn_moverole_ok = new wxButton(this, RolePage_OKMoveRole, g_StrTable[_T("MoveRoleDone")]);
 
 	sizerBoxMoveRole->Add(controlRoleName,			0, wxALL, CONTROL_BORDER);
 	sizerBoxMoveRole->Add(m_text_moverole_rolename, 0, wxALL, CONTROL_BORDER);
	sizerBoxMoveRole->Add(controlMapName,			0, wxALL, CONTROL_BORDER);
	sizerBoxMoveRole->Add(m_text_moverole_map,		0, wxALL, CONTROL_BORDER);
 	sizerBoxMoveRole->Add(controlX,					0, wxALL, CONTROL_BORDER);
 	sizerBoxMoveRole->Add(m_text_moverole_x,		0, wxALL, CONTROL_BORDER);
 	sizerBoxMoveRole->Add(controlZ,					0, wxALL, CONTROL_BORDER);
 	sizerBoxMoveRole->Add(m_text_moverole_z,		0, wxALL, CONTROL_BORDER);
	sizerBoxMoveRole->Add(m_btn_moverole_ok,		0, wxALL, CONTROL_BORDER);

	// 角色禁言
	wxStaticBox *boxSpeakOff = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("SpeakOff")]);
	wxSizer* sizerBoxSpeakOff = new wxStaticBoxSizer(boxSpeakOff, wxHORIZONTAL);

	sizerBoxSpeakOff->Add(CreateSizerWithTextAndLabel(this, g_StrTable[_T("RoleName")], wxID_ANY, &m_text_RoleName),
		0,  wxALL, CONTROL_BORDER);
	m_add_SpeakOff = new wxButton(this, RolePage_AddSpeakOff, g_StrTable[_T("AddSpeakOff")]);
	m_del_SpeakOff = new wxButton(this, RolePage_DelSpeakOff, g_StrTable[_T("DelSpeakOff")]);
	sizerBoxSpeakOff->Add(m_add_SpeakOff, 0, wxALL, CONTROL_BORDER);
	sizerBoxSpeakOff->Add(m_del_SpeakOff, 0, wxALL, CONTROL_BORDER);

	// 角色需要记PK log
	wxStaticBox *boxPKLog = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("PKlog")]);
	wxSizer* sizerBoxPKLog = new wxStaticBoxSizer(boxPKLog, wxHORIZONTAL);

	sizerBoxPKLog->Add(CreateSizerWithTextAndLabel(this, g_StrTable[_T("RoleID")], wxID_ANY, &m_text_PKlogRoleID),
		0,  wxALL, CONTROL_BORDER);
	sizerBoxPKLog->Add(CreateSizerWithTextAndLabel(this, g_StrTable[_T("RoleName")], wxID_ANY, &m_text_PKlogRoleName),
		0,  wxALL, CONTROL_BORDER);
	m_btn_AddPKLog = new wxButton(this, RolePage_NeedPKLog, g_StrTable[_T("AddPKLog")]);
	sizerBoxPKLog->Add(m_btn_AddPKLog, 0, wxALL, CONTROL_BORDER);

	// 整体拼装
	sizerAll->Add(sizerBox,			0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBoxMin,		0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBoxBom,		0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBoxBom1,		0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerClearBaibao, 0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBoxItemResume, 0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBoxTitle,	0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBoxMoveRole, 0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBoxSpeakOff, 0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBoxPKLog,	0, wxGROW|wxALL, CONTROL_BORDER);

	SetSizer(sizerAll);
	sizerAll->Fit(this);

	sUserFrameMgrPgr->RegisterEventHandle(_T("tagRoleResume"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnRolePageEvent));
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagRoleProModify"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnRolePageEvent));
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagItemCreate"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnRolePageEvent));
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagItemDelete"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnRolePageEvent));
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagItemResume"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnRolePageEvent));
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagTitleIssue"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnRolePageEvent));
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagTitleDel"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnRolePageEvent));
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagMoveRole"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnRolePageEvent));
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagRoleSpeak"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnRoleSpeakEvent));
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagRolePKLog"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnRoleSpeakEvent));
}

void RolePage::OnRoleResumeOperation( wxCommandEvent& event )
{
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_RoleResume")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}
	tstring strOldName = m_text_delrole->GetValue();
	tstring strNewName = m_text_rolename->GetValue();

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));
			tagSWId id;
			sDataMgrPtr->GetSectionAndWorldID(strName, &id);
			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				tagNGMTC_RoleResume msg;
				msg.nOldID = _tstoi(strOldName.c_str());
				_sntprintf( msg.szNewName, X_SHORT_NAME, strNewName.c_str() );
				msg.dwSectionID = id.section_id;
				msg.dwWorldID = id.world_id;
				sNetSessionMgrPtr->SendMsg( &msg, msg.dwSize );
			}
		}
	}	
}

void RolePage::OnRoleProModifyOperation( wxCommandEvent& event )
{

	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_ProModify")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	tstring strRoleName = m_text_prorole->GetValue();

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strLel = m_text_level->GetValue().c_str();
		tstring strSkill = m_text_skillnum->GetValue().c_str();
		tstring strExp = m_text_exp->GetValue().c_str();
		tstring strMoney = m_text_money->GetValue().c_str();
		tstring strYuan = m_text_yuanbao->GetValue().c_str();
		tstring strAttPoint = m_text_attpoint->GetValue().c_str();

		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));
			tagSWId id;
			sDataMgrPtr->GetSectionAndWorldID(strName, &id);
			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				tagNGMTC_ProModify msg;
				_sntprintf( msg.szName, X_SHORT_NAME, strRoleName.c_str() );
				if(strLel.empty())
					msg.nLel = GT_INVALID;
				else
					msg.nLel = _tstoi( strLel.c_str() );
				if(strSkill.empty())
					msg.nSkill = GT_INVALID;
				else
					msg.nSkill = _tstoi( strSkill.c_str() );
				if(strExp.empty())
					msg.n64Exp = GT_INVALID;
				else
					msg.n64Exp = _tstol( strExp.c_str() );
				if(strMoney.empty())
					msg.n64Money = GT_INVALID;
				else
					msg.n64Money = _tstol( strMoney.c_str() );
				if(strYuan.empty())
					msg.nYuanbao = GT_INVALID;
				else
					msg.nYuanbao = _tstoi( strYuan.c_str() );
				if (strAttPoint.empty())
					msg.nAttPoint = GT_INVALID;
				else
					msg.nAttPoint = _tstoi(strAttPoint.c_str());
				msg.dwSectionID = id.section_id;
				msg.dwWorldID = id.world_id;
				sNetSessionMgrPtr->SendMsg( &msg, msg.dwSize );
			}
		}
	}		
}

DWORD RolePage::OnRolePageEvent( tagUserEvent* pGameEvent )
{
	if( pGameEvent->strEventName == _T("tagRoleResume") )
	{
		tagRoleResume* pEvt = (tagRoleResume*)pGameEvent;
		switch( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				wxMessageBox(g_StrTable[_T("RoleResumeMsg0")], wxT(""), wxOK|wxCENTRE, this);
				m_text_delrole->Clear();
				m_text_rolename->Clear();
			}
			break;
		case E_GMTool_ResumeRole_NameAlreadyExist:
			{
				wxMessageBox(g_StrTable[_T("RoleResumeMsg1")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_ResumeRole_RoleNotExist:
			{
				wxMessageBox(g_StrTable[_T("RoleResumeMsg2")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_ResumeRole_RoleNotReMove:
			{
				wxMessageBox(g_StrTable[_T("RoleResumeMsg4")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_Err:
		default:
			{
				wxMessageBox(g_StrTable[_T("RoleResumeMsg3")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		}
	}
	else if( pGameEvent->strEventName == _T("tagRoleProModify") )
	{
		tagRoleProModify* pEvt = (tagRoleProModify*)pGameEvent;
		switch( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				TCHAR szBuff[X_SHORT_STRING] = {0};
				_sntprintf( szBuff, X_SHORT_STRING, _T("RoleProModifyMsg%u"), pEvt->dwErrorCode );
				wxMessageBox(g_StrTable[szBuff], wxT(""), wxOK|wxCENTRE, this);
				m_text_prorole->Clear();
				m_text_level->Clear();
				m_text_exp->Clear();
				m_text_skillnum->Clear();
				m_text_money->Clear();
				m_text_yuanbao->Clear();
				m_text_attpoint->Clear();
			}
			break;
		case E_GMTool_ModifyRole_RoleNotExist:
			{
				wxMessageBox(g_StrTable[_T("RoleProModifyMsg1")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_Err:
			{
				wxMessageBox(g_StrTable[_T("RoleProModifyMsg2")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		
		}
	}
	else if( pGameEvent->strEventName == _T("tagItemCreate") )
	{
		tagItemCreate* pEvt = (tagItemCreate*)pGameEvent;
		switch ( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				wxMessageBox(g_StrTable[_T("ItemCreate0")], wxT(""), wxOK|wxCENTRE, this);
				m_text_itemrole->Clear();
				m_text_num->Clear();
				m_text_quality->Clear();
				m_combox_item->SetValue(wxT(""));
			}
			break;
		case E_GMTool_Err:
			{
				wxMessageBox(g_StrTable[_T("ItemCreate1")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_RoleNotExist:
			{
				wxMessageBox(g_StrTable[_T("RoleNoExist")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_CreateItem_EquipNumErr:
			{
				wxMessageBox(g_StrTable[_T("EquipNumErr")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_DeleteItem_ItemNotExist:
			{
				wxMessageBox(g_StrTable[_T("ItemNotExist")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_CreateItem_ItemTypeIDErr:
			{
				wxMessageBox(g_StrTable[_T("ItemTypeIDErr")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		}
	}
	else if( pGameEvent->strEventName == _T("tagItemDelete") )
	{	
		tagItemDelete* pEvt = (tagItemDelete*)pGameEvent;
		switch( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				wxMessageBox(g_StrTable[_T("ItemDelete0")], wxT(""), wxOK|wxCENTRE, this);
				m_text_itemownername->Clear();
				//m_text_itemdelid->Clear();
				m_text_Item64IDStr->Clear();
			}
			break;
		case E_GMTool_Err:
			{
				wxMessageBox(g_StrTable[_T("ItemDelete1")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_RoleNotExist:
			{
				wxMessageBox(g_StrTable[_T("RoleNoExist")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_DeleteItem_ItemNotExist:
			{
				wxMessageBox(g_StrTable[_T("ItemNotExist")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		}
	}
	else if( pGameEvent->strEventName == _T("tagItemResume") )
	{	
		tagItemResume* pEvt = (tagItemResume*)pGameEvent;
		switch( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				wxMessageBox(g_StrTable[_T("ItemResume0")], wxT(""), wxOK|wxCENTRE, this);
				//m_text_resumeitemid->Clear();
				m_text_resumeitemidstr->Clear();
				m_text_targetrolename->Clear();
			}
			break;
		case E_GMTool_Err:
			{
				wxMessageBox(g_StrTable[_T("ItemResume1")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_RoleNotExist:
			{
				wxMessageBox(g_StrTable[_T("RoleNoExist")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_ResumeItemNotExit:
			{
				wxMessageBox(g_StrTable[_T("ItemResume1")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_ResumeItemNotEquip:
			{
				wxMessageBox(g_StrTable[_T("ItemResume2")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		}
	}
	else if ( pGameEvent->strEventName == _T("tagTitleIssue") )
	{
		tagTitleIssue* pEvt = (tagTitleIssue*)pGameEvent;
		switch( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				wxMessageBox(g_StrTable[_T("SuspendOK")], wxT(""), wxOK|wxCENTRE, this);
				//m_text_resumeitemid->Clear();
				m_text_resumeitemidstr->Clear();
				m_text_targetrolename->Clear();
			}
			break;
		case E_GMTool_Err:
			{
				wxMessageBox(g_StrTable[_T("ErrorDone")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_RoleNotExist:
			{
				wxMessageBox(g_StrTable[_T("RoleNoExist")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		}
	}
	else if ( pGameEvent->strEventName == _T("tagTitleDel") )
	{
		tagTitleDel* pEvt = (tagTitleDel*)pGameEvent;
		switch( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				wxMessageBox(g_StrTable[_T("SuspendOK")], wxT(""), wxOK|wxCENTRE, this);
				//m_text_resumeitemid->Clear();
				m_text_resumeitemidstr->Clear();
				m_text_targetrolename->Clear();
			}
			break;
		case E_GMTool_Err:
			{
				wxMessageBox(g_StrTable[_T("ErrorDone")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_RoleNotExist:
			{
				wxMessageBox(g_StrTable[_T("RoleNoExist")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		}
	}
	else if ( pGameEvent->strEventName == _T("tagMoveRole") )
	{
		tagMoveRole* pEvt = (tagMoveRole*) pGameEvent;
		switch( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				wxMessageBox(g_StrTable[_T("MoveRoleSuccess")], wxT(""), wxOK|wxCENTRE, this);
				m_text_moverole_map->Clear();
				m_text_moverole_rolename->Clear();
				m_text_moverole_x->Clear();
				m_text_moverole_z->Clear();
			}
			break;
		case E_GMTool_RoleNotExist:
			{
				wxMessageBox(g_StrTable[_T("RoleNoExist")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_MoveRole_CoordinateErr:
			{
				wxMessageBox(g_StrTable[_T("MoveRoleCoordinateError")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_MoveRole_MapNameErr:
			{
				wxMessageBox(g_StrTable[_T("MoveRoleMapNameError")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_MoveRole_RoleOffLine:
			{
				wxMessageBox(g_StrTable[_T("MoveRoleRoleOffline")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		default:
			{
				wxMessageBox(g_StrTable[_T("ErrorDone")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		}
	}
	else if ( pGameEvent->strEventName == _T("tagRolePKLog"))
	{
		tagRolePKLog* pEvt = (tagRolePKLog*) pGameEvent;
		switch( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				wxMessageBox(g_StrTable[_T("SuspendOK")], wxT(""), wxOK|wxCENTRE, this);
				m_text_PKlogRoleID->Clear();
				m_text_PKlogRoleName->Clear();
			}
			break;
		case E_GMTool_AddPKLog_RoleInvalid:
			{
				wxMessageBox(g_StrTable[_T("RoleNoExist")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_AddPKLog_RoleOffline:
			{
				wxMessageBox(g_StrTable[_T("RoleNotOnline")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		}
	}

	return 0;
}

DWORD RolePage::OnRoleSpeakEvent(tagUserEvent* pGameEvent)
{
	if( pGameEvent->strEventName == _T("tagRoleSpeak") )
	{
		tagRoleSpeak* pEvt = (tagRoleSpeak*)pGameEvent;
		TCHAR szBuff[X_LONG_STRING] = {0};
		switch(pEvt->dwErrorCode)
		{
		case E_GMTool_Success:
			wxMessageBox(g_StrTable[_T("SuspendOK")], wxT(""), wxOK|wxCENTRE, this);
			break;
		case E_GMTool_RoleNotExist:
			wxMessageBox(g_StrTable[_T("RoleNoExist")], wxT(""), wxOK|wxCENTRE, this);
			break;
		default:
			break;
		}
	}

	return 0;
}

void RolePage::OnRoleItemProOperation( wxCommandEvent& event )
{
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_ItemCreate")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strItemName = m_combox_item->GetValue();
		const tagItem2GM* pItem = ItemProtoData::Inst()->FindItemProto(strItemName);
		if( !P_VALID(pItem) )
		{
			if (strItemName.empty())
			{
				wxMessageBox(g_StrTable[_T("NoSelectItem")], wxT(""), wxOK|wxCENTRE, this);
				return;
			}
		}

		tstring strRoleName = m_text_itemrole->GetValue();		
		tstring strItemNum = m_text_num->GetValue();
		tstring strItemQuality = m_text_quality->GetValue();		

		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));
			tagSWId id;
			sDataMgrPtr->GetSectionAndWorldID(strName, &id);
			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				tagNGMTC_ItemCreate msg;
				_sntprintf( msg.szName, X_SHORT_NAME, strRoleName.c_str() );
				
				{
					if (P_VALID(pItem))
					{
						msg.dwItemTypeID = pItem->dwTypeID;
					}
					else
					{
						msg.dwItemTypeID = _tstoi(strItemName.c_str());
					}				
				}

				msg.nItemNum = _tstoi(strItemNum.c_str());
				msg.byQuality = (BYTE)_tstoi(strItemQuality.c_str());
				msg.dwSectionID = id.section_id;
				msg.dwWorldID = id.world_id;
				sNetSessionMgrPtr->SendMsg( &msg, msg.dwSize );
			}
		}

	}
}

void RolePage::OnRoleItemDelOperation( wxCommandEvent& event )
{
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_ItemDelete")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strRoleName = m_text_itemownername->GetValue();
		//tstring strID = m_text_itemdelid->GetValue();
		tstring strItem64IDStr = m_text_Item64IDStr->GetValue();

		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));
			int nItemIDStringLen = strItem64IDStr.length();

			tagSWId id;
			sDataMgrPtr->GetSectionAndWorldID(strName, &id);
			
			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				int nMsgSize = 0;
				nMsgSize = sizeof(tagNGMTC_ItemDelete) - sizeof(TCHAR) + sizeof(TCHAR) * ( nItemIDStringLen + 1 );
				tagNGMTC_ItemDelete* pMsg = (tagNGMTC_ItemDelete*)malloc(nMsgSize);
				
				pMsg->dwID = Crc32("NGMTC_ItemDelete");
				pMsg->dwSize = nMsgSize;
				_sntprintf( pMsg->szName, X_SHORT_NAME, strRoleName.c_str() );
				_sntprintf( pMsg->sz64IDString, nItemIDStringLen * sizeof(TCHAR), strItem64IDStr.c_str() );
				pMsg->dwSectionID = id.section_id;
				pMsg->dwWorldID = id.world_id;
				sNetSessionMgrPtr->SendMsg( pMsg, nMsgSize );

				if (P_VALID(pMsg))
				{
					free(pMsg);
				}

			}
		}
	}

}

void RolePage::OnRoleItemResOperation( wxCommandEvent& event )
{
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_ItemResume")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strRoleName = m_text_targetrolename->GetValue();
		//tstring strItemID = m_text_resumeitemid->GetValue();
		tstring strItemIDStr = m_text_resumeitemidstr->GetValue();

		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));
			tagSWId id;

			sDataMgrPtr->GetSectionAndWorldID(strName, &id);

			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				int nMsgSize = sizeof(tagNGMTC_ItemResume) - sizeof(TCHAR) + sizeof(TCHAR)*(strItemIDStr.length()+1);
				tagNGMTC_ItemResume* pMsg = (tagNGMTC_ItemResume*)malloc(nMsgSize);

				pMsg->dwID = Crc32("NGMTC_ItemResume");
				pMsg->dwSize = nMsgSize;
				_sntprintf( pMsg->szTargetRole, X_SHORT_NAME, strRoleName.c_str() );
				//msg.n64SerialNum = _tstoi64(strItemID.c_str());
				_sntprintf( pMsg->sz64IDString, strItemIDStr.length()*sizeof(TCHAR), strItemIDStr.c_str());
				pMsg->dwSectionID = id.section_id;
				pMsg->dwWorldID = id.world_id;
				sNetSessionMgrPtr->SendMsg( pMsg, pMsg->dwSize );
			
				if (P_VALID(pMsg))
				{
					free(pMsg);
				}
			}
		}
	}

}

void RolePage::OnRoleClearBaibaoOperation(wxCommandEvent& event)
{

	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_ClearBaibao")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strRoleName = m_text_baibaoownername->GetValue();

		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));
			tagSWId id;

			sDataMgrPtr->GetSectionAndWorldID(strName, &id);

			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				tagNGMTC_ClearBaibao msg;
				_sntprintf( msg.szOwnerName, X_SHORT_NAME, strRoleName.c_str() );
				msg.dwSectionID = id.section_id;
				msg.dwWorldID = id.world_id;
				sNetSessionMgrPtr->SendMsg( &msg, msg.dwSize );
			}
		}
	}
}

void RolePage::OnRoleTitleIssue(wxCommandEvent& event)
{
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_IssueTitle")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strRoleName = m_text_title_rolename->GetValue();
		tstring strTitleID = m_text_title_titleid->GetValue();

		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));
			tagSWId id;

			sDataMgrPtr->GetSectionAndWorldID(strName, &id);

			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				// 分解角色名字串
				
				TCHAR szTmpStr[X_HUGE_STRING*X_HUGE_STRING] = {0};
				TCHAR* pStr = (TCHAR*)szTmpStr;
				_sntprintf( szTmpStr, X_HUGE_STRING*X_HUGE_STRING, strRoleName.c_str() );
				
				int i=0;
				int nLength = _tcslen(strRoleName.c_str());
				while(i<=nLength)
				{
					if (szTmpStr[i]==',' || szTmpStr[i]==0)
					{
						szTmpStr[i] = '\0';

						TCHAR szRoleName[X_SHORT_NAME] = {0};
						_tcscpy_s(szRoleName, X_SHORT_NAME, pStr);

						pStr = &(szTmpStr[i+1]);


						tagNGMTC_IssueTitle send;
						_tcscpy_s(send.szRoleName, X_SHORT_NAME, szRoleName);
						send.dwTitleID = _tstoi(strTitleID.c_str());
						send.dwSectionID = id.section_id;
						send.dwWorldID = id.world_id;
						sNetSessionMgrPtr->SendMsg(&send, send.dwSize);
					}
					++i;
				}
			}
		}
	}
}

void RolePage::OnRoleTitleDel(wxCommandEvent& event)
{
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_DelTitle")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strRoleName = m_text_title_rolename->GetValue();
		tstring strTitleID = m_text_title_titleid->GetValue();

		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));
			tagSWId id;

			sDataMgrPtr->GetSectionAndWorldID(strName, &id);

			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				// 分解角色名字串

				TCHAR szTmpStr[X_HUGE_STRING*X_HUGE_STRING] = {0};
				TCHAR* pStr = (TCHAR*)szTmpStr;
				_sntprintf( szTmpStr, X_HUGE_STRING*X_HUGE_STRING, strRoleName.c_str() );

				int i=0;
				int nLength = _tcslen(strRoleName.c_str());
				while(i<=nLength)
				{
					if (szTmpStr[i]==',' || szTmpStr[i]==0)
					{
						szTmpStr[i] = '\0';

						TCHAR szRoleName[X_SHORT_NAME] = {0};
						_tcscpy_s(szRoleName, X_SHORT_NAME, pStr);

						pStr = &(szTmpStr[i+1]);


						tagNGMTC_DelTitle send;
						_tcscpy_s(send.szRoleName, X_SHORT_NAME, szRoleName);
						send.dwTitleID = _tstoi(strTitleID.c_str());
						send.dwSectionID = id.section_id;
						send.dwWorldID = id.world_id;
						sNetSessionMgrPtr->SendMsg(&send, send.dwSize);
					}
					++i;
				}
			}
		}
	}
}

void RolePage::OnRoleGetMove(wxCommandEvent& event)
{
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_MoveRole")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strRoleName = m_text_moverole_rolename->GetValue();
		tstring strMapName = m_text_moverole_map->GetValue();
		tstring strX = m_text_moverole_x->GetValue();
		tstring strZ = m_text_moverole_z->GetValue();

		//获得所选游戏世界ID
		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));
			tagSWId id;
			sDataMgrPtr->GetSectionAndWorldID(strName, &id);
			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				tagNGMTC_MoveRole msg;
				msg.dwSectionID = id.section_id;
				msg.dwWorldID = id.world_id;
				_sntprintf( msg.szRoleName, X_SHORT_NAME, strRoleName.c_str() );
				_sntprintf( msg.szMapName, X_SHORT_NAME, strMapName.c_str() );
				msg.nX = _tstoi(strX.c_str());
				msg.nZ = _tstoi(strZ.c_str());
				sNetSessionMgrPtr->SendMsg(&msg, msg.dwSize);
			}
		}
	}
}

void RolePage::OnClearAttPoint(wxCommandEvent& event)
{

	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_ClearAttPoint")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	// 询问玩家是否确定这样做
	wxMessageDialog dialog( NULL, g_StrTable[_T("ConfirmClearAttPoint")], wxT(""), wxNO_DEFAULT|wxYES_NO|wxICON_INFORMATION);
	if( dialog.ShowModal() == wxID_NO) return;

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strRoleName = m_text_prorole->GetValue();

		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));
			tagSWId id;
			sDataMgrPtr->GetSectionAndWorldID(strName, &id);
			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				tagNGMTC_ClearAttPoint msg;
				msg.dwSectionID = id.section_id;
				msg.dwWorldID = id.world_id;
				_sntprintf( msg.szRoleName, X_SHORT_NAME, strRoleName.c_str() );
				sNetSessionMgrPtr->SendMsg(&msg, msg.dwSize);
			}
		}
	}
}

void RolePage::OnRoleSpeakOff(wxCommandEvent& event)
{
	// 检查权限
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_RoleSpeak")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strRoleName = m_text_RoleName->GetValue();

		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));

			tagSWId id;
			sDataMgrPtr->GetSectionAndWorldID(strName, &id);

			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				tagNGMTC_RoleSpeak send;
				send.dwSectionID = id.section_id;
				send.dwWorldID = id.world_id;
				_tcscpy_s(send.szRoleName, X_SHORT_NAME, strRoleName.c_str());
				send.bAction = 0;
				sNetSessionMgrPtr->SendMsg( &send, send.dwSize );
			}
		}
	}
}

void RolePage::OnRoleSpeakOn(wxCommandEvent& event)
{
	// 检查权限
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_RoleSpeak")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strRoleName = m_text_RoleName->GetValue();

		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));

			tagSWId id;
			sDataMgrPtr->GetSectionAndWorldID(strName, &id);

			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				tagNGMTC_RoleSpeak send;
				send.dwSectionID = id.section_id;
				send.dwWorldID = id.world_id;
				_tcscpy_s(send.szRoleName, X_SHORT_NAME, strRoleName.c_str());
				send.bAction = 1;
				sNetSessionMgrPtr->SendMsg( &send, send.dwSize );
			}
		}
	}
}

void RolePage::OnRoleAddPKLog(wxCommandEvent& event)
{
	// 检查权限
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_AddNeedPKLogRole")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
	if( P_VALID(pCheckList) )
	{
		std::vector<int> vec;
		m_pMyFrame->GetCheckListBoxChecked(vec);
		if( (int)vec.size()<=0 )
		{
			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		if( (int)vec.size()>1 )
		{
			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
			return;
		}

		tstring strRoleName = m_text_PKlogRoleName->GetValue();
		tstring strRoleID = m_text_PKlogRoleID->GetValue();

		//获得所选游戏世界ID
		std::map<DWORD, tagSWIdEx> mapIDEx;
		std::map<DWORD, tagSWIdEx>::iterator itEx;
		std::vector<int>::iterator it = vec.begin();
		for( ; it!=vec.end(); it++ )
		{
			tstring strName = pCheckList->GetString(unsigned int(*it));

			tagSWId id;
			sDataMgrPtr->GetSectionAndWorldID(strName, &id);

			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
			{
				tagNGMTC_AddNeedPKLogRole send;
				send.dwSectionID = id.section_id;
				send.dwWorldID = id.world_id;
				send.dwRoleID = _tstoi(strRoleID.c_str());
				_tcscpy_s(send.szRoleName, X_SHORT_NAME, strRoleName.c_str());
				sNetSessionMgrPtr->SendMsg( &send, send.dwSize );
			}
		}
	}
}