#include "pch.h"
#include "CM_Spider_STATE_Idle_Script.h"
#include "CM_Spider_FSMScript.h"

CM_Spider_STATE_Idle_Script::CM_Spider_STATE_Idle_Script()
	: m_bAlert(false)
{
	SetStateType(static_cast<UINT>(eM_A_States::IDLE));

}

CM_Spider_STATE_Idle_Script::~CM_Spider_STATE_Idle_Script()
{
}

void CM_Spider_STATE_Idle_Script::begin()
{
	CM_Spider_StatesScript::begin();
}

void CM_Spider_STATE_Idle_Script::tick()
{
	/*if (KEY_TAP(KEY::Y))
	{
		mKey = true;
	}

	if (mKey)
	{
		if (Turn90L(-30.f))
		{
			mKey = false;
		}
	}*/

	if (!m_bAlert)
	{
		if (DistBetwPlayer() < 800.f)
			m_bAlert = true;
	}

	if (m_bAlert && !m_bAlertOnce)
	{
		EmergePose();
		m_bAlertOnce = true;
	}
}


void CM_Spider_STATE_Idle_Script::EmergePose()
{
	m_MHQ->Animator3D()->Play(Spi_Alert, true);
	m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Alert, true);
}

void CM_Spider_STATE_Idle_Script::ChangeStateChase()
{
	m_MHQ->ChangeState((UINT)eM_A_States::MOVE);
}


void CM_Spider_STATE_Idle_Script::Enter()
{
	m_MHQ->Animator3D()->CompleteEvent(SpiGun_Alert) = std::bind(&CM_Spider_STATE_Idle_Script::ChangeStateChase, this);

	m_MHQ->Animator3D()->Play(Spi_IdleP, true);
	m_MHQ->GetGun()->Animator3D()->Play(SpiGun_IdleP, true);
}

void CM_Spider_STATE_Idle_Script::Exit()
{

}



