#pragma once
#include <Engine/CScript.h>
#include <Engine/CGameObject.h>

#define PIVOT_HIGH 150
#define PIVOT_MIDDLE 120
#define PIVOT_LOW 95

#define FOV_HIGH XM_PI / 2.25f
#define FOV_LOW XM_PI / 4.5f


class CP_FSMScript;
class CP_MouseCtrlScript : public CScript
{
private:
	CP_FSMScript*	m_PHQ;
	CCamera*		m_ctrlCam;
	Vec3			m_vCamOffset;		// »ó´ë ÁÂÇ¥
	float			m_MouseSensitivity;
	float			m_PrevCamRotY;

private:
	bool			m_IsChangeStance;
	
	float			m_pivotValue;
	tTimeCtrl		m_PivotBlend;
	float			m_curPivot;
	float			m_pivotRatio;

	float			m_fovValue;
	tTimeCtrl		m_FovBlend;
	float			m_curFov;
	float			m_fovRatio;


public:
	virtual void begin() override;
	virtual void tick() override;


public:
	void MoveCameraPos();
	void MoveCameraRot();
	void MouseRock();
	void OverrideObjRotY();

public:
	void SetOwner(CP_FSMScript* _owner) { m_PHQ = _owner; }
	void SetMainCam(CCamera* _cam) { m_ctrlCam = _cam; }
	void ChangeCamValue() { m_IsChangeStance = true; }

	void SetPivot(float _value)
	{
		m_pivotValue = _value;
		m_PivotBlend.Activate();
	}
	void SetFov(float _value)
	{
		m_fovValue = _value;
		m_FovBlend.Activate();
	}

public:
	CLONE(CP_MouseCtrlScript);

public:
	CP_MouseCtrlScript();
	virtual ~CP_MouseCtrlScript();
};

