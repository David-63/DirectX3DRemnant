#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>

CCameraMoveScript::CCameraMoveScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT)
	, m_fCamSpeed(1000.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fCamSpeed, "camera Speed");

}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::tick()
{
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
		Camera2DMove();
	else
		Camera3DMove();
}

void CCameraMoveScript::Camera2DMove()
{
	// 키 입력에 따른 카메라 이동
	Vec3 vPos = Transform()->GetRelativePos();

	float fSpeed = m_fCamSpeed;
	if (KEY_HOLD(KEY::LSHIFT))
		fSpeed *= 5.f;

	if (KEY_HOLD(KEY::W))
	{
		vPos.y += DT * fSpeed;
	}

	if (KEY_HOLD(KEY::S))
	{
		vPos.y -= DT * fSpeed;
	}

	if (KEY_HOLD(KEY::A))
	{
		vPos.x -= DT * fSpeed;
	}

	if (KEY_HOLD(KEY::D))
	{
		vPos.x += DT * fSpeed;
	}



	if (KEY_HOLD(KEY::_1))
	{
		float fScale = Camera()->GetScale();
		fScale += DT * 1.f;
		Camera()->SetScale(fScale);
	}

	if (KEY_HOLD(KEY::_2))
	{
		float fScale = Camera()->GetScale();
		fScale -= DT * 1.f;
		Camera()->SetScale(fScale);
	}

	Transform()->SetRelativePos(vPos);
}

void CCameraMoveScript::Camera3DMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = m_fCamSpeed;

	//if (KEY_HOLD(KEY::LSHIFT))
	//	fSpeed *= 10.f;

	if (KEY_HOLD(KEY::UP))
	{
		vPos += DT * vFront * fSpeed;
	}

	if (KEY_HOLD(KEY::DOWN))
	{
		vPos -= DT * vFront * fSpeed;
	}

	if (KEY_HOLD(KEY::LEFT))
	{
		vPos -= DT * vRight * fSpeed;
	}

	if (KEY_HOLD(KEY::RIGHT))
	{
		vPos += DT * vRight * fSpeed;
	}

	if (KEY_HOLD(KEY::RBTN))
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += vMouseDir.x * 0.005f;
		vRot.x -= vMouseDir.y * 0.005f;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);

}
