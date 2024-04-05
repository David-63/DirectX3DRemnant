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
	UINT         m_IdlePose;


public:
	virtual void begin() override;
	virtual void tick() override;

public:
	virtual void BeginOverlap(CCollider3D* _Other) override;
	virtual void OnOverlap(CCollider3D* _Other) override;
	virtual void EndOverlap(CCollider3D* _Other) override;

public:
	void SetIdlePose(UINT _pose) { m_IdlePose = _pose; }

private:
	void DeathCheck();

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

//Move
#define Lurker_MoveFront	   L"animclip\\Lurker\\Wasteland_Lurker_Walk_F.animclip"

//Death
#define Lurker_Death    	   L"animclip\\Lurker\\Wasteland_Lurker_Death_F_01.animclip"
#define Lurker_DeathGround     L"animclip\\Lurker\\Wasteland_Lurker_Death_Ground_B.animclip"

//attacked impact
#define Lurker_ImpactLegs      L"animclip\\Lurker\\Wasteland_Lurker_Impact_Heavy_Legs.animclip"
#define Lurker_ImpactLegL      L"animclip\\Lurker\\Wasteland_Lurker_Impact_LegL_01.animclip"
#define Lurker_ImpactLegR      L"animclip\\Lurker\\Wasteland_Lurker_Impact_LegR_01.animclip"
#define Lurker_ImpactTorso     L"animclip\\Lurker\\Wasteland_Lurker_Impact_Torso_F.animclip"
#define Lurker_ImpactFront     L"animclip\\Lurker\\Wasteland_Lurker_Impact_Heavy_F_01.animclip"

