#pragma once
#include "ComponentUI.h"

class MeshRenderUI :
    public ComponentUI
{

public:
    virtual int render_update() override;


public:
    void SelectMeshData(DWORD_PTR _Key);

public:
    MeshRenderUI();
    ~MeshRenderUI();
};

