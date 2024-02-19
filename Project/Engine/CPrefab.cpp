#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"
#include "CPathMgr.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CLight3D.h"
#include "CAnimator3D.h"
#include "CRigidBody.h"
#include "CCollider3D.h"
#include "CMeshRender.h"
#include "CParticleSystem.h"
#include "CDecal.h"

CPrefab::CPrefab()
	: CRes(RES_TYPE::PREFAB)
	, m_ProtoObj(nullptr)
{
}

CPrefab::~CPrefab()
{
	if (nullptr != m_ProtoObj)
		delete m_ProtoObj;
}

CGameObject* CPrefab::Instantiate()
{
	return m_ProtoObj->Clone();	
}

void CPrefab::RegisterProtoObject(CGameObject* _Proto)
{
	// 원본 오브젝트는 레벨 소속이 아니여야 한다.
	//assert(-1 == _Proto->GetLayerIndex());

	m_ProtoObj = _Proto;
}

int CPrefab::Save(const wstring& _strRelativePath)
{
	SetRelativePath(_strRelativePath);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;
	FILE* pFile = nullptr;

	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	std::vector<int> vec((UINT)COMPONENT_TYPE::END);
	vec = m_ProtoObj->GetIsComponentArr();
	fwrite(vec.data(), vec.size() * sizeof(int), 1, pFile);

	if (m_ProtoObj->Transform())
		m_ProtoObj->Transform()->SaveToLevelFile(pFile);

	if (m_ProtoObj->Camera())
		m_ProtoObj->Camera()->SaveToLevelFile(pFile);

	if (m_ProtoObj->Light3D())
		m_ProtoObj->Light3D()->SaveToLevelFile(pFile);

	if (m_ProtoObj->RigidBody())
		m_ProtoObj->RigidBody()->SaveToLevelFile(pFile);

	if (m_ProtoObj->Animator3D())
		m_ProtoObj->Animator3D()->SaveToLevelFile(pFile);

	if (m_ProtoObj->Collider3D())
		m_ProtoObj->Collider3D()->SaveToLevelFile(pFile);

	if (m_ProtoObj->MeshRender())
		m_ProtoObj->MeshRender()->SaveToLevelFile(pFile);

	if (m_ProtoObj->ParticleSystem())
		m_ProtoObj->ParticleSystem()->SaveToLevelFile(pFile);

	if (m_ProtoObj->Decal())
		m_ProtoObj->Decal()->SaveToLevelFile(pFile);

	fclose(pFile);
	return S_OK;
}

int CPrefab::Load(const wstring& _strFilePath)
{
	SetRelativePath(_strFilePath);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strFilePath;
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);

	m_ProtoObj = new CGameObject;

	std::vector<int> CompIsvec((UINT)COMPONENT_TYPE::END);
	fread(CompIsvec.data(), CompIsvec.size() *sizeof(int), 1, pFile);

	if (CompIsvec[(UINT)COMPONENT_TYPE::TRANSFORM])
	{
		m_ProtoObj->AddComponent(new CTransform);
		m_ProtoObj->Transform()->LoadFromLevelFile(pFile);
	}
	if (CompIsvec[(UINT)COMPONENT_TYPE::CAMERA])
	{
		m_ProtoObj->AddComponent(new CCamera);
		m_ProtoObj->Camera()->LoadFromLevelFile(pFile);
	}
	if (CompIsvec[(UINT)COMPONENT_TYPE::LIGHT3D])
	{
		m_ProtoObj->AddComponent(new CLight3D);
		m_ProtoObj->Light3D()->LoadFromLevelFile(pFile);
	}
	if (CompIsvec[(UINT)COMPONENT_TYPE::RIGIDBODY])
	{
		m_ProtoObj->AddComponent(new CRigidBody);
		m_ProtoObj->RigidBody()->LoadFromLevelFile(pFile);
	}
	if (CompIsvec[(UINT)COMPONENT_TYPE::ANIMATOR3D])
	{
		m_ProtoObj->AddComponent(new CAnimator3D);
		m_ProtoObj->Animator3D()->LoadFromLevelFile(pFile);
	}
	if (CompIsvec[(UINT)COMPONENT_TYPE::COLLIDER3D])
	{
		m_ProtoObj->AddComponent(new CCollider3D);
		m_ProtoObj->Collider3D()->LoadFromLevelFile(pFile);
	}

	if (CompIsvec[(UINT)COMPONENT_TYPE::MESHRENDER])
	{
		m_ProtoObj->AddComponent(new CMeshRender);
		m_ProtoObj->MeshRender()->LoadFromLevelFile(pFile);
	}

	if (CompIsvec[(UINT)COMPONENT_TYPE::PARTICLESYSTEM])
	{
		m_ProtoObj->AddComponent(new CParticleSystem);
		m_ProtoObj->ParticleSystem()->LoadFromLevelFile(pFile);
	}

	if (CompIsvec[(UINT)COMPONENT_TYPE::DECAL])
	{
		m_ProtoObj->AddComponent(new CDecal);
		m_ProtoObj->Decal()->LoadFromLevelFile(pFile);
	}

	fclose(pFile);
	return S_OK;
}