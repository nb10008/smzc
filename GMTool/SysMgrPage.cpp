#include "stdafx.h"
#include "wx/wx.h"
#include "wx/busyinfo.h"
#include "wx/panel.h"
#include "wx/bookctrl.h"
#include "wx/listbox.h"
#include "wx/checklst.h"
#include "GMTool.h"
#include "common.h"
#include "SysMgrPage.h"
#include "mylistctrl.h"
#include "resource.h"
#include "UserFrameMgr.h"
#include "DataMgr.h"
#include "NetSessionMgr.h"
#include "../ServerDefine/msg_gm_tool_c.h"

BEGIN_EVENT_TABLE(SysMgrPage, wxPanel)
EVT_BUTTON(SysMgrPage_OkAddFilter,			SysMgrPage::OnAddFilterWords)
EVT_BUTTON(SysMgrPage_OkDelFilter,		SysMgrPage::OnDelFilterWords)
END_EVENT_TABLE()

SysMgrPage::SysMgrPage(wxBookCtrlBase *book, const wxString& label):m_label(label),wxPanel(book, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
																						   wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN | wxTAB_TRAVERSAL)
{
	wxSizer* sizerAll = new wxBoxSizer(wxVERTICAL);

	wxTextValidator validator(wxFILTER_NUMERIC); 

	// 添加广告过滤词
	wxStaticBox *boxAdd = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("SysMgrAddFilter")]);
	wxSizer* sizerBoxAdd = new wxStaticBoxSizer(boxAdd, wxHORIZONTAL);

	wxControl *control1 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("FilterWords")]);
	m_Text_Add_Filter = new wxTextCtrl(this, wxID_ANY);	
	m_Text_Del_Filter = new wxTextCtrl(this, wxID_ANY);
	sizerBoxAdd->Add(control1, 0, wxALL, CONTROL_BORDER);
	sizerBoxAdd->Add(m_Text_Add_Filter, 0, wxALL, CONTROL_BORDER);
	
	m_Btn_Add_Ok = new wxButton(this, SysMgrPage_OkAddFilter, g_StrTable[_T("AddBlacklist")]);
	sizerBoxAdd->Add(m_Btn_Add_Ok, 0, wxALL, CONTROL_BORDER);

	// 删除广告过滤词
	wxStaticBox *boxDel = new wxStaticBox(this, wxID_ANY, g_StrTable[_T("SysMgrDelFilter")]);
	wxSizer* sizerBoxDel = new wxStaticBoxSizer(boxDel, wxHORIZONTAL);

	wxControl *control2 = new wxStaticText(this, wxID_ANY, g_StrTable[_T("FilterWords")]);
	sizerBoxDel->Add(control2, 0, wxALL, CONTROL_BORDER);
	sizerBoxDel->Add(m_Text_Del_Filter, 0, wxALL, CONTROL_BORDER);

	m_Btn_Del_Ok = new wxButton(this, SysMgrPage_OkDelFilter, g_StrTable[_T("DelFromBlack")]);
	sizerBoxDel->Add(m_Btn_Del_Ok, 0, wxALL, CONTROL_BORDER);

	//整体组装
	sizerAll->Add(sizerBoxAdd, 0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBoxDel, 0, wxGROW|wxALL, CONTROL_BORDER);

	SetSizer(sizerAll);
	sizerAll->Fit(this);

	sUserFrameMgrPgr->RegisterEventHandle(_T("tagAddFilterWords"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnSysMgrPageEvent));
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagDelFilterWords"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnSysMgrPageEvent));
}

void SysMgrPage::OnAddFilterWords(wxCommandEvent& event)
{
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_AddFilterWords")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	tstring strFilterWords = m_Text_Add_Filter->GetValue();
	if (strFilterWords.empty())
	{
		wxMessageBox(g_StrTable[_T("NoContent")], wxT(""), wxOK|wxCENTRE, this);
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
				tagNGMTC_AddFilterWords msg;
				msg.dwSectionID = id.section_id;
				msg.dwWorldID = id.world_id;
				_sntprintf( msg.szFilterWords, X_SHORT_NAME, strFilterWords.c_str() );				

				sNetSessionMgrPtr->SendMsg( &msg, msg.dwSize );
			}
		}
	}	

}
void SysMgrPage::OnDelFilterWords(wxCommandEvent& event)
{
	if( !sNetSessionMgrPtr->VerifyGMPrivilege(Crc32("NGMTC_DelFilterWords")) )
	{
		wxMessageBox(g_StrTable[_T("GMPrivilegeForbit")], wxT(""), wxOK|wxCENTRE, this);
		return;
	}

	tstring strFilterWords = m_Text_Del_Filter->GetValue();
	if (strFilterWords.empty())
	{
		wxMessageBox(g_StrTable[_T("NoContent")], wxT(""), wxOK|wxCENTRE, this);
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
				tagNGMTC_DelFilterWords msg;
				msg.dwSectionID = id.section_id;
				msg.dwWorldID = id.world_id;
				_sntprintf( msg.szFilterWords, X_SHORT_NAME, strFilterWords.c_str() );				

				sNetSessionMgrPtr->SendMsg( &msg, msg.dwSize );
			}
		}
	}	
}

DWORD SysMgrPage::OnSysMgrPageEvent(tagUserEvent* pGameEvent)
{
	if( pGameEvent->strEventName == _T("tagAddFilterWords") )
	{
		tagAddFilterWords* pEvt = (tagAddFilterWords*)pGameEvent;
		switch( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				wxMessageBox(g_StrTable[_T("FilterWords_AddSuc")], wxT(""), wxOK|wxCENTRE, this);
				m_Text_Add_Filter->Clear();
				
			}
			break;
		case E_GMTool_FilterWords_Existed:
			{
				wxMessageBox(g_StrTable[_T("FilterWords_AddExist")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_Err:
		default:
			{
				wxMessageBox(g_StrTable[_T("FilterWords_AddFail")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		}
	}
	else if( pGameEvent->strEventName == _T("tagDelFilterWords") )
	{
		tagDelFilterWords* pEvt = (tagDelFilterWords*)pGameEvent;
		switch( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				wxMessageBox(g_StrTable[_T("FilterWords_DelSuc")], wxT(""), wxOK|wxCENTRE, this);
				m_Text_Del_Filter->Clear();

			}
			break;
		case E_GMTool_FilterWords_NotExisted:
			{
				wxMessageBox(g_StrTable[_T("FilterWords_DelExist")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_Err:
		default:
			{
				wxMessageBox(g_StrTable[_T("FilterWords_DelFail")], wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		}
	}
	return 0;
}