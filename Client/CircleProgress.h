#pragma once

/** \class   CircleProgress
\brief   CircleProgress，继承GUIProgress
\remarks 底图使用GUIStatic::m_pPic来旋转，可以往逻辑区域内添加可旋转的图片
		 在文字渲染区域，渲染百分比
*/
struct tagCircleImage
{
	tagGUIImage*	img;			// 图片
	FLOAT			fSpeed;			// 转速
	tagRect			rcDest;			// 目标区域
	BOOL			bIsClockwise;	// 转向
};

class CircleProgress : public GUIProgress
{
public:
	CircleProgress(void);
	~CircleProgress(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	
	//! 加入的图片可以旋转，为匀速旋转
	VOID AddNewPic(const tstring strPath, tagRect rcDest, tagRect picRect, BOOL isClockwise = TRUE, FLOAT dwSpeed = 0);
private:
	vector<tagCircleImage*>		m_vecPicList;		
	FLOAT						m_fLastTime;
	INT							m_nCircleCount;
	FLOAT						m_fLastValue;
	FLOAT						m_fLastAngel;
};
