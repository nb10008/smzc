#pragma once

class FlashButton
	: public GUIButton
{
public:
	FlashButton(void);
	~FlashButton(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Update();

	void SetFlash(bool bVal, bool bText = false);
private:
	bool m_bFlash;
	bool m_bTextFlash;

};

class FlashPushButton
	: public GUIPushButton
{
public:
	FlashPushButton(void);
	~FlashPushButton(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Update();

	void SetFlash(bool bVal);
private:
	bool m_bFlash;

};

class FlashStatic
	: public GUIStatic
{
public:
	FlashStatic(void);
	~FlashStatic(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Update();

	void SetFlash(bool bText);
private:
	bool m_bTextFlash;

};