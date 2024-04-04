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

		// **** �ѹ��� ��ƼŬ�� �����Ű���� �� �Լ��� �� ��
		//m_pTest->ParticleSystem()->SetExcute(true);	

		// **** ������ �ð� ���ȸ� �����Ű�� ������ �� �Լ��� �� �� (bool ���� �� �ϳ� �����α�) 
		//m_pTest->ParticleSystem()->SetSpawnTime(10.f);
		//m_pTest->ParticleSystem()->SetTimeSpawn(true);


		test = true;

	}

	//m_pTest->ParticleSystem()->isFinish(); // �� �Լ��� ���� �ð��ʰ� ������ ��ƼŬ�� spawn�� ���� ������ ���� �� �� ����. 

	if (KEY_TAP(KEY::_5)) // �÷��̾ �浹ü�� �ε�ġ�� �� �� move�� ��ȯ�Ѵ�. ��� ���߿� �ٲ� ���� (�浹ü �̺�Ʈ �Լ� �ʿ���) 
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
