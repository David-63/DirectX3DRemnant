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


	CP_FSMScript::tP_LongGunInfo* gun = m_PHQ->GetLongGunInfo();

	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	if (KEY_TAP(KEY::LBTN))
	{
		if (CP_FSMScript::ePlayerStance::CrouchAim == curStance
			|| CP_FSMScript::ePlayerStance::Aim == curStance)
		{
			if (gun->Fire())
			{
				if (CP_FSMScript::ePlayerStance::CrouchAim == curStance)
				{
					// TogleInput ȣ���ؼ� ������ �Ͼ�� �����					
				}
				//m_PHQ->PlayAnimation(P_2RFire, false);
			}
		}		
	}
	if (KEY_TAP(KEY::R))
	{
		if (gun->ReloadMag())
		{
			if (CP_FSMScript::ePlayerStance::CrouchAim == curStance
				|| CP_FSMScript::ePlayerStance::Crouch == curStance)
			{
				m_PHQ->PlayAnimation(P_ReloadRifleCrouch, false);

			}
			else
			{
				m_PHQ->PlayAnimation(P_ReloadRifle, false);
			}

			// Reload ���·� ����
			m_isReload = true;
		}		
	}
	if (KEY_TAP(KEY::SPACE))
	{

		m_isDodge = true;
	}
	// ���� �������ֱ�
	if (m_isDodge)
	{
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::MOVE));
	}
	else if (m_isReload)
	{
		//m_PHQ->ChangeState(static_cast<UINT>(eP_States::MOVE));
	}
	else if (m_isMove)
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

void CP_STATEIdleScript::Enter()
{
	CallAnimation();
}

void CP_STATEIdleScript::Exit()
{
	ClearIdleState();
	m_PHQ->OverrideObjRotY();
}
