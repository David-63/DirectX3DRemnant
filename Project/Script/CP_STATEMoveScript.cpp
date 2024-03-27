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
	Vec2 moveDir = m_PHQ->GetMoveDir();
	if ((0.1 >= moveDir.y && -0.1 <= moveDir.y)
		&& (0.1 >= moveDir.x && -0.1 <= moveDir.x))
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::IDLE));


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

	
}

void CP_STATEMoveScript::translateInput()
{
	float moveMagnitude = 0.f;
	Vec3 vMoveVector(0.f, 0.f, 0.f);

	Vec3 vFront = m_PHQ->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = m_PHQ->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = m_PHQ->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	CP_FSMScript::tP_Info playerInfo = m_PHQ->GetPlayerInfo();

	if (CP_FSMScript::ePlayerStance::Crouch == curStance)
		moveMagnitude = playerInfo.P_Stat.MoveSpeed * ScaleDT * 0.1f;
	else if (CP_FSMScript::ePlayerStance::Sprint == curStance && m_PHQ->IsFrontDir())
		moveMagnitude = playerInfo.P_Stat.MoveSpeed * ScaleDT * 2.8f;
	else
		moveMagnitude = playerInfo.P_Stat.MoveSpeed * ScaleDT;


	if (KEY_HOLD(KEY::W))
		vMoveVector += vFront;
	if (KEY_HOLD(KEY::S))
		vMoveVector -= vFront;
	if (KEY_HOLD(KEY::A))
		vMoveVector -= vRight;
	if (KEY_HOLD(KEY::D))
		vMoveVector += vRight;
	vMoveVector *= moveMagnitude * 50.f;
	m_PHQ->RigidBody()->SetVelocity(vMoveVector);
}

void CP_STATEMoveScript::CallAnimation()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	Vec2 moveDir = m_PHQ->GetMoveDir();

	if (m_PHQ->IsFrontDir())
	{
		if (CP_FSMScript::ePlayerStance::Sprint == curStance)
		{
			if (0.3 <= moveDir.x)
				m_PHQ->PlayAnimation(P_R2MoveSprint_R, true);
			else if (-0.3 >= moveDir.x)
				m_PHQ->PlayAnimation(P_R2MoveSprint_L, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveSprint, true);
		}
		if (CP_FSMScript::ePlayerStance::Crouch == curStance)
		{
			if (0.3 <= moveDir.x)
			{
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_FR, true);
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::CrouchRight);
			}
			else if (-0.3 >= moveDir.x)
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
		else if (CP_FSMScript::ePlayerStance::Aim == curStance)
		{
			if (0.3 <= moveDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_FR, true);
			else if (-0.3 >= moveDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_FL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkAim, true);
			m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Aim);
		}
		else if (CP_FSMScript::ePlayerStance::Normal == curStance)
		{
			if (0.3 <= moveDir.x)
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::MoveRight);
				m_PHQ->PlayAnimation(P_R2MoveWalk_FR, true);
			}
			else if (-0.3 >= moveDir.x)
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
		if (CP_FSMScript::ePlayerStance::Crouch == curStance)
		{
			if (0.3 <= moveDir.x)
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::CrouchRight);
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_BR, true);
			}
			else if (-0.3 >= moveDir.x)
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
		else if (CP_FSMScript::ePlayerStance::Aim == curStance)
		{
			if (0.3 <= moveDir.x)

				m_PHQ->PlayAnimation(P_R2MoveWalkAim_BR, true);
			else if (-0.3 >= moveDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_BL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_B, true);
			m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Aim);
		}
		else if (CP_FSMScript::ePlayerStance::Normal == curStance)
		{
			if (0.3 <= moveDir.x)
			{
				m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::MoveRight);
				m_PHQ->PlayAnimation(P_R2MoveWalk_BR, true);
			}
			else if (-0.3 >= moveDir.x)
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