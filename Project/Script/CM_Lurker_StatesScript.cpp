#include "pch.h"
#include "CM_Lurker_StatesScript.h"
#include "CM_Lurker_FSMScript.h"
#include "Engine/CLevelMgr.h"
#include "Engine\CLevel.h"

eMeleeState  CM_Lurker_StatesScript::m_eMeleeState = eMeleeState::Heavy1;

CM_Lurker_StatesScript::CM_Lurker_StatesScript()
	: m_StateType(eM_States::END)
{
	SetName(L"CM_Lurker_StateScript");
}

CM_Lurker_StatesScript::~CM_Lurker_StatesScript()
{
}
void CM_Lurker_StatesScript::begin()
{
	m_MHQ = dynamic_cast<CM_Lurker_FSMScript*>(m_FSMHQ);
}

Vec3 CM_Lurker_StatesScript::GetPlayerPos()
{
	vector<CGameObject*> vec = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject();

	for (CGameObject* obj : vec)
	{
		return obj->Transform()->GetRelativePos();
	}

	return Vec3(0.f,0.f,0.f);
}

CGameObject* CM_Lurker_StatesScript::GetPlayer()
{
	vector<CGameObject*> vec = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject();

	for (CGameObject* obj : vec)
	{
		return obj;
	}

	return nullptr;
}

Vec3 CM_Lurker_StatesScript::GetMyPos()
{
	m_MHQ->Transform()->GetRelativePos();

	return m_MHQ->Transform()->GetRelativePos();
}

void CM_Lurker_StatesScript::SetMyPos(Vec3 _pos)
{
	m_MHQ->Transform()->SetRelativePos(_pos); 
}

Vec3 CM_Lurker_StatesScript::GetMyFront()
{
	return m_MHQ->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
}

Vec3 CM_Lurker_StatesScript::GetToPlayerDir()
{
	Vec3 dir = GetPlayerPos() - GetMyPos();
	dir.y = 0;
	dir = dir.Normalize();
	return dir;
}

float CM_Lurker_StatesScript::DistBetwPlayer()
{
	float dist = Vector3::Distance(GetMyPos(), GetPlayerPos());

	return dist;
}


float CM_Lurker_StatesScript::CalTurnDegree(Vec3 _dir)
{
	Vec3 front = -GetMyFront();

	float cosTheta = front.Dot(_dir);
	float Degree = acosf(cosTheta);

	Degree = Degree * 180.f / 3.1415926535f;
	float CrossResult = front.Cross(_dir).y;

	if (CrossResult > 0)
		return Degree;
	else
		return -Degree;
}

void CM_Lurker_StatesScript::SaveToScene(FILE* _pFile)
{
}

void CM_Lurker_StatesScript::LoadFromScene(FILE* _pFile)
{
}


