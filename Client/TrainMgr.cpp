#include "StdAfx.h"
#include "TrainMgr.h"

namespace SortTrain
{
	class SortByTrainID
	{
	public:
		bool operator()(const tagTrainProtoData &op1, const tagTrainProtoData &op2)
		{
			//试炼状态处于打开状态的排在前面
			if (op1.eType == ETS_Open)
			{
				return 1;
			} 
			else if (op1.eType != ETS_Open)
			{
				return 0;
			}
			return op1.dwTrainID < op2.dwTrainID;
		}
	};
}

TrainMgr::TrainMgr()
: m_trunk(this), 
m_pNetCmdMgr(NULL),
m_pGameFrameMgr(NULL),
m_pSession(NULL),
m_dwCurLoongTrainID(NULL)
{
}

TrainMgr::~TrainMgr()
{
}

bool TrainMgr::Init()
{
	if (!LoadTrainProto())
		return false;

	return true;	
}

VOID TrainMgr::Update()
{
	
}

void TrainMgr::Destroy()
{
	
	m_trains.clear();
}

TrainMgr* TrainMgr::Inst()
{
	static TrainMgr inst;
	return &inst;
}

bool TrainMgr::LoadTrainProto()
{
	if (!(m_data.LoadFromFile()))
		return false;
	InitTrainTable();

	return true;
}

void TrainMgr::InitTrainTable()
{
	const TrainProtoData::TrainProtoMap &data = m_data.GetData();
	TrainProtoData::TrainProtoMap::const_iterator begin = data.begin();
	TrainProtoData::TrainProtoMap::const_iterator end	= data.end();
	for (; begin != end; ++begin)
	{
		tagTrainProtoData temp = begin->second;
		m_trains.push_back(temp);
	}
}

void TrainMgr::SortTrains()
{
	SortTrain::SortByTrainID sortobj;
	std::sort(m_trains.begin(), m_trains.end(), sortobj);

}
const TrainMgr::TrainVector& TrainMgr::GetTrainVector()
{
	return m_trains;
}

DWORD TrainMgr::GetTrainID( DWORD index ) const
{
	if (index < m_trains.size())
		return m_trains[index].dwTrainID;
	return -1;
}

const VOID  TrainMgr::SetNewState(DWORD dwID,ETrainState eState)
{
	const TrainProtoData::TrainProtoMap &data = m_data.GetData();
	TrainProtoData::TrainProtoMap::const_iterator iter =  data.find(dwID);
	if (iter != data.end())
	{
		iter->second.eType = eState;
	}
}
const VOID TrainMgr::SetCompletNum(DWORD dwID,INT nNum)
{
	const TrainProtoData::TrainProtoMap &data = m_data.GetData();
	TrainProtoData::TrainProtoMap::const_iterator iter =  data.find(dwID);
	if (iter != data.end())
	{
		iter->second.nCurrentNum = nNum;
	}
}
VOID TrainMgr::ShowCenterMsgPic(tstring str)
{
	TrainFrame* pFrame = (TrainFrame*)m_pGameFrameMgr->GetFrame(_T("TrainFrame"));
	if( P_VALID(pFrame) )
	{
		pFrame->ShowPicCenter(str);
	}
}
