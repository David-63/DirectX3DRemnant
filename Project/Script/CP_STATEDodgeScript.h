#pragma once
#include "CP_StatesScript.h"

class CP_STATEDodgeScript : public CP_StatesScript
{
private:
public:
    virtual void tick() override;

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CP_STATEDodgeScript);

    CP_STATEDodgeScript();
    virtual ~CP_STATEDodgeScript();
};
