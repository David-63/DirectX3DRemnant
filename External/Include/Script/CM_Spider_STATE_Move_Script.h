#pragma once
#include "CM_Spider_StatesScript.h"


class CM_Spider_STATE_Move_Script : public CM_Spider_StatesScript
{
	enum class eTurnAni
	{
		R90,
		R135,
		L90,
		L135,
		L180,
	};
private:
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

	//Turn¿ë º¯¼ö2
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

	//Move
	bool       m_bMoveStartOnce;


	//GetBack
	int        m_iGetBackStage;
	bool       m_bBackProgress;
	bool       m_bBackAniOnce;

	bool mKey;
	bool       m_bEnterDistCheck;

	bool       m_bDirCheck;
	Vec3       m_vToPlayerDir;
	bool       m_bShootReady;
	float      m_fToPlayerDegree;

	class CMonsterMoveScript* m_CMoveScript;
	eAtkState m_eAtkInMove;

public:
	virtual void tick() override;
	virtual void begin() override;

public:


private:
	void Clear();
	bool AniTurn(Vec3 _dir);
	bool JustTurn(Vec3 _dir);

	void RenewPath();

	void GetBack();
	bool ShootTest();
	void TurnAction();
	void ActionByDist();


	void TurnComplete180();
	void TurnComplete90L();
	void TurnComplete90R();
	void TurnComplete135L();
	void TurnComplete135R();
	bool Turn180(float _degree);
	bool Turn90L(float _degree);
	bool Turn90R(float _degree);
	bool Turn135L(float _degree);
	bool Turn135R(float _degree);

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	CLONE(CM_Spider_STATE_Move_Script);

	CM_Spider_STATE_Move_Script();
	virtual ~CM_Spider_STATE_Move_Script();

};

