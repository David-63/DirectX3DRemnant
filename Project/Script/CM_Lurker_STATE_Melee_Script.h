#pragma once
#include "CM_Lurker_StatesScript.h"

class CM_Lurker_STATE_Melee_Script : public CM_Lurker_StatesScript
{
private:
	//common
	Vec3			 m_vAtkDir;
	bool		     m_bAtkComplete;
	float			 m_fSpeed;
	bool			 m_bNearPlayer;
	eMeleeState      m_eMeleeState;

	//dash
	bool			 m_bDashOnce;
	float			 m_fDashDist;

	//Heavy
	bool			 m_bHeavyOnce;

	//Slash
	bool			 m_bSlashOnce;
	
	Ptr<CSound>      m_Sound;

public:
	virtual void tick() override;
	virtual void begin() override;

public:
	void SetMeleeState(eMeleeState _state) { m_eMeleeState = _state; }

private:
	void Dash();
	void AtkComplete();
	void AdjustDashSpeed();
	void AdjustZeroSpeed();

	void Heavy1();

	void Slash();
	void SlashCombo();

	void DashHitBoxOn();
	void HeavyHitBoxOn();
	void SlashHitBoxOn();
	void SlashHitBoxOff();
	void SlashComboBoxOn();
	void SlashComboBoxOff();

	void DistCheck();
	

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Lurker_STATE_Melee_Script);

	CM_Lurker_STATE_Melee_Script();
	virtual ~CM_Lurker_STATE_Melee_Script();

};

