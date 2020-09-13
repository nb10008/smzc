#pragma once
#include "state_pet.h"

//! \class  StateSkill_PET
//! \base   State_PET
//! \brief  ³èÎïÊ©·¨×´Ì¬
//! \author hyu
//! \time   2010-1-5
class StateSkill_PET :
    public State_PET
{
public:
    StateSkill_PET(void);
    ~StateSkill_PET(void);

    virtual void Active(tagNetCmd* pNetCmd, NavMap* pNav, Pet* pRole);		
    virtual void Update(NavMap* pNav, Pet* pRole);							
    virtual void OnNetCmd(tagNetCmd* pNetCmd);
    virtual void OnGameEvent(tagGameEvent* pEvent);
};
