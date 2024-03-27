#include "pch.h"
#include "CM_Spider_StatesScript.h"
#include "CM_Spider_FSMScript.h"
#include "Engine/CLevelMgr.h"
#include "Engine\CLevel.h"


CM_Spider_StatesScript::CM_Spider_StatesScript()
	: m_StateType(eM_A_States::END)
{
	SetName(L"CM_Spider_StateScript");
}

CM_Spider_StatesScript::~CM_Spider_StatesScript()
{
}
void CM_Spider_StatesScript::begin()
{
	m_MHQ = dynamic_cast<CM_Spider_FSMScript*>(m_FSMHQ);
}

void CM_Spider_StatesScript::SaveToScene(FILE* _pFile)
{
}

void CM_Spider_StatesScript::LoadFromScene(FILE* _pFile)
{
}


