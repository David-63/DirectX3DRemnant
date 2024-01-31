#pragma once
#include "ResUI.h"

class AnimationUI : public ResUI
{

public:
    virtual int render_update() override;

public:
    AnimationUI();
    ~AnimationUI();
};

