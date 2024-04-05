#include "pch.h"
#include "CP_MouseCtrlScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CEngine.h>
#include "CP_FSMScript.h"

CP_MouseCtrlScript::CP_MouseCtrlScript()
	: CScript((UINT)SCRIPT_TYPE::P_MOUSECTRLSCRIPT), m_PHQ(nullptr), m_ctrlCam(nullptr), m_Weapon(nullptr), m_LongGun(nullptr)
	, m_CamInfo({ 40.f, 100.f, -150.f }, 1.54f), m_IsChangeStance(false), m_PivotValue(PIVOT_HIGH), m_FovValue(FOV_HIGH) // 1.14f
	
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
	// Find transform value
	Vec3 getCamRot = m_ctrlCam->Transform()->GetRelativeRot();
	Vec3 getObjRot = m_PHQ->Transform()->GetRelativeRot();
	
	// Calculate move position & rotation
	Vec2 mouseInput = CKeyMgr::GetInst()->GetMouseRaw();
	mouseInput.Normalize();
	float deltaYaw = XMConvertToRadians(mouseInput.x * m_CamInfo.MouseSensitivity);		// Y 축 회전, 수평으로 돌리는데 사용 
	float deltaPitch = XMConvertToRadians(mouseInput.y * m_CamInfo.MouseSensitivity);	// X 축 회전, 수직으로 돌리는데 사용
	float xCamRot, yObjRot;
	xCamRot = getCamRot.x + deltaPitch;					// 카메라 수직 회전량 구하기
	yObjRot = getObjRot.y + deltaYaw;					// 오브젝트의 수평 회전량 구하기
	Vec3 outCamEuler = Vec3(xCamRot, 0, 0);
	Vec3 outObjEuler = Vec3(0, yObjRot, 0);

	m_PHQ->Transform()->SetRelativeRot(outObjEuler);
	m_Weapon->Transform()->SetRelativeRot(outObjEuler);
	m_ctrlCam->Transform()->SetRelativeRot(outCamEuler);

	Vec3 Point = m_CamInfo.CamOffset;
	Point.y += m_PivotValue.CurValue;
	m_ctrlCam->Transform()->SetRelativePos(Point);	
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
