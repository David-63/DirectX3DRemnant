#include "pch.h"
#include "CB_STATEIdleScript.h"

CB_STATEIdleScript::CB_STATEIdleScript()
{
	SetStateType(static_cast<UINT>(eB_States::IDLE));
}

CB_STATEIdleScript::~CB_STATEIdleScript()
{
}


void CB_STATEIdleScript::Enter()
{
}


void CB_STATEIdleScript::tick()
{
	if(KEY_TAP(KEY::_5))
	{
		m_BHQ->ChangeState(static_cast<UINT>(eB_States::MOVE));
	}

}


void CB_STATEIdleScript::Exit()
{




}
