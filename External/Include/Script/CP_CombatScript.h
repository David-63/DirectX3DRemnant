#pragma once
#include <Engine/CScript.h>
#include <Engine/CGameObject.h>

class CP_FSMScript;
class CP_CombatScript : public CScript
{
private:
	CP_FSMScript*	m_PHQ;
	tTimeCtrl		m_lightLife;
	//tP_LongGunInfo* m_Gun;

public:
	virtual void begin() override;
	virtual void tick() override;


public:
	void SetOwner(CP_FSMScript* _owner) { m_PHQ = _owner; }

public:
	CLONE(CP_CombatScript);

public:
	CP_CombatScript();
	virtual ~CP_CombatScript();
};

