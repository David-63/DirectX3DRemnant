#include "pch.h"
#include "CB_FSMScript.h"
#include "CB_STATEDamagedScript.h"
#include <Engine\Physics.h>

CB_FSMScript::CB_FSMScript()
	: m_bPlaying(false)
	, m_iAnimCount(0)
	, m_eNoWeapon_Stance(eBossStance_NoWeapon::NORMAL_WALK)
	, m_iRatio(0.f)
{
	SetScriptType(SCRIPT_TYPE::B_FSMSCRIPT);
}

CB_FSMScript::~CB_FSMScript()
{
}

void CB_FSMScript::begin()
{


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

	GetOwner()->Animator3D()->Add(B_NC_Idle);
	GetOwner()->Animator3D()->Add(B_NC_Agree);
	GetOwner()->Animator3D()->Add(B_NC_Disagree);

	GetOwner()->Animator3D()->Add(B_Turn90_L);
	GetOwner()->Animator3D()->Add(B_Turn90_R);
	GetOwner()->Animator3D()->Add(B_Turn135_L);
	GetOwner()->Animator3D()->Add(B_Turn135_R);
	GetOwner()->Animator3D()->Add(B_Turn180_L);
	GetOwner()->Animator3D()->Add(B_Turn180_R);


	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::B_STATEIDLESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::B_STATEMOVESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::B_STATEDAMAGEDSCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::B_STATEDEADSCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::B_STATERANGEDSCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::B_STATEMELEESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::B_STATEHEALSCRIPT)));

	// =====================


	// ========================
	GetOwner()->Animator3D()->CompleteEvent(B_Walk_FL) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);

	GetOwner()->Animator3D()->CompleteEvent(B_Walk_FR) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);
	GetOwner()->Animator3D()->CompleteEvent(B_Walk_F) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);

	GetOwner()->Animator3D()->CompleteEvent(B_Walk_B) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);
	GetOwner()->Animator3D()->CompleteEvent(B_Walk_BL) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);
	GetOwner()->Animator3D()->CompleteEvent(B_Walk_BR) = std::bind(&CB_FSMScript::Phase1_AnimEnd, this);


	PlayAnim(B_Melee_Idle, true);
	ChangeState(static_cast<UINT>(eB_States::IDLE)); // ������ IDLE�ε� �۾����� ���� MOVE�� �س��� 



}

void CB_FSMScript::tick()
{
	CC_FSMScript::tick();

	BoneRigidbody_Check();

	if (GetAtkSign() && GetCurStateType() != (UINT)eB_States::DEAD)
	{
		tHitInfo info = GetHitInfo();

		//����������ϱ�
		m_tBossInfo.B_Health.GotDamage(info.Damage);

		if (GetCurStateType() == (UINT)eB_States::DAMAGED)
			return;


		ChangeState((UINT)eB_States::DAMAGED);
	}


}

void CB_FSMScript::BeginOverlap(CCollider3D* _Other)
{
	// ������ tTimeCtrl �� ���� �־����. 
	// ���� ȸ�� (�÷��̾��� �������� �ٴٶ��� ��Ÿ���� ������ ��)
	// �̋��� �������� ������ ���ߴ��� üũ�ϴ� �κ��� �������� RayCast�Լ����� hit�� true�̸����� ����ϸ� �ɵ�? 

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

Vec3 CB_FSMScript::GetPlayerPos()
{
	vector<CGameObject*> vec = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject();

	for (CGameObject* obj : vec)
	{
		return obj->Transform()->GetRelativePos();
	}

	return Vec3(0.f, 0.f, 0.f);
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
	m_bPlaying = false;

	// ������ 


	// === �ִϸ��̼��� 1���� ����Ǹ� �ʹ� ª�Ƽ� 2�� ����ϰ� �ϱ� ���� �ڵ� ===
	++m_iAnimCount;
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

void CB_FSMScript::BoneRigidbody_Check()
{
	//// �� ƽ���� �浹ü�� �� ��ġ�� �Ű��ִ� �ڵ� 
	Matrix retBoneat = Animator3D()->GetBoneMatrix(57); // ��ȣ�� �Ž� ��ȣ �� ���� 
	Matrix ownerMat = Transform()->GetWorldMat();
	Matrix totalMat = retBoneat * ownerMat;
	Vec3 retPos = totalMat.Translation();
	Vec4 retRot = DirectX::XMQuaternionRotationMatrix(totalMat);

	PxTransform retTransform;
	retTransform.p.x = retPos.x;
	retTransform.p.y = retPos.y;
	retTransform.p.z = retPos.z;
	retTransform.q.x = retRot.x;
	retTransform.q.y = retRot.y;
	retTransform.q.z = retRot.z;
	retTransform.q.w = retRot.w;

	RigidBody()->SetShapeLocalPxTransform(1, retTransform); // 0���� �ٴ��浹������ �����̱� ������ 1������ �ϱ� 

	// =============== 
	retBoneat = Animator3D()->GetBoneMatrix(12); // ��ȣ�� �Ž� ��ȣ �� ���� 
	ownerMat = Transform()->GetWorldMat();
	totalMat = retBoneat * ownerMat;
	retPos = totalMat.Translation();
	retRot = DirectX::XMQuaternionRotationMatrix(totalMat);

	retTransform;
	retTransform.p.x = retPos.x;
	retTransform.p.y = retPos.y;
	retTransform.p.z = retPos.z;
	retTransform.q.x = retRot.x;
	retTransform.q.y = retRot.y;
	retTransform.q.z = retRot.z;
	retTransform.q.w = retRot.w;

	RigidBody()->SetShapeLocalPxTransform(2, retTransform); // 0���� �ٴ��浹������ �����̱� ������ 1������ �ϱ� 

}

