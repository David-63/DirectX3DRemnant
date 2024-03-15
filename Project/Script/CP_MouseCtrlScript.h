#pragma once
#include <Engine/CScript.h>
#include <Engine/CGameObject.h>

class CP_FSMScript;
class CP_MouseCtrlScript : public CScript
{
private:
	CP_FSMScript*	m_PHQ;
	CCamera*		m_ctrlCam;
	Vec3			m_vCamOffset;		// 상대 좌표
	float			m_YPivot;			// 카메라 중심 좌표
	float			m_MouseSensitivity;


private:
	float			m_prevX;
	float			m_prevY;


public:
	virtual void tick() override;


public:
	void MouseRock();
	void CalcMouseAxisInput();
	void MoveCamera();

public:
	void SetOwner(CP_FSMScript* _owner) { m_PHQ = _owner; }
	void SetMainCam(CCamera* _cam) { m_ctrlCam = _cam; }
	void SetYPivot(float _pivot) { m_YPivot = _pivot; }

public:
	CLONE(CP_MouseCtrlScript);

public:
	CP_MouseCtrlScript();
	virtual ~CP_MouseCtrlScript();
};

