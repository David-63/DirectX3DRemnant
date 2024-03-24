#pragma once
#include "CM_Annointed_StatesScript.h"

class CM_Annointed_STATE_Atk_Script : public CM_Annointed_StatesScript
{
private:
	//common
	Vec3			 m_vAtkDir;
	bool		     m_bAtkComplete;
	float			 m_fSpeed;
	bool			 m_bNearPlayer;
	eAtkState        m_eAtkState;

	//dash
	bool			 m_bDashOnce;
	float			 m_fDashDist;

	//Heavy
	bool			 m_bHeavyOnce;

	//Slash
	bool			 m_bSlashOnce;


public:
	virtual void tick() override;
	virtual void begin() override;

public:
	void SetAtkState(eAtkState _state) { m_eAtkState = _state; }

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
	CLONE(CM_Annointed_STATE_Atk_Script);

	CM_Annointed_STATE_Atk_Script();
	virtual ~CM_Annointed_STATE_Atk_Script();

};

