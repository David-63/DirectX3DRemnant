#include "pch.h"
#include "CB_StateScript.h"
#include "CB_FSMScript.h"


CB_StateScript::CB_StateScript()
	: m_BStateType(eB_States::END)
{
	SetName(L"CB_StatesScript");
}


CB_StateScript::~CB_StateScript()
{
}


void CB_StateScript::begin()
{
	m_BHQ = dynamic_cast<CB_FSMScript*>(m_FSMHQ);
}


void CB_StateScript::SaveToScene(FILE* _pFile)
{
}

void CB_StateScript::LoadFromScene(FILE* _pFile)
{
}
