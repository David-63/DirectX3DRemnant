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
	// �Է� �ޱ�
	IdleMouseInput();
	
	// ���Ľ��� ���缭 ���� �����ϱ�
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	switch (curStance)
	{
	case CP_FSMScript::ePlayerStance::Normal:
		IdleNormalInput();
		break;
	case CP_FSMScript::ePlayerStance::Crouch:
		IdleCrouchInput();
		break;
	case CP_FSMScript::ePlayerStance::Aim:
		IdleAimInput();
		break;
	case CP_FSMScript::ePlayerStance::CrouchAim:
		IdleCrouchAimInput();
		break;
	}

	// ���� �������ֱ�
	if (m_isMove)
	{
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::MOVE));
	}
	
	if (m_isMelee)
	{
		//m_PHQ->ChangeState(static_cast<UINT>(eP_States::MELEE));
	}
}

void CP_STATEIdleScript::MoveInput()
{
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
	// 0. ���콺 �߾����� �ű��
	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();

	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	//SetCursorPos(centerX, centerY);


	// 1. ���콺 �Է°� �ޱ�
	POINT mousePos;
	GetCursorPos(&mousePos);

	m_horizontal = mousePos.x - centerX;
	m_vertical = mousePos.y - centerY;
}

void CP_STATEIdleScript::IdleNormalInput()
{
	// ���ٽ� ����
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	if (KEY_TAP(KEY::LBTN))
	{
		m_isMelee = true;
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::N);
	}

	if (KEY_TAP(KEY::RBTN))
	{
		if (CP_FSMScript::ePlayerStance::Crouch == curStance)
			m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::CrouchAim);	
		else
			m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Aim);
	}
	if (KEY_TAP(KEY::LCTRL))
	{
		if (CP_FSMScript::ePlayerStance::Aim == curStance)
			m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::CrouchAim);
		else
			m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Crouch);
	}

	// Move Input
	MoveInput();
}

void CP_STATEIdleScript::IdleAimInput()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	
	if (KEY_TAP(KEY::LBTN))
	{
		// ���
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
	MoveInput();
}

void CP_STATEIdleScript::IdleCrouchInput()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	if (KEY_TAP(KEY::LBTN))
	{
		m_isMelee = true;
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::N);
	}

	if (KEY_RELEASE(KEY::LCTRL))
	{
		if (CP_FSMScript::ePlayerStance::CrouchAim == curStance)
			m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Aim);
		else
			m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Normal);
	}

	if (KEY_TAP(KEY::RBTN))
	{
		m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::CrouchAim);
	}
	MoveInput();
}

void CP_STATEIdleScript::IdleCrouchAimInput()
{
	if (KEY_TAP(KEY::LBTN))
	{
		// ���
	}

	if (KEY_RELEASE(KEY::RBTN))
	{
		m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Crouch);
	}
	if (KEY_RELEASE(KEY::LCTRL))
	{
		m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Aim);
	}
	MoveInput();
}

void CP_STATEIdleScript::Enter()
{
	// �ִϸ��̼� ���
}

void CP_STATEIdleScript::Exit()
{
	ClearIdleState();
}
