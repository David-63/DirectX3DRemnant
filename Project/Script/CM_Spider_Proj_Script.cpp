#include "pch.h"
#include "CM_Spider_Proj_Script.h"
#include "Engine/CCollider3D.h"
#include "Engine/CRigidBody.h"
#include "Engine/CParticleSystem.h"

CM_Spider_Proj_Script::CM_Spider_Proj_Script()
	: CScript((UINT)SCRIPT_TYPE::M_SPIDER_PROJ_SCRIPT)
	, mOffset(0.f,0.f,0.f)
	, m_fDurationTime(0.f)
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

		Vec3 projPos = spiderPos - m_vdir * 150.f + mOffset;
		
		GetOwner()->Transform()->SetRelativePos(projPos);
		m_fDurationTime = 0.f;
	}
	else if (m_bShootStart)
	{
		Vec3 projPos = GetOwner()->Transform()->GetRelativePos();
		projPos += -m_vdir * DT * 1500.f;

		GetOwner()->Transform()->SetRelativePos(projPos);

		m_fDurationTime += DT;

		if (m_fDurationTime > 1000.f);
		{
			DestroyObject(GetOwner());
			m_fDurationTime = 0.f;
		}
	}
}

void CM_Spider_Proj_Script::BeginOverlap(CCollider3D* _Other)
{
}

void CM_Spider_Proj_Script::OnOverlap(CCollider3D* _Other)
{
}

void CM_Spider_Proj_Script::EndOverlap(CCollider3D* _Other)
{
}
