#pragma once
#include <Engine/CScript.h>
#include <Engine/CGameObject.h>

#define PIVOT_HIGH 70 // 150
#define PIVOT_LOW 25 // 95

#define FOV_HIGH XM_PI / 2.25f
#define FOV_LOW XM_PI / 5.75f



class CP_FSMScript;
class CP_MouseCtrlScript : public CScript
{
public:
	enum class eRotMat
	{
		Normal,
		MoveLeft,
		MoveRight,
		
		Crouch,
		CrouchLeft,
		CrouchRight,

		Aim,
		End,
	};
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
	ePlayerStance*	m_PlayerStance;
	CCamera*		m_ctrlCam;
	tCameraInfo		m_CamInfo;
	bool			m_IsChangeStance;
	tSwapValue		m_PivotValue;
	tSwapValue		m_FovValue;

private:
	CGameObject*	m_Weapon;
	CGameObject*	m_LongGun;
		
	Matrix			m_RotMat[(UINT)eRotMat::End];	// 리빙포인트 : 회전은 행렬곱의 순서와 상관이 없음
	Matrix			m_CurRotMat;

public:
	virtual void begin() override;
	virtual void tick() override;


private:
	void changeFov();
	void changeTransfrom();
	void mouseRock();
	void updateWeaponMatrix();

public:
	void OverrideObjRotY();

public:
	void SetOwner(CP_FSMScript* _owner) { m_PHQ = _owner; }
	void SetMainCam(CCamera* _cam) { m_ctrlCam = _cam; }
	void SetWeaponObj(CGameObject* _obj) { m_Weapon = _obj; }
	void ChangeWeaponMatrix(UINT _rotMat) { m_CurRotMat = m_RotMat[_rotMat]; }

	void SetPivot(float _value) { m_PivotValue.SetValue(_value); }
	void SetFov(float _value) { m_FovValue.SetValue(_value); }

public:
	CLONE(CP_MouseCtrlScript);

public:
	CP_MouseCtrlScript();
	virtual ~CP_MouseCtrlScript();
};

