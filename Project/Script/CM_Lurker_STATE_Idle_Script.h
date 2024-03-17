#pragma once
#include "CM_Lurker_StatesScript.h"

class CM_Lurker_STATE_Idle_Script : public CM_Lurker_StatesScript
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
	CLONE(CM_Lurker_STATE_Idle_Script);

	CM_Lurker_STATE_Idle_Script();
	virtual ~CM_Lurker_STATE_Idle_Script();

};

