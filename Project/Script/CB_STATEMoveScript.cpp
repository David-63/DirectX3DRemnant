#include "pch.h"
#include "CB_STATEMoveScript.h"
#include "CB_FSMScript.h"



CB_STATEMoveScript::CB_STATEMoveScript()
	: m_MoveTime(50.f)
	, m_First(false)
{
	SetStateType(static_cast<UINT>(eB_States::MOVE));
}

CB_STATEMoveScript::~CB_STATEMoveScript()
{
}

void CB_STATEMoveScript::tick()
{
	//m_MoveTime.curTime += (float)DT;

	CB_FSMScript::eBossMoveDir curDir = m_BHQ->GetMoveDir();


	// === 랜덤으로 난수를 구하고 해당 난수에 해당하는 Dir로 change한다.
	// 3-5번이 걸리면 다시한번 난수를 구한다.

	bool IsPlaying = m_BHQ->IsPlaying();

	// 지정한 시간동안 랜덤으로 재생된다. 
	// 맨 처음엔 B_Walk_FL가 재생되게 바꿔놨음.
	if (m_MoveTime.IsActivate())
	{
		m_MoveTime.curTime += (float)DT;

		if (IsPlaying)
			return;
		//if(m_First)
		//m_BHQ->RandomDir_SomeExclude();


		if (curDir == CB_FSMScript::eBossMoveDir::LF)
		{
			//m_BHQ->ChangeDir(CB_FSMScript::eBossMoveDir::LF);
			m_BHQ->SetPlaying(true);
			m_BHQ->PlayAnim(B_Walk_FL, false);


		}
		else if (curDir == CB_FSMScript::eBossMoveDir::RF)
		{
			//	m_BHQ->ChangeDir(CB_FSMScript::eBossMoveDir::RF);
			m_BHQ->SetPlaying(true);
			m_BHQ->PlayAnim(B_Walk_FR, false);
		}
		else if (curDir == CB_FSMScript::eBossMoveDir::F)
		{
			//	m_BHQ->ChangeDir(CB_FSMScript::eBossMoveDir::F);
			m_BHQ->SetPlaying(true);
			m_BHQ->PlayAnim(B_Walk_F, false);

		}
		else if (curDir == CB_FSMScript::eBossMoveDir::B)
		{
			//	m_BHQ->ChangeDir(CB_FSMScript::eBossMoveDir::B);
			m_BHQ->SetPlaying(true);
			m_BHQ->PlayAnim(B_Walk_B, false);

		}
		else if (curDir == CB_FSMScript::eBossMoveDir::LB)
		{
			//	m_BHQ->ChangeDir(CB_FSMScript::eBossMoveDir::LB);
			m_BHQ->SetPlaying(true);
			m_BHQ->PlayAnim(B_Walk_BL, false);

		}
		else if (curDir == CB_FSMScript::eBossMoveDir::RB)
		{
			//	m_BHQ->ChangeDir(CB_FSMScript::eBossMoveDir::RB);
			m_BHQ->SetPlaying(true);
			m_BHQ->PlayAnim(B_Walk_BR, false);

		}

		m_First = true;
	}

	else if (m_MoveTime.IsFinish())// 시간이 다 됐다면
	{
		// 2페이즈로 넘어간다. 
		// (Q. 근데 idle로 다시 넘어갔을 때 이게 2페이즈에 해당하는지 아닌지를 어떻게 체크할까?)
		// IDLE에서 BOOL 변수를 하나 만들어서 1페이즈 처음때 들어갔으면 TRUE 해놓고, FALSE일 때 2페이즈로 넘어가게하자
		//m_BHQ->ChangeState(static_cast<UINT>(eB_States::IDLE));

				// 이동 상태를 IDLE 상태로 변경한다. 
		//m_BHQ->ChangeState(static_cast<UINT>(eB_States::IDLE));
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
	m_MoveTime.Activate(); // 
}

void CB_STATEMoveScript::Exit()
{
}


