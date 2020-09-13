#pragma once

#include "MiniMapStatic.h"

/** \class   ZoneMapStatic
    \brief   区域地图控件，继承MiniMapStatic
*/

class ZoneMapStatic : public MiniMapStatic
{
public:
	ZoneMapStatic();
	virtual ~ZoneMapStatic();
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Update();
	virtual VOID Render();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);

	virtual tagPoint PointToMap( tagPoint ptPoint );
	virtual tagPoint UISizeToMap( tagPoint ptUI );

	VOID	SetCurShowMapID( const DWORD dwMapID ) { m_dwCurShowMapID = dwMapID; }
	DWORD	GetCurShowMapID() const { return m_dwCurShowMapID; }
private:
	VOID DrawElement( const INT nIndex, const tagPoint& ptPos, FLOAT fRotate = 0.0f );
	VOID DrawImg( const tagGUIImage* pImg, const tagPoint& ptPos, DWORD dwMulColor = 0xFFFFFFFF, FLOAT fRotate = 0.0f );

	void  DrawCityWarInfo(DWORD dwMapID);
	void  DrawBattleWarInfo(DWORD dwMapID);
private:
	tagPoint		m_ptLP;					// 本地玩家地图坐标
	DWORD			m_dwCurShowMapID;		// 当前显示的地图ID
	tagGUIImage*	m_pColumns[3];			// 0, 非城战； 1，守方；2，攻方
	tagGUIImage*	m_pDragon[3];			// 0, 非城战； 1，守方；2，攻方
	tagGUIImage*	m_pDoor[3];				// 0, 非城战； 1，守方；2，攻方
};
