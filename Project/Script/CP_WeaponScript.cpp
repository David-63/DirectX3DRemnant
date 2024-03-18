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
	m_HandBoneData = m_OwnerAnimator->GetMTBoneData(168);
	int frameIdx = m_OwnerAnimator->GetCurFrame();
	m_HandBoneData.matOffset;
	// 결국 프레임을 가져와야함
	Vec3 trans = m_HandBoneData.vecKeyFrame[frameIdx].vTranslate;
	Vec4 rotQ = m_HandBoneData.vecKeyFrame[frameIdx].qRot;
	Vec3 Scaling = m_HandBoneData.vecKeyFrame[frameIdx].vScale;
	Matrix MScaling = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, };
	MScaling._11 = Scaling.x;
	MScaling._22 = Scaling.y;
	MScaling._33 = Scaling.z;
	Matrix MRotation = XMMatrixRotationQuaternion(rotQ);

	Matrix frameMat = MScaling;	// Scale
	frameMat *= MRotation;		// Rotation
	frameMat._41 += trans.x;	// Transform
	frameMat._42 += trans.y;
	frameMat._43 += trans.z;
	frameMat._44 += 0.f;
	

	m_HandMatrix = frameMat * m_HandBoneData.matOffset;
	Matrix OwnerMat = m_OwnerTransform->GetWorldMat();
	Matrix finalMat = OwnerMat * m_HandMatrix;


	// 상대적인 위치
	Vec3 bonePos = finalMat.Translation();
	Transform()->SetRelativePos(bonePos);
	int a = 0;
}
