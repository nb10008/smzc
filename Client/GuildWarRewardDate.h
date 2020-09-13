#pragma once

enum EGuildPos
{
	EGP_HuiYuan = 0,		// 会员
	EGP_JingYing,				// 精英
	EGP_HuiZhang,			// 会长
	EGP_FuHuiZhang,		// 副会长
	EGP_GuGan,					// 骨干
};

struct GuildRewardData
{
	EGuildPos	ePos;
	INT				nOccupyNum;
	DWORD		dwItem;
	DWORD		dwBuff;
};

class GuildWarRewardDate
{
public:
	typedef std::map<pair<EGuildPos,INT>,GuildRewardData> RewardMap;

	GuildWarRewardDate(void);
	~GuildWarRewardDate(void);

	BOOL	LoadFormFile();
	const GuildRewardData* FindData(const EGuildPos& pos,const INT& num) const;

public:
	RewardMap m_mapGuildWarReward;
};
