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


	// === �������� ������ ���ϰ� �ش� ������ �ش��ϴ� Dir�� change�Ѵ�.
	// 3-5���� �ɸ��� �ٽ��ѹ� ������ ���Ѵ�.

	bool IsPlaying = m_BHQ->IsPlaying();

	// ������ �ð����� �������� ����ȴ�. 
	// �� ó���� B_Walk_FL�� ����ǰ� �ٲ����.
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

	else if (m_MoveTime.IsFinish())// �ð��� �� �ƴٸ�
	{
		// 2������� �Ѿ��. 
		// (Q. �ٵ� idle�� �ٽ� �Ѿ�� �� �̰� 2����� �ش��ϴ��� �ƴ����� ��� üũ�ұ�?)
		// IDLE���� BOOL ������ �ϳ� ���� 1������ ó���� ������ TRUE �س���, FALSE�� �� 2������� �Ѿ������
		//m_BHQ->ChangeState(static_cast<UINT>(eB_States::IDLE));

				// �̵� ���¸� IDLE ���·� �����Ѵ�. 
		//m_BHQ->ChangeState(static_cast<UINT>(eB_States::IDLE));
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
	m_MoveTime.Activate(); // 
}

void CB_STATEMoveScript::Exit()
{
}


