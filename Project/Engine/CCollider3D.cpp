#include "pch.h"
#include "CCollider3D.h"
#include "CResMgr.h"
#include "CScript.h"


CCollider3D::CCollider3D()
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	  , mType(COLLIDER3D_TYPE::NONE)
	  , mOtherNormal(Vec3{0.0f,0.f,0.f})
{
}

CCollider3D::CCollider3D(const CCollider3D& _collider)
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	  , mType(_collider.mType)
{
	mMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh");
	mMaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl");
}

CCollider3D::~CCollider3D()
{
}

void CCollider3D::SaveToLevelFile(FILE* _File)
{
	fwrite(&mType, sizeof(COLLIDER3D_TYPE), 1, _File);
}

void CCollider3D::LoadFromLevelFile(FILE* _File)
{
	fread(&mType, sizeof(COLLIDER3D_TYPE), 1, _File);
}

void CCollider3D::begin()
{
	AssertEx(IsPhysicsObject(), L"Collider::Initialize() - 충돌을 사용하기 위해서는 RigidBody->SetPhysical()가 선행되어야 함.");
	

}

void CCollider3D::tick()
{
}

void CCollider3D::finaltick()
{
}

void CCollider3D::OnCollisionEnter(CCollider3D* _otherCollider)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(_otherCollider);
	}
}

void CCollider3D::OnCollisionStay(CCollider3D* _otherCollider)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnOverlap(_otherCollider);
	}
}

void CCollider3D::OnCollisionExit(CCollider3D* _otherCollider)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(_otherCollider);
	}
}

void CCollider3D::OnTriggerEnter(CCollider3D* _otherCollider)
{
	if (GetOwner()->GetName() == L"HitBoxObject")
	{
		CGameObject* hitBox = GetOwner();

		const vector<CScript*>& vecScript = hitBox->GetScripts();

		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			vecScript[i]->BeginOverlap(_otherCollider);
		}
	}
	else
	{
		const vector<CScript*>& vecScript = GetOwner()->GetScripts();

		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			vecScript[i]->BeginOverlap(_otherCollider);
		}
	}
}

void CCollider3D::OnTriggerStay(CCollider3D* _otherCollider)
{
	if (GetOwner()->GetName() == L"HitBoxObject")
	{
		CGameObject* hitBox = GetOwner();

		const vector<CScript*>& vecScript = hitBox->GetScripts();

		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			vecScript[i]->OnOverlap(_otherCollider);
		}
	}
	else
	{
		const vector<CScript*>& vecScript = GetOwner()->GetScripts();

		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			vecScript[i]->OnOverlap(_otherCollider);
		}
	}
}

void CCollider3D::OnTriggerExit(CCollider3D* _otherCollider)
{
	if (GetOwner()->GetName() == L"HitBoxObject")
	{
		CGameObject* hitBox = GetOwner();

		const vector<CScript*>& vecScript = hitBox->GetScripts();

		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			vecScript[i]->EndOverlap(_otherCollider);
		}

	}
	else
	{
		const vector<CScript*>& vecScript = GetOwner()->GetScripts();

		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			vecScript[i]->EndOverlap(_otherCollider);
		}
	}
}
