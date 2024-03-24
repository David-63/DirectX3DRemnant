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

		/*matRot = Matrix::CreateFromQuaternion({ transform.q.x, transform.q.y, transform.q.z, transform.q.w });
		m_vRelativeRot.x = atan2f(matRot._32, matRot._33);
		m_vRelativeRot.y = atan2f(-matRot._31, sqrtf(matRot._32 * matRot._32 + matRot._33 * matRot._33));
		m_vRelativeRot.z = atan2f(matRot._21, matRot._11);*/
		

		auto quat = Quaternion::CreateFromYawPitchRoll(m_vRelativeRot.y, m_vRelativeRot.x, m_vRelativeRot.z);
		matRot = Matrix::CreateFromQuaternion(quat);

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
	if (isnan(_vRot.y))
		_vRot.y = 0.f;

	if (_vRot.y > 2 * 3.141592 || _vRot.y < -2 * 3.141592)
	{
		_vRot.y = fmod(_vRot.y, 2 * 3.141592);
	}

	m_vRelativeRot = _vRot;

	auto quat = Quaternion::CreateFromYawPitchRoll((m_vRelativeRot.y)
												 , (m_vRelativeRot.x)
										         , (m_vRelativeRot.z));

	if (IsPhysicsObject())
	{
		PxTransform tr = GetOwner()->RigidBody()->GetPhysicsTransform();
		tr.q.x = quat.x;
		tr.q.y = quat.y;
		tr.q.z = quat.z;
		tr.q.w = quat.w;

		GetOwner()->RigidBody()->GetRigidActor()->setGlobalPose(tr);
	}
}
