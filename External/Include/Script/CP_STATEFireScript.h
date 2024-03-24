#pragma once
#include "CP_StatesScript.h"

class CP_STATEFireScript : public CP_StatesScript
{
private:
public:
    virtual void tick() override;

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CP_STATEFireScript);

    CP_STATEFireScript();
    virtual ~CP_STATEFireScript();
};
