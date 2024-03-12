#pragma once
#include "CB_StateScript.h"

class CB_STATEMoveScript :
    public CB_StateScript
{
private:
    bool        m_IsMelee;
    bool        m_IsRanged;

public:
    virtual void tick() override;

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CB_STATEMoveScript);

    CB_STATEMoveScript();
    virtual ~CB_STATEMoveScript();


};

