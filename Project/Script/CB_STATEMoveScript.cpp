#include "pch.h"
#include "CB_STATEMoveScript.h"
#include "CB_FSMScript.h"




CB_STATEMoveScript::CB_STATEMoveScript()
	: m_DirSetCount(0)
	, m_GetWeapon(false)
	, m_fPrevForce(0.f)
	, m_fCurForce(0.f)
	, m_OnceEvade(false)
{
	SetStateType(static_cast<UINT>(eB_States::MOVE));
}

CB_STATEMoveScript::~CB_STATEMoveScript()
{
}

void CB_STATEMoveScript::begin()
{
	CB_StateScript::begin();

	m_BHQ->Animator3D()->CompleteEvent(B_Walk_Fast_F) = std::bind(&CB_STATEMoveScript::Weapon_StanceEvent, this);
	
	m_BHQ->Animator3D()->CompleteEvent(B_Melee_Evade_F) = std::bind(&CB_STATEMoveScript::Weapon_StanceEvent, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Evade_F, 0) = std::bind(&CB_STATEMoveScript::Evade_MoveZero, this);
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Evade_F, 9) = std::bind(&CB_STATEMoveScript::Evade_MoveRestore, this);
}

void CB_STATEMoveScript::tick()
{
	


//====================
	

	
	
	
	CB_FSMScript::eBossMoveDir curDir = m_BHQ->GetMoveDir();
	bool IsPlaying = m_BHQ->IsPlaying();
	CB_FSMScript::tB_Info BossInfo = m_BHQ->GetBossInfo();
	int BossHP = BossInfo.B_Health.CurHp;

	m_ratio = (float)BossHP / (float)BossInfo.B_Health.MaxHp;

	
	if (m_ratio < 0.7f)
	{
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::FAST_WALK);
		m_GetWeapon = true;
		return;
	}



	// 지정한 시간동안 랜덤으로 재생된다. 
	// 맨 처음엔 B_Walk_FL가 재생되게 바꿔놨음.
	bool test = IsPlaying;


		if (!m_GetWeapon)
		{

			NoWeapon_MoveToDir();

			if (IsPlaying)
				return;

			if (curDir == CB_FSMScript::eBossMoveDir::LF)
			{
		
				m_BHQ->PlayAnim(B_Walk_FL, false);
				m_BHQ->SetPlaying(true);

			}


			else if (curDir == CB_FSMScript::eBossMoveDir::RF)
			{
			
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
				m_BHQ->PlayAnim(B_Walk_B, false);

				m_BHQ->SetPlaying(true);

			}
			else if (curDir == CB_FSMScript::eBossMoveDir::LB)
			{
				m_BHQ->PlayAnim(B_Walk_BL, false);
				m_BHQ->SetPlaying(true);

			}
			else if (curDir == CB_FSMScript::eBossMoveDir::RB)
			{
				m_BHQ->PlayAnim(B_Walk_BR, false);
				m_BHQ->SetPlaying(true);

			}

			else if (curDir == CB_FSMScript::eBossMoveDir::R)
			{
				m_BHQ->PlayAnim(B_Walk_FR, false);
				m_BHQ->SetPlaying(true);
			}

			else if (curDir == CB_FSMScript::eBossMoveDir::L)
			{
				m_BHQ->PlayAnim(B_Walk_FL, false);
				m_BHQ->SetPlaying(true);
			}

		}

		else if (m_GetWeapon)
		{
		
			// stance를 바꾸는 함수도 하나 추가해야할듯. 	stance바꾸는 거는 complete event에서 해야할듯.
			Weapon_Move();
			

			if (IsPlaying)
				return;

			CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

			// 앞인지 뒤인지 판가름할 기준이 필요할듯 (플레이어의 등 앞뒤 기준으로????)
			if (curStance == CB_FSMScript::eBossStance_Weapon::FAST_WALK)
			{
				m_BHQ->PlayAnim(B_Walk_Fast_F, true);
				// 모든 공격, 회복 패턴이 끝나면 이 상태로 기본으로 돌아가게 한다. 
			
			}


			// 플레이어와의 거리가 엄청 멀 때만 이 state로 들어오게 하자. 
			else if (curStance == CB_FSMScript::eBossStance_Weapon::EVADE)
			{

				m_BHQ->PlayAnim(B_Melee_Evade_F, false); 

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
			else if (curStance == CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK)
			{
				// 이 STATE에 들어오면, 모션을 진행하기 전에 어느 정도 거리를 벌리고 걷다가 해당 상태 진행. (START-LOOP-END)
				m_BHQ->PlayAnim(B_Walk_BR, false);


			}

			m_BHQ->SetPlaying(true);
		
		}
		

	// ============= 회피
	// 쿨타임이 다 됐고, 마침 플레이어의 조준점이 캐릭터를 바라보고 있는 상황이라면?(레이캐스트가 TRUE라면)
	//if (m_EvadeCoolTime.IsFinish()) // && 
	//{
		// Evade 애니메이션을 재생한다. 

	//	// 애니메이션 재생이 끝나면 다시 move 상태로 돌아간다. (컴플리트 이벤트)
	//}
}

void CB_STATEMoveScript::Enter()
{
	// === 테스트 용도로 해뒀기 때문에 아래 두 줄은 테스트 끝나면 지우기
	m_GetWeapon = true;
	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::EVADE);
}

void CB_STATEMoveScript::Exit()
{
	m_OnceEvade = false;
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

	/*else if (CB_FSMScript::eBossStance::FAST_WALK == curStance)
		Force = BossInfo.B_Stat.MoveSpeed * ScaleDT * 1.5f;*/


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
			// 몬스터의 front 방향에 플레이어가 있었다면 이걸로. 플레이어가 몬스터의 back방향에 있었다면 += 으로 해야함
			vBossCurPos -= vBossFront * m_fCurForce;
			break;

		case CB_FSMScript::eBossStance_Weapon::EVADE:
		{
			// 플레이어와의 거리가 어느 정도 다다르면 플레이어 앞으로 이동한다.
			// 플레이어의 위치를 받아와서 거리를 체크한다.
			m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::EVADE);
		
		
			// 플레이어 쪽으로 이동한다.
			// 앞으로 이동하다가 플레이어 위치에 이르면 멈춘다.
			Vec3 PlayerPos = GetPlayerPos(); 

			Vec3 BossToPlayerDir = PlayerPos - vBossCurPos;
			BossToPlayerDir.Normalize();

			vBossCurPos += vBossFront * BossToPlayerDir * m_fCurForce;
	
		}
			break;



		case CB_FSMScript::eBossStance_Weapon::MELEE_ATK:
		{
	
		}
			break;


		case CB_FSMScript::eBossStance_Weapon::AOE:
			break;


		case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK:
			break;


		case CB_FSMScript::eBossStance_Weapon::END:
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

	if (CB_FSMScript::eBossStance_Weapon::FAST_WALK == curStance)
	{
		m_fCurForce = BossInfo.B_Stat.MoveSpeed * ScaleDT * 1.5f;
	
	}

	else if (CB_FSMScript::eBossStance_Weapon::EVADE == curStance)
	{
		m_fCurForce = BossInfo.B_Stat.MoveSpeed * ScaleDT * 20.f;

	}

	m_fPrevForce = m_fCurForce;
	m_OnceEvade = true; 
}

void CB_STATEMoveScript::Weapon_StanceEvent()
{
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

	else if (curStance == CB_FSMScript::eBossStance_Weapon::EVADE)
	{
		m_BHQ->ChangeState(static_cast<UINT>(eB_States::MELEE));
	}

}




void CB_STATEMoveScript::Evade_MoveZero()
{
	m_fCurForce = 0.f;
}

void CB_STATEMoveScript::Evade_MoveRestore()
{
	m_fCurForce = m_fPrevForce;
}








