#pragma once
#include "iconstatic.h"

enum ERollState
{
	ERS_WaitForStart = 0,
	ERS_Starting,
	ERS_SlowStopping,
};
class RollStatic :
	public IconStatic
{
public:
	RollStatic(void);
	virtual ~RollStatic(void);

	virtual VOID Update();


	VOID InitData();
	VOID Stop(int step = 0);
	VOID Start();
	VOID StopImmediately(int step = 0);

private:

	LONG   m_lLeft;
	LONG	m_lTop;
	int		m_iTexW;
	int		m_iTexH;

	LONG	m_lSpeed;
	LONG	m_lLoopValue;

	int		m_iStopStep;
	bool		m_bStarted;

	ERollState m_eState;
};
