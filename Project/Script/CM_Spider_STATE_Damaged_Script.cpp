#include "pch.h"
#include "CM_Spider_STATE_Damaged_Script.h"
#include "CM_Spider_FSMScript.h"


CM_Spider_STATE_Damaged_Script::CM_Spider_STATE_Damaged_Script()
{
	SetStateType(static_cast<UINT>(eM_A_States::DAMAGED));
}

CM_Spider_STATE_Damaged_Script::~CM_Spider_STATE_Damaged_Script()
{
}

void CM_Spider_STATE_Damaged_Script::begin()
{
	CM_Spider_StatesScript::begin();

	m_vBackDir = GetMyFront();

	m_MHQ->Animator3D()->CompleteEvent(Spi_ImpactF) = std::bind(&CM_Spider_STATE_Damaged_Script::AniComplete, this);
	m_MHQ->Animator3D()->ActionEvent(Spi_ImpactF, 40) = std::bind(&CM_Spider_STATE_Damaged_Script::SetStop, this);
	m_MHQ->Animator3D()->CompleteEvent(Spi_ImpactHeavyF) = std::bind(&CM_Spider_STATE_Damaged_Script::AniComplete, this);
	m_MHQ->Animator3D()->ActionEvent(Spi_ImpactHeavyF, 70) = std::bind(&CM_Spider_STATE_Damaged_Script::SetStop, this);
}

void CM_Spider_STATE_Damaged_Script::tick()
{
	if (m_bAniComplete)
		m_MHQ->ChangeState((UINT)eM_A_States::MOVE);

	Vec3 myPos = GetMyPos();
	myPos += m_vBackDir * DT * m_fBackSpeed;
	SetMyPos(myPos);
}



void CM_Spider_STATE_Damaged_Script::AniComplete()
{
	m_bAniComplete = true;
}

void CM_Spider_STATE_Damaged_Script::SetStop()
{
	m_fBackSpeed = 0.f;
}

void CM_Spider_STATE_Damaged_Script::Enter()
{
	m_ePart = eADPart::HeavyF;
	m_fBackSpeed = 100.f;

	switch (m_ePart)
	{
	case CM_Spider_STATE_Damaged_Script::eADPart::NormalF:
		m_MHQ->Animator3D()->Play(Spi_ImpactF, true);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_ImpactF, true);
		break;
	case CM_Spider_STATE_Damaged_Script::eADPart::HeavyF:
		m_MHQ->Animator3D()->Play(Spi_ImpactHeavyF, true);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_ImpactHeavyF, true);
		break;
	default:
		break;
	}
	
}

void CM_Spider_STATE_Damaged_Script::Exit()
{
	m_bAniComplete = false;
}




