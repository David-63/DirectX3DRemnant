#pragma once
#include "CM_Spider_StatesScript.h"


class CM_Spider_STATE_Damaged_Script : public CM_Spider_StatesScript
{
	enum class eADPart
	{
		NormalF,
		HeavyF,
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
	void SetPart(UINT _part) { m_ePart = (eADPart)_part; }

private:
	void AniComplete();
	void SetStop();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Spider_STATE_Damaged_Script);

	CM_Spider_STATE_Damaged_Script();
	virtual ~CM_Spider_STATE_Damaged_Script();

};

