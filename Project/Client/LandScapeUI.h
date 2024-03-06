#pragma once
#include "ComponentUI.h"

class LandScapeUI :
    public ComponentUI
{
public:
    virtual int render_update() override;

    void SelectFaceMesh(DWORD_PTR _Key);
    void SelectHeightMapTex(DWORD_PTR _Key);
public:
    LandScapeUI();
    ~LandScapeUI();
};

