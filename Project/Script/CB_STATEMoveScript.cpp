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



	// ������ �ð����� �������� ����ȴ�. 
	// �� ó���� B_Walk_FL�� ����ǰ� �ٲ����.
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
		
			// stance�� �ٲٴ� �Լ��� �ϳ� �߰��ؾ��ҵ�. 	stance�ٲٴ� �Ŵ� complete event���� �ؾ��ҵ�.
			Weapon_Move();
			

			if (IsPlaying)
				return;

			CB_FSMScript::eBossStance_Weapon curStance = m_BHQ->GetStance_Weapon();

			// ������ ������ �ǰ����� ������ �ʿ��ҵ� (�÷��̾��� �� �յ� ��������????)
			if (curStance == CB_FSMScript::eBossStance_Weapon::FAST_WALK)
			{
				m_BHQ->PlayAnim(B_Walk_Fast_F, true);
				// ��� ����, ȸ�� ������ ������ �� ���·� �⺻���� ���ư��� �Ѵ�. 
			
			}


			// �÷��̾���� �Ÿ��� ��û �� ���� �� state�� ������ ����. 
			else if (curStance == CB_FSMScript::eBossStance_Weapon::EVADE)
			{

				m_BHQ->PlayAnim(B_Melee_Evade_F, false); 

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
			else if (curStance == CB_FSMScript::eBossStance_Weapon::BLOOD_DRINK)
			{
				// �� STATE�� ������, ����� �����ϱ� ���� ��� ���� �Ÿ��� ������ �ȴٰ� �ش� ���� ����. (START-LOOP-END)
				m_BHQ->PlayAnim(B_Walk_BR, false);


			}

			m_BHQ->SetPlaying(true);
		
		}
		

	// ============= ȸ��
	// ��Ÿ���� �� �ư�, ��ħ �÷��̾��� �������� ĳ���͸� �ٶ󺸰� �ִ� ��Ȳ�̶��?(����ĳ��Ʈ�� TRUE���)
	//if (m_EvadeCoolTime.IsFinish()) // && 
	//{
		// Evade �ִϸ��̼��� ����Ѵ�. 

	//	// �ִϸ��̼� ����� ������ �ٽ� move ���·� ���ư���. (���ø�Ʈ �̺�Ʈ)
	//}
}

void CB_STATEMoveScript::Enter()
{
	// === �׽�Ʈ �뵵�� �صױ� ������ �Ʒ� �� ���� �׽�Ʈ ������ �����
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
	rf.Normalize(); //  ���� ����Ű�� �뵵�̹Ƿ� ���� ����ȭ

	Vec3 lf = -vRight + vFront;
	lf.Normalize(); // ���� ����Ű�� �뵵�̹Ƿ� ���� ����ȭ

	// ==== stance�� �̵� �ӵ�
	float Force = 0.f;
	CB_FSMScript::tB_Info BossInfo = m_BHQ->GetBossInfo();
	CB_FSMScript::eBossStance_NoWeapon curStance = m_BHQ->GetStance_NoWeapon();

	if (CB_FSMScript::eBossStance_NoWeapon::NORMAL_WALK == curStance)
		Force = BossInfo.B_Stat.MoveSpeed * ScaleDT;

	/*else if (CB_FSMScript::eBossStance::FAST_WALK == curStance)
		Force = BossInfo.B_Stat.MoveSpeed * ScaleDT * 1.5f;*/


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
			// ������ front ���⿡ �÷��̾ �־��ٸ� �̰ɷ�. �÷��̾ ������ back���⿡ �־��ٸ� += ���� �ؾ���
			vBossCurPos -= vBossFront * m_fCurForce;
			break;

		case CB_FSMScript::eBossStance_Weapon::EVADE:
		{
			// �÷��̾���� �Ÿ��� ��� ���� �ٴٸ��� �÷��̾� ������ �̵��Ѵ�.
			// �÷��̾��� ��ġ�� �޾ƿͼ� �Ÿ��� üũ�Ѵ�.
			m_BHQ->SetStance_Weapon(CB_FSMScript::eBossStance_Weapon::EVADE);
		
		
			// �÷��̾� ������ �̵��Ѵ�.
			// ������ �̵��ϴٰ� �÷��̾� ��ġ�� �̸��� �����.
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
		// ��� ��ü�� �÷��̾� ������ ���� �Ÿ�
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








