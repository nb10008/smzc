#include "StdAfx.h"
#include "KillingFrame.h"
#include "..\WorldDefine\role_att.h"
#include "GUIEffectMgr.h"
#include "BeAttackFrame.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"

const DWORD SHOW_TIME = 6000;
const DWORD EXPLODE_FADE_TIME = 500;

KillingFrame::KillingFrame(void):m_Trunk(this)
{
	m_pTex = NULL;
}

KillingFrame::~KillingFrame(void)
{
}
BOOL KillingFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	m_pCmdMgr->Register("NS_SendRoleKillCount", (NETMSGPROC)m_Trunk.sfp2(&KillingFrame::OnNS_SendRoleKillCount), _T("NS_SendRoleKillCount") );
	m_pFrameMgr->RegisterEventHandle( _T("OpenCloseKillingCounter"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&KillingFrame::OnOpenCloseKillingShow));
	// register game event handle

	IFS* pFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");

	LoadPic(0, _T("data\\ui\\mark\\Battle\\ko-0.bmp"), pFS );
	LoadPic(1, _T("data\\ui\\mark\\Battle\\ko-1.bmp"), pFS );
	LoadPic(2, _T("data\\ui\\mark\\Battle\\ko-2.bmp"), pFS );
	LoadPic(3, _T("data\\ui\\mark\\Battle\\ko-3.bmp"), pFS );
	LoadPic(4, _T("data\\ui\\mark\\Battle\\ko-4.bmp"), pFS );
	LoadPic(5, _T("data\\ui\\mark\\Battle\\ko-5.bmp"), pFS );
	LoadPic(6, _T("data\\ui\\mark\\Battle\\ko-6.bmp"), pFS );
	LoadPic(7, _T("data\\ui\\mark\\Battle\\ko-7.bmp"), pFS );
	LoadPic(8, _T("data\\ui\\mark\\Battle\\ko-8.bmp"), pFS );
	LoadPic(9, _T("data\\ui\\mark\\Battle\\ko-9.bmp"), pFS );

	LoadPic(10, _T("data\\ui\\mark\\Battle\\kills-0.bmp"), pFS );
	LoadPic(11, _T("data\\ui\\mark\\Battle\\kills-1.bmp"), pFS );
	LoadPic(12, _T("data\\ui\\mark\\Battle\\kills-2.bmp"), pFS );
	LoadPic(13, _T("data\\ui\\mark\\Battle\\kills-3.bmp"), pFS );
	LoadPic(14, _T("data\\ui\\mark\\Battle\\kills-4.bmp"), pFS );
	LoadPic(15, _T("data\\ui\\mark\\Battle\\kills-5.bmp"), pFS );
	LoadPic(16, _T("data\\ui\\mark\\Battle\\kills-6.bmp"), pFS );
	LoadPic(17, _T("data\\ui\\mark\\Battle\\kills-7.bmp"), pFS );
	LoadPic(18, _T("data\\ui\\mark\\Battle\\kills-8.bmp"), pFS );
	LoadPic(19, _T("data\\ui\\mark\\Battle\\kills-9.bmp"), pFS );


	LoadPic(110, _T("data\\ui\\mark\\Battle\\kills.bmp"), pFS );
	LoadPic(200, _T("data\\ui\\mark\\Battle\\ko.bmp"), pFS );

	m_texProgress = IDraw2D::Inst()->NewTexture(_T("data\\ui\\mark\\Battle\\L_00.bmp"),pFS);

	// 1个数字出现的情况
	m_vecPosforOneNum.push_back(Vector3(0.0f, 0.0f, 120.0f));
	// 2个数字出现
	m_vecPosforTwoNum.push_back(Vector3(15.0f, 0.0f, 120.0f));
	m_vecPosforTwoNum.push_back(Vector3(-15.0f, 0.0f, 120.0f));
	// 3个数字出现
	m_vecPosforThreeNum.push_back(Vector3(30.0f, 0.0f, 120.0f));
	m_vecPosforThreeNum.push_back(Vector3(0.0f, 0.0f, 120.0f));
	m_vecPosforThreeNum.push_back(Vector3(-30.0f, 0.0f, 120.0f));
	// 4个数字出现
	m_vecPosforFourNum.push_back(Vector3(45.0f, 0.0f, 120.0f));
	m_vecPosforFourNum.push_back(Vector3(15.0f, 0.0f, 120.0f));
	m_vecPosforFourNum.push_back(Vector3(-15.0f, 0.0f, 120.0f));
	m_vecPosforFourNum.push_back(Vector3(-45.0f, 0.0f, 120.0f));
	// 5个数字出现
	m_vecPosforFiveNum.push_back(Vector3(60.0f, 0.0f, 120.0f));
	m_vecPosforFiveNum.push_back(Vector3(30.0f, 0.0f, 120.0f));
	m_vecPosforFiveNum.push_back(Vector3(0.0f, 0.0f, 120.0f));
	m_vecPosforFiveNum.push_back(Vector3(-30.0f, 0.0f, 120.0f));
	m_vecPosforFiveNum.push_back(Vector3(-60.0f, 0.0f, 120.0f));

	m_dwTime_HP = 0;
	m_dwTime_MP = 0;
	m_dwTime_Anger = 0;

	m_bMarkHP = true;
	m_bMarkMP = true;
	m_bMarkAnger = true;

	m_bMarkHPloop = true;
	m_bMarkMPloop = true;
	m_bMarkAngerloop = true;

	m_dwFadeTime = 0;

	m_dwMonsterConter = 0;
	m_dwPlayConter = 0;
	m_nAlpha = 0;

	LoadAllRes();

	return bRet;
}

BOOL KillingFrame::Destroy()
{

	BOOL bRet = GameFrame::Destroy();


	map<int, ResTexture*>::iterator it1 = m_mapResPic.begin();
	while(it1!=m_mapResPic.end())
	{
		IDraw2D::Inst()->ReleaseTexture(it1->second);
		it1++;
	}

	IDraw2D::Inst()->ReleaseTexture(m_texProgress);
	m_mapResPic.clear();

	// unregister game event handle
	m_pCmdMgr->UnRegister("NS_SendRoleKillCount", (NETMSGPROC)m_Trunk.sfp2(&KillingFrame::OnNS_SendRoleKillCount));

	m_pFrameMgr->UnRegisterEventHandler( _T("OpenCloseKillingCounter"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&KillingFrame::OnOpenCloseKillingShow));

	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = 0;
	}
	m_strKillingNum.clear();
	m_mapResPic.clear();
	m_mapNumInfo.clear();
	m_mapPosInfo.clear();

	if(P_VALID(m_pTex))
		IDraw2D::Inst()->ReleaseTexture(m_pTex);

	return bRet;
}

BOOL KillingFrame::ReloadUI()
{
	m_mapResPic.clear();
	m_mapNumInfo.clear();
	m_mapPosInfo.clear();

	// load ui resources
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\kills.xml");
	if(!m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str()))
		return FALSE;
	// create the window
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pProgressBar = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("kills_pic\\pic")));  
	m_ProgressPoints = m_pProgressBar->GetView();

	TCHAR tbuff[128];
	for ( int i =0;i<12; i++)
	{
		_sntprintf(tbuff, sizeof(tbuff) / sizeof(TCHAR), _T("kills_pic\\num%d"),i+1);
		if (i < 1)
		{
			m_pKills = static_cast<GUIStatic*>(m_pWnd->GetChild(tbuff)); 
			m_mapPosInfo.insert(make_pair(i,m_pKills->GetView()));
		}
		else if (i < 6)
		{
			m_pNumMonsters[i-1]  =   static_cast<GUIStatic*>(m_pWnd->GetChild(tbuff));  
			m_mapPosInfo.insert(make_pair(i,m_pNumMonsters[i-1]->GetView()));
		}
		else if (i < 7)
		{
			m_pKO = static_cast<GUIStatic*>(m_pWnd->GetChild(tbuff));  
			m_mapPosInfo.insert(make_pair(i,m_pKO->GetView()));
		}
		else if (i < 12)
		{	
			m_pNumPlayers[i-7] = static_cast<GUIStatic*>(m_pWnd->GetChild(tbuff));  
			m_mapPosInfo.insert(make_pair(i,m_pNumPlayers[i-7]->GetView()));
		}
	}

	m_pWnd->SetInvisible(FALSE);
	m_pWnd->SetUnClickable(TRUE);

	m_eState = KSST_Enter;
	m_nTexMark = 0;
	m_nProgressMark = 0;

	//tstring strFlag = _T("data\\ui\\mark\\Battle\\login_gray.dds");
	tstring strFlag = _T("data\\ui\\login\\login_black.dds");
	if(P_VALID(m_pTex))
		IDraw2D::Inst()->ReleaseTexture(m_pTex);
	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	m_pTex = IDraw2D::Inst()->NewTexture(strFlag.c_str(), pIFS);

	if (!P_VALID(m_pTex))
		return TRUE;

	IDeviceTex *pRealTex = m_pTex->GetDeviceTex();
	if (!P_VALID(pRealTex))
		return TRUE;

	GUIWnd* pDesktop = m_pGUI->GetDesktop();
	m_rectSrc.top = m_rectSrc.left = 0;
	m_rectSrc.bottom = m_rectSrc.top + pRealTex->Height();
	m_rectSrc.right = m_rectSrc.left + pRealTex->Width();
	m_rectDest.left = 0;
	m_rectDest.top = 0;
	m_rectDest.right = pDesktop->GetSize().x;
	m_rectDest.bottom = pDesktop->GetSize().y;

	return TRUE;
}

VOID KillingFrame::Update()
{
	UpdataLocalPlayerUIeffect();
	NumberGenerator();
}

BOOL KillingFrame::EscCancel()
{
	return FALSE;
}

DWORD KillingFrame::OnNS_SendRoleKillCount(tagNS_SendRoleKillCount* pNetCmd, DWORD dwParam)
{
	if (pNetCmd->eRoleKillType == ERKT_Monster)
	{
		m_dwMonsterConter = pNetCmd->dwKillCount;
		if (m_dwMonsterConter > 99999)
			m_dwMonsterConter = 99999;
		//CalculateNumPos(EKT_Monster);
	}
	else if (pNetCmd->eRoleKillType == ERKT_Role)
	{
		m_dwPlayConter = pNetCmd->dwKillCount;
		if (m_dwPlayConter > 99999)
			m_dwPlayConter = 99999;
		//CalculateNumPos(EKT_Player);
	}
	return 0;
}

VOID KillingFrame::Render3D()
{
	IDraw2D::Inst()->BeginDraw(IDraw2D::EBlend_Alpha, true);

	RECT destRect,srcRect;					//屏幕绘制区域和图片纹理区域
	int texW=0,texH=0;
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	DWORD dwColor = 0xFFFFFFFF;
	if (P_VALID(plp) && plp->IsDead())
	{
		INT nStep = 0;
		float fTime = Cool3D::Kernel::Inst()->GetDeltaTime();
		nStep= 3;

		if( m_nAlpha >= 0 && m_nAlpha <= 0x88 )
		{
			m_nAlpha += nStep;
			if( m_nAlpha > 0x88 )
			{
				m_nAlpha = 0x88;
			}
		}
		dwColor = ( m_nAlpha << 24 ) | 0x00FFFFFF;

		IDraw2D::Inst()->Draw( &m_rectDest, &m_rectSrc, m_pTex, dwColor, ETFilter_POINT, 0, IDraw2D::EAWT_Write, 0.1f );
	}
	else{
		m_nAlpha = 0;
	}

	if (m_pWnd->IsInvisible())
	{
		IDraw2D::Inst()->EndDraw();
		return;
	}
		
	for (KillNumMap::iterator it = m_mapNumInfo.begin();
		it != m_mapNumInfo.end(); ++it)
	{
		if (!it->second.bNeedRende)
			continue;

		// 获得对应的位置信息
		tagPoint Points;
		std::map<int,tagPoint>::iterator iterPos = m_mapPosInfo.find(it->second.nPos);
		if (iterPos == m_mapPosInfo.end())
			continue;

		// 获得对应纹理信息
		std::map<int,ResTexture*>::iterator iterTex;
		if (it->second.eType == EKT_Monster)
		{
			iterTex = m_mapResPic.find(it->second.nNum + 10);
		}
		else if(it->second.eType == EKT_Player)
		{
			iterTex = m_mapResPic.find(it->second.nNum);
		}
		 
		if (iterTex == m_mapResPic.end())
			continue;

		// 滚动条
		texW = m_texProgress->GetDeviceTex()->Width();
		texH = m_texProgress->GetDeviceTex()->Height();

		m_nProgressMark +=10;

		//屏幕上绘制的位置
		destRect.left	= m_ProgressPoints.x + (LONG)(texW)/2 - m_nProgressMark;
		destRect.top	= m_ProgressPoints.y;
		destRect.right	= m_ProgressPoints.x + (LONG)(texW)/2 + m_nProgressMark;
		destRect.bottom	= m_ProgressPoints.y + (LONG)(texH);

		//对应的纹理的区域，指定纹理的像素坐标
		srcRect.left	= (LONG)(texW)/2 - m_nProgressMark;
		srcRect.top		= 0;
		srcRect.right	= (LONG)(texW)/2 + m_nProgressMark;
		srcRect.bottom	= (LONG)(texH);

		if (m_nProgressMark >= texW/2)
		{
			m_nProgressMark = texW/2;
		}
		IDraw2D::Inst()->Draw(&destRect, &srcRect,m_texProgress, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write,0.0);

		if ( KSST_Enter == m_eState)
		{
			texW=((*iterTex).second)->GetDeviceTex()->Width();
			texH=((*iterTex).second)->GetDeviceTex()->Height();
			Points = (*iterPos).second;

			m_nTexMark+=3;
			
			//屏幕上绘制的位置
			destRect.left	= Points.x;
			destRect.top	= Points.y + (LONG)(texH) - m_nTexMark;
			destRect.right	= Points.x + (LONG)(texW);
			destRect.bottom	= Points.y + (LONG)(texH);

			//对应的纹理的区域，指定纹理的像素坐标
			srcRect.left	= 0;
			srcRect.top		= 0;
			srcRect.right	= (LONG)(texW);
			srcRect.bottom	= (LONG)(m_nTexMark);


			if (m_nTexMark >= texH)
			{
				m_eState = KSST_Explode;
				break;
			}
			
			IDraw2D::Inst()->Draw(&destRect, &srcRect,(*iterTex).second, dwColor, ETFilter_POINT, 0, IDraw2D::EAWT_Write,0.0);

		}
		else if (KSST_Explode == m_eState)
		{
			texW=((*iterTex).second)->GetDeviceTex()->Width();
			texH=((*iterTex).second)->GetDeviceTex()->Height();

			Points = (*iterPos).second;
			if (it->second.bNeedBomb)
			{
				it->second.nBombState = 10;
				//屏幕上绘制的位置
				destRect.left	= Points.x;
				destRect.top	= Points.y;
				destRect.right	= Points.x + (LONG)(texW);
				destRect.bottom	= Points.y + (LONG)(texH);

				// 只爆炸一次
				it->second.bNeedBomb = false;
					
				m_eState = KSST_Remain;
				m_dwFadeTime = 0;
			}
			else
			{
				int k = it->second.nBombState;
				if(k != 0)
					it->second.nBombState -= 1;
				else
					it->second.nBombState = 0;

				int marks = it->second.nPos;
				if (marks > 6)
					marks -= 6;
	
				marks -= 1;

				//屏幕上绘制的位置
				destRect.left	= Points.x - k - marks*10*k/10;
				destRect.top	= Points.y - k;
				destRect.right	= Points.x + (LONG)(texW) + k -marks*10*k/10;
				destRect.bottom	= Points.y + (LONG)(texH) + k;

			}
			//对应的纹理的区域，指定纹理的像素坐标
			srcRect.left	= 0;
			srcRect.top		= 0;
			srcRect.right	= (LONG)(texW);
			srcRect.bottom	= (LONG)(texH);
			IDraw2D::Inst()->Draw(&destRect, &srcRect,(*iterTex).second, dwColor, ETFilter_POINT, 0, IDraw2D::EAWT_Write,0.0);
		}	
		else if (KSST_Remain == m_eState)
		{
			m_dwFadeTime += Kernel::Inst()->GetDeltaTimeDW();
			if (m_dwFadeTime > EXPLODE_FADE_TIME)
			{
				m_eState = KSST_Explode;
			}

			texW=((*iterTex).second)->GetDeviceTex()->Width();
			texH=((*iterTex).second)->GetDeviceTex()->Height();

			Points = (*iterPos).second;
			if (it->second.bNeedBomb)
			{
				// 只爆炸一次
				it->second.nBombState = 10;
				it->second.bNeedBomb = false;
				m_dwFadeTime = 0;
			}
			//屏幕上绘制的位置
			if (it->second.nPos == 0 || it->second.nPos == 6)
			{
				destRect.left	= Points.x;
				destRect.top	= Points.y;
				destRect.right	= Points.x + (LONG)(texW);
				destRect.bottom	= Points.y + (LONG)(texH);
			}
			else if (it->second.nPos == 1 || it->second.nPos == 7 )
			{
				destRect.left	= Points.x - 5;
				destRect.top	= Points.y - 5;
				destRect.right	= Points.x + (LONG)(texW) + 10;
				destRect.bottom	= Points.y + (LONG)(texH) + 10;
			}
			else if (it->second.nPos == 2 || it->second.nPos == 8 )
			{
				destRect.left	= Points.x - 15;
				destRect.top	= Points.y - 5;
				destRect.right	= Points.x + (LONG)(texW) ;
				destRect.bottom	= Points.y + (LONG)(texH) + 10;
			}
			else if (it->second.nPos == 3 || it->second.nPos == 9 )
			{
				destRect.left	= Points.x - 25;
				destRect.top	= Points.y - 5;
				destRect.right	= Points.x + (LONG)(texW) - 10;
				destRect.bottom	= Points.y + (LONG)(texH) + 10;
			}
			else if (it->second.nPos == 4 || it->second.nPos == 10 )
			{
				destRect.left	= Points.x - 35;
				destRect.top	= Points.y - 5;
				destRect.right	= Points.x + (LONG)(texW) - 20;
				destRect.bottom	= Points.y + (LONG)(texH) + 10;
			}
			else if (it->second.nPos == 5 || it->second.nPos == 11)
			{
				destRect.left	= Points.x - 45;
				destRect.top	= Points.y - 5;
				destRect.right	= Points.x + (LONG)(texW) - 30;
				destRect.bottom	= Points.y + (LONG)(texH) + 10;
			}
			
			

			//对应的纹理的区域，指定纹理的像素坐标
			srcRect.left	= 0;
			srcRect.top		= 0;
			srcRect.right	= (LONG)(texW);
			srcRect.bottom	= (LONG)(texH);
			IDraw2D::Inst()->Draw(&destRect, &srcRect,(*iterTex).second, dwColor, ETFilter_POINT, 0, IDraw2D::EAWT_Write,0.0);
		}
	}

	IDraw2D::Inst()->EndDraw();
}

VOID KillingFrame::DrawNumPic(RECT rc, ResTexture* pRes)
{
 	if( P_VALID(pRes) )
 	{
 		RECT srcRC;
 
 		srcRC.top=srcRC.left=0;
 		srcRC.bottom=srcRC.top+14;
 		srcRC.right=srcRC.left+16;
 
 		IDraw2D::Inst()->Draw(&rc, &srcRC, pRes, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write);
 	}
}

VOID KillingFrame::CalculateNumPos(EKillingTpye eType)
{
 	TCHAR szBuffer[X_SHORT_NAME] = {0};
// 	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nGold);

	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(plp))
		return;

	if ( EKT_Monster == eType)
	{
		bool NeedShowNumber = false;
		int LocalPlayerLevel = plp->GetRoleLevel();
		if ( LocalPlayerLevel <= 10)
		{
			if (   m_strKillingNum.nMonster == 5 
				|| m_strKillingNum.nMonster == 20 
				|| m_strKillingNum.nMonster == 40
				|| m_strKillingNum.nMonster == 60
				|| m_strKillingNum.nMonster == 80
				|| m_strKillingNum.nMonster%100 == 0
				)
			{
				PlayScreenEffect();
				NeedShowNumber=true;
			}
		}
		else if (LocalPlayerLevel <= 20)
		{
			if (m_strKillingNum.nMonster == 20 || m_strKillingNum.nMonster == 50 || m_strKillingNum.nMonster%100 == 0)
			{
				PlayScreenEffect();
				NeedShowNumber=true;
			}
		}
		else
		{
			if (m_strKillingNum.nMonster%100 == 0)
			{
				PlayScreenEffect();
				NeedShowNumber=true;
			}
		}
		if (m_strKillingNum.nMonster)
		{
			InsertToNumMap(EKT_Monster,100,0,true,false);

			int nCounter = m_strKillingNum.nMonster;

			// 万位
			int nFive = nCounter/10000%10;
			if (nFive == 0)
			{
				EraseFromNumMap(5);
			}else{
				InsertToNumMap(EKT_Monster,nFive,5);
			}
			// 千位
			int nFour = nCounter/1000%10;
			if (nFour == 0 && nFive == 0)
			{
				EraseFromNumMap(4);
			}else{
				InsertToNumMap(EKT_Monster,nFour,4);
			}

			// 百位
			int nThree = nCounter/100%10;
			if (nThree == 0 && nFour == 0 && nFive == 0)
			{
				EraseFromNumMap(3);
			}else{
				InsertToNumMap(EKT_Monster,nThree,3);
			}

			// 十位
			int nTwo = nCounter/10%10;
			if (nThree == 0 && nFour == 0 && nTwo == 0 && nFive == 0)
			{
				EraseFromNumMap(2);
			}else{
				InsertToNumMap(EKT_Monster,nTwo,2);
			}

			// 个位
			int nOne = nCounter%10;
			InsertToNumMap(EKT_Monster,nOne,1);

			GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
			if (NeedShowNumber)
			{
				if( 0 != nFive)
				{
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nOne);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforFiveNum[0],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nTwo);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforFiveNum[1],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nThree);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforFiveNum[2],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nFour);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforFiveNum[3],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nFive);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforFiveNum[4],Vector3(0.6,0.6,0.6));
				}
				else if( 0 != nFour)
				{
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nOne);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforFourNum[0],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nTwo);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforFourNum[1],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nThree);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforFourNum[2],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nFour);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforFourNum[3],Vector3(0.6,0.6,0.6));
				}
				else if (0 != nThree)
				{
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nOne);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforThreeNum[0],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nTwo);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforThreeNum[1],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nThree);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforThreeNum[2],Vector3(0.6,0.6,0.6));
				}
				else if (0 != nTwo)
				{
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nOne);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforTwoNum[0],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nTwo);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforTwoNum[1],Vector3(0.6,0.6,0.6));
				}
				else if (0 != nOne)
				{
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nOne);
					GUIEffectMgr::Inst()->PlayScreenEffect(szBuffer,pDesktop->GetView(),pDesktop->GetSize(),m_vecPosforOneNum[0],Vector3(0.6,0.6,0.6));
				}
			}
		}
	}
	else if (EKT_Player == eType)
	{
		/*
		bool NeedShowNumber = false;
		int LocalPlayerLevel = plp->GetRoleLevel();
		if ( LocalPlayerLevel < 20)
		{
			if (m_strKillingNum.nPlayer == 20 || m_strKillingNum.nPlayer == 50 || m_strKillingNum.nPlayer%100 == 0)
			{
				PlayScreenEffect();
				NeedShowNumber=true;
			}
		}
		else
		{
			if (m_strKillingNum.nPlayer%100 == 0)
			{
				PlayScreenEffect();
				NeedShowNumber=true;
			}
		}
		*/
		if (m_strKillingNum.nPlayer)
		{
			InsertToNumMap(EKT_Player,200,6,true,false);

			int nCounter = m_strKillingNum.nPlayer;

			// 万位
			int nFive = nCounter/10000%10;
			if (nFive == 0)
			{
				EraseFromNumMap(11);
			}else{
				InsertToNumMap(EKT_Player,nFive,11);
			}
			// 千位
			int nFour = nCounter/1000%10;
			if (nFour == 0 && nFive == 0)
			{
				EraseFromNumMap(10);
			}else{
				InsertToNumMap(EKT_Player,nFour,10);
			}

			// 百位
			int nThree = nCounter/100%10;
			if (nThree == 0 && nFour == 0 && nFive == 0)
			{
				EraseFromNumMap(9);
			}else{
				InsertToNumMap(EKT_Player,nThree,9);
			}

			// 十位
			int nTwo = nCounter/10%10;
			if (nThree == 0 && nFour == 0 && nTwo == 0 && nFive == 0)
			{
				EraseFromNumMap(8);
			}else{
				InsertToNumMap(EKT_Player,nTwo,8);
			}

			// 个位
			int nOne = nCounter%10;
			InsertToNumMap(EKT_Player,nOne,7);

			GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
			GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui002"),pDesktop->GetView(),pDesktop->GetSize());

			/*
			GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
			if (NeedShowNumber)
			{
				if( 0 != nFour)
				{
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nOne);
					GUIEffectMgr::Inst()->PlayGuiEffect(szBuffer,pDesktop,m_vecPosforFourNum[0],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nTwo);
					GUIEffectMgr::Inst()->PlayGuiEffect(szBuffer,pDesktop,m_vecPosforFourNum[1],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nThree);
					GUIEffectMgr::Inst()->PlayGuiEffect(szBuffer,pDesktop,m_vecPosforFourNum[2],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nFour);
					GUIEffectMgr::Inst()->PlayGuiEffect(szBuffer,pDesktop,m_vecPosforFourNum[3],Vector3(0.6,0.6,0.6));
				}
				else if (0 != nThree)
				{
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nOne);
					GUIEffectMgr::Inst()->PlayGuiEffect(szBuffer,pDesktop,m_vecPosforThreeNum[0],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nTwo);
					GUIEffectMgr::Inst()->PlayGuiEffect(szBuffer,pDesktop,m_vecPosforThreeNum[1],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nThree);
					GUIEffectMgr::Inst()->PlayGuiEffect(szBuffer,pDesktop,m_vecPosforThreeNum[2],Vector3(0.6,0.6,0.6));
				}
				else if (0 != nTwo)
				{
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nOne);
					GUIEffectMgr::Inst()->PlayGuiEffect(szBuffer,pDesktop,m_vecPosforTwoNum[0],Vector3(0.6,0.6,0.6));
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nTwo);
					GUIEffectMgr::Inst()->PlayGuiEffect(szBuffer,pDesktop,m_vecPosforTwoNum[1],Vector3(0.6,0.6,0.6));
				}
				else if (0 != nOne)
				{
					_sntprintf(szBuffer, X_SHORT_NAME, _T("txui%d"), nOne);
					GUIEffectMgr::Inst()->PlayGuiEffect(szBuffer,pDesktop,m_vecPosforOneNum[0],Vector3(0.6,0.6,0.6));
				}
			}
			*/
		}
	}
}

VOID KillingFrame::InsertToNumMap(tagRenderPosFlag flag)
{
	if (m_mapNumInfo.empty())
	{
		m_mapNumInfo.insert(make_pair(flag.nPos,flag));
	}
	else
	{
		KillNumMap::iterator it = m_mapNumInfo.find(flag.nPos);
		if (it != m_mapNumInfo.end())
		{
			it->second = flag;
		}
		else
		{
			m_mapNumInfo.insert(make_pair(flag.nPos,flag));
		}
	}
	
}

VOID KillingFrame::InsertToNumMap(EKillingTpye eType,int num,int pos,bool needRender /*= true*/,bool Bomb /*= true*/)
{
	tagRenderPosFlag KillingFlag;
	KillingFlag.bNeedRende = needRender;
	KillingFlag.eType = eType;
	KillingFlag.nNum = num;
	KillingFlag.nPos = pos;
	KillingFlag.bNeedBomb = Bomb;
	InsertToNumMap(KillingFlag);
}

VOID KillingFrame::EraseFromNumMap(int pos)
{
	if (m_mapNumInfo.empty())
		return;

	KillNumMap::iterator it = m_mapNumInfo.find(pos);
	if (it != m_mapNumInfo.end())
	{
		it->second.bNeedRende = false;
	}
}

bool KillingFrame::LoadPic(int ch,const TCHAR* szPicFileName,IFS *pFS)
{
	if (!m_mapResPic.empty()&& m_mapResPic.find(ch)!=m_mapResPic.end())
	{
		//已经使用过的字符不加载图片
		return false;
	}

	//加载对应的图片,并保存到map中
	m_mapResPic[ch]=IDraw2D::Inst()->NewTexture(szPicFileName,pFS);

	return true;
}

VOID KillingFrame::LoadAllRes()
{
	LoadRes(_T("txui001"));
	/*
	TCHAR zsBuff[X_SHORT_NAME];
	for (int i=0;i <10;i++)
	{
		_sntprintf(zsBuff,X_SHORT_NAME,_T("%d"),i);
		LoadRes(zsBuff);
	}
	*/
	
}

VOID KillingFrame::LoadRes(const TCHAR* szPicFileName)
{
	tstring strEffectPath;
	strEffectPath = _T("data\\system\\effect\\");
	strEffectPath += _T("ui");
	strEffectPath += _T("\\");
	strEffectPath += szPicFileName;
	strEffectPath += _T(".sfx");

	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	DWORD dwFileSize = pIFS->GetSize( strEffectPath.c_str() );
	if( 0 == dwFileSize || GT_INVALID == dwFileSize )
	{
		TRACE( _T("SFX file have not found.file=%s\r\n"), strEffectPath.c_str() );
		return;
	}

	ResMgr::Inst()->NewSpecialEffect(strEffectPath.c_str());
}

VOID KillingFrame::PlayScreenEffect()
{
	
	BeAttackFrame *pFrame =(BeAttackFrame*)m_pMgr->GetFrame(_T("BeAttack"));
	if( !P_VALID(pFrame) )
	{
		pFrame = (BeAttackFrame*)m_pMgr->CreateFrame( _T("World"), _T("BeAttack"), _T("BeAttackFrame"), 0 );
		if(P_VALID(pFrame))
			pFrame->SetRenderFlag(TRUE,true);
	}
	else
		pFrame->SetRenderFlag(TRUE,true);

	GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
	if(P_VALID(pDesktop))
		GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui001"),pDesktop->GetView(),pDesktop->GetSize(),Vector3(0.0f, 0.0f, 120.0f),Vector3(1.0,1.0,1.0),_T("ui"),true);
	
}

DWORD KillingFrame::OnOpenCloseKillingShow(tagGameEvent *pEvent)
{
	if ( m_pWnd->IsInvisible())
		m_pWnd->SetInvisible(FALSE);
	else
		m_pWnd->SetInvisible(TRUE);

	return 0;
}

VOID KillingFrame::UpdataLocalPlayerUIeffect()
{
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(plp))
		return;

	if (!plp->IsAttributeInited())
		return;

	float CurHP = (float)plp->GetAttribute(ERA_HP)/(float)plp->GetAttribute(ERA_MaxHP);
	float CurMP = (float)plp->GetAttribute(ERA_MP)/(float)plp->GetAttribute(ERA_MaxMP);
	INT CurAnger = plp->GetAttribute(ERA_Rage);

	if (CurHP <= 0.2 && !plp->IsDead())
	{
		m_dwTime_HP += Kernel::Inst()->GetDeltaTimeDW();
		if (m_bMarkHP)
		{
			GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
			GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui005"),pDesktop->GetView(),pDesktop->GetSize());
			m_bMarkHP = false;
		}

		if (m_dwTime_HP > SHOW_TIME && m_bMarkHPloop && !plp->IsDead())
		{
			GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
			GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui006"),pDesktop->GetView(),pDesktop->GetSize(),GUIEffectMgr::EET_HP);
			m_bMarkHPloop = false;
		}
	}
	else{
		if (!m_bMarkHPloop)
			GUIEffectMgr::Inst()->StopEffectByType(GUIEffectMgr::EET_HP);

		m_bMarkHP = true;
		m_dwTime_HP = 0;
		m_bMarkHPloop = true;
	}

	if (CurMP <= 0.2 )
	{
		m_dwTime_MP += Kernel::Inst()->GetDeltaTimeDW();
		if (m_bMarkMP)
		{
			GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
			GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui007"),pDesktop->GetView(),pDesktop->GetSize());
			m_bMarkMP = false;
		}

		if (m_dwTime_MP > SHOW_TIME && m_bMarkMPloop)
		{
			GUIWnd* pTestWnd = m_pGUI->GetWnd(_T("\\desktop\\characteratt\\att"));
			if (P_VALID(pTestWnd))
			{
				GUIEffectMgr::Inst()->PlayGuiEffect(_T("txui008"),pTestWnd,GUIEffectMgr::EET_MP);
			}
			m_bMarkMPloop = false;
		}
	}
	else{
		if (!m_bMarkMPloop)
			GUIEffectMgr::Inst()->StopEffectByType(GUIEffectMgr::EET_MP);

		m_bMarkMP = true;
		m_dwTime_MP = 0;
		m_bMarkMPloop = true;
	}

	if (CurAnger == 100)
	{
		m_dwTime_Anger += Kernel::Inst()->GetDeltaTimeDW();
		if (m_bMarkAnger)
		{
			GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
			GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui009"),pDesktop->GetView(),pDesktop->GetSize());
			m_bMarkAnger = false;
		}

		if (m_dwTime_Anger > SHOW_TIME && m_bMarkAngerloop)
		{
			GUIWnd* pTestWnd = m_pGUI->GetWnd(_T("\\desktop\\characteratt\\att"));
			if (P_VALID(pTestWnd))
			{
				GUIEffectMgr::Inst()->PlayGuiEffect(_T("txui010"),pTestWnd,GUIEffectMgr::EET_Anger);
			}
			m_bMarkAngerloop = false;
		}
	}
	else{
		if (!m_bMarkAngerloop)
			GUIEffectMgr::Inst()->StopEffectByType(GUIEffectMgr::EET_Anger);

		m_bMarkAnger = true;
		m_dwTime_Anger = 0;
		m_bMarkAngerloop = true;
	}
}

VOID KillingFrame::NumberGenerator()
{
	if (m_strKillingNum.nMonster < m_dwMonsterConter)
	{
		m_strKillingNum.nMonster++;
		CalculateNumPos(EKT_Monster);
	}

	if (m_strKillingNum.nPlayer < m_dwPlayConter)
	{
		m_strKillingNum.nPlayer++;
		CalculateNumPos(EKT_Player);
	}
}
