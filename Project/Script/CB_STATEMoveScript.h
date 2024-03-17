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
    bool        m_Phase2;
    tTimeCtrl   m_MoveTime;
 
    int         m_DirSetCount;

public:
    void MoveToDir();


public:
    CLONE(CB_STATEMoveScript);

    CB_STATEMoveScript();
    virtual ~CB_STATEMoveScript();


};

