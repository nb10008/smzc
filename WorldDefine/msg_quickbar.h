//-----------------------------------------------------------------------------
//!\file msg_quickbar.h
//!
//!\date 20010-02-5
//!
//!\brief 客户端与服务器快捷栏保存至服务器消息
//-----------------------------------------------------------------------------
// 快捷栏数据保存说明：
// 快捷栏总共有80个，由于数据传输的限制现在分成5次发送，客户端向服务器发送，每次发送2排，每排8个
//  
#pragma once
#pragma pack(push, 1)

const INT QUICK_BAR_COUNT = 2 ;  //快捷栏每次2排
const INT QUICK_BAR_BUTTON_COUNT = 8 ;//每一排有八个空格
const INT QUICK_BAR_SEND_COUNT = 5 ;  //快捷栏数据发送次数 

enum ENUM_QUICK_BAR
{
	QUICK_BAR_ONE = 0 ,
	QUICK_BAR_TWO = 1 ,
	QUICK_BAR_THREE = 2,
	QUICK_BAR_FOUR  = 3,
	QUICK_BAR_FIVE  = 4,
	QUICK_BAR_END,
};

//看情况是否从数据库加载数
enum ENUM_NEEDTOLOAD
{
	ENUMQUICK_TOLOAD = 0 ,
	ENUMQUICK_NOTTOLOAD = 1,
	ENUMQUICK_END,
};

//是否保存过 
enum ENUM_QUICKBAR_ISSAVED
{
	Enum_Saved = 0 ,    //数据库已经存过 
	Enum_NotSaved = 1,  //没有存过
	Enum_End,
};

struct tagQuickData
{
	INT16 	            m_eType;		//	按钮类型
	DWORD				m_dwParam1;		//	对应数据1(skill:id,	item:数目+位置, petskill:petid)
	DWORD				m_dwParam2;		//	对应数据2(skill:NULL,	item:typeid, petskill:skillid(带等级))
	INT64				m_n64Param3;		//	(skill:NULL, item: 64id, petskill:null)

	tagQuickData()
	{
		m_eType = 0;		
		m_dwParam1 = 0;		
		m_dwParam2 = 0;		
		m_n64Param3 = 0;
	}

};

struct tagQuickBarState
{
   INT16 	            m_eType;		//	按钮类型
   BOOL                 m_bIsChange;
   DWORD				m_dwParam1;		
   DWORD				m_dwParam2;		
   INT64				m_n64Param3;
};
//-----------------------------------------------------------------------------
// 快捷栏数据(发送给服务器) 第一版本
//-----------------------------------------------------------------------------
CMD_START(NC_MsgQuickBarData)
DWORD dwRoleID;
INT16 nLn;  //快捷栏所在的行
INT16 nCol; //快捷栏所在的列
tagQuickData btnData; //快捷栏两排
CMD_END

//-----------------------------------------------------------------------------
// 通知服务器保存数据(发送给服务器) 第一版本
//-----------------------------------------------------------------------------
CMD_START(NC_MsgInformSaveQuickBarData)

CMD_END

//-----------------------------------------------------------------------------
// 快捷栏数据(发送给客户端)
//-----------------------------------------------------------------------------
CMD_START(NS_MsgQuickBarData)
DWORD dwRoleID;
INT16 eQuickSendCount;    //快捷栏发送次数
tagQuickData btnData[QUICK_BAR_COUNT][QUICK_BAR_BUTTON_COUNT]; //快捷栏两排
CMD_END

//-----------------------------------------------------------------------------
// 从数据库读取数据给客户端
//-----------------------------------------------------------------------------
CMD_START(NC_MsgQuickBarDataLoadFromDB)
INT16 n16IsSave;    //是否要存
DWORD dwRoleID;
CMD_END


CMD_START(NS_MsgIsSavedQuickBarData)
DWORD dwRoleID;
INT16 nIsSaved;    //是否已经存过
CMD_END

CMD_START(NC_MsgIsSavedQuickBarData)
DWORD dwRoleID;
CMD_END
#pragma pack(pop) 