#pragma once

class PicStatic
	: public GUIStatic
{
public:
	PicStatic(void);
	~PicStatic(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);

	//ÉèÖÃ¿Ø¼þÍ¼Æ¬
	void SetPic(const tstring& strPicName)
	{
		if( P_VALID(m_pPic) )
			m_pRender->DestroyImage(m_pPic);

		m_strPic = strPicName;
		m_pPic = m_pRender->CreateImage(m_strPic, m_rcPic);
		if( !P_VALID(m_pPic) )
			IMSG(_T("ComposeStatic pic cann't createimage!\n"));

		SetRefresh( TRUE );
	}

	//ÉèÖÃ¿Ø¼þÍ¼Æ¬
	void SetPic(const tstring& strPicName, tagRect rcPic, EGUIAlphaWrite eAlphaWrite=EGUIAW_Write)
	{
		if( P_VALID(m_pPic) )
			m_pRender->DestroyImage(m_pPic);

		m_strPic = strPicName;
		m_rcPic = rcPic;
		m_eAlphaWrite = eAlphaWrite;
		m_dwPicColor = 0xFFFFFFFF;
		m_pPic = m_pRender->CreateImage(m_strPic, m_rcPic);
		if( !P_VALID(m_pPic) )
			IMSG(_T("ComposeStatic pic cann't createimage!\n"));

		SetRefresh( TRUE );
	}
private:
};