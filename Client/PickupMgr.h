#pragma once

enum
{
	EbMoney =0,	// 钱
	EbEquip,			// 装备
	EbMedicine ,	// 药水
	EbStrengthen,	// 强化材料
	EbGem,			// 宝石圣印
	EbQuest ,			// 任务活动
	EbSprite ,			// 妖精相关
	EbOthers ,		// 其他物品

	// 拾取品级限制
	EbWrite,			// 白
	EbYellow ,		// 黄
	EbGreen ,			// 绿
	EbBlue,			// 蓝
	EbOrange ,		// 橙
	EbPurple ,		// 紫
	EbPink ,			// 粉
};


struct tagPickStrategy
{
		// 拾取类型限制
	DWORD	bMoney :1,			// 钱
		bEquip :1,			// 装备
		bMedicine :1,		// 药水
		bStrengthen :1,	// 强化材料
		bGem :1,				// 宝石圣印
		bQuest :1,			// 任务活动
		bSprite :1,			// 妖精相关
		bOthers :1,			// 其他物品
									
		// 拾取品级限制
		bWrite :1,				// 白
		bYellow :1,			// 黄
		bGreen :1,			// 绿
		bBlue :1,				// 蓝
		bOrange :1,			// 橙
		bPurple :1,			// 紫
		bPink :1;				// 粉

	tagPickStrategy()
	{
		//	默认全部选中
		memset( this,-1,sizeof(*this) );
		//ZeroMemory(this,sizeof(*this));
		
	}
	tagPickStrategy& operator=(const tagPickStrategy& tag)
	{
		if(this==&tag)
			return *this;

		ZeroMemory(this,sizeof(*this));
		memcpy( this, &tag, sizeof(tagPickStrategy) );

		return *this;
	}
};

struct tagNS_PickItemStrategy;
class PickupMgr
{
	static PickupMgr s_guiPickMgr;
	PickupMgr(void);
	~PickupMgr(void);
	PickupMgr& operator=(PickupMgr&);
	PickupMgr(const PickupMgr&);
public:
	VOID Init();
	VOID Destroy();
	
	VOID OpenUI();
	VOID CloseUI();

	VOID SavePickStrategy();
	bool CanPick(DWORD itemID,int bQuality);
	tagPickStrategy& GetPickStrategy(){return m_PickStrategy;}
	static PickupMgr* Inst();
private:
	// 角色上线的时候同步拾取策略
	DWORD OnNS_PickItemStrategy(tagNS_PickItemStrategy* pNetCmd,DWORD);
private:
	TSFPTrunk<PickupMgr>			m_Trunk;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>	m_pFrameMgr;

	tagPickStrategy						m_PickStrategy;
};
