#pragma once
#include "CC_StatesScript.h"

enum class eP_States
{
    IDLE,
    MOVE,
    DODGE,
    FIRE,
    RELOAD,
    MELEE,
    DAMAGED,
    DEAD,
    END,
};
enum class eInpStance
{
    Crouch,
    Aim,
    Sprint,
    Mouse,
    End,
};

enum class eStanceCheck
{
    IsChange,
    IsFrontDir,
    End,
};
enum class ePlayerStance
{
    Normal,
    Sprint,
    Crouch,
    Aim,
    Dodge,
    End,
};
struct tPlayerStat
{
    float MoveSpeed;
    tPlayerStat() : MoveSpeed(1500.f) {}
};
struct tP_Info
{
    tPlayerStat     P_Stat;
    tHealthStat     P_Health;   // Creature FSM 에 명시되어 있음
};
struct tP_LongGunInfo
{
    int CurAmmo;
    int MaxAmmo;
    float Damage;
    float FireLate;
    float ReloadSpeed;

    tP_LongGunInfo() : CurAmmo(12), MaxAmmo(12), Damage(12), FireLate(0.22f), ReloadSpeed(0.22f) {}

    bool IsAble() { return (0 < CurAmmo) ? true : false; }

    bool Fire()
    {
        if (0 >= CurAmmo)
            return false;

        CurAmmo--;
        return true;
    }

    bool Reload()
    {
        if (MaxAmmo <= CurAmmo)
            return false;
        CurAmmo++;
        return true;
    }
    bool ReloadMag()
    {
        if (MaxAmmo <= CurAmmo)
            return false;
        CurAmmo = MaxAmmo;
        return true;
    }
};
class CP_FSMScript;
class CP_StatesScript : public CC_StatesScript
{
protected:
    CP_FSMScript*   m_PHQ;
    tP_LongGunInfo* m_Gun;
    ePlayerStance*  m_PlayerStance;
    tP_Info*        m_PlayerInfo;
    Vec2*           m_PlayerMoveDir;

private:
    eP_States       m_PStateType;

public:
    virtual void begin() override;

public:
    virtual void CallAnimation() {}

public:
    virtual void SetStateType(UINT _StateType) override { m_PStateType = static_cast<eP_States>(_StateType); }
    virtual UINT GetStateType() const override { return static_cast<UINT>(m_PStateType); }

public:
    CLONE(CP_StatesScript);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
public:
    CP_StatesScript();
    virtual ~CP_StatesScript();
};

