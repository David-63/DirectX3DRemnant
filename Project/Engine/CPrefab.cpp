#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"
#include "CPathMgr.h"
#include "CComponent.h"
#include "CScript.h"
#include "Script\CScriptMgr.h"
#include "CResMgr.h"

CPrefab::CPrefab()
	: CRes(RES_TYPE::PREFAB)
	, m_ProtoObj(nullptr)
{
}

CPrefab::~CPrefab()
{
	if (nullptr != m_ProtoObj)
	{		
		delete m_ProtoObj;
		m_ProtoObj = nullptr;
	}
}

CGameObject* CPrefab::Instantiate(Vec3 _pos, int _layerIdx)
{
	CGameObject* clone = m_ProtoObj->Clone();

	clone->Transform()->SetRelativePos(_pos);
	clone->SetLayerIdx(_layerIdx);

	if (clone->RigidBody())
	{
		ACTOR_TYPE type = clone->RigidBody()->GetActorType();
		clone->RigidBody()->SetPhysical(type);
		clone->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
		clone->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
		clone->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
	}
	

	return clone;
}

int CPrefab::SaveGameObject(CGameObject* _Object, const wstring& _strRelativePath)
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _strRelativePath;

	m_ProtoObj = _Object;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	if (nullptr == pFile)
		return E_FAIL;

	SaveGameObject(m_ProtoObj, pFile);

	fclose(pFile);

	return S_OK;
}

void CPrefab::RegisterProtoObject(CGameObject* _Proto)
{
	m_ProtoObj = _Proto;
}

int CPrefab::Save(const wstring& _strRelativePath)
{
	//wstring strPath = CPathMgr::GetInst()->GetContentPath();
	//strPath += _strRelativePath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _strRelativePath.c_str(), L"wb");

	if (nullptr == pFile)
		return E_FAIL;

	SaveGameObject(m_ProtoObj, pFile);

	fclose(pFile);

	return S_OK;
}

int CPrefab::Load(const wstring& _strFilePath)
{
	//wstring strPath = CPathMgr::GetInst()->GetContentPath();
	//strPath += _strFilePath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	if (nullptr == pFile)
		return E_FAIL;

	m_ProtoObj = nullptr;
	m_ProtoObj = LoadGameObject(pFile);

	fclose(pFile);

	return S_OK;
}

int CPrefab::PrefabLoad(const wstring& _strFilePath)
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _strFilePath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	if (nullptr == pFile)
		return E_FAIL;

	m_ProtoObj = nullptr;
	m_ProtoObj = LoadGameObject(pFile);

	fclose(pFile);

	return S_OK;
}

void CPrefab::FabClear()
{
	delete m_ProtoObj;
	m_ProtoObj = nullptr;
}

void CPrefab::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	// 이름
	SaveWString(_Object->GetName(), _File);

	// 컴포넌트
	for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (i == (UINT)COMPONENT_TYPE::END)
		{
			// 컴포넌트 타입 저장
			fwrite(&i, sizeof(UINT), 1, _File);
			break;
		}

		CComponent* Com = m_ProtoObj->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == Com)
			continue;

		// 컴포넌트 타입 저장
		fwrite(&i, sizeof(UINT), 1, _File);

		// 컴포넌트 정보 저장
		Com->SaveToLevelFile(_File);
	}

	// 스크립트	
	const vector<CScript*>& vecScript = m_ProtoObj->GetScripts();
	size_t ScriptCount = vecScript.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
		SaveWString(ScriptName, _File);
		vecScript[i]->SaveToLevelFile(_File);
	}


	// 자식 오브젝트
	const vector<CGameObject*>& vecChild = m_ProtoObj->GetChild();
	size_t ChildCount = vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		SaveGameObject(vecChild[i], _File);
	}

}

CGameObject* CPrefab::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// 이름
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// 컴포넌트
	while (true)
	{
		UINT ComponentType = 0;
		fread(&ComponentType, sizeof(UINT), 1, _File);

		// 컴포넌트 정보의 끝을 확인
		if ((UINT)COMPONENT_TYPE::END == ComponentType)
			break;

		CComponent* Component = nullptr;

		switch ((COMPONENT_TYPE)ComponentType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			Component = new CTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			Component = new CCollider3D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			Component = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			Component = new CAnimator3D;
			break;
		case COMPONENT_TYPE::LIGHT2D:
			Component = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			Component = new CLight3D;
			break;
		case COMPONENT_TYPE::CAMERA:
			Component = new CCamera;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			Component = new CMeshRender;
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			Component = new CParticleSystem;
			break;
		case COMPONENT_TYPE::TILEMAP:
			Component = new CTileMap;
			break;
		case COMPONENT_TYPE::LANDSCAPE:
			Component = new CLandScape;
			break;
		case COMPONENT_TYPE::SKYBOX:
			Component = new CSkyBox;
			break;
		case COMPONENT_TYPE::DECAL:
			Component = new CDecal;
			break;
		case COMPONENT_TYPE::RIGIDBODY:
			Component = new CRigidBody;
			break;

		}

		Component->LoadFromLevelFile(_File);
		pObject->AddComponent(Component);
	}

	// 스크립트	
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName;
		LoadWString(ScriptName, _File);
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pObject->AddComponent(pScript);
		pScript->LoadFromLevelFile(_File);
	}

	// 자식 오브젝트		
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		CGameObject* ChildObject = LoadGameObject(_File);
		pObject->AddChild(ChildObject);
	}

	return pObject;
}
