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
	// ���� ���콺 ��ġ ���
	POINT mousePos;
	GetCursorPos(&mousePos);
	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();

	// ȭ�� �߾� ��ǥ ���
	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	// ���콺�� ����� �̵��� ���
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
	


	// ���� ��ǥ
	vPos;
	Vec3(100, 200 + 45.f, 0);

	//����� ���� ��ǥ
	Vec3 pivot(0.f, 45.f, -30.f);

	// ī�޶��� ���� ��ǥ : ĳ������ ��ǥ + �ǹ� ��ǥ
	vPos += pivot;

	// ���콺 ȸ���� ���� ī�޶��� ��ǥ �̵���Ű��
	deltaX;
	// ī�޶� rot + deltaX + sensitive ���� radian���� ��ȯ







}

void CTestScript::SaveToLevelFile(FILE* _File)
{
}

void CTestScript::LoadFromLevelFile(FILE* _FILE)
{
}


