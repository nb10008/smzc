#include "StdAfx.h"
#include "clientcamera.h"
#include "Role.h"
#include "SDOAigw.h"
#include "MapMgr.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "GameSet.h"
ClientCamera::ClientCamera(void)
{
	m_bQuaking = false;
	m_fZFar = 25600.0f;
	m_fOccultDist = FLOAT_MAX;
	m_bFly = false;
	m_dwLastOccultTime = timeGetTime();
	m_bDecTransparent = FALSE;		
	m_fTransparent = 1.0f;
    m_HeightScaleRate = 1.0f;
	StopUpdate = false;
	m_eCamCtrlState = ECCS_UserContril;
	mTotalDuration = 10;
}


CONST FLOAT CCamera_ResetSpeed = FLOAT_PI/4;//镜头回复速度（弧度/秒）
CONST FLOAT CCamera_ZNear = 20;
CONST FLOAT MIN_DIST = 180.0f;	// 摄像机离人的最近距离
CONST FLOAT MIN_EXP_DIST = 200.0f;	// 手动控制摄像机时离人的最近距离

// 震动动画
CONST DWORD QUAKE_TIME = 300; // 动画时长
CONST int QUAKE_FRM_NUM = 10; // 帧数
POINT QUAKE_FRMS[QUAKE_FRM_NUM] = // 镜头偏移
{
	{0,5},	{0,-5},	{0,5},	{0,-5},	{0,5},	{0,-5},	{0,4},	
	{0,-4},	{0,4},	{0,-4},
};


void ClientCamera::SetDefault(float zFar)
{
	SetPerspective((FLOAT)Device()->GetClientWidth(),
		(FLOAT)Device()->GetClientHeight(),_DegToRad(45),CCamera_ZNear,zFar);
	Set(Vector3(0,500,0),0.39793599f,-0.18762225f,0,660);
	m_fExpectDist = m_fDist;
}

void ClientCamera::SetFov(float deg)
{
	SetPerspective((FLOAT)Device()->GetClientWidth(),
		(FLOAT)Device()->GetClientHeight(),_DegToRad(deg),CCamera_ZNear,m_fZFar);
	m_fExpectDist = m_fDist;
}


void ClientCamera::Update(Role* pLookRole, NavMap* pNavMap)
{
	// m_pConsole->Watch(_T("CameraDist"), (DWORD)GetDist());
	if(StopUpdate)
		return;

	if ( ECCS_Roam == m_eCamCtrlState)
	{
		if (m_pInput->IsKeyDown(DIK_F4))
			StopPathAni();

		UpatePathAni(Kernel::Inst()->GetDeltaTime());
	} 
	else if (ECCS_UserContril == m_eCamCtrlState)
	{
		// 鼠标滚轮
		TObjRef<GUISystem> pGUI;
		if( m_pInput->GetRelativeZ()!=0 && pGUI->GetMousePoint()==pGUI->GetDesktop() )
		{
			if( SDOAIgw::Inst()->IsGetFocus() )
				return;
			float delta = -m_pInput->GetRelativeZ() / 2.0f;
			m_fExpectDist += delta;

			if( m_fExpectDist > 1800 )
				m_fExpectDist = 1800;

			if( m_fExpectDist < MIN_EXP_DIST )
				m_fExpectDist = MIN_EXP_DIST;

			// 2.5D模式下，俯仰角设置
			if( m_b25D )
			{
				float fPitch = 0;
				if( m_fExpectDist < MIN_EXP_DIST )
				{
					fPitch = _DegToRad( 10 );
				}
				else if( m_fExpectDist > 1800 )
				{
					fPitch = -FLOAT_PI * 0.17f;
				}
				else
				{
					float fDiff = 1800.0f - m_fExpectDist;
					float fDeg = -30.0f + fDiff/40.0f;
					fPitch = _DegToRad( fDeg );
				}
				SetPitch( fPitch );
			}

			// 如果玩家正在拉动摄像机，则跳过2秒延时
			m_dwLastOccultTime = 0;
		}
		static int showcursorcount = 0;
		// 按下鼠标右键瞬间
		if( m_pInput->IsMouseDownOnce(1) )
		{
			HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));
			::SetCapture(hWnd);
			::GetCursorPos(&m_ptLast);
			if (showcursorcount>=0)
				showcursorcount = ::ShowCursor(FALSE);
		}

		// 松开鼠标右键瞬间
		if( m_pInput->IsMouseUpOnce(1) )
		{
			::ReleaseCapture();
			if(showcursorcount<0)
				showcursorcount = ::ShowCursor(TRUE);
		}

		//--按下鼠标右键并拖动
		if( m_pInput->IsMouseDown(1) && m_pInput->IsMouseMove() )
		{
			float yaw = GetYaw() - (-m_pInput->GetRelativeX()*1.0f) * 0.6f * 0.006545f;
			SetYaw(yaw);

			// 3D模式 可以俯仰
			if( !m_b25D )
			{
				float pitch = GetPitch() + (-m_pInput->GetRelativeY()*1.0f) * 0.006545f;
				if( pitch < -FLOAT_PI*0.45f )
					pitch = -FLOAT_PI*0.45f;
				if( pitch>_DegToRad(80) )
					pitch=_DegToRad(80);

				if( GetDist() > MIN_DIST || m_pInput->GetRelativeY() > 0 )
				{
					SetPitch(pitch);	// 距离太紧不允许镜头下拉
				}
			}

			::SetCursorPos(m_ptLast.x, m_ptLast.y);

			if(m_pInput->GetRelativeX()!=0)
			{
				tagRotateCameraEvent event(_T("tagRotateCameraEvent"),NULL);
				m_pFrmMgr->SendEvent(&event);
			}
		}

		//--飞行
		if(m_bFly)
		{
			if(m_pInput->IsKeyDown(DIK_UP))
			{
				Vector3 newPos=GetLookat()+GetForward()*40.0f;
				SetLookAtPos(newPos);
			}
			if(m_pInput->IsKeyDown(DIK_DOWN))
			{
				Vector3 newPos=GetLookat()-GetForward()*40.0f;
				SetLookAtPos(newPos);
			}
			if(m_pInput->IsKeyDown(DIK_LEFT))
			{
				Vector3 newPos=GetLookat()+GetSide()*40.0f;
				SetLookAtPos(newPos);
			}
			if(m_pInput->IsKeyDown(DIK_RIGHT))
			{
				Vector3 newPos=GetLookat()-GetSide()*40.0f;
				SetLookAtPos(newPos);
			}

			CameraEuler::UpdateViewMatrix();
			return;
		}

		//! test 
		if (m_pInput->IsKeyDown(DIK_F1))
		{
			TCamPathNode node;
			node.mPos = GetLookat();
			node.mRot = GetDirection();
			AddCamPoint(node);
		}
		else if (m_pInput->IsKeyDown(DIK_F2))
		{
			m_vecCamPathNodes.clear();
		}
		else if (m_pInput->IsKeyDown(DIK_F3))
		{
			GenerateCamPathData();
		}
		else if (m_pInput->IsKeyDown(DIK_F4))
		{
		}
		else if (m_pInput->IsKeyDown(DIK_F5))
		{
			PlayPathAni(false);
		}
		else if (m_pInput->IsKeyDown(DIK_F6))
		{
			PlayPathAni(true);
		}



		if( NULL == pLookRole )
			return;

		// 计算m_vLookAt
		Vector3 nextLookAt = pLookRole->GetPos();
		float height = 130.0f * m_HeightScaleRate;

		nextLookAt.y += height;

		// 镜头震动影响m_vLookAt
		UpdateQuake(nextLookAt);
		m_vLookat = nextLookAt;

		// m_vLookAt不能低于水面
		float waterH=0.0f;
		if( pNavMap->GetCollider()->GetWaterHeight(m_vLookat, waterH) )
		{
			waterH += 10.0f;
			if( m_vLookat.y < waterH )
				m_vLookat.y = waterH;
		}

		// 计算遮挡，实际是计算dist(from)
		Occult(pNavMap);
		CameraEuler::UpdateViewMatrix();	// 预先更新，计算m_vLookFrom

		// 根据 dist 修正 m_vLookAt
		FLOAT fUp = (GetDist() - MIN_DIST) / 10;
		if( fUp < 140 )	// 最大提高量
			m_vLookat.y += fUp;
		else
			m_vLookat.y += 140;

		// 最后更新矩阵
		//	Occult(pNavMap);
		//	CameraEuler::UpdateViewMatrix();

		// m_vLookFrom不能低于水面
		if( pNavMap->GetCollider()->GetWaterHeight(m_vFrom, waterH) )
		{
			waterH += 10.0f;
			if( m_vFrom.y < waterH )
				m_vFrom.y = waterH;
		}
		// m_vFrom不能低于地面
		float fTrnHeight = pNavMap->GetCollider()->SafeGetGroundHeightSlerp(m_vFrom.x, m_vFrom.z);
		// 计算from点和lookat点之间是否被遮挡，如果被遮挡就不修正from点的距离，防止镜头不停抖动！
		Vector3 dir = m_vFrom - m_vLookat;
		HugeQuadTreeSG* pHQT = (HugeQuadTreeSG*)MapMgr::Inst()->GetSceneGraph();
		TResult result = pHQT->RayCollide(Ray(dir, m_vFrom));
		if (!result.first)
		{
			if (m_vFrom.y < fTrnHeight + 10)
			{
				m_vFrom.y = fTrnHeight + 10;
			}
		}

		CameraBase::SetLookAt(m_vFrom,m_vLookat,m_vUp);
	}

	//UpdateRoleTransparent();
}


void ClientCamera::PlayQuake()
{
	if(!m_bQuaking)
	{
		m_dwQuakeStartTime=Kernel::Inst()->GetAccumTimeDW();
		m_bQuaking=true;
	}
}


void ClientCamera::UpdateQuake(Vector3& nextLookAt)
{
	if(!m_bQuaking)
		return;
	else if( !GameSet::Inst()->Get(EGS_CombatMusic) )
	{
		m_bQuaking = false;
		return;
	}

	DWORD time=Kernel::Inst()->GetAccumTimeDW()-m_dwQuakeStartTime;
	if(time<=QUAKE_TIME)
	{
		int frm=int(QUAKE_FRM_NUM*time/QUAKE_TIME);
		if(frm<0)
			frm=0;
		if(frm>=QUAKE_FRM_NUM)
			frm=QUAKE_FRM_NUM-1;
		nextLookAt.x+=QUAKE_FRMS[frm].x;
		nextLookAt.y+=QUAKE_FRMS[frm].y;
	}
	else
	{
		m_bQuaking=false;
	}
}


void ClientCamera::Occult(NavMap *pNavMap)
{
	if( !pNavMap )
		return;

	// 得到当前方向和距离
	Vector3 dir = m_vFrom - m_vLookat;
	float fCurrentLen = Vec3Len(&dir);
	dir /= fCurrentLen;

	// 是否需要尝试拉远
	if( fCurrentLen < (m_fExpectDist - 1) )// && (timeGetTime() - m_dwLastOccultTime > 2000) )	// 2秒后开始回复
	{
		const float SpringSpeed = 1000.0f;//每秒恢复距离
		float deltaTime=Kernel::Inst()->GetDeltaTime();

		fCurrentLen += SpringSpeed * deltaTime;
		if( fCurrentLen > m_fExpectDist )
			fCurrentLen = m_fExpectDist;
	}

	if( fCurrentLen > m_fExpectDist )
		fCurrentLen = m_fExpectDist;

	// 开始检测阻挡
	// 镜头和目标点的连线不能落到地面以下, 尝试使用NavMap自带的碰撞检测函数
	float f = 0;
	for(f=0; f<=fCurrentLen; f+=10.0f)
	{
		Vector3 current = m_vLookat + dir * f;
		float h = pNavMap->GetCollider()->SafeGetGroundHeightSlerp(current.x, current.z) + 30.0f;
		if( current.y < h )
		{
			m_dwLastOccultTime = timeGetTime();
			break;
		}
	}
	if( f > fCurrentLen )
		f = fCurrentLen;

	//--镜头和水面碰撞
	Vector3 intersectPos;
	if(pNavMap->GetCollider()->RayCollideWater(Ray(dir, m_vLookat, f),intersectPos))
	{
		intersectPos.y+=30.0f;//比水面稍高一点
		Vector3 temp = intersectPos - m_vLookat;
		f = Vec3Len(&temp);
		m_dwLastOccultTime = timeGetTime();
	}

	// 镜头和目标点的连线不能有障碍物
	HugeQuadTreeSG* pHQT = (HugeQuadTreeSG*)MapMgr::Inst()->GetSceneGraph();
	TResult result = pHQT->RayCollide(Ray(dir, m_vLookat));
	if( result.first )
	{
		if( result.second - 80.0f < f )
		{
			f = result.second - 80.0f;
			m_dwLastOccultTime = timeGetTime();
		}
	}

	if( f < MIN_DIST )	// 控制最小距离
	{
		f = MIN_DIST;
		m_bDecTransparent = TRUE;
	}
	else
	{
		m_bDecTransparent = FALSE;
	}

	SetDist(f);
}


void ClientCamera::UpdateRoleTransparent()
{
	if( !m_bDecTransparent )
	{
		if( m_fTransparent < 1.0f )
		{
			m_fTransparent += Kernel::Inst()->GetDeltaTime()*4.0f;
			if( m_fTransparent > 1.0f )
				m_fTransparent = 1.0f;
			RoleMgr::Inst()->GetLocalPlayer()->SetTransparent(m_fTransparent);
		}
	}
	else
	{
		if( m_fTransparent > 0.0f )
		{
			m_fTransparent -= Kernel::Inst()->GetDeltaTime()*4.0f;
			if( m_fTransparent < 0.0f )
				m_fTransparent = 0.0f;
			RoleMgr::Inst()->GetLocalPlayer()->SetTransparent(m_fTransparent);
		}
	}

}

VOID ClientCamera::GenerateCamPathData()
{
	if(m_vecCamPathNodes.empty())
		return;

	mCurve.clear();
	mRotCurve.clear();

	// 循环播放，把第一个点再插入一遍
	//m_vecCamPathNodes.push_back(m_vecCamPathNodes[0]);

	std::vector<TCamPathNode>::iterator it = m_vecCamPathNodes.begin();
	for ( ;it != m_vecCamPathNodes.end(); ++it)
	{
		mCurve.addPoint( it->mPos );
		mRotCurve.addPoint( it->mRot );
	}

	//创建曲线
	mCurve.setAutoCalculate(false);
	mCurve.recalcTangents();
	mRotCurve.setAutoCalculate(false);
	mRotCurve.recalcTangents();

}

VOID ClientCamera::SetPlayDuration( float duration )
{
	mTotalDuration = duration;
}
VOID ClientCamera::PlayPathAni( bool loop )
{
	mbLoop = loop;
	mCurAniTime = 0.0f;
	m_eCamCtrlState = ECCS_Roam;
}
VOID ClientCamera::StopPathAni()
{
	m_eCamCtrlState = ECCS_UserContril;
}

VOID ClientCamera::UpatePathAni( float deltaTime )
{
	if (m_vecCamPathNodes.empty())
		return;

	mCurAniTime += deltaTime;
	//计算百分比
	float tTimePer = mCurAniTime / mTotalDuration;
	//计算摄像机位置
	if( tTimePer< 0.95f )
	{
		Vector3 tNewPos = mCurve.interpolate( tTimePer );
		SetLookAtPos(tNewPos);
		Quaternion tNewRot = mRotCurve.interpolate( tTimePer );
		SetDirection(tNewRot);

		CameraEuler::UpdateViewMatrix();
	}
	else
	{
		if( mbLoop )
			mCurAniTime = 0.0f;
		else
			StopPathAni();
	}

}