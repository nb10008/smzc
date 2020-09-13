// SectionChangeToolDlg.h : header file
//

#pragma once

#include "tool.h"
#include "resource.h"
#include "afxwin.h"

class CSectionChangeToolDlg : public CDialog
{
// Construction
public:
	CSectionChangeToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SECTIONCHANGETOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
private:
	CTool*				m_pTool;
	TObjRef<Util>		m_pUtil;
	CString				m_XmlPath;
	LPCTSTR				m_szLineTemp;	// = NULL;	//Êä³ö
public:
//	afx_msg void OnLbnSelchangeList1();
	CListBox m_ProcessListBox;
	afx_msg void OnBnClickedButton2();
	VOID	Output(LPCTSTR szTmp)	{ m_ProcessListBox.AddString(szTmp); }
	CEdit m_XmlPathEdit;
};

extern CSectionChangeToolDlg* g_pDlg;
