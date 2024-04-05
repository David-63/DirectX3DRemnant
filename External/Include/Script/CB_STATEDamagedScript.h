#pragma once
#include "CB_StateScript.h"

class CB_STATEDamagedScript :
    public CB_StateScript
{

public:
    virtual void begin() override;

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    void AnimFinish();
    void BackSpeedZero();


private:
    Vec3 m_vBackDir;
    bool m_bFinish;
    float m_BackSpeed;
    



public:
    CLONE(CB_STATEDamagedScript);
    CB_STATEDamagedScript();
    virtual ~CB_STATEDamagedScript();
};

