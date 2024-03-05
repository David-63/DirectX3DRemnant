#pragma once
#include "CEntity.h"

class CGameObject;
class CState;

class CFSM :
    public CEntity
{

private:
    map<wstring, CState*> m_mapState;
    CGameObject* m_pObject;
    CState* m_pCurState;

public:
    void tick();
    void finaltick();

public:
    void SetObject(CGameObject* _pObject) { m_pObject = _pObject; }
    CGameObject* GetObject() { return m_pObject; }

    void AddState(wstring _strStateName, CState* _pState);
    CState* FindState(wstring _strStateName);
    void ChangeState(wstring _strNextStateName, const wstring& _AniName, bool _Stay);


    CLONE(CFSM);

public:
    CFSM();
    ~CFSM();
};

