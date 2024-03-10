#include "pch.h"
#include "CMonsterMoveScript.h"
#include "CPathFinderScript.h"
#include "Engine/CTimeMgr.h"

CMonsterMoveScript::CMonsterMoveScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERMOVESCRIPT)
	, m_Speed(100.f)
	, m_RenewCount(300)
{

}
CMonsterMoveScript::~CMonsterMoveScript()
{
}

void CMonsterMoveScript::begin()
{
	
}

void CMonsterMoveScript::tick()
{
	if (m_bTargetOn)
		RenewPath();

	if (!m_bCatch && m_bTargetOn)
		Trace();
}

void CMonsterMoveScript::MoveTo(Vec3 _dstPos)
{
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	m_XDiff = abs(pos.x - _dstPos.x);
	m_YDiff = abs(pos.z - _dstPos.z);

	if (pos.x < _dstPos.x && m_XDiff > 0.5f)
		pos.x += DT * m_Speed;
	else if(pos.x > _dstPos.x && m_XDiff > 0.5f)
		pos.x -= DT * m_Speed;

	if (pos.z < _dstPos.z && m_YDiff > 0.5f)
		pos.z += DT * m_Speed;
	else if (pos.z > _dstPos.z && m_YDiff > 0.5f)
		pos.z -= DT * m_Speed;
	
	GetOwner()->Transform()->SetRelativePos(pos);

	/*pos.y = 0.f;
	_dstPos.y = 0.f;
	float dst = Vector3::Distance(pos, m_finalDst);
	if (dst <= m_Pad+ 5.f)
		m_bCatch = true;*/
}

void CMonsterMoveScript::Trace()
{
	Vec3 dstPos = {};
	if (m_Stack->size() >= 2)
	{
		dstPos = m_Stack->top();

		Vec3 pos = GetOwner()->Transform()->GetRelativePos();
		m_XDiff = abs(pos.x - dstPos.x);
		m_YDiff = abs(pos.z - dstPos.z);

		if (m_XDiff <= 0.5f && m_YDiff <= 0.5f)
		{
			if (RenewPath())
			{
				dstPos = m_Stack->top();
			}
			else
			{
				m_Stack->pop();
				dstPos = m_Stack->top();
			}


			if (m_Stack->size() == 1)
			{
				dstPos = m_finalDst;
				m_bCatch = true;
			}
		}
	}

	MoveTo(dstPos);
}

bool CMonsterMoveScript::RenewPath()
{
	m_RenewCount++;
	if (m_RenewCount < 300)
		return false;

	m_RenewCount = 0;

	if (nullptr != m_TargetObj)
	{
		m_sPathFinder->SetDestObject(m_TargetObj);
		m_Stack = m_sPathFinder->GetPathStack();
		return true;
	}
	else
	{
		m_bTargetOn = false;
		return false;
	}

}

void CMonsterMoveScript::SetAndGetPath(CGameObject* _pObject)
{
	m_sPathFinder = GetOwner()->GetScript<CPathFinderScript>();
	m_TargetObj = _pObject;
	m_finalDst = _pObject->Transform()->GetRelativePos();
	m_Pad = m_sPathFinder->SetDestObject(_pObject);
	//m_Stack = m_sPathFinder->GetPathStack();
	m_bTargetOn = true;
	m_bCatch = false;
}




