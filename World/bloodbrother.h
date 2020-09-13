#pragma once
//-----------------------------------------------------------------------------
// 结拜系统服务器数据结构
//-----------------------------------------------------------------------------
struct tagBrotherData
{
	DWORD	dwTeatherID;	// 结拜大哥的ID（一个人只能有一个大哥，一个大哥可以带5个小弟）
	tagDWORDTime m_dwBrotherEndTime;	// 结拜结束时间，0为没有结拜

	tagBrotherData():dwTeatherID(0)
	{
		m_dwBrotherEndTime.Clear();
	};

	void SetEndTime(tagDWORDTime& time ){m_dwBrotherEndTime = time;}	
	void SetTeatherID(DWORD& dwID){	dwTeatherID = dwID;}	

	void init(DWORD dwID,tagDWORDTime time)
	{
		dwTeatherID = dwID;
		m_dwBrotherEndTime = time;
	}

	void clearTeatherInfo()
	{
		dwTeatherID = 0;
		m_dwBrotherEndTime.Clear();
	}

};



//结拜管理类
class BloodbrotherMgr
{
public:
	BloodbrotherMgr(void);
	~BloodbrotherMgr(void);

	//INT GetBortherNum(){return m_data.nBrotherNum;}	// 获得徒弟的数量，只有师傅才用到此方法，因为师父只能有一个
	DWORD GetTeatherID(){return m_data.dwTeatherID;}
	tagDWORDTime& GetEndTime(){return m_data.m_dwBrotherEndTime;}
	tagBrotherData& GetData(){return m_data;}
	void InitData(DWORD dwID,tagDWORDTime time);

	void SetEndTime(tagDWORDTime& time ){m_data.SetEndTime(time);}
	void SetTeatherID(DWORD& dwID){	m_data.SetTeatherID(dwID);}	
	void ClearTeatherData(){m_data.clearTeatherInfo();}

	bool CanMakeBrother();
private:
	tagBrotherData m_data;

};