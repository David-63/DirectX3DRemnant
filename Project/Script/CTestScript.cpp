#include "pch.h"
#include "CTestScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator3D.h>
#include <Engine\CCamera.h>
#include <Engine\CEngine.h>
#include <Engine\CCollisionMgr.h>

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
	Vec3 vInitPos = vPos;
	Vec3 vRot = Transform()->GetRelativeRot();
	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	float fSpeed = 100.f;

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

	if (KEY_TAP(KEY::UP) || KEY_TAP(KEY::DOWN) || KEY_TAP(KEY::LEFT) || KEY_TAP(KEY::RIGHT)
		|| KEY_RELEASE(KEY::UP) || KEY_RELEASE(KEY::DOWN) || KEY_RELEASE(KEY::LEFT) || KEY_RELEASE(KEY::RIGHT))
	{
		mDir = { 0.f,0.f,0.f };
	}
	

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

	if (KEY_PRESSED(KEY::UP))
	{
		mDir += DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		mDir -= DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		mDir -= DT * vRight * fSpeed;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		mDir += DT * vRight * fSpeed;
	}
	

	if (mWallCol)
		return;

	Transform()->SetRelativePos(vPos);

}

void CTestScript::BeginOverlap(CCollider3D* _Other)
{
	if (_Other->GetType() == COLLIDER3D_TYPE::Wall)
	{
		mWallCol = true;
		mDir = { 0.f,0.f,0.f };
	}
}

void CTestScript::OnOverlap(CCollider3D* _Other)
{
	if (_Other->GetType() == COLLIDER3D_TYPE::Wall)
	{
		Vec3 normal = _Other->GetOtherNormal();
		float result = mDir.Dot(-normal);

		if (mDir == Vec3(0.f, 0.f, 0.f))
			return;

		if (result > 0)
			mWallCol = true;
		else
			mWallCol = false;
	}
}

void CTestScript::EndOverlap(CCollider3D* _Other)
{
	if (_Other->GetType() == COLLIDER3D_TYPE::Wall)
	{
		mWallCol = false;
		_Other->SetOhterNormal(Vec3(0.f, 0.f, 0.f));
		mDir = { 0.f,0.f,0.f };
	}
}



void CTestScript::SaveToLevelFile(FILE* _File)
{
}

void CTestScript::LoadFromLevelFile(FILE* _FILE)
{
}


