#include "pch.h"
#include "CMonsterMoveScript.h"
#include "CPathFinderScript.h"
#include "Engine/CTimeMgr.h"


CMonsterMoveScript::CMonsterMoveScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERMOVESCRIPT)
	, m_fSpeed(100.f)
	, m_RenewCount(60)
	, m_iJustTurned(0)
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
	if (!m_bMoveOn)
		return;


	if (m_bTargetOn && !m_bNearPlayer)
		Trace();
}

void CMonsterMoveScript::MoveTo(Vec3 _dstPos)
{
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	m_XDiff = abs(pos.x - _dstPos.x);
	m_YDiff = abs(pos.z - _dstPos.z);

	if (pos.x < _dstPos.x && m_XDiff > 0.5f)
	{
		pos.x += DT * m_fSpeed;
		if (pos.x > _dstPos.x)
			pos.x = _dstPos.x;
	}
	else if (pos.x > _dstPos.x && m_XDiff > 0.5f)
	{
		pos.x -= DT * m_fSpeed;
		if (pos.x < _dstPos.x)
			pos.x = _dstPos.x;
	}

	if (pos.z < _dstPos.z && m_YDiff > 0.5f)
	{
		pos.z += DT * m_fSpeed;
		if (pos.z > _dstPos.z)
			pos.z = _dstPos.z;
	}
	else if (pos.z > _dstPos.z && m_YDiff > 0.5f)
	{
		pos.z -= DT * m_fSpeed;
		if (pos.z < _dstPos.z)
			pos.z = _dstPos.z;
	}
	
	
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

		m_iJustTurned = 0;

		if (m_XDiff <= 0.5f && m_YDiff <= 0.5f)
		{
			if (m_Stack->size() > 1)
			{
				m_Stack->pop();
				m_iJustTurned++;
			}
			
			dstPos = m_Stack->top();
			/*if (RenewPath())
			{
				dstPos = m_Stack->top();
			}
			else
			{
				m_Stack->pop();
				dstPos = m_Stack->top();
			}*/

			if (m_Stack->size() == 1)
			{
				dstPos = m_finalDst;
			}
		}
	}

	MoveTo(dstPos);
}

bool CMonsterMoveScript::RenewPath()
{
	m_RenewCount++;

	if (m_RenewCount < 60) // 60프레임마다 초기화 
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

void CMonsterMoveScript::Clear()
{
	m_bTargetOn = false;
	m_bMoveOn = false;
	m_iJustTurned = 0;
	m_RenewCount = 60;
	m_Stack = {};
}

void CMonsterMoveScript::SetAndGetPath(CGameObject* _pObject)
{
	m_sPathFinder = GetOwner()->GetScript<CPathFinderScript>();
	m_TargetObj = _pObject;
	m_finalDst = _pObject->Transform()->GetRelativePos();
	m_Pad = m_sPathFinder->SetDestObject(_pObject);
	m_Stack = m_sPathFinder->GetPathStack();
	m_bTargetOn = true;
}

void CMonsterMoveScript::MoveOn(bool _is)
{
	m_bMoveOn = _is;
	m_RenewCount = 0;
}






