#pragma once
#include "CC_FSMScript.h"
#include "CP_StatesScript.h"

#define AnimIdleStand L"animclip\\P_IdleStand.animclip"
#define AnimIdleCrouch L"animclip\\P_IdleCrouch.animclip"
#define AnimMoveCrouch L"animclip\\P_MoveCrouch.animclip"
#define AnimMoveWalk L"animclip\\P_MoveWalk.animclip"




// 플레이어는 몬스터와 다르게 장비중인 무기에 따라 스텟이 달라짐
struct tPlayerAttInfo
{
    int a = 0;
};


class CP_FSMScript : public CC_FSMScript
{
public:
    enum class ePlayerStance
    {
        Normal,
        Sprint,
        Crouch,
        Aim,
        CrouchAim,
        Dodge,
        End,
    };
    enum class ePlayerMoveDir
    {
        LF, F, RF,
         L, N,  R, 
        LB, B, RB,

    };

    struct tPlayerStat
    {
        float MoveSpeed;
        tPlayerStat() : MoveSpeed(400.f) {}
    };
    struct tP_Info
    {
        tPlayerStat     P_Stat;
        tHealthStat     P_Health;   // Creature FSM 에 명시되어 있음
    };

private:
    tP_Info     m_tPlayerInfo;

    // 스탠스와 방향은 버퍼로 관리해야함
    ePlayerStance   P_Stance;
    ePlayerMoveDir  P_MoveDir;
    bool            m_InpCrouch;
    bool            m_InpAim;
    bool            m_InpSprint;
    bool            m_IsChangeStance;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void PlayAnimation(wstring _name, bool _repeat);


public:
    void ChangeMoveDir(ePlayerMoveDir _dir) { P_MoveDir = _dir; }
    ePlayerMoveDir  GetMoveDir() { return P_MoveDir; }
    void ChangeStance(ePlayerStance _stance) { P_Stance = _stance; }
    ePlayerStance GetStance() { return P_Stance; }
    tP_Info GetPlayerInfo() { return m_tPlayerInfo; }

public:
    void InputCrouch() { m_InpCrouch ? m_InpCrouch = false : m_InpCrouch = true; m_IsChangeStance = true; }
    void InputAim() { m_InpAim ? m_InpAim = false : m_InpAim = true; m_IsChangeStance = true; }
    void InputSprint() { m_InpSprint ? m_InpSprint = false : m_InpSprint = true; m_IsChangeStance = true; }
    bool IsCrouch() { return m_InpCrouch; }
    bool IsAim() { return m_InpAim; }
    bool IsSprint() { return m_InpSprint; }

public:
    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

public:
    CLONE(CP_FSMScript);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

public:
    CP_FSMScript();
    virtual ~CP_FSMScript();
};

