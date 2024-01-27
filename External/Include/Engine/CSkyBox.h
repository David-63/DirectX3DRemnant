#pragma once

#include "CRenderComponent.h"

enum class SKYBOX_TYPE
{
    SPHERE,
    CUBE,
    End,
};

class CSkyBox :
    public CRenderComponent
{
private:
    SKYBOX_TYPE     m_Type;
    Ptr<CTexture>   m_SkyBoxTex;


public:
    SKYBOX_TYPE GetSkyBoxType() { return m_Type; }
    void SetSkyBoxType(SKYBOX_TYPE _Type);
    Ptr<CTexture> GetSkyBoxTexture() { return m_SkyBoxTex; }
    void SetSkyBoxTexture(Ptr<CTexture> _Tex);


public:
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CSkyBox);
public:
    CSkyBox();
    ~CSkyBox();
};

