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

	// 지정한 시간동안 랜덤으로 재생된다. 
	// 맨 처음엔 B_Walk_FL가 재생되게 바꿔놨음.
	if (m_MoveTime.IsActivate())
	{
		m_MoveTime.curTime += (float)DT;

		MoveToDir();

		if (m_MoveTime.IsFinish() && !m_Phase2) // 시간이 다 됐다면
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
	rf.Normalize(); //  방향 가리키는 용도이므로 벡터 정규화

	Vec3 lf = -vRight + vFront;
	lf.Normalize(); // 방향 가리키는 용도이므로 벡터 정규화




	// ==== stance별 이동 속도
	float Force = 0.f;
	CB_FSMScript::tB_Info BossInfo = m_BHQ->GetBossInfo();
	CB_FSMScript::eBossStance curStance = m_BHQ->GetStance();

	if (CB_FSMScript::eBossStance::NORMAL_WALK == curStance)
		Force = BossInfo.B_Stat.MoveSpeed * ScaleDT;

	else if (CB_FSMScript::eBossStance::FAST_WALK == curStance)
		Force = BossInfo.B_Stat.MoveSpeed * ScaleDT * 1.5f;


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





