#include "pch.h"
#include "CP_MouseCtrlScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CEngine.h>
#include "CP_FSMScript.h"

CP_MouseCtrlScript::CP_MouseCtrlScript()
	: CScript((UINT)SCRIPT_TYPE::P_MOUSECTRLSCRIPT), m_PHQ(nullptr), m_ctrlCam(nullptr)
	, m_CamInfo({ -180.f, 5.f, 35.f }, 0.54f), m_IsChangeStance(false), m_PivotValue(PIVOT_HIGH), m_FovValue(FOV_HIGH)
{
}

CP_MouseCtrlScript::~CP_MouseCtrlScript()
{
}

void CP_MouseCtrlScript::begin()
{
}

void CP_MouseCtrlScript::tick()
{
	if (KEY_TAP(KEY::ESC))
		m_PHQ->AbleMouse();

	if (!m_PHQ->IsAbleMouse())
		return;

	ctrlMovePos();
	ctrlMoveRot();
	mouseRock();

	updateWeaponMatrix();
}

void CP_MouseCtrlScript::ctrlMovePos()
{
	// 자세 가져오기
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();

	// Transform 정보 가져오기
	Vec3 objPos = m_PHQ->Transform()->GetRelativePos();
	Vec3 camF = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 camR = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 camU = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::UP);

	// lerp 진행 (FSM에서 활성화 시킴)
	if (m_PivotValue.BlendTime.IsActivate())
	{
		m_PivotValue.BlendTime.curTime += ScaleDT;
		if (m_PivotValue.BlendTime.IsFinish())
		{
			m_PivotValue.Reset();
		}
		m_PivotValue.GetRatio();
		m_PivotValue.CurValue = FloatLerp(m_PivotValue.CurValue, m_PivotValue.TargetValue, m_PivotValue.Ratio);
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

	// 변경사항 적용
	objPos.y += m_PivotValue.CurValue;
	Vec3 Point = objPos + camF * m_CamInfo.CamOffset.x + camR * m_CamInfo.CamOffset.z + camU * m_CamInfo.CamOffset.y; // OffX : front, offZ : right, offY : Up
	m_ctrlCam->Transform()->SetRelativePos(Point);
}

void CP_MouseCtrlScript::ctrlMoveRot()
{

	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();
	eP_States stateType = static_cast<eP_States>(m_PHQ->GetCurStateType());

	bool justRotCam = false;
	if (eP_States::IDLE == stateType)
	{
		if (CP_FSMScript::ePlayerStance::Normal == curStance
			|| CP_FSMScript::ePlayerStance::Crouch == curStance
			|| CP_FSMScript::ePlayerStance::Dodge == curStance)
			justRotCam = true;
	}

	Vec3 getCamRot = m_ctrlCam->Transform()->GetRelativeRot();
	Vec3 getObjRot = m_PHQ->Transform()->GetRelativeRot();
	Vec2 mouseInput = CKeyMgr::GetInst()->GetMouseRaw();
	mouseInput.Normalize();

	float deltaYaw = XMConvertToRadians(mouseInput.x * m_CamInfo.MouseSensitivity);
	float deltaPitch = XMConvertToRadians(mouseInput.y * m_CamInfo.MouseSensitivity); // Y축 반전 처리
	float xCamRot, yObjRot;

	xCamRot = getCamRot.x + deltaPitch;
	m_CamInfo.PrevCamRotY = getCamRot.y + deltaYaw;
	yObjRot = getObjRot.y + deltaYaw;

	if (justRotCam)
	{
		Vec3 outCamEuler = Vec3(xCamRot, m_CamInfo.PrevCamRotY, (int)0);
		m_ctrlCam->Transform()->SetRelativeRot(outCamEuler);
	}
	else
	{
		Vec3 outObjEuler = Vec3(0, yObjRot, (int)0);
		Vec3 outCamEuler = Vec3(xCamRot, yObjRot, (int)0);
		m_PHQ->Transform()->SetRelativeRot(outObjEuler);
		m_ctrlCam->Transform()->SetRelativeRot(outCamEuler);
	}

}

void CP_MouseCtrlScript::mouseRock()
{
	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();
	// 화면 중앙 좌표 계산
	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	// 마우스 커서를 화면 중앙으로 재설정
	SetCursorPos(centerX, centerY);
}

void CP_MouseCtrlScript::updateWeaponMatrix()
{
	// Get Bone FrameData
	tMTBone handBoneData = m_PHQ->Animator3D()->GetMTBoneData(176);
	int frameIdx = m_PHQ->Animator3D()->GetCurFrame();
	Vec3 trans = handBoneData.vecKeyFrame[frameIdx].vTranslate;
	Vec3 scale = handBoneData.vecKeyFrame[frameIdx].vScale;
	Vec4 qRot = handBoneData.vecKeyFrame[frameIdx].qRot;

	// bone Matrix 구성하기
	Matrix keyScaleMat = XMMatrixIdentity();
	keyScaleMat = XMMatrixScaling(scale.x, scale.y, scale.z);

	Vec3 oRot = QuatToEuler(qRot);
	Matrix keyRotMat = XMMatrixIdentity();
	keyRotMat = XMMatrixRotationX(oRot.x);
	keyRotMat *= XMMatrixRotationY(oRot.y);
	keyRotMat *= XMMatrixRotationZ(oRot.z);

	Matrix keyTranMat = XMMatrixTranslation(trans.x, trans.y, trans.z);

	Matrix boneMat = keyScaleMat * keyRotMat * keyTranMat;

	// Owner Mat 과 bone Mat 곱하기
	Matrix ownerMat = m_PHQ->Transform()->GetWorldMat();
	Matrix totalMat = ownerMat * boneMat;

	// total Mat 으로부터 위치값 가져오기
	Vec3 bonePosition = boneMat.Translation();
	m_Weapon->Transform()->SetRelativePos(bonePosition);

	// total Mat 으로부터 회전값 가져오기
	Vec4 boneQRot = DirectX::XMQuaternionRotationMatrix(boneMat);
	Vec3 boneRot = QuatToEuler(boneQRot);
	m_Weapon->Transform()->SetRelativeRot(boneRot);

}


void CP_MouseCtrlScript::OverrideObjRotY()
{
	m_PHQ->Transform()->SetRelativeRot(Vec3(0.f, m_CamInfo.PrevCamRotY, 0.f));
}