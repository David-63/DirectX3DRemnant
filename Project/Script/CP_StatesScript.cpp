#include "pch.h"
#include "CP_StatesScript.h"
#include "CP_FSMScript.h"

CP_StatesScript::CP_StatesScript() : m_PStateType(eP_States::END)
{
	SetName(L"CP_StatesScript");
}

CP_StatesScript::~CP_StatesScript()
{
}

void CP_StatesScript::begin()
{
	m_PHQ = dynamic_cast<CP_FSMScript*>(m_FSMHQ);
	m_Gun = m_PHQ->GetLongGunInfo();
	m_PlayerStance = m_PHQ->GetStance();
	m_PlayerMoveDir = m_PHQ->GetMoveDir();
	m_PlayerInfo = m_PHQ->GetPlayerInfo();
}

void CP_StatesScript::SaveToScene(FILE* _pFile)
{
}

void CP_StatesScript::LoadFromScene(FILE* _pFile)
{
}