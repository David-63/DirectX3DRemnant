#include "pch.h"
#include "CB_STATEHealScript.h"
#include "CB_FSMScript.h"

// blood drink Loop ป๓ลย 

CB_STATEHealScript::CB_STATEHealScript()
{
	SetStateType(static_cast<UINT>(eB_States::HEAL));
}

CB_STATEHealScript::~CB_STATEHealScript()
{
}

void CB_STATEHealScript::begin()
{
	CB_StateScript::begin();




}

void CB_STATEHealScript::Enter()
{
}

void CB_STATEHealScript::tick()
{

}

void CB_STATEHealScript::Exit()
{

	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_END);
	m_BHQ->ChangeState(static_cast<UINT>(eB_States::MOVE));
}

