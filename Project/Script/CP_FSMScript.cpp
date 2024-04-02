#include "pch.h"
#include "CP_FSMScript.h"
#include "CP_MouseCtrlScript.h"
#include <Engine/CRenderMgr.h>
#include <Engine/Physics.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CGameObject.h>

CP_FSMScript::CP_FSMScript()
	: m_TogleInput{ false, false, false, true }, m_StanceCheck{ false, true }, m_Weapon(nullptr)
	, P_Stance(ePlayerStance::Normal), m_StanceDelay(StanceDelay)
{
	SetScriptType(SCRIPT_TYPE::P_FSMSCRIPT);
}

CP_FSMScript::~CP_FSMScript()
{
}

void CP_FSMScript::begin()
{
	initState();
	initAnim();
	initWeapon();

	PlayAnimation(P_R2Idle, true);
	ChangeState(static_cast<UINT>(eP_States::IDLE));	
}

void CP_FSMScript::tick()
{
	m_MouseCtrl.tick();			// 마우스 할일 함, 딱히 바꿔주는건 없음
	stanceControl();			// 먼저 호출해도 이상 없을듯 (변동사항이 있는 경우 애니메이션을 호출함)
	CC_FSMScript::tick();		// 현재 State의 tick을 호출함
	inputCheck();
	colliderUpdate();
	
	// somthing debug code
	if (KEY_TAP(KEY::K))
	{
		DestroyObject(m_Weapon);
		DestroyObject(GetOwner());
	}
	if (KEY_TAP(KEY::O))
	{
		m_MuzzleFlash->ParticleSystem()->Module_Active_OnceSpawn();
	}
	if (KEY_TAP(KEY::P))
	{
		m_MuzzleFlash->ParticleSystem()->Module_Diactive_OnceSpawn();
	}
}

void CP_FSMScript::initState()
{
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEIDLESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEMOVESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEDODGESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATERELOADSCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEFIRESCRIPT)));
}

void CP_FSMScript::initAnim()
{
	Animator3D()->Add(P_R2Dodge);
	Animator3D()->Add(P_R2Dodge_L);
	Animator3D()->Add(P_R2Dodge_N);
	Animator3D()->Add(P_R2Dodge_R);

	Animator3D()->Add(P_R2Idle);
	Animator3D()->Add(P_R2IdleAim);
	Animator3D()->Add(P_R2IdleCrouch);

	Animator3D()->Add(P_R2Fire);
	Animator3D()->Add(P_R2Reload);
	Animator3D()->Add(P_R2ReloadCrouch);

	Animator3D()->Add(P_R2MoveWalk);
	Animator3D()->Add(P_R2MoveWalk_B);
	Animator3D()->Add(P_R2MoveWalk_BL);
	Animator3D()->Add(P_R2MoveWalk_BR);
	Animator3D()->Add(P_R2MoveWalk_FL);
	Animator3D()->Add(P_R2MoveWalk_FR);
	Animator3D()->Add(P_R2MoveWalkAim);
	Animator3D()->Add(P_R2MoveWalkAim_B);
	Animator3D()->Add(P_R2MoveWalkAim_BL);
	Animator3D()->Add(P_R2MoveWalkAim_BR);
	Animator3D()->Add(P_R2MoveWalkAim_FL);
	Animator3D()->Add(P_R2MoveWalkAim_FR);
	Animator3D()->Add(P_R2MoveWalkCrouch);
	Animator3D()->Add(P_R2MoveWalkCrouch_B);
	Animator3D()->Add(P_R2MoveWalkCrouch_BL);
	Animator3D()->Add(P_R2MoveWalkCrouch_BR);
	Animator3D()->Add(P_R2MoveWalkCrouch_FL);
	Animator3D()->Add(P_R2MoveWalkCrouch_FR);
	Animator3D()->Add(P_R2MoveSprint);
	Animator3D()->Add(P_R2MoveSprint_L);
	Animator3D()->Add(P_R2MoveSprint_R);

	Animator3D()->CompleteEvent(P_R2Fire) = std::bind(&CP_FSMScript::AfterCallAnim, this);
	Animator3D()->CompleteEvent(P_R2Reload) = std::bind(&CP_FSMScript::GotoIdle, this);
	Animator3D()->CompleteEvent(P_R2ReloadCrouch) = std::bind(&CP_FSMScript::GotoIdle, this);

	Animator3D()->CompleteEvent(P_R2Dodge) = std::bind(&CP_FSMScript::GotoMove, this);
	Animator3D()->CompleteEvent(P_R2Dodge_L) = std::bind(&CP_FSMScript::GotoMove, this);
	Animator3D()->CompleteEvent(P_R2Dodge_N) = std::bind(&CP_FSMScript::GotoMove, this);
	Animator3D()->CompleteEvent(P_R2Dodge_R) = std::bind(&CP_FSMScript::GotoMove, this);
}

void CP_FSMScript::initWeapon()
{
	Ptr<CMeshData> pMeshData = nullptr;
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\P_AssaultRifle.mdat");
	CGameObject* weapon = pMeshData->InstMesh();
	weapon->MeshRender()->SetFrustumCheck(false);
	weapon->SetName(L"LongGun");
	m_Weapon = weapon;
	m_MouseCtrl.SetWeaponObj(m_Weapon);
	CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(weapon, (UINT)LAYER_TYPE::Player, true);
	GetOwner()->AddChild(weapon);


	Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\P_FixMuzzleFlash.pref", L"prefab\\P_FixMuzzleFlash.pref");
	fab->PrefabLoad(L"prefab\\P_FixMuzzleFlash.pref");
	m_MuzzleFlash = fab.Get()->Instantiate(Vec3(13.f, 172.f, 108.f), 1);
	m_MuzzleFlash->SetName(L"P_MuzzleFlash");
	CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(m_MuzzleFlash, (UINT)LAYER_TYPE::Player, true);
	GetOwner()->AddChild(m_MuzzleFlash);
	tParticleModule ModuleData = m_MuzzleFlash->ParticleSystem()->GetModuleData();
	ModuleData.bDead = true;											// 이거 값이 true 인지 확인하기
	m_MuzzleFlash->ParticleSystem()->SetModuleData(ModuleData);
	m_MuzzleFlash->ParticleSystem()->Module_Active_OnceSpawn();
	
	fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\P_FixBullet.pref", L"prefab\\P_FixBullet.pref");
	fab->PrefabLoad(L"prefab\\P_FixBullet.pref");
	m_Bullet = fab.Get()->Instantiate(Vec3(5.f, 152.f, 110.f), 1);
	m_Bullet->SetName(L"P_Bullet");
	CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(m_Bullet, (UINT)LAYER_TYPE::Player, true);
	GetOwner()->AddChild(m_Bullet);
	ModuleData = m_Bullet->ParticleSystem()->GetModuleData();
	ModuleData.bDead = true;
	m_Bullet->ParticleSystem()->SetModuleData(ModuleData);
	m_Bullet->ParticleSystem()->Module_Active_OnceSpawn();

	m_MouseCtrl.SetOwner(this);
	CCamera* cam = CRenderMgr::GetInst()->GetMainCam();
	m_MouseCtrl.SetMainCam(cam);
	m_MouseCtrl.begin();
}


void CP_FSMScript::stanceControl()
{
	if (GetAtkSign())
	{
		tHitInfo info = GetHitInfo();
		ChangeState((UINT)eP_States::DAMAGED);
	}

	if (m_StanceCheck[(UINT)eStanceCheck::IsChange])
	{
		CP_StatesScript* curState = dynamic_cast<CP_StatesScript*>(GetCurState());
		if (m_TogleInput[(UINT)eInpStance::Sprint])
		{
			ChangeStance(ePlayerStance::Sprint);
			curState->CallAnimation();
			m_MouseCtrl.SetPivot(PIVOT_HIGH);
			m_MouseCtrl.SetFov(FOV_HIGH);
		}
		else if (m_TogleInput[(UINT)eInpStance::Aim])
		{
			ChangeStance(ePlayerStance::Aim);
			curState->CallAnimation();
			m_MouseCtrl.SetPivot(PIVOT_HIGH);
			m_MouseCtrl.SetFov(FOV_LOW);
			OverrideObjRotY();
		}
		else if (m_TogleInput[(UINT)eInpStance::Crouch])
		{
			ChangeStance(ePlayerStance::Crouch);
			curState->CallAnimation();
			m_MouseCtrl.SetPivot(PIVOT_LOW);
			m_MouseCtrl.SetFov(FOV_HIGH);
		}
		else
		{
			ChangeStance(ePlayerStance::Normal);
			curState->CallAnimation();
			m_MouseCtrl.SetPivot(PIVOT_HIGH);
			m_MouseCtrl.SetFov(FOV_HIGH);
		}

		m_StanceCheck[(UINT)eStanceCheck::IsChange] = false;

		// 자세 변경시 딜레이 주는 코드였..음
		//m_StanceDelay.curTime += ScaleDT;
		//
		//if (m_StanceDelay.IsFinish())
		//{
		//	m_StanceDelay.ResetTime();
		//	
		//}
	}
}

void CP_FSMScript::dirInput()
{
	if (KEY_TAP(KEY::W))
	{
		InputMove(0, 1.f);
	}
	if (KEY_TAP(KEY::S))
	{
		InputMove(0, -1.f);
	}
	if (KEY_TAP(KEY::A))
	{
		InputMove(-1.f, 0);
	}
	if (KEY_TAP(KEY::D))
	{
		InputMove(1.f, 0);
	}
	if (KEY_RELEASE(KEY::W))
	{
		InputMove(0, -1.f);
	}
	if (KEY_RELEASE(KEY::S))
	{
		InputMove(0, 1.f);
	}
	if (KEY_RELEASE(KEY::D))
	{
		InputMove(-1.f, 0);
	}
	if (KEY_RELEASE(KEY::A))
	{
		InputMove(1.f, 0);
	}
}

void CP_FSMScript::stanceInput()
{
	if (KEY_TAP(KEY::RBTN))
	{
		if (IsInput((UINT)eInpStance::Crouch))
			InputCrouch();

		InputAim();
	}
	if (KEY_TAP(KEY::LCTRL))
	{
		if (IsInput((UINT)eInpStance::Aim))
			InputAim();

		InputCrouch();
	}

	if (KEY_TAP(KEY::LSHIFT))
		InputSprint(true);
	if (KEY_RELEASE(KEY::LSHIFT))
		InputSprint(false);

	if (KEY_TAP(KEY::SPACE))
	{
		if (IsInput((UINT)eInpStance::Crouch))
			InputCrouch();
		if (IsInput((UINT)eInpStance::Aim))
			InputAim();
		InputSprint(false);

		DoDodge();
	}
}

void CP_FSMScript::inputCheck()
{
	if (m_TogleInput[(UINT)eInpStance::Mouse])
	{
		dirInput();				// wasd 방향에 대한 키입력을 감지함
		stanceInput();			// 자세 변경에 대한 키입력을 감지함
	}
}

void CP_FSMScript::colliderUpdate()
{
	Matrix retBoneMat = Animator3D()->GetBoneMatrix(46);
	Matrix ownerMat = GetOwner()->Transform()->GetWorldMat();
	Matrix totalMat = retBoneMat * ownerMat;
	Vec3 retPos = totalMat.Translation();
	Vec4 retRot = DirectX::XMQuaternionRotationMatrix(totalMat);
	PxTransform retTransform;
	retTransform.p.x = retPos.x;
	retTransform.p.y = retPos.y;
	retTransform.p.z = retPos.z;

	retTransform.q.x = retRot.x;
	retTransform.q.y = retRot.y;
	retTransform.q.z = retRot.z;
	retTransform.q.w = retRot.w;

	RigidBody()->SetShapeLocalPxTransform(1, retTransform);


	retBoneMat = Animator3D()->GetBoneMatrix(10);
	totalMat = retBoneMat * ownerMat;
	retPos = totalMat.Translation();
	retRot = DirectX::XMQuaternionRotationMatrix(totalMat);
	retTransform.p.x = retPos.x;
	retTransform.p.y = retPos.y;
	retTransform.p.z = retPos.z;

	retTransform.q.x = retRot.x;
	retTransform.q.y = retRot.y;
	retTransform.q.z = retRot.z;
	retTransform.q.w = retRot.w;
	RigidBody()->SetShapeLocalPxTransform(2, retTransform);
}

void CP_FSMScript::PlayAnimation(wstring _name, bool _repeat)
{
	GetOwner()->Animator3D()->Play(_name, _repeat);
	
}

void CP_FSMScript::AfterCallAnim()
{
	CP_StatesScript* curState = dynamic_cast<CP_StatesScript*>(GetCurState());
	curState->CallAnimation();
}

void CP_FSMScript::DoDodge()
{
	(0 <= m_moveDir.y) ? m_StanceCheck[(UINT)eStanceCheck::IsFrontDir] = true : m_StanceCheck[(UINT)eStanceCheck::IsFrontDir] = false;

	// 방향에 맞는 애니메이션 재생
	if (0.3 <= m_moveDir.x)
		PlayAnimation(P_R2Dodge_R, false);
	else if (-0.3 >= m_moveDir.x)
		PlayAnimation(P_R2Dodge_L, false);
	else
	{
		if (m_StanceCheck[(UINT)eStanceCheck::IsFrontDir])
			PlayAnimation(P_R2Dodge, false);
		else
			PlayAnimation(P_R2Dodge_N, false);
	}

	ChangeState(static_cast<UINT>(eP_States::DODGE));
}

void CP_FSMScript::ShootRay()
{
	CCamera* cam = CRenderMgr::GetInst()->GetMainCam();
	tRay rayInfo = cam->GetRay();
	tRayCastInfo*  rayResult = Physics::GetInst()->RayCast(rayInfo.vStart, rayInfo.vDir, 10000.f);
	// 레이를 허공에 날린 경우
	if (!rayResult->hitActor)
		return;
	CGameObject* targetObj = (CGameObject*)rayResult->hitActor->userData;
	UINT targetLayer = -1;
	CC_FSMScript* monsterScript = nullptr;
	bool keepGoing = false;

	if (targetObj)
		targetLayer = targetObj->GetLayerIndex();
	if (-1 != targetLayer && (UINT)LAYER_TYPE::Monster == targetLayer)
	{
		monsterScript = targetObj->GetScript<CC_FSMScript>();
		keepGoing = true;
	}
	if (keepGoing)
	{
		tHitInfo give = { GetOwner()->Transform()->GetRelativePos(),
			rayResult->hitPos, GetOwner(), 10 };
		monsterScript->GiveAtkInfo(give);
	}
}

void CP_FSMScript::GotoIdle()
{
	ChangeState(static_cast<UINT>(eP_States::IDLE));
}

void CP_FSMScript::GotoMove()
{
	ChangeState(static_cast<UINT>(eP_States::MOVE));
}

void CP_FSMScript::BeginOverlap(CCollider3D* _Other)
{

}

void CP_FSMScript::OnOverlap(CCollider3D* _Other)
{
}

void CP_FSMScript::EndOverlap(CCollider3D* _Other)
{
}


void CP_FSMScript::SaveToScene(FILE* _pFile)
{
}

void CP_FSMScript::LoadFromScene(FILE* _pFile)
{
}

