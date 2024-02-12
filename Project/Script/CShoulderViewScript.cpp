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
	, m_vPitchMinMax(Vec2(-20.f, 40.f)) // y�� ������ 2 
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


	// �������� �߽� ��ǥ�� ���
	POINT ptMousePos;
	HWND hWnd = CEngine::GetInst()->GetMainWnd();
	GetCursorPos(&ptMousePos); // ���콺�� ���� ��ũ�� ��ǥ�� ����ϴ�.
	ScreenToClient(hWnd, &ptMousePos); // ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ������ ��ǥ�� ��ȯ�մϴ�.

	// Ŭ���̾�Ʈ ������ �߾� ��ǥ�� ����մϴ�.
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	int centerX = (clientRect.right - clientRect.left) / 2;
	int centerY = (clientRect.bottom - clientRect.top) / 2;


	SetCursorPos(centerX, centerY);
	//CKeyMgr::GetInst()->SetMousePos(Vec2(centerX, centerY)); // �̰ɷ� �ϸ� ���콺 ���� �ȵ�


	// 2�� ��� �� ���� �Ʒ� ����
	float BeginRot = XMConvertToRadians(90.f);
	Transform()->SetRelativeRot(Vec3(0.f, BeginRot, 0.f));




}

void CShoulderViewScript::tick()
{

	////// ================= 
	//// 1. ���콺 �߾����� �̵���Ű�� �ڵ�
	////// �ٵ� �ٸ� �۾� ���ϹǷ� 2�� ���ٰ� 1������ �ٲٱ�
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
	// �� �Ʒ����ʹ� �׳� �� ��
	// =======================

	// ================





}



tInt2 CShoulderViewScript::CalcMouseMovement()
{
	// ���� ���콺 ��ġ ���
	POINT mousePos;
	GetCursorPos(&mousePos);

	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();

	// ȭ�� �߾� ��ǥ ���
	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	// ���콺�� ����� �̵��� ���(Caluclate the relative movement of the mouse)
	int deltaX = mousePos.x - centerX;
	int deltaY = mousePos.y - centerY;

	return tInt2{ deltaX, deltaY };

}

void CShoulderViewScript::SmoothCamRot(Vec3 _vCamRot)
{
	// ���Ϸ� ������ ���ʹϾ����� ��ȯ�Ͽ� �ε巯�� ȸ�� ���� 
	Vec3 vRelativeRot = Transform()->GetRelativeRot();

	//Quaternion currentQuat = Transform()->EulerToQuaternion(vRelativeRot);
	//Quaternion targetQuat = Transform()->EulerToQuaternion(_vCamRot);


	// Slerp�� ����Ͽ� ȸ�� ����
	float slerpFactor = DT * m_fMouseSensitivity;
	//	Quaternion smoothQuat = Quaternion::Slerp(currentQuat, targetQuat, slerpFactor);

		// �ε巴�� ������ ���ʹϾ��� �ٽ� ���Ϸ� ������ ��ȯ
	Vec3 smoothEuler{ 0.f, 0.f, 0.f };


	//	Transform()->QuaternionToEuler(smoothQuat, smoothEuler);

		//m_vMouseCamRot = smoothEuler;


}

void CShoulderViewScript::MouseCenterLock()
{
	Vec2 screenResoulution = CEngine::GetInst()->GetWindowResolution();

	// ȭ�� �߾� ��ǥ ���
	int centerX = screenResoulution.x / 2;
	int centerY = screenResoulution.y / 2;

	// ���콺 Ŀ���� ȭ�� �߾����� �缳��
	SetCursorPos(centerX, centerY);
}

void CShoulderViewScript::CameraRot()
{

	// ����� �̵����� ����Ͽ� ���� ������ �ʿ��� ó�� ����
	// ��: ī�޶� ȸ��, ĳ���� �̵� ��


	// ȸ���� ������ �� �ڵ�
	Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();

	tInt2 tMouseMovement = CalcMouseMovement();

	Vector3 vRot = Vec3(Transform()->GetRelativeRot());
	vRot.y += tMouseMovement.x * m_fMouseSensitivity; // ����Ƽ���� yaw
	//vRot.x += tMouseMovement.y * m_fMouseSensitivity;

	float RadianPitchLimitX = XMConvertToRadians(m_vPitchMinMax.x);
	float RadianPitchLimitY = XMConvertToRadians(m_vPitchMinMax.y);
	vRot.x = max(RadianPitchLimitX, min(vRot.x, RadianPitchLimitY));

	Transform()->SetRelativeRot(vRot);




	//// ======== �׽�Ʈ �� 
/* ���� ������ �� ī�޶��� �̵��� ���� ĳ���� ȸ��
	// ���콺 �������� ȸ�� ������ ��ȯ
	float yawChange = deltaX * m_fMouseSensitivity;
	float pitchChange = -deltaY * m_fMouseSensitivity; // DirectX�� Y���� �ݴ��̹Ƿ� -�� ���


	// ���� ĳ������ ȸ�� �� ���
	Vec3 currentRotation = m_Target->Transform()->GetRelativeRot();

	// ���� ȸ��(Yaw) ����
	Vec3 newRotation = currentRotation;

	newRotation.y += yawChange; // �¿�� ĳ���� ������
	newRotation.z += pitchChange; // ���Ϸ� ĳ���� ������(���� ���� �ɾ���ҵ�)
	newRotation.x = 0;

	// ĳ������ ȸ�� ����
	m_Target->Transform()->SetRelativeRot(newRotation); */




}

void CShoulderViewScript::CameraPos()
{
	Vec3 vPlayerPos = m_Target->Transform()->GetRelativePos();


	Vec3 CameraFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 CameraRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 CameraUp = Transform()->GetRelativeDir(DIR_TYPE::UP);

	//	vPlayerFront (ȸ���Ǹ鼭 ���� �Ǻ� ���� �� 3���⿡ ������Ѿ���)	
	// x // �յڷ� // �� �Ʒ�  // z = �¿��
	Vec3 Point = vPlayerPos + CameraFront * m_vCamPivot.x + CameraRight * m_vCamPivot.z + CameraUp * m_vCamPivot.y;


	// ī�޶��� ��ġ ����
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


	 //  // ���� ĳ������ ȸ�� ���� �����ɴϴ�.
	 //Vec3 PlayerRot = m_Target->Transform()->GetRelativeRot();

	 //// ī�޶��� ȸ�� ���� ĳ������ ȸ�� ������ ������Ʈ�մϴ�.
	 //PlayerRot.y = Transform()->GetRelativeRot().y;

	 //// ĳ������ ȸ�� ���� �����մϴ�.
	 //m_Target->Transform()->SetRelativeRot(PlayerRot);
}



void CShoulderViewScript::Input()
{
	// �κ��丮�� �� �� ī�޶� 9���� �ش��ϴ� �ɷ� �ٲ�� ���߿� 2�� �����ϸ� �ɵ�.
	if (KEY_TAP(KEY::_8))
	{
		m_bIsMouseRock = true;
	}

	if (KEY_TAP(KEY::_9))
	{
		m_bIsMouseRock = false;
	}

	// == ī�޶� ��ȯ
	if (KEY_PRESSED(KEY::RBTN))
	{
		m_eViewState = VIEW_STATE::AIMING;

		m_ModifiedFov = XM_PIDIV4; // 45���� ����
	}

	if (KEY_RELEASE(KEY::RBTN))
	{
		m_eViewState = VIEW_STATE::NORMAL;
	}

}

void CShoulderViewScript::ChangeStateFov()
{

	// Aiming ���³� �ƴϳĿ� ���� fov �� ����
	float targetFov = (m_eViewState == VIEW_STATE::AIMING) ? m_ModifiedFov : m_OriginFov;

	// 
	float Transition = 1;

	// ���� FOV�� ��ǥ FOV�� ���������� ����
	m_CurrentFov = Lerp(m_CurrentFov, targetFov, DT * Transition);

	// FOV ������Ʈ
	Camera()->SetFov(m_CurrentFov);


}

float CShoulderViewScript::Lerp(float _Start, float _End, float _Transition)
{

	return _Start + (_End - _Start) * _Transition;
}



void CShoulderViewScript::SaveToLevelFile(FILE* _File)
{
	// m_vCamOffset , m_fMouseSensitivity, m_vPitchMinMax , m_bIsMouseRock
	// ī�޶��� �þ߰� ����, m_OriginFov, ModifiedFov

}

void CShoulderViewScript::LoadFromLevelFile(FILE* _FILE)
{
}
