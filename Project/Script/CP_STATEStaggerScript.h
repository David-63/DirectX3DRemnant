#pragma once
#include "CP_StatesScript.h"

class CP_STATEStaggerScript : public CP_StatesScript
{
private:
public:
    virtual void tick() override;

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CP_STATEStaggerScript);

    CP_STATEStaggerScript();
    virtual ~CP_STATEStaggerScript();
};