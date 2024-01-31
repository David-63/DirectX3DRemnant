#include "pch.h"
#include "CTestScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator3D.h>

CTestScript::CTestScript()
	: CScript((UINT)SCRIPT_TYPE::TESTSCRIPT)
{
}

CTestScript::~CTestScript()
{
}


void CTestScript::begin()
{
	Animator3D()->StartEvent(L"firstAnim") = std::bind(&CTestScript::Start, this);
	Animator3D()->CompleteEvent(L"firstAnim") = std::bind(&CTestScript::Complete, this);
	Animator3D()->EndEvent(L"firstAnim") = std::bind(&CTestScript::End, this);

	Animator3D()->ActionEvent(L"firstAnim", 50) = std::bind(&CTestScript::Action, this);
}

void CTestScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();
	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	float fSpeed = 200.f;
	if (KEY_PRESSED(KEY::UP))
	{
		vPos += DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		vPos -= DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos -= DT * vRight * fSpeed;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos += DT * vRight * fSpeed;
	}
	Transform()->SetRelativePos(vPos);
	// Event Test

}

void CTestScript::SaveToLevelFile(FILE* _File)
{
}

void CTestScript::LoadFromLevelFile(FILE* _FILE)
{
}


