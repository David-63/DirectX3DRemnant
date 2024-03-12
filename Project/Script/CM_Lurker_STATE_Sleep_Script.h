#pragma once
#include "CM_Lurker_StatesScript.h"

class CM_Lurker_STATE_Sleep_Script : public CM_Lurker_StatesScript
{
private:


public:
	virtual void tick() override;


public:
	


public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Lurker_STATE_Sleep_Script);

	CM_Lurker_STATE_Sleep_Script();
	virtual ~CM_Lurker_STATE_Sleep_Script();
};

