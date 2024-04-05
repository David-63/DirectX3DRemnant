#pragma once
#include "CB_StateScript.h"

class CB_STATEMoveScript :
	public CB_StateScript
{
	enum class eTurnAni
	{
		R90,
		R135,
		L90,
		L135,
		L180,
	};

public:
	virtual void begin() override;
	virtual void tick() override;
public:
	virtual void Enter() override;
	virtual void Exit() override;

private:
	int         m_testcount;

	bool        m_IsPhase_1;
	bool        m_GetWeapon;

	bool        m_OnceSpawnSpell;
	bool        m_OnceSpawnSpellUp;
	bool        m_OnceSpawnSpell_Stop;
	bool        m_OnceSpawnSpell_FireSpawn;
	bool        m_OnceSpawnSpell_Fire;


	int         m_SpellCooltime;

	CGameObject* m_SpellObj_Spawn_R;
	CGameObject* m_SpellObj_Spawn_UP_R;
	CGameObject* m_SpellObj_Spawn_Stop_R;
	CGameObject* m_SpellObj_Fire_R;

	CGameObject* m_SpellObj_Spawn_L;
	CGameObject* m_SpellObj_Spawn_UP_L;
	CGameObject* m_SpellObj_Spawn_Stop_L;
	CGameObject* m_SpellObj_Fire_L;


	bool        m_bBloodDrink;


	int         m_DirSetCount;

	float       m_fPrevForce; // m_fCurForce �����
	float       m_fCurForce;
	class CMonsterMoveScript* m_CMoveScript;

	bool        m_bStorePlayerPos;
	Vec3        m_vPlayerPos;

	//Turn
	bool       m_bTurnSet;
	float      m_fTurnDegree;
	eTurnAni   m_eTurnAni;
	Vec3       m_vTurndir;
	bool       m_BTurned180;
	bool       m_BTurned90L;
	bool       m_BTurned90R;
	bool       m_BTurned135L;
	bool       m_BTurned135R;

	//Turn�� ����2
	bool                m_bTurnBtn;
	Vec3                m_vInitialRot;
	Vec3                m_vFinalRot;
	Vec3                m_vInitialPos;
	Vec3                m_vFinalPos;
	float               m_fFinishTime;
	float               m_fElapsedTime;
	float               m_fAniTurnSpeed;
	bool                m_bUseAniTurn;

	//JustTurn
	bool                m_bJustTurnSet;
	bool                m_bJustTurnOnce;
	float               m_fJustTurnSpeed;

	//MoveStart
	bool       m_bMoveStartOnce;
	bool       m_bEnterSign;
	bool       m_bTurnAtStart;

	//renewPath
	bool	   m_bPathRenewed;
	bool       m_bGetDirOnce;
	bool       m_bTurnAtRenew;

	//catchCheck
	int        m_iCatchStage;
	Vec3       m_vToPlayerDir;

	//GetBack
	int        m_iGetBackStage;

	bool		mKey;
	bool       m_bEnterDistCheck;

	float      m_fTime;

	bool bWeaponStateCheck;


public:

	void SpawnSpell();
	void SpawnSpellMove();
	void NoWeapon_MoveToDir();
	void Weapon_Move();
	void Weapon_StanceChange();
	void Weapon_StanceEvent();

	void FastWalk_End();
	int ZeroToOneRandom();



	bool JustTurn(Vec3 _dir);

	bool AniTurn(Vec3 _dir);
	bool Turn180(float _degree);
	bool Turn90L(float _degree);
	bool Turn90R(float _degree);
	bool Turn135L(float _degree);
	bool Turn135R(float _degree);


	void TurnComplete180();
	void TurnComplete90L();
	void TurnComplete90R();
	void TurnComplete135L();
	void TurnComplete135R();


	void RenewPath();
	void CatchCheck();

	bool MoveStart();
	Vec3 CalMoveDir();
	void RotationReset();

	void GetBack();
	bool DistCheck();
	void Clear();

	void DecidePattern();

public:
	CLONE(CB_STATEMoveScript);

	CB_STATEMoveScript();
	virtual ~CB_STATEMoveScript();


};

