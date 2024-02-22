#pragma once
#include <Engine\CScript.h>

class CParticleScript:
    public CScript
{
private:
    Vec3 vOffsetPos;
    CGameObject* m_pTarget;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetTarget(CGameObject* _Target)
    {
        m_pTarget = _Target;
    }

    void SetOffsetPos(Vec3 _vOffsetPos)
    {
		vOffsetPos = _vOffsetPos;
	}

    Vec3 GetOffsetPos()
    {
       return vOffsetPos;
    }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
 
    CLONE(CParticleScript);

public:
    CParticleScript();
    ~CParticleScript();


};

