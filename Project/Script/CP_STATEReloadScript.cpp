#include "pch.h"
#include "CP_STATEReloadScript.h"
#include "CP_FSMScript.h"
#include <Engine/CKeyMgr.h>

CP_STATEReloadScript::CP_STATEReloadScript()
{
	SetStateType(static_cast<UINT>(eP_States::RELOAD));
}

CP_STATEReloadScript::~CP_STATEReloadScript()
{
}

void CP_STATEReloadScript::tick()
{
	if (KEY_TAP(KEY::W))
	{
		m_PHQ->InputMove(0, 1.f);
	}
	if (KEY_TAP(KEY::S))
	{
		m_PHQ->InputMove(0, -1.f);
	}
	if (KEY_TAP(KEY::A))
	{
		m_PHQ->InputMove(-1.f, 0);
	}
	if (KEY_TAP(KEY::D))
	{
		m_PHQ->InputMove(1.f, 0);
	}
	if (KEY_RELEASE(KEY::W))
	{
		m_PHQ->InputMove(0, -1.f);
	}
	if (KEY_RELEASE(KEY::S))
	{
		m_PHQ->InputMove(0, 1.f);
	}
	if (KEY_RELEASE(KEY::D))
	{
		m_PHQ->InputMove(-1.f, 0);
	}
	if (KEY_RELEASE(KEY::A))
	{
		m_PHQ->InputMove(1.f, 0);
	}
}

void CP_STATEReloadScript::Enter()
{
}

void CP_STATEReloadScript::Exit()
{
}
