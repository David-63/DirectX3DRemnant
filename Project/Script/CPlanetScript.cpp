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
	// 이건 공전 (태양 중심 회전 , 위성은 상황이 다를듯?)
	
	// 기준벡터
	Vec3 unit = Vec3(0.f, 1.f, 0.f);

	// 회전량
	float rotAngle = accTime * moveSpeed * 0.7f;

	// 회전 위치 구하기
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
	// 이건 자전 (자기 중심 회전)
	Vec3 rot = Transform()->GetRelativeRot();
	rot.y += DT * rotateSpeed;
	Transform()->SetRelativeRot(rot);
}

void CPlanetScript::doSatellite()
{
	CGameObject* me = GetOwner();
	CGameObject* parent = me->GetParent();
	// 원점 좌표
	Vec3 center = parent->Transform()->GetRelativePos();

	// 기준벡터
	Vec3 unit = Vec3(0.f, 1.f, 0.f);

	// 회전량
	float rotAngle = accTime * moveSpeed * 0.7f;

	// 회전 위치 구하기
	XMMATRIX rotationMatrix = XMMatrixRotationX(rotAngle);	// 회전량 메트릭스
	XMVECTOR vectorToRotate = XMLoadFloat3(&unit);			// 단위벡터를 float3 타입으로 변환
	XMVECTOR rotatedVector = XMVector3TransformCoord(vectorToRotate, rotationMatrix); // 회전각 계산
	Vec3 result; // 최종적으로 구한 회전벡터
	XMStoreFloat3(&result, rotatedVector);
	result.Normalize();
	result *= moonDistance;		// 방향에 길이 곱해서 이동위치 구함
	center += result;		// 최종적으로 이동할 위치는 기준으로부터 이동위치를 더한값?

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



