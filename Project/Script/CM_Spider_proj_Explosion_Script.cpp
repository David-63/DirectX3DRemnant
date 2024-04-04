#include "pch.h"
#include "CM_Spider_proj_Explosion_Script.h"

CM_Spider_proj_Explosion_Script::CM_Spider_proj_Explosion_Script()
	: CScript((UINT)SCRIPT_TYPE::M_SPIDER_PROJ_EXPLOSION_SCRIPT)
	, m_deathTime(0.f)
{
}

CM_Spider_proj_Explosion_Script::~CM_Spider_proj_Explosion_Script()
{
}

void CM_Spider_proj_Explosion_Script::begin()
{
}

void CM_Spider_proj_Explosion_Script::tick()
{
	m_deathTime += DT;

	if (m_deathTime > 1.f)
	{
		Vec3 pos = GetOwner()->Transform()->GetRelativePos();
		pos.y += 10000.f;
		GetOwner()->Transform()->SetRelativePos(pos);
		DestroyObject(GetOwner());
	}
}

void CM_Spider_proj_Explosion_Script::BeginOverlap(CCollider3D* _Other)
{
}

void CM_Spider_proj_Explosion_Script::OnOverlap(CCollider3D* _Other)
{
}

void CM_Spider_proj_Explosion_Script::EndOverlap(CCollider3D* _Other)
{
}
