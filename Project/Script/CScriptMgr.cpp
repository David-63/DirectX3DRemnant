#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CCharacterMoveScript.h"
#include "CC_FSMScript.h"
#include "CC_StatesScript.h"
#include "CIdleStateScript.h"
#include "CMonsterMoveScript.h"
#include "CPathFinderScript.h"
#include "CPlayerScript.h"
#include "CPlayerScriptFsm.h"
#include "CP_FSMScript.h"
#include "CP_STATEIdleScript.h"
#include "CP_STATEMoveScript.h"
#include "CP_StatesScript.h"
#include "CShoulderViewScript.h"
#include "CTestScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCharacterMoveScript");
	_vec.push_back(L"CC_FSMScript");
	_vec.push_back(L"CC_StatesScript");
	_vec.push_back(L"CIdleStateScript");
	_vec.push_back(L"CMonsterMoveScript");
	_vec.push_back(L"CPathFinderScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerScriptFsm");
	_vec.push_back(L"CP_FSMScript");
	_vec.push_back(L"CP_STATEIdleScript");
	_vec.push_back(L"CP_STATEMoveScript");
	_vec.push_back(L"CP_StatesScript");
	_vec.push_back(L"CShoulderViewScript");
	_vec.push_back(L"CTestScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCharacterMoveScript" == _strScriptName)
		return new CCharacterMoveScript;
	if (L"CC_FSMScript" == _strScriptName)
		return new CC_FSMScript;
	if (L"CC_StatesScript" == _strScriptName)
		return new CC_StatesScript;
	if (L"CIdleStateScript" == _strScriptName)
		return new CIdleStateScript;
	if (L"CMonsterMoveScript" == _strScriptName)
		return new CMonsterMoveScript;
	if (L"CPathFinderScript" == _strScriptName)
		return new CPathFinderScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayerScriptFsm" == _strScriptName)
		return new CPlayerScriptFsm;
	if (L"CP_FSMScript" == _strScriptName)
		return new CP_FSMScript;
	if (L"CP_STATEIdleScript" == _strScriptName)
		return new CP_STATEIdleScript;
	if (L"CP_STATEMoveScript" == _strScriptName)
		return new CP_STATEMoveScript;
	if (L"CP_StatesScript" == _strScriptName)
		return new CP_StatesScript;
	if (L"CShoulderViewScript" == _strScriptName)
		return new CShoulderViewScript;
	if (L"CTestScript" == _strScriptName)
		return new CTestScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CHARACTERMOVESCRIPT:
		return new CCharacterMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::C_FSMSCRIPT:
		return new CC_FSMScript;
		break;
	case (UINT)SCRIPT_TYPE::C_STATESSCRIPT:
		return new CC_StatesScript;
		break;
	case (UINT)SCRIPT_TYPE::IDLESTATESCRIPT:
		return new CIdleStateScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERMOVESCRIPT:
		return new CMonsterMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::PATHFINDERSCRIPT:
		return new CPathFinderScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPTFSM:
		return new CPlayerScriptFsm;
		break;
	case (UINT)SCRIPT_TYPE::P_FSMSCRIPT:
		return new CP_FSMScript;
		break;
	case (UINT)SCRIPT_TYPE::P_STATEIDLESCRIPT:
		return new CP_STATEIdleScript;
		break;
	case (UINT)SCRIPT_TYPE::P_STATEMOVESCRIPT:
		return new CP_STATEMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::P_STATESSCRIPT:
		return new CP_StatesScript;
		break;
	case (UINT)SCRIPT_TYPE::SHOULDERVIEWSCRIPT:
		return new CShoulderViewScript;
		break;
	case (UINT)SCRIPT_TYPE::TESTSCRIPT:
		return new CTestScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CHARACTERMOVESCRIPT:
		return L"CCharacterMoveScript";
		break;

	case SCRIPT_TYPE::C_FSMSCRIPT:
		return L"CC_FSMScript";
		break;

	case SCRIPT_TYPE::C_STATESSCRIPT:
		return L"CC_StatesScript";
		break;

	case SCRIPT_TYPE::IDLESTATESCRIPT:
		return L"CIdleStateScript";
		break;

	case SCRIPT_TYPE::MONSTERMOVESCRIPT:
		return L"CMonsterMoveScript";
		break;

	case SCRIPT_TYPE::PATHFINDERSCRIPT:
		return L"CPathFinderScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPTFSM:
		return L"CPlayerScriptFsm";
		break;

	case SCRIPT_TYPE::P_FSMSCRIPT:
		return L"CP_FSMScript";
		break;

	case SCRIPT_TYPE::P_STATEIDLESCRIPT:
		return L"CP_STATEIdleScript";
		break;

	case SCRIPT_TYPE::P_STATEMOVESCRIPT:
		return L"CP_STATEMoveScript";
		break;

	case SCRIPT_TYPE::P_STATESSCRIPT:
		return L"CP_StatesScript";
		break;

	case SCRIPT_TYPE::SHOULDERVIEWSCRIPT:
		return L"CShoulderViewScript";
		break;

	case SCRIPT_TYPE::TESTSCRIPT:
		return L"CTestScript";
		break;

	}
	return nullptr;
}