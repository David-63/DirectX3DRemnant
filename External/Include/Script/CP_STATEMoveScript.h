#pragma once
#include "CP_StatesScript.h"

class CP_STATEMoveScript : public CP_StatesScript
{
private:
    bool        m_isMelee;

public:
    virtual void tick() override;

public:
    // Input Buffer로 관리해야함
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

