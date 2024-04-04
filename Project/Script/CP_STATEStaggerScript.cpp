#include "pch.h"
#include "CP_STATEStaggerScript.h"
#include "CP_FSMScript.h"

CP_STATEStaggerScript::CP_STATEStaggerScript()
{
	SetStateType(static_cast<UINT>(eP_States::STAGGER));
}

CP_STATEStaggerScript::~CP_STATEStaggerScript()
{
}


void CP_STATEStaggerScript::tick()
{
	float moveMagnitude = 0.f;
	Vec3 vMoveVector(0.f, 0.f, 0.f);
	Vec3 vFront = m_PHQ->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	if (-1 == m_hitDir)
		vMoveVector += vFront;
	else if (1 == m_hitDir)
		vMoveVector -= vFront;

	moveMagnitude = m_PlayerInfo->P_Stat.MoveSpeed * ScaleDT * 0.6f;
	vMoveVector *= moveMagnitude * 50.f;
	m_PHQ->RigidBody()->SetVelocity(vMoveVector);
}

void CP_STATEStaggerScript::CallAnimation()
{
	int hitDir = m_PHQ->GetHitDir();
	if (-1 == hitDir)
	{
		m_PHQ->PlayAnimation(P_R2ImpactStagger_B, true);
	}
	else if(1 == m_hitDir)
	{
		m_PHQ->PlayAnimation(P_R2ImpactStagger, true);
	}
}

void CP_STATEStaggerScript::Enter()
{
	m_hitDir = m_PHQ->GetHitDir();
	CallAnimation();
}

void CP_STATEStaggerScript::Exit()
{
	m_hitDir = 0;
}

