#include "pch.h"
#include "CTestScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>


CTestScript::CTestScript()
	: CScript((UINT)SCRIPT_TYPE::TESTSCRIPT)
{
}

CTestScript::~CTestScript()
{
}


void CTestScript::begin()
{
	//Animator3D()->StartEvent(L"firstAnim") = std::bind(&CTestScript::Start, this);
	//Animator3D()->CompleteEvent(L"firstAnim") = std::bind(&CTestScript::Complete, this);
	//Animator3D()->EndEvent(L"firstAnim") = std::bind(&CTestScript::End, this);
	
	//Animator3D()->ActionEvent(L"firstAnim", 50) = std::bind(&CTestScript::Action, this);
}

void CTestScript::tick()
{
	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	float fSpeed = 10000.f;


	if (KEY_TAP(KEY::J))
	{
		Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\P_Main.pref", L"prefab\\P_Main.pref");
		fab->PrefabLoad(L"prefab\\P_Main.pref");
		CGameObject* player = fab.Get()->Instantiate(Vec3(0, 0, 0), 1);
		SpawnGameObject(player, Vec3(0, 0, 0), 1);
	}

	if (KEY_HOLD(KEY::NUM_8))
	{
		vMoveVector += DT * vFront * fSpeed;
	}

	if (KEY_HOLD(KEY::NUM_5))
	{
		vMoveVector -= DT * vFront * fSpeed;
	}

	if (KEY_HOLD(KEY::NUM_4))
	{
		vMoveVector -= DT * vRight * fSpeed;
	}

	if (KEY_HOLD(KEY::NUM_6))
	{
		vMoveVector += DT * vRight * fSpeed;
	}

	GetOwner()->RigidBody()->SetVelocity(vMoveVector);
	vMoveVector = Vec3(0.f, 0.f, 0.f);
	
}

void CTestScript::BeginOverlap(CCollider3D* _Other)
{
	
}

void CTestScript::OnOverlap(CCollider3D* _Other)
{
}

void CTestScript::EndOverlap(CCollider3D* _Other)
{
}



void CTestScript::SaveToLevelFile(FILE* _File)
{
}

void CTestScript::LoadFromLevelFile(FILE* _FILE)
{
}


