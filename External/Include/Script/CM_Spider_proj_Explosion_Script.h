#pragma once
#include <Engine\CScript.h>

class CM_Spider_proj_Explosion_Script : public CScript
{
private:
	float m_deathTime;

public:
	CM_Spider_proj_Explosion_Script();
	~CM_Spider_proj_Explosion_Script();
	CLONE(CM_Spider_proj_Explosion_Script);

public:
	virtual void begin() override;
	virtual void tick() override;

public:

public:
	
};
