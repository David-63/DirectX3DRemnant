#include "pch.h"
#include "CB_FSMScript.h"

CB_FSMScript::CB_FSMScript()
	: m_bPlaying(false)
	, m_iAnimCount(0)
	, m_eStance(eBossStance::NORMAL_WALK)
{
}

CB_FSMScript::~CB_FSMScript()
{
}

void CB_FSMScript::begin()
{
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::B_STATEIDLESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::B_STATEMOVESCRIPT)));

	GetOwner()->Animator3D()->Add(B_Alert);

	GetOwner()->Animator3D()->Add(B_BloodDrink_End);
	GetOwner()->Animator3D()->Add(B_BloodDrink_Loop);
	GetOwner()->Animator3D()->Add(B_BloodDrink_Start);

	GetOwner()->Animator3D()->Add(B_Walk_Fast_F);
	GetOwner()->Animator3D()->Add(B_Walk_Fast_FL);
	GetOwner()->Animator3D()->Add(B_Walk_Fast_FR);

	GetOwner()->Animator3D()->Add(B_Walk_B);
	GetOwner()->Animator3D()->Add(B_Walk_BL);
	GetOwner()->Animator3D()->Add(B_Walk_BR);
	GetOwner()->Animator3D()->Add(B_Walk_F);
	GetOwner()->Animator3D()->Add(B_Walk_FL);
	GetOwner()->Animator3D()->Add(B_Walk_FR);

	GetOwner()->Animator3D()->Add(B_Down_End);
	GetOwner()->Animator3D()->Add(B_Down_Loop);
	GetOwner()->Animator3D()->Add(B_Down_Start);

	GetOwner()->Animator3D()->Add(B_Impact_Heavy_Head_L);
	GetOwner()->Animator3D()->Add(B_Impact_Heavy_Head_R);

	GetOwner()->Animator3D()->Add(B_Melee_Atk_L01);
	GetOwner()->Animator3D()->Add(B_Melee_Atk_L02);
	GetOwner()->Animator3D()->Add(B_Melee_Atk_L03);
	GetOwner()->Animator3D()->Add(B_Melee_Atk_R01);
	GetOwner()->Animator3D()->Add(B_Melee_Atk_R02);
	GetOwner()->Animator3D()->Add(B_Melee_Atk_R03);
	GetOwner()->Animator3D()->Add(B_Melee_Atk_Double01);
	GetOwner()->Animator3D()->Add(B_Melee_Atk_Double02);
	GetOwner()->Animator3D()->Add(B_Melee_Atk_Weapon_AOE);
	GetOwner()->Animator3D()->Add(B_NC_Atk_Summon_Weapons);

	GetOwner()->Animator3D()->Add(B_Melee_Death);

	GetOwner()->Animator3D()->Add(B_Melee_Down_End);
	GetOwner()->Animator3D()->Add(B_Melee_Down_Loop);
	GetOwner()->Animator3D()->Add(B_Melee_Down_Start);

	GetOwner()->Animator3D()->Add(B_Melee_Evade_B);
	GetOwner()->Animator3D()->Add(B_Melee_Evade_F);
	GetOwner()->Animator3D()->Add(B_Melee_Evade_L);
	GetOwner()->Animator3D()->Add(B_Melee_Evade_R);

	GetOwner()->Animator3D()->Add(B_Melee_Idle);
	GetOwner()->Animator3D()->Add(B_Melee_Return_Passive);

	GetOwner()->Animator3D()->Add(B_NC_Idle);
	GetOwner()->Animator3D()->Add(B_NC_Agree);
	GetOwner()->Animator3D()->Add(B_NC_Disagree);





	// =====================


	// ========================
	GetOwner()->Animator3D()->CompleteEvent(B_Walk_FL) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);

	GetOwner()->Animator3D()->CompleteEvent(B_Walk_FR) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);
	GetOwner()->Animator3D()->CompleteEvent(B_Walk_F) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);

	GetOwner()->Animator3D()->CompleteEvent(B_Walk_B) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);
	GetOwner()->Animator3D()->CompleteEvent(B_Walk_BL) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);
	GetOwner()->Animator3D()->CompleteEvent(B_Walk_BR) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);

	//GetOwner()->Animator3D()->CompleteEvent(B_Melee_Idle) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);

	//PlayAnim(B_Melee_Idle, true);
	PlayAnim(B_Walk_FL, true);
	ChangeState(static_cast<UINT>(eB_States::IDLE)); // 원래는 IDLE인데 작업중일 때만 MOVE로 해놓기 



}

void CB_FSMScript::tick()
{
	CC_FSMScript::tick();


}

void CB_FSMScript::BeginOverlap(CCollider3D* _Other)
{
	// 보스는 tTimeCtrl 을 갖고 있어야함. 
	// 보스 회피 (플레이어의 조준점에 다다랐고 쿨타임이 끝났을 때)
	// 이떄의 조준점이 보스를 향했는지 체크하는 부분은 피직스의 RayCast함수에서 hit가 true이면으로 계산하면 될듯? 

}

void CB_FSMScript::OnOverlap(CCollider3D* _Other)
{
}

void CB_FSMScript::EndOverlap(CCollider3D* _Other)
{
}

void CB_FSMScript::PlayAnim(wstring _name, bool _repeat)
{
	GetOwner()->Animator3D()->Play(_name, _repeat);
}



void CB_FSMScript::SaveToScene(FILE* _pFile)
{
}

void CB_FSMScript::LoadFromScene(FILE* _pFile)
{
}

CB_FSMScript::eBossMoveDir CB_FSMScript::RandomDir_SomeExclude()
{
	B_MoveDir =
		static_cast<eBossMoveDir>(rand() % static_cast<UINT>(eBossMoveDir::END));

	if (B_MoveDir == eBossMoveDir::L || B_MoveDir == eBossMoveDir::R)
		return RandomDir_SomeExclude();

	return B_MoveDir;
}

CB_FSMScript::eBossMoveDir CB_FSMScript::RandomDir()
{

	B_MoveDir = static_cast<eBossMoveDir>(rand() % static_cast<UINT>(eBossMoveDir::END));

	return eBossMoveDir();
}


void CB_FSMScript::Phase1_AnimEnd()
{
	++m_iAnimCount;

	m_bPlaying = false;

	if (m_iAnimCount == 1)
	{
		B_MoveDir = B_MoveDir;
	}

	else if (m_iAnimCount == 2)
	{
		RandomDir();
		m_iAnimCount = 0;
	}



}
