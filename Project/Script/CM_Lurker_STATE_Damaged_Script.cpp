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

void CM_Lurker_STATE_Damaged_Script::tick()
{
}

void CM_Lurker_STATE_Damaged_Script::begin()
{
	CM_Lurker_StatesScript::begin();
}

void CM_Lurker_STATE_Damaged_Script::Enter()
{
	//m_MHQ->Animator3D()->CompleteEvent(Lurker_ALERT01) = std::bind(&CM_Lurker_STATE_Idle_Script::ChangeStateChase, this);
}

void CM_Lurker_STATE_Damaged_Script::Exit()
{
}


