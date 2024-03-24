#pragma once
#include "CC_FSMScript.h"
#include "CM_Annointed_StatesScript.h"

class CM_Annointed_FSMScript : public CC_FSMScript
{
public:
	enum class eAnnointedState
	{
		Idle,
		Melee,
		Move,
		Damaged,
		Dead,

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

private:
	void DeathCheck();

public:
	CLONE(CM_Annointed_FSMScript);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CM_Annointed_FSMScript();
	virtual ~CM_Annointed_FSMScript();

};


//Idle
//#define Lurker_ALERT01         L"animclip\\Lurker\\Lurker_Alert_01.animclip"


//Melee


//Move

//Death

//attacked impact

//Turn
