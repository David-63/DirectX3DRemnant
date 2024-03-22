#pragma once
#include "CM_Lurker_StatesScript.h"

//#define Lurker_ALERT01         L"animclip\\Lurker\\Lurker_Alert_01.animclip"

enum class eLDPart
{
	Legs,
	LegL,
	LegR,
	Torso,
	Heavy,
};
class CM_Lurker_STATE_Damaged_Script : public CM_Lurker_StatesScript
{

private:
	eLDPart m_ePart;
	bool m_bAniComplete;
	float m_fBackSpeed;
	Vec3 m_vBackDir;

public:
	virtual void tick() override;
	virtual void begin() override;

public:
	void SetPart(eLDPart _part) { m_ePart = _part; }

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

