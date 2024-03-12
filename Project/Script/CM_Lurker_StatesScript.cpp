#include "pch.h"
#include "CM_Lurker_StatesScript.h"
#include "CM_Lurker_FSMScript.h"

CM_Lurker_StatesScript::CM_Lurker_StatesScript()
	: m_StateType(eM_States::END)
{
	SetName(L"CM_Lurker_StateScript");
}

CM_Lurker_StatesScript::~CM_Lurker_StatesScript()
{
}
void CM_Lurker_StatesScript::begin()
{
	m_MHQ = dynamic_cast<CM_Lurker_FSMScript*>(m_FSMHQ);
}

void CM_Lurker_StatesScript::SaveToScene(FILE* _pFile)
{
}

void CM_Lurker_StatesScript::LoadFromScene(FILE* _pFile)
{
}


