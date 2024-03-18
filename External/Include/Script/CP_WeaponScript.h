#pragma once
#include <Engine/CScript.h>
#include <Engine/CGameObject.h>

class CP_FSMScript;
class CP_WeaponScript : public CScript
{
private:
	CP_FSMScript* m_PHQ;
	CAnimator3D* m_OwnerAnimator;
	CTransform* m_OwnerTransform;
	tMTBone m_HandBoneData;
	Matrix m_HandMatrix;
	
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetOwner(CP_FSMScript* _owner) { m_PHQ = _owner; }

public:
	CLONE(CP_WeaponScript);

public:
	CP_WeaponScript();
	virtual ~CP_WeaponScript();
};

