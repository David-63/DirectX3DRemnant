#pragma once
#include "ResUI.h"
class PrefabUI :
    public ResUI
{
private:
    Vec3 mPosition;
    int mLayerIdx;
    char mName[32];

    CGameObject* obj;

public:
    virtual int render_update() override;

public:
    PrefabUI();
    ~PrefabUI();
};

