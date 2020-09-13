#pragma once

struct tagCreateRolePreviewItem
{
	BYTE bSex;					// 性别
	INT nCareerType;			// 职业
	DWORD dwWeapon;				// 武器
	tstring strWeaponEffect1;	// 武器特效1
	tstring strWeaponEffect2;	// 武器特效2
	tstring strWeaponEffect3;	// 武器特效3
	DWORD dwHat;				// 头盔
	DWORD dwUpper;				// 上衣
	DWORD dwLower;				// 下衣
	DWORD dwGlove;				// 手腕
	DWORD dwBoot;				// 鞋子
	DWORD dwEffectID;			// 装备特效
	tagCreateRolePreviewItem()
	{
		memset( this, 0, sizeof(*this) );
	}
};

class CreateRolePreviewData
{

public:
	CreateRolePreviewData(void);
	~CreateRolePreviewData(void);

	virtual void LoadFromFile();

	const tagCreateRolePreviewItem* FindPreviewInfoBySexAndCareer( BYTE bSex, INT nCareerType )
	{
		map<INT,  tagCreateRolePreviewItem>::const_iterator iter = m_mapPreviewCfg.begin();
		for( iter; iter != m_mapPreviewCfg.end(); iter++ )
		{
			if( iter->second.bSex == bSex && iter->second.nCareerType == nCareerType )
			{
				return &(iter->second);
			}
		}
		return NULL;
	}

private:
	map<INT,  tagCreateRolePreviewItem>	m_mapPreviewCfg;		// 职业装备展示
};
