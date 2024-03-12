#pragma once
#include "CC_FSMScript.h"
#include "CP_StatesScript.h"


class CM_Lurker_FSMScript : public CC_FSMScript
{
public:
	enum class eLurkerState
	{
	   Sleep,
	   Alert,
	   Idle,

	};

	struct tMonsterStat
	{
		float MoveSpeed;
		float AlertDist;

	};

	struct tM_Info
	{
		tMonsterStat     M_Stat;
		tHealthStat      M_Health;
	};

private:
	tM_Info      m_tMonsterInfo;


public:
	virtual void begin() override;
	virtual void tick() override;

public:
	virtual void BeginOverlap(CCollider3D* _Other) override;
	virtual void OnOverlap(CCollider3D* _Other) override;
	virtual void EndOverlap(CCollider3D* _Other) override;

public:
	CLONE(CM_Lurker_FSMScript);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CM_Lurker_FSMScript();
	virtual ~CM_Lurker_FSMScript();

};

