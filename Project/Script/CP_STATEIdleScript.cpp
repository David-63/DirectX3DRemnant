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

	// 입력 받기
	IdleMouseInput();
	
	if (KEY_TAP(KEY::RBTN))
	{
		m_PHQ->InputAim();
	}
	if (KEY_TAP(KEY::LCTRL))
	{
		m_PHQ->InputCrouch();
	}
	if (KEY_TAP(KEY::W))
	{
		m_PHQ->InputMove(0, 1.f);
	}
	if (KEY_TAP(KEY::S))
	{
		m_PHQ->InputMove(0, -1.f);
	}
	if (KEY_TAP(KEY::A))
	{
		m_PHQ->InputMove(-1.f, 0);
	}
	if (KEY_TAP(KEY::D))
	{
		m_PHQ->InputMove(1.f, 0);
	}
	if (KEY_RELEASE(KEY::W))
	{
		m_PHQ->InputMove(0, -1.f);
	}
	if (KEY_RELEASE(KEY::S))
	{
		m_PHQ->InputMove(0, 1.f);
	}
	if (KEY_RELEASE(KEY::D))
	{
		m_PHQ->InputMove(-1.f, 0);
	}
	if (KEY_RELEASE(KEY::A))
	{
		m_PHQ->InputMove(1.f, 0);
	}

	Vec2 moveDir = m_PHQ->GetMoveDir();

	if (Vec2(0, 0) != moveDir)
		m_isMove = true;

	/*if (KEY_TAP(KEY::SPACE))
	{
		m_isMove = true;
	}*/
	

	// 상태 변경해주기
	if (m_isMove)
	{
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::MOVE));
	}	
}

void CP_STATEIdleScript::CallAnimation()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();

	if (CP_FSMScript::ePlayerStance::CrouchAim == curStance)
	{
		m_PHQ->PlayAnimation(P_IdleR2AimCrouch, true);
	}
	else if (CP_FSMScript::ePlayerStance::Crouch == curStance)
	{
		m_PHQ->PlayAnimation(P_IdleR2Crouch, true);
	}
	else if (CP_FSMScript::ePlayerStance::Aim == curStance)
	{
		m_PHQ->PlayAnimation(P_IdleR2Aim, true);
	}
	else if (CP_FSMScript::ePlayerStance::Normal == curStance)
	{
		m_PHQ->PlayAnimation(P_IdleR2, true);
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
	// 애니메이션 재생
	CallAnimation();
}

void CP_STATEIdleScript::Exit()
{
	ClearIdleState();
}
