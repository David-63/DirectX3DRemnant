#pragma once
#include "CM_Lurker_StatesScript.h"


class CM_Lurker_STATE_Damaged_Script : public CM_Lurker_StatesScript
{
	enum class eLDPart
	{
		Legs,
		LegL,
		LegR,
		Torso,
		Heavy,
	};

private:
	eLDPart m_ePart;
	bool m_bAniComplete;
	float m_fBackSpeed;
	Vec3 m_vBackDir;

public:
	virtual void tick() override;
	virtual void begin() override;

public:
	void SetPart(UINT _part) { m_ePart = (eLDPart)_part; }

private:
	void AniComplete();
	void SetStop();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Lurker_STATE_Damaged_Script);

	CM_Lurker_STATE_Damaged_Script();
	virtual ~CM_Lurker_STATE_Damaged_Script();

};

