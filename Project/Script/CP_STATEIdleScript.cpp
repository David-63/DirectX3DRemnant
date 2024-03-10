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
	
	

	Input();

	// 상태 변경해주기
	if (m_isMove)
	{
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::MOVE));
	}	
}

void CP_STATEIdleScript::Input()
{
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
		m_isMove = true;
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::F);
	}
	if (KEY_TAP(KEY::S))
	{
		m_isMove = true;
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::B);
	}
	if (KEY_TAP(KEY::A))
	{
		m_isMove = true;
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::L);
	}
	if (KEY_TAP(KEY::D))
	{
		m_isMove = true;
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::R);
	}
	if (KEY_TAP(KEY::SPACE))
	{
		m_isMove = true;
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::N);
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

void CP_STATEIdleScript::IdleNormalInput()
{
	// 스텐스 변경
	
	// Move Input
	Input();
}

void CP_STATEIdleScript::IdleAimInput()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	
	if (KEY_TAP(KEY::LBTN))
	{
		// 사격
	}

	if (KEY_RELEASE(KEY::RBTN))
	{
		if (CP_FSMScript::ePlayerStance::CrouchAim == curStance)
			m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Crouch);
		else
			m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Normal);
	}
	
	if (KEY_TAP(KEY::LCTRL))
	{
		m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::CrouchAim);
	}
	Input();
}

void CP_STATEIdleScript::IdleCrouchInput()
{	
}

void CP_STATEIdleScript::IdleCrouchAimInput()
{
	if (KEY_TAP(KEY::LBTN))
	{
		// 사격
	}

	if (KEY_RELEASE(KEY::RBTN))
	{
		m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Crouch);
	}
	if (KEY_RELEASE(KEY::LCTRL))
	{
		m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Aim);
	}
	Input();
}

void CP_STATEIdleScript::Enter()
{
	// 애니메이션 재생
}

void CP_STATEIdleScript::Exit()
{
	ClearIdleState();
}
