#include "pch.h"
#include "CP_STATEIdleScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CEngine.h>
#include "CP_FSMScript.h"

CP_STATEIdleScript::CP_STATEIdleScript()
{
	SetStateType(static_cast<UINT>(eP_States::IDLE));
}

CP_STATEIdleScript::~CP_STATEIdleScript()
{
}


void CP_STATEIdleScript::begin()
{
	CP_StatesScript::begin();
	m_readyToFire.SetFinishTime(m_Gun->FireLate);
}

void CP_STATEIdleScript::tick()
{
	// 이동상태 확인

	if (Vec2(0, 0) != *m_PlayerMoveDir)
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::MOVE));


	if (!m_readyToFire.IsFinish())
	{
		m_readyToFire.curTime += ScaleDT;
	}
	else
	{
		m_readyToFire.Activate();
	}

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
					m_PHQ->ShootRay();
					//m_PHQ->ChangeState(static_cast<UINT>(eP_States::FIRE));
				}
			}
			
		}
	}
	
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
}

void CP_STATEIdleScript::CallAnimation()
{
	if (ePlayerStance::Crouch == *m_PlayerStance)
	{
		m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Crouch);
		m_PHQ->PlayAnimation(P_R2IdleCrouch, true);
	}
	else if (ePlayerStance::Aim == *m_PlayerStance)
	{
		m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Aim);
		m_PHQ->PlayAnimation(P_R2IdleAim, true);
	}
	else if (ePlayerStance::Normal == *m_PlayerStance)
	{
		m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Normal);
		m_PHQ->PlayAnimation(P_R2Idle, true);
	}
}

void CP_STATEIdleScript::Enter()
{
	CallAnimation();
}

void CP_STATEIdleScript::Exit()
{
	m_PHQ->OverrideObjRotY();
}
