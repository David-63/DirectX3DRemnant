#include "pch.h"
#include "CFSM.h"
#include "CState.h"
#include "CAnimator3D.h"

CFSM::CFSM()
	: m_pObject(nullptr)
	, m_pCurState(nullptr)
{
}

CFSM::~CFSM()
{

	for (const auto& iter : m_mapState)
	{
		delete iter.second;
	}
}



void CFSM::tick()
{
	if (nullptr != m_pCurState)
		m_pCurState->tick();
}

void CFSM::finaltick()
{
	if(nullptr != m_pCurState)
		m_pCurState->finaltick();
}

void CFSM::AddState(wstring _strStateName, CState* _pState)
{
	CState* pState = FindState(_strStateName);
	assert(nullptr == pState);

	_pState->SetStateName(_strStateName);
	_pState->SetFSM(this);
	
	m_mapState.insert(make_pair(_strStateName, _pState));
}

CState* CFSM::FindState(wstring _strStateName)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_strStateName);

	if (m_mapState.end() == iter)
	{
		return nullptr;
	}

	return iter->second;
}

void CFSM::ChangeState(wstring _strNextStateName, const wstring& _AniName, bool _Stay)
{
	CState* pNextState = FindState(_strNextStateName);
	assert(pNextState);

	if (nullptr != m_pCurState)
		m_pCurState->Exit(); // 이전 상태 종료하면서 처리할 일

	
	m_pCurState = pNextState;

	m_pCurState->Enter(); //현재 상태로 돌입할때 처리할 일 


	// 애니메이션 

}

