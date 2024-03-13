#pragma once
#include "CC_FSMScript.h"
#include "CP_StatesScript.h"

#define P_AnimIdleStand L"animclip\\player\\P_IdleStand.animclip"
#define P_AnimIdleCrouch L"animclip\\player\\P_IdleCrouch.animclip"
#define P_AnimMoveCrouch L"animclip\\player\\P_MoveCrouch.animclip"
#define P_AnimMoveCrouchB L"animclip\\player\\P_MoveCrouchB.animclip"
#define P_AnimMoveCrouchBL L"animclip\\player\\P_MoveCrouchBL.animclip"
#define P_AnimMoveCrouchBR L"animclip\\player\\P_MoveCrouchBR.animclip"
#define P_AnimMoveCrouchFL L"animclip\\player\\P_MoveCrouchFL.animclip"
#define P_AnimMoveCrouchFR L"animclip\\player\\P_MoveCrouchFR.animclip"
#define P_AnimMoveWalk L"animclip\\player\\P_MoveWalk.animclip"
#define P_AnimMoveWalkB L"animclip\\player\\P_MoveWalkB.animclip"
#define P_AnimMoveWalkBL L"animclip\\player\\P_MoveWalkBL.animclip"
#define P_AnimMoveWalkBR L"animclip\\player\\P_MoveWalkBR.animclip"
#define P_AnimMoveWalkFL L"animclip\\player\\P_MoveWalkFL.animclip"
#define P_AnimMoveWalkFR L"animclip\\player\\P_MoveWalkFR.animclip"

// 플레이어는 몬스터와 다르게 장비중인 무기에 따라 스텟이 달라짐

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

    // 토글 입력
    bool            m_InpCrouch;
    bool            m_InpAim;
    bool            m_InpSprint;
    bool            m_IsChangeStance;

    // 방향
    Vec2        m_moveDir;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void PlayAnimation(wstring _name, bool _repeat);


public:
    void ChangeStance(ePlayerStance _stance) { P_Stance = _stance; }
    ePlayerStance GetStance() { return P_Stance; }
    tP_Info GetPlayerInfo() { return m_tPlayerInfo; }

public:
    void ClearStanceChange() { m_IsChangeStance = false; }
    void InputCrouch() { m_InpCrouch ? m_InpCrouch = false : m_InpCrouch = true; m_IsChangeStance = true; }
    void InputAim() { m_InpAim ? m_InpAim = false : m_InpAim = true; m_IsChangeStance = true; }
    void InputSprint() { m_InpSprint ? m_InpSprint = false : m_InpSprint = true; m_IsChangeStance = true; }
    bool IsCrouch() { return m_InpCrouch; }
    bool IsAim() { return m_InpAim; }
    bool IsSprint() { return m_InpSprint; }

    void InputMove(Vec2 _input) { m_moveDir += _input; }
    void InputMove(int _inputX, int _inputY) { m_moveDir += Vec2(_inputX, _inputY); }
    void ClearMoveDir() { m_moveDir = Vec2(0, 0); }
    Vec2 GetMoveDir() { return m_moveDir; }

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

