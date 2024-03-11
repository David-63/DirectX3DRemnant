#include "pch.h"
#include "CIdleStateScript.h"


CIdleStateScript::CIdleStateScript()
{
	SetStateType(eState::IDLE);
}

CIdleStateScript::~CIdleStateScript()
{
}


void CIdleStateScript::tick()
{
	// 입력을 받아서 상태를 변경해야함
}

void CIdleStateScript::Enter()
{
}

void CIdleStateScript::Exit()
{
}