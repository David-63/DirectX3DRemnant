#include "pch.h"
#include "CP_FSMScript.h"
#include "CP_MouseCtrlScript.h"
#include <Engine/CRenderMgr.h>

CP_FSMScript::CP_FSMScript()
	: m_InpCrouch(false), m_InpAim(false), m_InpSprint(false), m_IsChangeStance(true)
	, P_Stance(ePlayerStance::Normal), m_StanceDelay(StanceDelay), m_ableMouse(true)
{
}

CP_FSMScript::~CP_FSMScript()
{
}

void CP_FSMScript::begin()
{
	// State 넣어주기
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEIDLESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEMOVESCRIPT)));


	// 애니메이션
	GetOwner()->Animator3D()->Add(P_IdleR2);
	GetOwner()->Animator3D()->Add(P_IdleR2Aim);
	GetOwner()->Animator3D()->Add(P_IdleR2AimCrouch);
	GetOwner()->Animator3D()->Add(P_IdleR2Crouch);

	GetOwner()->Animator3D()->Add(P_MoveR2AimCrouchWalk);
	GetOwner()->Animator3D()->Add(P_MoveR2AimCrouchWalk_B);
	GetOwner()->Animator3D()->Add(P_MoveR2AimCrouchWalk_BL);
	GetOwner()->Animator3D()->Add(P_MoveR2AimCrouchWalk_BR);
	GetOwner()->Animator3D()->Add(P_MoveR2AimCrouchWalk_FL);
	GetOwner()->Animator3D()->Add(P_MoveR2AimCrouchWalk_FR);
	GetOwner()->Animator3D()->Add(P_MoveR2AimWalk);
	GetOwner()->Animator3D()->Add(P_MoveR2AimWalk_B);
	GetOwner()->Animator3D()->Add(P_MoveR2AimWalk_BL);
	GetOwner()->Animator3D()->Add(P_MoveR2AimWalk_BR);
	GetOwner()->Animator3D()->Add(P_MoveR2AimWalk_FL);
	GetOwner()->Animator3D()->Add(P_MoveR2AimWalk_FR);
	GetOwner()->Animator3D()->Add(P_MoveR2CrouchWalk);
	GetOwner()->Animator3D()->Add(P_MoveR2CrouchWalk_B);
	GetOwner()->Animator3D()->Add(P_MoveR2CrouchWalk_BL);
	GetOwner()->Animator3D()->Add(P_MoveR2CrouchWalk_BR);
	GetOwner()->Animator3D()->Add(P_MoveR2CrouchWalk_FL);
	GetOwner()->Animator3D()->Add(P_MoveR2CrouchWalk_FR);
	GetOwner()->Animator3D()->Add(P_MoveR2Jog);
	GetOwner()->Animator3D()->Add(P_MoveR2Jog_B);
	GetOwner()->Animator3D()->Add(P_MoveR2Jog_BL);
	GetOwner()->Animator3D()->Add(P_MoveR2Jog_BR);
	GetOwner()->Animator3D()->Add(P_MoveR2Jog_FL);
	GetOwner()->Animator3D()->Add(P_MoveR2Jog_FR);


	// GetOwner()->Animator3D()->CompleteEvent(P_MoveR2Jog)

	PlayAnimation(P_IdleR2, true);
	ChangeState(static_cast<UINT>(eP_States::IDLE));

	m_MouseCtrl.SetOwner(this);
	m_MouseCtrl.SetMainCam(CRenderMgr::GetInst()->GetMainCam());
}

void CP_FSMScript::tick()
{
	CC_FSMScript::tick();

	m_MouseCtrl.tick();

	// 상태 제어용 딜레이 타임
	

	if (m_IsChangeStance)
	{
		m_StanceDelay.curTime += ScaleDT;

		if (m_StanceDelay.IsFinish())
		{
			m_StanceDelay.ResetTime();

			CP_StatesScript* curState = dynamic_cast<CP_StatesScript*>(GetCurState());
			if (m_InpSprint)
			{
				ChangeStance(ePlayerStance::Sprint);
				curState->CallAnimation();
			}
			else if (m_InpCrouch && m_InpAim)
			{
				ChangeStance(ePlayerStance::CrouchAim);
				curState->CallAnimation();
			}
			else if (m_InpCrouch)
			{
				ChangeStance(ePlayerStance::Crouch);
				curState->CallAnimation();
			}
			else if (m_InpAim)
			{
				ChangeStance(ePlayerStance::Aim);
				curState->CallAnimation();
			}
			else
			{
				ChangeStance(ePlayerStance::Normal);
				curState->CallAnimation();
			}
			m_IsChangeStance = false;
		}
	}
}

void CP_FSMScript::PlayAnimation(wstring _name, bool _repeat)
{
	GetOwner()->Animator3D()->Play(_name, _repeat);
}

void CP_FSMScript::BeginOverlap(CCollider3D* _Other)
{

}

void CP_FSMScript::OnOverlap(CCollider3D* _Other)
{
}

void CP_FSMScript::EndOverlap(CCollider3D* _Other)
{
}


void CP_FSMScript::SaveToScene(FILE* _pFile)
{
}

void CP_FSMScript::LoadFromScene(FILE* _pFile)
{
}

