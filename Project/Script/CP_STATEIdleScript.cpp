#include "pch.h"
#include "CP_STATEIdleScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CEngine.h>
#include "CP_FSMScript.h"

CP_STATEIdleScript::CP_STATEIdleScript() : m_isMove(false), m_isMelee(false), m_horizontal(0), m_vertical(0)
{
	SetStateType(static_cast<UINT>(eP_States::IDLE));
}

CP_STATEIdleScript::~CP_STATEIdleScript()
{
}

void CP_STATEIdleScript::tick()
{
	if (KEY_TAP(KEY::RBTN))
	{
		if (m_PHQ->IsInput((UINT)eInpStance::Crouch))
			m_PHQ->InputCrouch();
		
		m_PHQ->InputAim();
	}
	if (KEY_TAP(KEY::LCTRL))
	{
		if (m_PHQ->IsInput((UINT)eInpStance::Aim))
			m_PHQ->InputAim();

		m_PHQ->InputCrouch();
	}
	

	Vec2 moveDir = m_PHQ->GetMoveDir();

	if (Vec2(0, 0) != moveDir)
		m_isMove = true;


	CP_FSMScript::tP_LongGunInfo* gun = m_PHQ->GetLongGunInfo();
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	if (CP_FSMScript::ePlayerStance::Aim == curStance)
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			if (gun->Fire())
			{
				m_isFire = true;
				m_PHQ->PlayAnimation(P_R2Fire, false);
			}
		}
	}
	
	if (KEY_TAP(KEY::R))
	{
		if (gun->ReloadMag())
		{
			if (CP_FSMScript::ePlayerStance::Crouch == curStance)
			{
				m_PHQ->PlayAnimation(P_R2ReloadCrouch, false);
			}
			else
			{
				m_PHQ->PlayAnimation(P_R2Reload, false);
			}

			// Reload 상태로 변경
			m_isReload = true;
		}		
	}
	if (KEY_TAP(KEY::SPACE))
	{
		if (m_PHQ->IsInput((UINT)eInpStance::Crouch))
			m_PHQ->InputCrouch();
		if (m_PHQ->IsInput((UINT)eInpStance::Aim))
			m_PHQ->InputAim();
		m_PHQ->PlayAnimation(P_R2Dodge_N, false);
		m_isDodge = true;
	}
	
	// 상태 변경해주기
	if (m_isDodge)
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::DODGE));
	else if (m_isFire)
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::FIRE));
	else if (m_isReload)
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::RELOAD));
	else if (m_isMove)
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::MOVE));
}

void CP_STATEIdleScript::CallAnimation()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();

	if (CP_FSMScript::ePlayerStance::Crouch == curStance)
	{
		m_PHQ->PlayAnimation(P_R2IdleCrouch, true);
	}
	else if (CP_FSMScript::ePlayerStance::Aim == curStance)
	{
		m_PHQ->PlayAnimation(P_R2IdleAim, true);
	}
	else if (CP_FSMScript::ePlayerStance::Normal == curStance)
	{
		m_PHQ->PlayAnimation(P_R2Idle, true);
	}
}

void CP_STATEIdleScript::IdleMouseInput()
{
	// 0. 마우스 중앙으로 옮기기
	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();

	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	//SetCursorPos(centerX, centerY);


	// 1. 마우스 입력값 받기
	POINT mousePos;
	GetCursorPos(&mousePos);

	m_horizontal = mousePos.x - centerX;
	m_vertical = mousePos.y - centerY;
}

void CP_STATEIdleScript::Enter()
{
	CallAnimation();
}

void CP_STATEIdleScript::Exit()
{
	ClearIdleState();
}
