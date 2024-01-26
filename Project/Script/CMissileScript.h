#pragma once
#include <Engine\CScript.h>

class CMissileScript :
    public CScript
{
private:
    Vec3            m_vDir;     // �̵� ����
    float           m_fSpeed;   // �̵� �ӷ�

    CGameObject*    m_TargetObject;

public:
    void SetSpeed(float _Speed) { m_fSpeed = _Speed; }

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMissileScript);
public:
    CMissileScript();
    ~CMissileScript();
};

