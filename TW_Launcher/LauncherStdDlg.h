#pragma once

#include "SemiTranDlg/TranslucentDialog.h"



// LauncherStdDlg dialog

class CLauncherStdDlg : public CTranslucentDialog
{
public:
	CLauncherStdDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLauncherStdDlg();

// Dialog Data
	enum { IDD = IDD_STANDARDDIALOG };


	virtual CStandardDialog* CreateStandardDialog();
	virtual void OnInitChildrenWnds();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	//消息处理
	virtual BOOL PreTranslateMessage(MSG* pMsg); 
	//消息处理
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
