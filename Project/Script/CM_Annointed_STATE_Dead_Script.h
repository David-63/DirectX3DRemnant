#pragma once
#include "CM_Annointed_StatesScript.h"


class CM_Annointed_STATE_Dead_Script : public CM_Annointed_StatesScript
{

private:
	bool m_bdeath;
	bool m_bAfterDeath;
	float m_fTime;


public:
	virtual void tick() override;
	virtual void begin() override;

public:


private:
	void EraseThis();
	void AniStop();
	void PlayDeathGround();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Annointed_STATE_Dead_Script);

	CM_Annointed_STATE_Dead_Script();
	virtual ~CM_Annointed_STATE_Dead_Script();

};

