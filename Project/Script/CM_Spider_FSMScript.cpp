#include "pch.h"
#include "CM_Spider_FSMScript.h"
#include "CM_Spider_StatesScript.h"
#include "CM_Spider_STATE_Damaged_Script.h"

CM_Spider_FSMScript::CM_Spider_FSMScript()
{
	SetScriptType(SCRIPT_TYPE::M_SPIDER_FSMSCRIPT);
}

CM_Spider_FSMScript::~CM_Spider_FSMScript()
{
}

void CM_Spider_FSMScript::begin()
{
	m_tMonsterInfo.M_Health.MaxHp = 100.f;
	m_tMonsterInfo.M_Health.CurHp = 100.f;

	Ptr<CMeshData> data = CResMgr::GetInst()->LoadFBX(L"fbx\\Spider\\Wasteland_Spider_Gun_Passive_Idle.fbx");
	m_Gun = data->Instantiate();
	m_Gun->Animator3D()->SimpleGen(L"animclip\\Spider\\Wasteland_Spider_Gun_Passive_Idle.animclip");

	m_Gun->SetName(L"Gun");
	m_Gun->SetLayerIdx((UINT)LAYER_TYPE::Monster);

	CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(m_Gun, (UINT)LAYER_TYPE::Monster, false);
	
	GetOwner()->AddChild(m_Gun);

	GetOwner()->Animator3D()->Add(Spi_IdleP);
	m_Gun->Animator3D()->Add(SpiGun_IdleP);
	GetOwner()->Animator3D()->Add(Spi_Alert);
	m_Gun->Animator3D()->Add(SpiGun_Alert);
	GetOwner()->Animator3D()->Add(Spi_Idle);
	m_Gun->Animator3D()->Add(SpiGun_Idle);

	GetOwner()->Animator3D()->Add(Spi_Death);
	m_Gun->Animator3D()->Add(SpiGun_Death);

	GetOwner()->Animator3D()->Add(Spi_ImpactF);
	m_Gun->Animator3D()->Add(SpiGun_ImpactF);
	GetOwner()->Animator3D()->Add(Spi_ImpactHeavyF);
	m_Gun->Animator3D()->Add(SpiGun_ImpactHeavyF);

	GetOwner()->Animator3D()->Add(Spi_AtkPush);
	m_Gun->Animator3D()->Add(SpiGun_AtkPush);
	GetOwner()->Animator3D()->Add(Spi_Atk);
	m_Gun->Animator3D()->Add(SpiGun_Atk);

	GetOwner()->Animator3D()->Add(Spi_WalkF);
	m_Gun->Animator3D()->Add(SpiGun_WalkF);
	GetOwner()->Animator3D()->Add(Spi_WalkB);
	m_Gun->Animator3D()->Add(SpiGun_WalkB);
	GetOwner()->Animator3D()->Add(Spi_WalkL);
	m_Gun->Animator3D()->Add(SpiGun_WalkL);
	GetOwner()->Animator3D()->Add(Spi_WalkR);
	m_Gun->Animator3D()->Add(SpiGun_WalkR);

	GetOwner()->Animator3D()->Add(Spi_Turn90L);
	m_Gun->Animator3D()->Add(SpiGun_Turn90L);
	GetOwner()->Animator3D()->Add(Spi_Turn90R);
	m_Gun->Animator3D()->Add(SpiGun_Turn90R);
	GetOwner()->Animator3D()->Add(Spi_Turn135L);
	m_Gun->Animator3D()->Add(SpiGun_Turn135L);
	GetOwner()->Animator3D()->Add(Spi_Turn135R);
	m_Gun->Animator3D()->Add(SpiGun_Turn135R);
	GetOwner()->Animator3D()->Add(Spi_Turn180L);
	m_Gun->Animator3D()->Add(SpiGun_Turn180L);


	//애니먼저 생성해야함
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_SPIDER_STATE_IDLE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_SPIDER_STATE_ATK_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_SPIDER_STATE_MOVE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_SPIDER_STATE_DAMAGED_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_SPIDER_STATE_DEAD_SCRIPT)));

	//ChangeState(static_cast<UINT>(eSpiderState::Idle));
	ChangeState(static_cast<UINT>(eSpiderState::Attack));
}

void CM_Spider_FSMScript::tick()
{
	CC_FSMScript::tick();
	DeathCheck();

	if (KEY_TAP(KEY::O))
	{
		ChangeState((UINT)eSpiderState::Damaged);
	}

	if (GetAtkSign() && GetCurStateType() != (UINT)eSpiderState::Dead)
	{
		tHitInfo info = GetHitInfo();

		//대미지적용하기
		m_tMonsterInfo.M_Health.GotDamage(info.Damage);

		if (GetCurStateType() == (UINT)eM_A_States::DAMAGED)
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
		if (OwnerPos.y + 80.f <= info.HitPos.y && info.HitPos.y <= OwnerPos.y + 120.f
			&& OwnerPos.x - 8.f <= info.HitPos.x && info.HitPos.x <= OwnerPos.x + 8.f)
		{
			dynamic_cast<CM_Spider_STATE_Damaged_Script*>(FindStateScript((UINT)eM_A_States::DAMAGED))->SetPart(1);
		}
		else 
		{
			dynamic_cast<CM_Spider_STATE_Damaged_Script*>(FindStateScript((UINT)eM_A_States::DAMAGED))->SetPart(0);
		}

		ChangeState((UINT)eSpiderState::Damaged);

	}
}

void CM_Spider_FSMScript::BeginOverlap(CCollider3D* _Other)
{
}

void CM_Spider_FSMScript::OnOverlap(CCollider3D* _Other)
{
}

void CM_Spider_FSMScript::EndOverlap(CCollider3D* _Other)
{
}

void CM_Spider_FSMScript::DeathCheck()
{
	if (m_tMonsterInfo.M_Health.CurHp <= 0)
		ChangeState(static_cast<UINT>(eSpiderState::Dead));
}

void CM_Spider_FSMScript::SaveToScene(FILE* _pFile)
{
}

void CM_Spider_FSMScript::LoadFromScene(FILE* _pFile)
{
}


