#pragma once

struct tagEggLayerData
{
	int nLayer;
	int nNormalHammers;		// 金蛋消耗锤子的数量
	int nColourHammers;		// 彩蛋消耗锤子数量
	list<DWORD> list_Items;

	tagEggLayerData():nLayer(-1),nNormalHammers(0),nColourHammers(0)
	{
		list_Items.clear();
	}

	void clear()
	{
		nLayer = -1;
		nNormalHammers =0;
		nColourHammers =0;
		list_Items.clear();
	}
};

class EggData
{
public:
	EggData(void);
	~EggData(void);

	void LoadFromFile();
	const tagEggLayerData* GetEggLayerInfo(const int& nLayer);
private:
	map<int,tagEggLayerData*>	m_mapEggLayerInfo;

};
