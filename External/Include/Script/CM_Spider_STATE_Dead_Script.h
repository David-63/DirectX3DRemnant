#pragma once
#include "CM_Spider_StatesScript.h"


class CM_Spider_STATE_Dead_Script : public CM_Spider_StatesScript
{

private:
	bool m_bdeath;
	float m_fTime;


public:
	virtual void tick() override;
	virtual void begin() override;

public:


private:
	void EraseThis();
	void AniStop();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Spider_STATE_Dead_Script);

	CM_Spider_STATE_Dead_Script();
	virtual ~CM_Spider_STATE_Dead_Script();

};

