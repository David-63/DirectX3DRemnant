#include "pch.h"
#include "CHitBoxInfoScript.h"
#include "CC_FSMScript.h"


CHitBoxInfoScript::CHitBoxInfoScript()
	:CScript((UINT)SCRIPT_TYPE::HITBOXINFOSCRIPT)
{
}

CHitBoxInfoScript::~CHitBoxInfoScript()
{
}

void CHitBoxInfoScript::begin()
{
}

void CHitBoxInfoScript::tick()
{
}

void CHitBoxInfoScript::BeginOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == (UINT)LAYER_TYPE::Player)
	{
		_Other->GetOwner()->GetScript<CC_FSMScript>()->GiveAtkInfo(m_HitInfo);
	}
}

void CHitBoxInfoScript::OnOverlap(CCollider3D* _Other)
{
}

void CHitBoxInfoScript::EndOverlap(CCollider3D* _Other)
{
}



