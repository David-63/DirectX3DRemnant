#pragma once
#include "CC_FSMScript.h"
#include "CM_Lurker_StatesScript.h"

class CM_Lurker_FSMScript : public CC_FSMScript
{
public:
	enum class eLurkerState
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

public:
	CLONE(CM_Lurker_FSMScript);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CM_Lurker_FSMScript();
	virtual ~CM_Lurker_FSMScript();

};


//Idle
#define Lurker_ALERT01         L"animclip\\Lurker\\Lurker_Alert_01.animclip"
#define Lurker_ALERT03         L"animclip\\Lurker\\Wasteland_Lurker_Alert_03.animclip"
#define Lurker_IDLESTAND       L"animclip\\Lurker\\Wasteland_Lurker_Idle.animclip"
#define Lurker_IDLEDOWN        L"animclip\\Lurker\\Wasteland_Lurker_P_Idle.animclip"
#define Lurker_IDLEDOWNDOO     L"animclip\\Lurker\\Wasteland_Lurker_P_Idle_Fid.animclip"
#define Lurker_WALL_EMERGE     L"animclip\\Lurker\\Wasteland_Lurker_Emerge_Wall_01.animclip"
#define Lurker_WALL_EMERGE2    L"animclip\\Lurker\\Wasteland_Lurker_Emerge_Wall_01_Alt.animclip"
#define Lurker_EMERGE          L"animclip\\Lurker\\Wasteland_Lurker_Emerge_Fabricator_01.animclip"

//Melee
#define Lurker_Dash            L"animclip\\Lurker\\Wasteland_Lurker_Atk_Dash.animclip"
#define Lurker_Heavy1          L"animclip\\Lurker\\Wasteland_Lurker_Atk_Heavy_01.animclip"
#define Lurker_SlashR          L"animclip\\Lurker\\Wasteland_Lurker_Atk_Slash_R.animclip"
#define Lurker_SlashRCombo	   L"animclip\\Lurker\\Wasteland_Lurker_Atk_Slash_R_Combo.animclip"
