#pragma once
#include "CB_StateScript.h"

class CB_STATEIdleScript :
    public CB_StateScript
{
private:
    bool        m_isMove;
    bool        m_isMelee;


    int         m_horizontal;
    int         m_vertical;


public:
    virtual void begin() override;
public:
    virtual void Enter() override; 
    virtual void tick() override;
    virtual void Exit() override;

public:
    CLONE(CB_STATEIdleScript);
     
    CB_STATEIdleScript();
    virtual ~CB_STATEIdleScript();


};

