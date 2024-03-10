#include "pch.h"
#include "CStateScript.h"

CStateScript::CStateScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_StateType(eState::END)

{
	SetName(L"StateScript");
}

CStateScript::~CStateScript()
{
}


void CStateScript::begin()
{
	//m_Transform = GetPlayerScript()->GetOwner()->GetComponent<Transform>();
}

void CStateScript::tick()
{
}

void CStateScript::Enter()
{
}

void CStateScript::Exit()
{
}

