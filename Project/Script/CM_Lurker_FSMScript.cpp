#include "pch.h"
#include "CM_Lurker_FSMScript.h"
#include "CM_Lurker_StatesScript.h"

void CM_Lurker_FSMScript::begin()
{
	GetOwner()->Animator3D()->Add(Lurker_ALERT01);
	GetOwner()->Animator3D()->Add(Lurker_ALERT03);
	GetOwner()->Animator3D()->Add(Lurker_IDLESTAND);
	GetOwner()->Animator3D()->Add(Lurker_IDLEDOWN);
	GetOwner()->Animator3D()->Add(Lurker_IDLEDOWNDOO);
	GetOwner()->Animator3D()->Add(Lurker_WALL_EMERGE);
	GetOwner()->Animator3D()->Add(Lurker_WALL_EMERGE2);
	GetOwner()->Animator3D()->Add(Lurker_EMERGE);
	GetOwner()->Animator3D()->Add(Lurker_Turn180);
	GetOwner()->Animator3D()->Add(Lurker_Turn90L);
	GetOwner()->Animator3D()->Add(Lurker_Turn90R);
	GetOwner()->Animator3D()->Add(Lurker_Turn135L);
	GetOwner()->Animator3D()->Add(Lurker_Turn135R);
	GetOwner()->Animator3D()->Add(Lurker_Dash);
	GetOwner()->Animator3D()->Add(Lurker_Heavy1);
	GetOwner()->Animator3D()->Add(Lurker_SlashR);
	GetOwner()->Animator3D()->Add(Lurker_SlashRCombo);


	//局聪刚历 积己秦具窃
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_IDLE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_MELEE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_MOVE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_DAMAGED_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_DEAD_SCRIPT)));





	//ChangeState(static_cast<UINT>(eLurkerState::Idle));
	ChangeState(static_cast<UINT>(eLurkerState::Melee));
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
