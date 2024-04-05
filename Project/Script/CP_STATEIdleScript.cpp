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
	// 정지상태 확인
	if (Vec2(0, 0) != *m_PlayerMoveDir)
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::MELEE));

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
}
