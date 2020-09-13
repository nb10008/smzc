#pragma once
#include "BaseData.h"


class BuyTalentData : public BaseData
{
public:
	BuyTalentData(void);
	~BuyTalentData(void);

	static BuyTalentData* Inst();

	/** 加载购买第N点天资需要的花费
	*/
	virtual void LoadFromFile();

	/** param  表示第dwTalentNum点天资
	*/
	const INT64 FindCost( DWORD dwTalentNum )
	{
		std::map<DWORD, INT64>::iterator iter = m_mapCost.find( dwTalentNum );
		if( iter != m_mapCost.end() )
			return iter->second;
		else
			return 0;
	}

private:
	std::map<DWORD, INT64>	m_mapCost;

};
