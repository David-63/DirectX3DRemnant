#pragma once
#include "CC_StatesScript.h"

#define Lurker_Turn180       L"animclip\\Lurker\\Wasteland_Lurker_Turn180_L.animclip"
#define Lurker_Turn180N      L"animclip\\Lurker\\Wasteland_Lurker_Turn180_LN.animclip"
#define Lurker_Turn90L       L"animclip\\Lurker\\Wasteland_Lurker_Turn90_L.animclip"
#define Lurker_Turn90R       L"animclip\\Lurker\\Wasteland_Lurker_Turn90_R.animclip"
#define Lurker_Turn135L      L"animclip\\Lurker\\Wasteland_Lurker_Turn135_L.animclip"
#define Lurker_Turn135R      L"animclip\\Lurker\\Wasteland_Lurker_Turn135_R.animclip"

enum class eM_States
{
	IDLE,
	MELEE,
	MOVE,
	DAMAGED,
	DEAD,
	END,
};

enum class eMeleeState
{
	Dash,
	Heavy1,
	Slash,
	SlashCombo,
};

class CM_Lurker_FSMScript;

class CM_Lurker_StatesScript : public CC_StatesScript
{
protected:
	CM_Lurker_FSMScript* m_MHQ;
	static eMeleeState   m_eMeleeState;

private:
	eM_States           m_StateType;


public:
	virtual void begin() override;

public:
	virtual void SetStateType(UINT _StateType) override { m_StateType = static_cast<eM_States>(_StateType); }
	virtual UINT GetStateType() const override { return static_cast<UINT>(m_StateType); }


	/*Vec3 GetPlayerPos();
	CGameObject* GetPlayer();
	Vec3 GetMyPos();
	void SetMyPos(Vec3 _pos);
	Vec3 GetMyFront();
	Vec3 GetToPlayerDir();
	float DistBetwPlayer();
	float CalTurnDegree(Vec3 _dir);*/


private:

public:
	CLONE(CM_Lurker_StatesScript);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
public:
	CM_Lurker_StatesScript();
	virtual ~CM_Lurker_StatesScript();
};

