#include "pch.h"
#include "CM_Lurker_STATE_Melee_Script.h"
#include "CM_Lurker_STATE_Move_Script.h"

CM_Lurker_STATE_Move_Script::CM_Lurker_STATE_Move_Script()
{
	SetStateType(static_cast<UINT>(eM_States::MOVE));
}

CM_Lurker_STATE_Move_Script::~CM_Lurker_STATE_Move_Script()
{
}

void CM_Lurker_STATE_Move_Script::tick()
{
}

void CM_Lurker_STATE_Move_Script::begin()
{
	CM_Lurker_StatesScript::begin();
}

void CM_Lurker_STATE_Move_Script::Enter()
{
	//m_MHQ->Animator3D()->CompleteEvent(Lurker_ALERT01) = std::bind(&CM_Lurker_STATE_Idle_Script::ChangeStateChase, this);
}

void CM_Lurker_STATE_Move_Script::Exit()
{
}


