#include "StdAfx.h"
#include "EffectMgr.h"
#include "MapMgr.h"
#include "RoleMgr.h"
#include "Role.h"
#include "GameFrameMgr.h"
#include "NPC.h"
#include "CreatureData.h"
#include "SFXData.h"
#include "GameSet.h"
#include "SkillProtoData.h"
#include "EffectSetData.h"
#include "LocalPlayer.h"
#include "VirtualRole.h"

const Vector3 g_vStdEffectRefSize( 50.0f, 175.0f, 50.0f);    // 特效的标准参考大小，用来缩放特效
const float g_fDefaultRoleEffYOffset = 2.0f / 3.0f;          // 默认角色特效播放的Y方向偏移角色高度的2/3

EffectMgr::EffectMgr(void) :
m_Trunk( this ),
m_dwCurEffectID( EFFECT_START_ID )
{
}

EffectMgr::~EffectMgr(void)
{
}

EffectMgr g_effectMgr;
EffectMgr* EffectMgr::Inst()
{
	return &g_effectMgr;
}

VOID EffectMgr::Init()
{
	m_pGameFrameMgr = TObjRef<GameFrameMgr>();
	m_pGameFrameMgr->RegisterEventHandle( _T("CloseMap"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&EffectMgr::OnCloseMap));

	srand( timeGetTime() );
}

VOID EffectMgr::Destroy()
{
	m_pGameFrameMgr->UnRegisterEventHandler( _T("CloseMap"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EffectMgr::OnCloseMap));
	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); itr++ )
	{
		DeleteEffect( itr->second );
	}
	m_mapEffect.clear();
}

VOID EffectMgr::Update()
{
	SceneGraph* pSG = MapMgr::Inst()->GetSceneGraph();
	if( NULL == pSG )
		return;

	const float fTimeDelta = Kernel::Inst()->GetDeltaTime();

	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end();  )
	{
		tagBaseEffectData* pData = itr->second;
		if( !P_VALID(pData) || !P_VALID(pData->pSGNode) )
		{
			itr = m_mapEffect.erase( itr );
			continue;
		}
		if( GT_INVALID != pData->dwBindRoleID )
		{
			BOOL bBindRoleExist = UpdataRoleEffect( pData );
			// 绑定的角色已经不存在了
			if( !bBindRoleExist )
			{
				pData->pSGNode = NULL;
				DeleteEffect( pData );
				itr = m_mapEffect.erase( itr );
				continue;
			}
		}

		if( EET_Launch == pData->eType )
		{
			UpdataTargetPath( (tagLaunchEffectData*)pData, fTimeDelta );
		}

		if( EET_Win == pData->eType )
		{
			UpdateTargetPath4Win((tagWinEffectData*)pData, fTimeDelta );
		}

		// FOR DEBUG?? 特效在水面下看不到，如果在水下，把它放到水面上
		if( pData->bPullOnWater )
		{
			PullOnWater( pData );
		}

		// 更新特效
		pData->pSGNode->UserUpdate( fTimeDelta, pSG );

		if(pData->eType == EET_BeAttack) // 被击特效激发掉血事件
		{
			UpdateBeAtkEffectMsg((tagBeAttackEffectData*)pData);
		}

		// 删除已经关闭的特效
		if( pData->pSGNode->IsClosed() )
		{
			DeleteEffect( pData );
			itr = m_mapEffect.erase( itr );
			continue;
		}
		itr++;
	}
}

DWORD EffectMgr::PlayEffect(const TCHAR* szPath, const Vector3& vPos, const Vector3& vScale, const Vector3& vRot )
{
	SGAttachableEffectNode* pNode = LoadSFXFile( szPath, NULL );
	if( NULL == pNode )
		return GT_INVALID;

	Transform trans;
	trans.Scale( vScale.x, vScale.y, vScale.z );
	trans.Rotate( vRot.y, vRot.x, vRot.z );
	trans.Translate( vPos );
	pNode->SetWorldMat( trans.GetMatrix() );
	pNode->Play();
	tagBaseEffectData* pData = new tagBaseEffectData(EET_Common);
	pData->pSGNode = pNode;
	pData->dwBindRoleID = GT_INVALID;
	const DWORD dwID = AddEffect( pData, NULL, NULL );
	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
	}
	return dwID;
}

DWORD EffectMgr::PlayEffect(const Vector3& vPos,float yaw,const TCHAR* szEffectName, const Vector3* pSize/*=NULL*/, EEffectScaleType eScaleType/* = EEST_Null*/, const TCHAR* szFolderName/* = _T("combat")*/)
{
	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName, szFolderName );
	if( NULL == pNode )
		return GT_INVALID;

	Transform trans;
	if( NULL != pSize )
	{
		float fScale = GetEffectScale( eScaleType, *pSize );
		if( fScale > 3.0f )
			fScale = 3.0f; 
		trans.Scale( fScale, fScale, fScale );
	}
	trans.Translate( vPos );
	trans.RotateAxis('Y',yaw);
	pNode->SetWorldMat( trans.GetMatrix() );
	pNode->Play();
	tagBaseEffectData* pData = new tagBaseEffectData(EET_Common);
	pData->bPullOnWater = true;
	pData->pSGNode = pNode;
	pData->dwBindRoleID = GT_INVALID;
	const DWORD dwID = AddEffect( pData, NULL, NULL );
	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
	}
	return dwID;
}

DWORD EffectMgr::PlayUiSoundEffect(const TCHAR* szEffectName,GUIEffectMgr::EEffectType eType,const TCHAR* szFolderName/* = _T("ui")*/)
{
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(plp) || !plp->IsAttributeInited())
		return GT_INVALID;
	
	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName, szFolderName );
	if( NULL == pNode )
		return GT_INVALID;

	pNode->Play();

	tagUISoundEffectData* pData = new tagUISoundEffectData;
	pData->bPullOnWater = false;
	pData->pSGNode = pNode;
	pData->eScaleType = EEST_Null;
	pData->eSoundType = eType;

	const bool bCanMove = SFXCanMove( szEffectName );
	DWORD dwID = GT_INVALID;

	if( !bCanMove )
	{

		UpdataEffectMatrix( pData, plp );
		
		pData->dwBindRoleID = GT_INVALID;
		dwID = AddEffect( pData, NULL, NULL );
	}
	else
	{
		pData->dwBindRoleID = plp->GetID();
		dwID = AddEffect( pData, plp, NULL );
	}

	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
	}
	return dwID;

}

/** \播放一个虚拟角色特效,返回特效ID,播放失败返回GT_INVALID
*/
DWORD EffectMgr::PlayVirtualEffect(DWORD dwRoleID,const TCHAR* szEffectName,const TCHAR* szTagNodeName/*=NULL*/, EEffectScaleType eScaleType/* = EST_Y*/, const TCHAR* szFolderName/* = _T("combat")*/, const bool bPullOnWater/* = false*/ )
{

	VirtualRole*pVp =  RoleMgr::Inst()->GetVirtualRole(dwRoleID);
	if( !P_VALID(pVp) || !pVp->IsInitFSM() )
		return GT_INVALID;

	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName, szFolderName );
	if( NULL == pNode )
		return GT_INVALID;

	pNode->Play();

	tagVirtualRoleEffectData* pData = new tagVirtualRoleEffectData;
	pData->bPullOnWater = bPullOnWater;
	pData->pSGNode = pNode;
	pData->eScaleType = eScaleType;
	const bool bCanMove = SFXCanMove( szEffectName );
	DWORD dwID = GT_INVALID;

	if( !bCanMove )
	{
		// 不可移动特效设置矩阵
		if( NULL != szTagNodeName && szTagNodeName[0] != _T('\0') )
		{
			Matrix4 mat = MathConst::IdentityMat;
			pVp->GetBoneSpace( szTagNodeName, mat );
			pData->pSGNode->SetWorldMat( &mat );
		}
		else
		{
			UpdataEffectMatrix( pData, pVp );
		}
		pData->dwBindRoleID = GT_INVALID;
		dwID = AddEffect( pData, NULL, NULL );
	}
	else
	{
		pData->dwBindRoleID = dwRoleID;
		dwID = AddEffect( pData, pVp, szTagNodeName );
	}

	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
	}
	return dwID;
}

/** \停止绑定在虚拟角色上的所有特效
*/
VOID EffectMgr::StopVirtualEffect(DWORD dwRoleID)
{
	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); )
	{
		tagBaseEffectData* pData = itr->second;
		if( P_VALID(pData) && P_VALID(pData->pSGNode) )
		{
			if( EET_VirtualRole == pData->eType && dwRoleID == pData->dwBindRoleID )
			{
				DeleteEffect( pData );
				itr = m_mapEffect.erase( itr );
				continue;
			}
		}

		++itr ;
	}
}


DWORD EffectMgr::PlayRoleEffect(DWORD dwRoleID,const TCHAR* szEffectName,const TCHAR* szTagNodeName/*=NULL*/, EEffectScaleType eScaleType/* = EST_Y*/, const TCHAR* szFolderName/* = _T("combat")*/, const bool bPullOnWater/* = false*/ )
{
	Role* pRole = RoleMgr::Inst()->FindRole( dwRoleID );
	if( !P_VALID(pRole) || !pRole->IsAttributeInited() )
		return GT_INVALID;

	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName, szFolderName );
	if( NULL == pNode )
		return GT_INVALID;

	pNode->Play();
	
	tagRoleEffectData* pData = new tagRoleEffectData;
	pData->bPullOnWater = bPullOnWater;
	pData->pSGNode = pNode;
	pData->eScaleType = eScaleType;

	const bool bCanMove = SFXCanMove( szEffectName );
	DWORD dwID = GT_INVALID;

	if( !bCanMove )
	{
		// 不可移动特效设置矩阵
		if( NULL != szTagNodeName && szTagNodeName[0] != _T('\0') )
		{
			Matrix4 mat = MathConst::IdentityMat;
			pRole->GetBoneSpace( szTagNodeName, mat );
			pData->pSGNode->SetWorldMat( &mat );
		}
		else
		{
			UpdataEffectMatrix( pData, pRole );
		}
		pData->dwBindRoleID = GT_INVALID;
		dwID = AddEffect( pData, NULL, NULL );
	}
	else
	{
		pData->dwBindRoleID = dwRoleID;
		dwID = AddEffect( pData, pRole, szTagNodeName );
	}
	
	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
	}
	return dwID;
}

DWORD EffectMgr::PlayLaunchEffect(const tagHitTargetEvent& evt,const TCHAR* szEffectName,const TCHAR* szTagNodeName/*=NULL*/)
{
	Role* pSrcRole = RoleMgr::Inst()->FindRole( evt.dwSrcRoleID );
	if( NULL == pSrcRole || NULL == pSrcRole->GetSceneNode() )
		return GT_INVALID;

	Role* pTarRole = RoleMgr::Inst()->FindRole( evt.dwTarRoleID );
	if( NULL == pTarRole || !pTarRole->IsAttributeInited() )
		return GT_INVALID;


	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName );
	if( NULL == pNode )
		return GT_INVALID;
	
	Vector3 vDest = GetBeAttackPos( pTarRole );

	// 起始点可以在绑定的骨骼处（如果没有绑定骨骼则使用默认位置）
	Vector3 vSrc = pSrcRole->GetPos();
	Matrix4 matBone = MathConst::IdentityMat;
	if( pSrcRole->GetBoneSpace( szTagNodeName, matBone ) )
	{
		vSrc.x = matBone._41;
		vSrc.y = matBone._42;
		vSrc.z = matBone._43;
	}
	else
	{
		AABBox boxSrc;
		pSrcRole->GetSceneNode()->GetBox( boxSrc );
		vSrc.y += ( boxSrc.max.y - boxSrc.min.y ) * g_fDefaultRoleEffYOffset;
	}
	pNode->ResetTargetPath( vSrc, vDest );

	pNode->Play();
	pNode->SetBoxCenter( pSrcRole->GetPos() );

	tagLaunchEffectData* pData = new tagLaunchEffectData;
	pData->pSGNode = pNode;
	pData->evtHitTarget = evt;

	const DWORD dwID = AddEffect( pData, NULL, NULL );
	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
	}
	return dwID;
}

DWORD EffectMgr::PlayBeAttackEffect(DWORD dwRoleID,const TCHAR* szEffectBaseName,const int nNumEffect, DWORD dwSrcRoleID, EHitTargetCause eCause, DWORD dwMisc, DWORD dwSerial, EEffectScaleType eScaleType /*= EEST_YOutOnly*/ )
{
	if( IsEffectNumFull() )
		return GT_INVALID;

	if( !P_VALID(szEffectBaseName) || _T('\0') == szEffectBaseName[0] || nNumEffect <= 0 )
		return GT_INVALID;

	Role* pRole = RoleMgr::Inst()->FindRole( dwRoleID );
	if( NULL == pRole || ! pRole->IsAttributeInited() )
		return GT_INVALID;

	TCHAR szEffectName[X_SHORT_NAME];
	int nRand = 0;
	if( nNumEffect > 1 )
		nRand = rand() % nNumEffect;
	_sntprintf( szEffectName, X_SHORT_NAME, _T("%s_%02d"), szEffectBaseName, nRand + 1 );

	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName );
	if( NULL == pNode )
		return GT_INVALID;

	pNode->Play();

	tagBeAttackEffectData* pData = new tagBeAttackEffectData;
	pData->pSGNode = pNode;
	pData->dwBindRoleID = GT_INVALID;
	pData->dwTarRoleID = dwRoleID;
	pData->dwSrcRoleID = dwSrcRoleID;
	pData->dwMisc = dwMisc;
	pData->dwSerial = dwSerial;
	pData->dwMisc2 = 0;
	pData->eCause = eCause;
	pData->eScaleType = eScaleType;

	Vector3 vEffPos = GetBeAttackPos( pRole );
	Vector3 vScale = GetRoleEffectScale( pData->eScaleType, pRole );
	if( vScale.x < 1.0f || vScale.y < 1.0f || vScale.z < 1.0f )// 只放大不缩小
		vScale = Vector3( 1.0f, 1.0f, 1.0f );
	Transform trans;
	trans.Scale( vScale.x, vScale.y, vScale.z );
	trans.Rotate( pRole->GetDestYaw(), 0, 0 );
	trans.Translate( vEffPos );
	pData->pSGNode->SetWorldMat( trans.GetMatrix() );

	const DWORD dwID = AddEffect( pData, NULL, NULL );
	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
	}

	//! 添加地面特效
	_sntprintf( szEffectName, X_SHORT_NAME, _T("%s_%02d_org"), szEffectBaseName, nRand + 1 );

	SGAttachableEffectNode* pNodeEx = LoadSFXFile( szEffectName );
	if( NULL != pNodeEx )
	{
		pNodeEx->Play();

		tagBeAttackEffectData* pDataEx = new tagBeAttackEffectData;
		pDataEx->pSGNode = pNodeEx;
		pDataEx->dwBindRoleID = GT_INVALID;
		pDataEx->dwTarRoleID = dwRoleID;
		pDataEx->dwSrcRoleID = dwSrcRoleID;
		pDataEx->dwMisc = dwMisc;
		pDataEx->dwSerial = dwSerial;
		pDataEx->dwMisc2 = 0;
		pDataEx->eCause = eCause;
		pDataEx->eScaleType = eScaleType;
		//tagRoleEffectData* pDataEx = new tagRoleEffectData;
		//pDataEx->pSGNode = pNodeEx;
		//pDataEx->dwBindRoleID = GT_INVALID;
		//pDataEx->eScaleType = eScaleType;

		vEffPos = pRole->GetPos();
		vScale = GetRoleEffectScale( pData->eScaleType, pRole );
		Transform trans1;
		trans1.Scale( vScale.x, vScale.y, vScale.z );
		trans1.Rotate( pRole->GetDestYaw(), 0, 0 );
		trans1.Translate( vEffPos );
		pDataEx->pSGNode->SetWorldMat( trans1.GetMatrix() );

		const DWORD dwIDEx = AddEffect( pDataEx, NULL, NULL );
		if( GT_INVALID == dwIDEx )
		{
			SAFE_DEL( pDataEx->pSGNode );
			SAFE_DEL( pDataEx );
		}
	}

	return dwID;
}

/** \根据dwRoleID,dwSrcRoleID和指定的技能停止播放角色所有该技能产生的被攻击特效
*/
VOID EffectMgr::StopBeAttackEffect(DWORD dwRoleID, DWORD dwSrcRoleID, EHitTargetCause eCause, DWORD dwMisc, DWORD dwSerial)
{
	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end();  itr++)
	{
		tagBaseEffectData* pData = itr->second;
		if( !P_VALID(pData) || !P_VALID(pData->pSGNode) )
			continue;
		if(pData->eType == EET_BeAttack) 
		{
			tagBeAttackEffectData* pAtkData = (tagBeAttackEffectData*)pData;
			if(pData->pSGNode->IsClosed()) continue;
			if((pAtkData->dwTarRoleID == dwRoleID || dwRoleID==GT_INVALID) && pAtkData->dwSrcRoleID == dwSrcRoleID 
				&& pAtkData->eCause == eCause && pAtkData->dwMisc == dwMisc && pAtkData->dwSerial == dwSerial)
			{
				if( GT_INVALID != pData->dwBindRoleID )
				{
					// 已经被删掉了
					Role* pRole = RoleMgr::Inst()->FindRole( pData->dwBindRoleID );
					if( !P_VALID( pRole ) || !P_VALID( pRole->GetSceneNode() ) )
						continue;
				}
				pData->pSGNode->Close();
			}
		}
	}
}

VOID EffectMgr::PlayBuffEffect( DWORD dwRoleID,DWORD dwBuffID, EEffectScaleType eScaleType /*= EEST_Y*/  )
{
    const tagBuffProtoClient* pBuff=SkillProtoData::Inst()->FindBuffProto(dwBuffID);
    if (!P_VALID(pBuff))
        return;

	if(pBuff->eEffect[EBEM_Persist] == EBET_NoMovement)
	{
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		pLocalPlayer->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Stand,AvatarAniMixer::EAP_All,_T("ze01"),false);
	}
	
    if(pBuff->szEffectMod[0] == _T('\0'))
        return;

    tagEffectSet *effectset = EffectSetData::Inst()->FindEffectSet(pBuff->szEffectMod);

	Role* pRole = RoleMgr::Inst()->FindRole( dwRoleID );
	if( !P_VALID(pRole) || !pRole->IsAttributeInited() )
	{
		//加入缓存，等待对应的角色属性初始化完毕
		m_mapNeedProcessBuff.insert( make_pair(dwRoleID,dwBuffID) );
		return;
	}

	// 如果有相同的Buff特效在播放则不再增加
    BOOL found = FALSE;
	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); itr++ )
	{
		if( EET_Buff != itr->second->eType )
			continue;
		tagBuffEffectData* pData = (tagBuffEffectData*)itr->second;
		if( dwRoleID == pData->dwBindRoleID && dwBuffID == pData->dwBuffID && NULL != pData->pSGNode )
		{
			pData->pSGNode->Play();
            found = TRUE;
		}
	}

    if(found)return;

    tagEffectSet defaulteset;
    if(!P_VALID(effectset))
    {
        effectset = &defaulteset;
        defaulteset.model_node.push_back(make_pair(tstring(pBuff->szEffectMod), tstring(_T(""))));
    }

    for(vector<pair<tstring, tstring> >::iterator it = effectset->model_node.begin(); 
        it != effectset->model_node.end(); ++it)
    {
        LPCTSTR NodeName = NULL;
        if(it->second != _T("")&&it->second != _T("-1"))
        {
            NodeName = it->second.c_str();
        }
        SGAttachableEffectNode* pNode = LoadSFXFile( it->first.c_str() );
        if( NULL == pNode )
            continue;

        pNode->Play();

        tagBuffEffectData* pData = new tagBuffEffectData;
        pData->pSGNode = pNode;

        pData->dwBuffID = dwBuffID;
        pData->eScaleType = eScaleType;

        const bool bCanMove = SFXCanMove( it->first.c_str() );
        DWORD dwID = GT_INVALID;

        if( !bCanMove )
        {
            // 不可移动特效设置矩阵
            UpdataEffectMatrix( pData, pRole );
            pData->dwBindRoleID = GT_INVALID;
            dwID = AddEffect( pData, NULL, NULL );
        }
        else
        {
            pData->dwBindRoleID = dwRoleID;
            dwID = AddEffect( pData, pRole, NodeName );
        }

        if( GT_INVALID == dwID )
        {
            SAFE_DEL( pData->pSGNode );
            SAFE_DEL( pData );
        }
    }
}

VOID EffectMgr::StopBuffEffect( DWORD dwRoleID,DWORD dwBuffID )
{
	Role* pRole = RoleMgr::Inst()->FindRole( dwRoleID );
	if( NULL == pRole )
		return;

	// 所有相同的BUFF特效都停止
	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); itr++ )
	{
		if( EET_Buff != itr->second->eType )
			continue;
		tagBuffEffectData* pData = (tagBuffEffectData*)itr->second;
		if( dwRoleID == pData->dwBindRoleID && dwBuffID == pData->dwBuffID && NULL != pData->pSGNode )
		{
			if( GT_INVALID != pData->dwBindRoleID )
			{
				// 已经被删掉了
				Role* pRole = RoleMgr::Inst()->FindRole( pData->dwBindRoleID );
				if( !P_VALID( pRole ) || !P_VALID( pRole->GetSceneNode() ) )
					break;
			}
			pData->pSGNode->Close();
		}
	}
}

VOID EffectMgr::StopEffect( const DWORD dwEffectID )
{
	EffecMap::iterator itr = m_mapEffect.find( dwEffectID );
	if( itr != m_mapEffect.end() )
	{
		tagBaseEffectData* pData = itr->second;

		// 已经被删掉了
		if( GT_INVALID != pData->dwBindRoleID )
		{
			Role* pRole = RoleMgr::Inst()->FindRole( pData->dwBindRoleID );
			if( !P_VALID( pRole ) || !P_VALID( pRole->GetSceneNode() ) )
				return;
		}
		pData->pSGNode->Close();
	}
}

VOID EffectMgr::RemoveEffect(const DWORD dwEffectID)
{
    EffecMap::iterator itr = m_mapEffect.find( dwEffectID );
    if( itr != m_mapEffect.end() )
    {
        DeleteEffect(itr->second);
        m_mapEffect.erase(itr);
    }
}

SGAttachableEffectNode* EffectMgr::LoadSFXFile( const TCHAR* szEffectName, const TCHAR* szFolderName/* = _T("combat")*/ )
{
	if( !P_VALID(szEffectName) || _T('\0') == szEffectName[0] )
		return NULL;

	// 如果传了文件夹名称就构造路径，否则直接使用szEffectName作为路径
	tstring strEffectPath;
	if( NULL != szFolderName )
	{
		strEffectPath = _T("data\\system\\effect\\");
		strEffectPath += szFolderName;
		strEffectPath += _T("\\");
		strEffectPath += szEffectName;
		strEffectPath += _T(".sfx");
	}
	else
	{
		strEffectPath = szEffectName;
	}

	// FOR DEBUG??
	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	DWORD dwFileSize = pIFS->GetSize( strEffectPath.c_str() );
	if( 0 == dwFileSize || GT_INVALID == dwFileSize )
	{
		TRACE( _T("SFX file have not found.file=%s\r\n"), strEffectPath.c_str() );
		return NULL;
	}

	SGAttachableEffectNode *pEffect=new SGAttachableEffectNode;
	pEffect->SetUserWorldMat( true );
	pEffect->LoadRes( strEffectPath.c_str() );
	pEffect->SetUserUpdate( true );
	pEffect->SetUserPlay( true );
	return pEffect;
}

DWORD EffectMgr::AddEffect( tagBaseEffectData* pData, Role* pBindRole, const TCHAR* szTagNodeName )
{
	// 没有绑定角色
	if( !P_VALID( pBindRole ) )
	{
		if( NULL == MapMgr::Inst()->GetSceneGraph() )
			return GT_INVALID;

		pData->bBindTagNode = false;
		MapMgr::Inst()->GetSceneGraph()->AttachDynamicNode( pData->pSGNode );
	}
	else
	{
		SceneNode* pRoleSGNode = pBindRole->GetSceneNode();
		ASSERT( NULL != pRoleSGNode );

		if(pBindRole->IS_KIND_OF(Player))
		{
			Player* pPlayer = static_cast<Player*>(pBindRole);
			if ( EET_VirtualRole == pData->eType )		
				pRoleSGNode = pPlayer->GetRealSceneNode();
		}

		if(!P_VALID(pRoleSGNode))
			return GT_INVALID;

		// 绑定骨骼
		if( NULL != szTagNodeName && _T('\0') != szTagNodeName[0] )
		{
			pData->bBindTagNode = true;
			pData->pSGNode->BindToTagNode( szTagNodeName );
		}
		pRoleSGNode->AttachChild( pData->pSGNode );
	}
	m_mapEffect[m_dwCurEffectID] = pData;
	return m_dwCurEffectID++;
}

VOID EffectMgr::DeleteEffect( tagBaseEffectData* pData )
{
	if( P_VALID( pData->pSGNode ) )
	{
		// 已经被删掉了
		if( GT_INVALID != pData->dwBindRoleID )
		{
			Role* pRole = NULL;
			if ( EET_VirtualRole == pData->eType )
				pRole = (VirtualRole*)RoleMgr::Inst()->GetVirtualRole( pData->dwBindRoleID );
			else
				pRole = RoleMgr::Inst()->FindRole( pData->dwBindRoleID );

			if( !P_VALID( pRole ) || !P_VALID( pRole->GetSceneNode() ) )
			{
				SAFE_DEL( pData );
				return;
			}

			SceneNode* pRoleSGNode = pRole->GetSceneNode();
			if(pRole->IS_KIND_OF(Player))
			{
				Player* pPlayer = static_cast<Player*>(pRole);
				if ( pPlayer->GetRoleState(ERS_Transform)   // 变身
					|| EET_VirtualRole == pData->eType )								//元神
					pRoleSGNode = pPlayer->GetRealSceneNode();	
			}
			if (!P_VALID(pRoleSGNode))
			{
				//SAFE_DEL( pData->pSGNode );
				pData->pSGNode = NULL; //绑定不存在证明已经删除
				SAFE_DEL( pData );
				return;
			}

		}

		SceneNode* pFather = (SceneNode*)pData->pSGNode->GetParent();
		if( NULL != pFather  )
			pFather->DetachChild( pData->pSGNode );
		else
			MapMgr::Inst()->GetSceneGraph()->DetachDynamicNode( pData->pSGNode );
		SAFE_DEL( pData->pSGNode );
	}
	SAFE_DEL( pData );
}

VOID EffectMgr::OnCloseMap( tagGameEvent* )
{
	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end();  )
	{
		tagBaseEffectData* pData = itr->second;
		if( GT_INVALID == pData->dwBindRoleID )
		{
			DeleteEffect( pData );
			itr = m_mapEffect.erase( itr );
		}
		else
		{
			itr++;
		}
	}
}

VOID EffectMgr::ShowEffect( const DWORD dwEffectID, const bool bShow )
{
	EffecMap::iterator itr = m_mapEffect.find( dwEffectID );
	if( itr != m_mapEffect.end() )
	{
		tagBaseEffectData* pData = itr->second;
		pData->pSGNode->Show( bShow );
	}
}

VOID EffectMgr::ShowBindRoleEffect( const DWORD dwBindRoleID, const bool bShow )
{
	if( !P_VALID( dwBindRoleID ) )
		return;

	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); itr++ )
	{
		tagBaseEffectData* pData = itr->second;
		if( dwBindRoleID == pData->dwBindRoleID )
		{
			pData->pSGNode->Show( bShow );
		}
	}	
}

BOOL EffectMgr::UpdataRoleEffect( tagBaseEffectData* pData )
{
	Role* pRole = NULL;
	if ( EET_VirtualRole == pData->eType )
		pRole = (VirtualRole*)RoleMgr::Inst()->GetVirtualRole( pData->dwBindRoleID );
	else
		pRole = RoleMgr::Inst()->FindRole( pData->dwBindRoleID );
	if( !P_VALID( pRole ) || !P_VALID( pRole->GetSceneNode() ) )
		return FALSE;

	if( pData->eType == EET_Bone )
	{
		UpdataBoneEffectMatrix( pData, pRole );
		return TRUE;
	}

	if(pData->eType == EET_VirtualRole )
	{
		if( P_VALID(pData->pSGNode) )
		{
			tstring strBoneName = pData->pSGNode->GetTagNodeName();
			UpdataVirtualEffectMatrix( pData, (Player*)pRole, strBoneName.c_str() );
		}	
		return TRUE;
	}

	SceneNode* pRoleSGNode = pRole->GetSceneNode();
	if(pRole->IS_KIND_OF(Player))
	{
		Player* pPlayer = static_cast<Player*>(pRole);
		if ( EET_VirtualRole == pData->eType )
			pRoleSGNode = pPlayer->GetRealSceneNode();	
	}

	if (!P_VALID(pRoleSGNode))
		return FALSE;

	if( pData->bBindTagNode )
		return TRUE;

	// 更新
	UpdataEffectMatrix( pData, pRole );

	return TRUE;
}

VOID EffectMgr::UpdataTargetPath( tagLaunchEffectData* pData, const float fTimeDelta )
{
	Role* pSrcRole = RoleMgr::Inst()->FindRole( pData->evtHitTarget.dwSrcRoleID );
	if( NULL == pSrcRole || NULL == pSrcRole->GetSceneNode() )
		return;

	Role* pTarRole = RoleMgr::Inst()->FindRole( pData->evtHitTarget.dwTarRoleID );
	if( NULL == pTarRole || !pTarRole->IsAttributeInited() )
		return;

	// 命中
	if( pData->pSGNode->IsTargetPathArrive() && !pData->bEventSended )
	{
		pData->pSGNode->Close();
		m_pGameFrameMgr->SendEvent( &pData->evtHitTarget );
		pData->bEventSended = true;
	}
	else
	{
		Vector3 vDest = GetBeAttackPos( pTarRole );
		pData->pSGNode->ChangeTargetPathTarget( vDest );
	}
}

VOID EffectMgr::PullOnWater( tagBaseEffectData* pData )
{
	Matrix4 matWorld = pData->pSGNode->GetWorldMat();
	Vector3 vPos( matWorld._41, matWorld._42, matWorld._43 );

	NavMap* pNav = MapMgr::Inst()->GetNavMap();
	if( NULL == pNav )
		return;
	NavCollider* pCollider = pNav->GetCollider();
	if( NULL == pCollider || !pCollider->IsLoaded() )
		return;
	float fWaterH = 0.0f;
	if( pCollider->GetWaterHeight( vPos, fWaterH ) )
	{
		if( matWorld._42 < fWaterH )
		{
			matWorld._42 = fWaterH;
			pData->pSGNode->SetWorldMat( &matWorld );
		}
	}
}

Vector3	EffectMgr::GetRoleEffectScale( const EEffectScaleType eScaleType, Role* pRole )
{
	ASSERT( P_VALID(pRole) && pRole->IsAttributeInited() );

	if( EEST_RoleScale == eScaleType )
	{
		return pRole->GetObjectSpace().GetScale();
	}

	Vector3 vScale = pRole->GetObjectSpace().GetScale();
	if( pRole->IS_KIND_OF(NPC) )
	{
		NPC* pNPC = (NPC*)pRole;
		const tagCreatureMdlData* pMdlData = pNPC->GetMdlData();
		if( NULL != pMdlData )
		{
			vScale *= pMdlData->fBeAttackSfxScale;
		}
	}
	if( vScale.x > 3.0f || vScale.y > 3.0f || vScale.z > 3.0f )
		vScale = Vector3( 3.0f, 3.0f, 3.0f );
	return vScale;
}

float EffectMgr::GetEffectScale( const EEffectScaleType eScaleType, const Vector3& vSize )
{
	float fScale = 1.0f;
	switch( eScaleType )
	{
	case EEST_Y:
		{
			fScale = vSize.y / g_vStdEffectRefSize.y;
		}
		break;
	case EEST_XYZ:
		{
			fScale = ( vSize.x / g_vStdEffectRefSize.x + vSize.y / g_vStdEffectRefSize.y + vSize.z / g_vStdEffectRefSize.z ) / 3.0f;
		}
		break;
	case EEST_XZ:
		{
			fScale = ( vSize.x / g_vStdEffectRefSize.x + vSize.z / g_vStdEffectRefSize.z ) * 0.5f;
		}
		break;
	case EEST_YOutOnly:
		{
			fScale = vSize.y / g_vStdEffectRefSize.y;
			if( fScale < 1.0f )
				fScale = 1.0f;
		}
		break;
	}
	return fScale;
}

Vector3	EffectMgr::GetBeAttackPos( Role* pRole )//, BOOL isOrg /*= FALSE*/)
{
	ASSERT( P_VALID(pRole) && (pRole->IsAttributeInited()) );

	tstring strBoneName;
	if( pRole->GetSceneNode()->IS_KIND_OF(AvatarNode) )
	{
		//if (!isOrg)
		//	strBoneName = _T("Bip01 Spine2");
		//else
			strBoneName = _T("Bip01 Spine2");
	}
	else if( pRole->IS_KIND_OF(NPC) )
	{
		NPC* pNPC = (NPC*)pRole;
		//if (!isOrg)
		//	strBoneName = CreatureData::Inst()->FindNpcBoneName( pNPC->GetTypeID(), _T("tag_Back") );
		//else
			strBoneName = CreatureData::Inst()->FindNpcBoneName( pNPC->GetTypeID(), _T("tag_Back") );
		
	}

	Matrix4 matBone = MathConst::IdentityMat;
	if( pRole->GetBoneSpace( strBoneName.c_str(), matBone ) )
	{
		return Vector3( matBone._41, matBone._42, matBone._43 );
	}
	else
	{
		AABBox boxRole;
		pRole->GetSceneNode()->GetBox( boxRole );
		Vector3 vPos = pRole->GetPos();
		vPos.y += ( boxRole.max.y - boxRole.min.y ) * g_fDefaultRoleEffYOffset;
		return vPos;
	}
}

bool EffectMgr::SFXCanMove( const TCHAR* szFileName )
{
	if( !P_VALID(szFileName) || _T('\0') == szFileName[0] )
		return false;

	tstring strFileName = szFileName;
	tstring::size_type pos1 = strFileName.find_last_of( _T('\\') );
	tstring::size_type pos2 = strFileName.rfind( _T('.') );
	if( tstring::npos == pos1 )
		pos1 = 0;
	else
		pos1++;
	if( tstring::npos == pos2 )
		pos2 = strFileName.length();
		
	tstring strSFXName = strFileName.substr( pos1, pos2 - pos1 );
	const tagSFXData* pSFXData = SFXData::Inst()->FindData( strSFXName.c_str() );
	if( NULL == pSFXData )
		return true;

	return pSFXData->bCanMove;
}

VOID EffectMgr::UpdataEffectMatrix( tagBaseEffectData* pData, Role* pRole )
{
	ASSERT( P_VALID( pData ) );
	ASSERT( P_VALID(pRole) && pRole->IsAttributeInited() );

	Transform trans;
	Vector3 vScale = GetRoleEffectScale( pData->eScaleType, pRole );
	trans.Scale( vScale.x, vScale.y, vScale.z );
	trans.Rotate( pRole->GetDestYaw(), 0, 0 );
	trans.Translate( pRole->GetPos() );
	pData->pSGNode->SetWorldMat( trans.GetMatrix() );
}

bool EffectMgr::IsEffectNumFull()
{
	const float r = (float)GameSet::Inst()->Get( EGS_EffectDetail ) / SGEffectNode::MAX_LOD;
	const size_t nMaxEffectNum = (size_t)( MIN_EFFECT_NUM + r * ( MAX_EFFECT_NUM - MIN_EFFECT_NUM ) );
	return m_mapEffect.size() >= nMaxEffectNum;
}

void EffectMgr::PlayCacheBuff( DWORD dwRoleID )
{
	std::map<DWORD,DWORD>::iterator it = m_mapNeedProcessBuff.find( dwRoleID );
	if( it == m_mapNeedProcessBuff.end() )
		return;

	EffectMgr::Inst()->PlayBuffEffect( dwRoleID, it->second );
	m_mapNeedProcessBuff.erase( it );
}



DWORD EffectMgr::PlayRoleEquipEffect(DWORD dwRoleID,const TCHAR* szEffectName,const TCHAR* szBoneName/*=NULL*/, EBoneEffectType eType /*= EBET_Mantle*/, EEffectScaleType eScaleType/* = EST_Y*/, const TCHAR* szFolderName/* = _T("combat")*/, const bool bPullOnWater/* = false*/ )
{
	Role* pRole = RoleMgr::Inst()->FindRole( dwRoleID );
	if( !P_VALID(pRole) || !pRole->IsAttributeInited() )
		return GT_INVALID;

	// 增加新的特效之前先把上一个相同骨骼位置的特效关闭，一块骨骼上同时只允许挂载一种特效
	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); itr++ )
	{
		if( EET_Bone != itr->second->eType )
			continue;

		tagBoneEffectData* pData = (tagBoneEffectData*)itr->second;
		if( dwRoleID == pData->dwBindRoleID && eType == pData->eType && NULL != pData->pSGNode )
		{
			if( GT_INVALID != pData->dwBindRoleID )
			{
				// 已经被删掉了
				Role* pRole = RoleMgr::Inst()->FindRole( pData->dwBindRoleID );
				if( !P_VALID( pRole ) || !P_VALID( pRole->GetSceneNode() ) )
					break;
			}
			pData->pSGNode->Close();
		}
	}


	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName, szFolderName );
	if( NULL == pNode )
		return GT_INVALID;

	pNode->Play();
	tagBoneEffectData* pData = new tagBoneEffectData;
	pData->bPullOnWater = bPullOnWater;
	pData->pSGNode = pNode;
	pData->eScaleType = eScaleType;
	pData->bIsBoneNode = true;
	pData->szBoneName = szBoneName;
	pData->eType = eType;

	const bool bCanMove = SFXCanMove( szEffectName );
	DWORD dwID = GT_INVALID;

	if( !bCanMove )
	{
		// 不可移动特效设置矩阵
// 		if( NULL != szTagNodeName && szTagNodeName[0] != _T('\0') )
// 		{
// 			Matrix4 mat = MathConst::IdentityMat;
// 			pRole->GetBoneSpace( szTagNodeName, mat );
// 			pData->pSGNode->SetWorldMat( &mat );
// 		}
// 		else
// 		{
// 			UpdataEffectMatrix( pData, pRole );
// 		}
// 		pData->dwBindRoleID = GT_INVALID;
// 		dwID = AddEffect( pData, NULL, NULL );
	}
	else
	{
		pData->dwBindRoleID = dwRoleID;
		dwID = AddBoneEffect( pData, pRole, szBoneName );
	}

	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
	}
	return dwID;
}

DWORD EffectMgr::AddBoneEffect( tagBoneEffectData* pData, Role* pBindRole, const TCHAR* szBoneName )
{
	// 没有绑定角色
	if( !P_VALID( pBindRole ) )
	{
		if( NULL == MapMgr::Inst()->GetSceneGraph() )
			return GT_INVALID;

		pData->bBindTagNode = false;
		MapMgr::Inst()->GetSceneGraph()->AttachDynamicNode( pData->pSGNode );
	}
	else
	{
		SceneNode* pRoleSGNode = pBindRole->GetSceneNode();
		ASSERT( NULL != pRoleSGNode );

		// 绑定骨骼
		if( NULL != szBoneName && _T('\0') != szBoneName[0] )
		{
			pData->bIsBoneNode = true;
			pData->pSGNode->BindToTagNode( szBoneName );
		}
		pRoleSGNode->AttachChild( pData->pSGNode );
	}
	m_mapEffect[m_dwCurEffectID] = pData;
	return m_dwCurEffectID++;
}

VOID EffectMgr::UpdataBoneEffectMatrix( tagBaseEffectData* pData, Role* pRole )
{
	ASSERT( P_VALID( pData ) );
	ASSERT( P_VALID(pRole) && pRole->IsAttributeInited() );

	tstring ss = static_cast<tagBoneEffectData*>(pData)->szBoneName;

	Matrix4 mat = MathConst::IdentityMat;
	pRole->GetBoneSpace( ss.c_str(), mat );
	pData->pSGNode->SetWorldMat( &mat );
}

VOID EffectMgr::UpdataVirtualEffectMatrix( tagBaseEffectData* pData, Player* pRole, const TCHAR* szBoneName )
{
	//tstring ss = static_cast<tagBoneEffectData*>(pData)->szBoneName;
	SceneNode* pSGNode = pRole->GetRealSceneNode();
	if( P_VALID(pSGNode) && pSGNode->IS_STATIC_CLASS(EntityNode) )
	{
		EntityNode* pEntity = (EntityNode*)pSGNode;
		ResRenderable* pRes = pEntity->GetResObj();
		if( NULL == pRes || !pRes->IsCreated() )
			return;

		if( !pRes->IS_STATIC_CLASS(ResSkeleton) )
			return;

		ResSkeleton *pSkel=(ResSkeleton*)pRes;

		const int nBoneID = pSkel->GetBoneID( szBoneName );
		if( -1 == nBoneID )
			return;
		
		Matrix4 mat = MathConst::IdentityMat;
		NodeAnimationCtrl* pAni = (NodeAnimationCtrl*)pEntity->GetAniCtrl();
		mat = *pAni->GetBoneSpace( nBoneID );
		mat *= pEntity->GetWorldMat();
		pData->pSGNode->SetWorldMat( &mat );
	}
}

VOID EffectMgr::StopRoleEquipEffect(DWORD dwRoleID,EBoneEffectType eType)
{
	Role* pRole = RoleMgr::Inst()->FindRole( dwRoleID );
	if( NULL == pRole )
		return;

	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); itr++ )
	{
		if( EET_Bone != itr->second->eType )
			continue;

		tagBoneEffectData* pData = (tagBoneEffectData*)itr->second;
		if( dwRoleID == pData->dwBindRoleID && eType == pData->eType && NULL != pData->pSGNode )
		{
			if( GT_INVALID != pData->dwBindRoleID )
			{
				// 已经被删掉了
				Role* pRole = RoleMgr::Inst()->FindRole( pData->dwBindRoleID );
				if( !P_VALID( pRole ) || !P_VALID( pRole->GetSceneNode() ) )
					break;
			}
			pData->pSGNode->Close();
		}
	}
}


VOID EffectMgr::StopUiSoundEffect(GUIEffectMgr::EEffectType eType)
{
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(plp))
		return;

	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); itr++ )
	{
		if( EET_UiSound != itr->second->eType )
			continue;

		tagUISoundEffectData* pData = (tagUISoundEffectData*)itr->second;
		if( eType == pData->eSoundType && NULL != pData->pSGNode )
		{
			pData->pSGNode->Close();
		}
	}
}

const tstring EffectMgr::GetGlowName( const TCHAR* szBaseName, const BYTE byVal ) const
{
	if( NULL == szBaseName || _T('\0') == szBaseName[0] )
		return _T("");

	int n = 0;
	if( byVal < 4 )	return _T("");
	else if( byVal < 10 )	n = 1;
	else if( byVal < 18 )	n = 2;
	else if( byVal < 25 )	n = 3;
	else if( byVal < 30 )	n = 4;
	else if( byVal < 35 )	n = 5;
	else if( byVal < 40 )	n = 6;
	else if( byVal < 50 )	n = 7;
	else if( byVal < 60 )	n = 8;
	else if( byVal < 70 )	n = 9;
	else if( byVal < 80 )	n = 10;
	else if( byVal < 100 )	n = 11;
	else if( byVal == 100 )	n = 12;
	else	return _T("");

	TCHAR szBuff[X_SHORT_NAME];
	_sntprintf( szBuff, X_SHORT_NAME, _T("%s_%02d"), szBaseName, n );
	return szBuff;
}

VOID EffectMgr::UpdateBeAtkEffectMsg(tagBeAttackEffectData* pData)
{
	if(pData->pSGNode->IsClosed())
	{
		tagEffectHPChangeEvent evt;
		evt.dwSrcRoleID = pData->dwSrcRoleID;
		evt.dwTarRoleID = pData->dwTarRoleID;
		evt.dwMisc = pData->dwMisc;
		evt.dwSerial = pData->dwSerial;
		evt.dwMisc2 = 5;
		evt.eCause = pData->eCause;
		m_pGameFrameMgr->SendEvent( &evt);
	}
	else
	{
		const std::vector<EffectMsg::tagState>& vFrmMsg = pData->pSGNode->GetFrmMsgArray();

		for(std::vector<EffectMsg::tagState>::const_iterator it=vFrmMsg.begin(); it!=vFrmMsg.end();it++)
		{
			if(_tcsncmp(it->szMsg,_T("bak"),3)==0)
			{
				tagEffectHPChangeEvent evt;
				evt.dwSrcRoleID = pData->dwSrcRoleID;
				evt.dwTarRoleID = pData->dwTarRoleID;
				evt.dwMisc = pData->dwMisc;
				evt.dwSerial = pData->dwSerial;
				evt.dwMisc2 = pData->dwMisc2++;
				evt.eCause = pData->eCause;
				m_pGameFrameMgr->SendEvent( &evt);
			}
		}
	}
}

DWORD EffectMgr::PlayWinEffect( DWORD dwFromRoleID, DWORD dwTargetID, const TCHAR* szEffectName )
{
	Role* pSrcRole = RoleMgr::Inst()->FindRole( dwFromRoleID, false );
	if( NULL == pSrcRole || NULL == pSrcRole->GetSceneNode() || !pSrcRole->IsAttributeInited() )
		return GT_INVALID;

	Role* pTarRole = RoleMgr::Inst()->FindRole( dwTargetID, false );
	if( NULL == pTarRole || !pTarRole->IsAttributeInited() )
		return GT_INVALID;

	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName );
	if( NULL == pNode )
		return GT_INVALID;

	Vector3 vDest = GetWinEffectPos( pTarRole, true );
	Vector3 vSrc = GetWinEffectPos( pSrcRole, false );
	pNode->ResetTargetPath( vSrc, vDest );

	pNode->Play();
	pNode->SetBoxCenter( pSrcRole->GetPos() );

	tagWinEffectData* pData = new tagWinEffectData;
	pData->dwSrcRoleID = dwFromRoleID;
	pData->dwTarRoleID = dwTargetID;
	pData->pSGNode = pNode;

	const DWORD dwID = AddEffect( pData, NULL, NULL );
	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
	}
	return dwID;
}

Vector3 EffectMgr::GetWinEffectPos( Role* pRole, bool bTar )
{
	tstring strBoneName;
	if( pRole->GetSceneNode()->IS_KIND_OF(AvatarNode) )
	{
		if( bTar )
			strBoneName = g_strTagNodeName[ETN_RHd].c_str();
		else
			strBoneName = g_strTagNodeName[ETN_Back].c_str();
	}
	else if( pRole->IS_KIND_OF(NPC) )
	{
		NPC* pNPC = (NPC*)pRole;
		if( bTar )
			strBoneName = CreatureData::Inst()->FindNpcBoneName( pNPC->GetTypeID(), g_strTagNodeName[ETN_RHd].c_str() );
		else
			strBoneName = CreatureData::Inst()->FindNpcBoneName( pNPC->GetTypeID(), g_strTagNodeName[ETN_Back].c_str() );
	}

	Vector3 vPos(0,0,0);
	Matrix4 matBone = MathConst::IdentityMat;
	if( pRole->GetBoneSpace( strBoneName.c_str(), matBone ) )
	{
		vPos.x = matBone._41;
		vPos.y = matBone._42;
		vPos.z = matBone._43;
	}
	else
	{
		AABBox boxRole;
		pRole->GetSceneNode()->GetBox( boxRole );
		vPos = pRole->GetPos();
		vPos.y += ( boxRole.max.y - boxRole.min.y ) * g_fDefaultRoleEffYOffset;
	}

	return vPos;
}

void EffectMgr::UpdateTargetPath4Win( tagWinEffectData* pData, const float fTimeDelta )
{
	Role* pTarRole = RoleMgr::Inst()->FindRole( pData->dwTarRoleID, false );
	if( NULL == pTarRole || !pTarRole->IsAttributeInited() )
		return;

	// 命中
	if( pData->pSGNode->IsTargetPathArrive() )
	{
		pData->pSGNode->Close();
	}
	else
	{
		Vector3 vDest = GetWinEffectPos( pTarRole, true );
		pData->pSGNode->ChangeTargetPathTarget( vDest );
	}
}

VOID EffectMgr::ClearSenceEffect()
{
	DWORD dwLpID = RoleMgr::Inst()->GetLocalPlayerID();
	if ( GT_INVALID == dwLpID )
		return;

	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end();  )
	{
		tagBaseEffectData* pData = itr->second;
		if( P_VALID(pData) )
		{
			if ( pData->dwBindRoleID != dwLpID )  //本地玩家的不删
			{
				DeleteEffect( pData );
				itr = m_mapEffect.erase(itr);
				continue;
			}
		}
		++itr;
	}
}