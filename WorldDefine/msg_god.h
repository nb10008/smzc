#pragma once
#include "RoleDefine.h"
#include "clergy_define.h"

#pragma pack(push, 1)

CMD_START(NC_GetAttribute)  //获得属性值
	ERoleAttribute eType;
CMD_END

CMD_START(NS_GetAttribute)  //获得属性值反馈
	DWORD dwValue;
CMD_END

CMD_START(NC_PromotionClergy) //晋升
	DWORD dwClergy;				  //神职ID
CMD_END

CMD_START(NS_PromotionClergy) //晋升反馈
	DWORD dwErrorCode;        //错误码
	DWORD dwClergy;               //神职ID
CMD_END

//神职改变消息（广播用）
CMD_START(NS_ChangeClergy)		
	DWORD dwRoleID;					//角色ID
	DWORD dwClergy;					//神职ID
CMD_END

//////////////////////////////////////////////////////////////////////////
//	竞选结果
//////////////////////////////////////////////////////////////////////////
CMD_START(NC_CampaignResult) 
	ECampaignGodType eType;	// 查询的类型
CMD_END

CMD_START(NS_CampaignResult)
	ECampaignGodType eType;		// 查询的类型
	INT nNum;
	tagGodCampaignInfo sResult;	// 查询的结果
CMD_END

/////////////////////////////////////////////////////////////////////////
//	报名竞选
//////////////////////////////////////////////////////////////////////////
CMD_START(NC_Campaign)
	ECampaignGodType eType;
CMD_END

CMD_START(NS_Campaign)
	DWORD dwErrorCode;
CMD_END

/////////////////////////////////////////////////////////////////////////
//	竞选信息
//////////////////////////////////////////////////////////////////////////
CMD_START(NC_CampaignInfo)
CMD_END

CMD_START(NS_CampaignInfo)
	ECampaignGodType eType;	// 已经报名的神职，如果没有报名传ECGT_Null
	DWORD dwNum[ECGT_Num];	// 每一个神职的人数
CMD_END

//----------------------------------------------------------------------
//领域相关
/////////////////////////////////////////////////////////////////////////
//	激活领域能力
/////////////////////////////////////////////////////////////////////////
CMD_START(NC_ActivationArea)	
	DWORD		dwAreaID;				//领域ID
	bool		bActive;				// 是否激活
CMD_END

CMD_START(NS_ActivationArea)
	bool			bActive;			// 是否激活
	DWORD			dwErrorCode;		//错误码
	DWORD			dwAreaID;			//领域ID
	DWORD			dwCD;
CMD_END

/////////////////////////////////////////////////////////////////////////
//	增加领域
/////////////////////////////////////////////////////////////////////////
CMD_START(NC_AddArea)			
	DWORD	dwAreaID;				//领域ID
	bool    bAdd;					//是否增加（该消息不常用，大多数情况为被动增加）
CMD_END

CMD_START(NS_AddArea)			
	bool		   bAdd;			// 是否是增加，如果是false就是失去
	DWORD dwErrorCode;				//错误码
	DWORD dwAreaID;					//领域ID
		DWORD	dwTime;				//剩余时间（秒）（稀有领域专用）
CMD_END

//获取神之系谱相关消息
CMD_START(NC_GetGodRelation)
CMD_END

CMD_START(NS_GetGodRelation)
	ECampaignGodType eType;		// 查询的类型
	INT nNum;
	tagGodCampaignInfo sResult;	// 查询的结果
CMD_END

//稀有领域抢夺信息
CMD_START(NS_RareAreaInfo)
	DWORD	dwAreaID;						// 领域ID
	BOOL	bSuccess;						// 是否抢夺成功
	DWORD	dwRoleID;						// 被抢夺者ID
	DWORD	dwRobRoleID;					// 抢夺者ID
	TCHAR	szRoalName[X_SHORT_NAME];		// 抢夺者名称
CMD_END


#pragma pack(pop)