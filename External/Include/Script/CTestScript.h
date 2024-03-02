#pragma once
#include "Engine\CScript.h"

class CTestScript :
    public CScript
{
private:
    Vec3 pivot;
    Vec3 vMoveVector;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

public:
   

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override; 
    CLONE(CTestScript);

public:
    CTestScript();
    ~CTestScript();
};

