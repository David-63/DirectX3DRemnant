
#pragma once
#include "ResUI.h"

class AnimClipUI :
    public ResUI
{

public:
    virtual int render_update() override;

public:
    AnimClipUI();
    ~AnimClipUI();
};
