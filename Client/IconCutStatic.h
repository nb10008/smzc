#pragma once
#include "iconstatic.h"


class IconCutStatic :
	public IconStatic
{
public:
	IconCutStatic(void);
	~IconCutStatic(void);

	virtual VOID Update();
	virtual VOID Render();
	virtual VOID Destroy();
	
	VOID SetScaleValue(DWORD dwScale){m_ScaleValue = dwScale; }		// 是百分之多少的缩小  这个数要大于0 小于等于100	只影响到渲染
	VOID SetCurrValue(DWORD dwScale){m_CurrentValue = dwScale; }	
	VOID SetCutNums(DWORD dwNum){m_CutNums = dwNum; }

private:

	DWORD m_ScaleValue;				// 最终缩放值
	DWORD m_CurrentValue;			// 当前缩放值
	DWORD m_CutNums;
};
