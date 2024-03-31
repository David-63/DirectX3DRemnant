#include "pch.h"
#include "CB_STATEMeleeScript.h"
#include "CB_FSMScript.h"
#include "CHitBoxScript.h"
#include <cstdlib> // rand(), srand() �Լ��� ����ϱ� ���� ���
#include <ctime>   // time() �Լ��� ����ϱ� ���� ���



CB_STATEMeleeScript::CB_STATEMeleeScript()
	: m_bAtkComplete(false)
	, m_bDashOnce(false)
	, m_iAttackType(0)
	, m_iMeleeAtkType(0)
	, m_bAttack(false)
	, m_bNearPlayer(false)
	, m_bSpawnCircleSpell(false)
	, m_AOE_Circle(nullptr)
{
	SetStateType(static_cast<UINT>(eB_States::MELEE));
}

CB_STATEMeleeScript::~CB_STATEMeleeScript()
{
}

void CB_STATEMeleeScript::begin()
{
	CB_StateScript::begin();

	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_L01, 15) = std::bind(&CB_STATEMeleeScript::AttackBoxOn, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_L01, 17) = std::bind(&CB_STATEMeleeScript::AttackBoxOff, this);

	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_L02, 10) = std::bind(&CB_STATEMeleeScript::AttackBoxOn2, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_L02, 14) = std::bind(&CB_STATEMeleeScript::AttackBoxOff2, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_L02, 21) = std::bind(&CB_STATEMeleeScript::AttackBoxOn2, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_L02, 25) = std::bind(&CB_STATEMeleeScript::AttackBoxOff2, this);

	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_L03, 25) = std::bind(&CB_STATEMeleeScript::AttackBoxOn3, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_L03, 33) = std::bind(&CB_STATEMeleeScript::AttackBoxOff3, this);
	//m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_L03, 14) = std::bind(&CB_STATEMeleeScript::AttackBoxOff3, this);



	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_Weapon_AOE, 50) = std::bind(&CB_STATEMeleeScript::SpawnSpell, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_Weapon_AOE, 85) = std::bind(&CB_STATEMeleeScript::AOE_AttackBoxOn, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_Weapon_AOE, 110) = std::bind(&CB_STATEMeleeScript::AOE_AttackBoxOff, this);


	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_L01) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_L02) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_L03) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_R01) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_R02) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_R03) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_Weapon_AOE) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);

	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_Weapon_AOE, 15) = std::bind(&CB_STATEMeleeScript::SpawnSpell, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_Weapon_AOE, 87) = std::bind(&CB_STATEMeleeScript::SpawnSpellOff, this);


	// ===== ��Ʈ �ڽ�
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 150.f), Vec3(0.f, 100.f, -120.f));
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(0);

	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 150.f), Vec3(0.f, 100.f, -120.f));
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(1);

	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 150.f), Vec3(0.f, 100.f, -120.f));
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(2);

	// R
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 60.f), Vec3(0.f, 100.f, -100.f));
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(3);

	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 60.f), Vec3(0.f, 100.f, -100.f));
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(4);

	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 60.f), Vec3(0.f, 100.f, -100.f));
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(5);

	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 170.f, 150.f), Vec3(0.f, 100.f, -100.f));
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(6); // AOE

	// ============================= 





}

void CB_STATEMeleeScript::Enter()
{
	// �׽�Ʈ ������ ���ݻ��� �ϴ� �̸� ����(�׽�Ʈ �Ϸ� �� �����)
	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::AOE);


	//m_iAttackType = AttackTypeRandom();

	//if(m_iAttackType == 0)
	//	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::MELEE_ATK);
	//else if(m_iAttackType == 1)
		//m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::AOE);

}

void CB_STATEMeleeScript::tick()
{

	CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

	//// �÷��̾������� ���� ư��.
	//if(m_bAttack)
	//	PlayerToRotate();

	Vec3 PlayerPos = GetPlayerPos();


	bool bPlay = m_BHQ->IsPlaying();

	if (!bPlay)
	{
		if (bPlay)
			return;

		// ������ ������ �ǰ����� ������ �ʿ��ҵ� (�÷��̾��� �� �յ� ��������????)
		if (curStance == CB_FSMScript::eBossStance_Weapon::MELEE_ATK)
		{
			m_BHQ->PlayAnim(B_Melee_Atk_L01, true);

			/*	m_iMeleeAtkType = MeleeAttackRandom();

				if (m_iMeleeAtkType == 0)
				{
					m_BHQ->PlayAnim(B_Melee_Atk_L01, false);

				}

				else if (m_iMeleeAtkType == 1)
				{
					m_BHQ->PlayAnim(B_Melee_Atk_L02, false);

				}

				else if (m_iMeleeAtkType == 2)
				{
					m_BHQ->PlayAnim(B_Melee_Atk_L03, false);

				}
				else if (m_iMeleeAtkType == 3)
				{
					m_BHQ->PlayAnim(B_Melee_Atk_R01, false);
				}
				else if (m_iMeleeAtkType == 4)
				{
					m_BHQ->PlayAnim(B_Melee_Atk_R02, false);
				}
				else if (m_iMeleeAtkType == 5)
				{
					m_BHQ->PlayAnim(B_Melee_Atk_R03, false);
				}

		*/

		}


		else if (curStance == CB_FSMScript::eBossStance_Weapon::AOE)
		{

			m_BHQ->PlayAnim(B_Melee_Atk_Weapon_AOE, true);


		}



		m_BHQ->SetPlaying(true);


	}






}

void CB_STATEMeleeScript::Exit()
{
	// ���� �� ���� ������. ���� ������ ��ü �� ��� �� ���� Ÿ���� �޾Ҵٸ� blood drink ���·� �����ϱ� ����
	// ������ ��ҷ� �̵��ϴ� noWeapon ���·� �����ϰ� �̵����Ѿ��ϱ� ����. 
	// �ϴ��� fast_walk�� �ص�.
	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::FAST_WALK);


}

void CB_STATEMeleeScript::AttackBoxOn()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(0);


}

void CB_STATEMeleeScript::AttackBoxOn2()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(1);
}

void CB_STATEMeleeScript::AttackBoxOn3()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(2);
}

void CB_STATEMeleeScript::AttackBoxOn4()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(3);
}

void CB_STATEMeleeScript::AttackBoxOn5()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(4);
}

void CB_STATEMeleeScript::AttackBoxOn6()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(5);
}

void CB_STATEMeleeScript::AOE_AttackBoxOn()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(6);
}

void CB_STATEMeleeScript::AttackBoxOff()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(0);
}

void CB_STATEMeleeScript::AttackBoxOff2()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(1);
}

void CB_STATEMeleeScript::AttackBoxOff3()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(2);
}

void CB_STATEMeleeScript::AttackBoxOff4()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(3);
}

void CB_STATEMeleeScript::AttackBoxOff5()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(4);
}

void CB_STATEMeleeScript::AttackBoxOff6()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(5);
}

void CB_STATEMeleeScript::AOE_AttackBoxOff()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(6);
}

void CB_STATEMeleeScript::AttackEnd()
{
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(6);

	m_BHQ->SetPlaying(false);

	m_bAttack = true;

	// ====================



	// ===========================



	// �÷��̾ ������ �����̿� �ִٸ� ������ �ݺ��Ѵ� (�ٰŸ� ������ ��������� ������ �÷��̾�� �ϸ鼭 �����ҵ�?)
	DistCheck();

	if (m_bNearPlayer)
		m_iAttackType = AttackTypeRandom();

	// �÷��̾ �ٰŸ��� ���ٸ� �ٽ� Move ���·� ���ư���.
	else
		m_BHQ->ChangeState(static_cast<UINT>(eB_States::MOVE));
}

void CB_STATEMeleeScript::PlayerToRotate()
{

	//// �÷��̾ �ִ� �������� ���� ư��. �̸� ���� ������ �÷��̾� ���� ������ ���ϰ� �����Ѵ�. (�󸶳� ȸ���ؾߵǴ����� ���� ��)
	//Vec3 PlayerPos = GetPlayerPos();
	//Vec3 BossPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();

	//// ���Ͱ� �÷��̾ �ٶ󺸾ƾ��� ������ ���Ѵ�. �����̱� ������ ����ȭ�� ���ش�.
	//Vec3 ToPlayerDir = PlayerPos - BossPos;

	//// XMVector3Dot�� ����ϱ� ���� XMVECTOR�� ��ȯ
	//XMVECTOR toPlayerDir = XMLoadFloat3(&ToPlayerDir);
	//toPlayerDir = XMVector3Normalize(ToPlayerDir);

	//Vec3 Front = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	//// m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT)�� XMVECTOR�� ��ȯ
	//XMVECTOR vFront = XMLoadFloat3(&Front);
	//vFront = XMVector3Normalize(vFront);

	//// �� ���� ������ ������ ����Ͽ� ������ �÷��̾ �ٶ󺸷��� ��ŭ ȸ���ؾߵ����� �˾Ƴ���.
	//// ���⼭�� acos�� �־��� �ڽ��� �����κ��� ���� ������ ���ؿ� �� ���
	//XMVECTOR  DotResult = XMVector3Dot(vFront, toPlayerDir);

	//// ���� ������� ��Į�� ��(�ڻ��� ����) ����
	//float dotScalar = XMVectorGetX(DotResult);

	//// �ڻ��� ���� �����κ��� ���� ����(���� ����)�� �̾ƿ´�.
	//float angleRadians = acos(dotScalar);



	//=======================
	// ���� ������ ������ �����ɴϴ�.
	Vec3 Front = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);

	// �÷��̾��� ��ġ�� �����ɴϴ�.
	Vec3 playerPosition = GetPlayerPos();

	// ������ ��ġ�� �����ɴϴ�.
	Vec3 monsterPosition = m_BHQ->GetOwner()->Transform()->GetRelativePos();

	// �÷��̾ ���� ������ ����մϴ�.
	Vector3 targetDirection = playerPosition - monsterPosition;
	targetDirection.Normalize();

	// ���� ������ ����Ͽ� ���� ���⿡�� ��ǥ �������� ���������� ȸ���մϴ�.
	// t ���� ���� ������ �����ϸ�, �� ���� Ŭ���� ��ǥ �������� �� ������ ȸ���մϴ�.
	float t = 4; // ȸ�� �ӵ��� �����ϴ� ���Դϴ�. �ʿ信 ���� �����ϼ���.
	Vector3 newDirection = Vector3::Lerp(Front, targetDirection, t);

	// �� ������ ������ �������� �����մϴ�.
	m_BHQ->GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, newDirection.x, 0.f));




	//m_BHQ->GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, angleRadians, 0.f));

	m_bAttack = false;
}

void CB_STATEMeleeScript::SpawnSpell()
{
	Vec3 BossPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();
	Vec3 PlayerPos = GetPlayerPos();

	if (m_bSpawnCircleSpell)
		return;

	    Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Circle_AOE.pref", L"prefab\\Circle_AOE.pref");
		fab->PrefabLoad(L"prefab\\Circle_AOE.pref");

		//==== spawn Object�Լ��� cloneObj�Լ��� ��ġ �� �Ȱ��� ���ֱ� 
		m_AOE_Circle = fab->Instantiate(Vec3(300.f, 0.f, 400.f), 2);
		SpawnGameObject(m_AOE_Circle, Vec3(PlayerPos.x, PlayerPos.y + 210, PlayerPos.z), L"Effect");

		m_AOE_Circle->SetName(L"blood");
		m_bSpawnCircleSpell = true;






	/*Ptr<CPrefab> fab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Circle_AOE.pref");
	CGameObject* cloneObj = fab->Instantiate(BossPos, 4);
	SpawnGameObject(cloneObj, Vec3(BossPos), L"Effect");
	m_bSpawnCircleSpell = true;*/
}

void CB_STATEMeleeScript::SpawnSpellOff()
{
	DestroyObject(m_AOE_Circle);
	m_bSpawnCircleSpell = false;

}

UINT CB_STATEMeleeScript::MeleeAttackRandom()
{
	srand((unsigned int)time(NULL));
	return rand() % 6;


}
UINT CB_STATEMeleeScript::AttackTypeRandom()
{
	srand((unsigned int)time(NULL));
	return rand() % 2;
}

void CB_STATEMeleeScript::DistCheck()
{
	if (DistBetwPlayer() < 200.f)
		m_bNearPlayer = true;
	else
		m_bNearPlayer = false;

}

