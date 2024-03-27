#pragma once
#include "CB_StateScript.h"
class CB_STATEDeadScript :
    public CB_StateScript
{
public:
    virtual void begin() override;
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;


public:
    void Dead();

public:
    CLONE(CB_STATEDeadScript);
    CB_STATEDeadScript();
    virtual ~CB_STATEDeadScript();
};

