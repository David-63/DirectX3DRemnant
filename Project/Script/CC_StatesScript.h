#pragma once
#include <Engine\CScript.h>

class CC_FSMScript;
class CC_StatesScript : public CScript
{
protected:
    CC_FSMScript*   m_FSMHQ;
private:
    UINT            m_StateType; // 사실상 사용안함

public:
    void SetHQ(CC_FSMScript* _hq);

    virtual void SetStateType(UINT _StateType) { m_StateType = _StateType; }
    virtual UINT GetStateType()	const { return m_StateType; }

public:
    CLONE(CC_StatesScript);

    CC_StatesScript();
    virtual ~CC_StatesScript();
};

