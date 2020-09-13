#pragma once
#include "iconstatic.h"

class ChangeStatic :
	public IconStatic
{
public:
	ChangeStatic(void);
	~ChangeStatic(void);

	virtual VOID Update();
	VOID SetWAndH(int w,int h){m_iTexW = w; m_iTexH = h; }
	VOID PushPoint(LONG left,LONG top){POINT pt; pt.x =left; pt.y = top; m_vecPointList.push_back(pt); }
	VOID InitData();
	VOID Loop(bool b ){m_bLoopIng = b;}
	VOID SetTimeSpace(DWORD dw){m_dwTimeSpace = dw;}

private:
	LONG   m_lLeft;
	LONG	m_lTop;
	int		m_iTexW;
	int		m_iTexH;
	std::vector<POINT>	m_vecPointList;

	bool m_bLoopIng;
	std::vector<POINT>::iterator m_it;

	DWORD m_dwTimeDelat;
	DWORD m_dwTimeSpace;

};
