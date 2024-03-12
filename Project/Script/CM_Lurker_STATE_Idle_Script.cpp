#include "pch.h"
#include "CM_Lurker_STATE_Idle_Script.h"
#include "CM_Lurker_FSMScript.h"

CM_Lurker_STATE_Idle_Script::CM_Lurker_STATE_Idle_Script()
	: m_bAlert(false)
{
	SetStateType(static_cast<UINT>(eM_States::IDLE));

	switch (m_pose)
	{
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Up:
		//m_MHQ->Animator3D()->Play(L"")
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Down:

		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Wall:
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Landing:
		break;
	default:
		break;
	}
}

CM_Lurker_STATE_Idle_Script::~CM_Lurker_STATE_Idle_Script()
{
}

void CM_Lurker_STATE_Idle_Script::tick()
{
	
}


void CM_Lurker_STATE_Idle_Script::Enter()
{
}

void CM_Lurker_STATE_Idle_Script::Exit()
{
}


