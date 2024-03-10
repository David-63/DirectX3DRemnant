#pragma once
#include <Engine\CScript.h>

class CStateScript : public CScript
{
public:
    enum class eState
    {
        IDLE,
        TRACER,
        END,
    };


private:
    eState m_StateType;
    //Transform* m_Transform;


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void Enter() override;
    virtual void Exit() override;

public:
    void SetStateType(eState _StateType)
    {
        m_StateType = _StateType;
    }


public:
    eState GetStateType()	const
    {
        return m_StateType;
    }


public:
    CLONE(CStateScript);

    CStateScript();
    virtual ~CStateScript();

};

