#include "pch.h"
#include "CP_WeaponScript.h"
#include "CP_FSMScript.h"

CP_WeaponScript::CP_WeaponScript() : CScript((UINT)SCRIPT_TYPE::P_WEAPONSCRIPT)
{
}

CP_WeaponScript::~CP_WeaponScript()
{
}

void CP_WeaponScript::begin()
{
	m_OwnerAnimator = m_PHQ->Animator3D();
	m_OwnerTransform = m_PHQ->Transform();
}

void CP_WeaponScript::tick()
{
	m_HandMatrix = m_OwnerAnimator->GetBoneMatrix(168);
	

	Vec3 bonePos(m_HandMatrix._41, m_HandMatrix._42, m_HandMatrix._43);
	m_HandMatrix.Translation(bonePos);

}
