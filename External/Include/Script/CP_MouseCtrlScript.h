#pragma once
#include <Engine/CScript.h>
#include <Engine/CGameObject.h>

class CP_FSMScript;
class CP_MouseCtrlScript : public CScript
{
private:
	CP_FSMScript*	m_PHQ;
	CCamera*		m_ctrlCam;
	Vec3			m_vCamPivot;		// ī�޶� �߽� ��ǥ
	Vec3			m_vCamOffset;		// ��� ��ǥ
	float			m_MouseSensitivity;
public:
	virtual void tick() override;


public:
	void MouseRock();
	void CalcMouseAxisInput();
	void MoveCamera();

public:
	void SetOwner(CP_FSMScript* _owner) { m_PHQ = _owner; }
	void SetMainCam(CCamera* _cam) { m_ctrlCam = _cam; }

public:
	CLONE(CP_MouseCtrlScript);

public:
	CP_MouseCtrlScript();
	virtual ~CP_MouseCtrlScript();
};

