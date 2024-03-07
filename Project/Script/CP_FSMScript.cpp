#include "pch.h"
#include "CP_FSMScript.h"

CP_FSMScript::CP_FSMScript()
{
}

CP_FSMScript::~CP_FSMScript()
{
}

void CP_FSMScript::begin()
{
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEIDLESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEMOVESCRIPT)));

	ChangeState(static_cast<UINT>(eP_States::IDLE));
}

void CP_FSMScript::BeginOverlap(CCollider3D* _Other)
{
}

void CP_FSMScript::OnOverlap(CCollider3D* _Other)
{
}

void CP_FSMScript::EndOverlap(CCollider3D* _Other)
{
}


void CP_FSMScript::SaveToScene(FILE* _pFile)
{
}

void CP_FSMScript::LoadFromScene(FILE* _pFile)
{
}

