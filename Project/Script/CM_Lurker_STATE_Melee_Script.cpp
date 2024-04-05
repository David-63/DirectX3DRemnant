#include "pch.h"
#include "CM_Lurker_STATE_Melee_Script.h"
#include "CM_Lurker_FSMScript.h"
#include "Engine/func.h"
#include "CHitBoxScript.h"


CM_Lurker_STATE_Melee_Script::CM_Lurker_STATE_Melee_Script()
{
	SetStateType(static_cast<UINT>(eM_States::MELEE));
}

CM_Lurker_STATE_Melee_Script::~CM_Lurker_STATE_Melee_Script()
{
}

void CM_Lurker_STATE_Melee_Script::begin()
{
	CM_Lurker_StatesScript::begin();
	m_MHQ->Animator3D()->CompleteEvent(Lurker_Dash) = std::bind(&CM_Lurker_STATE_Melee_Script::AtkComplete, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_Dash, 20) = std::bind(&CM_Lurker_STATE_Melee_Script::AdjustDashSpeed, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_Dash, 18) = std::bind(&CM_Lurker_STATE_Melee_Script::DashHitBoxOn, this);

	m_MHQ->Animator3D()->CompleteEvent(Lurker_Heavy1) = std::bind(&CM_Lurker_STATE_Melee_Script::AtkComplete, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_Heavy1, 40) = std::bind(&CM_Lurker_STATE_Melee_Script::AdjustZeroSpeed, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_Heavy1, 30) = std::bind(&CM_Lurker_STATE_Melee_Script::HeavyHitBoxOn, this);

	m_MHQ->Animator3D()->CompleteEvent(Lurker_SlashR) = std::bind(&CM_Lurker_STATE_Melee_Script::AtkComplete, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_SlashR, 50) = std::bind(&CM_Lurker_STATE_Melee_Script::AdjustZeroSpeed, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_SlashR, 12) = std::bind(&CM_Lurker_STATE_Melee_Script::SlashHitBoxOn, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_SlashR, 30) = std::bind(&CM_Lurker_STATE_Melee_Script::SlashHitBoxOff, this);

	m_MHQ->Animator3D()->CompleteEvent(Lurker_SlashRCombo) = std::bind(&CM_Lurker_STATE_Melee_Script::AtkComplete, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_SlashRCombo, 50) = std::bind(&CM_Lurker_STATE_Melee_Script::AdjustZeroSpeed, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_SlashRCombo, 9) = std::bind(&CM_Lurker_STATE_Melee_Script::SlashComboBoxOn, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_SlashRCombo, 20) = std::bind(&CM_Lurker_STATE_Melee_Script::SlashComboBoxOff, this);


	Vec3 ShooterPos = m_MHQ->GetOwner()->Transform()->GetRelativePos();
	CGameObject* attacker = m_MHQ->GetOwner();

	tHitInfo info0;
	info0.Damage = 20;
	info0.KnockBackGrade = 2;
	info0.Shooter = attacker;
	info0.ShooterPos = ShooterPos;
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 50.f, 30.f), Vec3(0.f, 100.f, -200.f), info0);
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(0);

	tHitInfo info1;
	info1.Damage = 15;
	info1.KnockBackGrade = 2;
	info1.Shooter = attacker;
	info1.ShooterPos = ShooterPos;
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 50.f, 30.f), Vec3(0.f, 100.f, -160.f), info1);
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(1);

	tHitInfo info2;
	info2.Damage = 10;
	info2.KnockBackGrade = 1;
	info2.Shooter = attacker;
	info2.ShooterPos = ShooterPos;
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(180.f, 50.f, 30.f), Vec3(0.f, 100.f, -160.f), info2);
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(2);

	tHitInfo info3;
	info3.Damage = 10;
	info3.KnockBackGrade = 1;
	info3.Shooter = attacker;
	info3.ShooterPos = ShooterPos;
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(180.f, 50.f, 30.f), Vec3(-20.f, 100.f, -160.f), info3);
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(3);
}

void CM_Lurker_STATE_Melee_Script::tick()
{
	DistCheck();
	switch (m_eMeleeState)
	{
	case eMeleeState::Dash:
		Dash();
		break;
	case eMeleeState::Heavy1:
		Heavy1();
		break;
	case eMeleeState::Slash:
		Slash();
		break;
	case eMeleeState::SlashCombo:
		SlashCombo();
		break;
	default:
		break;
	}
}


void CM_Lurker_STATE_Melee_Script::Dash()
{
	if (!m_bDashOnce)
	{
		m_MHQ->Animator3D()->Play(Lurker_Dash, false);
		m_fSpeed = 300.f;
		m_vAtkDir = -GetMyFront();
		m_bDashOnce = true;
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
		m_bDashOnce = false;
		m_bAtkComplete = false;
		m_MHQ->ChangeState((UINT)eM_States::MOVE);
	}

}

void CM_Lurker_STATE_Melee_Script::AdjustDashSpeed()
{
	m_fSpeed = 30.f;

}

void CM_Lurker_STATE_Melee_Script::AtkComplete()
{
	m_bAtkComplete = true;
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(0);
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(1);
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(2);
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(3);
}


void CM_Lurker_STATE_Melee_Script::Heavy1()
{
	if (!m_bHeavyOnce)
	{
		m_MHQ->Animator3D()->Play(Lurker_Heavy1, false);

		m_vAtkDir = -GetMyFront();
		m_fSpeed = 200.f;
		m_bHeavyOnce = true;
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
		m_bHeavyOnce = false;
		m_bAtkComplete = false;
		m_MHQ->ChangeState((UINT)eM_States::MOVE);
	}
}

void CM_Lurker_STATE_Melee_Script::AdjustZeroSpeed()
{
	m_fSpeed = 0.f;
}

void CM_Lurker_STATE_Melee_Script::Slash()
{
	if (!m_bSlashOnce)
	{
		m_MHQ->Animator3D()->Play(Lurker_SlashR, false);

		m_vAtkDir = -GetMyFront();
		m_fSpeed = 100.f;
		m_bSlashOnce = true;
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
		m_bSlashOnce = false;
		m_bAtkComplete = false;
		m_eMeleeState = eMeleeState::SlashCombo;
	}
}

void CM_Lurker_STATE_Melee_Script::SlashCombo()
{
	if (!m_bSlashOnce)
	{
		m_MHQ->Animator3D()->Play(Lurker_SlashRCombo, false);

		m_vAtkDir = -GetMyFront();
		m_fSpeed = 100.f;
		m_bSlashOnce = true;
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
		m_bSlashOnce = false;
		m_bAtkComplete = false;
		m_MHQ->ChangeState((UINT)eM_States::MOVE);
	}
}

void CM_Lurker_STATE_Melee_Script::DashHitBoxOn()
{
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(0);
}

void CM_Lurker_STATE_Melee_Script::HeavyHitBoxOn()
{
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(1);
}

void CM_Lurker_STATE_Melee_Script::SlashHitBoxOn()
{
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(2);
}

void CM_Lurker_STATE_Melee_Script::SlashHitBoxOff()
{
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(2);
}

void CM_Lurker_STATE_Melee_Script::SlashComboBoxOn()
{
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(3);
}

void CM_Lurker_STATE_Melee_Script::SlashComboBoxOff()
{
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(3);
}

void CM_Lurker_STATE_Melee_Script::DistCheck()
{
	if (DistBetwPlayer() < 200.f)
		m_bNearPlayer = true;
	else
		m_bNearPlayer = false;
}

void CM_Lurker_STATE_Melee_Script::Enter()
{
	int a = 0;
}

void CM_Lurker_STATE_Melee_Script::Exit()
{
	m_bAtkComplete = false;
	m_bDashOnce = false;
	m_bHeavyOnce = false;
	m_bSlashOnce = false;
}


