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

	// Ű�� ���ÿ� ������ �� �밢������ �̵��ϸ鼭�� �ӵ��� �������� ���� �����ϱ� ���� ����ȭ
	// �׸��� y���� �������� �ʵ��� 0�� ���� �ο��Ѵ�.
	Vector3 direction = Vector3(m_fVertiInput, 0.f, m_fHzInput).Normalize();

	// ����Ƽ�� magnitude = length�Լ�
	if (direction.Length() >= 0.1f)
	{
		// ���� ���� ���� ���� �������� �ʾƵ� �Ǵ� ������ ������ ����ȭ�� �߱� ������
		// �׻� ���� ���� ���´�. 
		vPos = move(direction * fSpeed * DT);

	}

	Transform()->SetRelativePos(vPos);

}
