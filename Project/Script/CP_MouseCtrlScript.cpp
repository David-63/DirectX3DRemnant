#include "pch.h"
#include "CP_MouseCtrlScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CEngine.h>
#include "CP_FSMScript.h"

CP_MouseCtrlScript::CP_MouseCtrlScript()
	: CScript((UINT)SCRIPT_TYPE::P_MOUSECTRLSCRIPT), m_PHQ(nullptr), m_ctrlCam(nullptr), m_curPivot(PIVOT_HIGH), m_curFov(FOV_HIGH)	
, m_MouseSensitivity(0.4f), m_vCamOffset{ -200.f, 5.f, 45.f }, m_PivotBlend(0.5f), m_FovBlend(0.5f), m_IsChangeStance(false)
{
}

CP_MouseCtrlScript::~CP_MouseCtrlScript()
{
}

void CP_MouseCtrlScript::tick()
{
	if (KEY_TAP(KEY::ESC))
		m_PHQ->AbleMouse();

	if (!m_PHQ->IsAbleMouse())
		return;
	
	MoveCameraPos();
	MoveCameraRot();
	MouseRock();
}

void CP_MouseCtrlScript::MoveCameraPos()
{
	// �ڼ� ��������
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();

	// Transform ���� ��������
	Vec3 objPos = m_PHQ->Transform()->GetRelativePos();
	Vec3 camF = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 camR = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 camU = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::UP);

	// lerp ���� (FSM���� Ȱ��ȭ ��Ŵ)
	if (m_PivotBlend.IsActivate())
	{
		m_PivotBlend.curTime += ScaleDT;
		if (m_PivotBlend.IsFinish())
		{
			m_PivotBlend.ResetTime();
			m_pivotRatio = 0.f;
		}

		m_pivotRatio = m_PivotBlend.curTime / m_PivotBlend.maxTime;

		m_curPivot = FloatLerp(m_curPivot, m_pivotValue, m_pivotRatio);
	}
	if (m_FovBlend.IsActivate())
	{
		m_FovBlend.curTime += ScaleDT;
		if (m_FovBlend.IsFinish())
		{
			m_FovBlend.ResetTime();
			m_fovRatio = 0.f;
		}
		m_fovRatio = m_FovBlend.curTime / m_FovBlend.maxTime;

		m_curFov = FloatLerp(m_curFov, m_fovValue, m_fovRatio);
		m_ctrlCam->SetFov(m_curFov);
	}

	// ������� ����
	objPos.y = m_curPivot;
	Vec3 Point = objPos + camF * m_vCamOffset.x + camR * m_vCamOffset.z + camU * m_vCamOffset.y; // OffX : front, offZ : right, offY : Up
	m_ctrlCam->Transform()->SetRelativePos(Point);
}

void CP_MouseCtrlScript::MoveCameraRot()
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

	float deltaYaw = XMConvertToRadians(mouseInput.x * m_MouseSensitivity);
	float deltaPitch = XMConvertToRadians(mouseInput.y * m_MouseSensitivity); // Y�� ���� ó��
	float xRot, yRot;

	// state::idle
	if (justRotCam)
	{
		xRot = getCamRot.x + deltaPitch;
		yRot = getCamRot.y + deltaYaw;
		Vec3 outCamEuler = Vec3(xRot, yRot, (int)0);
		m_ctrlCam->Transform()->SetRelativeRot(outCamEuler);
	}
	// state::move || stance::Aim
	else
	{
		xRot = getCamRot.x + deltaPitch;
		yRot = getObjRot.y + deltaYaw;
		Vec3 outObjEuler = Vec3(0, yRot, (int)0);
		Vec3 outCamEuler = Vec3(xRot, yRot + XM_PI, (int)0);
		m_PHQ->Transform()->SetRelativeRot(outObjEuler);
		m_ctrlCam->Transform()->SetRelativeRot(outCamEuler);
	}
}

void CP_MouseCtrlScript::MouseRock()
{
	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();
	// ȭ�� �߾� ��ǥ ���
	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	// ���콺 Ŀ���� ȭ�� �߾����� �缳��
	SetCursorPos(centerX, centerY);
}