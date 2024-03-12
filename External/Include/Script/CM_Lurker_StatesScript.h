#pragma once
#include "CC_StatesScript.h"

enum class eM_States
{
	SLEEP,
	IDLE,
	MOVE,
	MELEE,
	DAMAGED,
	DEAD,
	END,
};

class CM_Lurker_FSMScript;

class CM_Lurker_StatesScript : public CC_StatesScript
{
protected:
	CM_Lurker_FSMScript* m_MHQ;

private:
	eM_States           m_StateType;

public:
	virtual void begin() override;

public:
	virtual void SetStateType(UINT _StateType) override { m_StateType = static_cast<eM_States>(_StateType); }
	virtual UINT GetStateType() const override { return static_cast<UINT>(m_StateType); }

public:
	CLONE(CM_Lurker_StatesScript);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
public:
	CM_Lurker_StatesScript();
	virtual ~CM_Lurker_StatesScript();
};

