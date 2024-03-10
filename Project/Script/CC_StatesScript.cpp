#include "pch.h"
#include "CC_StatesScript.h"
#include "CC_FSMScript.h"


CC_StatesScript::CC_StatesScript()
	: CScript((UINT)SCRIPT_TYPE::C_STATESSCRIPT)
	, m_StateType(0)
{
	SetName(L"C_StateScript");
}

CC_StatesScript::~CC_StatesScript()
{
}

void CC_StatesScript::SetHQ(CC_FSMScript* _hq)
{
	m_FSMHQ = _hq;
}