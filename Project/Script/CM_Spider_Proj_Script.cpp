#include "pch.h"
#include "CM_Spider_Proj_Script.h"
#include "Engine/CCollider3D.h"
#include "Engine/CRigidBody.h"
#include "Engine/CParticleSystem.h"

CM_Spider_Proj_Script::CM_Spider_Proj_Script()
	: CScript((UINT)SCRIPT_TYPE::M_SPIDER_PROJ_SCRIPT)
{
	GetOwner()->AddComponent(new CCollider3D);
	GetOwner()->AddComponent(new CRigidBody);

	tShapeInfo info = {};
	info.eGeomType = GEOMETRY_TYPE::Sphere;
	info.size = Vector3(10.f, 1.f, 2.f);
	GetOwner()->RigidBody()->PushBackShapeInfo(info);

	
}

CM_Spider_Proj_Script::~CM_Spider_Proj_Script()
{
}

void CM_Spider_Proj_Script::begin()
{
}

void CM_Spider_Proj_Script::tick()
{
}

void CM_Spider_Proj_Script::BeginOverlap(CCollider3D* _Other)
{
}

void CM_Spider_Proj_Script::OnOverlap(CCollider3D* _Other)
{
}

void CM_Spider_Proj_Script::EndOverlap(CCollider3D* _Other)
{
}
