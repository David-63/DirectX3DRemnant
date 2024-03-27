#pragma once
#include "CC_FSMScript.h"
#include "CP_StatesScript.h"
#include "CP_MouseCtrlScript.h"

#define P_R2Dodge                       L"animclip\\player\\P_R2Dodge.animclip"
#define P_R2Dodge_B                     L"animclip\\player\\P_R2Dodge_B.animclip"   // 이건 안쓰는게 맞을듯
#define P_R2Dodge_L                     L"animclip\\player\\P_R2Dodge_L.animclip"   
#define P_R2Dodge_N                     L"animclip\\player\\P_R2Dodge_N.animclip"
#define P_R2Dodge_R                     L"animclip\\player\\P_R2Dodge_R.animclip"
#define P_R2Reload                      L"animclip\\player\\P_R2Reload.animclip"
#define P_R2ReloadCrouch                L"animclip\\player\\P_R2ReloadCrouch.animclip"
#define P_R2Idle                        L"animclip\\player\\P_R2Idle.animclip"
#define P_R2IdleAim                     L"animclip\\player\\P_R2IdleAim.animclip"
#define P_R2IdleCrouch                  L"animclip\\player\\P_R2IdleCrouch.animclip"
#define P_R2MoveWalk                    L"animclip\\player\\P_R2MoveWalk.animclip"
#define P_R2MoveWalk_B                  L"animclip\\player\\P_R2MoveWalk_B.animclip"
#define P_R2MoveWalk_BL                 L"animclip\\player\\P_R2MoveWalk_BL.animclip"
#define P_R2MoveWalk_BR                 L"animclip\\player\\P_R2MoveWalk_BR.animclip"
#define P_R2MoveWalk_FL                 L"animclip\\player\\P_R2MoveWalk_FL.animclip"
#define P_R2MoveWalk_FR                 L"animclip\\player\\P_R2MoveWalk_FR.animclip"
#define P_R2MoveWalkAim                 L"animclip\\player\\P_R2MoveWalkAim.animclip"
#define P_R2MoveWalkAim_B               L"animclip\\player\\P_R2MoveWalkAim_B.animclip"
#define P_R2MoveWalkAim_BL              L"animclip\\player\\P_R2MoveWalkAim_BL.animclip"
#define P_R2MoveWalkAim_BR              L"animclip\\player\\P_R2MoveWalkAim_BR.animclip"
#define P_R2MoveWalkAim_FL              L"animclip\\player\\P_R2MoveWalkAim_FL.animclip"
#define P_R2MoveWalkAim_FR              L"animclip\\player\\P_R2MoveWalkAim_FR.animclip"
#define P_R2MoveWalkCrouch              L"animclip\\player\\P_R2MoveWalkCrouch.animclip"
#define P_R2MoveWalkCrouch_B            L"animclip\\player\\P_R2MoveWalkCrouch_B.animclip"
#define P_R2MoveWalkCrouch_BL           L"animclip\\player\\P_R2MoveWalkCrouch_BL.animclip"
#define P_R2MoveWalkCrouch_BR           L"animclip\\player\\P_R2MoveWalkCrouch_BR.animclip"
#define P_R2MoveWalkCrouch_FL           L"animclip\\player\\P_R2MoveWalkCrouch_FL.animclip"
#define P_R2MoveWalkCrouch_FR           L"animclip\\player\\P_R2MoveWalkCrouch_FR.animclip"
#define P_R2MoveSprint                  L"animclip\\player\\P_R2MoveSprint.animclip"
#define P_R2MoveSprint_L                L"animclip\\player\\P_R2MoveSprint_L.animclip"
#define P_R2MoveSprint_R                L"animclip\\player\\P_R2MoveSprint_R.animclip"

#define P_R2Fire                        L"animclip\\player\\P_R2Fire.animclip"

#define StanceDelay 0.1f

// 플레이어는 몬스터와 다르게 장비중인 무기에 따라 스텟이 달라짐
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

class CP_FSMScript : public CC_FSMScript
{
public:
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


private:
    tP_Info             m_PlayerInfo;
    tP_LongGunInfo      m_LongGunInfo;

private:
    CP_MouseCtrlScript  m_MouseCtrl;
    CGameObject*        m_Weapon;
    CGameObject*        m_LongGun;

    ePlayerStance       P_Stance;
    tTimeCtrl           m_StanceDelay;
    bool                m_TogleInput[(UINT)eInpStance::End];
    bool                m_StanceCheck[(UINT)eStanceCheck::End];
    Vec2                m_moveDir;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void stanceControl();
    void dirInput();
    void stanceInput();

public:
    void PlayAnimation(wstring _name, bool _repeat);
    void OverrideObjRotY() { m_MouseCtrl.OverrideObjRotY(); }
    void DoDodge();


public:
    void ChangeStance(ePlayerStance _stance) { P_Stance = _stance; }
    ePlayerStance GetStance() { return P_Stance; }
    tP_Info GetPlayerInfo() { return m_PlayerInfo; }
    tP_LongGunInfo* GetLongGunInfo() { return &m_LongGunInfo; }
    CP_MouseCtrlScript* GetMouseController() { return &m_MouseCtrl; }
public:
    void ClearStanceChange() { m_StanceCheck[(UINT)eStanceCheck::IsChange] = false; }
    void InputCrouch() { m_TogleInput[(UINT)eInpStance::Crouch] ? m_TogleInput[(UINT)eInpStance::Crouch] = false : m_TogleInput[(UINT)eInpStance::Crouch] = true; m_StanceCheck[(UINT)eStanceCheck::IsChange] = true; }
    void InputAim() { m_TogleInput[(UINT)eInpStance::Aim] ? m_TogleInput[(UINT)eInpStance::Aim] = false : m_TogleInput[(UINT)eInpStance::Aim] = true; m_StanceCheck[(UINT)eStanceCheck::IsChange] = true; }
    bool IsInput(UINT _stance) { return m_TogleInput[_stance]; }
    bool IsFrontDir() { return m_StanceCheck[m_StanceCheck[(UINT)eStanceCheck::IsFrontDir]]; }

    
    void InputSprint(bool _isHold) { m_TogleInput[(UINT)eInpStance::Sprint] = _isHold;  m_StanceCheck[(UINT)eStanceCheck::IsChange] = true; }
    bool IsSprint() { return m_TogleInput[(UINT)eInpStance::Sprint]; }

    void AbleMouse() { m_TogleInput[(UINT)eInpStance::Mouse] ? m_TogleInput[(UINT)eInpStance::Mouse] = false : m_TogleInput[(UINT)eInpStance::Mouse] = true; }
    bool IsAbleMouse() { return m_TogleInput[(UINT)eInpStance::Mouse]; }


public:
    void InputMove(Vec2 _input) { m_moveDir += _input; }
    void InputMove(int _inputX, int _inputY) { m_moveDir += Vec2(_inputX, _inputY); }
    void ClearMoveDir() { m_moveDir = Vec2(0, 0); }
    Vec2 GetMoveDir() { return m_moveDir; }

public:
    void GotoIdle();
    void GotoMove();

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

