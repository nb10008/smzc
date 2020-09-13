#include "StdAfx.h"
#include "ActionStateTransform.h"

ActionStateTransform::ActionStateTransform(void)
{
}

ActionStateTransform::~ActionStateTransform(void)
{
}

void ActionStateTransform::Active( NavMap* pNav,LocalPlayer* pRole )
{

}

void ActionStateTransform::OnExit()
{

}

void ActionStateTransform::Update( NavMap* pNav,LocalPlayer* pRole )
{

}

void ActionStateTransform::OnGameEvent( tagGameEvent* pEvent )
{

}

void ActionStateTransform::OnNetCmd( tagNetCmd* pNetCmd )
{

}

bool ActionStateTransform::IfCanChange2UpperState( FSM_LP::EUpperState state,tagGameEvent* pEvent )
{
    return false;
}

bool ActionStateTransform::IfCanChange2LowerState( FSM_LP::ELowerState state,tagGameEvent* pEvent )
{
    return false;
}