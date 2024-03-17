#include "pch.h"
#include "CB_STATEIdleScript.h"
#include "CB_FSMScript.h"

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
	if(KEY_TAP(KEY::_5)) // 플레이어가 충돌체에 부딪치면 그 때 move로 전환한다. 라고 나중에 바꿀 것임 (충돌체 이벤트 함수 쪽에서) 
	{
		m_BHQ->ChangeState(static_cast<UINT>(eB_States::MOVE));
	}

}


void CB_STATEIdleScript::Exit()
{




}
