/********************************************************************
created:	2008/04/28
filename: 	d:\loong_client\Client\TrainMgr.h
file path:	d:\loong_client\Client
author:		Su.Zhang
purpose:	龙之试炼管理器声明
*********************************************************************/
#pragma once

#include "TrainProtoData.h"
#include "TrainFrame.h"

class TrainMgr
{

public:

	typedef std::vector<tagTrainProtoData> TrainVector;

public:
	~TrainMgr();
	bool Init();
	VOID Update();
	void Destroy();

	static TrainMgr* Inst();

	const TrainProtoData&	GetData() const { return m_data; }
	const TrainVector&		GetTrainVector();
	void	SortTrains();
	DWORD	GetTrainID(DWORD index) const;
	const VOID SetNewState(DWORD dwID,ETrainState eState);
	const VOID SetCompletNum(DWORD dwID,INT nNum);
	VOID ShowCenterMsgPic(tstring str);


private:
	TrainMgr();

	bool LoadTrainProto();

	// 初始化活动表 
	void InitTrainTable();

private:
	TSFPTrunk<TrainMgr>			m_trunk;
	TObjRef<NetCmdMgr>			m_pNetCmdMgr;
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;
	TObjRef<NetSession>			m_pSession;

	TrainProtoData				m_data;				
	TrainVector					m_trains;

	DWORD						m_dwCurLoongTrainID;
};
