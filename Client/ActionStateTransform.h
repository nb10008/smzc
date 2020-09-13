#pragma once
#include "ActionState_LP.h"

//! \class  ActionStateTransform
//! \base   ActionState_LP
//! \brief  玩家变形状态
//! \detail 此状态玩家用其它模型替换
//! \author hyu
//! \date   2009-12-18

class ActionStateTransform :
    public ActionState_LP
{
public:
    ActionStateTransform(void);
    ~ActionStateTransform(void);

    //--ActionState_LP
    virtual void Active(NavMap* pNav,LocalPlayer* pRole);
    virtual void OnExit();
    virtual void Update(NavMap* pNav,LocalPlayer* pRole);
    virtual void OnGameEvent(tagGameEvent* pEvent);
    virtual void OnNetCmd(tagNetCmd* pNetCmd);
    virtual bool IfCanChange2UpperState(FSM_LP::EUpperState state,tagGameEvent* pEvent);
    virtual bool IfCanChange2LowerState(FSM_LP::ELowerState state,tagGameEvent* pEvent);
};
