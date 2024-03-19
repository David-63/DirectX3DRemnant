#pragma once
#include "CB_StateScript.h"
class CB_STATERangedScript :
    public CB_StateScript
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CLONE(CB_STATERangedScript);
    CB_STATERangedScript();
    virtual ~CB_STATERangedScript();

};

