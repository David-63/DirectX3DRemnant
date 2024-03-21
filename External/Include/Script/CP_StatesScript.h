#pragma once
#include "CC_StatesScript.h"

enum class eP_States
{
    IDLE,
    MOVE,
    DODGE,
    FIRE,
    RELOAD,
    MELEE,
    DAMAGED,
    DEAD,
    END,
};

class CP_FSMScript;
class CP_StatesScript : public CC_StatesScript
{
protected:
    CP_FSMScript*   m_PHQ;
private:
    eP_States       m_PStateType;

public:
    virtual void begin() override;

public:
    virtual void CallAnimation() {}

public:
    virtual void SetStateType(UINT _StateType) override { m_PStateType = static_cast<eP_States>(_StateType); }
    virtual UINT GetStateType() const override { return static_cast<UINT>(m_PStateType); }

public:
    CLONE(CP_StatesScript);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
public:
    CP_StatesScript();
    virtual ~CP_StatesScript();
};

