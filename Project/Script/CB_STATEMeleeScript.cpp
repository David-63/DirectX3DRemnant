#include "pch.h"
#include "CB_STATEMeleeScript.h"
#include "CB_FSMScript.h"



CB_STATEMeleeScript::CB_STATEMeleeScript()
	: m_bAtkComplete(false)
	, m_bDashOnce(false)
{
	SetStateType(static_cast<UINT>(eB_States::MELEE));
}

CB_STATEMeleeScript::~CB_STATEMeleeScript()
{
}

void CB_STATEMeleeScript::Enter()
{
	// �׽�Ʈ ������ ���ݻ��� �ϴ� �̸� ����(�׽�Ʈ �Ϸ� �� �����)

	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::MELEE_ATK);

	// �и� ���� ���� 1,2 �߿� ����. 


}

void CB_STATEMeleeScript::tick()
{


	CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

	bool bPlay = m_BHQ->IsPlaying();

	if (!bPlay)
	{
		if(bPlay)
		return;

		// ������ ������ �ǰ����� ������ �ʿ��ҵ� (�÷��̾��� �� �յ� ��������????)
		if (curStance == CB_FSMScript::eBossStance_Weapon::MELEE_ATK)
		{
			m_BHQ->PlayAnim(B_Melee_Atk_L01, false);
			m_BHQ->SetPlaying(true);
			// ��� ����, ȸ�� ������ ������ �� ���·� �⺻���� ���ư��� �Ѵ�. 

		}


		else if (curStance == CB_FSMScript::eBossStance_Weapon::AOE)
		{

			m_BHQ->PlayAnim(B_Melee_Evade_F, false);

		}
	}



}

void CB_STATEMeleeScript::Exit()
{
}
