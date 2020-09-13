#pragma once
#include "afxcmn.h"
#include "..\WorldDefine\MapAttDefine.h"
#include "afxwin.h"

// MapCloudDlg dialog

class MapCloudDlg : public CDialog
{
	DECLARE_DYNAMIC(MapCloudDlg)

public:
    virtual BOOL OnInitDialog();
	MapCloudDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MapCloudDlg();

// Dialog Data
	enum { IDD = IDD_CLOULD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedAdd();
    CListBox m_LayerNames;
    list<tagMapCloudLayer> m_cloudLayers;

    CEdit m_X;
    CEdit m_Y;
    CEdit m_Z;
    CEdit m_Freq;
    CEdit m_Scale;
    afx_msg void OnLbnSelchangeLayers();
    afx_msg void OnBnClickedDel();
    afx_msg void OnBnClickedApply();
};
