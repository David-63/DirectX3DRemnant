#pragma once
#include "ResUI.h"
class PrefabUI :
    public ResUI
{
private:
    Vec3 mPosition;
    int mLayerIdx;
    char mName[32];

public:
    virtual int render_update() override;

public:
    PrefabUI();
    ~PrefabUI();
};

