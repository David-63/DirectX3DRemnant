#include "pch.h"
#include "CP_STATEMoveScript.h"
#include <Engine/CTransform.h>
#include "CP_FSMScript.h"

CP_STATEMoveScript::CP_STATEMoveScript() : m_isMelee(false)
{
	SetStateType(static_cast<UINT>(eP_States::MOVE));
}

CP_STATEMoveScript::~CP_STATEMoveScript()
{
}

void CP_STATEMoveScript::tick()
{	
	// 스탠스 변경 키 : AIM
	if (KEY_TAP(KEY::RBTN))
	{
		m_PHQ->InputAim();
	}
	// 스탠스 변경 키 : CROUCH
	if (KEY_TAP(KEY::LCTRL))
	{
		m_PHQ->InputCrouch();
	}
	// 스탠스 변경 키 : DODGE

	// 이동
	TranslateInput();

	// 스테이트 변경 : IDEL
	if (KEY_RELEASE(KEY::W) || KEY_RELEASE(KEY::A)
		|| KEY_RELEASE(KEY::S) || KEY_RELEASE(KEY::D))
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::IDLE));	
	// 스테이트 변경 : MELEE
	// 스테이트 변경 : DAMAGED
}

void CP_STATEMoveScript::MoveKeyInput()
{
	// 방향이 바뀌는 키
	/*if (KEY_TAP(KEY::W))
	{
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::Front);
	}
	if (KEY_TAP(KEY::S))
	{
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::Back);
	}
	if (KEY_TAP(KEY::A))
	{
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::Left);
	}
	if (KEY_TAP(KEY::D))
	{
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::Right);
	}*/

	// 상태가 바뀌는 키
	if (KEY_TAP(KEY::SPACE))
	{
		// 현재 방향에 따라 Dodge 방향도 적용됨
		

		//m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::None);
	}

	if (KEY_TAP(KEY::LBTN))
	{
		m_isMelee = true;
		m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::N);
	}

	if (KEY_TAP(KEY::RBTN))
	{
		m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Aim);

	}
	if (KEY_RELEASE(KEY::RBTN))
	{
		m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Normal);
	}

	if (KEY_TAP(KEY::LCTRL))
	{
		m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Crouch);

	}
	if (KEY_RELEASE(KEY::LCTRL))
	{
		m_PHQ->ChangeStance(CP_FSMScript::ePlayerStance::Normal);
	}
}

void CP_STATEMoveScript::MoveMouseInput()
{
}

void CP_STATEMoveScript::TranslateInput()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();

	Vec3 vCurPos = m_PHQ->GetOwner()->Transform()->GetRelativePos();

	Vec3 vFront = m_PHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = m_PHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = m_PHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	// 이동량 구하기

	float moveMagnitude = 0.f;
	CP_FSMScript::tP_Info playerInfo = m_PHQ->GetPlayerInfo();
	
	if (CP_FSMScript::ePlayerStance::Normal == curStance)
		moveMagnitude = playerInfo.P_Stat.MoveSpeed * ScaleDT;
	if (CP_FSMScript::ePlayerStance::Sprint == curStance)
		moveMagnitude = playerInfo.P_Stat.MoveSpeed * ScaleDT * 1.8f;
	else
		moveMagnitude = playerInfo.P_Stat.MoveSpeed * ScaleDT * 0.3f;
	 
	if (KEY_HOLD(KEY::W))
	{
		vCurPos -= vFront * moveMagnitude;
	}
	if (KEY_HOLD(KEY::S))
	{
		vCurPos += vFront * moveMagnitude;
	}
	if (KEY_HOLD(KEY::A))
	{
		vCurPos -= vRight * moveMagnitude;
	}
	if (KEY_HOLD(KEY::D))
	{
		vCurPos += vRight * moveMagnitude;
	}

	m_PHQ->GetOwner()->Transform()->SetRelativePos(vCurPos);

}

void CP_STATEMoveScript::MoveNormalInput()
{
	//if (KEY_HOLD(KEY::W))
	//{
	//	// 이동
	//	m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::Front);
	//}
	//if (KEY_HOLD(KEY::S))
	//{
	//	m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::Back);
	//}
	//if (KEY_HOLD(KEY::A))
	//{
	//	m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::Left);
	//}
	//if (KEY_HOLD(KEY::D))
	//{
	//	m_PHQ->ChangeMoveDir(CP_FSMScript::ePlayerMoveDir::Right);
	//}
}

void CP_STATEMoveScript::MoveAimInput()
{
}

void CP_STATEMoveScript::MoveCrouchInput()
{
}

void CP_STATEMoveScript::MoveCrouchAimInput()
{
}

void CP_STATEMoveScript::Enter()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();

	if (CP_FSMScript::ePlayerStance::Crouch == curStance
		|| CP_FSMScript::ePlayerStance::CrouchAim == curStance)
		m_PHQ->PlayAnimation(AnimMoveCrouch, true);
	else
		m_PHQ->PlayAnimation(AnimMoveWalk, true);
}

void CP_STATEMoveScript::Exit()
{
}