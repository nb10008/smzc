#pragma once

class SkillButton;

class SkillItem
{
public:
	SkillItem(void);
	~SkillItem(void);

	VOID SetInvisible(BOOL b);

	VOID ReloadUI(GUIWnd* pWnd, INT nIndex);

	VOID UpdateSkill(DWORD dwID);

	DWORD GetSkillID();
	GUIWnd* GetSkillWnd();
	GUIWnd* GetProcessWnd();

	tstring GetExp();
private:
	VOID SetName(LPCTSTR szName);
	VOID SetExp(INT nExp, INT nMax);

private:
	SkillButton*	m_pSkill;
	GUIStatic*		m_pStcBack;
	GUIStatic*		m_pStcName;
	GUIStatic*		m_pStcLv;
	GUIProgress*	m_pProExp;

};
