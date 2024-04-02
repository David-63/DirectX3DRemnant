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

void CP_STATEMoveScript::begin()
{
	CP_StatesScript::begin();
	m_readyToFire.SetFinishTime(m_Gun->FireLate);
}

void CP_STATEMoveScript::tick()
{
	if ((0.1 >= m_PlayerMoveDir->y && -0.1 <= m_PlayerMoveDir->y)
		&& (0.1 >= m_PlayerMoveDir->x && -0.1 <= m_PlayerMoveDir->x))
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::IDLE));

	// 여기서 애니메이션 2중 호출됨 (조건을 완화시켜야할듯?) 
	// 의도한 로직은 진행방향이 바뀌면 애니메이션을 갱신시키는건데
	// 멈춰있다가 이동하니까 여기에 조건이 걸림
	if (m_prevDir != *m_PlayerMoveDir)
	{
		m_prevDir = *m_PlayerMoveDir;
		CallAnimation();
		if (m_prevDir != *m_PlayerMoveDir)
			int a = 0;
	}
	if (!m_readyToFire.IsFinish())
	{
		m_readyToFire.curTime += ScaleDT;
	}
	else
	{
		m_readyToFire.Activate();
	}


	translateInput();
	if (KEY_TAP(KEY::R))
	{
		if (m_Gun->ReloadMag())
		{
			if (ePlayerStance::Crouch == *m_PlayerStance)
				m_PHQ->PlayAnimation(P_R2ReloadCrouch, false); 
			else
				m_PHQ->PlayAnimation(P_R2Reload, false);

			if (m_PHQ->IsInput((UINT)eInpStance::Crouch))
				m_PHQ->InputCrouch();
			if (m_PHQ->IsInput((UINT)eInpStance::Aim))
				m_PHQ->InputAim();
			m_PHQ->InputSprint(false);
			m_PHQ->ChangeState(static_cast<UINT>(eP_States::RELOAD));
		}
	}
	if (!m_PHQ->IsSprint())
	{
		if (ePlayerStance::Aim == *m_PlayerStance)
		{
			if (KEY_HOLD(KEY::LBTN))
			{
				if (m_readyToFire.IsActivate())
				{
					m_readyToFire.ResetTime();
					if (m_Gun->Fire())
					{
						m_PHQ->PlayAnimation(P_R2Fire, false);
						CParticleSystem* particle = m_PHQ->GetBullet()->ParticleSystem();
						tParticleModule ModuleData = particle->GetModuleData();
						ModuleData.bDead = false;
						particle->SetModuleData(ModuleData);
						particle->ActiveParticle();
						particle = m_PHQ->GetMuzzelFlash()->ParticleSystem();
						ModuleData = particle->GetModuleData();
						ModuleData.bDead = false;
						particle->SetModuleData(ModuleData);
						particle->ActiveParticle();
						if (m_PHQ->IsInput((UINT)eInpStance::Crouch))
							m_PHQ->InputCrouch();
						m_PHQ->InputSprint(false);
					}
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

	if (ePlayerStance::Crouch == *m_PlayerStance)
		moveMagnitude = m_PlayerInfo->P_Stat.MoveSpeed * ScaleDT * 0.1f;
	else if (ePlayerStance::Sprint == *m_PlayerStance && m_PHQ->IsFrontDir())
		moveMagnitude = m_PlayerInfo->P_Stat.MoveSpeed * ScaleDT * 3.8f;
	else
		moveMagnitude = m_PlayerInfo->P_Stat.MoveSpeed * ScaleDT;


	if (KEY_HOLD(KEY::W))
		vMoveVector += vFront;
	if (KEY_HOLD(KEY::S))
		vMoveVector -= vFront;
	if (KEY_HOLD(KEY::A))
		vMoveVector -= vRight;
	if (KEY_HOLD(KEY::D))
	{
		vMoveVector += vRight;
		m_PHQ->InputSprint(false);
	}
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
	//CallAnimation();
}

void CP_STATEMoveScript::Exit()
{
	m_PHQ->InputSprint(false);
}