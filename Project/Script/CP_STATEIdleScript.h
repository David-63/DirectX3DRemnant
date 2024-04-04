#pragma once
#include "CP_StatesScript.h"

class CP_STATEIdleScript : public CP_StatesScript
{
private:

public:
    virtual void tick() override;

public:
    virtual void CallAnimation() override;

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CP_STATEIdleScript);

    CP_STATEIdleScript();
    virtual ~CP_STATEIdleScript();
};

