// MapCloudDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MapEditor.h"
#include "MapCloudDlg.h"
#include <tchar.h>

// MapCloudDlg dialog

IMPLEMENT_DYNAMIC(MapCloudDlg, CDialog)

MapCloudDlg::MapCloudDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MapCloudDlg::IDD, pParent)
{

}

MapCloudDlg::~MapCloudDlg()
{
}

void MapCloudDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LAYERS, m_LayerNames);
    DDX_Control(pDX, IDC_X, m_X);
    DDX_Control(pDX, IDC_Y, m_Y);
    DDX_Control(pDX, IDC_Z, m_Z);
    DDX_Control(pDX, IDC_FREQ, m_Freq);
    DDX_Control(pDX, IDC_SCALE, m_Scale);
}


BEGIN_MESSAGE_MAP(MapCloudDlg, CDialog)
    ON_BN_CLICKED(IDC_ADD, &MapCloudDlg::OnBnClickedAdd)
    ON_LBN_SELCHANGE(IDC_LAYERS, &MapCloudDlg::OnLbnSelchangeLayers)
    ON_BN_CLICKED(IDC_DEL, &MapCloudDlg::OnBnClickedDel)
    ON_BN_CLICKED(IDC_APPLY, &MapCloudDlg::OnBnClickedApply)
END_MESSAGE_MAP()


// MapCloudDlg message handlers

void MapCloudDlg::OnBnClickedAdd()
{
    // TODO: Add your control notification handler code here
    CFileDialog dlg(true,_T("fsm"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("*.fsm|*.fsm|*.*|*.*||"), this);
    if(IDOK == dlg.DoModal())
    {
        CString filename = dlg.GetPathName();
        filename.MakeLower();
        CString szAppPath=Kernel::Inst()->GetWorkPath();
        szAppPath.MakeLower();
        int num=filename.Replace(szAppPath,_T(""));
        if(num==0)
        {
            AfxMessageBox(_T("转换成相对路径失败!"));
            return;
        }

        int index = m_LayerNames.GetCurSel();
        index = index==-1?m_LayerNames.GetCount():index;
        m_LayerNames.InsertString(index, filename);
        int count = index;
        tagMapCloudLayer layer;
        _tcscpy(layer.szMdlPath, filename.GetString());
        list<tagMapCloudLayer>::iterator iter = m_cloudLayers.begin();
        while(count>0&&iter!=m_cloudLayers.end())
        {
         --count;
         ++iter;
        }
        m_cloudLayers.insert(iter, layer);
    }
}

BOOL MapCloudDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    for(list<tagMapCloudLayer>::iterator iter = m_cloudLayers.begin();
        iter != m_cloudLayers.end();
        ++iter)
        m_LayerNames.InsertString(m_LayerNames.GetCount(), iter->szMdlPath);
    return TRUE;
}


void MapCloudDlg::OnLbnSelchangeLayers()
{
    int index = m_LayerNames.GetCurSel();
    if(index!=-1)
    {
        list<tagMapCloudLayer>::iterator iter = m_cloudLayers.begin();
        while(index>0&&iter!=m_cloudLayers.end())
        {
            --index;
            ++iter;
        }
        if(iter!=m_cloudLayers.end())
        {
            CString buffer;
            buffer.Format(_T("%f"), iter->fAxis[0]);
            SetDlgItemText(IDC_X, buffer);
            buffer.Format(_T("%f"), iter->fAxis[1]);
            SetDlgItemText(IDC_Y, buffer);
            buffer.Format(_T("%f"), iter->fAxis[2]);
            SetDlgItemText(IDC_Z, buffer);
            buffer.Format(_T("%f"), iter->fFrequency);
            SetDlgItemText(IDC_FREQ, buffer);
            buffer.Format(_T("%f"), iter->fScale);
            SetDlgItemText(IDC_SCALE, buffer);
        }
    }
    else
    {
        SetDlgItemText(IDC_X, _T(""));
        SetDlgItemText(IDC_Y, _T(""));
        SetDlgItemText(IDC_Z, _T(""));
        SetDlgItemText(IDC_FREQ, _T(""));
        SetDlgItemText(IDC_SCALE, _T(""));
    }
}

void MapCloudDlg::OnBnClickedDel()
{
    int index = m_LayerNames.GetCurSel();
    if(index==-1)return;
    int count = index;
    m_LayerNames.DeleteString(index);
    list<tagMapCloudLayer>::iterator iter = m_cloudLayers.begin();
    while(count>0&&iter!=m_cloudLayers.end())
    {
        --count;
        ++iter;
    }
    m_cloudLayers.erase(iter);
}

void MapCloudDlg::OnBnClickedApply()
{
    int index = m_LayerNames.GetCurSel();
    if(index==-1)return;
    int count = index;
    list<tagMapCloudLayer>::iterator iter = m_cloudLayers.begin();
    while(count>0&&iter!=m_cloudLayers.end())
    {
        --count;
        ++iter;
    }
    CString buffer;
    GetDlgItemText(IDC_X, buffer);
    iter->fAxis[0] = (float)_wtof(buffer);
    GetDlgItemText(IDC_Y, buffer);
    iter->fAxis[1] = (float)_wtof(buffer);
    GetDlgItemText(IDC_Z, buffer);
    iter->fAxis[2] = (float)_wtof(buffer);
    GetDlgItemText(IDC_FREQ, buffer);
    iter->fFrequency = (float)_wtof(buffer);
    GetDlgItemText(IDC_SCALE, buffer);
    iter->fScale = (float)_wtof(buffer);
}
