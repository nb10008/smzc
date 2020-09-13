#pragma once
class EggButton;
class IconStatic;

enum EEggLayerNum
{
	EELN_Null = -1,
	EELN_Begin = 0,
	EELN_1 =	0,
	EELN_2 =	1,
	EELN_3 =	2,
	EELN_4 =	3,
	EELN_5 =	4,
	EELN_6 =	5,
	EELN_7 =	6,

	EELN_END = 6,
};

enum EEggPosNum
{
	EEPN_Null = -1,
	EEPN_Begin = 0,
	EEPN_1 =	0,
	EEPN_2 =	1,
	EEPN_3 =	2,
	EEPN_4 =	3,
	EEPN_5 =	4,
	EEPN_6 =	5,
	EEPN_7 =	6,

	EEPN_END = 6,
};

class EggLayer
{
public:
	EggLayer(void);
	~EggLayer(void);
	
	void init(EEggLayerNum layer);
	void SetEnable(bool bEnable);
	EEggLayerNum GetLayer(){return m_nLayer; }
	void pushback(EggButton* pBn,IconStatic* pSt);
	void clear();
	void SetColourEgg(EEggPosNum pos);
	void SetNormalEgg(EEggPosNum pos);
	void SetBrokedEgg(EEggPosNum pos,bool bEmpty=false,bool bColour=false);
	vector<EggButton*>& GetEggButtonList(){return m_vecEgg;}
	vector<IconStatic*>& GetEggButtonBackList(){return m_vecEggBack;}
private:
	vector<IconStatic*> m_vecEggBack;
	vector<EggButton*> m_vecEgg;
	EEggLayerNum	m_nLayer;

	tagRect rectEmpty;
	tagRect rectNormalBreak;
	tagRect rectColourBreak;

};
