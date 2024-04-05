#include "pch.h"
#include "CRigidBody.h"
#include "Physics.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CCollisionMgr.h"
#include "CAnimator3D.h"

CRigidBody::CRigidBody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, mMaxVelocity(10.f)
	, mActor(nullptr)
	, mbAppliedPhysics(false)
	, mbAppliedGravity(false)
	, mbIsActorInLevel(false)
	, mbdrawCollider(true)
	, mShapeInfos{}
	, mShapes{}
{
}
CRigidBody::CRigidBody(const CRigidBody& _Other)
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, mMaxVelocity(_Other.mMaxVelocity)
	, mActorType(_Other.mActorType)
	, mbdrawCollider(_Other.mbdrawCollider)
{
	mShapeInfos.clear();
	for (const auto& shapeInfo : _Other.mShapeInfos)
	{
		mShapeInfos.push_back(shapeInfo);
	}
}
CRigidBody::~CRigidBody()
{
	for (int i = 0; i < mGeometries.size(); ++i)
	{
		delete mGeometries[i];
		mGeometries[i] = nullptr;
	}

}
void CRigidBody::SaveToLevelFile(FILE* _File)
{
	fwrite(&mFreezeRotationFlag, sizeof(EnumFlags<FreezeRotationFlag, uint16_t>), 1, _File);
	fwrite(&mActorType, sizeof(ACTOR_TYPE), 1, _File);
	int VecSize = mShapeInfos.size();
	fwrite(&VecSize, sizeof(int), 1, _File);
	for (int i = 0; i < VecSize; ++i)
	{
		fwrite(&mShapeInfos[i], sizeof(tShapeInfo), 1, _File);
	}
}
void CRigidBody::LoadFromLevelFile(FILE* _File)
{
	fread(&mFreezeRotationFlag, sizeof(EnumFlags<FreezeRotationFlag, uint16_t>), 1, _File);
	fread(&mActorType, sizeof(ACTOR_TYPE), 1, _File);
	int VecSize = 0;
	fread(&VecSize, sizeof(int), 1, _File);
	tShapeInfo temp;
	for (int i = 0; i < VecSize; ++i)
	{
		fread(&temp, sizeof(tShapeInfo), 1, _File);
		mShapeInfos.push_back(temp);
	}
}
void CRigidBody::begin()
{
	if (true == mbAppliedPhysics &&
		false == mbIsActorInLevel &&
		nullptr != mActor)
		AddActorToLevel();
}
void CRigidBody::tick()
{
}
void CRigidBody::finaltick()
{
	//DrawDebugMesh();

	//if(!m_bSoketUse)

	for (int i = 0; i < mShapeInfos.size(); ++i)
	{
		SetShapeLocalPosByBone(i, mShapeInfos[i].boneIdx);
	}

}
void CRigidBody::Destroy()
{
	if (true == mbAppliedPhysics)
	{
		physx::PxRigidActor* pActor = mActor->is<physx::PxRigidActor>();
		pActor->userData = nullptr;
	}
}

CComponent* CRigidBody::CloneFromObj(CGameObject* _pGameObject)
{
	CRigidBody* rigidBody = _pGameObject->RigidBody();

	if (true == mbAppliedPhysics)
	{
		rigidBody->SetActorType(mActorType);
		rigidBody->SetShapeVector(mShapes, mShapeInfos);

		rigidBody->SetPhysical(mActorType);
		_pGameObject->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
		_pGameObject->Transform()->SetRelativeRot(GetOwner()->Transform()->GetRelativeRot());
		_pGameObject->Transform()->SetRelativeScale(GetOwner()->Transform()->GetRelativeScale());
	}

	if (ACTOR_TYPE::Static != mActorType)
	{
		rigidBody->SetVelocity(mVelocity);
		rigidBody->SetMaxVelocity(mMaxVelocity);
	}

	return rigidBody;
}

void CRigidBody::SetPhysical(ACTOR_TYPE _eActorType)
{
	AssertEx(mShapeInfos.size() != 0, L"ShapeInfo가 설정되지 않음.");

	mActorType = _eActorType;
	mbAppliedPhysics = true;

	CreateMaterial();
	CreateGeometry();
	CreateActor();
	CreateShape();


	InitializeActor();
}
bool CRigidBody::IsAppliedPhysics()
{
	return mbAppliedPhysics;
}
void CRigidBody::ReleaseActor()
{
	for (auto shape : mShapes)
	{
		shape->release();
	}

	mActor->release();
}
void CRigidBody::AddActorToLevel()
{
	AssertEx(mbAppliedPhysics, L"RigidBody::AddActorToLevel() - 물리가 들어가지 않은 오브젝트에 대한 AddActorToScene 호출");
	AssertEx(mActor, L"RigidBody::AddActorToLevel() - mpActor가 생성되지 않음");

	if (mActorType == ACTOR_TYPE::Dynamic)
	{
		std::vector<PxReal> shapeMasses(mShapes.size());
		for (int i = 0; i < mShapes.size(); i++)
		{
			shapeMasses[i] = 1.f;
		}

		std::vector<PxVec3> shapeLocalPoses(mShapes.size());
		for (int i = 0; i < mShapes.size(); i++)
		{
			Vec3 offset = mShapeInfos[i].offset;
			shapeLocalPoses[i] = { offset.x, offset.y, offset.z };
		}

		PxRigidBodyExt::setMassAndUpdateInertia(*GetDynamicActor(), shapeMasses[0], &shapeLocalPoses[0]);
	}
	
	Physics::GetInst()->AddActor(GetOwner());
	mbIsActorInLevel = true;
}
physx::PxTransform CRigidBody::GetPhysicsTransform()
{
	AssertEx(mbAppliedPhysics, L"RigidBody::GetPhysicsTransform() - 물리가 들어가지 않은 오브젝트에 대한 GetPhysicsTransform 호출");
	return GetActor<physx::PxRigidActor>()->getGlobalPose();
}
void CRigidBody::SetPhysicsTransform(physx::PxTransform _transform)
{
	//_transform.p.z = -_transform.p.z;
	AssertEx(mbAppliedPhysics, L"RigidBody::SetPhysicsTransform() - 물리가 들어가지 않은 오브젝트에 대한 SetPhysicsTransform 호출");
	GetActor<physx::PxRigidActor>()->setGlobalPose(_transform);
}
void CRigidBody::SetVelocity(const Vector3 _velocity)
{
	if (true == mbAppliedPhysics)
	{
		AssertEx(ACTOR_TYPE::Dynamic == mActorType, L"RigidBody::SetVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
		physx::PxVec3 pxVelocity = physx::PxVec3(_velocity.x, _velocity.y, _velocity.z);
		GetDynamicActor()->setLinearVelocity(pxVelocity);
	}
	else
	{
		mVelocity = _velocity;
	}

	CheckMaxVelocity();
}

void CRigidBody::SetVelocity(AXIS3D_TYPE _eAxis, float _velocity)
{
	if (true == mbAppliedPhysics)
	{
		AssertEx(ACTOR_TYPE::Dynamic == mActorType, L"RigidBody::SetVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
		PxVec3 velocity = GetDynamicActor()->getLinearVelocity();
		switch (_eAxis)
		{
		case AXIS3D_TYPE::X:
			velocity.x = _velocity;
			break;
		case AXIS3D_TYPE::Y:
			velocity.y = _velocity;
			break;
		case AXIS3D_TYPE::Z:
			velocity.z = _velocity;
			break;
		}
		GetDynamicActor()->setLinearVelocity(velocity);

		mVelocity.x = velocity.x;
		mVelocity.y = velocity.y;
		mVelocity.z = velocity.z;
	}
	else
	{
		switch (_eAxis)
		{
		case AXIS3D_TYPE::X:
			mVelocity.x = _velocity;
			break;
		case AXIS3D_TYPE::Y:
			mVelocity.y = _velocity;
			break;
		case AXIS3D_TYPE::Z:
			mVelocity.z = _velocity;
			break;
		}
	}

	CheckMaxVelocity();
}
void CRigidBody::AddVelocity(const Vector3& _velocity)
{
	if (true == mbAppliedPhysics)
	{
		AssertEx(ACTOR_TYPE::Dynamic == mActorType, L"RigidBody::AddVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
		PxVec3 velocity = GetDynamicActor()->getLinearVelocity();
		velocity.x += _velocity.x;
		velocity.y += _velocity.y;
		velocity.z += _velocity.z;

		GetDynamicActor()->setLinearVelocity(velocity);
	}
	else
	{
		mVelocity = _velocity;
	}

	CheckMaxVelocity();
}
void CRigidBody::AddVelocity(AXIS3D_TYPE _eAxis, float _velocity)
{
	if (true == mbAppliedPhysics)
	{
		AssertEx(ACTOR_TYPE::Dynamic == mActorType, L"RigidBody::AddVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
		PxVec3 velocity = GetDynamicActor()->getLinearVelocity();
		switch (_eAxis)
		{
		case AXIS3D_TYPE::X:
			velocity.x += _velocity;
			break;
		case AXIS3D_TYPE::Y:
			velocity.y += _velocity;
			break;
		case AXIS3D_TYPE::Z:
			velocity.z += _velocity;
			break;
		}
		GetDynamicActor()->setLinearVelocity(velocity);
	}
	else
	{
		switch (_eAxis)
		{
		case AXIS3D_TYPE::X:
			mVelocity.x += _velocity;
			break;
		case AXIS3D_TYPE::Y:
			mVelocity.y += _velocity;
			break;
		case AXIS3D_TYPE::Z:
			mVelocity.z += _velocity;
			break;
		}
	}
	CheckMaxVelocity();
}
void CRigidBody::AddGravity()
{
	mVelocity += GLOBAL_GRAVITY;
}
void CRigidBody::SetMaxVelocity(float _maxVelocity)
{
	if (true == mbAppliedPhysics)
	{
		AssertEx(ACTOR_TYPE::Dynamic == mActorType, L"RigidBody::SetMaxVelocity() - Dynamic Actor가 아닌 물체에 대한 SetMaxVelocity() 호출 시도");
		GetDynamicActor()->setMaxLinearVelocity(_maxVelocity);
	}

	mMaxVelocity = _maxVelocity;
}
void CRigidBody::CheckMaxVelocity()
{
	mVelocity.x = mVelocity.x < 0.f ? max(mVelocity.x, -mMaxVelocity) : min(mVelocity.x, mMaxVelocity);
	mVelocity.y = mVelocity.y < 0.f ? max(mVelocity.y, -mMaxVelocity) : min(mVelocity.y, mMaxVelocity);
	mVelocity.z = mVelocity.z < 0.f ? max(mVelocity.z, -mMaxVelocity) : min(mVelocity.z, mMaxVelocity);
}
void CRigidBody::ApplyGravity()
{
	if (true == mbAppliedPhysics)
	{
		AssertEx(ACTOR_TYPE::Dynamic == mActorType, L"RigidBody::ApplyGravity() - Dynamic Actor가 아닌 물체에 대한 ApplyGravity() 호출 시도");
		GetDynamicActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	}

	mbAppliedGravity = true;
}
void CRigidBody::RemoveGravity()
{
	if (true == mbAppliedPhysics)
	{
		AssertEx(ACTOR_TYPE::Dynamic == mActorType, L"RigidBody::RemoveGravity() - Dynamic Actor가 아닌 물체에 대한 RemoveGravity() 호출 시도");
		GetDynamicActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	}

	mbAppliedGravity = false;
}
void CRigidBody::SetFreezeRotation(FreezeRotationFlag flag, bool enable)
{
	for (int i = 0; i < mShapes.size(); ++i)
	{
		physx::PxActor* actor = mShapes[i]->getActor();
		assert(actor);

		physx::PxRigidDynamic* rigidActor = actor->is<physx::PxRigidDynamic>();
		if (rigidActor == nullptr)
			return;

		EnumFlags<FreezeRotationFlag, uint16_t> enumFlag{ flag };

		if (enable)
		{
			mFreezeRotationFlag |= enumFlag;
			if (enumFlag & FreezeRotationFlag::ROTATION_X)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
			if (enumFlag & FreezeRotationFlag::ROTATION_Y)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
			if (enumFlag & FreezeRotationFlag::ROTATION_Z)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
		}
		else
		{
			mFreezeRotationFlag &= ~enumFlag;
			if (enumFlag & FreezeRotationFlag::ROTATION_X)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
			if (enumFlag & FreezeRotationFlag::ROTATION_Y)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
			if (enumFlag & FreezeRotationFlag::ROTATION_Z)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);
		}

		//해당 축 이동막기
		//rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, true);
		//rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
	}
}

void CRigidBody::SetLinearDamping(float _damping)
{
	AssertEx(ACTOR_TYPE::Dynamic == mActorType, L"RigidBody::SetLinearDamping() - Dynamic Actor가 아닌 물체에 대한 SetLinearDamping() 호출 시도");
	GetActor<physx::PxRigidDynamic>()->setLinearDamping(_damping);
}
void CRigidBody::SetSimulationShapeFlag(bool _bFlag)
{
	AssertEx(mbAppliedPhysics, L"RigidBody::SetSimulationShapeFlag() - 물리가 들어가지 않은 오브젝트에 대한 SetSimulationShapeFlag 호출");
	for (auto shape : mShapes)
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, _bFlag);
	}
}
void CRigidBody::SetTriggerShapeFlag(bool _bFlag)
{
	AssertEx(mbAppliedPhysics, L"RigidBody::SetTriggerShapeFlag() - 물리가 들어가지 않은 오브젝트에 대한 SetTriggerShapeFlag 호출");
	for (auto shape : mShapes)
	{
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, _bFlag);
	}
}
void CRigidBody::SetActorInLevelFlag(bool _bFlag)
{
	mbIsActorInLevel = _bFlag;
}

void CRigidBody::AddForce(const Vector3& _force)
{
	AssertEx(mbAppliedPhysics, L"RigidBody::AddForce() - 물리가 들어가지 않은 오브젝트에 대한 AddForce 호출");
	AssertEx(ACTOR_TYPE::Static != mActorType, L"RigidBody::AddForce() - Static 객체에 대해 힘 적용");

	PxVec3 pxForce = PxVec3(_force.x, _force.y, _force.z);

	PxRigidBodyExt::addForceAtPos(
		*GetDynamicActor(),
		pxForce,
		GetOwner()->Transform()->GetPhysicsPosition(),
		physx::PxForceMode::eFORCE
	);
}

void CRigidBody::SetSleep(bool _isSleep)
{
	GetActor()->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, _isSleep);
}


void CRigidBody::AttachShape(int _idx)
{
	GetRigidActor()->attachShape(*mShapes[_idx]);
}

void CRigidBody::SetShapeLocalPos(int _idx, Vec3 _localPos)
{
	PxTransform localpose(PxVec3(_localPos.x, _localPos.y, _localPos.z));
	localpose.q = GetPhysicsTransform().q;

	mShapes[_idx]->setLocalPose(localpose);
	mShapeInfos[_idx].offset = _localPos;
}

Vec3 CRigidBody::GetShapePosition(int _shapeIdx)
{
	PxTransform localTr = mShapes[_shapeIdx]->getLocalPose();
	PxTransform worldTr = GetPhysicsTransform();
	Vec3 pos = {};
	pos.x = worldTr.p.x + localTr.p.x;
	pos.y = worldTr.p.y + localTr.p.y;
	pos.z = worldTr.p.z + localTr.p.z;

	return pos;
}

void CRigidBody::SetShapeLocalPosByBone(int _idx, UINT _boneIdx)
{
	if (nullptr == GetOwner()->Animator3D() || nullptr == GetOwner()->Animator3D()->GetCurAnim())
		return;

	tMTBone bone = GetOwner()->Animator3D()->GetMTBoneData(_boneIdx);
	if (bone.vecKeyFrame.size() == 0)
		return;

	Matrix retBoneMat = GetOwner()->Animator3D()->GetBoneMatrix(_boneIdx);
	retBoneMat._44 = 0;

	Matrix ownerMat = GetOwner()->Transform()->GetWorldMat();
	Matrix totalMat = retBoneMat * ownerMat;
	Vec3 bonePosition = totalMat.Translation();
	
	Vec4 boneQRot = DirectX::XMQuaternionRotationMatrix(retBoneMat);

	PxTransform tr;
	tr.p.x = bonePosition.x;
	tr.p.y = bonePosition.y;
	tr.p.z = bonePosition.z;

	tr.q.x = boneQRot.x;
	tr.q.y = boneQRot.y;
	tr.q.z = boneQRot.z;
	tr.q.w = boneQRot.w;

	mShapes[_idx]->setLocalPose(tr);

	
}

void CRigidBody::SetShapeLocalPxTransform(int _idx, physx::PxTransform _tr)
{
	mShapes[_idx]->setLocalPose(_tr);
}

void CRigidBody::SetBoneSoket(int _shapeIdx, int _boneIdx)
{
	mShapeInfos[_shapeIdx].boneIdx = _boneIdx;
	m_bSoketUse = true;
}


void CRigidBody::CreateGeometry()
{
	for (int i = 0; i < mShapeInfos.size(); ++i)
	{
		mGeometries.resize(mShapeInfos.size());

		switch (mShapeInfos[i].eGeomType)
		{
		case GEOMETRY_TYPE::Box:
		{
			mGeometries[i] = new Geometries(mShapeInfos[i].eGeomType, mShapeInfos[i].size / 2.f);
			//GetOwner()->Transform()->SetRelativeScale(mShapeInfos[i].size);
		}
		break;

		case GEOMETRY_TYPE::Capsule:
			mGeometries[i] = new Geometries(mShapeInfos[i].eGeomType, mShapeInfos[i].size.x / 2.f, mShapeInfos[i].size.y / 2.f);
			break;

		case GEOMETRY_TYPE::Sphere:
			mGeometries[i] = new Geometries(mShapeInfos[i].eGeomType, mShapeInfos[i].size.x / 2.f);
			break;

		case GEOMETRY_TYPE::Plane:
			mGeometries[i] = new Geometries(mShapeInfos[i].eGeomType);
			break;
		}

		AssertEx(mGeometries[i], L"RigidBody::CreateGeometry() - Geometry 생성 실패");
	}
}
void CRigidBody::CreateShape()
{
	mShapes = {};

	for (int i = 0; i < mShapeInfos.size(); ++i)
	{
		PxShape* shape = {};
		switch (mShapeInfos[i].eGeomType)
		{
		case GEOMETRY_TYPE::Box:
			shape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mGeometries[i]->boxGeom, *mMaterial);
			break;
		case GEOMETRY_TYPE::Capsule:
			shape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mGeometries[i]->capsuleGeom, *mMaterial);
			break;
		case GEOMETRY_TYPE::Sphere:
			shape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mGeometries[i]->sphereGeom, *mMaterial);
			break;
		case GEOMETRY_TYPE::Plane:
			shape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mGeometries[i]->planeGeom, *mMaterial);
			break;
		}


		AssertEx(shape, L"RigidBody::CreateShape() - Shape 생성 실패");
		mShapes.push_back(shape);

		SetShapeLocalPos(i, mShapeInfos[i].offset);
	}
}

void CRigidBody::CreateActor()
{
	Vec3 playerPos = GetOwner()->Transform()->GetRelativePos();
	float _x = playerPos.x;
	float _y = playerPos.y;
	float _z = playerPos.z;
	switch (mActorType)
	{
	case ACTOR_TYPE::Dynamic:
		mActor = Physics::GetInst()->GetPhysics()->
			createRigidDynamic(physx::PxTransform(PxVec3(_x, _y, _z)));
		break;

	case ACTOR_TYPE::Static:
		mActor = Physics::GetInst()->GetPhysics()->
			createRigidStatic(physx::PxTransform(PxVec3(_x, _y, _z)));
		break;

	case ACTOR_TYPE::Kinematic:
		mActor = Physics::GetInst()->GetPhysics()->
			createRigidDynamic(physx::PxTransform(PxVec3(_x, _y, _z)));
		mActor->is<physx::PxRigidDynamic>()->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
		break;
	}

	AssertEx(mActor, L"RigidBody::CreateActor() - Actor 생성 실패");
}
void CRigidBody::CreateMaterial()
{
	mMaterial = Physics::GetInst()->GetPhysics()->createMaterial(mShapeInfos[0].massProperties.staticFriction,
		mShapeInfos[0].massProperties.dynamicFriction,
		mShapeInfos[0].massProperties.restitution);
}
void CRigidBody::InitializeActor()
{ 
	//충돌 매트릭스 생성
	physx::PxRigidActor* pActor = mActor->is<physx::PxRigidActor>();
	pActor->userData = GetOwner();

	int layerIdx = GetOwner()->GetLayerIndex();
	std::array<UINT, MAX_LAYER> collisionGroup = CCollisionMgr::GetInst()->GetMat();

	for (int j = 0; j < mShapeInfos.size(); ++j)
	{
		mShapeInfos[j].filterData.word0 = 1 << layerIdx;

		for (int i = 0; i < MAX_LAYER; ++i)
		{
			int mask = 0;
			mask = 1 << i;
			if ((collisionGroup[layerIdx] & mask) != 0)
				mShapeInfos[j].filterData.word1 |= 1 << i;
		}
	}

	for (int i = 0; i < mShapes.size(); ++i)
	{
		mShapes[i]->setSimulationFilterData(mShapeInfos[i].filterData);

		if (mShapeInfos[i].CollideType == 1)
		{
			PxFilterData data;
			data.word0 = 1 << 1;
			mShapes[i]->setQueryFilterData(data);
		}
		else if (mShapeInfos[i].CollideType == 2)
		{
			PxFilterData data;
			data.word0 = 1 << 2;
			mShapes[i]->setQueryFilterData(data);
		}
		else if (mShapeInfos[i].CollideType == 3)
		{
			PxFilterData data;
			data.word0 = 1 << 3;
			mShapes[i]->setQueryFilterData(data);
		}
	}

	//위치적용
	Vector3 trPos = GetOwner()->Transform()->GetRelativePos();
	physx::PxVec3 myPos = physx::PxVec3(trPos.x, trPos.y, trPos.z);
	pActor->setGlobalPose(physx::PxTransform(myPos));

	//ccd, 회전관성 0
	//GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
	//GetRigidBody()->setAngularDamping(0);

	switch (mActorType)
	{
	case ACTOR_TYPE::Static:
		break;
	case ACTOR_TYPE::Dynamic:
		ApplyGravity();
		break;
	case ACTOR_TYPE::Kinematic:
		SetSimulationShapeFlag(false);
		SetTriggerShapeFlag(true);
		break;
	}
}

void CRigidBody::DrawDebugMesh()
{
	if (mbAppliedPhysics && mbdrawCollider)
	{
		for (int i = 0; i < mShapes.size(); ++i)
		{
			if (mShapeInfos[i].eGeomType == GEOMETRY_TYPE::Box)
			{
				DrawDebugCube(GetShapePosition(i), mShapeInfos[i].size, Vec4(0.7f, 0.0f, 0.7f, 0.6f), Vec3(0.f, 0.f, 0.f), 0.f, false);
			}
			else if (mShapeInfos[0].eGeomType == GEOMETRY_TYPE::Sphere)
			{
				DrawDebugSphere(GetShapePosition(i), mShapeInfos[i].size.x / 2.f, Vec4(0.7f, 1.f, 0.7f, 0.6f), Vec3(0.f, 0.f, 0.f), 0.f, false);
			}

		}
	}
}
