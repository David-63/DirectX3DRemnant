#include "pch.h"
#include "CM_Annointed_STATE_Dead_Script.h"
#include "CM_Annointed_FSMScript.h"

CM_Annointed_STATE_Dead_Script::CM_Annointed_STATE_Dead_Script()
	:m_fTime(0.f)
{
	SetStateType(static_cast<UINT>(eM_A_States::DEAD));
}

CM_Annointed_STATE_Dead_Script::~CM_Annointed_STATE_Dead_Script()
{
}

void CM_Annointed_STATE_Dead_Script::tick()
{
	if (m_bdeath)
	{
		//m_MHQ->Animator3D()->Play(Lurker_DeathGround, false);
		m_bdeath = false;
		m_bAfterDeath = true;
	}
	if (m_bAfterDeath)
	{
		m_fTime += DT;

		if (m_fTime > 10.f)
			EraseThis();
	}
}

void CM_Annointed_STATE_Dead_Script::begin()
{
	CM_Annointed_StatesScript::begin();
	//m_MHQ->Animator3D()->CompleteEvent(Lurker_Death) = std::bind(&CM_Annointed_STATE_Dead_Script::PlayDeathGround, this);
	//m_MHQ->Animator3D()->ActionEvent(Lurker_DeathGround, 20) = std::bind(&CM_Annointed_STATE_Dead_Script::AniStop, this);
}

void CM_Annointed_STATE_Dead_Script::EraseThis()
{
	DestroyObject(m_MHQ->GetOwner());
}

void CM_Annointed_STATE_Dead_Script::AniStop()
{
	m_MHQ->Animator3D()->Stop();
}

void CM_Annointed_STATE_Dead_Script::PlayDeathGround()
{
	m_bdeath = true;
	//m_MHQ->Animator3D()->Play(Lurker_DeathGround, false);
}

void CM_Annointed_STATE_Dead_Script::Enter()
{
	//m_MHQ->Animator3D()->Play(Lurker_Death, false);
}

void CM_Annointed_STATE_Dead_Script::Exit()
{

}



