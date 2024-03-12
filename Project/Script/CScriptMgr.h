#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	CAMERAMOVESCRIPT,
	CHARACTERMOVESCRIPT,
	C_FSMSCRIPT,
	C_STATESSCRIPT,
	IDLESTATESCRIPT,
	MONSTERMOVESCRIPT,
	M_LURKER_FSMSCRIPT,
	M_LURKER_STATESSCRIPT,
	M_LURKER_STATE_IDLE_SCRIPT,
	M_LURKER_STATE_SLEEP_SCRIPT,
	PATHFINDERSCRIPT,
	PLAYERSCRIPT,
	PLAYERSCRIPTFSM,
	P_FSMSCRIPT,
	P_STATEIDLESCRIPT,
	P_STATEMOVESCRIPT,
	P_STATESSCRIPT,
	SHOULDERVIEWSCRIPT,
	TESTSCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
