#include "pch.h"
#include "CB_STATEDeadScript.h"
#include "CB_FSMScript.h"


CB_STATEDeadScript::CB_STATEDeadScript()
{
	SetStateType(static_cast<UINT>(eB_States::DEAD));
}

CB_STATEDeadScript::~CB_STATEDeadScript()
{
}

void CB_STATEDeadScript::begin()
{
	CB_StateScript::begin();

	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Death) = std::bind(&CB_STATEDeadScript::Dead, this);

}

void CB_STATEDeadScript::Enter()
{
	m_BHQ->PlayAnim(B_Melee_Death, false);
}

void CB_STATEDeadScript::tick()
{

}

void CB_STATEDeadScript::Exit()
{
}

void CB_STATEDeadScript::Dead()
{
	DestroyObject(GetOwner());
}
