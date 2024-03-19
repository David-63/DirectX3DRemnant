#include "pch.h"
#include "CB_STATEDamagedScript.h"
#include "CB_FSMScript.h"


CB_STATEDamagedScript::CB_STATEDamagedScript()
{
	SetStateType(static_cast<UINT>(eB_States::DAMAGED));
}

CB_STATEDamagedScript::~CB_STATEDamagedScript()
{
}

void CB_STATEDamagedScript::Enter()
{
}

void CB_STATEDamagedScript::tick()
{
}

void CB_STATEDamagedScript::Exit()
{
}

