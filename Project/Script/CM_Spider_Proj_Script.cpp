#include "pch.h"
#include "CM_Spider_Proj_Script.h"
#include "Engine/CCollider3D.h"
#include "Engine/CRigidBody.h"
#include "Engine/CParticleSystem.h"
#include "CC_FSMScript.h"
#include "CM_Spider_proj_Explosion_Script.h"

CM_Spider_Proj_Script::CM_Spider_Proj_Script()
	: CScript((UINT)SCRIPT_TYPE::M_SPIDER_PROJ_SCRIPT)
	, mOffset(0.f,0.f,0.f)
{
}

CM_Spider_Proj_Script::~CM_Spider_Proj_Script()
{
}

void CM_Spider_Proj_Script::begin()
{
}

void CM_Spider_Proj_Script::tick()
{
	if (nullptr != mSpider && !m_bShootStart)
	{
		Vec3 spiderPos = mSpider->Transform()->GetRelativePos();
		spiderPos.y = 140.f;
		//spiderPos.y = 220.f;
		m_vdir = mSpider->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
		m_vdir.y += 0.08f;

		Vec3 m_vInitialPos = spiderPos - m_vdir * 150.f + mOffset;
		
		GetOwner()->Transform()->SetRelativePos(m_vInitialPos);
	}
	else if (m_bShootStart)
	{
		Vec3 projPos = GetOwner()->Transform()->GetRelativePos();
		projPos += -m_vdir * DT * 1500.f;

		GetOwner()->Transform()->SetRelativePos(projPos);

		if (Vector3::Distance(projPos, m_vInitialPos) > 5000.f)
		{
			DestroyObject(GetOwner());
		}

	}
}

void CM_Spider_Proj_Script::BeginOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == (UINT)LAYER_TYPE::Player)
	{
		tHitInfo info;
		info.Damage = 10;
		info.KnockBackGrade = 0;
		info.Shooter = GetOwner()->GetReserver();
		info.ShooterPos = GetOwner()->GetReserver()->Transform()->GetRelativePos();
		_Other->GetOwner()->GetScript<CC_FSMScript>()->GiveAtkInfo(info);
	}

	GetOwner()->RigidBody()->SetSleep(true);

	//폭발이펙트 오브젝트생성
	Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Spider_Proj_Explosion.pref", L"prefab\\Spider_Proj_Explosion.pref");
	fab->PrefabLoad(L"prefab\\Spider_Proj_Explosion.pref");

	Vec3 curPos = GetOwner()->Transform()->GetRelativePos();
	CGameObject* proj = fab->Instantiate(curPos, (UINT)LAYER_TYPE::Default);
	fab->FabClear();
	proj->ParticleSystem()->SetOffsetPos(Vec3(-30.f, -30.f, -30.f));
	proj->AddComponent(new CM_Spider_proj_Explosion_Script);
	
	proj->SetLayerIdx((UINT)LAYER_TYPE::Default);
	SpawnGameObject(proj, curPos, L"Default");

	DestroyObject(GetOwner());
}

void CM_Spider_Proj_Script::OnOverlap(CCollider3D* _Other)
{
}

void CM_Spider_Proj_Script::EndOverlap(CCollider3D* _Other)
{
}
