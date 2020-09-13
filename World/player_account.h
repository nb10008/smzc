/********************************************************************
	created:	2010/01/27
	created:	27:1:2010   13:21
	filename: 	e:\loong\trunk\LoongWorld\player_account.h
	file path:	e:\loong\trunk\LoongWorld
	file base:	player_account
	file ext:	h
	author:		Jason
	
	purpose:	
*********************************************************************/


#ifndef player_account_h__
#define player_account_h__


#include "../ServerDefine/mini_tool.h"

class PlayerSession;

/**
 * \ingroup LoongWorld
 *
 *	与离线时间统计相关的用户账号信息
 *	不排除将来扩展为完整信息的可能
 *
 * \version 1.0
 * first version
 *
 * \date 2010-01-27
 *
 * \author Jason
 *
 *
 */
struct PlayerAccount
{
	DWORD			m_dwAccountID;
	tagDWORDTime	m_timeLastLoginTime; // 账号退出时，再修改，并保存到数据库
	tagDWORDTime	m_timeLastLogoutTime;
	tagDWORDTime	m_timeLastReceiveDailyOfflineReward;
	tagDWORDTime	m_timeLastReceiveRegression;
	tagDWORDTime	m_timeTemLogin; // 这个用于缓存当前首次登录时间，它用于时间判断

	PlayerAccount();
	~PlayerAccount();

	PlayerAccount(const PlayerAccount & account);
	PlayerAccount & operator= (const PlayerAccount & account);

	/**
	 * 初始化玩家离线时间相关信息
	 * \param accountid 账号id
	 * \param logintime 上次账户玩家，退出游戏时间
	 * \param logouttime 上次玩家账号，退出时间
	 * \param dailyoffline 上次领取每日离线经验时间
	 * \param regression 上次领取回归奖励的时间
	 */
	VOID	Assign( DWORD accountid,tagDWORDTime logintime,tagDWORDTime logouttime,tagDWORDTime dailyoffline,tagDWORDTime regression,tagDWORDTime timeTemLogin );
	// 会影响账号登陆时间
	VOID	Login( PlayerSession * pPly );	
	// 会影响账号登出时间，并发送db server 更新玩家账号中的登出时间
	VOID	Logout( PlayerSession * pPly );
	// 得到账户，离线时间
	DWORD	GetOfflineTime();
	// 计算账户，当天离线时间
	DWORD	GetDailyOfflineTime();

	BOOL	CanReceiveDailyOfflineReward();
	BOOL	CanReceiveRegressionReward(DWORD delta);

};

/**
 * \ingroup LoongWorld
 *
 *	账号管理器，为了确保效率和线程安全，没有直接导出PlayerAccount（指针或引用），而是采用了比较低级的再封装的办法
 *
 * \version 1.0
 * first version
 *
 * \date 2010-01-27
 *
 * \author Jason
 *
 *
 */
class PlayerAccountMgr
{
	jm_tool::CSCreatedLock m_cs;
	typedef std::map<DWORD,PlayerAccount> TAccountMap;
	TAccountMap	m_mapAccount;
	PlayerAccountMgr(const PlayerAccountMgr &);
	PlayerAccountMgr & operator= (const PlayerAccountMgr & );
public:
	PlayerAccountMgr();
	~PlayerAccountMgr();

	// 收到db server 返回的信息后，调用，初始化一个账号时间信息
	VOID OnRecAccountInfo(const PlayerAccount &);

	// 影响玩家账号中，记录的登录时间，在玩家首次登陆时，调用
	VOID RoleLogin(DWORD accountid);
	// 影响玩家账号中，记录的登出时间，在玩家即将退出游戏时，调用
	VOID RoleLogout(DWORD accountid);
	// 得到账户，离线时间
	DWORD	GetOfflineTime(DWORD accountid);
	// 计算账户，当天离线时间
	DWORD	GetDailyOfflineTime(DWORD accountid);

	BOOL	CanReceiveDailyOfflineReward(DWORD accountid);
	BOOL	CanReceiveRegressionReward(DWORD accountid,DWORD delta);

	// 属性 当天离线奖励领取时间设置/读取
	VOID	ReceiveDailyOfflineRewardTime( DWORD accountid, tagDWORDTime  rec_time ) ;
	tagDWORDTime	ReceiveDailyOfflineRewardTime(DWORD accountid);

	// 属性 回归奖励领取设置/读取
	VOID	ReceiveRegressionRewardTime( DWORD accountid, tagDWORDTime  rec_time);
	tagDWORDTime ReceiveRegressionRewardTime(DWORD accountid);

	VOID	SaveAccountTimeInfoOneTick( DWORD accountID );
};


extern PlayerAccountMgr g_PlayerAccountMgr;

#endif // player_account_h__


