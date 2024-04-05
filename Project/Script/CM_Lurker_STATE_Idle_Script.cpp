#include "pch.h"
#include "CM_Lurker_STATE_Idle_Script.h"
#include "CM_Lurker_FSMScript.h"

CM_Lurker_STATE_Idle_Script::CM_Lurker_STATE_Idle_Script()
	: m_bAlert(false)
	, m_iAniCount(0)
{
	SetStateType(static_cast<UINT>(eM_States::IDLE));
	//m_pose = eInitialPose::Up;
	//m_pose = eInitialPose::Closet;
	
}

CM_Lurker_STATE_Idle_Script::~CM_Lurker_STATE_Idle_Script()
{
}

void CM_Lurker_STATE_Idle_Script::begin()
{
	CM_Lurker_StatesScript::begin();
	
}

void CM_Lurker_STATE_Idle_Script::tick()
{
	/*if (KEY_TAP(KEY::Y))
	{
		mKey = true;
	}

	if (mKey)
	{
		if (Turn90L(-30.f))
		{
			mKey = false;
		}
	}*/


	if (!m_bAlert)
	{
		if (DistBetwPlayer() < 800.f)
			m_bAlert = true;
	}
	

	if (m_bAlert && !m_bAlertOnce)
	{
		EmergePose();
		m_bAlertOnce = true;
	}
}


void CM_Lurker_STATE_Idle_Script::SetInitialPose(UINT _pose)
{
	m_pose = (eInitialPose)_pose; 
	InitialPose();
}

void CM_Lurker_STATE_Idle_Script::AniStop()
{
	if (!m_bStopOnce)
	{
		m_MHQ->Animator3D()->Stop();
		m_bStopOnce = true;
	}
}

void CM_Lurker_STATE_Idle_Script::InitialPose()
{
	switch (m_pose)
	{
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Up:
		m_MHQ->Animator3D()->Play(Lurker_IDLESTAND, true);
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Down:
		m_MHQ->Animator3D()->Play(Lurker_IDLEDOWN, true);
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::DownDoo:
		m_MHQ->Animator3D()->Play(Lurker_IDLEDOWNDOO, true);
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Wall:
		m_MHQ->Animator3D()->Play(Lurker_WALL_EMERGE, false);
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Wall2:
		m_MHQ->Animator3D()->Play(Lurker_WALL_EMERGE2, false);
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Closet:
		m_MHQ->Animator3D()->Play(Lurker_EMERGE, false);
		break;
	default:
		break;
	}
}

void CM_Lurker_STATE_Idle_Script::EmergePose()
{
	m_MHQ->Animator3D()->Reset();

	switch (m_pose)
	{
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Up:
		ChangeStateChase();
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Down:
		m_MHQ->Animator3D()->Play(Lurker_ALERT01, false);
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::DownDoo:
		m_MHQ->Animator3D()->Play(Lurker_ALERT03, false);
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Wall:
		m_MHQ->Animator3D()->Play(Lurker_WALL_EMERGE, false);
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Wall2:
		m_MHQ->Animator3D()->Play(Lurker_WALL_EMERGE2, false);
		break;
	case CM_Lurker_STATE_Idle_Script::eInitialPose::Closet:
		m_MHQ->Animator3D()->Play(Lurker_EMERGE, false);
		break;
	default:
		break;
	}
}

void CM_Lurker_STATE_Idle_Script::ChangeStateChase()
{
	m_MHQ->ChangeState((UINT)eM_States::MOVE);
}


void CM_Lurker_STATE_Idle_Script::Enter()
{
	m_MHQ->Animator3D()->CompleteEvent(Lurker_ALERT01) = std::bind(&CM_Lurker_STATE_Idle_Script::ChangeStateChase, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_ALERT03) = std::bind(&CM_Lurker_STATE_Idle_Script::ChangeStateChase, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_WALL_EMERGE) = std::bind(&CM_Lurker_STATE_Idle_Script::ChangeStateChase, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_WALL_EMERGE2) = std::bind(&CM_Lurker_STATE_Idle_Script::ChangeStateChase, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_EMERGE) = std::bind(&CM_Lurker_STATE_Idle_Script::ChangeStateChase, this);

	m_MHQ->Animator3D()->ActionEvent(Lurker_WALL_EMERGE, 36) = std::bind(&CM_Lurker_STATE_Idle_Script::AniStop, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_WALL_EMERGE2 , 70) = std::bind(&CM_Lurker_STATE_Idle_Script::AniStop, this);
	m_MHQ->Animator3D()->ActionEvent(Lurker_EMERGE, 64) = std::bind(&CM_Lurker_STATE_Idle_Script::AniStop, this);

}

void CM_Lurker_STATE_Idle_Script::Exit()
{
	m_iAniCount = 0;
	m_bAlert = false;
	m_bAlertOnce = false;
}


