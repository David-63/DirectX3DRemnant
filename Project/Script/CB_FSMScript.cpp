#include "pch.h"
#include "CB_FSMScript.h"

CB_FSMScript::CB_FSMScript()
{
}

CB_FSMScript::~CB_FSMScript()
{
}

void CB_FSMScript::begin()
{
	//AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEIDLESCRIPT)));
	//AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEMOVESCRIPT)));

	ChangeState(static_cast<UINT>(eB_States::IDLE));
}

void CB_FSMScript::tick()
{
	CC_FSMScript::tick();


}

void CB_FSMScript::BeginOverlap(CCollider3D* _Other)
{
}

void CB_FSMScript::OnOverlap(CCollider3D* _Other)
{
}

void CB_FSMScript::EndOverlap(CCollider3D* _Other)
{
}

void CB_FSMScript::SaveToScene(FILE* _pFile)
{
}

void CB_FSMScript::LoadFromScene(FILE* _pFile)
{
}
