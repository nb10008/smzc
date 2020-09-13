#include "StdAfx.h"
#include "Role.h"
#include "RoleShake.h"


RoleShake::RoleShake(void):
m_pRole(NULL),
m_bStarted(false),
m_type( ED_Axis_X ),
m_nShakeD(0),
m_nFrames(0)
{
	
}

RoleShake::~RoleShake(void)
{
}

void RoleShake::BindRole( Role* pRole, EDir type )
{
	m_pRole = pRole;
	m_type = type;
}

void RoleShake::Start( DWORD dwTimeLen, int nShakeD )
{
	if( m_bStarted )
		return;

	m_totalTime = dwTimeLen;
	m_nFrames = m_totalTime / 33;
	m_nShakeD = nShakeD;
	m_startTime = Kernel::Inst()->GetAccumTimeDW();
	m_bStarted = true;
}

void RoleShake::Update()
{
	if( !m_bStarted )
		return;

	if( m_nFrames <= 0 || m_totalTime == 0 )
		return;

	DWORD runTime = Kernel::Inst()->GetAccumTimeDW() - m_startTime;
	if( runTime <= m_totalTime )
	{
		int nIndex = int( m_nFrames * runTime / m_totalTime );
		if( nIndex < 0 )
			nIndex = 0;
		else if( nIndex >= m_nFrames )
			nIndex = m_nFrames -1;
		
		m_pRole->SetPos( m_pRole->GetShakeReferPos() + GetShakeOffset(nIndex), true );
	}
	else
	{
		m_bStarted = false;
		m_totalTime = 0;
		m_pRole->SetPos( m_pRole->GetShakeReferPos() );
		return;
	}
}

bool RoleShake::IsShaking()
{
	return m_bStarted;
}

Vector3 RoleShake::GetShakeOffset( int nIndex )
{
	Vector3 vec( 0, 0, 0 );
	if( m_bStarted )
	{
		float fValue = m_nShakeD * ( 1.0f - float(nIndex)/float(m_nFrames) );
		if( nIndex % 2 != 0 )
			fValue *= -1;
		
		if( m_type == ED_Axis_X )
			vec.x = fValue;
		else if( m_type == ED_Axis_Y )
			vec.y = fValue;
		else if( m_type == ED_Axis_Z )
			vec.z = fValue;
	}

	return vec;
}