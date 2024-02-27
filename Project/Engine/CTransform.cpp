#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"


using namespace physx;

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bAbsolute(false)
	, m_vRelativeDir{ Vec3(1.f, 0.f, 0.f) , Vec3(0.f, 1.f, 0.f) , Vec3(0.f, 0.f, 1.f) } // R, U, F
	, m_DebugSphereUse(false)
	, m_SphereRadius(50.f)
{
	SetName(L"Transform");
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	Matrix matRot = {};
	if (IsPhysicsObject())
	{
		physx::PxTransform transform = GetOwner()->RigidBody()->GetPhysicsTransform();

		m_matWorldScale = XMMatrixIdentity();
		m_matWorldScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

		Matrix matTranslation = Matrix::CreateTranslation({ transform.p.x, transform.p.y, transform.p.z });
		m_vRelativePos.x = transform.p.x;
		m_vRelativePos.y = transform.p.y;
		m_vRelativePos.z = transform.p.z;

		matRot = Matrix::CreateFromQuaternion({ transform.q.x, transform.q.y, transform.q.z, transform.q.w });
		m_vRelativeRot.x = atan2f(matRot._32, matRot._33);
		m_vRelativeRot.y = atan2f(-matRot._31, sqrtf(matRot._32 * matRot._32 + matRot._33 * matRot._33));
		m_vRelativeRot.z = atan2f(matRot._21, matRot._11);
		
		//auto quat = Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(m_vRelativeRot.y), XMConvertToRadians(m_vRelativeRot.x), XMConvertToRadians(m_vRelativeRot.z));
		//matRot = Matrix::CreateFromQuaternion(quat);

		m_matWorld = m_matWorldScale * matRot * matTranslation;
	}
	else
	{
		m_matWorldScale = XMMatrixIdentity();
		m_matWorldScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

		matRot = XMMatrixIdentity();
		matRot = XMMatrixRotationX(m_vRelativeRot.x);
		matRot *= XMMatrixRotationY(m_vRelativeRot.y);
		matRot *= XMMatrixRotationZ(m_vRelativeRot.z);

		Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);


		m_matWorld = m_matWorldScale * matRot * matTranslation;
	}

	Vec3 vDefaultDir[3] = {
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		m_vWorldDir[i] = m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], matRot);
	}

	// 상속구조에 맞춰서 크기와 위치를 세팅함 ( 회전 X )
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		if (m_bAbsolute)
		{
			Matrix matParentWorld = pParent->Transform()->m_matWorld;
			Matrix matParentScale = pParent->Transform()->m_matWorldScale;
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			// 월드 = 로컬월드 * 부모크기 역 * 부모 월드(크기/회전/이동)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			m_matWorldScale = pParent->Transform()->m_matWorldScale;
			m_matWorld *= pParent->Transform()->m_matWorld;
		}

		// 3방향 벡터 업데이트
		for (int i = 0; i < 3; ++i)
		{
			m_vWorldDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorld);
			m_vWorldDir[i].Normalize();
		}
	}

	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);

	if (m_DebugSphereUse)
	{
		//float scaledRadius = m_SphereRadius * max(m_vRelativeScale.x, max(m_vRelativeScale.y, m_vRelativeScale.z));
		// RealativePos를 처음에 넣어줬는데, 애초에 개념을 잘못 이해하고 있었음.
		// RelatviePos를 여기다가 넣어봤자 어차피 월드행렬의 것을 쓰고 있기 때문에 소용 없음
		// RelatviePos는 단순히 상대적인 개념이라 여기다가 넣는게 아니다. 
		DrawDebugSphere(GetWorldPos(), m_SphereRadius, Vec4(0.f, 1.f, 3.f, 1.f), Vec3(0.f, 0.f, 0.f), 0.f, true);
	}
}

void CTransform::UpdateData()
{
	// 위치값을 상수버퍼에 전달 및 바인딩		
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_transform.matWorld = m_matWorld;
	g_transform.matWorldInv = m_matWorldInv;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;


	pTransformBuffer->SetData(&g_transform);
	pTransformBuffer->UpdateData();
}

void CTransform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRot, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
	fwrite(&m_DebugSphereUse, sizeof(bool), 1, _File);
	fwrite(&m_SphereRadius, sizeof(float), 1, _File);
}

void CTransform::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_vRelativePos, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeRot, sizeof(Vec3), 1, _FILE);
	fread(&m_bAbsolute, sizeof(bool), 1, _FILE);
	fread(&m_DebugSphereUse, sizeof(bool), 1, _FILE);
	fread(&m_SphereRadius, sizeof(float), 1, _FILE);
}

XMFLOAT4 CTransform::EulerToQuat(float x, float y, float z)
{
	// 오일러 각도를 라디안으로 변환합니다 (만약 x, y, z가 이미 라디안 단위라면 이 단계는 생략합니다).
	float pitch = XMConvertToRadians(x); // X 축 회전 (상하 회전)
	float yaw = XMConvertToRadians(y);   // Y 축 회전 (좌우 회전)
	float roll = XMConvertToRadians(z);  // Z 축 회전 (틸트 회전)

	// 쿼터니언 생성
	XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

	// 쿼터니언을 XMVECTOR 타입에서 XMFLOAT4 타입으로 변환하여 사용할 수 있습니다.
	XMFLOAT4 quaternionFloat4;
	XMStoreFloat4(&quaternionFloat4, quaternion);

	return quaternionFloat4;
}

Vec3 CTransform::QuatToEuler(const XMFLOAT4& quaternion, Vec3& euler)
{
	// 쿼터니언을 XMVECTOR로 변환
	XMVECTOR quat = XMLoadFloat4(&quaternion);

	// Yaw (Y 축), Pitch (X 축), Roll (Z 축) 계산
	float yaw, pitch, roll;
	pitch = atan2(2.0f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z), 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y));
	yaw = asin(2.0f * (quaternion.w * quaternion.y - quaternion.z * quaternion.x));
	roll = atan2(2.0f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y), 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z));

	// 라디안을 도(degree)로 변환
	euler.x = XMConvertToDegrees(pitch);
	euler.y = XMConvertToDegrees(yaw);
	euler.z = XMConvertToDegrees(roll);


	return euler;
}

void CTransform::Move(const Vector3& _velocity)
{
	if (true == IsPhysicsObject())
	{
		CRigidBody* rigidBody = GetOwner()->RigidBody();
		physx::PxTransform transform = rigidBody->GetPhysicsTransform();

		transform.p.x += _velocity.x * CTimeMgr::GetInst()->GetDeltaTime();
		transform.p.y += _velocity.y * CTimeMgr::GetInst()->GetDeltaTime();
		transform.p.z += _velocity.z * CTimeMgr::GetInst()->GetDeltaTime();

		ACTOR_TYPE eActorType = rigidBody->GetActorType();

		if (ACTOR_TYPE::Kinematic == eActorType)
			rigidBody->GetDynamicActor()->setKinematicTarget(transform);
		else if (ACTOR_TYPE::Dynamic == eActorType)
			rigidBody->GetDynamicActor()->setGlobalPose(transform);
		else
			AssertEx(false, L"Transform::Move() - Static Actor에 대한 Move 호출");
	}

	else
	{
		m_vRelativePos.x += _velocity.x * CTimeMgr::GetInst()->GetDeltaTime();
		m_vRelativePos.y += _velocity.y * CTimeMgr::GetInst()->GetDeltaTime();
		m_vRelativePos.z += _velocity.z * CTimeMgr::GetInst()->GetDeltaTime();
	}
}

physx::PxVec3 CTransform::GetPhysicsPosition()
{
	CRigidBody* rigidBody = GetOwner()->RigidBody();
	physx::PxTransform transform = rigidBody->GetPhysicsTransform();
	physx::PxVec3 physxPos = transform.p;
	return physxPos;
}

void CTransform::SetRelativePos(Vec3 _vPos)
{
	m_vRelativePos = _vPos;

	if (IsPhysicsObject())
	{
		PxTransform tr = GetOwner()->RigidBody()->GetPhysicsTransform();
		tr.p.x = m_vRelativePos.x;
		tr.p.y = m_vRelativePos.y;
		tr.p.z = m_vRelativePos.z;

		GetOwner()->RigidBody()->SetPhysicsTransform(tr);
	}
}

void CTransform::SetRelativeRot(Vec3 _vRot)
{
	m_vRelativeRot = _vRot;

	auto quat = Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(m_vRelativeRot.y)
												 , XMConvertToRadians(m_vRelativeRot.x)
										         , XMConvertToRadians(m_vRelativeRot.z));

	if (IsPhysicsObject())
	{
		PxTransform tr = GetOwner()->RigidBody()->GetPhysicsTransform();
		tr.q.x = quat.x;
		tr.q.y = quat.y;
		tr.q.z = quat.z;
		tr.q.w = quat.w;

		GetOwner()->RigidBody()->SetPhysicsTransform(tr);
	}
}
