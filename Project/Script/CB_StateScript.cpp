#include "pch.h"
#include "CB_StateScript.h"
#include "CB_FSMScript.h"


CB_StateScript::CB_StateScript()
	: m_BStateType(eB_States::END)
	, m_ratio(0.f)
	//, m_EvadeCoolTime(5.f)
{
	SetName(L"CB_StatesScript");
}


CB_StateScript::~CB_StateScript()
{
}


void CB_StateScript::begin()
{
	m_BHQ = dynamic_cast<CB_FSMScript*>(m_FSMHQ);
}

Vec3 CB_StateScript::GetPlayerPos()
{

	vector<CGameObject*> vec = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject();

	for (CGameObject* obj : vec)
	{
		return obj->Transform()->GetRelativePos();
	}

	return Vec3(0.f, 0.f, 0.f);
}

CGameObject* CB_StateScript::GetPlayerObj()
{
	vector<CGameObject*> vec = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject();

	for (CGameObject* obj : vec)
	{
		return obj;
	}
}

void CB_StateScript::SaveToScene(FILE* _pFile)
{
}

void CB_StateScript::LoadFromScene(FILE* _pFile)
{
}
