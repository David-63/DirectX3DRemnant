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
		
		auto quat = Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(m_vRelativeRot.y), XMConvertToRadians(m_vRelativeRot.x), XMConvertToRadians(m_vRelativeRot.z));
		matRot = Matrix::CreateFromQuaternion(quat);

		m_matWorld = m_matWorldScale * matRot * matTranslation;
		//m_matWorld = m_matWorldScale * matTranslation;
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

	// ��ӱ����� ���缭 ũ��� ��ġ�� ������ ( ȸ�� X )
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		if (m_bAbsolute)
		{
			Matrix matParentWorld = pParent->Transform()->m_matWorld;
			Matrix matParentScale = pParent->Transform()->m_matWorldScale;
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			// ���� = ���ÿ��� * �θ�ũ�� �� * �θ� ����(ũ��/ȸ��/�̵�)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			m_matWorldScale = pParent->Transform()->m_matWorldScale;
			m_matWorld *= pParent->Transform()->m_matWorld;
		}

		// 3���� ���� ������Ʈ
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
		// RealativePos�� ó���� �־���µ�, ���ʿ� ������ �߸� �����ϰ� �־���.
		// RelatviePos�� ����ٰ� �־���� ������ ��������� ���� ���� �ֱ� ������ �ҿ� ����
		// RelatviePos�� �ܼ��� ������� �����̶� ����ٰ� �ִ°� �ƴϴ�. 
		DrawDebugSphere(GetWorldPos(), m_SphereRadius, Vec4(0.f, 1.f, 3.f, 1.f), Vec3(0.f, 0.f, 0.f), 0.f, true);
	}
}

void CTransform::UpdateData()
{
	// ��ġ���� ������ۿ� ���� �� ���ε�		
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
			AssertEx(false, L"Transform::Move() - Static Actor�� ���� Move ȣ��");
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
