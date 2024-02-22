#pragma once
#include "Engine\CScript.h"

class CTestScript :
    public CScript
{
private:
    Vec3 pivot;
    Vec3 mDir;
    bool mWallCol;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

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

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override; 
    CLONE(CTestScript);

public:
    CTestScript();
    ~CTestScript();
};

