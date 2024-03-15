#include "pch.h"
#include "CP_MouseCtrlScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CEngine.h>
#include "CP_FSMScript.h"

CP_MouseCtrlScript::CP_MouseCtrlScript()
	: CScript((UINT)SCRIPT_TYPE::P_MOUSECTRLSCRIPT), m_PHQ(nullptr), m_ctrlCam(nullptr)
	, m_MouseSensitivity(0.13f), m_vCamOffset{ -200.f, 5.f, 45.f }, m_YPivot(140.f)
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
	
	// 마우스 입력 받기
	CalcMouseAxisInput();
	// 카메라 이동 시키기
	MoveCamera();

	// 마우스 가두기
	MouseRock();
	
}

void CP_MouseCtrlScript::MouseRock()
{
	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();
	// 화면 중앙 좌표 계산
	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	RECT clip;
	clip.left = centerX - 10;
	clip.top = centerY - 10;
	clip.right = centerX + 10;
	clip.bottom = centerY + 10;


	ClipCursor(&clip);
	// 마우스 커서를 화면 중앙으로 재설정
	//SetCursorPos(centerX, centerY);
}

void CP_MouseCtrlScript::CalcMouseAxisInput()
{
	// 현재 마우스 위치 얻기
	POINT mousePos;
	GetCursorPos(&mousePos);

	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();

	// 화면 중앙 좌표 계산
	float centerX = screenResoulution.x / 2;
	float centerY = screenResoulution.y / 2;

	// 마우스의 상대적 이동량 계산(Caluclate the relative movement of the mouse)
	float deltaX = mousePos.x - centerX;
	float deltaY = mousePos.y - centerY;
	m_PHQ->m_MouseAxisInput = Vec2(deltaX, deltaY);
}

void CP_MouseCtrlScript::MoveCamera()
{
	Vec3 objPos = m_PHQ->Transform()->GetRelativePos();
	Vec3 camF = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 camR = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 camU = m_ctrlCam->Transform()->GetRelativeDir(DIR_TYPE::UP);

	// y의 위치는 State, stance 마다 다를 수 있음
	objPos.y += m_YPivot;


	// x // 앞뒤로 // 위 아래  // z = 좌우로
	Vec3 Point = objPos + camF * m_vCamOffset.x + camR * m_vCamOffset.z + camU * m_vCamOffset.y;
	//Vec3 Point = objPos + camF + camR + camU;
	m_ctrlCam->Transform()->SetRelativePos(Point);


	CP_StatesScript* curState = dynamic_cast<CP_StatesScript*>(m_PHQ->GetCurState());
	eP_States stateType = static_cast<eP_States>(curState->GetStateType());
	CP_FSMScript::ePlayerStance curStance = m_PHQ->GetStance();

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
	Vec2 mouseInput = m_PHQ->m_MouseAxisInput;
	float deltaYaw = XMConvertToRadians(mouseInput.x * m_MouseSensitivity);
	float deltaPitch = XMConvertToRadians(mouseInput.y * m_MouseSensitivity); // Y축 반전 처리
	float xRot, yRot;

	if (justRotCam)
	{
		xRot = getCamRot.x + deltaPitch;
		yRot = getCamRot.y + deltaYaw;
		Vec3 outCamEuler = Vec3(xRot, yRot, (int)0);
		m_ctrlCam->Transform()->SetRelativeRot(outCamEuler);
	}
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
