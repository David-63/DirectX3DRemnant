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
	Push,
	Shoot,
};

class CM_Spider_FSMScript;

class CM_Spider_StatesScript : public CC_StatesScript
{
protected:
	CM_Spider_FSMScript* m_MHQ;

private:
	eM_A_States           m_StateType;


public:
	virtual void begin() override;

public:
	virtual void SetStateType(UINT _StateType) override { m_StateType = static_cast<eM_A_States>(_StateType); }
	virtual UINT GetStateType() const override { return static_cast<UINT>(m_StateType); }


private:

public:
	CLONE(CM_Spider_StatesScript);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
public:
	CM_Spider_StatesScript();
	virtual ~CM_Spider_StatesScript();
};

