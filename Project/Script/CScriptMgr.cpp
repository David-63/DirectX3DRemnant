#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CCharacterMoveScript.h"
#include "CIdleStateScript.h"
#include "CMonsterMoveScript.h"
#include "CPathFinderScript.h"
#include "CPlayerScript.h"
#include "CPlayerScriptFsm.h"
#include "CShoulderViewScript.h"
#include "CTestScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCharacterMoveScript");
	_vec.push_back(L"CIdleStateScript");
	_vec.push_back(L"CMonsterMoveScript");
	_vec.push_back(L"CPathFinderScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerScriptFsm");
	_vec.push_back(L"CShoulderViewScript");
	_vec.push_back(L"CTestScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCharacterMoveScript" == _strScriptName)
		return new CCharacterMoveScript;
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

	case SCRIPT_TYPE::SHOULDERVIEWSCRIPT:
		return L"CShoulderViewScript";
		break;

	case SCRIPT_TYPE::TESTSCRIPT:
		return L"CTestScript";
		break;

	}
	return nullptr;
}