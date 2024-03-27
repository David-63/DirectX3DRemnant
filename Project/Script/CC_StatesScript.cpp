#include "pch.h"
#include "CC_StatesScript.h"
#include "CC_FSMScript.h"


CC_StatesScript::CC_StatesScript()
	: CScript((UINT)SCRIPT_TYPE::C_STATESSCRIPT)
	, m_StateType(0)
{
	SetName(L"C_StateScript");
}

CC_StatesScript::~CC_StatesScript()
{
}

void CC_StatesScript::SetHQ(CC_FSMScript* _hq)
{
	m_FSMHQ = _hq;
}

Vec3 CC_StatesScript::GetPlayerPos()
{
	vector<CGameObject*> vec = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject();

	for (CGameObject* obj : vec)
	{
		return obj->Transform()->GetRelativePos();
	}

	return Vec3(0.f, 0.f, 0.f);
}

CGameObject* CC_StatesScript::GetPlayer()
{
	vector<CGameObject*> vec = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject();

	for (CGameObject* obj : vec)
	{
		return obj;
	}

	return nullptr;
}

Vec3 CC_StatesScript::GetMyPos()
{
	m_FSMHQ->Transform()->GetRelativePos();

	return m_FSMHQ->Transform()->GetRelativePos();
}

void CC_StatesScript::SetMyPos(Vec3 _pos)
{
	m_FSMHQ->Transform()->SetRelativePos(_pos);
}

Vec3 CC_StatesScript::GetMyFront()
{
	return m_FSMHQ->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
}

Vec3 CC_StatesScript::GetToPlayerDir()
{
	Vec3 dir = GetPlayerPos() - GetMyPos();
	dir.y = 0;
	dir = dir.Normalize();
	return dir;
}

float CC_StatesScript::DistBetwPlayer()
{
	float dist = Vector3::Distance(GetMyPos(), GetPlayerPos());

	return dist;
}


float CC_StatesScript::CalTurnDegree(Vec3 _dir) 
{
	Vec3 front = -GetMyFront(); // 성수님 몬스터의 front가 등뒤라서 -를 붙여주심.

	float cosTheta = front.Dot(_dir);
	float Degree = acosf(cosTheta);

	Degree = Degree * 180.f / 3.1415926535f;
	float CrossResult = front.Cross(_dir).y;

	if (CrossResult > 0)
		return Degree;
	else
		return -Degree;
}