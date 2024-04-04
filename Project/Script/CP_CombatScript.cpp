#include "pch.h"

#include "CP_CombatScript.h"
#include "CP_FSMScript.h"

CP_CombatScript::CP_CombatScript() : CScript((UINT)SCRIPT_TYPE::P_COMBATSCRIPT), m_PHQ(nullptr), m_lightLife(0.1f)
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

	if (m_lightLife.IsActivate())
	{
		m_lightLife.curTime += ScaleDT;
		if (m_lightLife.IsFinish())
		{
			m_lightLife.ResetTime();
			m_PHQ->GetMuzzelFlash()->Light3D()->SetActiveLight(false);
		}
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
					m_PHQ->GetMuzzelFlash()->Light3D()->SetActiveLight(true);
					m_lightLife.Activate();
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
			// 애니메이션 넣어주고
			// 상태 바꿔주고

		}
	}
}

