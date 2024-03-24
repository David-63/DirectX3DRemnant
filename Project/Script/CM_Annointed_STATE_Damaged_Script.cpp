#include "pch.h"
#include "CM_Annointed_STATE_Damaged_Script.h"
#include "CM_Annointed_FSMScript.h"


CM_Annointed_STATE_Damaged_Script::CM_Annointed_STATE_Damaged_Script()
{
	SetStateType(static_cast<UINT>(eM_A_States::DAMAGED));
}

CM_Annointed_STATE_Damaged_Script::~CM_Annointed_STATE_Damaged_Script()
{
}

void CM_Annointed_STATE_Damaged_Script::begin()
{
	CM_Annointed_StatesScript::begin();

	m_vBackDir = GetMyFront();

	//m_MHQ->Animator3D()->CompleteEvent(Lurker_ImpactLegs) = std::bind(&CM_Annointed_STATE_Damaged_Script::AniComplete, this);
	//m_MHQ->Animator3D()->CompleteEvent(Lurker_ImpactLegL) = std::bind(&CM_Annointed_STATE_Damaged_Script::AniComplete, this);
	//m_MHQ->Animator3D()->CompleteEvent(Lurker_ImpactLegR) = std::bind(&CM_Annointed_STATE_Damaged_Script::AniComplete, this);
	//m_MHQ->Animator3D()->CompleteEvent(Lurker_ImpactTorso) = std::bind(&CM_Annointed_STATE_Damaged_Script::AniComplete, this);
	//m_MHQ->Animator3D()->CompleteEvent(Lurker_ImpactFront) = std::bind(&CM_Annointed_STATE_Damaged_Script::AniComplete, this);
	//m_MHQ->Animator3D()->ActionEvent(Lurker_ImpactTorso, 25) = std::bind(&CM_Annointed_STATE_Damaged_Script::SetStop, this);
	//m_MHQ->Animator3D()->ActionEvent(Lurker_ImpactFront, 8) = std::bind(&CM_Annointed_STATE_Damaged_Script::SetStop, this);
}

void CM_Annointed_STATE_Damaged_Script::tick()
{
	if (m_bAniComplete)
		m_MHQ->ChangeState((UINT)eM_A_States::MOVE);

	Vec3 myPos = GetMyPos();
	myPos += m_vBackDir * DT * m_fBackSpeed;
	SetMyPos(myPos);
}



void CM_Annointed_STATE_Damaged_Script::AniComplete()
{
	m_bAniComplete = true;
}

void CM_Annointed_STATE_Damaged_Script::SetStop()
{
	m_fBackSpeed = 0.f;
}

void CM_Annointed_STATE_Damaged_Script::Enter()
{
	m_ePart = eADPart::Heavy;
	switch (m_ePart)
	{
	case eADPart::Legs:
		//m_MHQ->Animator3D()->Play(Lurker_ImpactLegs, false);
		m_fBackSpeed = 50.f;
		break;
	case eADPart::LegL:
		//m_MHQ->Animator3D()->Play(Lurker_ImpactLegL, false);
		m_fBackSpeed = 50.f;
		break;
	case eADPart::LegR:
		//m_MHQ->Animator3D()->Play(Lurker_ImpactLegR, false);
		m_fBackSpeed = 50.f;
		break;
	case eADPart::Torso:
		//m_MHQ->Animator3D()->Play(Lurker_ImpactTorso, false);
		m_fBackSpeed = 50.f;
		break;
	case eADPart::Heavy:
		//m_MHQ->Animator3D()->Play(Lurker_ImpactFront, false);
		m_fBackSpeed = 70.f;
		break;
	default:
		break;
	}
}

void CM_Annointed_STATE_Damaged_Script::Exit()
{
	m_bAniComplete = false;
}




