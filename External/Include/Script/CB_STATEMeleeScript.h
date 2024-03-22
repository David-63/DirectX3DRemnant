#pragma once
#include "CB_StateScript.h"

class CB_STATEMeleeScript :
    public CB_StateScript
{
private:
    //common
    Vec3    m_vAtkDir;
    bool    m_bAtkComplete;
    bool    m_bAnimOnce;
    float   m_fSpeed;

    //dash
    bool    m_bDashOnce;
    float   m_fDashDist;


public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    
public:
    CLONE(CB_STATEMeleeScript);
    CB_STATEMeleeScript();
    virtual ~CB_STATEMeleeScript();


};

