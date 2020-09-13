#include "StdAfx.h"
#include "SeleRoleMgr.h"
#include "Player.h"
#include "GameSet.h"
#include "AvatarCfg.h"
#include "CameraSetData.h"
static tstring null_string = _T("");
SimpleMapCamera::SimpleMapCamera(void)
{

}



const float scale[] = {
	2.5f, 2.5f, 2.5f,
};

SeleRoleMgr::SeleRoleMgr(void):m_Trunk(this)
{
	m_pNullSG			= NULL;
	m_EntityNode		= NULL;
	m_pFont				= NULL;
	m_pSG				= NULL;
	m_dwSelRoleID		= GT_INVALID;
	m_dwType			= GT_INVALID;
}

SeleRoleMgr::~SeleRoleMgr(void)
{
}

SeleRoleMgr g_SeleRoleMgr;
SeleRoleMgr* SeleRoleMgr::Inst()
{
	return &g_SeleRoleMgr;
}

VOID SeleRoleMgr::Init()
{
	IFS* pFS=(IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_Map");
	m_nWaterReflect = GameSet::Inst()->Get(EGS_WaterReflect);
	m_nWaterRefract	= GameSet::Inst()->Get(EGS_WaterRefract);
	GameSet::Inst()->Set(EGS_WaterReflect, 2);
	GameSet::Inst()->Set(EGS_WaterRefract, 1);
	GameSet::Inst()->ApplyAll();
	m_pRender = TObjRef<GUIRender>();

	m_pFont = Device()->NewDepthFont();
	m_pFont->Create(8, 17, 0, FALSE, g_StrTable[_T("Font_MSYH")]);	
}

VOID SeleRoleMgr::SetCamera(const Vector3& from, const Vector3& lookat)
{
	m_Camera.SetPerspective((FLOAT)Device()->GetClientWidth(), (FLOAT)Device()->GetClientHeight(), 
		_DegToRad(70.0f), 1.0f, 150000.0f);
	m_Camera.SetLookAt(from, lookat);
}

VOID SeleRoleMgr::SetCamera(const Vector3& lookat, float fYaw, float fPitch, float fRoll, float fDist)
{
	m_Camera.SetPerspective((FLOAT)Device()->GetClientWidth(), (FLOAT)Device()->GetClientHeight(), 
		_DegToRad(45.0f), 1.0f, 150000.0f);
	m_Camera.Set(lookat, fYaw, fPitch, fRoll, fDist);
}

void SeleRoleMgr::OpenMap(const TCHAR* szMapName, CameraBase* pBindCamera, DWORD dwType/*=0*/)
{
	CloseMap();
	m_dwType = dwType;

	ResMgr::Inst()->EnableIOAsync(true);
	IFS* pMapFS=(IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_Map");
	IFS* pSysFS=(IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");

	//--在打开新的包之前, 必须保证异步IO完成了现有包的所有操作
	while(ResMgr::Inst()->GetAsyncQueueSize() > 0)
		Sleep(100);
	//--
	TCHAR szCpk[MAX_PATH];
	_stprintf(szCpk,_T("data\\map\\%s.cpk"),szMapName);
	pMapFS->SetWorkingPackage(szCpk);

	//--load game map
	TCHAR szPath[512];
	_stprintf(szPath,_T("data\\map\\%s\\%s.mb"),szMapName,szMapName);
	m_gameMap.LoadFromFile( pMapFS, szPath, GameMap::ELO_Static | GameMap::ELO_Sound | GameMap::ELO_MapCloudLayer );

	m_pSG=new HugeQuadTreeSG;
	m_pSG->LoadFromFile(pMapFS,szMapName);
	m_pSG->BindCamera(pBindCamera);

	//--set sky box
	SGSkyBox *pSkyBox=new SGSkyBox;
	TCHAR *szTopTex=m_gameMap.GetSkyBoxTop();
	if (szTopTex!=NULL
		&& _tcslen(szTopTex)>0)
	{
		Vector3 size;
		m_gameMap.GetSkyBoxSize(size);
		pSkyBox->Create(m_gameMap.GetSkyBoxTop(),m_gameMap.GetSkyBoxFrontRight(),
			m_gameMap.GetSkyBoxBackLeft(),size);

		Vector3 offset;
		m_gameMap.GetSkyBoxOff(offset);
		pSkyBox->SetOffset(offset);
		pSkyBox->EnableRenderShade(m_gameMap.IsRenderSkyShade());
		pSkyBox->SetShadeColor(m_gameMap.GetSkyShadeCol());
		m_pSG->SetSkyNode(pSkyBox);

		// 读取云层
		m_pSG->ClearCloudNodes();
		const vector<tagMapCloudLayer>& cloudLayer = m_gameMap.GetMapCloudLayer();
		for(vector<tagMapCloudLayer>::const_iterator iter = cloudLayer.begin(); iter != cloudLayer.end(); ++iter )
		{
			EntityNode *node = new EntityNode;
			node->LoadRes( iter->szMdlPath );
			m_pSG->AddCloudNodes(node, iter->fAxis, iter->fFrequency, iter->fScale);
		}

		SGSunNode* pSunNode = new SGSunNode;
		Filename strSunPath = m_gameMap.GetSkyBoxTop();
		if( pSunNode->Create( strSunPath.GetPath().c_str() ) )
		{
			pSunNode->EnableFlare( true );
			m_pSG->SetSunNode( pSunNode );
		}
		else
			SAFE_DELETE( pSunNode );
	}

	//--set sunlight
	tagLight sunLight = m_gameMap.GetSunLight();	
	sunLight.diffuse = m_gameMap.GetDynamicDiffCol();
	sunLight.diffuse.R *= m_gameMap.GetSunModulus();
	sunLight.diffuse.G *= m_gameMap.GetSunModulus();
	sunLight.diffuse.B *= m_gameMap.GetSunModulus();
	sunLight.ambient = m_gameMap.GetDynamicAmbCol();
	sunLight.specular = m_gameMap.GetDynamicSpecCol();

	m_pSG->SetSunLight(sunLight);

	float fLitNode=128*50.0f;
	float fMidNode=2048.0f*50.0f;
	float fBigNode=3072.0f*50.0f;
	float fSimpleTrn=6000.0f*50.0f;
	m_pSG->SetViewZoneSize(HugeQuadTreeSG::EVZT_LitNode,Vector3(fLitNode,fLitNode,fLitNode));
	m_pSG->SetViewZoneSize(HugeQuadTreeSG::EVZT_MidNode,Vector3(fMidNode,fMidNode,fMidNode));
	m_pSG->SetViewZoneSize(HugeQuadTreeSG::EVZT_BigNode,Vector3(fBigNode,fBigNode,fBigNode));
	m_pSG->SetViewZoneSize(HugeQuadTreeSG::EVZT_SimpleTerrain,Vector3(fSimpleTrn,fSimpleTrn,fSimpleTrn));

	//--set fog
	tagDistFog fog;
	m_gameMap.GetFog(fog);
	fog.start*=fBigNode;
	fog.end*=fBigNode;
	m_pSG->SetSceneFog(fog);

	//--set camera zfar
	CameraBase* pCamera=m_pSG->GetCamera();
	pCamera->SetZFar(fSimpleTrn);

	m_pSG->InvalidViewZone();

}

void SeleRoleMgr::CloseMap()
{
	//--释放场景图
	if( P_VALID(m_pSG) )
		m_pSG->DetachDynamicNode(m_EntityNode);

	SAFE_DELETE(m_EntityNode);
	if( P_VALID(m_pSG) )
		SAFE_DELETE(m_pSG);
	GameSet::Inst()->Set(EGS_WaterReflect, m_nWaterReflect);
	GameSet::Inst()->Set(EGS_WaterRefract, m_nWaterRefract);
	GameSet::Inst()->ApplyAll();
}

VOID SeleRoleMgr::Destroy(SceneGraph* pSG)
{
	vector<tagSeleRole>::iterator ite;
	for(ite=m_vecSeleRole.begin(); ite!=m_vecSeleRole.end(); ite++)
	{	
		(*ite).pPlayer->DetachSceneNodeFromSG(pSG);
		if( P_VALID((*ite).pPlayer) )
			SAFE_DEL( (*ite).pPlayer );
	}
	m_vecSeleRole.clear();
	if( P_VALID(m_pSG) )
		m_pSG->DetachDynamicNode(m_EntityNode);

	SAFE_DELETE(m_EntityNode);
	if( P_VALID(m_pSG) )
		SAFE_DELETE(m_pSG);
	GameSet::Inst()->Set(EGS_WaterReflect, m_nWaterReflect);
	GameSet::Inst()->Set(EGS_WaterRefract, m_nWaterRefract);
	GameSet::Inst()->ApplyAll();

	SAFE_DELETE(m_pFont);
}
VOID SeleRoleMgr::Destroy()
{
	if( !P_VALID(m_pSG) )
		return;

	if( m_dwType == ESelect_Role)
	{
		vector<tagSeleRole>::iterator ite;
		for(ite=m_vecSeleRole.begin(); ite!=m_vecSeleRole.end(); ite++)
		{	
			(*ite).pPlayer->DetachSceneNodeFromSG(m_pSG);
			if( P_VALID((*ite).pPlayer) )
				SAFE_DEL( (*ite).pPlayer );
		}
		m_vecSeleRole.clear();
		if( P_VALID(m_pSG) )
			m_pSG->DetachDynamicNode(m_EntityNode);

		SAFE_DELETE(m_EntityNode);
		if( P_VALID(m_pSG) )
			SAFE_DELETE(m_pSG);
	}
	else if( m_dwType == ECreate_Role )
	{
		vector<tagSeleRole>::iterator ite;
		for(ite=m_vecSeleRole.begin(); ite!=m_vecSeleRole.end(); ite++)
		{	
			if( P_VALID((*ite).pPlayer) )
				SAFE_DEL( (*ite).pPlayer );
		}
		m_vecSeleRole.clear();
		if( P_VALID(m_pSG) )
			SAFE_DELETE(m_pSG);
	}
	else if( m_dwType == EPlayer_Login )
	{
		if( P_VALID(m_pSG) )
			m_pSG->DetachDynamicNode(m_EntityNode);

		SAFE_DELETE(m_EntityNode);
		if( P_VALID(m_pSG) )
			SAFE_DELETE(m_pSG);
	}
	GameSet::Inst()->Set(EGS_WaterReflect, m_nWaterReflect);
	GameSet::Inst()->Set(EGS_WaterRefract, m_nWaterRefract);
	GameSet::Inst()->ApplyAll();

	SAFE_DELETE(m_pFont);
}


VOID SeleRoleMgr::DeleteRole(DWORD dwRoleID, SceneGraph* pSG)
{
	for(int i=0; i<(int)m_vecSeleRole.size(); i++)
	{
		if( dwRoleID ==m_vecSeleRole[i].simRole.dwRoleID )
		{
			m_vecSeleRole[i].pPlayer->DetachSceneNodeFromSG(pSG);
			if( P_VALID(m_vecSeleRole[i].pPlayer) )
				SAFE_DEL( m_vecSeleRole[i].pPlayer );
			m_vecSeleRole.erase(m_vecSeleRole.begin()+i);
			break;
		}
	}
	ResetPostion();
}

bool CompareLocalPlayerID(const SeleRoleMgr::tagSeleRole& role1, const SeleRoleMgr::tagSeleRole& role2)
{
	return role1.simRole.dwRoleID < role2.simRole.dwRoleID;
}

VOID SeleRoleMgr::CreateRole(tagSimRole tagRole, Player* pNode)
{
	ASSERT( m_vecSeleRole.size() < MAX_ROLE );
	if( m_vecSeleRole.size() >= MAX_ROLE )
		return;

	tagSeleRole role;
	role.simRole = tagRole;
	role.pPlayer = pNode;

	// 新创建的角色，客户端通过wDressMdlID来穿时装
	if( DressMdIDInvalid != role.simRole.Avatar.wDressMdlID )
	{
		AvatarCfg::Inst()->MakeNewPlayerFahion( role.simRole.Avatar.bySex, role.simRole.Avatar.wDressMdlID, role.simRole.AvatarEquip );
	}

	role.pPlayer->LoadAvatar(role.simRole.Avatar, role.simRole.AvatarEquip, role.simRole.sDisplaySet);
	int i = m_vecSeleRole.size();
	float fScale = scale[i];
	Vector3 vScale = Vector3(fScale, fScale, fScale);
	role.pPlayer->SetScale(vScale, false);
	m_vecSeleRole.push_back(role);
	sort( m_vecSeleRole.begin(),m_vecSeleRole.end(), CompareLocalPlayerID);
	ResetPostion();
}

VOID SeleRoleMgr::AttachSceneNodeToSG(SceneGraph* pSG)
{
	vector<tagSeleRole>::iterator ite = m_vecSeleRole.begin();
	for(; ite!=m_vecSeleRole.end();ite++)
	{
		(*ite).pPlayer->AttachSceneNodeToSG(pSG);
	}
}

VOID SeleRoleMgr::DetachSceneNodeFromSG(SceneGraph* pSG)
{
	vector<tagSeleRole>::iterator ite = m_vecSeleRole.begin();
	for(; ite!=m_vecSeleRole.end();ite++)
	{
		(*ite).pPlayer->DetachSceneNodeFromSG(pSG);
	}
}

VOID SeleRoleMgr::Update()
{
	vector<tagSeleRole>::iterator ite = m_vecSeleRole.begin();
	for(; ite!=m_vecSeleRole.end();ite++)
	{
		(*ite).pPlayer->Update();
		//2010-08-12 Alex  选中的角色加上高光

		if((*ite).simRole.dwRoleID!=GetSelRoleID() ){
			(*ite).pPlayer->SetMulColor(1.0f,1.0f,1.0f);
		}else{
            (*ite).pPlayer->SetMulColor(2.0f,2.0f,2.0f);
		}
	}
	if(P_VALID(m_pSG))
		m_pSG->InvalidViewZone();
}

VOID SeleRoleMgr::UpdateSG(float deltaTime)
{
	if( P_VALID(m_pNullSG) && P_VALID(m_pNullSG->GetCamera()) )
		m_pNullSG->Render(deltaTime);
	if( P_VALID(m_pSG) && P_VALID(m_pSG->GetCamera()) )
		m_pSG->Render(deltaTime);
}

int SeleRoleMgr::GetMouseSelectRole(int mouseX,int mouseY)
{
	int ret=-1;

	Ray ray;
	m_Camera.GetMouseRay(mouseX, mouseY, ray);

	float dist = FLOAT_MAX;
	TResult tr;

	int i=0;
	for(vector<tagSeleRole>::iterator iter=m_vecSeleRole.begin();
		iter!=m_vecSeleRole.end();++iter)
	{
		Player*	pRole = (*iter).pPlayer;
		ASSERT( P_VALID(pRole) );

		AABBox box;
		pRole->GetSceneNode()->GetBox(box);

		tr=ray.Intersect(box);

		if(tr.first && tr.second<dist)
		{
			dist=tr.second;
			ret=i;
			m_dwSelRoleID = iter->simRole.dwRoleID;
		}
		i++;
	}

	return ret;
}

int SeleRoleMgr::GetPickupRole( DWORD dwRoleID )
{
	for( size_t i = 0; i < m_vecSeleRole.size(); i++ )
	{
		if( m_vecSeleRole[i].simRole.dwRoleID == dwRoleID )
			return i;
	}
	return GT_INVALID;
}

DWORD SeleRoleMgr::GetRoleId(int i) const
{ 
	if(i<0 || i>(int)m_vecSeleRole.size())
		return -1;
	return m_vecSeleRole[i].simRole.dwRoleID; 
}

const tagSimRole& SeleRoleMgr::GetRoleInfo(int i)const
{ 
	ASSERT(i>=0 || i<=(int)m_vecSeleRole.size());
	return m_vecSeleRole[i].simRole;
}

VOID SeleRoleMgr::ResetPostion()
{
	const tagCameraData* pData = CameraSetData::Inst()->GetData();
	Vector3 plyaerPos[3];
	plyaerPos[0].x = pData->dwRole1Pos_x;
	plyaerPos[0].y = pData->dwRole1Pos_y;
	plyaerPos[0].z = pData->dwRole1Pos_z;
	plyaerPos[1].x = pData->dwRole2Pos_x;
	plyaerPos[1].y = pData->dwRole2Pos_y;
	plyaerPos[1].z = pData->dwRole2Pos_z;
	plyaerPos[2].x = pData->dwRole3Pos_x;
	plyaerPos[2].y = pData->dwRole3Pos_y;
	plyaerPos[2].z = pData->dwRole3Pos_z;
	for(int i=0; i<(int)m_vecSeleRole.size(); i++)
	{
		//if( m_vecSeleRole[i].simRole.Avatar.bySex == 0 )
		m_vecSeleRole[i].pPlayer->SetPos(plyaerPos[i]);
// 		else 
// 			m_vecSeleRole[i].pPlayer->SetPos(playerMPos[i]);
		m_vecSeleRole[i].pPlayer->FaceBack();
		if(i == 0)
		{
			m_vecSeleRole[0].pPlayer->SetYaw(-0.1*3.14);
			Vector3 scal = m_vecSeleRole[0].pPlayer->GetDesScale();
			m_vecSeleRole[0].pPlayer->SetScale(Vector3(3.2f, 3.2f, 3.2f));
		}
		else if(i == 1)
		{
			m_vecSeleRole[1].pPlayer->SetYaw(-0.3*3.14);
			Vector3 scal = m_vecSeleRole[1].pPlayer->GetDesScale();
			m_vecSeleRole[1].pPlayer->SetScale(Vector3(3.2f, 3.2f, 3.2f));
		}
		else if( i == 2)
		{
			m_vecSeleRole[2].pPlayer->SetYaw(-0.1*3.14);
			Vector3 scal = m_vecSeleRole[2].pPlayer->GetDesScale();
			m_vecSeleRole[2].pPlayer->SetScale(Vector3(3.2f, 3.2f, 3.2f));
		}
		m_vecSeleRole[i].pPlayer->Update();
	}
}

void SeleRoleMgr::RefreshTrack()
{
	TCHAR szBuff[X_SHORT_NAME] = {0};
	for(int i=0; i<(int)m_vecSeleRole.size(); i++)
	{
		_stprintf( szBuff, _T("cho%d"), 1 );
		Player* player = m_vecSeleRole[i].pPlayer;
		player->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All, szBuff,true, false, 0.0f, 0.0f);
	}
}

bool SeleRoleMgr::IsReadyTrackEnd( int i ) const
{
	ASSERT(i>=0 || i<=(int)m_vecSeleRole.size());
	Player* player = m_vecSeleRole[i].pPlayer;
	EItemTypeEx weaponType = player->GetRWeaponType();
	tstring strAct = GetHandUpName(weaponType);
	return player->GetAniPlayer()->IsTrackEnd( AvatarAniPlayer::EAT_Other,strAct.c_str() );
}

void SeleRoleMgr::PlayReadyTrack( int i )const
{
	ASSERT(i>=0 || i<=(int)m_vecSeleRole.size());	
	Player* player = m_vecSeleRole[i].pPlayer;
	EItemTypeEx weaponType = player->GetRWeaponType();
	tstring strAct = GetHandUpName(weaponType);
	
	player->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All, 
		strAct.c_str(), false, false, 0.0f, 0.2f);
	player->HandUpWeapon(true);
}

void SeleRoleMgr::SwitchToInitState(void)const
{
	for( int i=0; i<(int)m_vecSeleRole.size(); ++i )
	{
		Player* player = m_vecSeleRole[i].pPlayer;
		EItemTypeEx weaponType = player->GetRWeaponType();
		
		if( P_VALID( player ) )
		{
			player->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Stand,AvatarAniMixer::EAP_All, 
				CombatSysUtil::Inst()->GetStandTrackName(weaponType), true, false, 0.0f, 0.0f);
			
			player->HandUpWeapon( false );
		}
	}
}

const TCHAR* SeleRoleMgr::GetHandUpName(EItemTypeEx eWeapon)const
{
	switch(eWeapon)
	{
		//--徒手
	case EITE_Null:		//徒手
	case EITE_Faqi:		//法器
		return _T("zee1");
		//--腰间
	case EITE_Sword:	//剑
		return _T("zea1");
		//--腰后
	case EITE_Blade:	//刀
		return _T("zeb1");
		//--背后
	case EITE_Wand:		//长兵
		return _T("zec1");
	case EITE_Bow:		//弓
		return _T("zed1");
	case EITE_Qin:		//琴
		return _T("zeg1");
	}
	return _T("zee1");
}

void SeleRoleMgr::PlayStandTrack(int i)const
{
	ASSERT(i>=0 || i<=(int)m_vecSeleRole.size());	
	Player* player = m_vecSeleRole[i].pPlayer;
	EItemTypeEx weaponType = player->GetRWeaponType();
	player->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Stand,AvatarAniMixer::EAP_All, 
		CombatSysUtil::Inst()->GetStandTrackName(weaponType), true, false, 0.0f, 0.0f);
}

void SeleRoleMgr::PlayStandUpTrack(int nCur, int nLast)const
{
	ASSERT(nCur>=0 || nCur<=(int)m_vecSeleRole.size());
	Player* player = m_vecSeleRole[nCur].pPlayer;
	TCHAR szBuff[X_SHORT_NAME] = {0};
	_stprintf( szBuff, _T("cfm%d"), 1 );
	player->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All, szBuff, false, false, 0.0f, 0.2f);

	if(nLast>=0 && nLast<(int)m_vecSeleRole.size())
	{
		Player* player = m_vecSeleRole[nLast].pPlayer;
		TCHAR szBuff[X_SHORT_NAME] = {0};
		_stprintf( szBuff, _T("chg%d"), 1 );
		player->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All, szBuff, false, false, 0.0f, 0.2f);
	}
}

void SeleRoleMgr::IsStandUpTrack(int nCur)const
{
	if(nCur<0 || nCur>=(int)m_vecSeleRole.size())
		return ;
	Player* player = m_vecSeleRole[nCur].pPlayer;
	TCHAR szBuff[X_SHORT_NAME] = {0};
	_stprintf( szBuff, _T("cfm%d"), 1 );
	if(player->GetAniPlayer()->IsTrackEnd(AvatarAniPlayer::EAT_Other,szBuff))
	{
		if( EITE_Blade == player->GetRWeaponType() )
			player->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Stand,AvatarAniMixer::EAP_All,_T("std2"),true, false, 0.0f, 0.2f);
		else
			player->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Stand,AvatarAniMixer::EAP_All,_T("std1"),true, false, 0.0f, 0.2f);
	}
}

void SeleRoleMgr::IsSitDownTrack(int nLast)const
{
	if(nLast<0 || nLast>=(int)m_vecSeleRole.size())
		return ;
	Player* player = m_vecSeleRole[nLast].pPlayer;
	TCHAR szBuff[X_SHORT_NAME] = {0};
	_stprintf( szBuff, _T("chg%d"), 1 );
	if(player->GetAniPlayer()->IsTrackEnd(AvatarAniPlayer::EAT_Other,szBuff))
	{
		_stprintf( szBuff, _T("cho%d"), 1 );
		player->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All, szBuff,true, false, 0.0f, 0.2f);
	}
}

void SeleRoleMgr::DrawSelectedRoleFlag( int nRole )
{
	if(nRole<0 || nRole>=(int)m_vecSeleRole.size())
		return ;
	Player* player = m_vecSeleRole[nRole].pPlayer;
	if( P_VALID(player) )
	{
		player->GetHeadPos(m_tagHead.worldPos);
		m_tagHead.scrPos = m_Camera.WorldToScreenF(m_tagHead.worldPos);
		m_tagHead.scrPos.z /= 2;

		m_tagHead.strName = m_vecSeleRole[nRole].simRole.szRoleName;

		if( m_tagHead.scrPos.z<0.0f || m_tagHead.scrPos.z>0.5f )
			return;

		POINT pt = m_Camera.WorldToScreen(m_tagHead.worldPos);
		pt.y -= 20;

		tagPoint ptSize;
		m_pRender->GetTextSize(m_tagHead.strName.c_str(), NULL, ptSize);

		RECT rcName;
		rcName.left = pt.x - ptSize.x/2;
		rcName.right = pt.x + ptSize.x/2;
		rcName.top = pt.y - ptSize.y;
		rcName.bottom = pt.y;

		m_pFont->Begin();
		RECT rect1;
		rect1.top = rcName.top-1;
		rect1.bottom = rcName.bottom-1;
		rect1.left = rcName.left;
		rect1.right = rcName.right;
		m_pFont->DrawText(m_tagHead.strName.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, m_tagHead.scrPos.z, 1);

		rect1.top = rcName.top+1;
		rect1.bottom = rcName.bottom+1;
		rect1.left = rcName.left;
		rect1.right = rcName.right;
		m_pFont->DrawText(m_tagHead.strName.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, m_tagHead.scrPos.z, 1);

		rect1.top = rcName.top;
		rect1.bottom = rcName.bottom;
		rect1.left = rcName.left-1;
		rect1.right = rcName.right-1;
		m_pFont->DrawText(m_tagHead.strName.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, m_tagHead.scrPos.z, 1);

		rect1.top = rcName.top;
		rect1.bottom = rcName.bottom;
		rect1.left = rcName.left+1;
		rect1.right = rcName.right+1;
		m_pFont->DrawText(m_tagHead.strName.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, m_tagHead.scrPos.z, 1);

		//绘制姓名
		m_pFont->DrawText(m_tagHead.strName.c_str(), &rcName, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF00FF00, m_tagHead.scrPos.z, 1);

		m_pFont->End();
	}
}

VOID SeleRoleMgr::DrawOldPlayerRewardFlag()
{
	for(size_t i = 0; i < m_vecSeleRole.size(); i++)
	{
		Player* player = m_vecSeleRole[i].pPlayer;
		if( !P_VALID(player) || !m_vecSeleRole[i].simRole.bOldPlayerBack )
			continue;

		m_tagHead.worldPos = player->GetPos();
		m_tagHead.scrPos = m_Camera.WorldToScreenF(m_tagHead.worldPos);
		m_tagHead.scrPos.z /= 2;

		m_tagHead.strName = g_StrTable[_T("OldPlayerBackGame")];

		if( m_tagHead.scrPos.z<0.0f || m_tagHead.scrPos.z>0.5f )
			return;

		POINT pt = m_Camera.WorldToScreen(m_tagHead.worldPos);
		pt.y += 15;

		tagPoint ptSize;
		m_pRender->GetTextSize(m_tagHead.strName.c_str(), NULL, ptSize);

		RECT rcName;
		rcName.left = pt.x - ptSize.x/2;
		rcName.right = pt.x + ptSize.x/2;
		rcName.top = pt.y - ptSize.y;
		rcName.bottom = pt.y;

		m_pFont->Begin();
		RECT rect1;
		rect1.top = rcName.top-1;
		rect1.bottom = rcName.bottom-1;
		rect1.left = rcName.left;
		rect1.right = rcName.right;
		m_pFont->DrawText(m_tagHead.strName.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, m_tagHead.scrPos.z - 0.2f, 1);

		rect1.top = rcName.top+1;
		rect1.bottom = rcName.bottom+1;
		rect1.left = rcName.left;
		rect1.right = rcName.right;
		m_pFont->DrawText(m_tagHead.strName.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, m_tagHead.scrPos.z - 0.2f, 1);

		rect1.top = rcName.top;
		rect1.bottom = rcName.bottom;
		rect1.left = rcName.left-1;
		rect1.right = rcName.right-1;
		m_pFont->DrawText(m_tagHead.strName.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, m_tagHead.scrPos.z - 0.2f, 1);

		rect1.top = rcName.top;
		rect1.bottom = rcName.bottom;
		rect1.left = rcName.left+1;
		rect1.right = rcName.right+1;
		m_pFont->DrawText(m_tagHead.strName.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, m_tagHead.scrPos.z - 0.2f, 1);

		//绘制姓名
		m_pFont->DrawText(m_tagHead.strName.c_str(), &rcName, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF00FF00, m_tagHead.scrPos.z - 0.2f, 1);

		m_pFont->End();
	}
}

//----------------------------------------------------------------------------------------------------------------
void SimpleMapCamera::Update(const Vector3& nextLookAt)
{
	float deltatime=0.035f;

	m_targetLookAt=nextLookAt;

	const float springConst=4.6f;  // Hooke's Constant
	const float springLen=20.0f;
	Vector3 disp=m_vLookat-m_targetLookAt;
	float displen=Vec3Len(&disp);		

	if(displen > 0.05f)
	{
		Vector3 olddir;
		// Calculate Spring Force
		float force=springConst * (springLen - displen) ;
		// Apply Spring Force
		disp /= displen;
		olddir=disp;
		disp *= (force * deltatime);
		m_vLookat+=disp;

		Vector3 newdir=m_vLookat-m_targetLookAt;
		Vec3Normalize(newdir);
		if(D3DXVec3Dot(&olddir,&newdir)<=0.0f)
			m_vLookat=m_targetLookAt;
	}

	UpdateViewMatrix();
}

tstring SeleRoleMgr::GetRoleNameByID(DWORD id)
{
	for (vector<tagSeleRole>::iterator it = m_vecSeleRole.begin(); it != m_vecSeleRole.end(); ++it)
	{
		if (it->simRole.dwRoleID == id)
		{
			return it->pPlayer->GetRoleName();
		}
	}
	return _T("");
}