#pragma once
#include "CC_StatesScript.h"

enum class eM_A_States
{
	IDLE,
	ATTACK,
	MOVE,
	DAMAGED,
	DEAD,
	END,
};

enum class eAtkState
{
	Dash,
	Heavy1,
	Slash,
	SlashCombo,
};

class CM_Annointed_FSMScript;

class CM_Annointed_StatesScript : public CC_StatesScript
{
protected:
	CM_Annointed_FSMScript* m_MHQ;

private:
	eM_A_States           m_StateType;


public:
	virtual void begin() override;

public:
	virtual void SetStateType(UINT _StateType) override { m_StateType = static_cast<eM_A_States>(_StateType); }
	virtual UINT GetStateType() const override { return static_cast<UINT>(m_StateType); }


private:

public:
	CLONE(CM_Annointed_StatesScript);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
public:
	CM_Annointed_StatesScript();
	virtual ~CM_Annointed_StatesScript();
};

