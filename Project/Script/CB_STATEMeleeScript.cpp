#include "pch.h"
#include "CB_STATEMeleeScript.h"
#include "CB_FSMScript.h"
#include "CHitBoxScript.h"
#include <cstdlib> // rand(), srand() 함수를 사용하기 위한 헤더
#include <ctime>   // time() 함수를 사용하기 위한 헤더


// 후후
CB_STATEMeleeScript::CB_STATEMeleeScript()
	: m_bAtkComplete(false)
	, m_bDashOnce(false)
	, m_iAttackType(0)
	, m_iMeleeAtkType(0)
	, m_bNearPlayer(false)
	, m_bSpawnCircleSpell(false)
	, m_AOE_Circle(nullptr)
	, m_bStorePlayerPos(false)
	, m_fCurForce(20.f)
	, m_AOE_Fog(nullptr)
	, m_bSpawnFog(false)
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


	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_Weapon_AOE, 19) = std::bind(&CB_STATEMeleeScript::SpawnSpell, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_Weapon_AOE, 84) = std::bind(&CB_STATEMeleeScript::SpawnSpellGravity, this); 
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_Weapon_AOE, 86) = std::bind(&CB_STATEMeleeScript::SpawnSpellOff, this);

	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_Weapon_AOE, 85) = std::bind(&CB_STATEMeleeScript::AOE_AttackBoxOn, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Atk_Weapon_AOE, 110) = std::bind(&CB_STATEMeleeScript::AOE_AttackBoxOff, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_Weapon_AOE) = std::bind(&CB_STATEMeleeScript::AOE_End, this);

	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_L01) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_L02) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_L03) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_R01) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_R02) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_R03) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);
	
	//m_BHQ->Animator3D()->CompleteEvent(B_Melee_Atk_Weapon_AOE) = std::bind(&CB_STATEMeleeScript::AttackEnd, this);


	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Evade_F) = std::bind(&CB_STATEMeleeScript::EvadeEnd, this);
	//m_BHQ->Animator3D()->StartEvent(B_Melee_Evade_F) = std::bind(&CB_STATEMeleeScript::Evade_MoveZero, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Evade_F, 1) = std::bind(&CB_STATEMeleeScript::Evade_MoveZero, this); // 48이 끝 
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Evade_F, 8) = std::bind(&CB_STATEMeleeScript::Evade_MoveRestore, this); // 8
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Evade_F, 25) = std::bind(&CB_STATEMeleeScript::Evade_MoveZero, this); 
	



	Vec3 ShooterPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();
	CGameObject* attacker = m_BHQ->GetOwner();

	// ======
	// 히트박스
	tHitInfo info0;
	info0.Damage = 20;
	info0.KnockBackGrade = 2;
	info0.Shooter = attacker;
	info0.ShooterPos = ShooterPos;
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 150.f), Vec3(0.f, 100.f, -120.f), info0);
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(0);


	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 150.f), Vec3(0.f, 100.f, -120.f), info0);
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(1);


	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 150.f), Vec3(0.f, 100.f, -120.f), info0);
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(2);

	// R
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 60.f), Vec3(0.f, 100.f, -100.f), info0);
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(3);

	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 60.f), Vec3(0.f, 100.f, -100.f), info0);
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(4);

	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 150.f, 60.f), Vec3(0.f, 100.f, -100.f), info0);
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(5);


	tHitInfo info1;
	info1.Damage = 50;
	info1.KnockBackGrade = 2;
	info1.Shooter = attacker;
	info1.ShooterPos = ShooterPos;
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(120.f, 170.f, 150.f), Vec3(0.f, 100.f, -100.f), info1);
	m_BHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(6); // AOE

	// ============================= 





}

void CB_STATEMeleeScript::Enter()
{
	// 테스트 용으로 공격상태 일단 미리 지정(테스트 완료 후 지우기)
//	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::AOE);
	//m_BHQ->GetStance_Weapon();


}

void CB_STATEMeleeScript::tick()
{
	CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

	if (curStance == CB_FSMScript::eBossStance_Weapon::EVADE)
		EvadeMove();



	Vec3 PlayerPos = GetPlayerPos();


	bool bPlay = m_BHQ->IsPlaying();

	if (!bPlay)
	{
		if (bPlay)
			return;

		// 앞인지 뒤인지 판가름할 기준이 필요할듯 (플레이어의 등 앞뒤 기준으로????)
		if (curStance == CB_FSMScript::eBossStance_Weapon::EVADE)
		{
			m_BHQ->PlayAnim(B_Melee_Evade_F, false);
		}
		
		else if (curStance == CB_FSMScript::eBossStance_Weapon::MELEE_ATK)
		{
			//m_BHQ->PlayAnim(B_Melee_Atk_L01, false);

				m_iMeleeAtkType = MeleeAttackRandom();

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
	m_BHQ->SetPlaying(false);



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
	m_BHQ->SetPlaying(false);

	if (DistCheck()) // 플레이어가 근처에 있다면
	{
		int RandomNum = ZeroToOneRandom();

		if (RandomNum == 0)
			m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::MELEE_ATK);

		else if (RandomNum == 1)
			m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::AOE);
	}

	else
	{
		m_BHQ->ChangeState(static_cast<UINT>(eB_States::MOVE));
	}
}

void CB_STATEMeleeScript::AOE_End()
{

	m_BHQ->SetPlaying(false);

	if (DistCheck()) // 플레이어가 근처에 있다면
	{
		int RandomNum = ZeroToOneRandom();

		if (RandomNum == 0)
			m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::MELEE_ATK);

		else if (RandomNum == 1)
			m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::AOE);
	}

	else
	{
		m_BHQ->ChangeState(static_cast<UINT>(eB_States::MOVE));
	}

	//tParticleModule ModuleData = m_AOE_Fog->ParticleSystem()->GetModuleData();
	//ModuleData.bDead = true;
	//m_AOE_Fog->ParticleSystem()->SetModuleData(ModuleData);



}

void CB_STATEMeleeScript::PlayerToRotate()
{

	//// 플레이어가 있는 방향으로 몸을 튼다. 이를 위해 보스와 플레이어 간의 각도를 구하고 내적한다. (얼마나 회전해야되는지에 대한 값)
	//Vec3 PlayerPos = GetPlayerPos();
	//Vec3 BossPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();

	//// 몬스터가 플레이어를 바라보아야할 방향을 구한다. 방향이기 때문에 정규화를 해준다.
	//Vec3 ToPlayerDir = PlayerPos - BossPos;

	//// XMVector3Dot를 사용하기 위해 XMVECTOR로 변환
	//XMVECTOR toPlayerDir = XMLoadFloat3(&ToPlayerDir);
	//toPlayerDir = XMVector3Normalize(ToPlayerDir);

	//Vec3 Front = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	//// m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT)를 XMVECTOR로 변환
	//XMVECTOR vFront = XMLoadFloat3(&Front);
	//vFront = XMVector3Normalize(vFront);

	//// 두 벡터 사이의 내적을 계산하여 보스가 플레이어를 바라보려면 얼만큼 회전해야될지를 알아낸다.
	//// 여기서의 acos는 주어진 코싸인 값으로부터 라디안 각도를 구해올 때 사용
	//XMVECTOR  DotResult = XMVector3Dot(vFront, toPlayerDir);

	//// 내적 결과에서 스칼라 값(코사인 각도) 추출
	//float dotScalar = XMVectorGetX(DotResult);

	//// 코사인 각도 값으로부터 실제 각도(라디안 단위)를 뽑아온다.
	//float angleRadians = acos(dotScalar);



	//=======================
	// 현재 몬스터의 방향을 가져옵니다.
	Vec3 Front = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);

	// 플레이어의 위치를 가져옵니다.
	Vec3 playerPosition = GetPlayerPos();

	// 몬스터의 위치를 가져옵니다.
	Vec3 monsterPosition = m_BHQ->GetOwner()->Transform()->GetRelativePos();

	// 플레이어를 향한 방향을 계산합니다.
	Vector3 targetDirection = playerPosition - monsterPosition;
	targetDirection.Normalize();

	// 선형 보간을 사용하여 현재 방향에서 목표 방향으로 점진적으로 회전합니다.
	// t 값은 보간 비율을 결정하며, 이 값이 클수록 목표 방향으로 더 빠르게 회전합니다.
	float t = 4; // 회전 속도를 조절하는 값입니다. 필요에 따라 조절하세요.
	Vector3 newDirection = Vector3::Lerp(Front, targetDirection, t);

	// 새 방향을 몬스터의 방향으로 설정합니다.
	m_BHQ->GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, newDirection.x, 0.f));




	//m_BHQ->GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, angleRadians, 0.f));

}

void CB_STATEMeleeScript::SpawnSpell()
{
	Vec3 BossPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();
	Vec3 PlayerPos = GetPlayerPos();


	// 이미 한번 생성했다면 객체 생성 x 
	if (nullptr == m_AOE_Circle && !m_bSpawnCircleSpell)
	{
		Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Circle_AOE.pref", L"prefab\\Circle_AOE.pref");
		fab->PrefabLoad(L"prefab\\Circle_AOE.pref");

		//==== spawn Object함수랑 cloneObj함수랑 위치 값 똑같이 해주기 
		m_AOE_Circle = fab.Get()->Instantiate(Vec3(PlayerPos), 2);
		//fab->FabClear();

		SpawnGameObject(m_AOE_Circle, Vec3(PlayerPos), L"Effect");
		m_AOE_Circle->SetName(L"blood");

		m_bSpawnCircleSpell = true;
	}

		tParticleModule ModuleData = m_AOE_Circle->ParticleSystem()->GetModuleData();
		ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
		ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::GRAVITY] = false;
		ModuleData.bDead = false;
		m_AOE_Circle->ParticleSystem()->SetModuleData(ModuleData);

}



void CB_STATEMeleeScript::SpawnSpellGravity()
{

	tParticleModule ModuleData  = m_AOE_Circle->ParticleSystem()->GetModuleData();
	ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = false;
	ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::GRAVITY] = true;
	ModuleData.fGravityForce = 6500.f;

	m_AOE_Circle->ParticleSystem()->SetModuleData(ModuleData);
	

}


void CB_STATEMeleeScript::SpawnSpellOff()
{
	tParticleModule ModuleData = m_AOE_Circle->ParticleSystem()->GetModuleData();
	ModuleData.bDead = true;


	ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
	ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::GRAVITY] = false;
	m_AOE_Circle->ParticleSystem()->SetModuleData(ModuleData);



	Vec3 BossPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();
	Vec3 PlayerPos = GetPlayerPos();


	// 이미 한번 생성했다면 객체 생성 x 
	if (nullptr == m_AOE_Fog && !m_bSpawnFog)
	{
		Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Circle_FOG.pref", L"prefab\\Circle_FOG.pref");
		fab->PrefabLoad(L"prefab\\Circle_FOG.pref");

		//==== spawn Object함수랑 cloneObj함수랑 위치 값 똑같이 해주기 
		m_AOE_Fog = fab.Get()->Instantiate(Vec3(PlayerPos), 2);
	

		tParticleModule ModuleData = m_AOE_Fog->ParticleSystem()->GetModuleData();
		ModuleData.bDead = false;
		m_AOE_Fog->ParticleSystem()->SetModuleData(ModuleData);

		SpawnGameObject(m_AOE_Fog, Vec3(PlayerPos), L"Effect");
		m_AOE_Fog->SetName(L"AOE_FOG	");

		m_bSpawnFog = true;
	}


}

void CB_STATEMeleeScript::EvadeEnd()
{

	m_BHQ->SetPlaying(false);
	m_bStorePlayerPos = false; // Move Evade를 할 때 플레이어 위치를 저장하는 시점을 조정하기 위한 변수


	if (DistCheck()) // 플레이어가 근처에 있다면
	{
		int RandomNum = ZeroToOneRandom();

		if (RandomNum == 0)
			m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::MELEE_ATK);

		else if (RandomNum == 1)
			m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::AOE);
	}

	else
	{
		m_BHQ->ChangeState(static_cast<UINT>(eB_States::MOVE));
	}

}

void CB_STATEMeleeScript::Evade_MoveZero()
{
	m_fCurForce = 0.f;
}

void CB_STATEMeleeScript::Evade_MoveRestore()
{
	m_fCurForce = 20.f;
}

int CB_STATEMeleeScript::ZeroToOneRandom()
{
	srand((unsigned int)time(NULL));
	return rand() % 2;
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

bool CB_STATEMeleeScript::DistCheck()
{
	if (DistBetwPlayer() < 250.f)
	{
		m_bNearPlayer = true;
		return true;

	}
	else
	{
		m_bNearPlayer = false;
		return false;
	}


}

void CB_STATEMeleeScript::EvadeMove()
{
	Vec3 vBossCurPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();
	Vec3 vBossFront = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);

	if (!m_bStorePlayerPos)
	{
		m_vPlayerPos = GetPlayerPos();

		m_vPlayerPos.z = m_vPlayerPos.z; // 착지할 때 플레이어와 겹치면 안되므로 약간의 여유 공간을 둔다. 

		m_bStorePlayerPos = true;
	}

	Vec3 BossToPlayerDir = m_vPlayerPos - vBossCurPos;
	BossToPlayerDir.Normalize();

	vBossCurPos += -vBossFront * m_fCurForce;

	m_BHQ->GetOwner()->Transform()->SetRelativePos(vBossCurPos);
}
