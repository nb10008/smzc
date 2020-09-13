// AutoLoginMFCDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CAutoLoginMFCDlg dialog
class CAutoLoginMFCDlg : public CDialog
{
// Construction
public:
	CAutoLoginMFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AUTOLOGINMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	int GetProcessConut(const TCHAR* szExeName);
	bool m_bisOpen;

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_pButt;
};
