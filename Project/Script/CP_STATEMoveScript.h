#pragma once
#include "CP_StatesScript.h"

class CP_STATEMoveScript : public CP_StatesScript
{
private:
    Vec2        m_prevDir;
    tTimeCtrl   m_readyToFire;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void moveVelocity();

public:
    virtual void CallAnimation() override;

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CP_STATEMoveScript);

    CP_STATEMoveScript();
    virtual ~CP_STATEMoveScript();
};

