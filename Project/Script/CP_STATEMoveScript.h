#pragma once
#include "CP_StatesScript.h"

class CP_STATEMoveScript : public CP_StatesScript
{
private:
    bool        m_isMelee;
    bool        m_isFront; // 앞 뒤 구분
    bool        m_isDodge;

    Vec2        m_prevDir;

public:
    virtual void tick() override;

private:
    void translateInput();

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

