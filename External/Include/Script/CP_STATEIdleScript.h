#pragma once
#include "CP_StatesScript.h"

class CP_STATEIdleScript : public CP_StatesScript
{
private:
    bool        m_isMove;
    bool        m_isMelee;

    int         m_horizontal;
    int         m_vertical;

public:
    virtual void tick() override;


public:
    virtual void CallAnimation() override;


public:
    void MoveInput();
    void IdleMouseInput();

public:
    void IdleNormalInput();
    void IdleAimInput();
    void IdleCrouchInput();
    void IdleCrouchAimInput();

    void ClearIdleState() { m_isMove = false; m_isMelee = false; }

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CP_STATEIdleScript);

    CP_STATEIdleScript();
    virtual ~CP_STATEIdleScript();
};

