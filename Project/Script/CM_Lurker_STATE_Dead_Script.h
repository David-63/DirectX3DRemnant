#pragma once
#include "CM_Lurker_StatesScript.h"

//#define Lurker_ALERT01         L"animclip\\Lurker\\Lurker_Alert_01.animclip"


class CM_Lurker_STATE_Dead_Script : public CM_Lurker_StatesScript
{

private:


public:
	virtual void tick() override;
	virtual void begin() override;

public:


private:


public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Lurker_STATE_Dead_Script);

	CM_Lurker_STATE_Dead_Script();
	virtual ~CM_Lurker_STATE_Dead_Script();

};

