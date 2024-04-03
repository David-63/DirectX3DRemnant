#pragma once
#include "CP_StatesScript.h"

class CP_STATEDodgeScript : public CP_StatesScript
{
private:
    Vec2 m_evadeDir;
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
