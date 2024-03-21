#include "pch.h"
#include "CP_STATEMoveScript.h"
#include <Engine/CTransform.h>
#include "CP_FSMScript.h"

CP_STATEMoveScript::CP_STATEMoveScript() : m_isMelee(false), m_isFront(true)
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

	// 방향 입력
	if (KEY_TAP(KEY::W))
	{
		m_PHQ->InputMove(0, 1.f);
	}
	if (KEY_TAP(KEY::S))
	{
		m_PHQ->InputMove(0, -1.f);
	}
	if (KEY_TAP(KEY::A))
	{
		m_PHQ->InputMove(-1.f, 0);
	}
	if (KEY_TAP(KEY::D))
	{
		m_PHQ->InputMove(1.f, 0);
	}
	if (KEY_RELEASE(KEY::W))
	{
		m_PHQ->InputMove(0, -1.f);
	}
	if (KEY_RELEASE(KEY::S))
	{
		m_PHQ->InputMove(0, 1.f);
	}
	if (KEY_RELEASE(KEY::D))
	{
		m_PHQ->InputMove(-1.f, 0);
	}
	if (KEY_RELEASE(KEY::A))
	{
		m_PHQ->InputMove(1.f, 0);
	}

	Vec2 moveDir = m_PHQ->GetMoveDir();
	// 앞 뒤 구분
	if (0 <= moveDir.y)
		m_isFront = true;
	else
		m_isFront = false;

	if (moveDir != m_prevDir)
	{
		m_prevDir = moveDir;
		CallAnimation();
	}
	// 이동
	TranslateInput();

	// 애니메이션을 변경해주려면 이동 방향이 바뀐걸 확인 할 수 있어야함

	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();

	if (CP_FSMScript::ePlayerStance::Aim == curStance)
	{
		CP_FSMScript::tP_LongGunInfo* gun = m_PHQ->GetLongGunInfo();

		if (gun->IsAble())
		{
			if (KEY_TAP(KEY::LBTN))
			{
				gun->Fire();
			}
		}
	}


	// 이동량이 없으면 Idle로 변경
	if ((0.1 >= moveDir.y && -0.1 <= moveDir.y)
		&& (0.1 >= moveDir.x && -0.1 <= moveDir.x))
		m_PHQ->ChangeState(static_cast<UINT>(eP_States::IDLE));

	// 스테이트 변경 : MELEE
	// 스테이트 변경 : DAMAGED
}

void CP_STATEMoveScript::CallAnimation()
{
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();

	// 현재 방향 구하기

	if (m_isFront)
	{
		if (CP_FSMScript::ePlayerStance::Crouch == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_FR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_FL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch, true);
		}
		else if (CP_FSMScript::ePlayerStance::Aim == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_FR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_FL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkAim, true);
		}
		else if (CP_FSMScript::ePlayerStance::Normal == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalk_FR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalk_FL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalk, true);
		}
	}
	else
	{
		if (CP_FSMScript::ePlayerStance::Crouch == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_BR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_BL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkCrouch_B, true);
		}
		else if (CP_FSMScript::ePlayerStance::Aim == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_BR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_BL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalkAim_B, true);
		}
		else if (CP_FSMScript::ePlayerStance::Normal == curStance)
		{
			if (0.3 <= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalk_BR, true);
			else if (-0.3 >= m_prevDir.x)
				m_PHQ->PlayAnimation(P_R2MoveWalk_BL, true);
			else
				m_PHQ->PlayAnimation(P_R2MoveWalk_B, true);
		}
	}
}

void CP_STATEMoveScript::MoveKeyInput()
{
	
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
		moveMagnitude = playerInfo.P_Stat.MoveSpeed * ScaleDT * 0.1f;
	
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
		vCurPos += vRight * moveMagnitude;
	}
	if (KEY_HOLD(KEY::D))
	{
		vCurPos -= vRight * moveMagnitude;
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
}

void CP_STATEMoveScript::Exit()
{
	//m_PHQ->ClearMoveDir();
	m_prevDir = Vec2(0, 0);
}