#include "pch.h"
#include "CB_STATEHealScript.h"
#include "CB_FSMScript.h"

// blood drink Loop ���� 

CB_STATEHealScript::CB_STATEHealScript()
{
	SetStateType(static_cast<UINT>(eB_States::HEAL));
	m_fTime = 0.f;
}

CB_STATEHealScript::~CB_STATEHealScript()
{
}

void CB_STATEHealScript::begin()
{
	CB_StateScript::begin();

	m_BHQ->Animator3D()->CompleteEvent(B_BloodDrink_End) = std::bind(&CB_STATEHealScript::BloodDrink_LoopEnd, this);


}

void CB_STATEHealScript::Enter()
{
	m_BHQ->PlayAnim(B_BloodDrink_Loop, true);
}

void CB_STATEHealScript::tick()
{

	// ������ Y�� Z�� ȸ���� ���� ��� ��ƼŬ ������ֱ�
	bool IsPlaying = m_BHQ->IsPlaying();

	m_fTime += DT;

	// ������ ü���� ���� ü�¿��� 10% �̻� ȸ���ƴٸ� blood end�� ���ư���.
	CB_FSMScript::tB_Info BossInfo = m_BHQ->GetBossInfo();
	int BossHP = BossInfo.B_Health.CurHp;

	if (m_fTime < 30.f)
	{
		BossHP += 0.5;
	}

	else if (m_fTime >= 30.f)
	{
		/*bool IsPlaying = m_BHQ->IsPlaying();
		if (IsPlaying)
			return;


		m_BHQ->PlayAnim(B_BloodDrink_End, false);

		m_fTime = 0.f;*/
	}




}

void CB_STATEHealScript::Exit()
{

}

void CB_STATEHealScript::BloodDrink_LoopEnd()
{
	m_BHQ->SetPlaying(false);
	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::FAST_WALK);
	m_BHQ->ChangeState(static_cast<UINT>(eB_States::MOVE));
}

