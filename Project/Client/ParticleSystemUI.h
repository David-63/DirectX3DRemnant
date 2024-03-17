#pragma once
#include "ComponentUI.h"

class CParticleSystem;

class ParticleSystemUI
    : public ComponentUI
{
private:
    CParticleSystem*  m_Particle;
    Ptr<CTexture>     m_ParticleTex;
    Ptr<CTexture>     m_NoiseTex;
    float             m_AccTime;

    float             m_fDebugTime;
    wstring           m_SelectMtrlName;

public:
    virtual int render_update() override;

public:
    void SelectMesh(DWORD_PTR _Key);
    void SelectMaterial(DWORD_PTR _Key);
    void SelectTexture(DWORD_PTR _Key);
    void SelectNoiseTex(DWORD_PTR _Key);



public:


public:
    ParticleSystemUI();
    virtual ~ParticleSystemUI();
};

