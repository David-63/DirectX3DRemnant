#pragma once
#include "ComponentUI.h"

class ParticleSystemUI : public ComponentUI
{
public:
    virtual int render_update() override;

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

