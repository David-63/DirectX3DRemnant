#pragma once
#include "ComponentUI.h"

class LandScapeUI :
    public ComponentUI
{
public:
    virtual int render_update() override;

public:
    LandScapeUI();
    ~LandScapeUI();
};

