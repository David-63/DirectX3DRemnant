#include "pch.h"
#include "CP_STATEMoveScript.h"
#include <Engine/CTransform.h>
#include "CP_FSMScript.h"

CP_STATEMoveScript::CP_STATEMoveScript()
{
	SetStateType(static_cast<UINT>(eP_States::MOVE));
}

CP_STATEMoveScript::~CP_STATEMoveScript()
{
}

void CP_STATEMoveScript::tick()
{
	if ((0.1 >= m_PlayerMoveDir->y && -0.1 <= m_PlayerMoveDir->y)
		&& (0.1 >= m_PlayerMoveDir->x && -0.1 <= m_PlayerMoveDir->x))
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::IDLE));

	if (m_prevDir != *m_PlayerMoveDir)
	{
		m_prevDir = *m_PlayerMoveDir;
		if (KEY_HOLD(KEY::U))
		{
			int a = 0;
		}
		CallAnimation();
		
	}

	moveVelocity();
}

void CP_STATEMoveScript::moveVelocity()
{
	float moveMagnitude = 0.f;
	Vec3 vMoveVector(0.f, 0.f, 0.f);

	Vec3 vFront = m_PHQ->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = m_PHQ->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = m_PHQ->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	// 방향부터 계산

	if (0.3 <= m_PlayerMoveDir->x)
		vMoveVector += vRight;
	else if (-0.3 >= m_PlayerMoveDir->x)
		vMoveVector -= vRight;
	if (0.3 <= m_PlayerMoveDir->y)
		vMoveVector += vFront;
	else if (-0.3 >= m_PlayerMoveDir->y)
		vMoveVector -= vFront;

	// 상태에 맞게 이동량 계산
	if (ePlayerStance::Crouch == *m_PlayerStance)
		moveMagnitude = m_PlayerInfo->P_Stat.MoveSpeed * ScaleDT * 0.1f;
	else if (ePlayerStance::Sprint == *m_PlayerStance && m_PHQ->IsFrontDir())
		moveMagnitude = m_PlayerInfo->P_Stat.MoveSpeed * ScaleDT * 3.8f;
	else
		moveMagnitude = m_PlayerInfo->P_Stat.MoveSpeed * ScaleDT;

	// 최종 이동량 계산
	vMoveVector *= moveMagnitude * 50.f;
	m_PHQ->RigidBody()->SetVelocity(vMoveVector);
}

void CP_STATEMoveScript::CallAnimation()
{
	if (m_PHQ->IsFrontDir())
	{
		if (ePlayerStance::Sprint == *m_PlayerStance)
		{
			if (0.3 <= m_PlayerMoveDir->x)
				m_PHQ->PlayAnimation(P_R2MoveSprint_R, true);
			else if (-0.3 >= m_PlayerMoveDir->x)
				m_PHQ->PlayAnimation(P_R2MoveSprint_L, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveSprint, true);
		}
		if (ePlayerStance::Crouch == *m_PlayerStance)
		{
			if (0.3 <= m_PlayerMoveDir->x)
			{
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_FR, true);
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::CrouchRight);
			}
			else if (-0.3 >= m_PlayerMoveDir->x)
			{
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_FL, true);
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::CrouchLeft);
			}
			else
			{
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch, true);
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Crouch);
			}
		}
		else if (ePlayerStance::Aim == *m_PlayerStance)
		{
			if (0.3 <= m_PlayerMoveDir->x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_FR, true);
			else if (-0.3 >= m_PlayerMoveDir->x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_FL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkAim, true);
			m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Aim);
		}
		else if (ePlayerStance::Normal == *m_PlayerStance)
		{
			if (0.3 <= m_PlayerMoveDir->x)
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::MoveRight);
				m_PHQ->PlayAnimation(P_R2MoveWalk_FR, true);
			}
			else if (-0.3 >= m_PlayerMoveDir->x)
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::MoveLeft);
				m_PHQ->PlayAnimation(P_R2MoveWalk_FL, true);
			}
			else
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Normal);
				m_PHQ->PlayAnimation(P_R2MoveWalk, true);
			}
		}
	}
	else
	{
		if (ePlayerStance::Crouch == *m_PlayerStance)
		{
			if (0.3 <= m_PlayerMoveDir->x)
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::CrouchRight);
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_BR, true);
			}
			else if (-0.3 >= m_PlayerMoveDir->x)
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::CrouchLeft);
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_BL, true);
			}
			else
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Crouch);
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_B, true);
			}
		}
		else if (ePlayerStance::Aim == *m_PlayerStance)
		{
			if (0.3 <= m_PlayerMoveDir->x)

				m_PHQ->PlayAnimation(P_R2MoveWalkAim_BR, true);
			else if (-0.3 >= m_PlayerMoveDir->x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_BL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_B, true);
			m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Aim);
		}
		else if (ePlayerStance::Normal == *m_PlayerStance)
		{
			if (0.3 <= m_PlayerMoveDir->x)
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::MoveRight);
				m_PHQ->PlayAnimation(P_R2MoveWalk_BR, true);
			}
			else if (-0.3 >= m_PlayerMoveDir->x)
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::MoveLeft);
				m_PHQ->PlayAnimation(P_R2MoveWalk_BL, true);
			}
			else
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Normal);
				m_PHQ->PlayAnimation(P_R2MoveWalk_B, true);
			}
		}
	}
}



void CP_STATEMoveScript::Enter()
{
	CallAnimation();
}

void CP_STATEMoveScript::Exit()
{
	m_PHQ->InputSprint(false);
}