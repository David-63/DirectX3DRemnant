#include "pch.h"
#include "CM_Lurker_FSMScript.h"
#include "CM_Lurker_StatesScript.h"

CM_Lurker_FSMScript::CM_Lurker_FSMScript()
{
	SetScriptType(SCRIPT_TYPE::M_LURKER_FSMSCRIPT);
}

CM_Lurker_FSMScript::~CM_Lurker_FSMScript()
{
}

void CM_Lurker_FSMScript::begin()
{
	m_tMonsterInfo.M_Health.MaxHp = 1000.f;
	m_tMonsterInfo.M_Health.CurHp = 1000.f;

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
	GetOwner()->Animator3D()->Add(Lurker_MoveFront);
	GetOwner()->Animator3D()->Add(Lurker_Death);
	GetOwner()->Animator3D()->Add(Lurker_DeathGround);

	GetOwner()->Animator3D()->Add(Lurker_ImpactLegs);
	GetOwner()->Animator3D()->Add(Lurker_ImpactLegL);
	GetOwner()->Animator3D()->Add(Lurker_ImpactLegR);
	GetOwner()->Animator3D()->Add(Lurker_ImpactTorso);
	GetOwner()->Animator3D()->Add(Lurker_ImpactFront);



	//局聪刚历 积己秦具窃
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_IDLE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_MELEE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_MOVE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_DAMAGED_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_DEAD_SCRIPT)));


	ChangeState(static_cast<UINT>(eLurkerState::Idle));
	//ChangeState(static_cast<UINT>(eLurkerState::Move));


}

void CM_Lurker_FSMScript::tick()
{
	CC_FSMScript::tick();
	DeathCheck();


	if (KEY_TAP(KEY::O))
	{
		ChangeState((UINT)eLurkerState::Damaged);
	}
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

void CM_Lurker_FSMScript::DeathCheck()
{
	if (m_tMonsterInfo.M_Health.CurHp <= 0)
		ChangeState(static_cast<UINT>(eLurkerState::Dead));
}

void CM_Lurker_FSMScript::SaveToScene(FILE* _pFile)
{
}

void CM_Lurker_FSMScript::LoadFromScene(FILE* _pFile)
{
}


