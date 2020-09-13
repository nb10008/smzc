#include "stdafx.h"
#include "wx/wx.h"
#include "wx/busyinfo.h"
#include "wx/panel.h"
#include "wx/bookctrl.h"
#include "wx/listbox.h"
#include "wx/checklst.h"
#include "GMTool.h"
#include "common.h"
#include "GuildPage.h"
#include "mylistctrl.h"
#include "resource.h"
#include "UserFrameMgr.h"
#include "DataMgr.h"
#include "NetSessionMgr.h"
#include "../ServerDefine/msg_gm_tool_c.h"

BEGIN_EVENT_TABLE(GuildPage, wxPanel)
	//EVT_BUTTON(GuildPage_OkChgMemLvl,			GuildPage::OnGuildChgMemLvl)
	EVT_BUTTON(GuildPage_OkChgGuildMoney,		GuildPage::OnGuildChgMoney)
	EVT_BUTTON(GuildPage_OkChgGuildStat,		GuildPage::OnGuildChgState)
END_EVENT_TABLE()

GuildPage::GuildPage(wxBookCtrlBase *book, const wxString& label):m_label(label), wxPanel(book, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
																						  wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN | wxTAB_TRAVERSAL)
{
	//巨大的构造函数
	wxSizer* sizerAll = new wxBoxSizer(wxVERTICAL);

	wxTextValidator validator(wxFILTER_NUMERIC); 

	// 修改成员等级
// 	wxStaticBox *box = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("GuildChgMemLvl")]);
// 	wxSizer* sizerBox = new wxStaticBoxSizer(box, wxHORIZONTAL);
// 
// 	wxControl *control1 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("GuildName")]);
// 	wxControl *control2 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("RoleName")]);
// 	wxControl *control3 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("GuildRoleLvl")]);
// 	m_text_guildname1 = new wxTextCtrl(this, wxID_ANY);
// 	m_text_guildrolename = new wxTextCtrl(this, wxID_ANY);
// 	m_text_guildrolelvl = new wxTextCtrl(this, wxID_ANY);
// 	sizerBox->Add(control1, 0, wxALL, CONTROL_BORDER);
// 	sizerBox->Add(m_text_guildname1, 0, wxALL, CONTROL_BORDER);
// 	sizerBox->Add(control2, 0, wxALL, CONTROL_BORDER);
// 	sizerBox->Add(m_text_guildrolename, 0, wxALL, CONTROL_BORDER);
// 	sizerBox->Add(control3, 0, wxALL, CONTROL_BORDER);
// 	sizerBox->Add(m_text_guildrolelvl, 0, wxALL, CONTROL_BORDER);
// 	m_btn_ok1 = new wxButton(this, GuildPage_OkChgMemLvl, g_StrTable[_T("OkModify")]);
// 	sizerBox->Add(m_btn_ok1, 0, wxALL, CONTROL_BORDER);


	// 修改帮派金钱
	wxStaticBox *boxMoney = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("GuildChgMoney")]);
	wxSizer* sizerBoxMoney = new wxStaticBoxSizer(boxMoney, wxHORIZONTAL);

	wxControl *control4 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("GuildName")]);
	wxControl *control5 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("GuildMoney")]);
	m_text_guildname2 = new wxTextCtrl(this, wxID_ANY);
	m_text_guildmoney = new wxTextCtrl(this, wxID_ANY);
	sizerBoxMoney->Add(control4, 0, wxALL, CONTROL_BORDER);
	sizerBoxMoney->Add(m_text_guildname2, 0, wxALL, CONTROL_BORDER);
	sizerBoxMoney->Add(control5, 0, wxALL, CONTROL_BORDER);
	sizerBoxMoney->Add(m_text_guildmoney, 0, wxALL, CONTROL_BORDER);
	m_btn_ok2 = new wxButton(this, GuildPage_OkChgMemLvl, g_StrTable[_T("OkModify")]);
	sizerBoxMoney->Add(m_btn_ok2, 0, wxALL, CONTROL_BORDER);

	// 修改帮派状态
	wxStaticBox *boxState = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("GuildChgState")]);
	wxSizer* sizerBoxState = new wxStaticBoxSizer(boxState, wxHORIZONTAL);

	wxControl *control6 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("GuildName")]);
	wxControl *control7 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("GuildState")]);
	m_text_guildname3 = new wxTextCtrl(this, wxID_ANY);
	m_text_guildstate = new wxTextCtrl(this, wxID_ANY);
	sizerBoxState->Add(control6, 0, wxALL, CONTROL_BORDER);
	sizerBoxState->Add(m_text_guildname3, 0, wxALL, CONTROL_BORDER);
	sizerBoxState->Add(control7, 0, wxALL, CONTROL_BORDER);
	sizerBoxState->Add(m_text_guildstate, 0, wxALL, CONTROL_BORDER);
	m_btn_ok3 = new wxButton(this, GuildPage_OkChgMemLvl, g_StrTable[_T("OkModify")]);
	sizerBoxState->Add(m_btn_ok3, 0, wxALL, CONTROL_BORDER);

	// 整体组装
	//sizerAll->Add(sizerBox,			0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBoxMoney,	0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBoxState,	0, wxGROW|wxALL, CONTROL_BORDER);

	SetSizer(sizerAll);
	sizerAll->Fit(this);
}

// void GuildPage::OnGuildChgMemLvl(wxCommandEvent& event)
// {
// 	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_GuildChgMemLvl")) )
// 	{
// 		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
// 		return;
// 	}
// 
// 	tstring strGuildName = m_text_guildname1->GetValue();
// 	tstring strRoleName = m_text_guildrolename->GetValue();
// 	tstring strLvl = m_text_guildrolelvl->GetValue();
// 
// 	wxCheckListBox* pCheckList = m_pMyFrame->GetCheckListBox();
// 	if( P_VALID(pCheckList) )
// 	{
// 		std::vector<int> vec;
// 		m_pMyFrame->GetCheckListBoxChecked(vec);
// 		if( (int)vec.size()<=0 )
// 		{
// 			wxMessageBox(g_StrTable[_T("NoSelectWorld")], wxT(""), wxOK|wxCENTRE, this);
// 			return;
// 		}
// 
// 		if( (int)vec.size()>1 )
// 		{
// 			wxMessageBox(g_StrTable[_T("MorethanWorld")], wxT(""), wxOK|wxCENTRE, this);
// 			return;
// 		}
// 
// 		//获得所选游戏世界ID
// 		std::map<DWORD, tagSWIdEx> mapIDEx;
// 		std::map<DWORD, tagSWIdEx>::iterator itEx;
// 		std::vector<int>::iterator it = vec.begin();
// 		for( ; it!=vec.end(); it++ )
// 		{
// 			tstring strName = pCheckList->GetString(unsigned int(*it));
// 			tagSWId id;
// 			sDataMgrPtr->GetSectionAndWorldID(strName, &id);
// 			if( id.section_id!=GT_INVALID && id.world_id!=GT_INVALID )
// 			{
// 				tagNGMTC_GuildChgMemLvl msg;
// 				msg.dwSectionID = id.section_id;
// 				msg.dwWorldID = id.world_id;
// 				_sntprintf( msg.szGuildName, X_SHORT_NAME, strGuildName.c_str() );
// 				_sntprintf( msg.szRoleName, X_SHORT_NAME, strRoleName.c_str() );
// 				msg.nLvl = _tstoi(strLvl.c_str());
// 				
// 				sNetSessionMgrPtr->SendMsg( &msg, msg.dwSize );
// 			}
// 		}
// 	}	
// }

void GuildPage::OnGuildChgMoney(wxCommandEvent& event)
{
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_GuildChgFund")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	tstring strGuildName = m_text_guildname2->GetValue();
	tstring strFund = m_text_guildmoney->GetValue();

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
				tagNGMTC_GuildChgFund msg;
				msg.dwSectionID = id.section_id;
				msg.dwWorldID = id.world_id;
				_sntprintf( msg.szGuildName, X_SHORT_NAME, strGuildName.c_str() );
				msg.n32Fund = _tstoi(strFund.c_str());

				sNetSessionMgrPtr->SendMsg( &msg, msg.dwSize );
			}
		}
	}	
}

void GuildPage::OnGuildChgState(wxCommandEvent& event)
{
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_GuildChgState")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	tstring strGuildName = m_text_guildname1->GetValue();
	tstring strState = m_text_guildstate->GetValue();

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
				tagNGMTC_GuildChgState msg;
				msg.dwSectionID = id.section_id;
				msg.dwWorldID = id.world_id;
				_sntprintf( msg.szGuildName, X_SHORT_NAME, strGuildName.c_str() );
				msg.dwState = _tstoi(strState.c_str());

				sNetSessionMgrPtr->SendMsg( &msg, msg.dwSize );
			}
		}
	}	
}