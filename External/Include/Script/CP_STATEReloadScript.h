#pragma once
#include "CP_StatesScript.h"

class CP_STATEReloadScript : public CP_StatesScript
{
private:
public:
    virtual void tick() override;

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CP_STATEReloadScript);

    CP_STATEReloadScript();
    virtual ~CP_STATEReloadScript();
};
