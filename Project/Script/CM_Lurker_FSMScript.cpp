#include "pch.h"
#include "CM_Lurker_FSMScript.h"

void CM_Lurker_FSMScript::begin()
{
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_SLEEP_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_IDLE_SCRIPT)));

	ChangeState(static_cast<UINT>(eLurkerState::Sleep));
}

void CM_Lurker_FSMScript::tick()
{
	CC_FSMScript::tick();

}

void CM_Lurker_FSMScript::BeginOverlap(CCollider3D* _Other)
{
}

void CM_Lurker_FSMScript::OnOverlap(CCollider3D* _Other)
{
}

void CM_Lurker_FSMScript::EndOverlap(CCollider3D* _Other)
{
}

void CM_Lurker_FSMScript::SaveToScene(FILE* _pFile)
{
}

void CM_Lurker_FSMScript::LoadFromScene(FILE* _pFile)
{
}

CM_Lurker_FSMScript::CM_Lurker_FSMScript()
{
}

CM_Lurker_FSMScript::~CM_Lurker_FSMScript()
{
}
