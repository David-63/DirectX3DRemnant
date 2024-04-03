#include "pch.h"
#include "CP_MouseCtrlScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CEngine.h>
#include "CP_FSMScript.h"

CP_MouseCtrlScript::CP_MouseCtrlScript()
	: CScript((UINT)SCRIPT_TYPE::P_MOUSECTRLSCRIPT), m_PHQ(nullptr), m_ctrlCam(nullptr), m_Weapon(nullptr), m_LongGun(nullptr)
	, m_CamInfo({ 100.f, 140.f, -180.f }, 0.54f), m_IsChangeStance(false), m_PivotValue(PIVOT_HIGH), m_FovValue(FOV_HIGH)
	
{
	m_RotMat[(UINT)eRotMat::Normal] = XMMatrixIdentity();
	m_RotMat[(UINT)eRotMat::Normal] = XMMatrixRotationX(0);
	m_RotMat[(UINT)eRotMat::Normal] *= XMMatrixRotationY(0);
	m_RotMat[(UINT)eRotMat::Normal] *= XMMatrixRotationZ(-2.7f);

	m_RotMat[(UINT)eRotMat::MoveLeft] = XMMatrixIdentity();
	m_RotMat[(UINT)eRotMat::MoveLeft] = XMMatrixRotationX(0.6f);
	m_RotMat[(UINT)eRotMat::MoveLeft] *= XMMatrixRotationY(1.4f);
	m_RotMat[(UINT)eRotMat::MoveLeft] *= XMMatrixRotationZ(-2.2f);

	// 문제있음
	m_RotMat[(UINT)eRotMat::MoveRight] = XMMatrixIdentity();
	m_RotMat[(UINT)eRotMat::MoveRight] = XMMatrixRotationX(-1.4f);
	m_RotMat[(UINT)eRotMat::MoveRight] *= XMMatrixRotationY(-0.2f);
	m_RotMat[(UINT)eRotMat::MoveRight] *= XMMatrixRotationZ(2.8f);
	
	m_RotMat[(UINT)eRotMat::Crouch] = XMMatrixIdentity();
	m_RotMat[(UINT)eRotMat::Crouch] = XMMatrixRotationX(-0.35f);
	m_RotMat[(UINT)eRotMat::Crouch] *= XMMatrixRotationY(0.18f);
	m_RotMat[(UINT)eRotMat::Crouch] *= XMMatrixRotationZ(3.3f);

	// 문제있음
	m_RotMat[(UINT)eRotMat::CrouchLeft] = XMMatrixIdentity();
	m_RotMat[(UINT)eRotMat::CrouchLeft] = XMMatrixRotationX(0);
	m_RotMat[(UINT)eRotMat::CrouchLeft] *= XMMatrixRotationY(0);
	m_RotMat[(UINT)eRotMat::CrouchLeft] *= XMMatrixRotationZ(0);

	// 문제있음
	m_RotMat[(UINT)eRotMat::CrouchRight] = XMMatrixIdentity();
	m_RotMat[(UINT)eRotMat::CrouchRight] = XMMatrixRotationX(0);
	m_RotMat[(UINT)eRotMat::CrouchRight] *= XMMatrixRotationY(0);
	m_RotMat[(UINT)eRotMat::CrouchRight] *= XMMatrixRotationZ(0);

	m_RotMat[(UINT)eRotMat::Aim] = XMMatrixIdentity();
	m_RotMat[(UINT)eRotMat::Aim] = XMMatrixRotationX(0.1f);
	m_RotMat[(UINT)eRotMat::Aim] *= XMMatrixRotationY(4.8f);
	m_RotMat[(UINT)eRotMat::Aim] *= XMMatrixRotationZ(3.3f);
	
}

CP_MouseCtrlScript::~CP_MouseCtrlScript()
{
}

void CP_MouseCtrlScript::begin()
{
	m_PlayerStance = m_PHQ->GetStance();

	m_ctrlCam->Transform()->SetRelativePos(m_CamInfo.CamOffset);
}

void CP_MouseCtrlScript::tick()
{
	if (KEY_TAP(KEY::ESC))
		m_PHQ->AbleMouse();

	if (!m_PHQ->IsAbleMouse())
		return;

	changeFov();
	changeTransfrom();
	mouseRock();

	updateWeaponMatrix();
}

void CP_MouseCtrlScript::changeFov()
{
	if (m_PivotValue.BlendTime.IsActivate())
	{
		m_PivotValue.BlendTime.curTime += ScaleDT;
		if (m_PivotValue.BlendTime.IsFinish())
		{
			m_PivotValue.Reset();
		}
		m_PivotValue.GetRatio();
		m_PivotValue.CurValue = FloatLerp(m_PivotValue.CurValue, m_PivotValue.TargetValue, m_PivotValue.Ratio);
		m_ctrlCam->SetFov(m_FovValue.CurValue);
	}
	if (m_FovValue.BlendTime.IsActivate())
	{
		m_FovValue.BlendTime.curTime += ScaleDT;
		if (m_FovValue.BlendTime.IsFinish())
		{
			m_FovValue.Reset();
		}
		m_FovValue.GetRatio();
		m_FovValue.CurValue = FloatLerp(m_FovValue.CurValue, m_FovValue.TargetValue, m_FovValue.Ratio);
		m_ctrlCam->SetFov(m_FovValue.CurValue);
	}
	
}

void CP_MouseCtrlScript::changeTransfrom()
{
	// Find stance or state about Rotation Mode
	eP_States stateType = static_cast<eP_States>(m_PHQ->GetCurStateType());
	

	// ==========================================================================


	// 이부분이 필요하긴 한데
	// 상대적으로 계산해야하니까 수식을 수정해야함
	// Vec3 objPos = m_PHQ->Transform()->GetRelativePos();
	// Vec3 camF = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	// Vec3 camR = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	// Vec3 camU = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::UP);
	// objPos.y += m_PivotValue.CurValue;
	// Vec3 Point = objPos + camF * m_CamInfo.CamOffset.x + camR * m_CamInfo.CamOffset.z + camU * m_CamInfo.CamOffset.y; // OffX : front, offZ : right, offY : Up
	// m_ctrlCam->Transform()->SetRelativePos(Point);
	
	// Find transform value
	Vec3 getCamRot = m_ctrlCam->Transform()->GetRelativeRot();
	Vec3 getObjRot = m_PHQ->Transform()->GetRelativeRot();
	Vec2 mouseInput = CKeyMgr::GetInst()->GetMouseRaw();
	mouseInput.Normalize();

	
	
	// Calculate move magnitude
	float deltaYaw = XMConvertToRadians(mouseInput.x * m_CamInfo.MouseSensitivity);		// Y 축 회전, 수평으로 돌리는데 사용 
	float deltaPitch = XMConvertToRadians(mouseInput.y * m_CamInfo.MouseSensitivity);	// X 축 회전, 수직으로 돌리는데 사용
	float xCamRot, yObjRot;

	m_CamInfo.PrevCamRotY = getCamRot.y + deltaYaw;		// 카메라 수평 회전량 구하기
	xCamRot = getCamRot.x + deltaPitch;					// 카메라 수직 회전량 구하기

	yObjRot = getObjRot.y + deltaYaw;					// 오브젝트의 수평 회전량 구하기


	// =============================================================================
	

	// 회전 조건
	bool justRotCam = false;
	if (eP_States::IDLE == stateType)
	{
		if (ePlayerStance::Normal == *m_PlayerStance
			|| ePlayerStance::Crouch == *m_PlayerStance
			|| ePlayerStance::Dodge == *m_PlayerStance)
			justRotCam = true;
	}
	
	
	// Apply change according to Rotation Mode
	if (justRotCam)
	{
		//Vec3 outCamEuler = Vec3(xCamRot, m_CamInfo.PrevCamRotY, 0);
		//m_ctrlCam->Transform()->SetRelativeRot(outCamEuler);
		
		// 다시 생각해보니까
		// 위의 Move 스크립트는 사실상 없어도 되고

		// rot으로 카메라 만 회전시켜야 하면
		// 오브젝트와 카메라간에 오프셋 벡터 방향만큼 상대적으로 거리를 유지해야함

		// 카메라만 회전시키는건데
		// 본격적으로 회전시키려면 카메라의 위치도 바꿔주고
		// 카메라의 위치도 회전시켜야함


		
		// 카메라의 회전 구하기
		Vec3 outCamEuler = Vec3(xCamRot, m_CamInfo.PrevCamRotY, 0);
		m_ctrlCam->Transform()->SetRelativeRot(outCamEuler);


		// 카메라의 위치 구하기
		// Object Position 으로부터 Front 방향 벡터를 가져와서
		// Vec3 Point = objPos + camF * m_CamInfo.CamOffset.x + camR * m_CamInfo.CamOffset.z + camU * m_CamInfo.CamOffset.y; // OffX : front, offZ : right, offY : Up
		// m_ctrlCam->Transform()->SetRelativePos(Point);
		// 이거로 구하는게 맞긴한듯?
		// 상대적이니까 objPos가 없어도 될거같음

		// owner 방향 가져와서
		//Vec3 ownerF = m_ctrlCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		//Vec3 ownerR = m_ctrlCam->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		//Vec3 ownerU = m_ctrlCam->Transform()->GetWorldDir(DIR_TYPE::UP);
		//Vec3 Point = ownerF * m_CamInfo.CamOffset.x + ownerR * -m_CamInfo.CamOffset.z + ownerU * m_CamInfo.CamOffset.y; // OffX : front, offZ : right, offY : Up
		//m_ctrlCam->Transform()->SetRelativePos(Point);

		// 여기까지
		// 카메라를 회전시키고, 그담에 오브젝트의 방향에 맞게 위치를 이동시킴

	}
	else
	{
		Vec3 outObjEuler = Vec3(0, yObjRot, 0);
		Vec3 outCamEuler = Vec3(xCamRot, yObjRot, 0);
		m_PHQ->Transform()->SetRelativeRot(outObjEuler);
		m_ctrlCam->Transform()->SetRelativeRot(Vec3(0,0,0));
		m_Weapon->Transform()->SetRelativeRot(outObjEuler);
	}
}

void CP_MouseCtrlScript::mouseRock()
{
	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();
	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	// Force cursor to move
	SetCursorPos(centerX, centerY);
}

void CP_MouseCtrlScript::updateWeaponMatrix()
{
	Matrix retBoneMat = m_PHQ->Animator3D()->GetBoneMatrix(176);
	retBoneMat._44 = 1;
	Vec3 bonePosition = retBoneMat.Translation();
	m_Weapon->Transform()->SetRelativePos(bonePosition);

	Matrix weaponMat = retBoneMat * m_CurRotMat;
	Vec4 boneQRot = DirectX::XMQuaternionRotationMatrix(weaponMat);
	Vec3 boneRot = QuatToEuler(boneQRot);
	m_Weapon->Transform()->SetRelativeRot(boneRot);
}


void CP_MouseCtrlScript::OverrideObjRotY()
{
	m_PHQ->Transform()->SetRelativeRot(Vec3(0.f, m_CamInfo.PrevCamRotY, 0.f));
}