#pragma once
#include "ResUI.h"
class MeshDataUI :
    public ResUI
{
private:
    char mName[32];

public:
    virtual int render_update() override;

public:
    MeshDataUI();
    ~MeshDataUI();
};

