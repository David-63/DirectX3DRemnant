#include "pch.h"
#include "CM_Spider_STATE_Atk_Script.h"
#include "CM_Spider_FSMScript.h"
#include "Engine/func.h"
#include "CHitBoxScript.h"


CM_Spider_STATE_Atk_Script::CM_Spider_STATE_Atk_Script()
{
	SetStateType(static_cast<UINT>(eM_A_States::ATTACK));
}

CM_Spider_STATE_Atk_Script::~CM_Spider_STATE_Atk_Script()
{
}

void CM_Spider_STATE_Atk_Script::begin()
{
	CM_Spider_StatesScript::begin();
	m_MHQ->Animator3D()->CompleteEvent(Spi_Atk) = std::bind(&CM_Spider_STATE_Atk_Script::AtkComplete, this);
	//m_MHQ->Animator3D()->ActionEvent(Spi_Atk, 50) = std::bind(&CM_Spider_STATE_Atk_Script::AdjustZeroSpeed, this);

	m_MHQ->Animator3D()->CompleteEvent(Spi_AtkPush) = std::bind(&CM_Spider_STATE_Atk_Script::AtkComplete, this);
	m_MHQ->Animator3D()->ActionEvent(Spi_AtkPush, 20) = std::bind(&CM_Spider_STATE_Atk_Script::AdjustZeroSpeed, this);
	m_MHQ->Animator3D()->ActionEvent(Spi_AtkPush, 34) = std::bind(&CM_Spider_STATE_Atk_Script::PushHitBoxOn, this);
	
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(90.f, 50.f, 30.f), Vec3(0.f, 100.f, -150.f));
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(0);
	
	m_eAtkState = eAtkState::Shoot;
}

void CM_Spider_STATE_Atk_Script::tick()
{
	DistCheck();

	switch (m_eAtkState)
	{
	case eAtkState::Push:
		Push();
		break;
	case eAtkState::Shoot:
		Shoot();
		break;
	default:
		break;
	}
}


void CM_Spider_STATE_Atk_Script::Shoot()
{
	if (!m_bShootOnce)
	{
		m_MHQ->Animator3D()->Play(Spi_Atk, false);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Atk, false);
		m_bShootOnce = true;
	}

	if (m_bAtkComplete)
	{
		m_bShootOnce = false;
		m_bAtkComplete = false;
		m_MHQ->ChangeState((UINT)eM_A_States::MOVE);
	}
	
}

void CM_Spider_STATE_Atk_Script::AdjustDashSpeed()
{
	m_fSpeed = 30.f;

}

void CM_Spider_STATE_Atk_Script::AtkComplete()
{
	m_bAtkComplete = true;
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(0);
}


void CM_Spider_STATE_Atk_Script::Push()
{
	if (!m_bPushOnce)
	{
		m_MHQ->Animator3D()->Play(Spi_AtkPush, false);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_AtkPush, false);

		m_vAtkDir = -GetMyFront();
		m_fSpeed = 50.f;
		m_bPushOnce = true;
	}

	if (!m_bAtkComplete)
	{
		if (m_bNearPlayer)
			return;

		Vec3 myPos = GetMyPos();
		myPos += m_vAtkDir * DT * m_fSpeed;
		SetMyPos(myPos);
	}
	else
	{
		m_bPushOnce = false;
		m_bAtkComplete = false;
		m_MHQ->ChangeState((UINT)eM_A_States::MOVE);
	}
}

void CM_Spider_STATE_Atk_Script::AdjustZeroSpeed()
{
	m_fSpeed = 0.f;
}

void CM_Spider_STATE_Atk_Script::Clear()
{
	m_bAtkComplete = false;
	m_bShootOnce = false;
	m_bPushOnce = false;
}


void CM_Spider_STATE_Atk_Script::PushHitBoxOn()
{
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(0);
}

void CM_Spider_STATE_Atk_Script::DistCheck()
{
	if (DistBetwPlayer() < 200.f)
		m_bNearPlayer = true;
	else
		m_bNearPlayer = false;
}

void CM_Spider_STATE_Atk_Script::Enter()
{
	
}

void CM_Spider_STATE_Atk_Script::Exit()
{
	Clear();
}


