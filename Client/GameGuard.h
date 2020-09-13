#pragma once

class GameGuard
{
	TSFPTrunk<GameGuard>		m_Trunk;
	static GameGuard s_inst;
	GameGuard();
	GameGuard(const GameGuard&);
public:
	~GameGuard();
	BOOL NPGameMonCallback(DWORD dwMsg, DWORD dwArg);
	static GameGuard* Inst();
};