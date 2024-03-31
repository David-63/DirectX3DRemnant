#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	B_FSMSCRIPT,
	B_STATEDAMAGEDSCRIPT,
	B_STATEDEADSCRIPT,
	B_STATEHEALSCRIPT,
	B_STATEIDLESCRIPT,
	B_STATEMELEESCRIPT,
	B_STATEMOVESCRIPT,
	B_STATERANGEDSCRIPT,
	B_STATESCRIPT,
	CAMERAMOVESCRIPT,
	CHARACTERMOVESCRIPT,
	C_FSMSCRIPT,
	C_STATESSCRIPT,
	HITBOXSCRIPT,
	IDLESTATESCRIPT,
	MONSTERMOVESCRIPT,
	M_LURKER_FSMSCRIPT,
	M_LURKER_STATESSCRIPT,
	M_LURKER_STATE_DAMAGED_SCRIPT,
	M_LURKER_STATE_DEAD_SCRIPT,
	M_LURKER_STATE_IDLE_SCRIPT,
	M_LURKER_STATE_MELEE_SCRIPT,
	M_LURKER_STATE_MOVE_SCRIPT,
	M_SPIDER_FSMSCRIPT,
	M_SPIDER_PROJ_SCRIPT,
	M_SPIDER_STATESSCRIPT,
	M_SPIDER_STATE_ATK_SCRIPT,
	M_SPIDER_STATE_DAMAGED_SCRIPT,
	M_SPIDER_STATE_DEAD_SCRIPT,
	M_SPIDER_STATE_IDLE_SCRIPT,
	M_SPIDER_STATE_MOVE_SCRIPT,
	PATHFINDERSCRIPT,
	PLAYERSCRIPT,
	PLAYERSCRIPTFSM,
	P_FSMSCRIPT,
	P_MOUSECTRLSCRIPT,
	P_STATEDODGESCRIPT,
	P_STATEFIRESCRIPT,
	P_STATEIDLESCRIPT,
	P_STATEMOVESCRIPT,
	P_STATERELOADSCRIPT,
	P_STATESSCRIPT,
	P_STATESTAGGERSCRIPT,
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
