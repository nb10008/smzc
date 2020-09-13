#pragma once

class Role;

class RoleShake
{
public:
	enum EDir
	{
		ED_None = -1,
		ED_Axis_X,
		ED_Axis_Y,
		ED_Axis_Z
	};

public:
	RoleShake(void);
	~RoleShake(void);

	//! 绑定角色
	void BindRole( Role* pRole, EDir type = ED_Axis_X );

	//! 开始振动
	void Start( DWORD dwTimeLen, int nShakeD );

	//! 旋转处理
	void Update();

	//! 返回当前是否正在旋转中
	bool IsShaking();

private:
	//! 得到振动产生的偏移
	Vector3 GetShakeOffset( int nIndex );

private:
	Role* m_pRole;

	bool  m_bStarted;		// 已经开始振动否?
	EDir  m_type;			// 振动产生便宜的方向
	float m_startTime;		// 振动的开始时间
	DWORD m_totalTime;		// 振动的总时间
	int	  m_nShakeD;		// 振动的幅度

	int	  m_nFrames;		// m_totalTime 时间内有的帧数
};
