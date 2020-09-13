#include "StdAfx.h"
#include "ZoneMapStatic.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "MapMgr.h"
#include "Role.h"
#include "NPC.h"
#include "Player.h"
#include "QuestMgr.h"
#include "CombatEvent.h"
#include "..\WorldDefine\creature_define.h"
#include "TeamSys.h"
#include "MapLogicData.h"
#include "CreatureData.h"
#include "ToolTipFrame.h"
#include "CityMgr.h"
#include "GuildMgr.h"
#include "OverSeaData.h"

ZoneMapStatic::ZoneMapStatic() : 
m_dwCurShowMapID( GT_INVALID )
{
	for( int i = 0; i < EMME_Num; i++ )
	{
		m_pElementImg[i] = NULL;
		m_bElementShow[i] = FALSE;
	}
	m_bElementShow[EMME_LocalPlayer] = TRUE;
	m_bElementShow[EMME_Teammate]   = TRUE;
	m_bElementShow[EMME_AcceptQuestNPC] = TRUE;
	m_bElementShow[EMME_CompleteQuestNPC] = TRUE;
    m_bElementShow[EMME_RepeatAcceptQuestNPC] = TRUE;
    m_bElementShow[EMME_RepeatCompleteQuestNPC] = TRUE;
	m_bElementShow[EMME_ActiveMonster] = TRUE;
}

ZoneMapStatic::~ZoneMapStatic()
{

}

BOOL ZoneMapStatic::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	if( !MiniMapStatic::Init( pSystem, pFather, pXmlElement ) )
		return FALSE;

	tstring strFilename = _T("data\\ui\\god\\zhu_white.bmp");
	m_pColumns[0] = m_pRender->CreateImage( strFilename, tagRect() );
	strFilename = _T("data\\ui\\god\\zhu_blue.bmp");
	m_pColumns[1] = m_pRender->CreateImage( strFilename, tagRect() );
	strFilename = _T("data\\ui\\god\\zhu_red.bmp");
	m_pColumns[2] = m_pRender->CreateImage( strFilename, tagRect() );
	strFilename = _T("data\\ui\\god\\nowar_open.bmp");
	m_pDoor[0] = m_pRender->CreateImage( strFilename, tagRect() );
	strFilename = _T("data\\ui\\god\\war_close.bmp");
	m_pDoor[1] = m_pRender->CreateImage( strFilename, tagRect() );
	strFilename = _T("data\\ui\\god\\war_open.bmp");
	m_pDoor[2] = m_pRender->CreateImage( strFilename, tagRect() );
	strFilename = _T("data\\ui\\god\\ball_yellow.bmp");
	m_pDragon[0] = m_pRender->CreateImage( strFilename, tagRect() );
	strFilename = _T("data\\ui\\god\\ball_green.bmp");
	m_pDragon[1] = m_pRender->CreateImage( strFilename, tagRect() );
	strFilename = _T("data\\ui\\god\\ball_red.bmp");
	m_pDragon[2] = m_pRender->CreateImage( strFilename, tagRect() );
	return TRUE;
}

VOID ZoneMapStatic::Destroy()
{
	for(int i=0; i<3; i++)
	{
		if(P_VALID(m_pColumns[i]))
		{
			m_pRender->DestroyImage(m_pColumns[i]);
			m_pColumns[i] = NULL;
		}

		if(P_VALID(m_pDoor[i]))
		{
			m_pRender->DestroyImage(m_pDoor[i]);
			m_pDoor[i] = NULL;
		}

		if(P_VALID(m_pDragon[i]))
		{
			m_pRender->DestroyImage(m_pDragon[i]);
			m_pDragon[i] = NULL;
		}
	}
	MiniMapStatic::Destroy();
}

VOID ZoneMapStatic::Update()
{
	if( !P_VALID( m_pColorTex ) )
		return;
	if( m_bInvisible )
		return;

	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();

	if( NULL == pLocalPlayer )
		return;

	const Vector3& vLPPos = pLocalPlayer->GetPos();
	m_ptLP = WorldToMap( Vector2( vLPPos.x, vLPPos.z ) );

	// 子窗口也需要实时渲染
	std::list<GUIWnd*>::reverse_iterator it;
	for(it=m_listChild.rbegin(); it!=m_listChild.rend(); ++it)
		(*it)->SetRefresh();

	GUIStatic::Update();
}

VOID ZoneMapStatic::Render()
{
	if( m_bInvisible )
		return GUIWnd::Render();

	if( !P_VALID( m_pColorTex ) || !P_VALID( m_pCanvas ) )
		return;

	if( !P_VALID( m_pColorTex->dwHandle ) )
		return;

	// 地图
	m_pCanvas->rc.SetLTWH( m_ptView, m_ptSize );
	m_pRender->Draw( m_pCanvas, m_pColorTex, m_dwPicColor, m_eAlphaWrite );

	tstring strMapName = MapMgr::Inst()->GetCurMapName();
	if( strMapName == _T("f01") || strMapName == _T("f02") ||
		strMapName == _T("f03") || strMapName == _T("f04") ||
		strMapName == _T("f05") || strMapName == _T("f06") ||
		strMapName == _T("f07") )
		return;
    // 本地玩家
    if( m_dwCurShowMapID == MapMgr::Inst()->GetCurMapID() )
    {
        DrawElement( EMME_LocalPlayer, m_ptLP, RoleMgr::Inst()->GetLocalPlayer()->GetYaw() + D3DX_PI );
    }

	if( m_dwCurShowMapID == MapMgr::Inst()->GetCurMapID() )
	{
		// 可接任务NPC
		const QuestQuery::NPCSet& setAcceptNPCs = QuestMgr::Inst()->GetQuery()->GetAcceptQuestNPCs();
		for( QuestQuery::NPCSet::const_iterator cItr = setAcceptNPCs.begin(); cItr != setAcceptNPCs.end(); cItr++ )
		{
			bool bContinue = false;

			if (OverSeaData::Inst()->IsNPCHide((*cItr)))
				bContinue = true;

			if (QuestMgr::Inst()->GetData()->IsNPCHasQuestByCurrentFilter((*cItr)) == FALSE)
				bContinue = true;
			if (QuestMgr::Inst()->GetData()->IsFilterByPlayerLevel() == TRUE)
			{
				if (QuestMgr::Inst()->GetData()->IsNPCHasQuestByCurrentFilterByPlayerLevel((*cItr)) == FALSE)
					bContinue = true;
			}
			if (bContinue)
				continue;

            // 是否可重复任务

            EMiniMapElement renderType = EMME_AcceptQuestNPC;

            vector<UINT16> quests;
            QuestMgr::Inst()->GetData()->GetNPCAcceptQuests((*cItr), quests);

            const tagQuestProto* pQProto = NULL;

            if (quests.size() > 0)
                pQProto = QuestMgr::Inst()->GetData()->GetQuestProto(quests[0]);

            if (P_VALID(pQProto) && pQProto->repeatable)
                renderType = EMME_RepeatAcceptQuestNPC;


			vector<Vector3> vecPos;
			QuestMgr::Inst()->GetQuery()->GetNPCPosition( (*cItr), vecPos );
			for( size_t n = 0; n < vecPos.size(); n++ )
			{
				const tagPoint ptPos = WorldToMap( Vector2( vecPos[n].x, vecPos[n].z ) );
				DrawElement( renderType, ptPos );
			}
		}

		// 可交任务NPC
		const QuestQuery::NPCSet& setCompleteNPCs = QuestMgr::Inst()->GetQuery()->GetCompleteQuestNPCs();
		for( QuestQuery::NPCSet::const_iterator cItr = setCompleteNPCs.begin(); cItr != setCompleteNPCs.end(); cItr++ )
		{
            // 是否可重复任务
			if (OverSeaData::Inst()->IsNPCHide((*cItr)))
				continue;

            EMiniMapElement renderType = EMME_CompleteQuestNPC;

            vector<UINT16> quests;
            QuestMgr::Inst()->GetData()->GetNPCAcceptQuests((*cItr), quests);

            const tagQuestProto* pQProto = NULL;

            if (quests.size() > 0)
                pQProto = QuestMgr::Inst()->GetData()->GetQuestProto(quests[0]);

            if (P_VALID(pQProto) && pQProto->repeatable)
                renderType = EMME_RepeatCompleteQuestNPC;

			vector<Vector3> vecPos;
			QuestMgr::Inst()->GetQuery()->GetNPCPosition( (*cItr), vecPos );
			for( size_t n = 0; n < vecPos.size(); n++ )
			{
				const tagPoint ptPos = WorldToMap( Vector2( vecPos[n].x, vecPos[n].z ) );
				DrawElement( renderType, ptPos );
			}
		}
	}


	// 队友
	// 城战的时候不画
	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLocalPlayer) && !pLocalPlayer->IsInState(ES_CSAttack) && !pLocalPlayer->IsInState(ES_CSDefence))
	{
		const vector<tagTeammateData>& vecTeammates = TeamSys::Inst()->GetTeammateList();
		for( size_t i = 0; i < vecTeammates.size(); i++ )
		{
			const tagTeammateData& data = vecTeammates[i];
			if( data.dwMapID == m_dwCurShowMapID )
			{
				const tagPoint ptPos = WorldToMap( Vector2( data.fX, data.fZ ) );
				DrawElement( EMME_Teammate, ptPos );
			}
		}
	}

	// 寻路目的点标记
	if( m_bShowDestMark && m_dwDestMarkMapID == m_dwCurShowMapID && P_VALID(m_pImgDestMark) && P_VALID( m_pImgDestMark->dwHandle ) )
	{
		BYTE byAlpha = BYTE( m_dwDestMarkColor>>24 );
		byAlpha += (BYTE)( Kernel::Inst()->GetDeltaTime() * 0xFF );
		m_dwDestMarkColor = ( byAlpha << 24 ) | ( m_dwDestMarkColor & 0x00ffffff );
		tagPoint ptDestMark = WorldToMap( m_vDestMark );
		DrawImg( m_pImgDestMark, ptDestMark, m_dwDestMarkColor );
	}

	// 活动怪物
	if( GetCurShowMapID() == MapMgr::Inst()->GetCurMapID() )
	{
		const vector<tagACreaturePosInfo> vecMonster = RoleMgr::Inst()->GetMonsterInfo();
		for( size_t i = 0; i < vecMonster.size(); i++ )
		{
			if (OverSeaData::Inst()->IsNPCHide(vecMonster[i].dwTypeID))
				continue;

			const tagPoint ptPos = WorldToMap( Vector2( vecMonster[i].fX, vecMonster[i].fZ ) );
			DrawElement( EMME_ActiveMonster, ptPos );
		}
	}

	// 城战的时候画进攻方和防守方
	if(P_VALID(pLocalPlayer) && (pLocalPlayer->IsInState(ES_CSAttack) || pLocalPlayer->IsInState(ES_CSDefence)))
	{
		const map<DWORD,Role*>& roleMap = RoleMgr::Inst()->GetRoleMap();
		for(map<DWORD,Role*>::const_iterator itor = roleMap.begin(); itor!=roleMap.end(); itor++)
		{
			if(itor->second->IS_STATIC_CLASS(Player)) // local player is not drawed
			{
				Player* pPlayer = (Player*)itor->second;
				if(pPlayer->IsAttributeInited())
				{
					const tagPoint ptPos = WorldToMap(Vector2(pPlayer->GetPos().x, pPlayer->GetPos().z));
					if(pPlayer->IsInState(ES_CSAttack))
						DrawElement(EMME_AttackSide, ptPos);
					else if(pPlayer->IsInState(ES_CSDefence))
						DrawElement(EMME_DefenceSide, ptPos);
				}
			}
		}
	}
	DrawCityWarInfo(MapMgr::Inst()->GetCurMapID());
	DrawBattleWarInfo(MapMgr::Inst()->GetCurMapID());

	return GUIWnd::Render();
}

BOOL ZoneMapStatic::OnInputMessage(tagGUIInputMsg* pMsg)
{
	if( !P_VALID( m_pColorTex ) )
		return GUIStatic::OnInputMessage( pMsg );

	switch( pMsg->dwType )
	{
		case WM_MOUSEMOVE:
			// 显示TIPS
			if( PointInWnd( pMsg->pt ) )
			{
				tstring strTip = g_StrTable[_T("TipsDefaultFont")];
				Vector2 vPointPos = MapToWorld( PointToMap( pMsg->pt ) );
				tstringstream stream;
				stream<<(LONG)( vPointPos.x / 50.0f )<<_T(" ")<<(LONG)( vPointPos.y / 50.0f )<<_T("\\n");
				strTip += stream.str().c_str();

				if( m_dwCurShowMapID == MapMgr::Inst()->GetCurMapID() )
				{
					// 可接任务NPC
					const QuestQuery::NPCSet& setAcceptNPCs = QuestMgr::Inst()->GetQuery()->GetAcceptQuestNPCs();
					for( QuestQuery::NPCSet::const_iterator cItr = setAcceptNPCs.begin(); cItr != setAcceptNPCs.end(); cItr++ )
					{
						if (OverSeaData::Inst()->IsNPCHide((*cItr)))
							continue;

						vector<Vector3> vecPos;
						QuestMgr::Inst()->GetQuery()->GetNPCPosition( (*cItr), vecPos );
						for( size_t n = 0; n < vecPos.size(); n++ )
						{
							if( IsMousePoint( EMME_AcceptQuestNPC, Vector2( vecPos[n].x, vecPos[n].z ), pMsg->pt ) )
							{
								tstringstream strInfo;
								const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt( (*cItr) );
								ASSERT( P_VALID( pProto ) );
								strTip += pProto->szName;
								strTip += _T("\\n");

								// 取得当前NPC的任务列表
								QuestQuery *pQQ = QuestMgr::Inst()->GetQuery();
								QuestData *pQD = QuestMgr::Inst()->GetData();
								vector<UINT16> quests;
								pQQ->GetNPCAcceptQuests(*cItr, quests);
								vector<UINT16>::iterator iend = quests.end();
								for (vector<UINT16>::iterator iIter = quests.begin(); iIter != iend; ++iIter)
								{
									INT16 n16Category = pQD->GetQuestCategoryByQuestID(*iIter);
									if( n16Category > 0 && n16Category < 11 )
									{
										tstring questTypeName(_T(""));
										questTypeName = pQD->GetTaskTypeName( n16Category );
										if( !questTypeName.empty() )
										{
											strTip += _T("[");
											strTip += questTypeName;
											strTip += _T("]");
										}
									}
								}
								strTip += _T("\\n");
							}
						}
					}

					// 可交任务NPC
					const QuestQuery::NPCSet& setCompleteNPCs = QuestMgr::Inst()->GetQuery()->GetCompleteQuestNPCs();
					for( QuestQuery::NPCSet::const_iterator cItr = setCompleteNPCs.begin(); cItr != setCompleteNPCs.end(); cItr++ )
					{
						vector<Vector3> vecPos;
						QuestMgr::Inst()->GetQuery()->GetNPCPosition( (*cItr), vecPos );
						for( size_t n = 0; n < vecPos.size(); n++ )
						{
							if (OverSeaData::Inst()->IsNPCHide((*cItr)))
								continue;

							if( IsMousePoint( EMME_CompleteQuestNPC, Vector2( vecPos[n].x, vecPos[n].z ), pMsg->pt ) )
							{
								const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt( (*cItr) );
								ASSERT( P_VALID( pProto ) );
								strTip += pProto->szName;
								strTip += _T("\\n");
							}
						}
					}
				}

				// 队友
				const vector<tagTeammateData>& vecTeammates = TeamSys::Inst()->GetTeammateList();
				for( size_t i = 0; i < vecTeammates.size(); i++ )
				{
					const tagTeammateData& data = vecTeammates[i];
					if( data.dwMapID == m_dwCurShowMapID )
					{
						Vector2 vPos = Vector2( data.fX, data.fZ );
						if( IsMousePoint( EMME_Teammate, vPos, pMsg->pt ) )
						{
							strTip += data.strName;
							strTip += _T("\\n");
						}
					}
				}

				// 活动怪物
				const vector<tagACreaturePosInfo> vecMonster = RoleMgr::Inst()->GetMonsterInfo();
				for( size_t i = 0; i < vecMonster.size(); i++ )
				{
					const tagCreatureProto *pProto = CreatureData::Inst()->FindNpcAtt( vecMonster[i].dwTypeID );
					if( IsMousePoint( EMME_ActiveMonster, Vector2( vecMonster[i].fX, vecMonster[i].fZ ), pMsg->pt ) && P_VALID(pProto) && pProto->bActive )
					{
						if (OverSeaData::Inst()->IsNPCHide(pProto->dwTypeID))
							continue;

						strTip += g_StrTable[_T("Active_Monster")];
						strTip += pProto->szName;
						strTip += _T("\\n");
					}
				}

				ShowTip( pMsg->pt + tagPoint( 16, 16 ), strTip.c_str() );
			}
			else
			{
				ShowTip( NULL, _T("") );
			}
			break;
		case WM_LBUTTONDBLCLK:
			if( PointInWnd( pMsg->pt ) )
			{
				// 当前地图
				if( MapMgr::Inst()->GetCurMapID() == m_dwCurShowMapID )
				{
					tagPoint ptDestMark = PointToMap( pMsg->pt );
					Vector2 vDestMark = MapToWorld( ptDestMark );
					LPAutoMoveTo( vDestMark, 0.0f );
				}
				// 跨地图寻路
				else
				{
					const tstring& strCurMapName = MapMgr::Inst()->GetCurMapName();
					const tagMapLogicData* pDestMapLogicData = MapLogicData::Inst()->FindMapLogicData( m_dwCurShowMapID );
					ASSERT( P_VALID( pDestMapLogicData ) );
					NavMap* pNav = MapMgr::Inst()->GetNavMap();
					PlayerNavMap* pPlayerNav = pNav->GetPlayerNavMap();
					Vector3 vSwitchPos( 0.0f, 0.0f, 0.0f );
					const bool bFind = pPlayerNav->FindPathByMap( strCurMapName.c_str(), pDestMapLogicData->szName.c_str(), vSwitchPos );
					if( bFind )
						LPAutoMoveTo( vSwitchPos, 0.0f );
				}
			}
			break;
        case WM_RBUTTONUP:
            if( PointInWnd(pMsg->pt) )
            {
                tagGUIEvent event(this, EGUIE_Click, pMsg);
                SendEvent(&event);
            }
            break;
		default:
			break;
	}
	return GUIStatic::OnInputMessage( pMsg );
}

tagPoint ZoneMapStatic::PointToMap( tagPoint ptPoint )
{
	ASSERT( P_VALID( m_pColorTex ) );
	tagPoint ptControl = ptPoint - m_ptView;
	ptControl.x = LONG( ptControl.x * ( (FLOAT)m_pColorTex->ptSize.x / m_ptSize.x ) + 0.5f );
	ptControl.y = LONG( ptControl.y * ( (FLOAT)m_pColorTex->ptSize.y / m_ptSize.y ) + 0.5f );
	return ptControl;
}

tagPoint ZoneMapStatic::UISizeToMap( tagPoint ptUI )
{
	ASSERT( P_VALID( m_pColorTex ) );
	ptUI.x = (LONG)( ptUI.x * ( (FLOAT)m_pColorTex->ptSize.x / m_ptSize.x ) + 0.5f );
	ptUI.y = (LONG)( ptUI.y * ( (FLOAT)m_pColorTex->ptSize.y / m_ptSize.y ) + 0.5f );
	return ptUI;
}

VOID ZoneMapStatic::DrawElement( const INT nIndex, const tagPoint& ptPos, FLOAT fRotate )
{
	ASSERT( nIndex >= 0 && nIndex < EMME_Num );
	if( !m_bElementShow[nIndex] || !P_VALID( m_pElementImg[nIndex] ) || !P_VALID( m_pElementImg[nIndex]->dwHandle ) )
		return;

	tagGUIImage* pImg = m_pElementImg[nIndex];

	DrawImg( m_pElementImg[nIndex], ptPos, 0xFFFFFFFF, fRotate );
}

VOID ZoneMapStatic::DrawImg( const tagGUIImage* pImg, const tagPoint& ptPos, DWORD dwMulColor/* = 0xFFFFFFFF*/, FLOAT fRotate/* = 0.0f */)
{
	ASSERT( P_VALID( pImg ) && P_VALID( pImg->dwHandle ) );

	tagRect rc;
	rc.left   = LONG( ptPos.x * ( (FLOAT)m_ptSize.x / m_pColorTex->ptSize.x ) + 0.5f ) - pImg->ptSize.x / 2;
	rc.top    = LONG( ptPos.y * ( (FLOAT)m_ptSize.y / m_pColorTex->ptSize.y ) + 0.5f ) - pImg->ptSize.y / 2;
	rc.right  = rc.left + pImg->ptSize.x;
	rc.bottom = rc.top + pImg->ptSize.y;

	// 出界，不画
	if( rc.left < 0 || rc.right > m_ptSize.x || rc.top < 0 || rc.bottom > m_ptSize.y )
		return;

	tagRect rcDest = rc + m_ptView - m_pCanvas->ptOffset;
	IDraw2D::Inst()->Draw(	(const RECT*)&rcDest, 
							(const RECT*)&pImg->rc, 
							(ResTexture*)pImg->dwHandle, 
							dwMulColor, ETFilter_POINT, fRotate, IDraw2D::EAWT_NoWrite );
}

#define DRAW_CITY_IMG_IF(a, b) \
	if(a) \
	DrawImg(b[2], WorldToMap(Vector2(itor->second.vPos.x, itor->second.vPos.z)),0xFFFFFFFF, 0.); \
	else \
	DrawImg(b[1], WorldToMap(Vector2(itor->second.vPos.x, itor->second.vPos.z)),0xFFFFFFFF, 0.);

#define DRAW_COLUMN_IMG_IF(a) DRAW_CITY_IMG_IF(CityMgr::Inst()->IsColumnBeDestoryed(dwMapID, a), m_pColumns)

#define LONGZHU_POS (Vector2(730*TILE_SCALE, 755*TILE_SCALE)))

void  ZoneMapStatic::DrawCityWarInfo(DWORD dwMapID)
{
	if(dwMapID != MapMgr::Inst()->GetCurMapID() || !CityMgr::Inst()->IsCityWarMap(dwMapID)) return;
	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLocalPlayer)) return;
	if(pLocalPlayer->IsInState(ES_CSAttack) || pLocalPlayer->IsInState(ES_CSDefence))
	{
		if(dwMapID == MapMgr::Inst()->GetCurMapID())
		{
			GameMap& gameMap = MapMgr::Inst()->GetGameMap();
			const vector<tagMapDoor>& mapDoor = gameMap.GetMapDoor();
			if(mapDoor.size() == 3)  // 一共3个门
			{
				int index = 0;
				for(vector<tagMapDoor>::const_iterator itor=mapDoor.begin(); itor!=mapDoor.end(); itor++)
				{
					if(CityMgr::Inst()->IsDoorBeDestoryed(dwMapID, index))
						DrawImg(m_pDoor[2], WorldToMap(Vector2(itor->vPos.x, itor->vPos.z)),0xFFFFFFFF, 0.); 
					else
						DrawImg(m_pDoor[1], WorldToMap(Vector2(itor->vPos.x, itor->vPos.z)),0xFFFFFFFF, 0.); 
					index++;
				}
			}
			if(CityMgr::Inst()->IsDragonBeDestoryed(dwMapID)) 
				DrawImg(m_pDragon[2], WorldToMap(LONGZHU_POS,0xFFFFFFFF, 0.); 
			else 
				DrawImg(m_pDragon[1], WorldToMap(LONGZHU_POS,0xFFFFFFFF, 0.);
			const map<DWORD,tagMapNPC>& mapMPC = gameMap.GetNpc();
			for(map<DWORD,tagMapNPC>::const_iterator itor = mapMPC.begin(); itor!=mapMPC.end(); itor++)
			{
				DWORD typeID =itor->second.dwTypeID;
				switch(typeID)
				{
				//case  4900029: // 龙珠
				//	DRAW_CITY_IMG_IF(CityMgr::Inst()->IsDragonBeDestoryed(dwMapID), m_pDragon);
				//	break;
				case 4900030: // 玄机阴阳柱
					DRAW_COLUMN_IMG_IF(0);
					break;
				case 4900043: // 太冲阴阳柱
					DRAW_COLUMN_IMG_IF(1);
					break;
				case 4900044: // 天伏阴阳柱
					DRAW_COLUMN_IMG_IF(2);
					break;
				case 4900045: // 申火阴阳柱
					DRAW_COLUMN_IMG_IF(3);
					break;
				case 4900046: // 刑妄阴阳柱
					DRAW_COLUMN_IMG_IF(4);
					break;
				}
			}
		}
	}
	else // 非城战
	{
		GameMap& gameMap = MapMgr::Inst()->GetGameMap();
		const vector<tagMapDoor>& mapDoor = gameMap.GetMapDoor();
		if(mapDoor.size() == 3)  // 一共3个门
		{
			int index = 0;
			for(vector<tagMapDoor>::const_iterator itor=mapDoor.begin(); itor!=mapDoor.end(); itor++)
			{
				if(CityMgr::Inst()->IsDoorBeDestoryed(dwMapID, index))
					DrawImg(m_pDoor[0], WorldToMap(Vector2(itor->vPos.x, itor->vPos.z)),0xFFFFFFFF, 0.); 
				else
					DrawImg(m_pDoor[0], WorldToMap(Vector2(itor->vPos.x, itor->vPos.z)),0xFFFFFFFF, 0.); 
				index++;
			}
		}
		DrawImg(m_pDragon[0], WorldToMap(LONGZHU_POS,0xFFFFFFFF, 0.); 
		const map<DWORD,tagMapNPC>& mapMPC = gameMap.GetNpc();
		for(map<DWORD,tagMapNPC>::const_iterator itor = mapMPC.begin(); itor!=mapMPC.end(); itor++)
		{
			DWORD typeID =itor->second.dwTypeID;
			switch(typeID)
			{
			//case  4900029: // 龙珠
			//	DrawImg(m_pDragon[0], WorldToMap(Vector2(itor->second.vPos.x, itor->second.vPos.z)),0xFFFFFFFF, 0.); 
			//	break;
			case 4900030: // 玄机阴阳柱
			case 4900043: // 太冲阴阳柱				
			case 4900044: // 天伏阴阳柱
			case 4900045: // 申火阴阳柱
			case 4900046: // 刑妄阴阳柱
				DrawImg(m_pColumns[0], WorldToMap(Vector2(itor->second.vPos.x, itor->second.vPos.z)),0xFFFFFFFF, 0.); 
				break;
			}
		}
	}
}

void  ZoneMapStatic::DrawBattleWarInfo(DWORD dwMapID)
{
	if(dwMapID != MapMgr::Inst()->GetCurMapID()) 
		return;
	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLocalPlayer)) 
		return;
	if(pLocalPlayer->IsInState(ES_CSAttack) || pLocalPlayer->IsInState(ES_CSDefence))
	{
		for( int i = 0; i < EFT_Monster + 1; i++)
		{
			const tagFlagInfo info = GuildMgr::Inst()->GetFlagState((EFlagType)i);
			switch(info.eState)
			{
			case EGCT_NULL:
				DrawImg(m_pColumns[0], WorldToMap(Vector2(info.vPos.x, info.vPos.z)),0xFFFFFFFF, 0.);
				break;
			case EGCT_Defend:
				DrawImg(m_pColumns[1], WorldToMap(Vector2(info.vPos.x, info.vPos.z)),0xFFFFFFFF, 0.);
				break;
			case EGCT_Capture:
				DrawImg(m_pColumns[2], WorldToMap(Vector2(info.vPos.x, info.vPos.z)),0xFFFFFFFF, 0.);
				break;
			}
		}
	}
}
