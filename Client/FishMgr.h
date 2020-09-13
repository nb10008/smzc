#pragma once

struct tagNS_Fishing;
class ItemFrame;
class LocalPlayer;
class Item;

class FishMgr
{
public:
	FishMgr(void);
	~FishMgr(void);
	
	static FishMgr* Inst();

	VOID SetFishFoodType( DWORD dwFishFoodType );

	DWORD GetFishFoodType();

	VOID SetItemN64ID( INT64 n64ItemID );

	INT64 GetItemN64ID();

	VOID StartFish( Item* pItem );

	VOID Init();

	VOID Destroy();
private:
	ItemFrame*					m_pItemFrame;
	DWORD						m_dwFishFoodType;
	INT64						m_n64ItemID;
};
