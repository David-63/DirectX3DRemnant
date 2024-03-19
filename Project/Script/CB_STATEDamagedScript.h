#pragma once
#include "CB_StateScript.h"

class CB_STATEDamagedScript :
    public CB_StateScript
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CLONE(CB_STATEDamagedScript);
    CB_STATEDamagedScript();
    virtual ~CB_STATEDamagedScript();
};

