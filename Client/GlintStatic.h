#pragma once

class GlintStatic : public GUIStatic
{
public:
	GlintStatic(void);
	~GlintStatic(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Update();
	virtual VOID Destroy();
	virtual VOID Render();

	bool IsGlintNow(){ return m_bShowGlint; }

	void BeginGlint()
	{ 
		m_bShowGlint = true;
		if( m_bShowGlint )
			m_dwLastGlint = timeGetTime();
	}

	void EndGlint()
	{
		m_bShowGlint = false;
	}

	/** 设置闪烁时候的图片
	*/
	void SetGlintPic( tstring strPath )
	{
		if( !P_VALID(m_pGlintPic) )
		{
			m_pRender->DestroyImage( m_pGlintPic );
			m_pGlintPic = NULL;
		}

		if( strPath.empty() )
			return;

		m_pGlintPic = m_pRender->CreateImage( strPath, m_rcPic );
	}

	/** 设置闪烁图片的颜色
	*/
	void SetGlintPicColor( DWORD dwColor )
	{
		m_dwGlintPicColor = dwColor;
	}

	/**  设置闪烁时候的文字
	*/
	void SetGlintText( tstring strText )
	{
		m_strGlintText = strText;
	}

	/** 设置闪烁时文字的颜色
	*/
	void SetGlintTextColor( DWORD dwColor ){ m_dwGlintTextColor = dwColor; }

	/**
	设置多长时间闪烁一下, 单位是毫秒
	*/
	void SetGlintInterval( DWORD dwInterval ){ m_dwGlintInterval = dwInterval; }

protected:
	void DrawGlint();

protected:
	bool m_bShowGlint;							//是否闪烁
	bool m_bShowInterval;

	tagGUIImage*	m_pGlintPic;				//闪烁图片
	DWORD			m_dwGlintPicColor;			//闪烁图片颜色

	tstring			m_strGlintText;				//闪烁时候的文字
	DWORD			m_dwGlintTextColor;			//文字闪烁的颜色
	DWORD			m_dwGlintInterval;			//多长时间闪烁一下，毫秒
	DWORD			m_dwLastGlint;				//上次计时的时间
};
