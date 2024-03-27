#include "pch.h"
#include "CM_Spider_STATE_Dead_Script.h"
#include "CM_Spider_FSMScript.h"

CM_Spider_STATE_Dead_Script::CM_Spider_STATE_Dead_Script()
	:m_fTime(0.f)
{
	SetStateType(static_cast<UINT>(eM_A_States::DEAD));
}

CM_Spider_STATE_Dead_Script::~CM_Spider_STATE_Dead_Script()
{
}

void CM_Spider_STATE_Dead_Script::tick()
{
	if (m_bdeath)
	{
		m_fTime += DT;

		if (m_fTime > 10.f)
			EraseThis();
	}
}

void CM_Spider_STATE_Dead_Script::begin()
{
	CM_Spider_StatesScript::begin();
	m_MHQ->Animator3D()->ActionEvent(Spi_Death, 97) = std::bind(&CM_Spider_STATE_Dead_Script::AniStop, this);
}

void CM_Spider_STATE_Dead_Script::EraseThis()
{
	DestroyObject(m_MHQ->GetOwner());
}

void CM_Spider_STATE_Dead_Script::AniStop()
{
	m_MHQ->Animator3D()->Stop();
	m_MHQ->GetGun()->Animator3D()->Stop();
	m_bdeath = true;
}

void CM_Spider_STATE_Dead_Script::Enter()
{
	m_MHQ->Animator3D()->Play(Spi_Death, false);
	m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Death, false);
}

void CM_Spider_STATE_Dead_Script::Exit()
{

}



