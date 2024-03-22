#include "pch.h"
#include "CP_STATEMoveScript.h"
#include <Engine/CTransform.h>
#include "CP_FSMScript.h"

CP_STATEMoveScript::CP_STATEMoveScript() : m_isMelee(false), m_isFront(true)
{
	SetStateType(static_cast<UINT>(eP_States::MOVE));
}

CP_STATEMoveScript::~CP_STATEMoveScript()
{
}

void CP_STATEMoveScript::tick()
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

	if (KEY_TAP(KEY::LSHIFT))
		m_PHQ->InputSprint(true);
	if (KEY_RELEASE(KEY::LSHIFT))
		m_PHQ->InputSprint(false);
	
	

	Vec2 moveDir = m_PHQ->GetMoveDir();
	// 앞 뒤 구분
	if (0 <= moveDir.y)
		m_isFront = true;
	else
		m_isFront = false;

	if (moveDir != m_prevDir)
	{
		m_prevDir = moveDir;
		CallAnimation();
	}
	// 이동
	translateInput();


	if (KEY_TAP(KEY::SPACE))
	{
		if (m_PHQ->IsInput((UINT)eInpStance::Crouch))
			m_PHQ->InputCrouch();
		if (m_PHQ->IsInput((UINT)eInpStance::Aim))
			m_PHQ->InputAim();
		m_PHQ->InputSprint(false);
		m_isDodge = true;
		CallAnimation();
	}

	// 애니메이션을 변경해주려면 이동 방향이 바뀐걸 확인 할 수 있어야함

	CP_FSMScript::tP_LongGunInfo* gun = m_PHQ->GetLongGunInfo();
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
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
			m_PHQ->ChangeState(static_cast<UINT>(eP_States::RELOAD));
		}
	}
	if (!m_PHQ->IsSprint())
	{
		if (CP_FSMScript::ePlayerStance::Aim == curStance)
		{
			CP_FSMScript::tP_LongGunInfo* gun = m_PHQ->GetLongGunInfo();
			if (KEY_HOLD(KEY::LBTN))
			{
				if (gun->Fire())
				{
					m_PHQ->PlayAnimation(P_R2Fire, false);
					m_PHQ->ChangeState(static_cast<UINT>(eP_States::FIRE));
				}
			}
		}
	}

	// 이동량이 없으면 Idle로 변경
	if ((0.1 >= moveDir.y && -0.1 <= moveDir.y)
		&& (0.1 >= moveDir.x && -0.1 <= moveDir.x))
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::IDLE));

	// 스테이트 변경 : MELEE
	// 스테이트 변경 : DAMAGED
}

void CP_STATEMoveScript::translateInput()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();

	Vec3 vCurPos = m_PHQ->GetOwner()->Transform()->GetRelativePos();

	Vec3 vFront = m_PHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = m_PHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = m_PHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	// 이동량 구하기

	CP_FSMScript::tP_Info playerInfo = m_PHQ->GetPlayerInfo();

	float moveMagnitude = 0.f;
	if (CP_FSMScript::ePlayerStance::Crouch == curStance)
		moveMagnitude = playerInfo.P_Stat.MoveSpeed * ScaleDT * 0.1f;
	else if (CP_FSMScript::ePlayerStance::Sprint == curStance && m_isFront)
		moveMagnitude = playerInfo.P_Stat.MoveSpeed * ScaleDT * 2.8f;
	else
		moveMagnitude = playerInfo.P_Stat.MoveSpeed * ScaleDT;


	if (KEY_HOLD(KEY::W))
	{
		vCurPos -= vFront * moveMagnitude;
	}
	if (KEY_HOLD(KEY::S))
	{
		vCurPos += vFront * moveMagnitude;
	}
	if (KEY_HOLD(KEY::A))
	{
		vCurPos += vRight * moveMagnitude;
	}
	if (KEY_HOLD(KEY::D))
	{
		vCurPos -= vRight * moveMagnitude;
	}

	m_PHQ->GetOwner()->Transform()->SetRelativePos(vCurPos);
}

void CP_STATEMoveScript::CallAnimation()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	if (m_isDodge)
	{
		// 방향에 맞는 애니메이션 재생
		if (0.3 <= m_prevDir.x)
			m_PHQ->PlayAnimation(P_R2Dodge_R, false);
		else if (-0.3 >= m_prevDir.x)
			m_PHQ->PlayAnimation(P_R2Dodge_L, false);
		else
		{
			if (m_isFront)
				m_PHQ->PlayAnimation(P_R2Dodge, false);
			else
				m_PHQ->PlayAnimation(P_R2Dodge_N, false);
		}
		// 방향 Release 해주기
		/*Vec2 moveDir = m_PHQ->GetMoveDir();
		if (0 != moveDir.y)
		{
			if (m_isFront)
				m_PHQ->InputMove(0, -1.f);
			else
				m_PHQ->InputMove(0, 1.f);
		}
		if (0.3 <= m_prevDir.x)
			m_PHQ->InputMove(-1.f, 0.f);
		else if (-0.3 >= m_prevDir.x)
			m_PHQ->InputMove(1.f, 0.f);*/

		m_PHQ->ChangeState(static_cast<UINT>(eP_States::DODGE));
	}
	// 현재 방향 구하기
	else if (m_isFront)
	{
		if (CP_FSMScript::ePlayerStance::Sprint == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveSprint_R, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveSprint_L, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveSprint, true);
		}
		if (CP_FSMScript::ePlayerStance::Crouch == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_FR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_FL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch, true);
		}
		else if (CP_FSMScript::ePlayerStance::Aim == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_FR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_FL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkAim, true);
		}
		else if (CP_FSMScript::ePlayerStance::Normal == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalk_FR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalk_FL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalk, true);
		}
	}
	else
	{
		if (CP_FSMScript::ePlayerStance::Crouch == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_BR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_BL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_B, true);
		}
		else if (CP_FSMScript::ePlayerStance::Aim == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_BR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_BL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_B, true);
		}
		else if (CP_FSMScript::ePlayerStance::Normal == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalk_BR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalk_BL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalk_B, true);
		}
	}
}



void CP_STATEMoveScript::Enter()
{
}

void CP_STATEMoveScript::Exit()
{
	m_PHQ->InputSprint(false);	
	m_isDodge = false;
}