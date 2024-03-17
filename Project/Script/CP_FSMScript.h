#pragma once
#include "CC_FSMScript.h"
#include "CP_StatesScript.h"
#include "CP_MouseCtrlScript.h"

#define P_IdleR2                    L"animclip\\player\\P_IdleR2.animclip"
#define P_IdleR2Aim                 L"animclip\\player\\P_IdleR2Aim.animclip"
#define P_IdleR2AimCrouch           L"animclip\\player\\P_IdleR2AimCrouch.animclip"
#define P_IdleR2Crouch              L"animclip\\player\\P_IdleR2Crouch.animclip"
#define P_MoveR2AimCrouchWalk       L"animclip\\player\\P_MoveR2AimCrouchWalk.animclip"
#define P_MoveR2AimCrouchWalk_B     L"animclip\\player\\P_MoveR2AimCrouchWalk_B.animclip"
#define P_MoveR2AimCrouchWalk_BL    L"animclip\\player\\P_MoveR2AimCrouchWalk_BL.animclip"
#define P_MoveR2AimCrouchWalk_BR    L"animclip\\player\\P_MoveR2AimCrouchWalk_BR.animclip"
#define P_MoveR2AimCrouchWalk_FL    L"animclip\\player\\P_MoveR2AimCrouchWalk_FL.animclip"
#define P_MoveR2AimCrouchWalk_FR    L"animclip\\player\\P_MoveR2AimCrouchWalk_FR.animclip"
#define P_MoveR2AimWalk             L"animclip\\player\\P_MoveR2AimWalk.animclip"
#define P_MoveR2AimWalk_B           L"animclip\\player\\P_MoveR2AimWalk_B.animclip"
#define P_MoveR2AimWalk_BL          L"animclip\\player\\P_MoveR2AimWalk_BL.animclip"
#define P_MoveR2AimWalk_BR          L"animclip\\player\\P_MoveR2AimWalk_BR.animclip"
#define P_MoveR2AimWalk_FL          L"animclip\\player\\P_MoveR2AimWalk_FL.animclip"
#define P_MoveR2AimWalk_FR          L"animclip\\player\\P_MoveR2AimWalk_FR.animclip"
#define P_MoveR2CrouchWalk          L"animclip\\player\\P_MoveR2CrouchWalk.animclip"
#define P_MoveR2CrouchWalk_B        L"animclip\\player\\P_MoveR2CrouchWalk_B.animclip"
#define P_MoveR2CrouchWalk_BL       L"animclip\\player\\P_MoveR2CrouchWalk_BL.animclip"
#define P_MoveR2CrouchWalk_BR       L"animclip\\player\\P_MoveR2CrouchWalk_BR.animclip"
#define P_MoveR2CrouchWalk_FL       L"animclip\\player\\P_MoveR2CrouchWalk_FL.animclip"
#define P_MoveR2CrouchWalk_FR       L"animclip\\player\\P_MoveR2CrouchWalk_FR.animclip"
#define P_MoveR2Jog                 L"animclip\\player\\P_MoveR2Jog.animclip"
#define P_MoveR2Jog_B               L"animclip\\player\\P_MoveR2Jog_B.animclip"
#define P_MoveR2Jog_BL              L"animclip\\player\\P_MoveR2Jog_BL.animclip"
#define P_MoveR2Jog_BR              L"animclip\\player\\P_MoveR2Jog_BR.animclip"
#define P_MoveR2Jog_FL              L"animclip\\player\\P_MoveR2Jog_FL.animclip"
#define P_MoveR2Jog_FR              L"animclip\\player\\P_MoveR2Jog_FR.animclip"


#define StanceDelay 0.1f

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
        tPlayerStat() : MoveSpeed(1500.f) {}
    };
    struct tP_Info
    {
        tPlayerStat     P_Stat;
        tHealthStat     P_Health;   // Creature FSM 에 명시되어 있음
    };

private:
    tP_Info             m_tPlayerInfo;

    ePlayerStance       P_Stance;
    tTimeCtrl           m_StanceDelay;

    // 토글 입력
    bool                m_ableMouse;
    bool                m_InpCrouch;
    bool                m_InpAim;
    bool                m_InpSprint;
    bool                m_IsChangeStance;

    // 방향
    Vec2                m_moveDir;

    // 카메라
    CP_MouseCtrlScript  m_MouseCtrl;
public:
    Vec2                m_MouseAxisInput;


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

    void AbleMouse() { m_ableMouse ? m_ableMouse = false : m_ableMouse = true; }
    bool IsAbleMouse() { return m_ableMouse; }


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

