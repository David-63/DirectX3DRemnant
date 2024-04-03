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
		DestroyObject(GetOwner());
	}
}
