#pragma once
#include "CP_StatesScript.h"

class CP_STATEIdleScript : public CP_StatesScript
{
private:
    bool        m_isMove;
    bool        m_isDodge;
    bool        m_isReload;
    bool        m_isFire;       // ���� �̵� �ִϸ��̼��� ������ �̷��� �Ҽ��ۿ� ����
    bool        m_isMelee;

    int         m_horizontal;
    int         m_vertical;

public:
    virtual void tick() override;


public:
    virtual void CallAnimation() override;


public:
    void IdleMouseInput();

public:
    void ClearIdleState() { m_isMove = false; m_isMelee = false; m_isReload = false; m_isDodge = false; }

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CP_STATEIdleScript);

    CP_STATEIdleScript();
    virtual ~CP_STATEIdleScript();
};

