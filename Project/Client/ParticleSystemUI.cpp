#include "pch.h"
#include "ParticleSystemUI.h"

ParticleSystemUI::ParticleSystemUI() : ComponentUI("##ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)
{
    SetName("ParticleSystem");
}

ParticleSystemUI::~ParticleSystemUI()
{
}


int ParticleSystemUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    return TRUE;
}