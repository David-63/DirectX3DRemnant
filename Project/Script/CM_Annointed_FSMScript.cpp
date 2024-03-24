#include "pch.h"
#include "CM_Annointed_FSMScript.h"
#include "CM_Annointed_StatesScript.h"

CM_Annointed_FSMScript::CM_Annointed_FSMScript()
{
}

CM_Annointed_FSMScript::~CM_Annointed_FSMScript()
{
}

void CM_Annointed_FSMScript::begin()
{
	m_tMonsterInfo.M_Health.MaxHp = 1000.f;
	m_tMonsterInfo.M_Health.CurHp = 1000.f;

	//GetOwner()->Animator3D()->Add(Lurker_ALERT01);




	//局聪刚历 积己秦具窃
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_ANNOINTED_STATE_IDLE_SCRIPT)));
	//AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_ANNOINTED_STATE_MELEE_SCRIPT)));
	//AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_ANNOINTED_STATE_MOVE_SCRIPT)));
	//AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_ANNOINTED_STATE_DAMAGED_SCRIPT)));
	//AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_ANNOINTED_STATE_DEAD_SCRIPT)));


	ChangeState(static_cast<UINT>(eAnnointedState::Idle));
	//ChangeState(static_cast<UINT>(eAnnointedState::Move));
}

void CM_Annointed_FSMScript::tick()
{
	CC_FSMScript::tick();
	DeathCheck();

	if (KEY_TAP(KEY::O))
	{
		ChangeState((UINT)eAnnointedState::Damaged);
	}
}

void CM_Annointed_FSMScript::BeginOverlap(CCollider3D* _Other)
{
}

void CM_Annointed_FSMScript::OnOverlap(CCollider3D* _Other)
{
}

void CM_Annointed_FSMScript::EndOverlap(CCollider3D* _Other)
{
}

void CM_Annointed_FSMScript::DeathCheck()
{
	if (m_tMonsterInfo.M_Health.CurHp <= 0)
		ChangeState(static_cast<UINT>(eAnnointedState::Dead));
}

void CM_Annointed_FSMScript::SaveToScene(FILE* _pFile)
{
}

void CM_Annointed_FSMScript::LoadFromScene(FILE* _pFile)
{
}


