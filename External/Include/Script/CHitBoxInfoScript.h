#pragma once
#include <Engine\CScript.h>

class CHitBoxInfoScript :
    public CScript
{
private:
    tHitInfo m_HitInfo;

public:
    virtual void begin() override;
    virtual void tick() override;


public:
    void SetHitInfo(tHitInfo _info) { m_HitInfo = _info; }

public:
    virtual void BeginOverlap(CCollider3D* _Other);
    virtual void OnOverlap(CCollider3D* _Other);
    virtual void EndOverlap(CCollider3D* _Other);

private:
    CLONE(CHitBoxInfoScript);

public:
    CHitBoxInfoScript();
    ~CHitBoxInfoScript();
};

