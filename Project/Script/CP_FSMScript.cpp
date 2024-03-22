#include "pch.h"
#include "CP_FSMScript.h"
#include "CP_MouseCtrlScript.h"
#include <Engine/CRenderMgr.h>

CP_FSMScript::CP_FSMScript()
	: m_InpStance{ false, false, false, true }, m_IsChangeStance(true)
	, P_Stance(ePlayerStance::Normal), m_StanceDelay(StanceDelay)
{
}

CP_FSMScript::~CP_FSMScript()
{
}

void CP_FSMScript::begin()
{
	// State 넣어주기
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEIDLESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEMOVESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEDODGESCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATERELOADSCRIPT)));
	AddState(dynamic_cast<CC_StatesScript*>(CScriptMgr::GetScript(SCRIPT_TYPE::P_STATEFIRESCRIPT)));


	// 애니메이션
	GetOwner()->Animator3D()->Add(P_R2Dodge);
	GetOwner()->Animator3D()->Add(P_R2Dodge_L);
	GetOwner()->Animator3D()->Add(P_R2Dodge_N);
	GetOwner()->Animator3D()->Add(P_R2Dodge_R);
		
	GetOwner()->Animator3D()->Add(P_R2Idle);
	GetOwner()->Animator3D()->Add(P_R2IdleAim);
	GetOwner()->Animator3D()->Add(P_R2IdleCrouch);
	
	GetOwner()->Animator3D()->Add(P_R2Fire);
	GetOwner()->Animator3D()->Add(P_R2Reload);
	GetOwner()->Animator3D()->Add(P_R2ReloadCrouch);
	
	GetOwner()->Animator3D()->Add(P_R2MoveWalk);
	GetOwner()->Animator3D()->Add(P_R2MoveWalk_B);
	GetOwner()->Animator3D()->Add(P_R2MoveWalk_BL);
	GetOwner()->Animator3D()->Add(P_R2MoveWalk_BR);
	GetOwner()->Animator3D()->Add(P_R2MoveWalk_FL);
	GetOwner()->Animator3D()->Add(P_R2MoveWalk_FR);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkAim);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkAim_B);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkAim_BL);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkAim_BR);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkAim_FL);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkAim_FR);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkCrouch);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkCrouch_B);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkCrouch_BL);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkCrouch_BR);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkCrouch_FL);
	GetOwner()->Animator3D()->Add(P_R2MoveWalkCrouch_FR);
	GetOwner()->Animator3D()->Add(P_R2MoveSprint);
	GetOwner()->Animator3D()->Add(P_R2MoveSprint_L);
	GetOwner()->Animator3D()->Add(P_R2MoveSprint_R);

	

	GetOwner()->Animator3D()->CompleteEvent(P_R2Fire) = std::bind(&CP_FSMScript::GotoIdle, this);
	GetOwner()->Animator3D()->CompleteEvent(P_R2Reload) = std::bind(&CP_FSMScript::GotoIdle, this);
	GetOwner()->Animator3D()->CompleteEvent(P_R2ReloadCrouch) = std::bind(&CP_FSMScript::GotoIdle, this);

	GetOwner()->Animator3D()->CompleteEvent(P_R2Dodge) = std::bind(&CP_FSMScript::GotoMove, this);
	GetOwner()->Animator3D()->CompleteEvent(P_R2Dodge_L) = std::bind(&CP_FSMScript::GotoMove, this);
	GetOwner()->Animator3D()->CompleteEvent(P_R2Dodge_N) = std::bind(&CP_FSMScript::GotoMove, this);
	GetOwner()->Animator3D()->CompleteEvent(P_R2Dodge_R) = std::bind(&CP_FSMScript::GotoMove, this);

	// GetOwner()->Animator3D()->CompleteEvent(P_MoveR2Jog)

	PlayAnimation(P_R2Idle, true);
	ChangeState(static_cast<UINT>(eP_States::IDLE));


	Ptr<CMeshData> pMeshData = nullptr;
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\P_AssaultRifle.mdat");
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\player\\P_AssaultRifle.fbx");
	
	m_Weapon = nullptr;
	m_Weapon = pMeshData->InstMesh();
	m_Weapon->SetName(L"LongGun");
	m_Weapon->MeshRender()->SetFrustumCheck(false);
	SpawnGameObject(m_Weapon, Vec3(200.f, 0.f, 0.f), 1);
	

	m_MouseCtrl.SetOwner(this);
	m_MouseCtrl.SetWeaponObj(m_Weapon);
	m_MouseCtrl.SetMainCam(CRenderMgr::GetInst()->GetMainCam());
	m_MouseCtrl.begin();
}

void CP_FSMScript::tick()
{
	CC_FSMScript::tick();	// 현재 State의 tick을 호출	
	dirInput();
	stanceControl(); // Stance 변동 감지 및 제어
	m_MouseCtrl.tick(); // 상태 적용이 완료된 다음에 마우스 호출
}

void CP_FSMScript::stanceControl()
{
	if (m_IsChangeStance)
	{
		m_StanceDelay.curTime += ScaleDT;

		if (m_StanceDelay.IsFinish())
		{
			m_StanceDelay.ResetTime();
			m_MouseCtrl.ChangeCamValue();
			CP_StatesScript* curState = dynamic_cast<CP_StatesScript*>(GetCurState());
			if (m_InpStance[(UINT)eInpStance::Sprint])
			{
				ChangeStance(ePlayerStance::Sprint);
				curState->CallAnimation();
				m_MouseCtrl.SetPivot(PIVOT_HIGH);
				m_MouseCtrl.SetFov(FOV_HIGH);
			}
			else if (m_InpStance[(UINT)eInpStance::Aim])
			{
				ChangeStance(ePlayerStance::Aim);
				curState->CallAnimation();
				m_MouseCtrl.SetPivot(PIVOT_HIGH);
				m_MouseCtrl.SetFov(FOV_LOW);				
			}
			else if (m_InpStance[(UINT)eInpStance::Crouch])
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
			m_IsChangeStance = false;
		}
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

void CP_FSMScript::PlayAnimation(wstring _name, bool _repeat)
{
	GetOwner()->Animator3D()->Play(_name, _repeat);
	
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

