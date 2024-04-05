#include "pch.h"
#include "CHitBoxScript.h"
#include "Engine/CLevelMgr.h"
#include "Engine/CLevel.h"
#include "Engine/CRigidBody.h"
#include "CHitBoxInfoScript.h"

CHitBoxScript::CHitBoxScript()
	: CScript(SCRIPT_TYPE::HITBOXSCRIPT)
	, m_iCurIdx(0)
{
	SetName(L"HitBoxScript");
	
}

CHitBoxScript::~CHitBoxScript()
{
	for (CGameObject* obj : m_vGameObjects)
	{
		if (nullptr != obj)
		{
			DestroyObject(obj);
		}
	}
}

void CHitBoxScript::begin()
{
}

void CHitBoxScript::tick()
{
	m_vOwnerPos = GetOwner()->Transform()->GetRelativePos();
	m_vOwnerRot = GetOwner()->Transform()->GetRelativeRot();
	m_vOwnerFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	m_vOwnerRight = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	for (int i = 0; i < m_vGameObjects.size(); ++i)
	{
		m_vFinalPos = m_vOwnerPos;
		m_vFinalPos.y += m_vOffsets[i].y;
		m_vFinalPos += m_vOwnerFront * m_vOffsets[i].z;
		m_vFinalPos += m_vOwnerRight * m_vOffsets[i].x;

		m_vGameObjects[i]->Transform()->SetRelativePos(m_vFinalPos);
		m_vGameObjects[i]->Transform()->SetRelativeRot(m_vOwnerRot);
	}
}

void CHitBoxScript::MakeHitBox(bool _isPlayer, Vec3 _size, Vec3 _posOffset, tHitInfo _hitInfo)
{
	CGameObject* obj = new CGameObject();
	obj->SetName(L"HitBoxObject");
	obj->AddComponent(new CTransform);
	obj->AddComponent(new CCollider3D);
	obj->AddComponent(new CMeshRender);
	obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_Debug"));
	obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

	if (_isPlayer)
		obj->SetLayerIdx((UINT)LAYER_TYPE::HitBoxPlayer);
	else
		obj->SetLayerIdx((UINT)LAYER_TYPE::HitBoxMonster);

	obj->Transform()->SetRelativePos(_posOffset);

	obj->AddComponent(new CRigidBody);
	tShapeInfo info = {};
	info.eGeomType = GEOMETRY_TYPE::Box;
	info.size = _size;
	obj->RigidBody()->PushBackShapeInfo(info);

	obj->RigidBody()->SetPhysical(ACTOR_TYPE::Kinematic);
	obj->RigidBody()->SetSleep(true);
	obj->RigidBody()->SetDrawCollider(false);

	if (_isPlayer)
		CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(obj, (UINT)LAYER_TYPE::HitBoxPlayer, true);
	else
		CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(obj, (UINT)LAYER_TYPE::HitBoxMonster, true);

	obj->AddComponent(new CHitBoxInfoScript);
	obj->GetScript<CHitBoxInfoScript>()->SetHitInfo(_hitInfo);

	obj->SetReserver(GetOwner());
	
	m_vGameObjects.push_back(obj);
	m_vOffsets.push_back(_posOffset);
}

void CHitBoxScript::SetSleep(int _idx)
{
	m_vGameObjects[_idx]->RigidBody()->SetSleep(true);
	m_vGameObjects[_idx]->MeshRender()->SetRenderOff(true);
}

void CHitBoxScript::SetAwake(int _idx)
{
	m_vGameObjects[_idx]->RigidBody()->SetSleep(false);
	m_vGameObjects[_idx]->MeshRender()->SetRenderOff(false);
	m_iCurIdx = _idx;
}

void CHitBoxScript::BeginOverlap(CCollider3D* _Other)
{
	int a = 0;
}

void CHitBoxScript::OnOverlap(CCollider3D* _Other)
{

}

void CHitBoxScript::EndOverlap(CCollider3D* _Other)
{
	int a = 0;
}



