#include "pch.h"
#include "CB_STATEDeadScript.h"
#include "CB_FSMScript.h"


CB_STATEDeadScript::CB_STATEDeadScript()
{
	SetStateType(static_cast<UINT>(eB_States::DEAD));
}

CB_STATEDeadScript::~CB_STATEDeadScript()
{
}

void CB_STATEDeadScript::Enter()
{
}

void CB_STATEDeadScript::tick()
{

}

void CB_STATEDeadScript::Exit()
{
}
