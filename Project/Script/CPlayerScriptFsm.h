#pragma once
#include <Engine\CScript.h>
#include "CStateScript.h"

class CFSM;



class CPlayerScriptFsm :
    public CScript
{

public:
    struct tPlayerInfo
    {
        int HP;
        int MP;
    };

public:
    virtual void begin() override;
    virtual void tick() override;


    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;



private:
    std::map<CStateScript::eState, CStateScript*> m_State;
    CStateScript*                                 m_CurState;
    CStateScript::eState                          m_PrevState;
    tPlayerInfo                                   m_tPlayerInfo;

public:
    void AddState(CStateScript* _State);
    CStateScript* GetStateScript(CStateScript::eState _State);
    void ChangeState(CStateScript::eState _NextState);



public:
    CLONE(CPlayerScriptFsm);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

public:
    CPlayerScriptFsm();
    virtual ~CPlayerScriptFsm();
};

