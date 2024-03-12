#pragma once
#include "CP_StatesScript.h"

class CP_STATEMoveScript : public CP_StatesScript
{
private:
    bool        m_isMelee;
    bool        m_isFront; // 菊 第 备盒

    Vec2        m_prevDir;

public:
    virtual void tick() override;

public:
    virtual void CallAnimation() override;

public:
    // Input Buffer肺 包府秦具窃
    void MoveKeyInput();
    void MoveMouseInput();


    void TranslateInput();


    void MoveNormalInput();
    void MoveAimInput();
    void MoveCrouchInput();
    void MoveCrouchAimInput();


public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CP_STATEMoveScript);

    CP_STATEMoveScript();
    virtual ~CP_STATEMoveScript();
};

