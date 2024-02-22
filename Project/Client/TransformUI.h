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
public:
    virtual int render_update() override;

public:
    TransformUI();
    ~TransformUI();
};

