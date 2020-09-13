/********************************************************************
	created:	2010/01/15
	created:	15:1:2010   15:41
	filename: 	e:\loong\trunk\LoongWorld\offline_reward.h
	file path:	e:\loong\trunk\LoongWorld
	file base:	offline_reward
	file ext:	h
	author:		Jason
	
	purpose:	离线挂机功能
*********************************************************************/

#ifndef offline_reward_h__
#define offline_reward_h__


class Role;

/**
 * \ingroup LoongWorld
 *
 *	离线奖励累计器
 *
 * \version 1.0
 * first version
 *
 * \date 2010-01-15
 *
 * \author jpma
 *
 * \bug 
 *
 */
class OfflineReward
{
	DWORD	m_dwAccountID;	// 当前玩家账号
	DWORD	m_dwRoleID;		// 玩家离线时角色id
	enum {E_OfflineTimeout = 24 * 60 * 2};	//离线挂机最长时间
public:
	OfflineReward()
		: m_dwAccountID(0),m_dwRoleID(0)
	{
	}
	~OfflineReward(){}

	enum{ E_Offline_NotFindPlayer = 1, E_ConfigFileNotComplete = 2, E_OfflineHang = 3, E_RefuseOfflineReward = 4, E_NotEnoughMoney = 5};

	/**
	 *	在玩家登陆地图后，也即在role_init成功调用后，再调用这个函数
	 * \param player 玩家角色
	 * \return 成功返回E_Success
	 */
	DWORD PlayerLogin(Role * player);
	// 用法同上,不过，为了防止误用，加了一个参数，这个参数很重要
	DWORD PlayerLogout(Role * player,BOOL exitGame = FALSE);

	// what领取类型，0表示领取免费经验，1表示领取全部经验
	DWORD RecExp(Role * player,BYTE what,INT & exp);

	static INT OfflineRewardSwitch();
	static void OfflineRewardSwitch(INT switch_v);

private:
	/**
	 * 计算玩家免费离线挂，经验
	 * \param player 
	 * \return 可获得经验值
	 */
	INT64 CalOfflineRewardFreeExp(Role * player, INT nMinute);
	/**
	 * 计算玩家付费经验值
	 * \param player 
	 * \return 付费经验值
	 */
	INT64 CalOfflineRewardPayedExp(Role * player, INT nMinute);
	INT64 CalOfflineExp(Role * player);

	INT CalOfflineMinutes(Role * player);
};


#endif // offline_reward_h__


