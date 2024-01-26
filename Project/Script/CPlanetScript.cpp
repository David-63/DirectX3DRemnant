#include "pch.h"
#include "CPlanetScript.h"
#include <Engine/CTimeMgr.h>
#include "Engine/CGameObject.h"

CPlanetScript::CPlanetScript() : CScript((UINT)SCRIPT_TYPE::PLANETSCRIPT)
{
}

CPlanetScript::~CPlanetScript()
{
}

void CPlanetScript::begin()
{
	Vec3 pos = Transform()->GetRelativePos();
	radius = pos.x;
}

void CPlanetScript::tick()
{
	accTime += DT;
	if (isSatellite)
	{
		doSatellite();
	}
	else
	{
		if (isOrbit)
			orbit();

		if (isRotate)
			rotationPlanet();
	}
}

void CPlanetScript::orbit()
{
	// �̰� ���� (�¾� �߽� ȸ�� , ������ ��Ȳ�� �ٸ���?)
	
	// ���غ���
	Vec3 unit = Vec3(0.f, 1.f, 0.f);

	// ȸ����
	float rotAngle = accTime * moveSpeed * 0.7f;

	// ȸ�� ��ġ ���ϱ�
	XMMATRIX rotationMatrix = XMMatrixRotationZ(rotAngle);
	XMVECTOR vectorToRotate = XMLoadFloat3(&unit);
	XMVECTOR rotatedVector = XMVector3TransformCoord(vectorToRotate, rotationMatrix);
	Vec3 result;
	XMStoreFloat3(&result, rotatedVector);
	result.Normalize();
	result *= radius;
	Transform()->SetRelativePos(result);

}

void CPlanetScript::rotationPlanet()
{
	// �̰� ���� (�ڱ� �߽� ȸ��)
	Vec3 rot = Transform()->GetRelativeRot();
	rot.y += DT * rotateSpeed;
	Transform()->SetRelativeRot(rot);
}

void CPlanetScript::doSatellite()
{
	CGameObject* me = GetOwner();
	CGameObject* parent = me->GetParent();
	// ���� ��ǥ
	Vec3 center = parent->Transform()->GetRelativePos();

	// ���غ���
	Vec3 unit = Vec3(0.f, 1.f, 0.f);

	// ȸ����
	float rotAngle = accTime * moveSpeed * 0.7f;

	// ȸ�� ��ġ ���ϱ�
	XMMATRIX rotationMatrix = XMMatrixRotationX(rotAngle);	// ȸ���� ��Ʈ����
	XMVECTOR vectorToRotate = XMLoadFloat3(&unit);			// �������͸� float3 Ÿ������ ��ȯ
	XMVECTOR rotatedVector = XMVector3TransformCoord(vectorToRotate, rotationMatrix); // ȸ���� ���
	Vec3 result; // ���������� ���� ȸ������
	XMStoreFloat3(&result, rotatedVector);
	result.Normalize();
	result *= moonDistance;		// ���⿡ ���� ���ؼ� �̵���ġ ����
	center += result;		// ���������� �̵��� ��ġ�� �������κ��� �̵���ġ�� ���Ѱ�?

	Transform()->SetRelativePos(result);
}

void CPlanetScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&isOrbit, sizeof(bool), 1, _File);
	fwrite(&radius, sizeof(float), 1, _File);
	fwrite(&moveSpeed, sizeof(float), 1, _File);
	fwrite(&isRotate, sizeof(bool), 1, _File);
	fwrite(&rotateSpeed, sizeof(float), 1, _File);
	fwrite(&accTime, sizeof(float), 1, _File);
	fwrite(&isSatellite, sizeof(bool), 1, _File);
}

void CPlanetScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&isOrbit, sizeof(bool), 1, _FILE);
	fread(&radius, sizeof(float), 1, _FILE);
	fread(&moveSpeed, sizeof(float), 1, _FILE);
	fread(&isRotate, sizeof(bool), 1, _FILE);
	fread(&rotateSpeed, sizeof(float), 1, _FILE);
	fread(&accTime, sizeof(float), 1, _FILE);
	fread(&isSatellite, sizeof(bool), 1, _FILE);
}



