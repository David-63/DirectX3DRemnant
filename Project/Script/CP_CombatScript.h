#pragma once
#include <Engine/CScript.h>
#include <Engine/CGameObject.h>

class CP_FSMScript;
class CP_CombatScript : public CScript
{
private:
	CP_FSMScript* m_PHQ;

private:
	CGameObject* m_Weapon;
	CGameObject* m_LongGun;


public:
	virtual void begin() override;
	virtual void tick() override;


private:
	void SetOwner(CP_FSMScript* _owner) { m_PHQ = _owner; }
	void SetWeaponObj(CGameObject* _obj) { m_Weapon = _obj; }

public:
	CLONE(CP_CombatScript);

public:
	CP_CombatScript();
	virtual ~CP_CombatScript();
};

