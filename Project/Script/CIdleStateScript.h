#pragma once
#include "CStateScript.h"


class CIdleStateScript :
    public CStateScript
{

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CIdleStateScript);

    CIdleStateScript();
    virtual ~CIdleStateScript();


};

