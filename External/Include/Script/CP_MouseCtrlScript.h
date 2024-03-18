#pragma once
#include <Engine/CScript.h>
#include <Engine/CGameObject.h>

#define PIVOT_HIGH 150
#define PIVOT_MIDDLE 120
#define PIVOT_LOW 95

#define FOV_HIGH XM_PI / 2.25f
#define FOV_LOW XM_PI / 5.75f



class CP_FSMScript;
class CP_MouseCtrlScript : public CScript
{
private:
	struct tCameraInfo
	{
		Vec3			CamOffset;
		float			MouseSensitivity;
		float			PrevCamRotY;

		tCameraInfo(Vec3 _offset, float _sensitive) : CamOffset(_offset), MouseSensitivity(_sensitive), PrevCamRotY(0) {}
	};
	struct tSwapValue
	{
		tTimeCtrl		BlendTime;
		float			TargetValue;
		float			CurValue;
		float			Ratio;

		void SetValue(float _value)
		{
			TargetValue = _value;
			BlendTime.Activate();
		}
		float GetRatio()
		{
			return Ratio = BlendTime.curTime / BlendTime.maxTime;
		}
		void Reset()
		{
			BlendTime.ResetTime();
			Ratio = 0.f;
		}

		tSwapValue() : BlendTime(0.5f), TargetValue(0.f), CurValue(0.f), Ratio(0.f) {}
		tSwapValue(float _curValue) : BlendTime(0.5f), TargetValue(0.f), CurValue(_curValue), Ratio(0.f) {}
	};
private:
	CP_FSMScript*	m_PHQ;
	CCamera*		m_ctrlCam;
	tCameraInfo		m_CamInfo;
	bool			m_IsChangeStance;
	tSwapValue		m_PivotValue;
	tSwapValue		m_FovValue;

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

	void SetPivot(float _value) { m_PivotValue.SetValue(_value); }
	void SetFov(float _value) { m_FovValue.SetValue(_value); }

public:
	CLONE(CP_MouseCtrlScript);

public:
	CP_MouseCtrlScript();
	virtual ~CP_MouseCtrlScript();
};

