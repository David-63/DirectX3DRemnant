#include "pch.h"
#include "CShoulderViewScript.h"
#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>

CShoulderViewScript::CShoulderViewScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT)
	, m_Target(nullptr)
	, m_fMouseSensitivity(0.005f)
	, m_vPitchMinMax(Vec2(-20.f, 40.f)) // y값 원래는 2 
	, m_vCamPivot{ -51.f, 37.f, 18.f }
	, m_firstTick(true)
	, m_bIsMouseRock(true)
	, m_eViewState(VIEW_STATE::NORMAL)
	, m_OriginFov{}
	, m_ModifiedFov{}
	, m_CurrentFov(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_vCamPivot.x, "Pivot_Y");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_vCamPivot.y, "Pivot_Z");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_vCamPivot.z, "Pivot_X");

	AddScriptParam(SCRIPT_PARAM::VEC2, &m_vPitchMinMax, "PitchMinMax");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMouseSensitivity, "MouseSesitivity");

}

CShoulderViewScript::~CShoulderViewScript()
{
}

void CShoulderViewScript::begin()
{
	m_OriginFov = Camera()->GetFov();


	// 윈도우의 중심 좌표를 계산
	POINT ptMousePos;
	HWND hWnd = CEngine::GetInst()->GetMainWnd();
	GetCursorPos(&ptMousePos); // 마우스의 현재 스크린 좌표를 얻습니다.
	ScreenToClient(hWnd, &ptMousePos); // 스크린 좌표를 클라이언트 영역의 좌표로 변환합니다.

	// 클라이언트 영역의 중앙 좌표를 계산합니다.
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	int centerX = (clientRect.right - clientRect.left) / 2;
	int centerY = (clientRect.bottom - clientRect.top) / 2;


	SetCursorPos(centerX, centerY);
	//CKeyMgr::GetInst()->SetMousePos(Vec2(centerX, centerY)); // 이걸로 하면 마우스 고정 안됨


	// 2번 방식 쓸 때는 아래 각도
	float BeginRot = XMConvertToRadians(90.f);
	Transform()->SetRelativeRot(Vec3(0.f, BeginRot, 0.f));




}

void CShoulderViewScript::tick()
{

	////// ================= 
	//// 1. 마우스 중앙으로 이동시키는 코드
	////// 근데 다른 작업 못하므로 2번 쓰다가 1번으로 바꾸기
	////// ================= 
	//
	Input();

	Vector3 vRot = Vec3(Transform()->GetRelativeRot());

	if (m_firstTick)
	{

		Transform()->SetRelativeRot(vRot);
		m_firstTick = false;
	}

	else
	{
		if (m_bIsMouseRock)
		{

			CameraPos();
			CameraRot();
			CharacterRot();

			ChangeStateFov();
			MouseCenterLock();

		}


	}

	//// ==================	
	// 이 아래부터는 그냥 쓸 것
	// =======================

	// ================





}



tInt2 CShoulderViewScript::CalcMouseMovement()
{
	// 현재 마우스 위치 얻기
	POINT mousePos;
	GetCursorPos(&mousePos);

	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();

	// 화면 중앙 좌표 계산
	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	// 마우스의 상대적 이동량 계산(Caluclate the relative movement of the mouse)
	int deltaX = mousePos.x - centerX;
	int deltaY = mousePos.y - centerY;

	return tInt2{ deltaX, deltaY };

}

void CShoulderViewScript::SmoothCamRot(Vec3 _vCamRot)
{
	// 오일러 각도를 쿼터니언으로 변환하여 부드러운 회전 적용 
	Vec3 vRelativeRot = Transform()->GetRelativeRot();

	//Quaternion currentQuat = Transform()->EulerToQuaternion(vRelativeRot);
	//Quaternion targetQuat = Transform()->EulerToQuaternion(_vCamRot);


	// Slerp를 사용하여 회전 보간
	float slerpFactor = DT * m_fMouseSensitivity;
	//	Quaternion smoothQuat = Quaternion::Slerp(currentQuat, targetQuat, slerpFactor);

		// 부드럽게 보간된 쿼터니언을 다시 오일러 각도로 변환
	Vec3 smoothEuler{ 0.f, 0.f, 0.f };


	//	Transform()->QuaternionToEuler(smoothQuat, smoothEuler);

		//m_vMouseCamRot = smoothEuler;


}

void CShoulderViewScript::MouseCenterLock()
{
	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();

	// 화면 중앙 좌표 계산
	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	// 마우스 커서를 화면 중앙으로 재설정
	SetCursorPos(centerX, centerY);
}

void CShoulderViewScript::CameraRot()
{

	// 상대적 이동량을 사용하여 게임 내에서 필요한 처리 수행
	// 예: 카메라 회전, 캐릭터 이동 등


	// 회전각 적용한 내 코드
	Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();

	tInt2 tMouseMovement = CalcMouseMovement();

	Vector3 vRot = Vec3(Transform()->GetRelativeRot());
	vRot.y += tMouseMovement.x * m_fMouseSensitivity; // 유니티기준 yaw
	//vRot.x += tMouseMovement.y * m_fMouseSensitivity;

	float RadianPitchLimitX = XMConvertToRadians(m_vPitchMinMax.x);
	float RadianPitchLimitY = XMConvertToRadians(m_vPitchMinMax.y);
	vRot.x = max(RadianPitchLimitX, min(vRot.x, RadianPitchLimitY));

	Transform()->SetRelativeRot(vRot);




	//// ======== 테스트 중 
/* 에임 상태일 때 카메라의 이동에 따라 캐릭터 회전
	// 마우스 움직임을 회전 각도로 변환
	float yawChange = deltaX * m_fMouseSensitivity;
	float pitchChange = -deltaY * m_fMouseSensitivity; // DirectX는 Y축이 반대이므로 -를 사용


	// 현재 캐릭터의 회전 값 얻기
	Vec3 currentRotation = m_Target->Transform()->GetRelativeRot();

	// 수평 회전(Yaw) 갱신
	Vec3 newRotation = currentRotation;

	newRotation.y += yawChange; // 좌우로 캐릭터 움직임
	newRotation.z += pitchChange; // 상하로 캐릭터 움직임(각도 제한 걸어야할듯)
	newRotation.x = 0;

	// 캐릭터의 회전 갱신
	m_Target->Transform()->SetRelativeRot(newRotation); */




}

void CShoulderViewScript::CameraPos()
{
	Vec3 vPlayerPos = m_Target->Transform()->GetRelativePos();


	Vec3 CameraFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 CameraRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 CameraUp = Transform()->GetRelativeDir(DIR_TYPE::UP);

	//	vPlayerFront (회전되면서 변한 피봇 값을 각 3방향에 적용시켜야함)	
	// x // 앞뒤로 // 위 아래  // z = 좌우로
	Vec3 Point = vPlayerPos + CameraFront * m_vCamPivot.x + CameraRight * m_vCamPivot.z + CameraUp * m_vCamPivot.y;


	// 카메라의 위치 설정
	Transform()->SetRelativePos(Point);
}

void CShoulderViewScript::CharacterRot()
{
	/* Vec3 PlayerRot = Vec3(m_Target->Transform()->GetRelativeRot().x, Transform()->GetRelativeRot().y, m_Target->Transform()->GetRelativeRot().z);
	 m_Target->Transform()->SetRelativeRot(PlayerRot);*/


	 //Vector2 MousePos = CKeyMgr::GetInst()->GetMousePos();

	 //tInt2 tMouseMovement = CalcMouseMovement();
	 //Vector3 worldAimTarget = Vector3(tMouseMovement.x, tMouseMovement.y, 1.f);

	 //Vector3 PlayerPos = m_Target->Transform()->GetRelativePos();

	 //worldAimTarget.y = PlayerPos.y;
	 //Vector3 aimDirection = (worldAimTarget - PlayerPos).Normalize();

	 //Vector3 PlayerFront = m_Target->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	 //PlayerFront = XMVectorLerp(PlayerFront, aimDirection, 200.f);
	 //m_Target->Transform()->SetRelativeRot(PlayerFront); 


	 //  // 현재 캐릭터의 회전 값을 가져옵니다.
	 //Vec3 PlayerRot = m_Target->Transform()->GetRelativeRot();

	 //// 카메라의 회전 값을 캐릭터의 회전 값으로 업데이트합니다.
	 //PlayerRot.y = Transform()->GetRelativeRot().y;

	 //// 캐릭터의 회전 값을 설정합니다.
	 //m_Target->Transform()->SetRelativeRot(PlayerRot);
}



void CShoulderViewScript::Input()
{
	// 인벤토리를 열 때 카메라가 9번에 해당하는 걸로 바뀌도록 나중에 2차 가공하면 될듯.
	if (KEY_TAP(KEY::_8))
	{
		m_bIsMouseRock = true;
	}

	if (KEY_TAP(KEY::_9))
	{
		m_bIsMouseRock = false;
	}

	// == 카메라 전환
	if (KEY_PRESSED(KEY::RBTN))
	{
		m_eViewState = VIEW_STATE::AIMING;

		m_ModifiedFov = XM_PIDIV4; // 45도로 설정
	}

	if (KEY_RELEASE(KEY::RBTN))
	{
		m_eViewState = VIEW_STATE::NORMAL;
	}

}

void CShoulderViewScript::ChangeStateFov()
{

	// Aiming 상태냐 아니냐에 따른 fov 값 조절
	float targetFov = (m_eViewState == VIEW_STATE::AIMING) ? m_ModifiedFov : m_OriginFov;

	// 
	float Transition = 1;

	// 현재 FOV를 목표 FOV로 점진적으로 변경
	m_CurrentFov = Lerp(m_CurrentFov, targetFov, DT * Transition);

	// FOV 업데이트
	Camera()->SetFov(m_CurrentFov);


}

float CShoulderViewScript::Lerp(float _Start, float _End, float _Transition)
{

	return _Start + (_End - _Start) * _Transition;
}



void CShoulderViewScript::SaveToLevelFile(FILE* _File)
{
	// m_vCamOffset , m_fMouseSensitivity, m_vPitchMinMax , m_bIsMouseRock
	// 카메라의 시야각 정보, m_OriginFov, ModifiedFov

}

void CShoulderViewScript::LoadFromLevelFile(FILE* _FILE)
{
}
