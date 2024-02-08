#include "pch.h"
#include "CTestScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator3D.h>
#include <Engine\CCamera.h>
#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>


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
	


	// 기준 좌표
	vPos;
	Vec3(100, 200 + 45.f, 0);

	//상대적 기준 좌표
	Vec3 pivot(0.f, 45.f, -30.f);

	// 카메라의 기준 좌표 : 캐릭터의 좌표 + 피벗 좌표
	vPos += pivot;

	// 마우스 회전에 따른 카메라의 좌표 이동시키기
	deltaX;
	// 카메라 rot + deltaX + sensitive 값을 radian으로 변환







}

void CTestScript::SaveToLevelFile(FILE* _File)
{
}

void CTestScript::LoadFromLevelFile(FILE* _FILE)
{
}


