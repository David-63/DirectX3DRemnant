#pragma once
#include "ComponentUI.h"
#include <Engine/CMesh.h>
#include <Engine/CAnimClip.h>

class Animator3DUI : public ComponentUI
{
private:
    Ptr<CMesh>      m_SelectMesh;
    Ptr<CAnimClip>  m_SelectAnimClip;

public:
    virtual int render_update() override;

public:
    void SelectMesh(DWORD_PTR _Key);
    void SelectAnimClip(DWORD_PTR _Key);

public:
    Animator3DUI();
    ~Animator3DUI();
};
