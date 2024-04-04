#pragma once
#include "CP_StatesScript.h"

class CP_STATEStaggerScript : public CP_StatesScript
{
private:
    int     m_hitDir;
    float   m_maxTime;
    float   m_curTime;
public:
    virtual void tick() override;

public:
    virtual void CallAnimation() override;

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CP_STATEStaggerScript);

    CP_STATEStaggerScript();
    virtual ~CP_STATEStaggerScript();
};
