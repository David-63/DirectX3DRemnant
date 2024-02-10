#include "pch.h"
#include "CCharacterMoveScript.h"

CCharacterMoveScript::CCharacterMoveScript()
	: CScript((UINT)SCRIPT_TYPE::CHARACTERMOVESCRIPT)
	, m_vDir{}
	, m_fHzInput(0.f)
	, m_fVertiInput(0.f)

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

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = 500.f;

	
	//Vec3 Dir = vFront * m_fHzInput + vRight * m_fVertiInput;

	if (KEY_PRESSED(KEY::W))
	{

		vPos += DT * vRight * fSpeed;
		
	}

	if (KEY_PRESSED(KEY::S))
	{

		vPos -= DT * vRight * fSpeed;
		
	}

	if (KEY_PRESSED(KEY::A))
	{

		vPos += DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos -= DT * vFront * fSpeed;
	
	}

	// 키를 동시에 눌렀을 때 대각선으로 이동하면서도 속도가 빨라지는 것을 방지하기 위해 정규화
	// 그리고 y축은 움직이지 않도록 0의 값을 부여한다.
	Vector3 direction = Vector3(m_fVertiInput, 0.f, m_fHzInput).Normalize();

	// 유니티의 magnitude = length함수
	if (direction.Length() >= 0.1f)
	{
		// 음수 값이 나올 것을 걱정하지 않아도 되는 이유는 위에서 정규화를 했기 때문에
		// 항상 양의 값만 나온다. 
		vPos = move(direction * fSpeed * DT);

	}

	Transform()->SetRelativePos(vPos);

}
