#include "pch.h"
#include "CB_STATEMoveScript.h"
#include "CB_FSMScript.h"




CB_STATEMoveScript::CB_STATEMoveScript()
	: m_MoveTime(10.f)
	, m_DirSetCount(0)
	, m_Phase2(false)
{
	SetStateType(static_cast<UINT>(eB_States::MOVE));
}

CB_STATEMoveScript::~CB_STATEMoveScript()
{
}

void CB_STATEMoveScript::tick()
{
	


//====================
	CB_FSMScript::eBossMoveDir curDir = m_BHQ->GetMoveDir();
	bool IsPlaying = m_BHQ->IsPlaying();

	// ������ �ð����� �������� ����ȴ�. 
	// �� ó���� B_Walk_FL�� ����ǰ� �ٲ����.
	if (m_MoveTime.IsActivate())
	{
		m_MoveTime.curTime += (float)DT;

		MoveToDir();

		if (m_MoveTime.IsFinish() && !m_Phase2) // �ð��� �� �ƴٸ�
		{
			m_Phase2 = true;
			m_MoveTime.ResetTime();
			m_MoveTime.Activate();
		}

		if (IsPlaying)
			return;

		if (curDir == CB_FSMScript::eBossMoveDir::LF)
		{
			//m_BHQ->ChangeDir(CB_FSMScript::eBossMoveDir::LF);
		
			m_BHQ->PlayAnim(B_Walk_FL, false);
			m_BHQ->SetPlaying(true);

		}


		else if (curDir == CB_FSMScript::eBossMoveDir::RF)
		{
			//	m_BHQ->ChangeDir(CB_FSMScript::eBossMoveDir::RF);
			m_BHQ->PlayAnim(B_Walk_FR, false);
			m_BHQ->SetPlaying(true);
		}

		else if (curDir == CB_FSMScript::eBossMoveDir::F)
		{
			//	m_BHQ->ChangeDir(CB_FSMScript::eBossMoveDir::F);

			if (!m_Phase2)
			{
				m_BHQ->SetStance(CB_FSMScript::eBossStance::NORMAL_WALK);
				m_BHQ->PlayAnim(B_Walk_F, false);
			}

			else
			{
				m_BHQ->SetStance(CB_FSMScript::eBossStance::FAST_WALK);
				m_BHQ->PlayAnim(B_Walk_Fast_F, true);

			}


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
	m_MoveTime.Activate(); 
}

void CB_STATEMoveScript::Exit()
{
}

void CB_STATEMoveScript::MoveToDir()
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
	CB_FSMScript::eBossStance curStance = m_BHQ->GetStance();

	if (CB_FSMScript::eBossStance::NORMAL_WALK == curStance)
		Force = BossInfo.B_Stat.MoveSpeed * ScaleDT;

	else if (CB_FSMScript::eBossStance::FAST_WALK == curStance)
		Force = BossInfo.B_Stat.MoveSpeed * ScaleDT * 1.5f;


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





