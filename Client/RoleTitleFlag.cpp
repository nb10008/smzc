#include "stdafx.h"
#include "RoleTitleFlag.h"

RoleTitleFlag::RoleTitleFlag( void )
{

}

RoleTitleFlag::~RoleTitleFlag( void )
{
	Destroy();
}

void RoleTitleFlag::Destroy()
{
	map<tstring, ResTexture*>::iterator it = m_mapRes.begin();
	while(it!=m_mapRes.end())
	{	
		IDraw2D::Inst()->ReleaseTexture(it->second);
		it = m_mapRes.erase(it);
	}
	m_mapRes.clear();

	map<tstring, ResTexture*>::iterator it1 = m_mapResTeam.begin();
	while(it1!=m_mapResTeam.end())
	{
		IDraw2D::Inst()->ReleaseTexture(it1->second);
		it1 = m_mapResTeam.erase(it1);
	}
	m_mapResTeam.clear();

    map<tstring, ResTexture*>::iterator it2 = m_mapResPicTitle.begin();
    while(it2!=m_mapResPicTitle.end())
    {
        IDraw2D::Inst()->ReleaseTexture(it2->second);
        it2 = m_mapResPicTitle.erase(it2);
    }
    m_mapResPicTitle.clear();

	map<tstring, ResTexture*>::iterator it3 = m_mapResPicGodJob.begin();
	while(it3!=m_mapResPicGodJob.end())
	{
		IDraw2D::Inst()->ReleaseTexture(it3->second);
		it3 = m_mapResPicGodJob.erase(it3);
	}
	m_mapResPicGodJob.clear();
}

void RoleTitleFlag::DrawRoleTitleFlag( RECT rc, float z, const TCHAR* szTitle )
{
	if( szTitle==NULL || _tcslen(szTitle)==0 )
		return;

	tstring strRes = szTitle;
	map<tstring, ResTexture*>::iterator it = m_mapRes.find(strRes);
	if( it!=m_mapRes.end() )
	{
		DrawRoleTitleFlag(rc, z, it->second);
	}
	else 
	{
		TCHAR szFlag[_MAX_PATH]={0};
		IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
		_stprintf(szFlag, _T("data\\ui\\mark\\title\\%s.bmp"), szTitle);
		ResTexture *newTex = IDraw2D::Inst()->NewTexture(szFlag, pIFS);
		m_mapRes.insert( make_pair(strRes, newTex) );
		DrawRoleTitleFlag(rc, z, newTex);
	}
}

void RoleTitleFlag::DrawRoleTitleFlag( RECT rc, float z, ResTexture* pRes )
{
	if( P_VALID(pRes) )
	{
		RECT srcRC;

		srcRC.top=srcRC.left=0;
		srcRC.bottom=srcRC.top+14;
		srcRC.right=srcRC.left+16;

		IDraw2D::Inst()->Draw(&rc, &srcRC, pRes, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, z);
	}
}

void RoleTitleFlag::DrawTeamFlag( RECT rc, float z, const TCHAR* szTeam )
{
	if( szTeam==NULL || _tcslen(szTeam)==0 )
		return;

	tstring strRes = szTeam;
	map<tstring, ResTexture*>::iterator it = m_mapResTeam.find(strRes);
	if( it!=m_mapResTeam.end() )
	{
		DrawRoleTitleFlag(rc, z, it->second);
	}
	else 
	{
		IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
		ResTexture *newTex = IDraw2D::Inst()->NewTexture(szTeam, pIFS);
		m_mapResTeam.insert( make_pair(strRes, newTex) );
		DrawTeamFlag(rc, z, newTex);
	}
}

void RoleTitleFlag::DrawTeamFlag( RECT rc, float z, ResTexture* pRes )
{
	if( P_VALID(pRes) )
	{
		RECT srcRC;

		srcRC.top=srcRC.left=0;
		srcRC.bottom=srcRC.top+16;
		srcRC.right=srcRC.left+15;

		IDraw2D::Inst()->Draw(&rc, &srcRC, pRes, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, z);
	}
}

void RoleTitleFlag::DrawVipFlag(RECT rc, float z, const TCHAR* szVip)
{
	if( szVip==NULL || _tcslen(szVip)==0 )
		return;

	tstring strRes = szVip;
	map<tstring, ResTexture*>::iterator it = m_mapResTeam.find(strRes);
	if( it!=m_mapResTeam.end() )
	{
		DrawVipFlag(rc, z, it->second);
	}
	else 
	{
		IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
		ResTexture *newTex = IDraw2D::Inst()->NewTexture(szVip, pIFS);
		m_mapResTeam.insert( make_pair(strRes, newTex) );
		DrawVipFlag(rc, z, newTex);
	}
}

void RoleTitleFlag::DrawVipFlag(RECT rc, float z, ResTexture* pRes)
{
	if( P_VALID(pRes) )
	{
		RECT srcRC;

		srcRC.top=srcRC.left=0;
		srcRC.bottom=srcRC.top+pRes->GetDeviceTex()->Height();
		srcRC.right=srcRC.left+pRes->GetDeviceTex()->Width();

		IDraw2D::Inst()->Draw(&rc, &srcRC, pRes, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, z);
	}
}

void RoleTitleFlag::DrawGodJob(RECT rc, float z, const TCHAR* szGod)
{
	if( szGod==NULL || _tcslen(szGod)==0 )
		return;

	tstring strRes = szGod;
	map<tstring, ResTexture*>::iterator it = m_mapResPicGodJob.find(strRes);
	if( it!=m_mapResPicGodJob.end() )
	{
		DrawVipFlag(rc, z, it->second);
	}
	else 
	{
		IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
		ResTexture *newTex = IDraw2D::Inst()->NewTexture(szGod, pIFS);
		m_mapResPicGodJob.insert( make_pair(strRes, newTex) );
		DrawVipFlag(rc, z, newTex);
	}
}
void RoleTitleFlag::DrawGodJob(RECT rc, float z, ResTexture* pRes)
{
	if( P_VALID(pRes) )
	{
		RECT srcRC;

		srcRC.top=srcRC.left=0;
		srcRC.bottom=srcRC.top+pRes->GetDeviceTex()->Height();
		srcRC.right=srcRC.left+pRes->GetDeviceTex()->Width();

		IDraw2D::Inst()->Draw(&rc, &srcRC, pRes, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, z);
	}
}

void RoleTitleFlag::DrawPicTitle( RECT rc, float z, LPCTSTR szTitlePic )
{
    if( szTitlePic==NULL || _tcslen(szTitlePic)==0 )
        return;

    tstring strRes = szTitlePic;
    map<tstring, ResTexture*>::iterator it = m_mapResPicTitle.find(strRes);
    if( it!=m_mapResPicTitle.end() )
    {
        DrawPicTitle(rc, z, it->second);
    }
    else 
    {
        TCHAR szFlag[_MAX_PATH]={0};
        IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
        _stprintf(szFlag, _T("data\\ui\\pic_title\\%s.bmp"), szTitlePic);
        ResTexture *newTex = IDraw2D::Inst()->NewTexture(szFlag, pIFS);
        m_mapResPicTitle.insert( make_pair(strRes, newTex) );
        DrawPicTitle(rc, z, newTex);
    }
}

void RoleTitleFlag::DrawPicTitle( RECT rc, float z, ResTexture* pRes )
{
    if( P_VALID(pRes) )
    {
        RECT srcRC;

        srcRC.top=srcRC.left=0;
        srcRC.bottom=srcRC.top+pRes->GetDeviceTex()->Height();
        srcRC.right=srcRC.left+pRes->GetDeviceTex()->Width();

        IDraw2D::Inst()->Draw(&rc, &srcRC, pRes, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, z);
    }
}