#include "pch.h"
#include "CC_FSMScript.h"

CC_FSMScript::CC_FSMScript()
    : CScript((UINT)SCRIPT_TYPE::C_FSMSCRIPT)
    , m_CurState(nullptr), m_PrevState(0)
{
}

CC_FSMScript::~CC_FSMScript()
{
	for (const auto& pair : m_States)
	{
		if (nullptr != pair.second)
			delete pair.second;
	}

	m_States.clear();
}

void CC_FSMScript::begin()
{
	// 이거 애매하네
	//AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::IDLESTATESCRIPT)));

	//m_CurState = FindStateScript(STATE_IDLE);
	//ChangeState(m_CurState->GetStateType());
}

void CC_FSMScript::tick()
{
	if (!m_CurState)
		return;

	m_CurState->tick();
	//m_PrevState = m_CurState->GetStateType(); 이건 어쩌피 ChangeState 가 호출되면 알아서 세팅됨
}

void CC_FSMScript::BeginOverlap(CCollider3D* _Other)
{
}

void CC_FSMScript::OnOverlap(CCollider3D* _Other)
{
}

void CC_FSMScript::EndOverlap(CCollider3D* _Other)
{
}

void CC_FSMScript::AddState(CC_StatesScript* _State)
{
	if (nullptr == _State)
		return;
	_State->SetHQ(this);
	_State->begin();
	UINT eState = _State->GetStateType();
	m_States.insert(std::make_pair(eState, _State));
}

CC_StatesScript* CC_FSMScript::FindStateScript(UINT _State)
{
	std::map<UINT, CC_StatesScript*>::iterator iter = m_States.find(_State);

	if (iter != m_States.end())
		return iter->second;

	return nullptr;
}

void CC_FSMScript::ChangeState(UINT _NextState)
{
	CC_StatesScript* inState = FindStateScript(_NextState);
	// 인자로 들어온 스크립트가 없으면 터짐
	assert(inState);

	// 현재 상태가 없는 경우
	if (!m_CurState)
	{
		m_CurState = inState;
		m_CurState->Enter();
		return;
	}
	// 다음 상태가 지금과 같은 경우
	CC_StatesScript* pNextState = inState;
	if (pNextState->GetStateType() == m_CurState->GetStateType())
		return;

	m_CurState->Exit();
	m_PrevState = m_CurState->GetStateType();
	m_CurState = pNextState;
	m_CurState->Enter();
}


void CC_FSMScript::SaveToScene(FILE* _pFile)
{
}

void CC_FSMScript::LoadFromScene(FILE* _pFile)
{
}


