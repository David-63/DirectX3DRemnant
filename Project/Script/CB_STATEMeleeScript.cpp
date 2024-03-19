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
	// 테스트 용으로 공격상태 일단 미리 지정(테스트 완료 후 지우기)

	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::MELEE_ATK);

	// 밀리 공격 패턴 1,2 중에 고른다. 


}

void CB_STATEMeleeScript::tick()
{


	CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

	bool bPlay = m_BHQ->IsPlaying();

	if (!bPlay)
	{
		if(bPlay)
		return;

		// 앞인지 뒤인지 판가름할 기준이 필요할듯 (플레이어의 등 앞뒤 기준으로????)
		if (curStance == CB_FSMScript::eBossStance_Weapon::MELEE_ATK)
		{
			m_BHQ->PlayAnim(B_Melee_Atk_L01, false);
			m_BHQ->SetPlaying(true);
			// 모든 공격, 회복 패턴이 끝나면 이 상태로 기본으로 돌아가게 한다. 

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
