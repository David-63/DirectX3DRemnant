#pragma once
#include "CM_Annointed_StatesScript.h"


class CM_Annointed_STATE_Damaged_Script : public CM_Annointed_StatesScript
{
	enum class eADPart
	{
		Legs,
		LegL,
		LegR,
		Torso,
		Heavy,
	};

private:
	eADPart m_ePart;
	bool m_bAniComplete;
	float m_fBackSpeed;
	Vec3 m_vBackDir;

public:
	virtual void tick() override;
	virtual void begin() override;

public:
	void SetPart(eADPart _part) { m_ePart = _part; }

private:
	void AniComplete();
	void SetStop();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Annointed_STATE_Damaged_Script);

	CM_Annointed_STATE_Damaged_Script();
	virtual ~CM_Annointed_STATE_Damaged_Script();

};

