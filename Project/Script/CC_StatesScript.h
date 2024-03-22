#pragma once
#include <Engine\CScript.h>

class CC_FSMScript;
class CC_StatesScript : public CScript
{
protected:
    CC_FSMScript*   m_FSMHQ;
private:
    UINT            m_StateType; // ��ǻ� ������

public:
    void SetHQ(CC_FSMScript* _hq);

    virtual void SetStateType(UINT _StateType) { m_StateType = _StateType; }
    virtual UINT GetStateType()	const { return m_StateType; }

    Vec3 GetPlayerPos();
    CGameObject* GetPlayer();
    Vec3 GetMyPos();
    void SetMyPos(Vec3 _pos);
    Vec3 GetMyFront();
    Vec3 GetToPlayerDir();
    float DistBetwPlayer();
    float CalTurnDegree(Vec3 _dir);

public:
    CLONE(CC_StatesScript);

    CC_StatesScript();
    virtual ~CC_StatesScript();
};
