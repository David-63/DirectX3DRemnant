#pragma once
#include "CM_Annointed_StatesScript.h"

class CM_Annointed_STATE_Idle_Script : public CM_Annointed_StatesScript
{
	enum class eInitialPose
	{
		Up,
		Down,
		DownDoo,
		Wall,
		Wall2,
		Closet,
	};
private:
	bool m_bAlert;
	bool m_bAlertOnce;
	eInitialPose m_pose;
	int m_iEmergeStage;

	bool mKey;

public:
	virtual void tick() override;
	virtual void begin() override;

public:
	void ClearIdleState() {}
	void SetInitialPose(UINT _pose);

private:
	void InitialPose();
	void EmergePose();
	void ChangeStateChase();


public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Annointed_STATE_Idle_Script);

	CM_Annointed_STATE_Idle_Script();
	virtual ~CM_Annointed_STATE_Idle_Script();

};

