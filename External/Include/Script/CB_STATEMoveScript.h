#pragma once
#include "CB_StateScript.h"

class CB_STATEMoveScript :
    public CB_StateScript
{

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

private:
    bool        m_IsMelee;
    bool        m_IsRanged;
    tTimeCtrl   m_MoveTime;
    bool        m_First;


public:
    CLONE(CB_STATEMoveScript);

    CB_STATEMoveScript();
    virtual ~CB_STATEMoveScript();


};

