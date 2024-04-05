#include "pch.h"
#include "CB_STATEDamagedScript.h"
#include "CB_FSMScript.h"


CB_STATEDamagedScript::CB_STATEDamagedScript()
	: m_bFinish(false)
	, m_BackSpeed(0.f)
{
	SetStateType(static_cast<UINT>(eB_States::DAMAGED));
}

CB_STATEDamagedScript::~CB_STATEDamagedScript()
{
}

void CB_STATEDamagedScript::begin()
{
	CB_StateScript::begin();

	m_vBackDir = GetMyFront(); // 뒤 방향을 갖고 온다.

	m_BHQ->Animator3D()->ActionEvent(B_Impact_Heavy_Head_L, 5) = std::bind(&CB_STATEDamagedScript::BackSpeedZero, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Impact_Heavy_Head_L) = std::bind(&CB_STATEDamagedScript::AnimFinish, this);


}

void CB_STATEDamagedScript::Enter()
{


	m_BHQ->PlayAnim(B_Impact_Heavy_Head_L, false);
	m_BackSpeed = 50.f;

}

void CB_STATEDamagedScript::tick()
{

	if (m_bFinish)
		m_BHQ->ChangeState((UINT)eB_States::MOVE);

	Vec3 myPos = GetMyPos();
	myPos += m_vBackDir * DT * m_BackSpeed;
	SetMyPos(myPos);
}

void CB_STATEDamagedScript::Exit()
{
	m_bFinish = false;


}

void CB_STATEDamagedScript::AnimFinish()
{
	m_bFinish = true;


}

void CB_STATEDamagedScript::BackSpeedZero()
{

	m_BackSpeed = 0.f;
}

