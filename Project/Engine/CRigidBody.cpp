#include "pch.h"
#include "CRigidBody.h"
#include "Physics.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CCollisionMgr.h"

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
	, mOffset(0.f, 0.f, 0.f)
{
}
CRigidBody::~CRigidBody()
{
	for (int i = 0; i < mShapeInfos.size(); ++i)
	{
		delete mShapeInfos[i].pGeometries;
		mShapeInfos[i].pGeometries = nullptr;
	}
}
void CRigidBody::SaveToLevelFile(FILE* _File)
{
	fwrite(&mFreezeRotationFlag, sizeof(EnumFlags<FreezeRotationFlag, uint16_t>), 1, _File);
	fwrite(&mActorType, sizeof(ACTOR_TYPE), 1, _File);
	fwrite(&mShapeInfos, sizeof(std::vector<tShapeInfo>), 1, _File);
	fwrite(&mShapes, sizeof(std::vector<physx::PxShape*>), 1, _File);
}
void CRigidBody::LoadFromLevelFile(FILE* _File)
{
	fread(&mFreezeRotationFlag, sizeof(EnumFlags<FreezeRotationFlag, uint16_t>), 1, _File);
	fread(&mActorType, sizeof(ACTOR_TYPE), 1, _File);
	fread(&mShapeInfos, sizeof(std::vector<tShapeInfo>), 1, _File);
	fread(&mShapes, sizeof(std::vector<physx::PxShape*>), 1, _File);
}
void CRigidBody::begin()
{
	if (true == mbAppliedPhysics &&
		false == mbIsActorInLevel &&
		nullptr != mActor)
		Physics::GetInst()->AddActor(GetOwner());
}
void CRigidBody::tick()
{
}
void CRigidBody::finaltick()
{
	if (true == mbAppliedGravity && false == mbAppliedPhysics)
	{
		AddGravity();
	}

	DrawDebugMesh();

	if (true == mbAppliedPhysics && ACTOR_TYPE::Static == mActorType)
		return;
	else
	{
		GetOwner()->Transform()->Move(mVelocity);
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

	CreateMaterial();
	CreateGeometry();
	CreateActor();
	CreateShape();

	mbAppliedPhysics = true;
	InitializeActor();
}
bool CRigidBody::IsAppliedPhysics()
{
	return mbAppliedPhysics;
}
void CRigidBody::AddActorToLevel()
{
	AssertEx(mbAppliedPhysics, L"RigidBody::AddActorToLevel() - 물리가 들어가지 않은 오브젝트에 대한 AddActorToScene 호출");
	AssertEx(mActor, L"RigidBody::AddActorToLevel() - mpActor가 생성되지 않음");

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
void CRigidBody::SetVelocity(const Vector3& _velocity)
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
		GetDynamicActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
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
	for (int i = 0; i <mShapes.size(); ++i)
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

		rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, true);
		rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
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


void CRigidBody::SetShapeLocalPos(int _idx, CTransform* _transform)
{
	Vec3 vecPos = _transform->GetRelativePos();
	Vec3 vecRot = _transform->GetRelativeRot();
	XMFLOAT4 quat = Transform()->EulerToQuat(vecRot.x, vecRot.y, vecRot.z);

	PxTransform pxTransform;
	pxTransform.p.x = vecPos.x;
	pxTransform.p.y = vecPos.y;
	pxTransform.p.z = vecPos.z;

	pxTransform.q.x = quat.x;
	pxTransform.q.y = quat.y;
	pxTransform.q.z = quat.z;
	pxTransform.q.w = quat.w;

	mShapes[_idx]->setLocalPose(pxTransform);
}

void CRigidBody::SetShapeLocalPos(int _idx, Vec3 _localPos)
{
	PxTransform localpose(PxVec3(_localPos.x, _localPos.y, _localPos.z));
	localpose.q = GetPhysicsTransform().q;

	mShapes[_idx]->setLocalPose(localpose);
}

void CRigidBody::AttachShape(int _idx)
{
	GetRigidActor()->attachShape(*mShapes[_idx]);
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


void CRigidBody::CreateGeometry()
{
	for (int i = 0; i < mShapeInfos.size(); ++i)
	{
		mShapeInfos[i].size /= 2.f;
		switch (mShapeInfos[i].eGeomType)
		{
		case GEOMETRY_TYPE::Box:
		{
			mShapeInfos[i].pGeometries = new Geometries(mShapeInfos[i].eGeomType, mShapeInfos[i].size);
			GetOwner()->Transform()->SetRelativeScale(mShapeInfos[i].size * 2);
		}
		break;

		case GEOMETRY_TYPE::Capsule:
			mShapeInfos[i].pGeometries = new Geometries(mShapeInfos[i].eGeomType, mShapeInfos[i].size.x, mShapeInfos[i].size.y);
			break;

		case GEOMETRY_TYPE::Sphere:
			mShapeInfos[i].pGeometries = new Geometries(mShapeInfos[i].eGeomType, mShapeInfos[i].size.x);
			break;

		case GEOMETRY_TYPE::Plane:
			mShapeInfos[i].pGeometries = new Geometries(mShapeInfos[i].eGeomType);
			break;
		}

		AssertEx(mShapeInfos[i].pGeometries, L"RigidBody::CreateGeometry() - Geometry 생성 실패");
	}
}
void CRigidBody::CreateShape()
{
	for(tShapeInfo& info : mShapeInfos)
	{
		PxShape* shape = {};
		switch (info.eGeomType)
		{
		case GEOMETRY_TYPE::Box:
			shape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), info.pGeometries->boxGeom, *mMaterial);
			break;
		case GEOMETRY_TYPE::Capsule:
			shape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), info.pGeometries->capsuleGeom, *mMaterial);
			break;
		case GEOMETRY_TYPE::Sphere:
			shape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), info.pGeometries->sphereGeom, *mMaterial);
			break;
		case GEOMETRY_TYPE::Plane:
			shape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), info.pGeometries->planeGeom, *mMaterial);
			break;
		}


		AssertEx(shape, L"RigidBody::CreateShape() - Shape 생성 실패");
		mShapes.push_back(shape);
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
	}
	

	Vector3 trPos = GetOwner()->Transform()->GetRelativePos();
	physx::PxVec3 myPos = physx::PxVec3(trPos.x, trPos.y, trPos.z);
	pActor->setGlobalPose(physx::PxTransform(myPos));


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
				DrawDebugCube(GetShapePosition(i), mShapeInfos[i].size * 2, Vec4(0.7f, 0.0f, 0.7f, 0.6f), Vec3(0.f, 0.f, 0.f), 0.f, true);
			}
			else if (mShapeInfos[0].eGeomType == GEOMETRY_TYPE::Sphere)
			{
				DrawDebugSphere(GetShapePosition(i), mShapeInfos[i].size.x, Vec4(0.7f, 1.f, 0.7f, 0.6f), Vec3(0.f, 0.f, 0.f), 0.f, true);
			}

		}
	}
}
