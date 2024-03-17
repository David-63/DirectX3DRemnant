#include "pch.h"
#include "CHitBoxScript.h"
#include "Engine/CLevelMgr.h"
#include "Engine/CLevel.h"
#include "Engine/CRigidBody.h"

CHitBoxScript::CHitBoxScript()
	: CScript(SCRIPT_TYPE::HITBOXSCRIPT)
	, m_iCurIdx(0)
{
	SetName(L"HitBoxScript");
	
}

CHitBoxScript::~CHitBoxScript()
{
}

void CHitBoxScript::begin()
{
}

void CHitBoxScript::tick()
{
	int a = m_vGameObjects.size();
}

void CHitBoxScript::MakeHitBox(bool _isPlayer, Vec3 _size, Vec3 _posOffset)
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
	
	GetOwner()->AddChild(obj);
	m_vGameObjects.push_back(obj);
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



