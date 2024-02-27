#pragma once
#include "ComponentUI.h"

class CGameObject;

class TransformUI :
    public ComponentUI
{
private:
    Vec3 vPos;
    Vec3 vScale;
    Vec3 vRot;
    char mName[32];


public:
    virtual int render_update() override;

public:
    TransformUI();
    ~TransformUI();
};

