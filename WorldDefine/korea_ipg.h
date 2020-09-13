//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_item.h
// author: 
// actor:
// data: 2011-9-6
// last:
// brief: 韩国ipg消息结构
//-----------------------------------------------------------------------------
#pragma once

//#define	CHECK_CASH_PORT		23100
//#define	CHECK_BALANCE_PORT	23200
#define USERNO_LEN 40
#define USERID_LEN 40

#define IPG_PACKET_TYPE_SAVECASH  100			//存款(‘100’)
#define IPG_PACKET_TYPE_TAKECASH  200			//提款(‘200’)
#define IPG_PACKET_TYPE_TAKECASHCANCEL  201		//提款取消(‘201’)

#pragma pack(push, 1)
/*----------port定义---------------------- 
功能					Port
存款(取消)/提款(取消)	23100
余额查询				23200
Cash送礼				23300
充值					23400
-----------------------------------------*/


/*------------------存款(取消)/提款(取消)-----------------------------------
name			client		bytes	require	description
packet_type		char		4		Y		Packet 类型(in) => ‘100’
user_no			char		40		Y		用户编码 (in)
user_id			char		40		Y		用户 ID (in)
company_cd		char		4		Y		公司code (in) ‘DFLY’
nick_name		char		30		Y		用户昵称 (in)
event_no		long		6		Y		活动编号 (in)
event_amt		long		12		Y		活动金额 (in)
agency_no		char		20		N		加盟店订单号(in)
etc1			char		100		N		其他1 (in)
user_name		char		50		Y		名字 (in)
social_no		char		13		N		身份证号 (in)   ‘-‘不带
email			char		50		N		E-MAIL (in)
zip_cd			char		6		N		邮政编码 (in)   ‘-‘不带
tel_no			char		20		N		电话号码 (in)
sex				char		1		Y		性别 (in)
birthday_dt		char		8		N		生日 (in)
nation_cd		char		2		Y		ISO 国家code (in)
sponsor_cd		char		20		Y		赞助code (in)
user_grade		char		2				用户等级(out)
bxaid			char		20				订单号 (out)
retcode			char		4				BillFlexIPG 结果code (retval)

* packet_type : 存款(‘100’), 存款取消 (‘101’), 提款(‘200’), 提款取消(‘201’)
* sex : ‘F’, ‘M’
----------------------------------------------------------------------------*/
//typedef struct
//{
//	char	pack_type[4];			// '200'
//	char	UserNo[USERNO_LEN];		//用户编码						[in]
//	char	UserId[USERID_LEN];		//用户id	        			[in]
//	char	CompanyCd[4+1];			//网站code						[in]
//	char	NickName[30];			//用户昵称						[in]
//	char	UserIp[15+1];			//用户 ip   					[in]
//	char	OrgBxaid[20+1];			//取消时原本订单号				[in]
//	char	GoodsNo[6];				//物品编号 						[in]
//	char	EventNo[6];				//活动编号						[in]
//	char	Amt[12];			     //活动,物品金额    			[in]
//	char	AgencyNo[20+1];			//加盟店订单号					[in]
//	char	Etc1[100];				//自有变数1						[in]
//	char	Etc2[100];				//自有变数2						[in]
//	char	Etc3[100];				//自有变数3						[in]
//	char	Etc4[100];				//自有变数4						[in]
//	char	UserName[50];			//名字							[in]
//	char	SocialNo[13+1];			//居民编号 – null允许('-'不带)	[in]
//	char	Email[50];				//E-MAIL – null允许			[in]
//	char	ZipCd[6+1];				//邮政编码 – null允许('-'不带)	[in]
//	char	TelNo[20];				//电话号码(手机) – null允许	[in]
//	char	Sex[2];					//性别							[in]
//	char	BirthdayDt[9];			//生日 – null允许				[in]
//	char	NationCd[3];			//ISO 国家code – null允许		[in] 
//	char	SponsorCd[20];			//赞助code- null允许			[in]
//	char	UserGrade[2+1];			//用户等级						[out]
//	char	Bxaid[20+1];			//BillFlex IPG 订单号			[out]
//	char	RetCode[4+1];			//BillFlex IPG 结果值			[out]
//} PACKET_INOUT, *pPACKET_INOUT;

//获取cash
/*----------------------------------------------------------------------------
name				client		bytes	require		Description
user_no				char		40		Y			用户编码(in)
user_id				char		40		Y			用户 ID (in)
company_cd			char		4		Y			公司code (in) ‘DFLY’
nick_name			char		30		Y			用户昵称 (in)
cash_shop			long		12					商城cash余额(out)
cash_contents		long		12					Cash余额信息(out)
cash_bonus			long		12					奖励cash余额(out)
cash_etc			long		12					其他cash余额(out)
mileage_shop		long		12					商城积分 (分数) 余额 (out)
mileage_contents	long		12					积分信息 (信息) 余额 (out)
retcode				char		4					BillFlexIPG 结果code(retval)
----------------------------------------------------------------------------*/

//struct PACKET_GETCASH
//{
//	char	UserNo[USERNO_LEN];		//捞侩磊 锅龋					[in]
//	char	UserId[USERID_LEN];		//捞侩磊 id					[in]
//	char	CompanyCd[4+1];			//荤捞飘 内靛					[in]
//	char	NickName[30];			//捞侩磊 喊疙					[in]
//	
//	char	CashShop[12];			//Shopping-Mall Cash		[out]
//	char	CashContents[12];		//Contents Cash				[out]
//	char	CashBonus[12];			//Bonus Cash				[out]
//	char	CashEtc[12];			//Mileage Point				[out]
//	char	MileageShop[12];		//Shopping-Mall Mileage		[out]
//	char	MileageContents[12];	//Contents Mileage			[out]
//	char	RetCode[4+1];			//BillFlex IPG 搬苞 code. 	[out]
//	
//	PACKET_GETCASH() { memset( this, 0x00, sizeof(PACKET_GETCASH) ); strcpy( CompanyCd, "DFLY" ); } 
//};

typedef struct tagGetCash
{
	char	UserNo[USERNO_LEN];		//用户编码						[in]
	char	UserId[USERID_LEN];		//用户id	        			[in]
	char	CompanyCd[4+1];			//网站code						[in]
	char	NickName[30];			//用户昵称						[in]
	char	CashShop[12];			//商城cash余额					[out]
	char	CashContents[12];		//Cash余额信息					[out]
	char	CashBonus[12];			//奖励cash余额					[out]
	char	CashEtc[12];			//其他cash余额					[out]
	char	MileageShop[12];		//商城积分 (分数) 余额			[out]
	char	MileageContents[12];	//积分信息 (信息) 余额			[out]
	char	RetCode[4+1];			//BillFlex IPG 结果值			[out]

	tagGetCash() { memset( this, 0x00, sizeof(tagGetCash) ); strcpy( CompanyCd, "DFLY" ); } 

} GETCASH, *PGETCASH;



//提款
/*----------------------------------------------------------------------------
name			client	bytes	require		description
packet_type		char	4		Y			Packet 类型 (in) => ‘200’
user_no			char	40		Y			用户编码 (in)
user_id			char	40		Y			用户 ID (in)
company_cd		char	4		Y			公司code (in) ‘DFLY’
nick_name		char	30		Y			用户昵称 (in)
user_ip			char	15		Y			用户 IP (in)
goods_no		long	6		Y			物品编号(in)
pay_amt			long	12		Y			物品金额 (in)
agency_no		char	20		N			加盟店订单号(in)
etc2			char	100		N			其他2 (in)
etc3			char	100		N			其他3 (in)
etc4			char	100		N			其他4 (in)
bxaid			char	20					订单号(out)
retcode			char	4					BillFlexIPG 结果code (retval)
----------------------------------------------------------------------------*/
typedef struct tagTakeCash
{
	char	pack_type[4];			// '200'
	char	UserNo[USERNO_LEN];		//用户编码						[in]
	char	UserId[USERID_LEN];		//用户id	        			[in]
	char	CompanyCd[4+1];			//网站code						[in]
	char	NickName[30];			//用户昵称						[in]
	char	UserIp[15+1];			//用户 ip   					[in]
	char	GoodsNo[6];				//物品编号 						[in]
	char	Amt[12];			     //活动,物品金额    			[in]
	char	AgencyNo[20+1];			//加盟店订单号					[in]
	char	Etc2[100];				//自有变数2						[in]
	char	Etc3[100];				//自有变数3						[in]
	char	Etc4[100];				//自有变数4						[in]
	char	Bxaid[20+1];			//BillFlex IPG 订单号			[out]
	char	RetCode[4+1];			//BillFlex IPG 结果值			[out]
} TACKCASH, *PTACKCASH;

//提款取消
/*----------------------------------------------------------------------------
name			client	bytes	require	description
packet_type		char	4		Y		Packet 类型(in) => ‘201’
user_no			char	40		Y		用户编码(in)
user_id			char	40		Y		用户 ID (in)
company_cd		char	4		Y		公司code (in) ‘DFLY’
nick_name		char	30		Y		用户昵称 (in)
user_ip			char	15		Y		用户 IP (in)
org_bxaid		char	20		Y		提款时订单号(in)
goods_no		long	6		Y		物品编码 (in)
agency_no		char	20		N		加盟店订单号(in)
etc2			char	100		N		其他2 (in)
etc3			char	100		N		其他3 (in)
etc4			char	100		N		其他4 (in)
bxaid			char	20				订单号 (out)
retcode			char	4				BillFlexIPG 结果code (retval)
----------------------------------------------------------------------------*/
typedef struct tagTakeCashCancel
{
	char	pack_type[4];			// '200'
	char	UserNo[USERNO_LEN];		//用户编码						[in]
	char	UserId[USERID_LEN];		//用户id	        			[in]
	char	CompanyCd[4+1];			//网站code						[in]
	char	NickName[30];			//用户昵称						[in]
	char	UserIp[15+1];			//用户 ip   					[in]
	char    OrgBxaId[20+1];			//提款时订单号					[in]
	char	GoodsNo[6];				//物品编号 						[in]
	char	AgencyNo[20+1];			//加盟店订单号					[in]
	char	Etc2[100];				//自有变数2						[in]
	char	Etc3[100];				//自有变数3						[in]
	char	Etc4[100];				//自有变数4						[in]
	char	Bxaid[20+1];			//BillFlex IPG 订单号			[out]
	char	RetCode[4+1];			//BillFlex IPG 结果值			[out]
} TACKCASHCANCEL, *PTACKCASHCANCEL;

//存款处理
/*----------------------------------------------------------------------------
name				client		bytes		require		description
packet_type			char		4			Y			Packet 类型(in) => ‘100’
user_no				char		40			Y			用户编码 (in)
user_id				char		40			Y			用户 ID (in)
company_cd			char		4			Y			公司code (in) ‘DFLY’
nick_name			char		30			Y			用户昵称 (in)
event_no			long		6			Y			活动编号 (in)
event_amt			long		12			Y			活动金额 (in)
agency_no			char		20			N			加盟店订单号(in)
etc1				char		100			N			其他1 (in)
user_name			char		50			Y			名字 (in)
social_no			char		13			N			身份证号 (in)   ‘-‘不带
email				char		50			N			E-MAIL (in)
zip_cd				char		6			N			邮政编码 (in)   ‘-‘不带
tel_no				char		20			N			电话号码 (in)
sex					char		1			Y			性别 (in)
birthday_dt			char		8			N			生日 (in)
nation_cd			char		2			Y			ISO 国家code (in)
sponsor_cd			char		20			Y			赞助code (in)
user_grade			char		2						用户等级(out)
bxaid				char		20						订单号 (out)
retcode				char		4						BillFlexIPG 结果code (retval)
----------------------------------------------------------------------------*/
typedef struct tagSaveCash
{
	char	pack_type[4];			// '200'
	char	UserNo[USERNO_LEN];		//用户编码						[in]
	char	UserId[USERID_LEN];		//用户id						[in]
	char	CompanyCd[4+1];			//网站code						[in]
	char	NickName[30];			//用户昵称						[in]
	char	EventNo[6];				//活动编号						[in]
	char	Amt[12];			    //活动,物品金额					[in]
	char	AgencyNo[20+1];			//加盟店订单号					[in]
	char	Etc1[100];				//自有变数1						[in]
	char	UserName[50];			//名字							[in]
	char	SocialNo[13+1];			//居民编号 – null允许('-'不带)	[in]
	char	Email[50];				//E-MAIL – null允许			[in]
	char	ZipCd[6+1];				//邮政编码 – null允许('-'不带)	[in]
	char	TelNo[20];				//电话号码(手机) – null允许	[in]
	char	Sex[2];					//性别							[in]
	char	BirthdayDt[9];			//生日 – null允许				[in]
	char	NationCd[3];			//ISO 国家code – null允许		[in] 
	char	SponsorCd[20];			//赞助code- null允许			[in]
	char	UserGrade[2+1];			//用户等级						[out]
	char	Bxaid[20+1];			//BillFlex IPG 订单号			[out]
	char	RetCode[4+1];			//BillFlex IPG 结果值			[out]
} SAVECASH, *PSAVECASH;

#pragma pack(pop)
