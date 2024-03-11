#include "pch.h"
#include "CCharacterMoveScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>



CCharacterMoveScript::CCharacterMoveScript()
	: CScript((UINT)SCRIPT_TYPE::CHARACTERMOVESCRIPT)

{
}

CCharacterMoveScript::~CCharacterMoveScript()
{
}

void CCharacterMoveScript::begin()
{
}

void CCharacterMoveScript::tick()
{
	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	float fSpeed = 10000.f;


	if (KEY_HOLD(KEY::UP))
	{
		vMoveVector += DT * vFront * fSpeed;
	}

	if (KEY_HOLD(KEY::DOWN))
	{
		vMoveVector -= DT * vFront * fSpeed;
	}

	if (KEY_HOLD(KEY::LEFT))
	{
		vMoveVector -= DT * vRight * fSpeed;
	}

	if (KEY_HOLD(KEY::RIGHT))
	{
		vMoveVector += DT * vRight * fSpeed;
	}

	GetOwner()->RigidBody()->SetVelocity(vMoveVector);
	vMoveVector = Vec3(0.f, 0.f, 0.f);
}

void CCharacterMoveScript::BeginOverlap(CCollider3D* _Other)
{
}

void CCharacterMoveScript::OnOverlap(CCollider3D* _Other)
{
}

void CCharacterMoveScript::EndOverlap(CCollider3D* _Other)
{
}

void CCharacterMoveScript::SaveToLevelFile(FILE* _File)
{
}

void CCharacterMoveScript::LoadFromLevelFile(FILE* _FILE)
{
}
