#include "pch.h"
#include "CP_FSMScript.h"

CP_FSMScript::CP_FSMScript()
	: m_InpCrouch(false), m_InpAim(false), m_InpSprint(false), m_IsChangeStance(true)
	, P_Stance(ePlayerStance::Normal)
{
	// ������ ȣ�� ���Ŀ� GameObject���� Owner ��������
}

CP_FSMScript::~CP_FSMScript()
{
}

void CP_FSMScript::begin()
{
	// State �־��ֱ�
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEIDLESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEMOVESCRIPT)));


	// �ִϸ��̼�
	GetOwner()->Animator3D()->Add(AnimIdleStand);
	GetOwner()->Animator3D()->Add(AnimIdleCrouch);

	GetOwner()->Animator3D()->Add(AnimMoveCrouch);
	GetOwner()->Animator3D()->Add(AnimMoveCrouchFL);
	GetOwner()->Animator3D()->Add(AnimMoveCrouchFR);
	GetOwner()->Animator3D()->Add(AnimMoveCrouchB);
	GetOwner()->Animator3D()->Add(AnimMoveCrouchBL);
	GetOwner()->Animator3D()->Add(AnimMoveCrouchBR);
	GetOwner()->Animator3D()->Add(AnimMoveWalk);
	GetOwner()->Animator3D()->Add(AnimMoveWalkFL);
	GetOwner()->Animator3D()->Add(AnimMoveWalkFR);
	GetOwner()->Animator3D()->Add(AnimMoveWalkB);
	GetOwner()->Animator3D()->Add(AnimMoveWalkBL);
	GetOwner()->Animator3D()->Add(AnimMoveWalkBR);

	PlayAnimation(AnimIdleStand, true);
	ChangeState(static_cast<UINT>(eP_States::IDLE));
}

void CP_FSMScript::tick()
{
	CC_FSMScript::tick();

	if (m_IsChangeStance)
	{
		CP_StatesScript* curState = dynamic_cast<CP_StatesScript*>(GetCurState());
		// �켱������ ���Ľ� ����
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

		// �޸��� �߰�
		// ȸ�� �߰�
		
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

