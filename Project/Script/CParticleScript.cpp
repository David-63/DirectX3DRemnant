#include "pch.h"
#include "CParticleScript.h"

CParticleScript::CParticleScript()
	: CScript((UINT)SCRIPT_TYPE::PARTICLESCRIPT)
{
}

CParticleScript::~CParticleScript()
{
}


void CParticleScript::begin()

{
}

void CParticleScript::tick()
{
	// test���Դϴ�
}

void CParticleScript::SaveToLevelFile(FILE* _File)
{
}

void CParticleScript::LoadFromLevelFile(FILE* _FILE)
{
}


