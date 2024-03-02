#pragma once
#include "CRenderComponent.h"

// 목표
// 빈 게임오브젝트에서 메쉬 머티리얼 세팅 되도록 보안하기

class CMeshRender :
    public CRenderComponent
{
public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CMeshRender);
public:
    CMeshRender();
    ~CMeshRender();
};

