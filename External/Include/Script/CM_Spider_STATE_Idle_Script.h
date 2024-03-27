#pragma once
#include "CM_Spider_StatesScript.h"

class CM_Spider_STATE_Idle_Script : public CM_Spider_StatesScript
{
private:
	bool m_bAlert;
	bool m_bAlertOnce;

	bool mKey;

public:
	virtual void tick() override;
	virtual void begin() override;

public:
	void ClearIdleState() {}

private:
	void EmergePose();
	void ChangeStateChase();


public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Spider_STATE_Idle_Script);

	CM_Spider_STATE_Idle_Script();
	virtual ~CM_Spider_STATE_Idle_Script();

};

