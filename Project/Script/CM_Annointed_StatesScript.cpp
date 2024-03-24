#include "pch.h"
#include "CM_Annointed_StatesScript.h"
#include "CM_Annointed_FSMScript.h"
#include "Engine/CLevelMgr.h"
#include "Engine\CLevel.h"


CM_Annointed_StatesScript::CM_Annointed_StatesScript()
	: m_StateType(eM_A_States::END)
{
	SetName(L"CM_Annointed_StateScript");
}

CM_Annointed_StatesScript::~CM_Annointed_StatesScript()
{
}
void CM_Annointed_StatesScript::begin()
{
	m_MHQ = dynamic_cast<CM_Annointed_FSMScript*>(m_FSMHQ);
}

void CM_Annointed_StatesScript::SaveToScene(FILE* _pFile)
{
}

void CM_Annointed_StatesScript::LoadFromScene(FILE* _pFile)
{
}


