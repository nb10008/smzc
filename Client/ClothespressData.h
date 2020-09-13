#pragma once


struct tagClothespressData
{
	DWORD nSuitId;
	DWORD dwSuitTypeID[5];
	DWORD dwBufferID[3];
};

class ClothespressData
{
public:
	ClothespressData(void);
	~ClothespressData(void);

	static ClothespressData* Inst();

	/** \‘ÿ»Î“¬≥˜–≈œ¢
	*/
	virtual void LoadFromFile();
	const vector<tagClothespressData>& GetMaleFashionData() { return m_vecMaleFashion; }
	const vector<tagClothespressData>& GetFemaleFashionData() { return m_vecFemaleFashion; }
	tstring GetSuitName(INT nSuitID );
	const tagClothespressData &GetDIYFashion() { return m_DiyFashion; }
	tagClothespressData* GetFashionInfo(DWORD dwIndex);
	size_t GetAllFashionNum() { return 5*m_mapFashion.size(); }

private:
	map<DWORD, tagClothespressData> m_mapFashion;
	vector<tagClothespressData> m_vecMaleFashion;
	vector<tagClothespressData> m_vecFemaleFashion;
	tagClothespressData m_DiyFashion;
	map<DWORD, tstring> m_mapSuitName;
};
