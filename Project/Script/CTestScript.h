#pragma once
#include "Engine\CScript.h"

class CTestScript :
    public CScript
{
private:
    Vec3 pivot;
    CGameObject* m_pTarget;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void Start()
    {
        int a = 0;
    }
    void Complete()
    {
        int a = 0;
    }
    
    void End()
    {
        int a = 0;
    }
    void Action()
    {
        int a = 0;
    }


    void SetTarget(CGameObject* _Target)
    {
		m_pTarget = _Target;
	}

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override; 
    CLONE(CTestScript);

public:
    CTestScript();
    ~CTestScript();
};

