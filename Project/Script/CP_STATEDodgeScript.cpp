#include "pch.h"
#include "CP_STATEDodgeScript.h"
#include "CP_FSMScript.h"
#include <Engine/CKeyMgr.h>
CP_STATEDodgeScript::CP_STATEDodgeScript()
{
	SetStateType(static_cast<UINT>(eP_States::DODGE));
}

CP_STATEDodgeScript::~CP_STATEDodgeScript()
{
}

void CP_STATEDodgeScript::tick()
{
	float moveMagnitude = 0.f;
	Vec3 vMoveVector(0.f, 0.f, 0.f);

	Vec3 vFront = m_PHQ->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = m_PHQ->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = m_PHQ->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	// 방향부터 계산

	if (0.3 <= m_evadeDir.x)
		vMoveVector += vRight;
	else if (-0.3 >= m_evadeDir.x)
		vMoveVector -= vRight;
	if (0.3 <= m_evadeDir.y)
		vMoveVector += vFront;
	else
		vMoveVector -= vFront;

	// 상태에 맞게 이동량 계산
	moveMagnitude = m_PlayerInfo->P_Stat.MoveSpeed * ScaleDT;

	// 최종 이동량 계산
	if (m_curTime <= m_maxTime)
	{
		m_curTime += ScaleDT;
	}

	float totalSpeed = FloatLerp(moveMagnitude * 80.f, moveMagnitude, m_curTime / m_maxTime);
	vMoveVector *= totalSpeed;
	m_PHQ->RigidBody()->SetVelocity(vMoveVector);
}

void CP_STATEDodgeScript::Enter()
{
	m_evadeDir = *m_PHQ->GetMoveDir();
	m_maxTime = 1.f;
	m_curTime = 0.f;
}

void CP_STATEDodgeScript::Exit()
{
	m_evadeDir = Vec2(0.f, 0.f);
}
