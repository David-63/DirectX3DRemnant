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
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::B_STATEIDLESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::B_STATEMOVESCRIPT)));

	ChangeState(static_cast<UINT>(eB_States::IDLE));
}

void CB_FSMScript::tick()
{
	CC_FSMScript::tick();

}

void CB_FSMScript::BeginOverlap(CCollider3D* _Other)
{
	// 보스는 tTimeCtrl 을 갖고 있어야함. 
	// 보스 회피 (플레이어의 조준점에 다다랐고 쿨타임이 끝났을 때)
	// 이떄의 조준점이 보스를 향했는지 체크하는 부분은 피직스의 RayCast함수에서 hit가 true이면으로 계산하면 될듯? 

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
