#pragma once
#include "CB_StateScript.h"

class CB_STATEMoveScript :
    public CB_StateScript
{

public:
    virtual void begin() override;
    virtual void tick() override;
public:
    virtual void Enter() override;
    virtual void Exit() override;

private:
    bool        m_IsMelee;
    bool        m_IsRanged;
    bool        m_GetWeapon;
    bool        m_OnceEvade;
    
 
    int         m_DirSetCount;

    float       m_fPrevForce; // m_fCurForce ÀúÀå¿ë
    float       m_fCurForce;

public:
    void NoWeapon_MoveToDir();
    void Weapon_Move();
    void Weapon_StanceChange();
    void Weapon_StanceEvent();


    void Evade_MoveZero();
    void Evade_MoveRestore();

public:
    CLONE(CB_STATEMoveScript);

    CB_STATEMoveScript();
    virtual ~CB_STATEMoveScript();


};

