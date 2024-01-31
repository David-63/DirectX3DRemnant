#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecParentObj; // ���̾ �����ִ� �θ������Ʈ
    vector<CGameObject*>    m_vecObject;    // ������ ������� �ش� ���̾ �����ִ� ��� ������Ʈ
    int                     m_iLayerIdx;    // ���̾� ��ȣ (0~31)


public:    
    void begin();
    void tick();
    void finaltick(); 


public:
    int GetLayerIndex() { return m_iLayerIdx; }
    const vector<CGameObject*>& GetParentObject() { return m_vecParentObj; }
    const vector<CGameObject*>& GetObjects() { return m_vecObject; }


private:
    // Layer �� �θ������Ʈ �Է�
    // _bMove : �ڽ� ������Ʈ���� �θ� ������Ʈ�� ���� �̵� �� ������ üũ
    void AddGameObject(CGameObject* _Object, bool _bMove);
    void RegisterObject(CGameObject* _Object){ m_vecObject.push_back(_Object); }    
    void RemoveFromParentList(CGameObject* _Obj);
    void AddParentList(CGameObject* _Obj);

    CLONE(CLayer)
public:
    CLayer();
    ~CLayer();

    friend class CLevel;
    friend class CGameObject;
    friend class CEventMgr;
};

