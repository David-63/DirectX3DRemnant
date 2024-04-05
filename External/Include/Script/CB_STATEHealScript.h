#pragma once
#include "CB_StateScript.h"



class CB_STATEHealScript :
    public CB_StateScript
{

public:
    virtual void begin() override;

    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;


private:
    float m_fTime;
public:
    void BloodDrink_LoopEnd();



public:
    CLONE(CB_STATEHealScript);
    CB_STATEHealScript();
    virtual ~CB_STATEHealScript();



};

