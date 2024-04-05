#include "pch.h"
#include "CM_Lurker_FSMScript.h"
#include "CM_Lurker_StatesScript.h"
#include "CM_Lurker_STATE_Damaged_Script.h"
#include "CM_Lurker_STATE_Idle_Script.h"

CM_Lurker_FSMScript::CM_Lurker_FSMScript()
{
	SetScriptType(SCRIPT_TYPE::M_LURKER_FSMSCRIPT);
}

CM_Lurker_FSMScript::~CM_Lurker_FSMScript()
{
}

void CM_Lurker_FSMScript::begin()
{
	m_tMonsterInfo.M_Health.MaxHp = 100.f;
	m_tMonsterInfo.M_Health.CurHp = 100.f;

	GetOwner()->Animator3D()->Add(Lurker_ALERT01);
	GetOwner()->Animator3D()->Add(Lurker_ALERT03);
	GetOwner()->Animator3D()->Add(Lurker_IDLESTAND);
	GetOwner()->Animator3D()->Add(Lurker_IDLEDOWN);
	GetOwner()->Animator3D()->Add(Lurker_IDLEDOWNDOO);
	GetOwner()->Animator3D()->Add(Lurker_WALL_EMERGE);
	GetOwner()->Animator3D()->Add(Lurker_WALL_EMERGE2);
	GetOwner()->Animator3D()->Add(Lurker_EMERGE);
	GetOwner()->Animator3D()->Add(Lurker_Turn180);
	GetOwner()->Animator3D()->Add(Lurker_Turn90L);
	GetOwner()->Animator3D()->Add(Lurker_Turn90R);
	GetOwner()->Animator3D()->Add(Lurker_Turn135L);
	GetOwner()->Animator3D()->Add(Lurker_Turn135R);
	GetOwner()->Animator3D()->Add(Lurker_Dash);
	GetOwner()->Animator3D()->Add(Lurker_Heavy1);
	GetOwner()->Animator3D()->Add(Lurker_SlashR);
	GetOwner()->Animator3D()->Add(Lurker_SlashRCombo);
	GetOwner()->Animator3D()->Add(Lurker_MoveFront);
	GetOwner()->Animator3D()->Add(Lurker_Death);
	GetOwner()->Animator3D()->Add(Lurker_DeathGround);

	GetOwner()->Animator3D()->Add(Lurker_ImpactLegs);
	GetOwner()->Animator3D()->Add(Lurker_ImpactLegL);
	GetOwner()->Animator3D()->Add(Lurker_ImpactLegR);
	GetOwner()->Animator3D()->Add(Lurker_ImpactTorso);
	GetOwner()->Animator3D()->Add(Lurker_ImpactFront);



	//애니먼저 생성해야함
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_IDLE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_MELEE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_MOVE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_DAMAGED_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_LURKER_STATE_DEAD_SCRIPT)));

	dynamic_cast<CM_Lurker_STATE_Idle_Script*>(FindStateScript(0))->SetInitialPose(m_IdlePose);


	ChangeState(static_cast<UINT>(eLurkerState::Idle));
	//ChangeState(static_cast<UINT>(eLurkerState::Move));


}

void CM_Lurker_FSMScript::tick()
{
	CC_FSMScript::tick();
	DeathCheck();

	if (GetAtkSign() && GetCurStateType() != (UINT)eM_States::DEAD)
	{
		tHitInfo info = GetHitInfo();

		//대미지적용하기
		m_tMonsterInfo.M_Health.GotDamage(info.Damage);

		if (GetCurStateType() == (UINT)eM_States::DAMAGED)
			return;

		//앞뒤 구분, 뒤면 피격상태안됨
		Vec3 hitPos = info.HitPos;
		hitPos.y = 0.f;
		Vec3 OwnerPos = GetOwner()->Transform()->GetRelativePos();
		OwnerPos.y = 0.f;
		Vec3 dir = hitPos - OwnerPos;
		dir = dir.Normalize();

		Vec3 front = -GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
		if (front.Dot(dir) < 0)
			return;

		//부위에 따른 피격효과 
		OwnerPos = GetOwner()->Transform()->GetRelativePos();
		if (OwnerPos.y + 122.f <= info.HitPos.y && info.HitPos.y <= OwnerPos.y + 140.f
			&& OwnerPos.x - 20.f <= info.HitPos.x && info.HitPos.x <= OwnerPos.x + 20.f)
		{
			dynamic_cast<CM_Lurker_STATE_Damaged_Script*>(FindStateScript((UINT)eM_States::DAMAGED))->SetPart(4);
		}
		else if(OwnerPos.y + 90.f <= info.HitPos.y && info.HitPos.y <= OwnerPos.y + 170.f)
		{
			dynamic_cast<CM_Lurker_STATE_Damaged_Script*>(FindStateScript((UINT)eM_States::DAMAGED))->SetPart(3);
		}
		else if (front.Cross(dir).y > 0 && info.HitPos.y <= OwnerPos.y + 80.f)
		{
			dynamic_cast<CM_Lurker_STATE_Damaged_Script*>(FindStateScript((UINT)eM_States::DAMAGED))->SetPart(2);
		}
		else if (front.Cross(dir).y < 0 && info.HitPos.y <= OwnerPos.y + 80.f)
		{
			dynamic_cast<CM_Lurker_STATE_Damaged_Script*>(FindStateScript((UINT)eM_States::DAMAGED))->SetPart(1);
		}
		else
		{
			dynamic_cast<CM_Lurker_STATE_Damaged_Script*>(FindStateScript((UINT)eM_States::DAMAGED))->SetPart(0);
		}

		ChangeState((UINT)eLurkerState::Damaged);

	}
	
}

void CM_Lurker_FSMScript::BeginOverlap(CCollider3D* _Other)
{
}

void CM_Lurker_FSMScript::OnOverlap(CCollider3D* _Other)
{
}

void CM_Lurker_FSMScript::EndOverlap(CCollider3D* _Other)
{
}

void CM_Lurker_FSMScript::DeathCheck()
{
	if (m_tMonsterInfo.M_Health.CurHp <= 0)
		ChangeState(static_cast<UINT>(eLurkerState::Dead));
}

void CM_Lurker_FSMScript::SaveToScene(FILE* _pFile)
{
}

void CM_Lurker_FSMScript::LoadFromScene(FILE* _pFile)
{
}


