#pragma once
#include "CM_Spider_StatesScript.h"

class CM_Spider_STATE_Atk_Script : public CM_Spider_StatesScript
{
private:
	//common
	Vec3			 m_vAtkDir;
	bool		     m_bAtkComplete;
	float			 m_fSpeed;
	bool			 m_bNearPlayer;
	eAtkState        m_eAtkState;

	//Shoot
	bool			 m_bShootOnce;
	CGameObject*     m_CProj;
	bool             m_bLiftSet;
	float            m_fElapsedTime;
	float            m_fFinalTime;
	bool             m_bShootLift;

	//Push
	bool			 m_bPushOnce;



public:
	virtual void tick() override;
	virtual void begin() override;

public:
	void SetAtkState(eAtkState _state) { m_eAtkState = _state; }

private:
	void AtkComplete();
	void AdjustDashSpeed();
	void AdjustZeroSpeed();
	void Clear();

	void Push();

	void Shoot();
	void ShootComplete();
	void CreateProj();
	void LiftProj();
	void LiftStart() { m_bShootLift = true; }
	

	void PushHitBoxOn();

	void DistCheck();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Spider_STATE_Atk_Script);

	CM_Spider_STATE_Atk_Script();
	virtual ~CM_Spider_STATE_Atk_Script();

};

