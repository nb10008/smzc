#pragma once

enum EActivityMode
{
	EActivityMode_Day,
	EActivityMode_Week,
	EActivityMode_Month,
	EActivityMode_Year
};

enum ENewActivityType
{
	ENewActivityType_Festival,  // 节日
	ENewActivityType_Scene,		// 场景
	ENewActivityType_Fallow,	// 休闲
	ENewActivityType_PVP,		// PVP
	ENewActivityType_Instance,	// 副本
	ENewActivityType_Shuaguai,	// 刷怪
	ENewActivityType_Daily		// 日常
};

struct tagActivityProtoData 
{
	UINT16 id;				// 活动id
	EActivityMode mode;		// 活动类型
	tstring name;			// 活动名称
	tstring desc;			// 活动说明
	//EActivityType type;	// 活动类型
	BOOL acceptTimeLimit;	// 接取时间限制
	ENewActivityType activityType; // 活动类型
	
	INT  prompt;			// 是否提示
	INT  startyear;        //开始年
	INT  endyear;         //结束年
	INT  startmonth;		// 开始月，节日活动使用
	INT  endmonth;			// 结束月
	INT  startday;			// 开始日，节日活动使用
	INT	 endday;			// 结束日

	EWeek week;				// 星期几
	int startHour;			// 开始小时
	int startMinute;		// 开始分钟
	int endHour;			// 结束小时
	int endMinute;			// 结束分钟
	DWORD acceptNpcID;		// 接取NPC的ID
	INT32 minLevel;			// 等级最小限制
	INT32 maxLevel;			// 等级最大限制
	INT acceptTimes;		// 接取次数限制
	bool visible;			// 是否显示在每日活动列表中
};

struct tagDailyQuestProtoData 
{
	UINT16	id;					// 活动id
	tstring name;				// 活动名称
	UINT16	preID;				// 前置活动id
	INT32	minLevel;			// 等级最小限制
	INT32	maxLevel;			// 等级最大限制
	INT		acceptTimes;		// 接取次数限制
	mutable INT		doneTimes;
	tstring desc;
	DWORD	acceptNpcID;
};

class ActivityProtoData
{
public:
	typedef std::map<UINT16, tagActivityProtoData> ActivityProtoMap;
	typedef std::map<UINT16, tagDailyQuestProtoData> DailyProtoMap;
public:

	BOOL LoadFromFile();
	const ActivityProtoMap& GetData() const;
	const tagActivityProtoData *GetQuestProto(UINT16 id) const;

	const DailyProtoMap& GetDailyData() const;
	const tagDailyQuestProtoData *GetDailyProto(UINT16 id) const;

	VOID ClearDailyQuestDoneTimes();
private:
	ActivityProtoMap m_mapActivityProtoData;	// 活动ID所对应的属性表
	DailyProtoMap    m_mapDaily;
};


