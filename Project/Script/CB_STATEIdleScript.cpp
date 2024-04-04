#include "pch.h"
#include "CB_STATEIdleScript.h"
#include "CB_FSMScript.h"

CB_STATEIdleScript::CB_STATEIdleScript()
	: test(false)
	, m_pTest(nullptr)
{
	SetStateType(static_cast<UINT>(eB_States::IDLE));
}

CB_STATEIdleScript::~CB_STATEIdleScript()
{
}


void CB_STATEIdleScript::begin()
{
	CB_StateScript::begin();
}

void CB_STATEIdleScript::Enter()
{

}


void CB_STATEIdleScript::tick()
{
	if (!test)
	{
		/*Ptr<CPrefab> fab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\tset333.pref");
		m_pTest = fab->Instantiate(Vec3(0, 0, 0), 4);
		SpawnGameObject(m_pTest, Vec3(0, 0, 0), L"Effect");

		m_pTest->SetName(L"blood");*/

		// **** 한번만 파티클을 실행시키려면 이 함수를 쓸 것
		//m_pTest->ParticleSystem()->SetExcute(true);	

		// **** 지정한 시간 동안만 실행시키고 끄려면 이 함수를 쓸 것 (bool 변수 뭐 하나 만들어두기) 
		//m_pTest->ParticleSystem()->SetSpawnTime(10.f);
		//m_pTest->ParticleSystem()->SetTimeSpawn(true);


		test = true;

	}

	//m_pTest->ParticleSystem()->isFinish(); // 이 함수를 쓰면 시간초가 지나고 파티클이 spawn이 끝난 시점을 갖고 올 수 있음. 

	if (KEY_TAP(KEY::_5)) // 플레이어가 충돌체에 부딪치면 그 때 move로 전환한다. 라고 나중에 바꿀 것임 (충돌체 이벤트 함수 쪽에서) 
	{
		m_BHQ->ChangeState(static_cast<UINT>(eB_States::MOVE));
	}

	//float dist = DistBetwPlayer();
	//if (dist < 2000.f) // 
	//	m_BHQ->ChangeState(static_cast<UINT>(eB_States::MOVE));

}


void CB_STATEIdleScript::Exit()
{




}
