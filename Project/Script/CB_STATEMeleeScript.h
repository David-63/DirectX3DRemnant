#pragma once
#include "CB_StateScript.h"

class CB_STATEMeleeScript :
    public CB_StateScript
{
private:
    //common

    CGameObject* m_AOE_Circle;
    bool    m_bSpawnCircleSpell;
    bool    m_bAttack;
    bool    m_bAtkComplete;
    bool    m_bAnimOnce;
    float   m_fSpeed;

    bool    m_bNearPlayer;


    //dash
    bool    m_bDashOnce;
    float   m_fDashDist;
    UINT    m_iMeleeAtkType;
    UINT    m_iAttackType;


public:
    virtual void begin() override;

    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;


public:
    UINT MeleeAttackRandom();
    UINT AttackTypeRandom();
    void DistCheck();

    void AttackBoxOn();
    void AttackBoxOn2();
    void AttackBoxOn3();
    void AttackBoxOn4();
    void AttackBoxOn5();
    void AttackBoxOn6();
    void AOE_AttackBoxOn();

    void AttackBoxOff();
    void AttackBoxOff2();
    void AttackBoxOff3();
    void AttackBoxOff4();
    void AttackBoxOff5();
    void AttackBoxOff6();
    void AOE_AttackBoxOff();

    void AttackEnd();
    void PlayerToRotate();
    void SpawnSpell();
    void SpawnSpellOff();





public:
    CLONE(CB_STATEMeleeScript);
    CB_STATEMeleeScript();
    virtual ~CB_STATEMeleeScript();


};

