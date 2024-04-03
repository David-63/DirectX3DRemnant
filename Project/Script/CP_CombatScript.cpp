#include "pch.h"

#include "CP_CombatScript.h"
#include "CP_FSMScript.h"

CP_CombatScript::CP_CombatScript() : CScript((UINT)SCRIPT_TYPE::P_COMBATSCRIPT)
{
}

CP_CombatScript::~CP_CombatScript()
{
}

void CP_CombatScript::begin()
{
	//m_Gun = m_PHQ->GetLongGunInfo();
}

void CP_CombatScript::tick()
{
	// tick 체크
	if (!m_PHQ->GetLongGunInfo()->ReadyToFire.IsActivate())
	{
		if (!m_PHQ->GetLongGunInfo()->ReadyToFire.IsFinish())
			m_PHQ->GetLongGunInfo()->ReadyToFire.curTime += ScaleDT;
		else
			m_PHQ->GetLongGunInfo()->ReadyToFire.Activate();
	}

	if (m_PHQ->GetAtkSign())
	{
		tHitInfo info = m_PHQ->GetHitInfo();

		// 방향 찾기
		Vec3 ownerPos = m_PHQ->Transform()->GetRelativePos();
		Vec3 ownerFrontDir = m_PHQ->Transform()->GetRelativeDir(DIR_TYPE::FRONT);

		Vec2 m_Location = Vec2(info.ShooterPos.x, info.ShooterPos.z);
		Vec2 p_Location = Vec2(ownerPos.x, ownerPos.z);
		Vec2 hitDir = p_Location - m_Location;
		hitDir.Normalize();

		Vec2 p_frontDir = Vec2(ownerFrontDir.x, ownerFrontDir.z);

		
		// Back Attack!!!1
		if (p_frontDir.Dot(hitDir) > 0.2f)
			m_PHQ->HitDir(-1);
		else
			m_PHQ->HitDir(1);

		// 피해 넣어주기
		m_PHQ->GetPlayerInfo()->P_Health.GotDamage(info.Damage);

		
		m_PHQ->ChangeState((UINT)eP_States::STAGGER);
	}

	// input 체크
	if (KEY_HOLD(KEY::LBTN))
	{
		if (m_PHQ->IsInput((UINT)eInpStance::Aim))
		{
			// 원거리 공격
			if (m_PHQ->GetLongGunInfo()->ReadyToFire.IsActivate())
			{
				m_PHQ->GetLongGunInfo()->ReadyToFire.ResetTime();
				if (m_PHQ->GetLongGunInfo()->Fire())
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
				}
			}
		}
		else
		{
			// 근거리 공격
		}
	}
	// 플레이어의 공격 기능을 여기에 추가

	// 플레이어의 피격 기능을 여기에 추가
}

