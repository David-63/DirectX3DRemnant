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
	m_PHQ->RigidBody()->SetVelocity(Vec3(0, 0, 0));

}

void CP_STATEReloadScript::Enter()
{

}

void CP_STATEReloadScript::Exit()
{
}
