#include "pch.h"
#include "CB_STATEMoveScript.h"
#include "CB_FSMScript.h"
#include "CMonsterMoveScript.h"
#include <cstdlib> // rand(), srand() 함수를 사용하기 위한 헤더
#include <ctime>   // time() 함수를 사용하기 위한 헤더




CB_STATEMoveScript::CB_STATEMoveScript()
	: m_DirSetCount(0)
	, m_GetWeapon(false)
	, m_fPrevForce(0.f)
	, m_fCurForce(0.f)
	, m_OnceEvade(false)
	, m_OnceSpawnSpell(false)
	, m_bBloodDrink(false)
	, m_iCatchStage(0)
	, m_fAniTurnSpeed(1.f)  // turn 애니메이션으로 turn하는 속도 
	, m_fJustTurnSpeed(2.f) // 애니메이션이 없이 trun하는 속도
	, m_iGetBackStage(0)
	, m_bStorePlayerPos(false)
	, m_vPlayerPos(Vec3(0, 0, 0))
	, m_testcount(0)
	, m_bOnEvade(false)
	, m_IsPhase_1(true)
{
	SetStateType(static_cast<UINT>(eB_States::MOVE));
}

CB_STATEMoveScript::~CB_STATEMoveScript()
{
}

void CB_STATEMoveScript::begin()
{
	CB_StateScript::begin();

	m_CMoveScript = m_BHQ->GetOwner()->GetScript<CMonsterMoveScript>();
	m_CMoveScript->SetSpeed(300.f);

	m_BHQ->Animator3D()->CompleteEvent(B_Turn180_R) = std::bind(&CB_STATEMoveScript::TurnComplete180, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Turn90_L) = std::bind(&CB_STATEMoveScript::TurnComplete90L, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Turn90_R) = std::bind(&CB_STATEMoveScript::TurnComplete90R, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Turn135_L) = std::bind(&CB_STATEMoveScript::TurnComplete135L, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Turn135_R) = std::bind(&CB_STATEMoveScript::TurnComplete135R, this);

	// ==========================m_BHQ->SetPlaying(false);
	m_BHQ->Animator3D()->CompleteEvent(B_Walk_Fast_F) = std::bind(&CB_STATEMoveScript::FastWalk_End, this);
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Evade_F) = std::bind(&CB_STATEMoveScript::Evade_End, this);
	m_BHQ->Animator3D()->StartEvent(B_Melee_Evade_F) = std::bind(&CB_STATEMoveScript::Evade_MoveZero, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Evade_F, 8) = std::bind(&CB_STATEMoveScript::Evade_MoveRestore, this); // 8
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Evade_F, 46) = std::bind(&CB_STATEMoveScript::Evade_MoveZero, this); // 48이 끝 

	m_BHQ->Animator3D()->CompleteEvent(B_BloodDrink_Start) = std::bind(&CB_STATEMoveScript::BloodDrink_StartEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_BloodDrink_Loop) = std::bind(&CB_STATEMoveScript::BloodDrink_LoopEnd, this);


}



void CB_STATEMoveScript::Enter()
{
	m_bEnterSign = true;

	// 근방에 플레이어가 있는지 탐지 (캐릭터와 겹치지 않게 하기 위해
	if (DistCheck())
		m_bEnterDistCheck = true;

	//=== 테스트 용도로 해뒀기 때문에 아래 두 줄은 테스트 끝나면 지우기
   /*m_GetWeapon = false;
   m_BHQ->SetStance_NoWeapon(CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK);*/

	m_GetWeapon = true;
	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::EVADE);

}


void CB_STATEMoveScript::tick()
{



	//====================


		// 보스가 front로 갔다가 오른쪽으로 회전을 했다면 무기는 어떻게 회전해야될까?
		// 회전할 때 같이 회전시켜줘야한다. 


		//===========


	CB_FSMScript::eBossMoveDir curDir = m_BHQ->GetMoveDir();
	CB_FSMScript::eBossStance_NoWeapon NoWeaponCurStance = m_BHQ->GetStance_NoWeapon();
	CB_FSMScript::eBossStance_Weapon WeaponCurStance = m_BHQ->GetStance_Weapon();

	bool IsPlaying = m_BHQ->IsPlaying();
	CB_FSMScript::tB_Info BossInfo = m_BHQ->GetBossInfo();
	int BossHP = BossInfo.B_Health.CurHp;

	m_ratio = (float)BossHP / (float)BossInfo.B_Health.MaxHp;


	if (BossHP <= 0.0f)
	{
		m_BHQ->ChangeState((UINT)eB_States::DEAD);
	}


	// 2페이즈 시작
	if (m_ratio < 0.7f)
	{
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::FAST_WALK);
		m_GetWeapon = true;
		m_IsPhase_1 = false;
		return;
	}

	else if (m_ratio < 0.4f) // 피 조금씩만 오르게 하기. 2페이즈 기준에 도달하지 않도록
	{
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START);
	}




	// Blood drink 상태 체크 조건 
	if (WeaponCurStance == CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START
		&& m_bBloodDrink)
	{
		Vec3 BossPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();

		// *** 플레이어가 지정한 위치로 도착했다면 바꾼다. 여기서의 지정 위치는 나중에 ui로 조정해보고 하기 
		if (BossPos == Vec3(0, 0, 0))
		{
			m_BHQ->SetPlaying(false); // 언제 도착할지 모르므로 실시간으로 체크하다가 false를 하게끔 해준다. 
			m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_LOOP); // loop와 관련된거는 Heal 스크립트에서 처리한다. 이를 위해 chagestate해줌
			m_BHQ->ChangeState((UINT)eB_States::HEAL);

		}

	}


	// ============================================
	// 플레이어와의 거리를 체크해서 melee 공격을 할지 말지 고름
	float dist = DistBetwPlayer(); // 1395
	if (dist < 180.f)
		m_CMoveScript->SetNearPlayer(true);
	else
		m_CMoveScript->SetNearPlayer(false);

	if (!m_IsPhase_1 || m_bBloodDrink)
	{
		// move상태로 처음 들어왔을 떄 캐릭터가 근방에 있었다면
		if (m_bEnterDistCheck)
		{
			if (dist < 180.f)
				GetBack(); // 플레이어와 겹치지 않게 약간 거리 조절 

			else
			{
				m_bEnterDistCheck = false;
				m_iGetBackStage = 0;
			}
		}

		// move상태로 처음 들어왔을 떄 캐릭터가 근방에 없었다면
		else
		{
			//  move상태로 지금 막 전환된 상태라 enter 함수를 이전 프레임에 호출했었다면
			if (m_bEnterSign)
			{
				// 움직이기 시작했다면 
				if (MoveStart())
					m_bEnterSign = false;
			}

			// move 상태 그대로라면
			else
			{
				float dist = DistBetwPlayer();

				// 새로 길을 찾는다.
				if (m_iCatchStage == 0 && !m_bOnEvade)
					RenewPath();

			}
		}

	}

	// 지정한 시간동안 랜덤으로 재생된다. 
	// 맨 처음엔 B_Walk_FL가 재생되게 바꿔놨음.
	bool test = IsPlaying;


	if (!m_GetWeapon)
	{

		NoWeapon_MoveToDir();


		if (IsPlaying)
			return;


		if (NoWeaponCurStance == CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK)
		{
			if (curDir == CB_FSMScript::eBossMoveDir::LF)
			{
				m_BHQ->SetStance_NoWeapon(CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK);

				m_BHQ->PlayAnim(B_Walk_FL, false);
				m_BHQ->SetPlaying(true);

			}


			else if (curDir == CB_FSMScript::eBossMoveDir::RF)
			{

				m_BHQ->SetStance_NoWeapon(CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK);

				m_BHQ->PlayAnim(B_Walk_FR, false);
				m_BHQ->SetPlaying(true);
			}

			else if (curDir == CB_FSMScript::eBossMoveDir::F)
			{
				//	m_BHQ->ChangeDir(CB_FSMScript::eBossMoveDir::F);
				m_BHQ->SetStance_NoWeapon(CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK);

				m_BHQ->PlayAnim(B_Walk_F, false);


				m_BHQ->SetPlaying(true);

			}
			else if (curDir == CB_FSMScript::eBossMoveDir::B)
			{
				m_BHQ->SetStance_NoWeapon(CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK);
				m_BHQ->PlayAnim(B_Walk_B, false);

				m_BHQ->SetPlaying(true);

			}
			else if (curDir == CB_FSMScript::eBossMoveDir::LB)
			{
				m_BHQ->SetStance_NoWeapon(CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK);
				m_BHQ->PlayAnim(B_Walk_BL, false);
				m_BHQ->SetPlaying(true);

			}
			else if (curDir == CB_FSMScript::eBossMoveDir::RB)
			{
				m_BHQ->SetStance_NoWeapon(CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK);
				m_BHQ->PlayAnim(B_Walk_BR, false);
				m_BHQ->SetPlaying(true);

			}

			else if (curDir == CB_FSMScript::eBossMoveDir::R)
			{
				m_BHQ->SetStance_NoWeapon(CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK);
				m_BHQ->PlayAnim(B_Walk_FR, false);
				m_BHQ->SetPlaying(true);
			}

			else if (curDir == CB_FSMScript::eBossMoveDir::L)
			{
				m_BHQ->SetStance_NoWeapon(CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK);
				m_BHQ->PlayAnim(B_Walk_FL, false);
				m_BHQ->SetPlaying(true);
			}
		}

	}

	else if (m_GetWeapon)
	{

		// stance를 바꾸는 함수도 하나 추가해야할듯. 	stance바꾸는 거는 complete event에서 해야할듯.
		Weapon_Move();

		// m_iCatchStage가 0이 아니라면
		CatchCheck(); // move에서 근접 공격 state로 넘어가는 함수임.

		if (IsPlaying)
			return;

		CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

		// 앞인지 뒤인지 판가름할 기준이 필요할듯 (플레이어의 등 앞뒤 기준으로????)
		// 플레이어의 체력이 전체 대비 20%, 40%이면 Blood drink를 한다. 때문에 GetWeapon을 false로 해줄 일은 없다. 
		// 그리고 지정된 위치로 플레이어가 움직이고, 그 위치에 도달하면 Blood drink를 한다.
		// 이를 위해선 fast walk를 먼저 해야하는데 그 부분은 좀 생각해보자. blood drink로 상태 바꿀때 fast walk로 상태 전환하고
		// bool 값이 켜져있고 지정한 위치에 다다르면 blood drink 상태로 바꾸는걸 여기 fast_walk 안에서 해줄지 아니면 다른곳에서
		// 해줄지 복
		if (curStance == CB_FSMScript::eBossStance_Weapon::FAST_WALK)
		{
			m_BHQ->PlayAnim(B_Walk_Fast_F, true);
			// 모든 공격, 회복 패턴이 끝나면 이 상태로 기본으로 돌아가게 한다. 

		}


		// 플레이어와의 거리가 엄청 멀 때만 이 state로 들어오게 하자. 
		else if (curStance == CB_FSMScript::eBossStance_Weapon::EVADE)
		{

			m_BHQ->PlayAnim(B_Melee_Evade_F, false); // 애니메이션이 여러 번 들어오는 문제는 아님 

		}

		else if (curStance == CB_FSMScript::eBossStance_Weapon::MELEE_ATK)
		{
			// 공격 1,2를 랜덤으로 지정 (L, R 공격도 랜덤)
			m_BHQ->PlayAnim(B_Walk_B, false);

		}
		else if (curStance == CB_FSMScript::eBossStance_Weapon::AOE)
		{
			// 근거리 공격 중 하나. MELEE_ATK과 AOE랑 두 상태는 랜덤으로 하게 하자. 
			m_BHQ->PlayAnim(B_Walk_BL, false);


		}
		else if (curStance == CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START)
		{
			m_bBloodDrink = true;

			// 이 STATE에 들어오면, 모션을 진행하기 전에 어느 정도 거리를 벌리고 걷다가 해당 상태 진행. (START-LOOP-END)
			m_BHQ->PlayAnim(B_Walk_Fast_F, true);


		}

		else if (curStance == CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_END)
		{
			// ING일때는 BloodDrink에서 처리 
			m_BHQ->PlayAnim(B_BloodDrink_Start, false);

		}

		m_BHQ->SetPlaying(true);

	}

	// 해당 상태일때는 원을 생성한다. (몬스터 모션 x)
	if (NoWeaponCurStance == CB_FSMScript::eBossStance_NoWeapon::SPAWN_SPELL)
	{

		if (m_OnceSpawnSpell)
			return;

		m_OnceSpawnSpell = true; // 구체 스크립트 exit에서 false로 바꿔주기

		// 아래에서 구체를 생성하는 코드 작성. 그리고 구체 스크립트 컴포넌트를 갖다붙인다. (상태 변경x 캐릭터가 움직여야하므로)
		// 동시에 구체 2개를 생성한다. (플레이어의 좌우에)

	}


	// ============= 회피
	// 쿨타임이 다 됐고, 마침 플레이어의 조준점이 캐릭터를 바라보고 있는 상황이라면?(레이캐스트가 TRUE라면)
	//if (m_EvadeCoolTime.IsFinish()) // && 
	//{
		// Evade 애니메이션을 재생한다. 

	//	// 애니메이션 재생이 끝나면 다시 move 상태로 돌아간다. (컴플리트 이벤트)
	//}
}


void CB_STATEMoveScript::Exit()
{
	m_OnceEvade = false;

	Clear();
	m_CMoveScript->Clear();
}

void CB_STATEMoveScript::NoWeapon_MoveToDir()
{
	Vec3 vFront = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);


	Vec3 rf = vRight + vFront;
	rf.Normalize(); //  방향 가리키는 용도이므로 벡터 정규화

	Vec3 lf = -vRight + vFront;
	lf.Normalize(); // 방향 가리키는 용도이므로 벡터 정규화

	// ==== stance별 이동 속도
	float Force = 0.f;
	CB_FSMScript::tB_Info BossInfo = m_BHQ->GetBossInfo();
	CB_FSMScript::eBossStance_NoWeapon curStance = m_BHQ->GetStance_NoWeapon();

	if (CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK == curStance)
		Force = BossInfo.B_Stat.MoveSpeed * ScaleDT;




	// ===== 직접적인 이동

	Vec3 vCurPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();
	CB_FSMScript::eBossMoveDir curDir = m_BHQ->GetMoveDir();

	switch (curDir)
	{
	case CB_FSMScript::eBossMoveDir::F:
		vCurPos -= vFront * Force;
		break;

	case CB_FSMScript::eBossMoveDir::B:
		vCurPos += vFront * Force;
		break;

	case CB_FSMScript::eBossMoveDir::R:
		vCurPos -= vRight * Force;
		break;

	case CB_FSMScript::eBossMoveDir::RF:
		vCurPos -= rf * Force;
		break;

	case CB_FSMScript::eBossMoveDir::RB:
		vCurPos += lf * Force;
		break;

	case CB_FSMScript::eBossMoveDir::L:
		vCurPos += vRight * Force;
		break;

	case CB_FSMScript::eBossMoveDir::LF:
		vCurPos -= lf * Force;
		break;

	case CB_FSMScript::eBossMoveDir::LB:
		vCurPos += rf * Force;
		break;


	}

	m_BHQ->GetOwner()->Transform()->SetRelativePos(vCurPos);
}

void CB_STATEMoveScript::Weapon_Move()
{

	CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

	Weapon_StanceChange();


	// ===== 직접적인 이동
	Vec3 vBossFront = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vRight = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	Vec3 vBossCurPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();

	switch (curStance)
	{
	case CB_FSMScript::eBossStance_Weapon::FAST_WALK:
	{
		// 몬스터의 front 방향에 플레이어가 있었다면 이걸로. 플레이어가 몬스터의 back방향에 있었다면 += 으로 해야함


	// blood rink일때만 사용한다 이런식으로 하기
	//	vBossCurPos -= vBossFront  * m_fCurForce;
	}
	break;

	case CB_FSMScript::eBossStance_Weapon::EVADE:
	{
		// 플레이어와의 거리가 어느 정도 다다르면 플레이어 앞으로 이동한다.
		// 플레이어의 위치를 받아와서 거리를 체크한다.
		//m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::EVADE);

		// 플레이어 쪽으로 이동한다.
		// 앞으로 이동하다가 플레이어 위치에 이르면 멈춘다.
		if (!m_bStorePlayerPos)
		{
			m_vPlayerPos = GetPlayerPos();

			m_vPlayerPos.z = m_vPlayerPos.z + 120.f; // 착지할 때 플레이어와 겹치면 안되므로 약간의 여유 공간을 둔다. 

			m_bStorePlayerPos = true;
		}

		Vec3 BossToPlayerDir = m_vPlayerPos - vBossCurPos;
		BossToPlayerDir.Normalize();

		vBossCurPos += -vBossFront * m_fCurForce;
		//m_BHQ->GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f)); // 회전이 혼자 되길래 강제 세팅

	}
	break;

	case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START:
	{

		Vec3 BossCurPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();

		// 지정한 위치까지 이동한다. (일단 0,0,0으로 해둠)
		Vec3 TargetPos = Vec3(0, 0, 0);
		Vec3 BossToTartgetDir = TargetPos - BossCurPos;
		BossToTartgetDir.Normalize();

		vBossCurPos += vBossFront * BossToTartgetDir * m_fCurForce;


	}
	break;
	}

	m_BHQ->GetOwner()->Transform()->SetRelativePos(vBossCurPos);
}

void CB_STATEMoveScript::Weapon_StanceChange()
{
	CB_FSMScript::tB_Info BossInfo = m_BHQ->GetBossInfo();
	CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

	if (curStance == curStance && m_OnceEvade)
		return;

	if (CB_FSMScript::eBossStance_Weapon::FAST_WALK == curStance
		|| CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START == curStance)
	{
		m_fCurForce = BossInfo.B_Stat.MoveSpeed * ScaleDT * 3.f;

	}

	else if (CB_FSMScript::eBossStance_Weapon::EVADE == curStance)
	{
		m_fCurForce = BossInfo.B_Stat.MoveSpeed * ScaleDT * 17.f;
		m_OnceEvade = true;

	}

	m_fPrevForce = m_fCurForce;

}

void CB_STATEMoveScript::Weapon_StanceEvent() // 현재 안씀 
{

	// 이거는 Move 상태내에서의 stance를 바꾸는 함수임.

	m_BHQ->SetPlaying(false);



	CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();
	if (curStance == CB_FSMScript::eBossStance_Weapon::FAST_WALK)
	{
		// 대상 객체와 플레이어 사이의 현재 거리
		Vec3 PlayerPos = GetPlayerPos();


		Vec3 BossPos = GetOwner()->Transform()->GetRelativePos();
		float currentDistance = Vec3::Distance(BossPos, GetPlayerPos());

		if (currentDistance > 80.f && currentDistance < 90.f)
		{
			curStance = CB_FSMScript::eBossStance_Weapon::EVADE;

		}
	}



}

void CB_STATEMoveScript::FastWalk_End()
{

}

void CB_STATEMoveScript::Evade_End()
{
	m_fCurForce = m_fPrevForce;

	m_BHQ->SetPlaying(false);
	m_bStorePlayerPos = false; // Move Evade를 할 때 플레이어 위치를 저장하는 시점을 조정하기 위한 변수
	m_bOnEvade = false; // 점프를 할 때는 ReNewPath 함수 호출을 막기 위한 이유 

	int RandomNum = ZeroToOneRandom();

	if (RandomNum == 0)
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::MELEE_ATK);

	else if (RandomNum == 1)
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::AOE);

	m_BHQ->ChangeState((UINT)eB_States::MELEE);

}

void CB_STATEMoveScript::BloodDrink_StartEnd()
{

	m_BHQ->SetPlaying(false);

	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_LOOP);



}

void CB_STATEMoveScript::BloodDrink_LoopEnd()
{
}

void CB_STATEMoveScript::BloodDrink_End()
{
}




void CB_STATEMoveScript::Evade_MoveZero()
{
	m_fCurForce = 0.f;
}

void CB_STATEMoveScript::Evade_MoveRestore()
{
	m_fCurForce = m_fPrevForce;
}

int CB_STATEMoveScript::ZeroToOneRandom()
{
	srand((unsigned int)time(NULL));
	return rand() % 2;
}

bool CB_STATEMoveScript::JustTurn(Vec3 _dir) // 애니메이션이 없는 turn (자잘자잘하게 turn할때는 이 turn으로) 
{
	if (!m_bJustTurnSet)
	{
		m_bJustTurnSet = true;
		m_fTurnDegree = CalTurnDegree(_dir);
		m_vInitialRot = m_BHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += m_fTurnDegree;
		m_fFinishTime = 1.f;
		m_fElapsedTime = 0.f;

		//m_MHQ->Animator3D()->Play(Lurker_IDLESTAND, true);
	}

	m_fElapsedTime += DT * m_fJustTurnSpeed;

	if (m_fElapsedTime < m_fFinishTime)
	{
		Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
		vecRot = TransDegreeToRadVector(vecRot);
		m_BHQ->Transform()->SetRelativeRot(vecRot);
	}
	else
	{
		m_vFinalRot = TransDegreeToRadVector(m_vFinalRot);

		m_BHQ->Transform()->SetRelativeRot(m_vFinalRot);

		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;
		m_bJustTurnSet = false;

		return true;
	}


	return false;
}

bool CB_STATEMoveScript::AniTurn(Vec3 _dir)
{
	if (!m_bTurnSet)
	{
		m_bTurnSet = true;
		m_fTurnDegree = CalTurnDegree(_dir);

		if (0.f <= m_fTurnDegree && m_fTurnDegree < 100.f)
			m_eTurnAni = eTurnAni::R90;
		else if (100 <= m_fTurnDegree && m_fTurnDegree < 160.f)
			m_eTurnAni = eTurnAni::R135;
		else if (160 <= m_fTurnDegree && m_fTurnDegree <= 180.f)
			m_eTurnAni = eTurnAni::L180;
		else if (-100.f <= m_fTurnDegree && m_fTurnDegree < 0.f)
			m_eTurnAni = eTurnAni::L90;
		else if (-135 <= m_fTurnDegree && m_fTurnDegree < -100.f)
			m_eTurnAni = eTurnAni::L135;
		else if (-180 <= m_fTurnDegree && m_fTurnDegree < -135.f)
			m_eTurnAni = eTurnAni::L180;
	}

	switch (m_eTurnAni)
	{
	case CB_STATEMoveScript::eTurnAni::R90:
		if (Turn90R(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CB_STATEMoveScript::eTurnAni::R135:
		if (Turn135R(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CB_STATEMoveScript::eTurnAni::L90:
		if (Turn90L(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CB_STATEMoveScript::eTurnAni::L135:
		if (Turn135L(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CB_STATEMoveScript::eTurnAni::L180:
		if (Turn180(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

bool CB_STATEMoveScript::Turn180(float _degree)
{
	if (!m_bTurnBtn)
	{

		m_BHQ->Animator3D()->Play(B_Turn180_R, false); // 그냥 L R 나누지말고 한개만 사용

		m_vInitialRot = m_BHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += _degree;
		m_fFinishTime = m_BHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * m_fAniTurnSpeed;


	if (!m_BTurned180)
	{
		if (m_fElapsedTime < m_fFinishTime)
		{
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_BHQ->Transform()->SetRelativeRot(vecRot);
		}
		else
		{
			m_fElapsedTime = m_fFinishTime;
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_BHQ->Transform()->SetRelativeRot(vecRot);

			m_BTurned180 = false;
			m_bTurnBtn = false;
			m_vInitialRot = { 0.f, 0.f, 0.f };
			m_vFinalRot = { 0.f,0.f,0.f };
			m_fFinishTime = 0.f;
			m_fElapsedTime = 0.f;

			return true;
		}
	}
	else
	{
		m_BTurned180 = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}

bool CB_STATEMoveScript::Turn90L(float _degree)
{
	if (!m_bTurnBtn)
	{

		m_BHQ->Animator3D()->Play(B_Turn90_L, false);

		m_vInitialRot = m_BHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += _degree;
		m_fFinishTime = m_BHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * m_fAniTurnSpeed;


	if (!m_BTurned90L)
	{
		if (m_fElapsedTime < m_fFinishTime)
		{
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_BHQ->Transform()->SetRelativeRot(vecRot);
		}
		else
		{
			m_fElapsedTime = m_fFinishTime;
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_BHQ->Transform()->SetRelativeRot(vecRot);

			m_BTurned90L = false;
			m_bTurnBtn = false;
			m_vInitialRot = { 0.f, 0.f, 0.f };
			m_vFinalRot = { 0.f,0.f,0.f };
			m_fFinishTime = 0.f;
			m_fElapsedTime = 0.f;

			return true;
		}
	}
	else
	{
		m_BTurned90L = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}

bool CB_STATEMoveScript::Turn90R(float _degree)
{
	if (!m_bTurnBtn)
	{
		m_BHQ->Animator3D()->Play(B_Turn90_R, false);

		m_vInitialRot = m_BHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += _degree;
		m_fFinishTime = m_BHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * m_fAniTurnSpeed;

	if (!m_BTurned90R)
	{
		if (m_fElapsedTime < m_fFinishTime)
		{
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_BHQ->Transform()->SetRelativeRot(vecRot);
		}
		else
		{
			m_fElapsedTime = m_fFinishTime;
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_BHQ->Transform()->SetRelativeRot(vecRot);

			m_BTurned90R = false;
			m_bTurnBtn = false;
			m_vInitialRot = { 0.f, 0.f, 0.f };
			m_vFinalRot = { 0.f,0.f,0.f };
			m_fFinishTime = 0.f;
			m_fElapsedTime = 0.f;

			return true;
		}
	}
	else
	{
		m_BTurned90R = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}

bool CB_STATEMoveScript::Turn135L(float _degree)
{
	if (!m_bTurnBtn)
	{
		m_BHQ->Animator3D()->Play(B_Turn135_L, false);

		m_vInitialRot = m_BHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += _degree;
		m_fFinishTime = m_BHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * m_fAniTurnSpeed;


	if (!m_BTurned135L)
	{
		if (m_fElapsedTime < m_fFinishTime)
		{
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_BHQ->Transform()->SetRelativeRot(vecRot);
		}
		else
		{
			m_fElapsedTime = m_fFinishTime;
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_BHQ->Transform()->SetRelativeRot(vecRot);

			m_BTurned135L = false;
			m_bTurnBtn = false;
			m_vInitialRot = { 0.f, 0.f, 0.f };
			m_vFinalRot = { 0.f,0.f,0.f };
			m_fFinishTime = 0.f;
			m_fElapsedTime = 0.f;

			return true;
		}
	}
	else
	{
		m_BTurned135L = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}

bool CB_STATEMoveScript::Turn135R(float _degree)
{
	if (!m_bTurnBtn)
	{

		m_BHQ->Animator3D()->Play(B_Turn135_R, false);

		m_vInitialRot = m_BHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += _degree;
		m_fFinishTime = m_BHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * m_fAniTurnSpeed;


	if (!m_BTurned135R)
	{
		if (m_fElapsedTime < m_fFinishTime)
		{
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_BHQ->Transform()->SetRelativeRot(vecRot);
		}
		else
		{
			m_fElapsedTime = m_fFinishTime;
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_BHQ->Transform()->SetRelativeRot(vecRot);

			m_BTurned135L = false;
			m_bTurnBtn = false;
			m_vInitialRot = { 0.f, 0.f, 0.f };
			m_vFinalRot = { 0.f,0.f,0.f };
			m_fFinishTime = 0.f;
			m_fElapsedTime = 0.f;

			return true;
		}
	}
	else
	{
		m_BTurned135L = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}

void CB_STATEMoveScript::TurnComplete180()
{
	m_BTurned180 = true;
	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::FAST_WALK);
	m_BHQ->SetPlaying(false);

}

void CB_STATEMoveScript::TurnComplete90L()
{
	m_BTurned90L = true;
	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::FAST_WALK);
	m_BHQ->SetPlaying(false);
}

void CB_STATEMoveScript::TurnComplete90R()
{
	m_BTurned90R = true;
	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::FAST_WALK);
	m_BHQ->SetPlaying(false);
}

void CB_STATEMoveScript::TurnComplete135L()
{
	m_BTurned135L = true;
	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::FAST_WALK);
	m_BHQ->SetPlaying(false);
}

void CB_STATEMoveScript::TurnComplete135R()
{
	m_BTurned135R = true;
	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::FAST_WALK);
	m_BHQ->SetPlaying(false);

}

void CB_STATEMoveScript::RotationReset()
{
	Vec3 rot = m_BHQ->Transform()->GetRelativeRot();
	TransRadToDegreeVector(rot);
	if (rot.y > 1080.f)
	{
		rot.y = fmod(rot.y, 360.f);

		rot = TransDegreeToRadVector(rot);
		m_BHQ->Transform()->SetRelativeRot(rot);
	}
	else if (rot.y < -1080.f)
	{
		rot.y = fmod(rot.y, 360.f);

		rot = TransDegreeToRadVector(rot);
		m_BHQ->Transform()->SetRelativeRot(rot);
	}
}

void CB_STATEMoveScript::RenewPath()
{
	if (m_CMoveScript->RenewPath())
	{
		m_bPathRenewed = true;
		RotationReset();
	}

	if (m_bPathRenewed) //&& m_CMoveScript->IsJustTurned() > 0)
	{
		if (!m_bGetDirOnce)
		{
			m_vTurndir = CalMoveDir();
			m_bGetDirOnce = true;
		}

		if (JustTurn(m_vTurndir))
		{
			m_bPathRenewed = false;
			m_bGetDirOnce = false;
		}
	}
}

void CB_STATEMoveScript::CatchCheck() // move에서 근접 공격 state로 넘어가는 함수임.
{

	if (DistBetwPlayer() > 850.f)
	{
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::EVADE);
	}

	else if (DistBetwPlayer() < 250.f)
	{
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::MELEE_ATK);
	}




	// 먼거리일 경우, ChangeState로 해준다. 
	if (m_iCatchStage == 0)
	{

		CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

		float dist = DistBetwPlayer();
		switch (curStance)
		{
		case CB_FSMScript::eBossStance_Weapon::EVADE:
		{
			m_vToPlayerDir = GetToPlayerDir(); // 플레이어의 현재 방향을 갖고 온다.
			m_iCatchStage++;
		}
		break;

		case CB_FSMScript::eBossStance_Weapon::MELEE_ATK:
		{
			m_vToPlayerDir = GetToPlayerDir(); // 플레이어의 현재 방향을 갖고 온다.
			m_iCatchStage++;
		}

		break;

		case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START:
			break;
		case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_LOOP:
			break;
		case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_END:
			break;
		case CB_FSMScript::eBossStance_Weapon::END:
			break;
		}
	}

	else if (m_iCatchStage == 1)
	{
		CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

		switch (curStance)
		{
		case CB_FSMScript::eBossStance_Weapon::EVADE:
		{
			if (JustTurn(m_vToPlayerDir)) // 플레이어 방향으로 몸을 튼다.
				m_iCatchStage++;
		}
		break;

		case CB_FSMScript::eBossStance_Weapon::MELEE_ATK:
		{
			if (JustTurn(m_vToPlayerDir)) // 플레이어 방향으로 몸을 튼다.
				m_iCatchStage++;
		}
		break;
		case CB_FSMScript::eBossStance_Weapon::AOE:
			break;
		case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START:
			break;
		case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_LOOP:
			break;
		case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_END:
			break;
		case CB_FSMScript::eBossStance_Weapon::END:
			break;
		}
	}


	else if (m_iCatchStage == 2)
	{
		CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

		switch (curStance)
		{
		case CB_FSMScript::eBossStance_Weapon::EVADE:
		{
			m_CMoveScript->MoveOn(false); // 멈춘다.
			//m_BHQ->SetPlaying(false); // 애니메이션 작동되도록 false로 바꿔줌

			m_bOnEvade = true; // renewPath 함수 호출을 막기 위해 true

			m_iCatchStage = 0;
		}
		break;

		case CB_FSMScript::eBossStance_Weapon::MELEE_ATK:
		{
			m_CMoveScript->MoveOn(false); // 멈춘다.
			m_BHQ->ChangeState((UINT)eB_States::MELEE); // 근접 공격 상태로 바꾼다. 
			m_BHQ->SetPlaying(false); // 애니메이션 작동되도록 false로 바꿔줌

			m_iCatchStage = 0;
		}
		break;

		case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START:
			break;
		case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_LOOP:
			break;
		case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_END:
			break;
		case CB_FSMScript::eBossStance_Weapon::END:
			break;
		}





	}

	//	switch (m_eMeleeState)
	//	{
	//	case eMeleeState::Dash:
	//		if (DistBetwPlayer() < 450.f)
	//		{
	//			m_vToPlayerDir = GetToPlayerDir();
	//			m_iCatchStage++;
	//		}
	//		break;
	//	case eMeleeState::Heavy1:
	//		if (DistBetwPlayer() < 350.f)
	//		{
	//			m_vToPlayerDir = GetToPlayerDir();
	//			m_iCatchStage++;
	//		}
	//		break;
	//	case eMeleeState::Slash:
	//		if (DistBetwPlayer() < 250.f)
	//		{
	//			m_vToPlayerDir = GetToPlayerDir();
	//			m_iCatchStage++;
	//		}
	//		break;
	//	default:
	//		break;
	//	}
	//}
	//else if (m_iCatchStage == 1) // 여기서 turn해줌 
	//{
	//	switch (m_eMeleeState)
	//	{
	//	case eMeleeState::Dash:
	//		if (JustTurn(m_vToPlayerDir)) // JustTurn은 bool값임 
	//			m_iCatchStage++;
	//		break;
	//	case eMeleeState::Heavy1:
	//		if (JustTurn(m_vToPlayerDir))
	//			m_iCatchStage++;
	//		break;
	//	case eMeleeState::Slash:
	//		if (JustTurn(m_vToPlayerDir))
	//			m_iCatchStage++;
	//		break;
	//	default:
	//		break;
	//	}
	//}
	//else if (m_iCatchStage == 2)
	//{
	//	switch (m_eMeleeState)
	//	{
	//	case eMeleeState::Dash:
	//		m_CMoveScript->MoveOn(false);
	//		m_MHQ->ChangeState((UINT)eM_States::MELEE);
	//		m_iCatchStage = 0;
	//		break;
	//	case eMeleeState::Heavy1:
	//		m_CMoveScript->MoveOn(false);
	//		m_MHQ->ChangeState((UINT)eM_States::MELEE);
	//		m_iCatchStage = 0;
	//		break;
	//	case eMeleeState::Slash:
	//		m_CMoveScript->MoveOn(false);
	//		m_MHQ->ChangeState((UINT)eM_States::MELEE);
	//		m_iCatchStage = 0;
	//		break;
	//	default:
	//		break;
	//	}
	//}

}

bool CB_STATEMoveScript::MoveStart()
{
	m_CMoveScript->SetAndGetPath(GetPlayer()); // 길찾기랑 관련있음 (꼭 해줘야함) 

	RotationReset(); // 회전 초기화
	float prevDegree = CalTurnDegree(m_vTurndir);

	// 회전할 방향 계산 
	m_vTurndir = CalMoveDir();
	float curDegree = CalTurnDegree(m_vTurndir);
	// 회전할 각도 계산 
	float diff = abs(prevDegree - curDegree);

	// 큰 각도일 때만 애니메이션으로 회전하게 하는게 자연스러워서 아래처럼 구분함 
	if (diff > 70.f)
		m_bUseAniTurn = true;
	else
		m_bUseAniTurn = false;

	if (diff < 10.f && DistBetwPlayer() < 200.f)
		return true;

	m_bMoveStartOnce = true;


	if (m_bUseAniTurn)
	{
		if (AniTurn(m_vTurndir))
		{
			m_CMoveScript->MoveOn(true);
			m_bMoveStartOnce = false;
			m_bEnterSign = false;

			return true;
		}
	}
	else
	{
		if (JustTurn(m_vTurndir))
		{
			m_CMoveScript->MoveOn(true);
			m_bMoveStartOnce = false;
			m_bEnterSign = false;

			return true;
		}
	}

	return false;

	if (!m_bMoveStartOnce)
	{
		m_bMoveStartOnce = true;
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::FAST_WALK);
		m_BHQ->SetPlaying(true);
	}

	return false;
}

Vec3 CB_STATEMoveScript::CalMoveDir()
{

	std::stack<Vec3>* stack = m_CMoveScript->GetStack();

	if (stack->size() <= 2)
		return Vec3(0.f, 0.f, 0.f);

	Vec3 startPos = stack->top();
	stack->pop();
	Vec3 secondPos = stack->top();
	stack->push(startPos);

	Vec3 dir = secondPos - startPos;
	dir = dir.Normalize();

	return dir;
}

void CB_STATEMoveScript::GetBack()
{
	if (m_iGetBackStage == 0)
	{
		m_vToPlayerDir = GetToPlayerDir();
		m_iGetBackStage++;
	}
	else if (m_iGetBackStage == 1)
	{
		if (JustTurn(m_vToPlayerDir))
		{
			m_iGetBackStage++;
		}
	}
	else if (m_iGetBackStage == 2) // 몬스터가 얘는 
	{
		// 뒤로 이동
		/*Vec3 myPos = GetMyPos();
		myPos += GetMyFront() * DT * 100.f;
		m_BHQ->Transform()->SetRelativePos(myPos);*/
	}
}

bool CB_STATEMoveScript::DistCheck()
{
	// 거리 값은 내가 알아서 조정하기 

	float test = DistBetwPlayer();

	// 근거리에 플레이어가 있는지 (DistBetwPlayer가 작다면)
	if (DistBetwPlayer() < 200.f)
		return true;
	else
		return false;

}

void CB_STATEMoveScript::Clear()
{

	m_bTurnSet = false;
	m_BTurned180 = false;
	m_BTurned90L = false;
	m_BTurned90R = false;
	m_BTurned135L = false;
	m_BTurned135R = false;
	m_bTurnBtn = false;
	m_bMoveStartOnce = false;
	m_bEnterSign = false;
	m_bTurnAtStart = false;
	m_bPathRenewed = false;
	m_bGetDirOnce = false;
	m_bTurnAtRenew = false;
	m_fElapsedTime = 0.f;
	m_iCatchStage = 0;
}








