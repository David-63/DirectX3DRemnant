#pragma once
#include <Engine\CScript.h>

class CHitBoxScript :
    public CScript
{
private:
    vector<CGameObject*>    m_vGameObjects;
    float                   m_fCamSpeed;
    int                     m_iCurIdx;

public:
    virtual void begin() override;
    virtual void tick() override;

   

public:
    void MakeHitBox(bool _isPlayer, Vec3 _size, Vec3 _posOffset);
    void SetSleep(int _idx);
    void SetAwake(int _idx);

public:
    virtual void BeginOverlap(CCollider3D* _Other);
    virtual void OnOverlap(CCollider3D* _Other);
    virtual void EndOverlap(CCollider3D* _Other);

private:

    CLONE(CHitBoxScript);
public:
    CHitBoxScript();
    ~CHitBoxScript();
};

