#include "StdAfx.h"
#include "AvatarCfg.h"

AvatarCfg::AvatarCfg(void)
{
}

AvatarCfg::~AvatarCfg(void)
{
}

static AvatarCfg g_AvatarCfg;
AvatarCfg* AvatarCfg::Inst()
{
	return &g_AvatarCfg;
}

const TCHAR* AvatarCfg::GetHairTypeMdlPath(BYTE bySex, WORD id, BYTE byTransSkinIndex /* = 0 */)
{ 
	if(id<0 || id>=AVATAR_HAIR_TYPE_EX)
		return NULL;
	TCHAR path[512] = {0};
	map<DWORD, tagTransSkin>::iterator iter = m_mapTransSkin.find(byTransSkinIndex);
	if(iter != m_mapTransSkin.end())
	{
		TCHAR szTemp[X_LONG_NAME] = {0};			// 如果是变身蒙皮就读hat文件夹下面的
		if(bySex)
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatarboy\\hat\\%s.fskin"), iter->second.strTSHat.c_str());
		else
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatargirl\\hat\\%s.fskin"), iter->second.strTSHat.c_str());
		m_strPath = szTemp;
	}
	else
	{
		if(bySex)
		{
			_stprintf(path, _T("data\\model\\avatarboy\\hair\\Mhair%02d_1_hair.fskin"), id+1);
			m_strPath = path;
		}
		else
		{
			_stprintf(path, _T("data\\model\\avatargirl\\hair\\Fhair%02d_1_hair.fskin"), id+1);
			m_strPath = path;
		}
	}
	
	return m_strPath.c_str();
}

const TCHAR* AvatarCfg::GetHairColorTexPath(BYTE bySex, WORD hairID, WORD colorID, BYTE byTransSkinIndex /* = 0 */)
{
	if(hairID<0 || hairID>=AVATAR_HAIR_TYPE_EX)
		return NULL;
	if(colorID<0 || colorID>=AVATAR_HAIR_COLOR)
		return NULL;
	TCHAR path[512] = {0};
	map<DWORD, tagTransSkin>::iterator iter = m_mapTransSkin.find(byTransSkinIndex);
	if(iter != m_mapTransSkin.end())
	{
		TCHAR szTemp[X_LONG_NAME] = {0};
		if(bySex)
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatarboy\\hair\\%s.tga"), iter->second.strTSHairColor.c_str());
		else
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatargirl\\hair\\%s.tga"), iter->second.strTSHairColor.c_str());
		m_strPath = szTemp;
	}
	else
	{
		if(bySex)
		{
			_stprintf(path, _T("data\\model\\avatarboy\\hair\\Mhair%02d_%d.tga"), hairID+1, colorID+1);
		}
		else
		{
			_stprintf(path, _T("data\\model\\avatargirl\\hair\\Fhair%02d_%d.tga"), hairID+1, colorID+1);
		}
		m_strPath = path;
	}

	return m_strPath.c_str();
}

const TCHAR* AvatarCfg::GetFaceMdlPath(BYTE bySex, WORD id, BYTE byTransSkinIndex /* = 0 */)
{
	if(id<0 || id>=AVATAR_FACE_EX)
		return NULL;
	TCHAR path[512] = {0};
	map<DWORD, tagTransSkin>::iterator iter = m_mapTransSkin.find(byTransSkinIndex);
	if(iter != m_mapTransSkin.end())
	{
		TCHAR szTemp[X_LONG_NAME] = {0};
		if(bySex)
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatarboy\\face\\%s.fskin"), iter->second.strTSFace.c_str());
		else
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatargirl\\face\\%s.fskin"), iter->second.strTSFace.c_str());
		m_strPath = szTemp;
	}
	else
	{
		if(bySex)
		{
			_stprintf(path, _T("data\\model\\avatarboy\\face\\Mface%02d_face.fskin"), id+1);
		}
		else
		{
			_stprintf(path, _T("data\\model\\avatargirl\\face\\Fface%02d_face.fskin"), id+1);	
		}
		m_strPath = path;
	}
	
	return m_strPath.c_str();
}

const TCHAR* AvatarCfg::GetFaceDtlTexPath(BYTE bySex, WORD id)
{
	if(id<0 || id>=AVATAR_FACE_DETAIL)
		return NULL;

	// 默认没有细节贴图
	if(0==id)
		return _T("");

	TCHAR path[512] = {0};
	if(bySex)
	{
		_stprintf(path, _T("data\\model\\avatarboy\\face\\MfaceDetail%02d.tga"), id);
	}
	else
	{
		_stprintf(path, _T("data\\model\\avatargirl\\face\\FfaceDetail%02d.tga"), id);	
	}
	m_strPath = path;
	return m_strPath.c_str();
}

const TCHAR* AvatarCfg::GetUpperUWMdlPath(BYTE bySex,BOOL bFashion, BYTE byTransSkinIndex /* = 0 */ )
{
	map<DWORD, tagTransSkin>::iterator iter = m_mapTransSkin.find(byTransSkinIndex);
	if(iter != m_mapTransSkin.end())
	{
		TCHAR szTemp[X_LONG_NAME] = {0};
		if(bySex)
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatarboy\\upper\\%s.fskin"), iter->second.strTSUpper.c_str());
		else
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatargirl\\upper\\%s.fskin"), iter->second.strTSUpper.c_str());
		m_strPath = szTemp;
	}
	else
	{
		if(bySex)
		{
			if( bFashion )
				m_strPath = _T("data\\model\\avatarboy\\upper\\uw2_upper.fskin");
			else
				m_strPath = _T("data\\model\\avatarboy\\upper\\uw1_upper.fskin");
		}
		else
		{
			if( bFashion )
				m_strPath = _T("data\\model\\avatargirl\\upper\\uw2_upper.fskin");
			else
				m_strPath = _T("data\\model\\avatargirl\\upper\\uw1_upper.fskin");
		}
	}
	return m_strPath.c_str();
}

const TCHAR* AvatarCfg::GetLowerUWMdlPath(BYTE bySex,BOOL bFashion, BYTE byTransSkinIndex /* = 0 */)
{
	map<DWORD, tagTransSkin>::iterator iter = m_mapTransSkin.find(byTransSkinIndex);
	if(iter != m_mapTransSkin.end())
	{
		TCHAR szTemp[X_LONG_NAME] = {0};
		if(bySex)
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatarboy\\lower\\%s.fskin"), iter->second.strTSLower.c_str());
		else
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatargirl\\lower\\%s.fskin"), iter->second.strTSLower.c_str());
		m_strPath = szTemp;
	}
	else
	{
		if(bySex)
		{
			if( bFashion )
				m_strPath = _T("data\\model\\avatarboy\\lower\\uw2_lower.fskin");
			else
				m_strPath = _T("data\\model\\avatarboy\\lower\\uw1_lower.fskin");
		}
		else
		{
			if( bFashion )
				m_strPath = _T("data\\model\\avatargirl\\lower\\uw2_lower.fskin");
			else
				m_strPath = _T("data\\model\\avatargirl\\lower\\uw1_lower.fskin");
		}
	}
	return m_strPath.c_str();
}

const TCHAR* AvatarCfg::GetBootUWMdlPath(BYTE bySex,BOOL bFashion, BYTE byTransSkinIndex /* = 0 */)
{
	map<DWORD, tagTransSkin>::iterator iter = m_mapTransSkin.find(byTransSkinIndex);
	if(iter != m_mapTransSkin.end())
	{
		TCHAR szTemp[X_LONG_NAME] = {0};
		if(bySex)
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatarboy\\boot\\%s.fskin"), iter->second.strTSBoot.c_str());
		else
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatargirl\\boot\\%s.fskin"), iter->second.strTSBoot.c_str());
		m_strPath = szTemp;
	}
	else
	{
		if(bySex)
		{
			if( bFashion )
				m_strPath = _T("data\\model\\avatarboy\\boot\\uw2_boot.fskin");
			else
				m_strPath = _T("data\\model\\avatarboy\\boot\\uw1_boot.fskin");
		}
		else
		{
			if( bFashion )
				m_strPath = _T("data\\model\\avatargirl\\boot\\uw2_boot.fskin");
			else
				m_strPath = _T("data\\model\\avatargirl\\boot\\uw1_boot.fskin");
		}
	}
	
	return m_strPath.c_str();
}

const TCHAR* AvatarCfg::GetGloveUWMdlPath(BYTE bySex,BOOL bRight,BOOL bFashion, BYTE byTransSkinIndex /* = 0 */)
{
	map<DWORD, tagTransSkin>::iterator iter = m_mapTransSkin.find(byTransSkinIndex);
	if(iter != m_mapTransSkin.end())
	{
		TCHAR szTemp[X_LONG_NAME] = {0};
		if(bySex)
		{
			if( bRight )
				_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatarboy\\glove\\%s.fskin"), iter->second.strTSGlove_r.c_str());
			else
				_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatarboy\\glove\\%s.fskin"), iter->second.strTSGlove_l.c_str());
		}
		else
		{
			if( bRight )
				_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatargirl\\glove\\%s.fskin"), iter->second.strTSGlove_r.c_str());
			else
				_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatargirl\\glove\\%s.fskin"), iter->second.strTSGlove_l.c_str());
		}
		m_strPath = szTemp;
	}
	else
	{
		if(bySex)
		{
			if( bFashion )
			{
				if( bRight )
					m_strPath = _T("data\\model\\avatarboy\\glove\\uw2_r_glove.fskin");
				else
					m_strPath = _T("data\\model\\avatarboy\\glove\\uw2_l_glove.fskin");
			}
			else
			{
				if( bRight )
					m_strPath = _T("data\\model\\avatarboy\\glove\\uw1_r_glove.fskin");
				else
					m_strPath = _T("data\\model\\avatarboy\\glove\\uw1_l_glove.fskin");
			}
		}
		else
		{
			if( bFashion )
			{
				if( bRight )
					m_strPath = _T("data\\model\\avatargirl\\glove\\uw2_r_glove.fskin");
				else
					m_strPath = _T("data\\model\\avatargirl\\glove\\uw2_l_glove.fskin");
			}
			else
			{
				if( bRight )
					m_strPath = _T("data\\model\\avatargirl\\glove\\uw1_r_glove.fskin");
				else
					m_strPath = _T("data\\model\\avatargirl\\glove\\uw1_l_glove.fskin");
			}
		}
	}
	
	return m_strPath.c_str();
}


const TCHAR* AvatarCfg::GetBackUWMdlPath(BYTE bySex, BYTE byTransSkinIndex /* = 0 */)
{
	map<DWORD, tagTransSkin>::iterator iter = m_mapTransSkin.find(byTransSkinIndex);
	if(iter != m_mapTransSkin.end())
	{
		TCHAR szTemp[X_LONG_NAME] = {0};
		if(bySex)
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatarboy\\mantle\\%s.fskin"), iter->second.strTSMantle.c_str());
		else
			_sntprintf(szTemp, X_LONG_NAME, _T("data\\model\\avatargirl\\mantle\\%s.fskin"), iter->second.strTSMantle.c_str());
		m_strPath = szTemp;
	}
	else
		return _T("");
	return m_strPath.c_str();
}

void AvatarCfg::MakeNewPlayerFahion( const BYTE bySex, const int n, tagAvatarEquip& out ) const
{
	ASSERT( n >= 0 && n < AVATAR_DRESS );
	ZeroMemory( &out, sizeof(tagAvatarEquip) );
	//const int nPartSex = 10 * ( 1 - bySex );
	//const int nPartIndex = 100 * n;
	//DWORD dwStartTypeID = 8000000 + nPartSex + nPartIndex;
	switch(n)
	{
	case 0:
		{
			out.Set( EAE_Upper, 8060001, 0, 0  );
			out.Set( EAE_Lower, 8070001, 0, 0  );
			out.Set( EAE_Glove, 8080001, 0, 0  );
			out.Set( EAE_Boot,  8090001, 0, 0  ); 
		}
		break;
	case 1:
		{
			out.Set( EAE_Upper, 8061001, 0, 0  );
			out.Set( EAE_Lower, 8071001, 0, 0  );
			out.Set( EAE_Glove, 8081001, 0, 0  );
			out.Set( EAE_Boot,  8091001, 0, 0  ); 
		}
		break;
	default:
		{
			out.Set( EAE_Upper, 8060001, 0, 0  );
			out.Set( EAE_Lower, 8070001, 0, 0  );
			out.Set( EAE_Glove, 8080001, 0, 0  );
			out.Set( EAE_Boot,  8090001, 0, 0  ); 
		}
		break;
	}
}

VOID AvatarCfg::LoadFromFile()
{
	m_mapTransSkin.clear();
	tstring strPath = _T("data\\system\\attdata\\trans_res.xml");
	list<tstring> SkinList;
	TObjRef<VarContainer> con = CreateObj( "SkinList", "VarContainer" );
	if(!con->Load("VFS_System", strPath.c_str(), "id", &SkinList))
	{
		IMSG(_T("Load file trans_res.xml failed\r\n"));
		return;
	}
	list<tstring>::iterator end = SkinList.end();
	for (list<tstring>::iterator iter = SkinList.begin(); iter != end; ++iter)
	{
		tagTransSkin TransSkin;
		TransSkin.dwIndex = con->GetDword(_T("id"), iter->c_str(), 0);
		TransSkin.strTSFace = con->GetString(_T("face"), (*iter).c_str(), _T(""));
		TransSkin.strTSHat = con->GetString(_T("hat"), (*iter).c_str(), _T(""));
		TransSkin.strTSHair = con->GetString(_T("hair"), (*iter).c_str(), _T(""));
		TransSkin.strTSHairColor = con->GetString(_T("hair_color"), (*iter).c_str(), _T(""));
		TransSkin.strTSUpper = con->GetString(_T("upper"), (*iter).c_str(), _T(""));
		TransSkin.strTSLower = con->GetString(_T("lower"), (*iter).c_str(), _T(""));
		TransSkin.strTSGlove_r = con->GetString(_T("glove_r"), (*iter).c_str(), _T(""));
		TransSkin.strTSGlove_l = con->GetString(_T("glove_l"), (*iter).c_str(), _T(""));
		TransSkin.strTSBoot = con->GetString(_T("boot"), (*iter).c_str(), _T(""));
		TransSkin.strTSMantle = con->GetString(_T("mantle"), (*iter).c_str(), _T(""));
		m_mapTransSkin.insert(make_pair(TransSkin.dwIndex, TransSkin));
	}
	KillObj("SkinList");
}