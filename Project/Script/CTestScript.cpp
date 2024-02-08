#include "pch.h"
#include "CTestScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator3D.h>
#include <Engine\CCamera.h>
#include <Engine\CEngine.h>

CTestScript::CTestScript()
	: CScript((UINT)SCRIPT_TYPE::TESTSCRIPT)
{
}

CTestScript::~CTestScript()
{
}


void CTestScript::begin()
{
	//Animator3D()->StartEvent(L"firstAnim") = std::bind(&CTestScript::Start, this);
	//Animator3D()->CompleteEvent(L"firstAnim") = std::bind(&CTestScript::Complete, this);
	//Animator3D()->EndEvent(L"firstAnim") = std::bind(&CTestScript::End, this);
	
	//Animator3D()->ActionEvent(L"firstAnim", 50) = std::bind(&CTestScript::Action, this);
}

void CTestScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();
	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);

	// 현재 마우스 위치 얻기
	POINT mousePos;
	GetCursorPos(&mousePos);
	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();

	// 화면 중앙 좌표 계산
	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	// 마우스의 상대적 이동량 계산
	int deltaX = mousePos.x - centerX;
	int deltaY = mousePos.y - centerY;

	float fSpeed = 100.f;
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

}

void CTestScript::SaveToLevelFile(FILE* _File)
{
}

void CTestScript::LoadFromLevelFile(FILE* _FILE)
{
}


