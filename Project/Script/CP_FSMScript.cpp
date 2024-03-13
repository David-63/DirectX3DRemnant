#include "pch.h"
#include "CP_FSMScript.h"

CP_FSMScript::CP_FSMScript()
	: m_InpCrouch(false), m_InpAim(false), m_InpSprint(false), m_IsChangeStance(true)
	, P_Stance(ePlayerStance::Normal)
{
	// 생성자 호출 이후에 GameObject에서 Owner 세팅해줌
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


	PlayAnimation(P_IdleR2, true);
	ChangeState(static_cast<UINT>(eP_States::IDLE));
}

void CP_FSMScript::tick()
{
	CC_FSMScript::tick();

	if (m_IsChangeStance)
	{
		CP_StatesScript* curState = dynamic_cast<CP_StatesScript*>(GetCurState());
		// 우선순위로 스탠스 적용
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

		// 달리기 추가
		// 회피 추가
		
		m_IsChangeStance = false;
	}
}

void CP_FSMScript::PlayAnimation(wstring _name, bool _repeat)
{
	GetOwner()->Animator3D()->Play(_name, _repeat);
}

void CP_FSMScript::BeginOverlap(CCollider3D* _Other)
{
	// Ray -> CollisionMgr != BeginOverlap

	// Ray -> EventMgr != AddEvent


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

