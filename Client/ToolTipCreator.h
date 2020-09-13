#pragma once

class Item;
struct tagRoleBuff;
struct tagItemCost;
/** \class ToolTipCreator
	\brief 提示信息生成器
*/
class ToolTipCreator
{
public:
	ToolTipCreator(void);
	~ToolTipCreator(void);

	static ToolTipCreator* Inst();

	tstring GetItemTips(Item* pItem);
	tstring GetItemTips(DWORD dwID, BYTE byQuality = 0, DWORD dwNameId = GT_INVALID);
	tstring GetSkillTips(DWORD dwID);
	tstring GetGESkillTips( DWORD dwID );
	tstring GetTalentTips(DWORD dwID);
	tstring GetBuffTips(const tagRoleBuff* pBuff);
    tstring GetPriceTips(const tagItemCost* pCost, BOOL bSingle = FALSE);
    tstring GetStallPriceTips(const tagItemCost* pCost, int nCount);
    tstring GetPetSkillTips(DWORD dwID);
	tstring GetCommerceGoodsTips(DWORD dwGoodID);
	tstring GetGuildSkillTips(DWORD dwID);
	tstring GetHotKeyTips( LPCTSTR szBaseTip, DWORD key1, DWORD key2, LPCTSTR szSpace = NULL, bool bParenthesis = true );
	tstring GetGuildPosTips( INT nPos );
	tstring GetAreaTips( DWORD dwID );
	tstring GetAreaShowTips( DWORD dwID );
	tstring GetPulseTips( DWORD dwID );

	VOID ShowEquipCompare(DWORD dwTypeID);

    // 解析动态tips类型
    tstring ParseDynamicTips(LPCTSTR szInput);
    tstring ParseKey(tstring& strName, tstring& strValue);
};
