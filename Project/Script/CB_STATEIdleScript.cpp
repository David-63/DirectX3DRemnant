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
	if(KEY_TAP(KEY::_5)) // �÷��̾ �浹ü�� �ε�ġ�� �� �� move�� ��ȯ�Ѵ�. ��� ���߿� �ٲ� ���� (�浹ü �̺�Ʈ �Լ� �ʿ���) 
	{
		m_BHQ->ChangeState(static_cast<UINT>(eB_States::MOVE));
	}

}


void CB_STATEIdleScript::Exit()
{




}
