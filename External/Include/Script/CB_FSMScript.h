#pragma once
#include "CC_FSMScript.h"
#include "CB_StateScript.h"

class CB_FSMScript :
    public CC_FSMScript
{
    enum class eBossStance
    {
        Normal,
        Crouch,
        Aim,
        CrouchAim,
        Dodge,
        End,
    };
    enum class eBossMoveDir
    {
        LF, F, RF,
        L, N, R,
        LB, B, RB,

    };

    struct tBossStat
    {
        float MoveSpeed;

    };
    struct tP_Info
    {
        tBossStat     P_Stat;

        tHealthStat     P_Health;   // Creature FSM 에 명시되어 있음
    };

private:
    tP_Info         m_tPlayerInfo;
    eBossMoveDir    P_MoveDir;

public:
    virtual void begin() override;
    virtual void tick() override;


public:
    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

public:
    CLONE(CB_FSMScript);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

public:
    CB_FSMScript();
    virtual ~CB_FSMScript();





};

