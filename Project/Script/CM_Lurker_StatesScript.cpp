#include "pch.h"
#include "CM_Lurker_StatesScript.h"
#include "CM_Lurker_FSMScript.h"
#include "Engine/CLevelMgr.h"
#include "Engine\CLevel.h"

bool CM_Lurker_StatesScript::m_bAniComplete = false;

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
	m_MHQ->Animator3D()->CompleteEvent(Lurker_Turn180) = std::bind(&CM_Lurker_StatesScript::TurnComplete, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_Turn90L) = std::bind(&CM_Lurker_StatesScript::TurnComplete, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_Turn90R) = std::bind(&CM_Lurker_StatesScript::TurnComplete, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_Turn135L) = std::bind(&CM_Lurker_StatesScript::TurnComplete, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_Turn135R) = std::bind(&CM_Lurker_StatesScript::TurnComplete, this);
}

bool CM_Lurker_StatesScript::Turn180()
{
	if (!m_bTurnBtn)
	{

		m_MHQ->Animator3D()->Play(Lurker_Turn180, false);

		m_vInitialRot = m_MHQ->Transform()->GetRelativeRot();
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y -= 180;
		m_fFinishTime = m_MHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * 0.01717f;


	if (!m_bAniComplete)
	{
		Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
		m_MHQ->Transform()->SetRelativeRot(vecRot);
	}
	else
	{
		m_bAniComplete = false;
		m_bTurnBtn = false;
		m_vInitialRot = {0.f, 0.f, 0.f};
		m_vFinalRot = {0.f,0.f,0.f};
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}

bool CM_Lurker_StatesScript::Turn90L()
{
	if (!m_bTurnBtn)
	{

		m_MHQ->Animator3D()->Play(Lurker_Turn90L, false);

		m_vInitialRot = m_MHQ->Transform()->GetRelativeRot();
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y -= 90;
		m_vInitialPos = m_MHQ->Transform()->GetRelativePos();
		m_fFinishTime = m_MHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * 0.01717f;


	if (!m_bAniComplete)
	{
		Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
		m_MHQ->Transform()->SetRelativeRot(vecRot);
	}
	else
	{
		m_bAniComplete = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}

bool CM_Lurker_StatesScript::Turn90R()
{

	if (!m_bTurnBtn)
	{

		m_MHQ->Animator3D()->Play(Lurker_Turn90R, false);

		m_vInitialRot = m_MHQ->Transform()->GetRelativeRot();
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += 90;
		m_vInitialPos = m_MHQ->Transform()->GetRelativePos();
		m_fFinishTime = m_MHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * 0.01717f;


	if (!m_bAniComplete)
	{
		Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
		m_MHQ->Transform()->SetRelativeRot(vecRot);
	}
	else
	{
		m_bAniComplete = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}

bool CM_Lurker_StatesScript::Turn135L()
{
	if (!m_bTurnBtn)
	{

		m_MHQ->Animator3D()->Play(Lurker_Turn135L, false);

		m_vInitialRot = m_MHQ->Transform()->GetRelativeRot();
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y -= 135;
		m_vInitialPos = m_MHQ->Transform()->GetRelativePos();
		m_fFinishTime = m_MHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * 0.01717f;


	if (!m_bAniComplete)
	{
		Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
		m_MHQ->Transform()->SetRelativeRot(vecRot);
	}
	else
	{
		m_bAniComplete = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}

bool CM_Lurker_StatesScript::Turn135R()
{
	if (!m_bTurnBtn)
	{

		m_MHQ->Animator3D()->Play(Lurker_Turn135R, false);

		m_vInitialRot = m_MHQ->Transform()->GetRelativeRot();
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += 135;
		m_vInitialPos = m_MHQ->Transform()->GetRelativePos();
		m_fFinishTime = m_MHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * 0.01717f;


	if (!m_bAniComplete)
	{
		Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
		m_MHQ->Transform()->SetRelativeRot(vecRot);
	}
	else
	{
		m_bAniComplete = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
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

void CM_Lurker_StatesScript::TurnComplete()
{
	m_bAniComplete = true;
}

void CM_Lurker_StatesScript::SaveToScene(FILE* _pFile)
{
}

void CM_Lurker_StatesScript::LoadFromScene(FILE* _pFile)
{
}


