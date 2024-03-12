#pragma once
#include "CM_Lurker_StatesScript.h"

class CM_Lurker_STATE_Idle_Script : public CM_Lurker_StatesScript
{
private:


public:
	virtual void tick() override;


public:
	void ClearIdleState() {}


public:
	virtual void Enter() override;
	virtual void Exit() override;
	
public:
	CLONE(CM_Lurker_STATE_Idle_Script);

	CM_Lurker_STATE_Idle_Script();
	virtual ~CM_Lurker_STATE_Idle_Script();

};

