#include "pch.h"
#include "CM_Spider_STATE_Atk_Script.h"
#include "CM_Spider_FSMScript.h"
#include "Engine/func.h"
#include "CHitBoxScript.h"
#include "CM_Spider_Proj_Script.h"
#include "Engine/CLevelMgr.h"
#include "Engine/CLevel.h"
#include "Engine/CParticleSystem.h"


CM_Spider_STATE_Atk_Script::CM_Spider_STATE_Atk_Script()
{
	SetStateType(static_cast<UINT>(eM_A_States::ATTACK));
}

CM_Spider_STATE_Atk_Script::~CM_Spider_STATE_Atk_Script()
{
}

void CM_Spider_STATE_Atk_Script::begin()
{
	CM_Spider_StatesScript::begin();
	m_MHQ->Animator3D()->CompleteEvent(Spi_Atk) = std::bind(&CM_Spider_STATE_Atk_Script::ShootComplete, this);
	//m_MHQ->Animator3D()->StartEvent(Spi_Atk) = std::bind(&CM_Spider_STATE_Atk_Script::CreateProj, this);
	m_MHQ->Animator3D()->StartEvent(Spi_Atk) = std::bind(&CM_Spider_STATE_Atk_Script::SetDelayStart, this);
	m_MHQ->Animator3D()->ActionEvent(Spi_Atk, 60) = std::bind(&CM_Spider_STATE_Atk_Script::LiftStart, this);

	m_MHQ->Animator3D()->CompleteEvent(Spi_AtkPush) = std::bind(&CM_Spider_STATE_Atk_Script::AtkComplete, this);
	m_MHQ->Animator3D()->ActionEvent(Spi_AtkPush, 20) = std::bind(&CM_Spider_STATE_Atk_Script::AdjustZeroSpeed, this);
	m_MHQ->Animator3D()->ActionEvent(Spi_AtkPush, 34) = std::bind(&CM_Spider_STATE_Atk_Script::PushHitBoxOn, this);
	
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->MakeHitBox(false, Vec3(90.f, 50.f, 30.f), Vec3(0.f, 100.f, -150.f));
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(0);
	
	m_eAtkState = eAtkState::Shoot;
}

void CM_Spider_STATE_Atk_Script::tick()
{
	DistCheck();

	switch (m_eAtkState)
	{
	case eAtkState::Push:
		Push();
		break;
	case eAtkState::Shoot:
		Shoot();
		break;
	default:
		break;
	}

	if (m_bShootLift)
		LiftProj();

	if (m_bdelayStart)
	{
		CreateDelay();
	}
}


void CM_Spider_STATE_Atk_Script::Shoot()
{
	if (!m_bShootOnce)
	{
		m_MHQ->Animator3D()->Play(Spi_Atk, false);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Atk, false);
		m_bShootOnce = true;
	}

	if (m_bAtkComplete)
	{
		m_bShootOnce = false;
		m_bAtkComplete = false;
		m_MHQ->ChangeState((UINT)eM_A_States::MOVE);
	}
	
}

void CM_Spider_STATE_Atk_Script::ShootComplete()
{
	m_bAtkComplete = true;
	m_CProj->GetScript< CM_Spider_Proj_Script>()->ShootStart(true);
	m_CProj = nullptr;
}

void CM_Spider_STATE_Atk_Script::CreateDelay()
{
	m_fDelayTime += DT;

	if (m_fDelayTime > 0.3f)
	{
		CreateProj();
		m_fDelayTime = 0.f;
		m_bdelayStart = false;
	}
}

void CM_Spider_STATE_Atk_Script::CreateProj()
{
	Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\SpiderProjectile.pref", L"prefab\\SpiderProjectile.pref");
	fab->PrefabLoad(L"prefab\\SpiderProjectile.pref");

	Vec3 spiderPos = m_MHQ->GetOwner()->Transform()->GetRelativePos();
	spiderPos.y = 140.f;
	Vec3 frontdir = m_MHQ->GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 projPos = spiderPos - frontdir * 150.f;

	CGameObject* proj = fab->Instantiate(projPos, (UINT)LAYER_TYPE::HitBoxMonster);
	fab->FabClear();

	proj->SetName(L"Spider_Projectile");
	proj->AddComponent(new CM_Spider_Proj_Script);
	proj->AddComponent(new CCollider3D);
	proj->AddComponent(new CRigidBody);
	
	//proj->ParticleSystem()->se

	proj->GetScript<CM_Spider_Proj_Script>()->SetSpider(m_MHQ->GetOwner());

	//proj->AddComponent(new CMeshRender);
	//proj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	//proj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);
	
	CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(proj, L"HitBoxMonster", true);

	proj->SetLayerIdx((UINT)LAYER_TYPE::HitBoxMonster);
	tShapeInfo info = {};
	info.eGeomType = GEOMETRY_TYPE::Sphere;
	info.size = Vector3(20.f, 1.f, 2.f);
	proj->RigidBody()->PushBackShapeInfo(info);
	proj->RigidBody()->SetPhysical(ACTOR_TYPE::Kinematic);

	m_CProj = proj;
}

void CM_Spider_STATE_Atk_Script::LiftProj()
{
	if (!m_CProj)
		return;

	if (!m_bLiftSet)
	{
		m_bLiftSet = true;
		m_fElapsedTime = 0.f;
		m_fFinalTime = 2.f;
	}

	m_fElapsedTime += DT * 8.f;

	if (m_fElapsedTime < m_fFinalTime)
	{
		Vec3 vecOffset = Vector3::Lerp(Vec3(0.f,0.f,0.f), Vec3(0.f, 80.f, 0.f), m_fElapsedTime / m_fFinalTime);
		m_CProj->GetScript< CM_Spider_Proj_Script>()->SetOffset(vecOffset);
	}
	else
	{
		m_CProj->GetScript< CM_Spider_Proj_Script>()->SetOffset(Vec3(0.f, 80.f, 0.f));

		m_bLiftSet = false;
		m_fElapsedTime = 0.f;
		m_bShootLift = false;
	}

}

void CM_Spider_STATE_Atk_Script::AdjustDashSpeed()
{
	m_fSpeed = 30.f;

}

void CM_Spider_STATE_Atk_Script::AtkComplete()
{
	m_bAtkComplete = true;
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetSleep(0);
}


void CM_Spider_STATE_Atk_Script::Push()
{
	if (!m_bPushOnce)
	{
		m_MHQ->Animator3D()->Play(Spi_AtkPush, false);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_AtkPush, false);

		m_vAtkDir = -GetMyFront();
		m_fSpeed = 50.f;
		m_bPushOnce = true;
	}

	if (!m_bAtkComplete)
	{
		if (m_bNearPlayer)
			return;

		Vec3 myPos = GetMyPos();
		myPos += m_vAtkDir * DT * m_fSpeed;
		SetMyPos(myPos);
	}
	else
	{
		m_bPushOnce = false;
		m_bAtkComplete = false;
		m_MHQ->ChangeState((UINT)eM_A_States::MOVE);
	}
}

void CM_Spider_STATE_Atk_Script::AdjustZeroSpeed()
{
	m_fSpeed = 0.f;
}

void CM_Spider_STATE_Atk_Script::Clear()
{
	m_bAtkComplete = false;
	m_bShootLift = false;
	m_CProj = nullptr;
	m_bShootOnce = false;
	m_bPushOnce = false;
	m_fElapsedTime = 0.f;
	m_bShootLift = false;
}


void CM_Spider_STATE_Atk_Script::PushHitBoxOn()
{
	m_MHQ->GetOwner()->GetScript<CHitBoxScript>()->SetAwake(0);
}

void CM_Spider_STATE_Atk_Script::DistCheck()
{
	if (DistBetwPlayer() < 200.f)
		m_bNearPlayer = true;
	else
		m_bNearPlayer = false;
}

void CM_Spider_STATE_Atk_Script::Enter()
{
	
}

void CM_Spider_STATE_Atk_Script::Exit()
{
	Clear();
}


