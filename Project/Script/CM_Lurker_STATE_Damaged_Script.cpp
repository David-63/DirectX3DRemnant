#include "pch.h"
#include "CM_Lurker_STATE_Damaged_Script.h"
#include "CM_Lurker_FSMScript.h"

CM_Lurker_STATE_Damaged_Script::CM_Lurker_STATE_Damaged_Script()
{
	SetStateType(static_cast<UINT>(eM_States::DAMAGED));
}

CM_Lurker_STATE_Damaged_Script::~CM_Lurker_STATE_Damaged_Script()
{
}

void CM_Lurker_STATE_Damaged_Script::begin()
{
	CM_Lurker_StatesScript::begin();

	m_vBackDir = GetMyFront();

	m_MHQ->Animator3D()->CompleteEvent(Lurker_ImpactLegs) = std::bind(&CM_Lurker_STATE_Damaged_Script::AniComplete, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_ImpactLegL) = std::bind(&CM_Lurker_STATE_Damaged_Script::AniComplete, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_ImpactLegR) = std::bind(&CM_Lurker_STATE_Damaged_Script::AniComplete, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_ImpactTorso) = std::bind(&CM_Lurker_STATE_Damaged_Script::AniComplete, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_ImpactFront) = std::bind(&CM_Lurker_STATE_Damaged_Script::AniComplete, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_ImpactTorso, 25) = std::bind(&CM_Lurker_STATE_Damaged_Script::SetStop, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_ImpactFront, 8) = std::bind(&CM_Lurker_STATE_Damaged_Script::SetStop, this);
}

void CM_Lurker_STATE_Damaged_Script::tick()
{
	if (m_bAniComplete)
		m_MHQ->ChangeState((UINT)eM_States::MOVE);

	Vec3 myPos = GetMyPos();
	myPos += m_vBackDir * DT * m_fBackSpeed;
	SetMyPos(myPos);
}



void CM_Lurker_STATE_Damaged_Script::AniComplete()
{
	m_bAniComplete = true;
}

void CM_Lurker_STATE_Damaged_Script::SetStop()
{
	m_fBackSpeed = 0.f;
}

void CM_Lurker_STATE_Damaged_Script::Enter()
{
	m_ePart = eLDPart::Heavy;
	switch (m_ePart)
	{
	case eLDPart::Legs:
		m_MHQ->Animator3D()->Play(Lurker_ImpactLegs, false);
		m_fBackSpeed = 50.f;
		break;
	case eLDPart::LegL:
		m_MHQ->Animator3D()->Play(Lurker_ImpactLegL, false);
		m_fBackSpeed = 50.f;
		break;
	case eLDPart::LegR:
		m_MHQ->Animator3D()->Play(Lurker_ImpactLegR, false);
		m_fBackSpeed = 50.f;
		break;
	case eLDPart::Torso:
		m_MHQ->Animator3D()->Play(Lurker_ImpactTorso, false);
		m_fBackSpeed = 50.f;
		break;
	case eLDPart::Heavy:
		m_MHQ->Animator3D()->Play(Lurker_ImpactFront, false);
		m_fBackSpeed = 70.f;
		break;
	default:
		break;
	}
}

void CM_Lurker_STATE_Damaged_Script::Exit()
{
	m_bAniComplete = false;
}


