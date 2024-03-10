#pragma once
#include "CC_FSMScript.h"
#include "CP_StatesScript.h"

// �÷��̾�� ���Ϳ� �ٸ��� ������� ���⿡ ���� ������ �޶���
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

    };
    struct tP_Info
    {
        tPlayerStat     P_Stat;

        tHealthStat     P_Health;   // Creature FSM �� ��õǾ� ����
    };

private:
    tP_Info     m_tPlayerInfo;

    // ���Ľ��� ������ ���۷� �����ؾ���
    ePlayerStance   P_Stance;
    ePlayerMoveDir  P_MoveDir;

    bool            m_InpCrouch;
    bool            m_InpAim;
    bool            m_InpSprint;



public:
    virtual void begin() override;
    virtual void tick() override;


public:
    void ChangeMoveDir(ePlayerMoveDir _dir) { P_MoveDir = _dir; }
    ePlayerMoveDir  GetMoveDir() { return P_MoveDir; }
    void ChangeStance(ePlayerStance _stance) { P_Stance = _stance; }
    ePlayerStance GetStance() { return P_Stance; }

public:
    void InputCrouch() { m_InpCrouch ? false : true; }
    void InputAim() { m_InpAim ? false : true; }
    void InputSprint() { m_InpSprint ? false : true; }
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

