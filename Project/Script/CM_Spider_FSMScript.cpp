#include "pch.h"
#include "CM_Spider_FSMScript.h"
#include "CM_Spider_StatesScript.h"

CM_Spider_FSMScript::CM_Spider_FSMScript()
{
}

CM_Spider_FSMScript::~CM_Spider_FSMScript()
{
}

void CM_Spider_FSMScript::begin()
{
	m_tMonsterInfo.M_Health.MaxHp = 1000.f;
	m_tMonsterInfo.M_Health.CurHp = 1000.f;

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


	//局聪刚历 积己秦具窃
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_SPIDER_STATE_IDLE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_SPIDER_STATE_ATK_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_SPIDER_STATE_MOVE_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_SPIDER_STATE_DAMAGED_SCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::M_SPIDER_STATE_DEAD_SCRIPT)));

	//ChangeState(static_cast<UINT>(eSpiderState::Idle));
	ChangeState(static_cast<UINT>(eSpiderState::Move));
}

void CM_Spider_FSMScript::tick()
{
	CC_FSMScript::tick();
	DeathCheck();

	if (KEY_TAP(KEY::O))
	{
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


