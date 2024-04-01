#include "pch.h"
#include "CScriptMgr.h"

#include "CB_FSMScript.h"
#include "CB_STATEDamagedScript.h"
#include "CB_STATEDeadScript.h"
#include "CB_STATEHealScript.h"
#include "CB_STATEIdleScript.h"
#include "CB_STATEMeleeScript.h"
#include "CB_STATEMoveScript.h"
#include "CB_STATERangedScript.h"
#include "CB_StateScript.h"
#include "CCameraMoveScript.h"
#include "CCharacterMoveScript.h"
#include "CC_FSMScript.h"
#include "CC_StatesScript.h"
#include "CHitBoxScript.h"
#include "CIdleStateScript.h"
#include "CMonsterMoveScript.h"
#include "CM_Lurker_FSMScript.h"
#include "CM_Lurker_StatesScript.h"
#include "CM_Lurker_STATE_Damaged_Script.h"
#include "CM_Lurker_STATE_Dead_Script.h"
#include "CM_Lurker_STATE_Idle_Script.h"
#include "CM_Lurker_STATE_Melee_Script.h"
#include "CM_Lurker_STATE_Move_Script.h"
#include "CM_Spider_FSMScript.h"
#include "CM_Spider_Proj_Script.h"
#include "CM_Spider_StatesScript.h"
#include "CM_Spider_STATE_Atk_Script.h"
#include "CM_Spider_STATE_Damaged_Script.h"
#include "CM_Spider_STATE_Dead_Script.h"
#include "CM_Spider_STATE_Idle_Script.h"
#include "CM_Spider_STATE_Move_Script.h"
#include "CPathFinderScript.h"
#include "CPlayerScript.h"
#include "CPlayerScriptFsm.h"
#include "CP_FSMScript.h"
#include "CP_MouseCtrlScript.h"
#include "CP_STATEDodgeScript.h"
#include "CP_STATEFireScript.h"
#include "CP_STATEIdleScript.h"
#include "CP_STATEMoveScript.h"
#include "CP_STATEReloadScript.h"
#include "CP_StatesScript.h"
#include "CP_STATEStaggerScript.h"
#include "CShoulderViewScript.h"
#include "CTestScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CB_FSMScript");
	_vec.push_back(L"CB_STATEDamagedScript");
	_vec.push_back(L"CB_STATEDeadScript");
	_vec.push_back(L"CB_STATEHealScript");
	_vec.push_back(L"CB_STATEIdleScript");
	_vec.push_back(L"CB_STATEMeleeScript");
	_vec.push_back(L"CB_STATEMoveScript");
	_vec.push_back(L"CB_STATERangedScript");
	_vec.push_back(L"CB_StateScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCharacterMoveScript");
	_vec.push_back(L"CC_FSMScript");
	_vec.push_back(L"CC_StatesScript");
	_vec.push_back(L"CHitBoxScript");
	_vec.push_back(L"CIdleStateScript");
	_vec.push_back(L"CMonsterMoveScript");
	_vec.push_back(L"CM_Lurker_FSMScript");
	_vec.push_back(L"CM_Lurker_StatesScript");
	_vec.push_back(L"CM_Lurker_STATE_Damaged_Script");
	_vec.push_back(L"CM_Lurker_STATE_Dead_Script");
	_vec.push_back(L"CM_Lurker_STATE_Idle_Script");
	_vec.push_back(L"CM_Lurker_STATE_Melee_Script");
	_vec.push_back(L"CM_Lurker_STATE_Move_Script");
	_vec.push_back(L"CM_Spider_FSMScript");
	_vec.push_back(L"CM_Spider_Proj_Script");
	_vec.push_back(L"CM_Spider_StatesScript");
	_vec.push_back(L"CM_Spider_STATE_Atk_Script");
	_vec.push_back(L"CM_Spider_STATE_Damaged_Script");
	_vec.push_back(L"CM_Spider_STATE_Dead_Script");
	_vec.push_back(L"CM_Spider_STATE_Idle_Script");
	_vec.push_back(L"CM_Spider_STATE_Move_Script");
	_vec.push_back(L"CPathFinderScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerScriptFsm");
	_vec.push_back(L"CP_FSMScript");
	_vec.push_back(L"CP_MouseCtrlScript");
	_vec.push_back(L"CP_STATEDodgeScript");
	_vec.push_back(L"CP_STATEFireScript");
	_vec.push_back(L"CP_STATEIdleScript");
	_vec.push_back(L"CP_STATEMoveScript");
	_vec.push_back(L"CP_STATEReloadScript");
	_vec.push_back(L"CP_StatesScript");
	_vec.push_back(L"CP_STATEStaggerScript");
	_vec.push_back(L"CShoulderViewScript");
	_vec.push_back(L"CTestScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CB_FSMScript" == _strScriptName)
		return new CB_FSMScript;
	if (L"CB_STATEDamagedScript" == _strScriptName)
		return new CB_STATEDamagedScript;
	if (L"CB_STATEDeadScript" == _strScriptName)
		return new CB_STATEDeadScript;
	if (L"CB_STATEHealScript" == _strScriptName)
		return new CB_STATEHealScript;
	if (L"CB_STATEIdleScript" == _strScriptName)
		return new CB_STATEIdleScript;
	if (L"CB_STATEMeleeScript" == _strScriptName)
		return new CB_STATEMeleeScript;
	if (L"CB_STATEMoveScript" == _strScriptName)
		return new CB_STATEMoveScript;
	if (L"CB_STATERangedScript" == _strScriptName)
		return new CB_STATERangedScript;
	if (L"CB_StateScript" == _strScriptName)
		return new CB_StateScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCharacterMoveScript" == _strScriptName)
		return new CCharacterMoveScript;
	if (L"CC_FSMScript" == _strScriptName)
		return new CC_FSMScript;
	if (L"CC_StatesScript" == _strScriptName)
		return new CC_StatesScript;
	if (L"CHitBoxScript" == _strScriptName)
		return new CHitBoxScript;
	if (L"CIdleStateScript" == _strScriptName)
		return new CIdleStateScript;
	if (L"CMonsterMoveScript" == _strScriptName)
		return new CMonsterMoveScript;
	if (L"CM_Lurker_FSMScript" == _strScriptName)
		return new CM_Lurker_FSMScript;
	if (L"CM_Lurker_StatesScript" == _strScriptName)
		return new CM_Lurker_StatesScript;
	if (L"CM_Lurker_STATE_Damaged_Script" == _strScriptName)
		return new CM_Lurker_STATE_Damaged_Script;
	if (L"CM_Lurker_STATE_Dead_Script" == _strScriptName)
		return new CM_Lurker_STATE_Dead_Script;
	if (L"CM_Lurker_STATE_Idle_Script" == _strScriptName)
		return new CM_Lurker_STATE_Idle_Script;
	if (L"CM_Lurker_STATE_Melee_Script" == _strScriptName)
		return new CM_Lurker_STATE_Melee_Script;
	if (L"CM_Lurker_STATE_Move_Script" == _strScriptName)
		return new CM_Lurker_STATE_Move_Script;
	if (L"CM_Spider_FSMScript" == _strScriptName)
		return new CM_Spider_FSMScript;
	if (L"CM_Spider_Proj_Script" == _strScriptName)
		return new CM_Spider_Proj_Script;
	if (L"CM_Spider_StatesScript" == _strScriptName)
		return new CM_Spider_StatesScript;
	if (L"CM_Spider_STATE_Atk_Script" == _strScriptName)
		return new CM_Spider_STATE_Atk_Script;
	if (L"CM_Spider_STATE_Damaged_Script" == _strScriptName)
		return new CM_Spider_STATE_Damaged_Script;
	if (L"CM_Spider_STATE_Dead_Script" == _strScriptName)
		return new CM_Spider_STATE_Dead_Script;
	if (L"CM_Spider_STATE_Idle_Script" == _strScriptName)
		return new CM_Spider_STATE_Idle_Script;
	if (L"CM_Spider_STATE_Move_Script" == _strScriptName)
		return new CM_Spider_STATE_Move_Script;
	if (L"CPathFinderScript" == _strScriptName)
		return new CPathFinderScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayerScriptFsm" == _strScriptName)
		return new CPlayerScriptFsm;
	if (L"CP_FSMScript" == _strScriptName)
		return new CP_FSMScript;
	if (L"CP_MouseCtrlScript" == _strScriptName)
		return new CP_MouseCtrlScript;
	if (L"CP_STATEDodgeScript" == _strScriptName)
		return new CP_STATEDodgeScript;
	if (L"CP_STATEFireScript" == _strScriptName)
		return new CP_STATEFireScript;
	if (L"CP_STATEIdleScript" == _strScriptName)
		return new CP_STATEIdleScript;
	if (L"CP_STATEMoveScript" == _strScriptName)
		return new CP_STATEMoveScript;
	if (L"CP_STATEReloadScript" == _strScriptName)
		return new CP_STATEReloadScript;
	if (L"CP_StatesScript" == _strScriptName)
		return new CP_StatesScript;
	if (L"CP_STATEStaggerScript" == _strScriptName)
		return new CP_STATEStaggerScript;
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
	case (UINT)SCRIPT_TYPE::B_FSMSCRIPT:
		return new CB_FSMScript;
		break;
	case (UINT)SCRIPT_TYPE::B_STATEDAMAGEDSCRIPT:
		return new CB_STATEDamagedScript;
		break;
	case (UINT)SCRIPT_TYPE::B_STATEDEADSCRIPT:
		return new CB_STATEDeadScript;
		break;
	case (UINT)SCRIPT_TYPE::B_STATEHEALSCRIPT:
		return new CB_STATEHealScript;
		break;
	case (UINT)SCRIPT_TYPE::B_STATEIDLESCRIPT:
		return new CB_STATEIdleScript;
		break;
	case (UINT)SCRIPT_TYPE::B_STATEMELEESCRIPT:
		return new CB_STATEMeleeScript;
		break;
	case (UINT)SCRIPT_TYPE::B_STATEMOVESCRIPT:
		return new CB_STATEMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::B_STATERANGEDSCRIPT:
		return new CB_STATERangedScript;
		break;
	case (UINT)SCRIPT_TYPE::B_STATESCRIPT:
		return new CB_StateScript;
		break;
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
	case (UINT)SCRIPT_TYPE::HITBOXSCRIPT:
		return new CHitBoxScript;
		break;
	case (UINT)SCRIPT_TYPE::IDLESTATESCRIPT:
		return new CIdleStateScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERMOVESCRIPT:
		return new CMonsterMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::M_LURKER_FSMSCRIPT:
		return new CM_Lurker_FSMScript;
		break;
	case (UINT)SCRIPT_TYPE::M_LURKER_STATESSCRIPT:
		return new CM_Lurker_StatesScript;
		break;
	case (UINT)SCRIPT_TYPE::M_LURKER_STATE_DAMAGED_SCRIPT:
		return new CM_Lurker_STATE_Damaged_Script;
		break;
	case (UINT)SCRIPT_TYPE::M_LURKER_STATE_DEAD_SCRIPT:
		return new CM_Lurker_STATE_Dead_Script;
		break;
	case (UINT)SCRIPT_TYPE::M_LURKER_STATE_IDLE_SCRIPT:
		return new CM_Lurker_STATE_Idle_Script;
		break;
	case (UINT)SCRIPT_TYPE::M_LURKER_STATE_MELEE_SCRIPT:
		return new CM_Lurker_STATE_Melee_Script;
		break;
	case (UINT)SCRIPT_TYPE::M_LURKER_STATE_MOVE_SCRIPT:
		return new CM_Lurker_STATE_Move_Script;
		break;
	case (UINT)SCRIPT_TYPE::M_SPIDER_FSMSCRIPT:
		return new CM_Spider_FSMScript;
		break;
	case (UINT)SCRIPT_TYPE::M_SPIDER_PROJ_SCRIPT:
		return new CM_Spider_Proj_Script;
		break;
	case (UINT)SCRIPT_TYPE::M_SPIDER_STATESSCRIPT:
		return new CM_Spider_StatesScript;
		break;
	case (UINT)SCRIPT_TYPE::M_SPIDER_STATE_ATK_SCRIPT:
		return new CM_Spider_STATE_Atk_Script;
		break;
	case (UINT)SCRIPT_TYPE::M_SPIDER_STATE_DAMAGED_SCRIPT:
		return new CM_Spider_STATE_Damaged_Script;
		break;
	case (UINT)SCRIPT_TYPE::M_SPIDER_STATE_DEAD_SCRIPT:
		return new CM_Spider_STATE_Dead_Script;
		break;
	case (UINT)SCRIPT_TYPE::M_SPIDER_STATE_IDLE_SCRIPT:
		return new CM_Spider_STATE_Idle_Script;
		break;
	case (UINT)SCRIPT_TYPE::M_SPIDER_STATE_MOVE_SCRIPT:
		return new CM_Spider_STATE_Move_Script;
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
	case (UINT)SCRIPT_TYPE::P_MOUSECTRLSCRIPT:
		return new CP_MouseCtrlScript;
		break;
	case (UINT)SCRIPT_TYPE::P_STATEDODGESCRIPT:
		return new CP_STATEDodgeScript;
		break;
	case (UINT)SCRIPT_TYPE::P_STATEFIRESCRIPT:
		return new CP_STATEFireScript;
		break;
	case (UINT)SCRIPT_TYPE::P_STATEIDLESCRIPT:
		return new CP_STATEIdleScript;
		break;
	case (UINT)SCRIPT_TYPE::P_STATEMOVESCRIPT:
		return new CP_STATEMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::P_STATERELOADSCRIPT:
		return new CP_STATEReloadScript;
		break;
	case (UINT)SCRIPT_TYPE::P_STATESSCRIPT:
		return new CP_StatesScript;
		break;
	case (UINT)SCRIPT_TYPE::P_STATESTAGGERSCRIPT:
		return new CP_STATEStaggerScript;
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
	case SCRIPT_TYPE::B_FSMSCRIPT:
		return L"CB_FSMScript";
		break;

	case SCRIPT_TYPE::B_STATEDAMAGEDSCRIPT:
		return L"CB_STATEDamagedScript";
		break;

	case SCRIPT_TYPE::B_STATEDEADSCRIPT:
		return L"CB_STATEDeadScript";
		break;

	case SCRIPT_TYPE::B_STATEHEALSCRIPT:
		return L"CB_STATEHealScript";
		break;

	case SCRIPT_TYPE::B_STATEIDLESCRIPT:
		return L"CB_STATEIdleScript";
		break;

	case SCRIPT_TYPE::B_STATEMELEESCRIPT:
		return L"CB_STATEMeleeScript";
		break;

	case SCRIPT_TYPE::B_STATEMOVESCRIPT:
		return L"CB_STATEMoveScript";
		break;

	case SCRIPT_TYPE::B_STATERANGEDSCRIPT:
		return L"CB_STATERangedScript";
		break;

	case SCRIPT_TYPE::B_STATESCRIPT:
		return L"CB_StateScript";
		break;

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

	case SCRIPT_TYPE::HITBOXSCRIPT:
		return L"CHitBoxScript";
		break;

	case SCRIPT_TYPE::IDLESTATESCRIPT:
		return L"CIdleStateScript";
		break;

	case SCRIPT_TYPE::MONSTERMOVESCRIPT:
		return L"CMonsterMoveScript";
		break;

	case SCRIPT_TYPE::M_LURKER_FSMSCRIPT:
		return L"CM_Lurker_FSMScript";
		break;

	case SCRIPT_TYPE::M_LURKER_STATESSCRIPT:
		return L"CM_Lurker_StatesScript";
		break;

	case SCRIPT_TYPE::M_LURKER_STATE_DAMAGED_SCRIPT:
		return L"CM_Lurker_STATE_Damaged_Script";
		break;

	case SCRIPT_TYPE::M_LURKER_STATE_DEAD_SCRIPT:
		return L"CM_Lurker_STATE_Dead_Script";
		break;

	case SCRIPT_TYPE::M_LURKER_STATE_IDLE_SCRIPT:
		return L"CM_Lurker_STATE_Idle_Script";
		break;

	case SCRIPT_TYPE::M_LURKER_STATE_MELEE_SCRIPT:
		return L"CM_Lurker_STATE_Melee_Script";
		break;

	case SCRIPT_TYPE::M_LURKER_STATE_MOVE_SCRIPT:
		return L"CM_Lurker_STATE_Move_Script";
		break;

	case SCRIPT_TYPE::M_SPIDER_FSMSCRIPT:
		return L"CM_Spider_FSMScript";
		break;

	case SCRIPT_TYPE::M_SPIDER_PROJ_SCRIPT:
		return L"CM_Spider_Proj_Script";
		break;

	case SCRIPT_TYPE::M_SPIDER_STATESSCRIPT:
		return L"CM_Spider_StatesScript";
		break;

	case SCRIPT_TYPE::M_SPIDER_STATE_ATK_SCRIPT:
		return L"CM_Spider_STATE_Atk_Script";
		break;

	case SCRIPT_TYPE::M_SPIDER_STATE_DAMAGED_SCRIPT:
		return L"CM_Spider_STATE_Damaged_Script";
		break;

	case SCRIPT_TYPE::M_SPIDER_STATE_DEAD_SCRIPT:
		return L"CM_Spider_STATE_Dead_Script";
		break;

	case SCRIPT_TYPE::M_SPIDER_STATE_IDLE_SCRIPT:
		return L"CM_Spider_STATE_Idle_Script";
		break;

	case SCRIPT_TYPE::M_SPIDER_STATE_MOVE_SCRIPT:
		return L"CM_Spider_STATE_Move_Script";
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

	case SCRIPT_TYPE::P_MOUSECTRLSCRIPT:
		return L"CP_MouseCtrlScript";
		break;

	case SCRIPT_TYPE::P_STATEDODGESCRIPT:
		return L"CP_STATEDodgeScript";
		break;

	case SCRIPT_TYPE::P_STATEFIRESCRIPT:
		return L"CP_STATEFireScript";
		break;

	case SCRIPT_TYPE::P_STATEIDLESCRIPT:
		return L"CP_STATEIdleScript";
		break;

	case SCRIPT_TYPE::P_STATEMOVESCRIPT:
		return L"CP_STATEMoveScript";
		break;

	case SCRIPT_TYPE::P_STATERELOADSCRIPT:
		return L"CP_STATEReloadScript";
		break;

	case SCRIPT_TYPE::P_STATESSCRIPT:
		return L"CP_StatesScript";
		break;

	case SCRIPT_TYPE::P_STATESTAGGERSCRIPT:
		return L"CP_STATEStaggerScript";
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