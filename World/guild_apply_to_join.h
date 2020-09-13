/********************************************************************
	created:	2010/02/08
	created:	8:2:2010   10:25
	filename: 	e:\loong\trunk\LoongWorld\guild_apply_to_join.h
	file path:	e:\loong\trunk\LoongWorld
	file base:	guild_apply_to_join
	file ext:	h
	author:		Jason
	
	purpose:	申请加入帮派
*********************************************************************/


#ifndef guild_apply_to_join_h__
#define guild_apply_to_join_h__


class Role;

struct tagGuildPriority
{
	DWORD	dwGuildID;
	tagGuildPriority()
		:dwGuildID(0){}
	tagGuildPriority(DWORD dwV)
		:dwGuildID(dwV){}
	virtual ~tagGuildPriority(){}

	tagGuildPriority& operator = ( const tagGuildPriority & t )
	{
		dwGuildID = t.dwGuildID;
		return *this;
	}
	tagGuildPriority(const tagGuildPriority & t)
		:dwGuildID(t.dwGuildID){}
};
bool operator < (const tagGuildPriority & t,const tagGuildPriority & p);
bool operator == (const tagGuildPriority & t,const tagGuildPriority & p);




/**
 * \ingroup LoongWorld
 * 申请加入帮派辅助工具类
 *
 * \par requirements
 *
 * \version 1.0
 * first version
 *
 * \date 2010-02-09
 *
 * \author Jason
 *
 * 
 * \todo 
 *
 * \bug 
 *
 */
class ApplyToJoinGuildToolkit
{
protected:
	ApplyToJoinGuildToolkit(){}
	virtual ~ApplyToJoinGuildToolkit(){}
public:
	
	/**
	 * 工厂方法，线程安全的单件模式
	 * \return 
	 */
	static ApplyToJoinGuildToolkit * GetApplyToJoinGuildToolkit();

	/**
	 * 在帮派初始化后（after having loaded all guild informations from db server），初始化时，调用
	 * \param dwGuildID 帮派id
	 */
	virtual VOID AddGuild(DWORD dwGuildID) = 0;
	virtual VOID DelGuild(DWORD dwGuildID) = 0;

	/**
	 * 得到帮派排行表，只有前100个
	 * \param pRole 
	 * \return 
	 */
	virtual DWORD GetGuildRanking(Role * pRole) = 0;

	/**
	* 帮派排行，只有前100个
	* \param pRole 
	* \return 
	*/
	virtual VOID GuildRanking() = 0;
	
	/**
	 * 玩家申请加入某个帮派，会给帮派增加申请单，并回复消息给role
	 * \param pRole 
	 * \return 
	 */
	virtual DWORD ApplyToJoinGuild(Role * pRole,DWORD dwGuildID) = 0;

	/**
	 * 帮主回复
	 * \param pGuildOwner 
	 * \param dwSrcRoleID 
	 * \param dwGuildID 
	 * \return 
	 */
	virtual DWORD ResponseApplierToJoinGuild(Role * pGuildOwner,DWORD dwSrcRoleID,DWORD dwGuildID) = 0;

	/**
	 * 帮主取出申请加入帮派帮派清单，发送清单给role
	 * \param pRole 
	 * \param dwGuildID 
	 * \return 
	 */
	virtual DWORD GetApplyBill(Role * pRole,DWORD dwGuildID) = 0;

	/**
	 * 得到帮派宗旨，并返回消息给role
	 * \param pRole 
	 * \param dwGuildID 
	 * \return 
	 */
	virtual DWORD GetGuildTenet( Role * pRole,DWORD dwGuildID ) = 0;

	virtual VOID Update() = 0;

	/**
	 * 联系帮主 
	 * \param pRole 
	 * \param dwGuildID 
	 * \return 
	 */
	virtual DWORD ConstractGuildMaster( Role * pRole,DWORD dwGuildID ) = 0;

	virtual DWORD OnGuildMasterNotice( Role * pRole ) = 0;

	/**
	 * 同意或者拒绝申请的入派请求，如果对方在线，会通知对方
	 * \param pMaster 
	 * \param dwApplier 
	 * \param agreement 
	 * \return 
	 */
	virtual DWORD ApproveToJoinGuild( Role * pMaster , DWORD dwApplier , BOOL agreement = FALSE ) = 0;

	virtual DWORD SearchGuild(Role * pRole, INT findType,const tstring & strFindWhat) = 0;

	virtual DWORD ChangeGuildMember( DWORD dwGuildID, DWORD dwRoleID ) = 0;
};



#endif // guild_apply_to_join_h__



