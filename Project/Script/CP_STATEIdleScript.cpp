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

void CP_STATEIdleScript::tick()
{
	Vec2 moveDir = m_PHQ->GetMoveDir();

	if (Vec2(0, 0) != moveDir)
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::MOVE));


	CP_FSMScript::tP_LongGunInfo* gun = m_PHQ->GetLongGunInfo();
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	if (CP_FSMScript::ePlayerStance::Aim == curStance)
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			if (gun->Fire())
			{
				m_PHQ->PlayAnimation(P_R2Fire, false);
				m_PHQ->ChangeState(static_cast<UINT>(eP_States::FIRE));
			}
		}
	}
	
	if (KEY_TAP(KEY::R))
	{
		if (gun->ReloadMag())
		{
			if (CP_FSMScript::ePlayerStance::Crouch == curStance)
				m_PHQ->PlayAnimation(P_R2ReloadCrouch, false);
			else
				m_PHQ->PlayAnimation(P_R2Reload, false);

			m_PHQ->ChangeState(static_cast<UINT>(eP_States::RELOAD));
		}		
	}		
}

void CP_STATEIdleScript::CallAnimation()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();

	if (CP_FSMScript::ePlayerStance::Crouch == curStance)
	{
		m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Crouch);
		m_PHQ->PlayAnimation(P_R2IdleCrouch, true);
	}
	else if (CP_FSMScript::ePlayerStance::Aim == curStance)
	{
		m_PHQ->GetMouseController()->ChangeWeaponMatrix((UINT)CP_MouseCtrlScript::eRotMat::Aim);
		m_PHQ->PlayAnimation(P_R2IdleAim, true);
	}
	else if (CP_FSMScript::ePlayerStance::Normal == curStance)
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
