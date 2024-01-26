#pragma once
#include "ComponentUI.h"

class Light3DUI : public ComponentUI
{
public:
    virtual int render_update() override;

public:
    Light3DUI();
    ~Light3DUI();
};

