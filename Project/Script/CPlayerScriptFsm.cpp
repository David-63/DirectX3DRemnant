#include "pch.h"
#include "CPlayerScriptFsm.h"
#include "CIdleStateScript.h"


CPlayerScriptFsm::CPlayerScriptFsm()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPTFSM)
{
}

CPlayerScriptFsm::~CPlayerScriptFsm()
{
	for (const auto& pair : m_State)
	{
		if (nullptr != pair.second)
			delete pair.second;
	}

	m_State.clear();
}


void CPlayerScriptFsm::begin()
{
	//GetOwner()->Transform(); 

	AddState(new CIdleStateScript);

	m_CurState = GetStateScript(CStateScript::eState::IDLE);
	m_CurState->Enter();

}

void CPlayerScriptFsm::tick()
{

	m_CurState->tick();

	// 마무리 짓기 전에 현재 상태였던 걸 이전 상태에다가 저장해둠 
	m_PrevState = m_CurState->GetStateType();
}

void CPlayerScriptFsm::BeginOverlap(CCollider3D* _Other)
{
}

void CPlayerScriptFsm::OnOverlap(CCollider3D* _Other)
{
}

void CPlayerScriptFsm::EndOverlap(CCollider3D* _Other)
{
}

void CPlayerScriptFsm::AddState(CStateScript* _State)
{
	if (nullptr == _State)
		return;

	_State->begin();
	CStateScript::eState eState = _State->GetStateType();
	m_State.insert(std::make_pair(eState, _State));


}

CStateScript* CPlayerScriptFsm::GetStateScript(CStateScript::eState _State)
{
	std::map<CStateScript::eState, CStateScript*>::iterator iter = m_State.find(_State);

	if (iter != m_State.end())
		return iter->second;

	return nullptr;
}

void CPlayerScriptFsm::ChangeState(CStateScript::eState _NextState)
{
	CStateScript* pNextState = GetStateScript(_NextState);
	if (pNextState->GetStateType() == m_CurState->GetStateType())
		return;

	m_CurState->Exit();
	m_PrevState = m_CurState->GetStateType();
	m_CurState = pNextState;
	m_CurState->Enter();
}

void CPlayerScriptFsm::SaveToScene(FILE* _pFile)
{
}

void CPlayerScriptFsm::LoadFromScene(FILE* _pFile)
{
}

