#include "pch.h"
#include "CB_STATEMoveScript.h"
#include "CB_FSMScript.h"
#include "CMonsterMoveScript.h"
#include <cstdlib> // rand(), srand() �Լ��� ����ϱ� ���� ���
#include <ctime>   // time() �Լ��� ����ϱ� ���� ���




CB_STATEMoveScript::CB_STATEMoveScript()
	: m_DirSetCount(0)
	, m_GetWeapon(false)
	, m_fPrevForce(0.f)
	, m_fCurForce(0.f)
	, m_OnceEvade(false)
	, m_OnceSpawnSpell(false)
	, m_bBloodDrink(false)
	, m_iCatchStage(0)
	, m_fAniTurnSpeed(1.f)  // turn �ִϸ��̼����� turn�ϴ� �ӵ� 
	, m_fJustTurnSpeed(2.f) // �ִϸ��̼��� ���� trun�ϴ� �ӵ�
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
	m_BHQ->Animator3D()->ActionEvent(B_Melee_Evade_F, 46) = std::bind(&CB_STATEMoveScript::Evade_MoveZero, this); // 48�� �� 

	m_BHQ->Animator3D()->CompleteEvent(B_BloodDrink_Start) = std::bind(&CB_STATEMoveScript::BloodDrink_StartEnd, this);
	m_BHQ->Animator3D()->CompleteEvent(B_BloodDrink_Loop) = std::bind(&CB_STATEMoveScript::BloodDrink_LoopEnd, this);


}



void CB_STATEMoveScript::Enter()
{
	m_bEnterSign = true;

	// �ٹ濡 �÷��̾ �ִ��� Ž�� (ĳ���Ϳ� ��ġ�� �ʰ� �ϱ� ����
	if (DistCheck())
		m_bEnterDistCheck = true;

	//=== �׽�Ʈ �뵵�� �صױ� ������ �Ʒ� �� ���� �׽�Ʈ ������ �����
   /*m_GetWeapon = false;
   m_BHQ->SetStance_NoWeapon(CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK);*/

	m_GetWeapon = true;
	m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::EVADE);

}


void CB_STATEMoveScript::tick()
{



	//====================


		// ������ front�� ���ٰ� ���������� ȸ���� �ߴٸ� ����� ��� ȸ���ؾߵɱ�?
		// ȸ���� �� ���� ȸ����������Ѵ�. 


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


	// 2������ ����
	if (m_ratio < 0.7f)
	{
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::FAST_WALK);
		m_GetWeapon = true;
		m_IsPhase_1 = false;
		return;
	}

	else if (m_ratio < 0.4f) // �� ���ݾ��� ������ �ϱ�. 2������ ���ؿ� �������� �ʵ���
	{
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START);
	}




	// Blood drink ���� üũ ���� 
	if (WeaponCurStance == CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START
		&& m_bBloodDrink)
	{
		Vec3 BossPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();

		// *** �÷��̾ ������ ��ġ�� �����ߴٸ� �ٲ۴�. ���⼭�� ���� ��ġ�� ���߿� ui�� �����غ��� �ϱ� 
		if (BossPos == Vec3(0, 0, 0))
		{
			m_BHQ->SetPlaying(false); // ���� �������� �𸣹Ƿ� �ǽð����� üũ�ϴٰ� false�� �ϰԲ� ���ش�. 
			m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_LOOP); // loop�� ���õȰŴ� Heal ��ũ��Ʈ���� ó���Ѵ�. �̸� ���� chagestate����
			m_BHQ->ChangeState((UINT)eB_States::HEAL);

		}

	}


	// ============================================
	// �÷��̾���� �Ÿ��� üũ�ؼ� melee ������ ���� ���� ��
	float dist = DistBetwPlayer(); // 1395
	if (dist < 180.f)
		m_CMoveScript->SetNearPlayer(true);
	else
		m_CMoveScript->SetNearPlayer(false);

	if (!m_IsPhase_1 || m_bBloodDrink)
	{
		// move���·� ó�� ������ �� ĳ���Ͱ� �ٹ濡 �־��ٸ�
		if (m_bEnterDistCheck)
		{
			if (dist < 180.f)
				GetBack(); // �÷��̾�� ��ġ�� �ʰ� �ణ �Ÿ� ���� 

			else
			{
				m_bEnterDistCheck = false;
				m_iGetBackStage = 0;
			}
		}

		// move���·� ó�� ������ �� ĳ���Ͱ� �ٹ濡 �����ٸ�
		else
		{
			//  move���·� ���� �� ��ȯ�� ���¶� enter �Լ��� ���� �����ӿ� ȣ���߾��ٸ�
			if (m_bEnterSign)
			{
				// �����̱� �����ߴٸ� 
				if (MoveStart())
					m_bEnterSign = false;
			}

			// move ���� �״�ζ��
			else
			{
				float dist = DistBetwPlayer();

				// ���� ���� ã�´�.
				if (m_iCatchStage == 0 && !m_bOnEvade)
					RenewPath();

			}
		}

	}

	// ������ �ð����� �������� ����ȴ�. 
	// �� ó���� B_Walk_FL�� ����ǰ� �ٲ����.
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

		// stance�� �ٲٴ� �Լ��� �ϳ� �߰��ؾ��ҵ�. 	stance�ٲٴ� �Ŵ� complete event���� �ؾ��ҵ�.
		Weapon_Move();

		// m_iCatchStage�� 0�� �ƴ϶��
		CatchCheck(); // move���� ���� ���� state�� �Ѿ�� �Լ���.

		if (IsPlaying)
			return;

		CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

		// ������ ������ �ǰ����� ������ �ʿ��ҵ� (�÷��̾��� �� �յ� ��������????)
		// �÷��̾��� ü���� ��ü ��� 20%, 40%�̸� Blood drink�� �Ѵ�. ������ GetWeapon�� false�� ���� ���� ����. 
		// �׸��� ������ ��ġ�� �÷��̾ �����̰�, �� ��ġ�� �����ϸ� Blood drink�� �Ѵ�.
		// �̸� ���ؼ� fast walk�� ���� �ؾ��ϴµ� �� �κ��� �� �����غ���. blood drink�� ���� �ٲܶ� fast walk�� ���� ��ȯ�ϰ�
		// bool ���� �����ְ� ������ ��ġ�� �ٴٸ��� blood drink ���·� �ٲٴ°� ���� fast_walk �ȿ��� ������ �ƴϸ� �ٸ�������
		// ������ ��
		if (curStance == CB_FSMScript::eBossStance_Weapon::FAST_WALK)
		{
			m_BHQ->PlayAnim(B_Walk_Fast_F, true);
			// ��� ����, ȸ�� ������ ������ �� ���·� �⺻���� ���ư��� �Ѵ�. 

		}


		// �÷��̾���� �Ÿ��� ��û �� ���� �� state�� ������ ����. 
		else if (curStance == CB_FSMScript::eBossStance_Weapon::EVADE)
		{

			m_BHQ->PlayAnim(B_Melee_Evade_F, false); // �ִϸ��̼��� ���� �� ������ ������ �ƴ� 

		}

		else if (curStance == CB_FSMScript::eBossStance_Weapon::MELEE_ATK)
		{
			// ���� 1,2�� �������� ���� (L, R ���ݵ� ����)
			m_BHQ->PlayAnim(B_Walk_B, false);

		}
		else if (curStance == CB_FSMScript::eBossStance_Weapon::AOE)
		{
			// �ٰŸ� ���� �� �ϳ�. MELEE_ATK�� AOE�� �� ���´� �������� �ϰ� ����. 
			m_BHQ->PlayAnim(B_Walk_BL, false);


		}
		else if (curStance == CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START)
		{
			m_bBloodDrink = true;

			// �� STATE�� ������, ����� �����ϱ� ���� ��� ���� �Ÿ��� ������ �ȴٰ� �ش� ���� ����. (START-LOOP-END)
			m_BHQ->PlayAnim(B_Walk_Fast_F, true);


		}

		else if (curStance == CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_END)
		{
			// ING�϶��� BloodDrink���� ó�� 
			m_BHQ->PlayAnim(B_BloodDrink_Start, false);

		}

		m_BHQ->SetPlaying(true);

	}

	// �ش� �����϶��� ���� �����Ѵ�. (���� ��� x)
	if (NoWeaponCurStance == CB_FSMScript::eBossStance_NoWeapon::SPAWN_SPELL)
	{

		if (m_OnceSpawnSpell)
			return;

		m_OnceSpawnSpell = true; // ��ü ��ũ��Ʈ exit���� false�� �ٲ��ֱ�

		// �Ʒ����� ��ü�� �����ϴ� �ڵ� �ۼ�. �׸��� ��ü ��ũ��Ʈ ������Ʈ�� ���ٺ��δ�. (���� ����x ĳ���Ͱ� ���������ϹǷ�)
		// ���ÿ� ��ü 2���� �����Ѵ�. (�÷��̾��� �¿쿡)

	}


	// ============= ȸ��
	// ��Ÿ���� �� �ư�, ��ħ �÷��̾��� �������� ĳ���͸� �ٶ󺸰� �ִ� ��Ȳ�̶��?(����ĳ��Ʈ�� TRUE���)
	//if (m_EvadeCoolTime.IsFinish()) // && 
	//{
		// Evade �ִϸ��̼��� ����Ѵ�. 

	//	// �ִϸ��̼� ����� ������ �ٽ� move ���·� ���ư���. (���ø�Ʈ �̺�Ʈ)
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
	rf.Normalize(); //  ���� ����Ű�� �뵵�̹Ƿ� ���� ����ȭ

	Vec3 lf = -vRight + vFront;
	lf.Normalize(); // ���� ����Ű�� �뵵�̹Ƿ� ���� ����ȭ

	// ==== stance�� �̵� �ӵ�
	float Force = 0.f;
	CB_FSMScript::tB_Info BossInfo = m_BHQ->GetBossInfo();
	CB_FSMScript::eBossStance_NoWeapon curStance = m_BHQ->GetStance_NoWeapon();

	if (CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK == curStance)
		Force = BossInfo.B_Stat.MoveSpeed * ScaleDT;




	// ===== �������� �̵�

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


	// ===== �������� �̵�
	Vec3 vBossFront = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vRight = m_BHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	Vec3 vBossCurPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();

	switch (curStance)
	{
	case CB_FSMScript::eBossStance_Weapon::FAST_WALK:
	{
		// ������ front ���⿡ �÷��̾ �־��ٸ� �̰ɷ�. �÷��̾ ������ back���⿡ �־��ٸ� += ���� �ؾ���


	// blood rink�϶��� ����Ѵ� �̷������� �ϱ�
	//	vBossCurPos -= vBossFront  * m_fCurForce;
	}
	break;

	case CB_FSMScript::eBossStance_Weapon::EVADE:
	{
		// �÷��̾���� �Ÿ��� ��� ���� �ٴٸ��� �÷��̾� ������ �̵��Ѵ�.
		// �÷��̾��� ��ġ�� �޾ƿͼ� �Ÿ��� üũ�Ѵ�.
		//m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::EVADE);

		// �÷��̾� ������ �̵��Ѵ�.
		// ������ �̵��ϴٰ� �÷��̾� ��ġ�� �̸��� �����.
		if (!m_bStorePlayerPos)
		{
			m_vPlayerPos = GetPlayerPos();

			m_vPlayerPos.z = m_vPlayerPos.z + 120.f; // ������ �� �÷��̾�� ��ġ�� �ȵǹǷ� �ణ�� ���� ������ �д�. 

			m_bStorePlayerPos = true;
		}

		Vec3 BossToPlayerDir = m_vPlayerPos - vBossCurPos;
		BossToPlayerDir.Normalize();

		vBossCurPos += -vBossFront * m_fCurForce;
		//m_BHQ->GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f)); // ȸ���� ȥ�� �Ǳ淡 ���� ����

	}
	break;

	case CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK_START:
	{

		Vec3 BossCurPos = m_BHQ->GetOwner()->Transform()->GetRelativePos();

		// ������ ��ġ���� �̵��Ѵ�. (�ϴ� 0,0,0���� �ص�)
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

void CB_STATEMoveScript::Weapon_StanceEvent() // ���� �Ⱦ� 
{

	// �̰Ŵ� Move ���³������� stance�� �ٲٴ� �Լ���.

	m_BHQ->SetPlaying(false);



	CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();
	if (curStance == CB_FSMScript::eBossStance_Weapon::FAST_WALK)
	{
		// ��� ��ü�� �÷��̾� ������ ���� �Ÿ�
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
	m_bStorePlayerPos = false; // Move Evade�� �� �� �÷��̾� ��ġ�� �����ϴ� ������ �����ϱ� ���� ����
	m_bOnEvade = false; // ������ �� ���� ReNewPath �Լ� ȣ���� ���� ���� ���� 

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

bool CB_STATEMoveScript::JustTurn(Vec3 _dir) // �ִϸ��̼��� ���� turn (���������ϰ� turn�Ҷ��� �� turn����) 
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

		m_BHQ->Animator3D()->Play(B_Turn180_R, false); // �׳� L R ���������� �Ѱ��� ���

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

void CB_STATEMoveScript::CatchCheck() // move���� ���� ���� state�� �Ѿ�� �Լ���.
{

	if (DistBetwPlayer() > 850.f)
	{
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::EVADE);
	}

	else if (DistBetwPlayer() < 250.f)
	{
		m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::MELEE_ATK);
	}




	// �հŸ��� ���, ChangeState�� ���ش�. 
	if (m_iCatchStage == 0)
	{

		CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

		float dist = DistBetwPlayer();
		switch (curStance)
		{
		case CB_FSMScript::eBossStance_Weapon::EVADE:
		{
			m_vToPlayerDir = GetToPlayerDir(); // �÷��̾��� ���� ������ ���� �´�.
			m_iCatchStage++;
		}
		break;

		case CB_FSMScript::eBossStance_Weapon::MELEE_ATK:
		{
			m_vToPlayerDir = GetToPlayerDir(); // �÷��̾��� ���� ������ ���� �´�.
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
			if (JustTurn(m_vToPlayerDir)) // �÷��̾� �������� ���� ư��.
				m_iCatchStage++;
		}
		break;

		case CB_FSMScript::eBossStance_Weapon::MELEE_ATK:
		{
			if (JustTurn(m_vToPlayerDir)) // �÷��̾� �������� ���� ư��.
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
			m_CMoveScript->MoveOn(false); // �����.
			//m_BHQ->SetPlaying(false); // �ִϸ��̼� �۵��ǵ��� false�� �ٲ���

			m_bOnEvade = true; // renewPath �Լ� ȣ���� ���� ���� true

			m_iCatchStage = 0;
		}
		break;

		case CB_FSMScript::eBossStance_Weapon::MELEE_ATK:
		{
			m_CMoveScript->MoveOn(false); // �����.
			m_BHQ->ChangeState((UINT)eB_States::MELEE); // ���� ���� ���·� �ٲ۴�. 
			m_BHQ->SetPlaying(false); // �ִϸ��̼� �۵��ǵ��� false�� �ٲ���

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
	//else if (m_iCatchStage == 1) // ���⼭ turn���� 
	//{
	//	switch (m_eMeleeState)
	//	{
	//	case eMeleeState::Dash:
	//		if (JustTurn(m_vToPlayerDir)) // JustTurn�� bool���� 
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
	m_CMoveScript->SetAndGetPath(GetPlayer()); // ��ã��� �������� (�� �������) 

	RotationReset(); // ȸ�� �ʱ�ȭ
	float prevDegree = CalTurnDegree(m_vTurndir);

	// ȸ���� ���� ��� 
	m_vTurndir = CalMoveDir();
	float curDegree = CalTurnDegree(m_vTurndir);
	// ȸ���� ���� ��� 
	float diff = abs(prevDegree - curDegree);

	// ū ������ ���� �ִϸ��̼����� ȸ���ϰ� �ϴ°� �ڿ��������� �Ʒ�ó�� ������ 
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
	else if (m_iGetBackStage == 2) // ���Ͱ� ��� 
	{
		// �ڷ� �̵�
		/*Vec3 myPos = GetMyPos();
		myPos += GetMyFront() * DT * 100.f;
		m_BHQ->Transform()->SetRelativePos(myPos);*/
	}
}

bool CB_STATEMoveScript::DistCheck()
{
	// �Ÿ� ���� ���� �˾Ƽ� �����ϱ� 

	float test = DistBetwPlayer();

	// �ٰŸ��� �÷��̾ �ִ��� (DistBetwPlayer�� �۴ٸ�)
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








