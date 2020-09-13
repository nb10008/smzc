#pragma once

#include "msg_common_errorcode.h"
#define MAX_MD5_ARRAY	36
#define MAX_MIBAO		6

#pragma pack(push, 1)

//--------------------------------------------------------------------------------------------
// Login Mode
//--------------------------------------------------------------------------------------------
enum
{
	LoginMode_InnerTest = 0,					//！		登录模式：0			内测使用
	LoginMode_MainLand = 1,						//！		登录模式：1			大陆
	LoginMode_Thunder = 2,						//！		登录模式：2			迅雷
	LoginMode_Taiwan_Orange = 3,				//！		登录模式：3			台湾_Loong		（橘子）
	LoginMode_SDOA = 4,							//！		登录模式：4			盛大
	LoginMode_Korea = 5,						//！		登录模式：5			韩国
	LoginMode_Vietnam = 6,						//！		登录模式：6			越南
	LoginMode_Taiwan_OMG = 7,					//！		登录模式：7			台湾_SM			（OMG）
	LoginMode_Singapore_OMG = 8,				//！		登录模式：8			新加坡				（OMG）	（在登录模式0的基础上增加了保存密码的功能）
	LoginMode_Douke = 9,						//!			登录模式：9			豆客登录模式
	LoginMode_JinshanKK = 10,					//!			登录模式：10		金山快快
	LoginMode_Korea_SM = 11,					//!			登陆模式：11		韩国_SM
};


//------------------------------------------------------------------------------
// 运营商ID
//------------------------------------------------------------------------------
enum EChannel_Type
{
	ECT_JinShanKuaiKuai =  10013,   //金山快快
	ECT_YYYuYinPingTai  =  10113 ,   //YY语音平台
};


#define CROP_NAME_MAX_LEN 10

//储存格式是 节点1大小 + 节点1内容 + 节点2大小 + 节点2内容
//--------------------------------------------------------------------------------------------
// Login 错误代码
//--------------------------------------------------------------------------------------------

enum
{
	E_ProofResult_Account_No_Match				= 1,	//此帐号非法(帐户不存在或者密码不对,密保错误)
	E_ProofResult_Account_In_Use				= 2,	//此帐号当前已经在使用
	E_ProofResult_Account_Freezed				= 3,	//此帐号被停用
	E_ProofResult_Wrong_Build_Number			= 4,	//版本号不对
	E_ProofResult_Disabled						= 5,	//此帐号已经被封停
	E_ProofResult_Queue							= 6,	//开始排队	
	E_ProofResult_Mibao_Error					= 7,	//密保错误
	E_ProofResult_Wrong_Type					= 8,	//非法验证策略
	E_ProofResult_Proofing						= 9,	//正在验证或已经验证完成

	E_SelectWorld_Server_Full					= 11,	//该服务器当前拥挤
	E_SelectWorld_Server_Suspended				= 12,	//该服务器当前阻塞
	E_SelectWorld_Server_Maintenance			= 13,	//该服务器当前维护
	E_SelectWorld_Failed						= 14,	//出现异常，选择失败
	E_SelectWorld_Short_Time					= 15,	//间隔时间太短,请稍候重试
	E_SelectWorld_No_Select						= 16,	//你已经选择成功,不可再选
	E_SelectWorld_GameWorldName_Wrong			= 17,	//非法的游戏世界名


	E_ProofResult_Forbid_MemberCenter			= 18,	//您的会员中心账号已被封停，请联系客服
	E_ProofResult_Forbid_GMTool					= 19,	//您的游戏账号已被封停，请联系客服	
	E_ProofResult_Forbid_CellPhone				= 20,	//您的账号已被锁定，请联系客服
	E_ProofResult_Forbid_MiBao					= 21,	//您的账号已经挂失密保卡，请完成解绑后登录

	E_ProofResult_Forbid_Wrong_OTP				= 22,	//則系統回傳〝請您關閉遊戲登入畫面，並進行GASH帳號升級，由beanfun!登入遊戲，網址為tw.beanfun.gamania.com〞
	E_ProofResult_Forbid_Wrong_Event_OTP		= 23,	//〝您已升級為beanfun!帳號，請由beanFun登入〞

	E_ProofResult_Orange_VipBar_Limited			= 24,	//"本服务器是金牌网吧专用，请选择其他服务器"
	E_ProofResult_AuthServer_Unreachable		= 25,	//"无法连接到认证服务器"
	E_ProofResult_EmptyField					= 26,	//字段不能为空
	E_PrrofResult_WrongFormat					= 27,	//格式错误
	// Jason 2010-4-26 v2.1.0
	E_PrrofResult_PKRunawayPunishmentState		= 28,	// PK逃逸惩罚状态
	E_ProofResult_TimeOut                       = 29,

	E_ProofResult_GameNet_CanNotConnect_WEB     = 30,	//不能和验证GameNet进行通信
	E_ProofResult_GameNet_Error_Token			= 31,	//非法的token
	E_ProofResult_GameNet_Block					= 32,	//账户已经被GameNet锁定
	E_ProofResult_GameNet_Frozen				= 33,	//账户已经被GameNet冻结
	E_ProofResult_GameNet_ERROR					= 34,	//GameNet验证错误
	E_ProofResult_GameNet_XML					= 35,	//解析XML错误
	E_ProofResult_GameNet_InsertAccount			= 35,	//插入account错误


	E_YYYuYinPingTai_GetTokenID                 = 100,  // YY请求TokenID标识码
	E_KoreaProof_InputOldPsd_Error				= 110,	// 旧密码输入有误
	E_KoreaProof_InputNewPsd_Error				= 111,  // 新密码输入有误
	E_KoreaProof_TwicePsd_ChgError				= 112,	// 密码修改有误
	E_KoreaProof_TwicePsd_SetError				= 113,	// 密码设置有误
	E_KoreaProof_TwicePsd_InputError			= 114,  // 输入密码有误

	E_WebLoginMode_GetURL						= 200,	// Web请求URL地址
};




//---------------------------------------------------------------------------------------------
// 排队时间10分钟
//---------------------------------------------------------------------------------------------
#define QUEUE_TIME	10*60

enum ELM_LoginMode
{
	ELM_NULL				= 0,
	ELM_Web_LoginMode		= 100314789,	//Web登入方式
};

#define CROP_NAME_MAX_LEN 10

struct tagWeb_URLInfo
{
	DWORD dwCropID;
	TCHAR szDescName[CROP_NAME_MAX_LEN];
	DWORD dwSize;
	CHAR  szURL[1];
};

//储存格式是 节点1大小 + 节点1内容 + 节点2大小 + 节点2内容

//-----------------------------------------------------------------------------
// 验证消息
//-----------------------------------------------------------------------------
CMD_START(NLC_SafeProof)
	DWORD	dwProofKey;						// 安全验证key
	CHAR	szUserName[X_SHORT_NAME];		// 用户名
	DWORD	dwType;							// 验证方式
	DWORD	dwCurVersionID;					// 当前版本ID
	DWORD	dwWorldNameCrc;					// 游戏世界名的crc
	CHAR	szPsd[MAX_MD5_ARRAY];			// 密码
	DWORD	dwChannelID;					// 运营商ID
	ELM_LoginMode	dwLoginMode;
	bool    bIsGetTokenID;                  // 是否YY请求TokenID
	bool	bIsGetURL;						// 是否Web请求URL地址
	CHAR    szMac[MAX_MAC_LENGTH];          // mac明文
	CHAR	szUserID[X_SHORT_NAME];			// UserID
	CHAR	szAppKey[X_APPKEY];				// AppKey
	CHAR	szToken[X_TOKEN];				// Token
	CHAR	szGUID[X_LONG_NAME];			// GUID值  必须放在最底下
	DWORD	dwConnID;					// 绑定ID （YY）
	DWORD	dwReserved;				// 保留字段，YY等使用(dwReserved 保存消息大小，byData保存GUID)  
	BYTE    byData[1];
CMD_END;

CMD_START(NLS_ProofResult)
	DWORD	dwErrorCode;					// 错误码
	DWORD   dwLoginSessionID;				// 大区ID
	BOOL	bGuard;							// 防沉迷
	DWORD	dwIndex;						// 等候序列号
	DWORD	dwVerifyCode;					// 登陆服务器发送给客户端的验证码，连接ZoneServer时要用到
	DWORD	dwAccountID;					// 帐号ID
	DWORD   dwIP;							// ZoneServer IP 地址
	DWORD   dwPort;							// ZoneServer 端口号
	DWORD	dwConnID;						// 绑定ID （YY）
	CHAR		szUserName[X_SHORT_NAME];		// 用户名
	DWORD	dwReserved;						// 保留字段，盛大圈圈等使用(1.表示缺少信息，0表示正常)
    DWORD	dwURLNum;						// Web的地址的链接个数
	BOOL	bNeedTelNum;					// 是否还需要用户填写手机号信息
	BYTE    byData[1];                      // 保留字段，YY等使用(dwReserved 保存消息大小，byData保存tokenID)  
CMD_END;

//------------------------------------------------------------------------------
// Web登入
//------------------------------------------------------------------------------
CMD_START(NLS_WebLoginProof)
DWORD dwTokenID;
DWORD dwLoginID;
CHAR  szSign[MAX_MD5_ARRAY*2];
CHAR  szLoginURL[MAX_MD5_ARRAY*2];
DWORD dwURLSize;
CHAR  szURL[1];
CMD_END

//------------------------------------------------------------------------------
// 密保
//------------------------------------------------------------------------------
CMD_START(NLS_MiBao)
	CHAR	szMiBao[MAX_MIBAO];				// 密保矩阵提示码
CMD_END

CMD_START(NLC_MiBao)
	DWORD	dwMBCrc;						// 密保crc
CMD_END

//------------------------------------------------------------------------------
// (韩国)二次密码认证
//------------------------------------------------------------------------------
CMD_START(NLS_SecondPsdProof)
	bool	bIsFirstLogin;
CMD_END

//------------------------------------------------------------------------------
// (韩国)设置二次密码
//------------------------------------------------------------------------------
CMD_START(NLS_SetSecondPsd)
	DWORD	dwEorrorCode;
CMD_END

CMD_START(NLC_SetSecondPsd)
	DWORD	dwOldSecondPsdCrc;
	CHAR	czNewSecondPsd[X_SHORT_NAME];	//二次密码
	bool	bIsChangeTwPsd;					//是否修改二次密码
CMD_END

//------------------------------------------------------------------------------
// (韩国)输入二次密码
//------------------------------------------------------------------------------
CMD_START(NLS_InputSecondPsd)
	DWORD	dwEorrorCode;
CMD_END

CMD_START(NLC_InputSecondPsd)
	DWORD	dwTwicePsdCrc;
CMD_END

//-------------------------------------------------------------------------------
// 排队
//-------------------------------------------------------------------------------
CMD_START(NLS_Queue)
	DWORD	dwPosition;						// 排队的位置
	DWORD	dwTime;							// 排队的剩余时间
CMD_END;

//------------------------------------------------------------------------------
// 排队消息广播
//------------------------------------------------------------------------------
CMD_START(NLS_Broadcast)
TCHAR	szBMsg[X_LONG_NAME];				// 排队消息广播
CMD_END

//-------------------------------------------------------------------------------
//	登录协议验证码			by: Jampe
//-------------------------------------------------------------------------------

#define PK_StrToID(n, s)		do{													\
									n = (DWORD)strlen(s);							\
									DWORD step = (n >> 5) + 1;						\
									for(DWORD i = n; i >= step; i -= step)			\
										n = n ^ ((n << 5) + (n >> 2) + s[i - 1]);	\
								}while(0)

#define ProofKey(k, p)			do{												\
									DWORD n[3];									\
									PK_StrToID(n[0], p->szUserName);			\
									PK_StrToID(n[1], p->szPsd);					\
									PK_StrToID(n[2], p->szGUID);				\
									k = p->dwWorldNameCrc + p->dwCurVersionID;	\
									k = (k << 2) + p->dwType;					\
									k = k + n[0] + n[1] + n[2];					\
								}while(0)


#pragma pack(pop)

