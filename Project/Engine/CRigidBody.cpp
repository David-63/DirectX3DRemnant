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
	, mShape(nullptr)
	, mMaterial(nullptr)
	, mbAppliedPhysics(false)
	, mbAppliedGravity(false)
	, mbIsActorInLevel(false)
	, mbdrawCollider(true)
{
}
CRigidBody::~CRigidBody()
{
	delete mPhysicsInfo.pGeometries;
	mPhysicsInfo.pGeometries = nullptr;
}
void CRigidBody::SaveToLevelFile(FILE* _File)
{
}
void CRigidBody::LoadFromLevelFile(FILE* _File)
{
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

	if (mbAppliedPhysics && mbdrawCollider)
	{
		if (mPhysicsInfo.eGeomType == GEOMETRY_TYPE::Box)
		{
			DrawDebugCube(GetOwner()->Transform()->GetWorldPos(), mPhysicsInfo.size*2, Vec4(0.7f, 0.0f, 0.7f, 0.6f), Vec3(0.f, 0.f, 0.f), 0.f, true);
		}
		else if (mPhysicsInfo.eGeomType == GEOMETRY_TYPE::Sphere)
		{
			DrawDebugSphere(GetOwner()->Transform()->GetWorldPos(), mPhysicsInfo.size.x, Vec4(0.7f, 1.f, 0.7f, 0.6f), Vec3(0.f, 0.f, 0.f), 0.f, true);
		}
	}

	if (true == mbAppliedPhysics && ACTOR_TYPE::Static == mPhysicsInfo.eActorType)
		return;
	else
	{
		CTransform* tr = (CTransform*)GetOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM);
		tr->Move(mVelocity);
	}

	GetOwner()->Transform()->Move(mVelocity);

	


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
		tPhysicsInfo info = {};
		info.size = GetGeometrySize();
		info.eActorType = mPhysicsInfo.eActorType;
		info.eGeomType = mPhysicsInfo.eGeomType;
		info.massProperties = mPhysicsInfo.massProperties;

		rigidBody->SetPhysical(info);
		_pGameObject->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
		_pGameObject->Transform()->SetRelativeRot(GetOwner()->Transform()->GetRelativeRot());
		_pGameObject->Transform()->SetRelativeScale(GetOwner()->Transform()->GetRelativeScale());
	}

	if (ACTOR_TYPE::Static != mPhysicsInfo.eActorType)
	{
		rigidBody->SetVelocity(mVelocity);
		rigidBody->SetMaxVelocity(mMaxVelocity);
	}

	return rigidBody;
}
void CRigidBody::SetPhysical(const tPhysicsInfo& _physicsInfo)
{
	mPhysicsInfo = _physicsInfo;

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
		AssertEx(ACTOR_TYPE::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::SetVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
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
		AssertEx(ACTOR_TYPE::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::SetVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
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
		AssertEx(ACTOR_TYPE::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::AddVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
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
		AssertEx(ACTOR_TYPE::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::AddVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
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
		AssertEx(ACTOR_TYPE::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::SetMaxVelocity() - Dynamic Actor가 아닌 물체에 대한 SetMaxVelocity() 호출 시도");
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
		AssertEx(ACTOR_TYPE::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::ApplyGravity() - Dynamic Actor가 아닌 물체에 대한 ApplyGravity() 호출 시도");
		GetDynamicActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
	}

	mbAppliedGravity = true;
}
void CRigidBody::RemoveGravity()
{
	if (true == mbAppliedPhysics)
	{
		AssertEx(ACTOR_TYPE::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::RemoveGravity() - Dynamic Actor가 아닌 물체에 대한 RemoveGravity() 호출 시도");
		GetDynamicActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	}

	mbAppliedGravity = false;
}
void CRigidBody::SetFreezeRotation(FreezeRotationFlag flag, bool enable)
{
	physx::PxActor* actor = mShape->getActor();
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

void CRigidBody::SetLinearDamping(float _damping)
{
	AssertEx(ACTOR_TYPE::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::SetLinearDamping() - Dynamic Actor가 아닌 물체에 대한 SetLinearDamping() 호출 시도");
	GetActor<physx::PxRigidDynamic>()->setLinearDamping(_damping);
}
void CRigidBody::SetSimulationShapeFlag(bool _bFlag)
{
	AssertEx(mbAppliedPhysics, L"RigidBody::SetSimulationShapeFlag() - 물리가 들어가지 않은 오브젝트에 대한 SetSimulationShapeFlag 호출");
	mShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, _bFlag);
}
void CRigidBody::SetTriggerShapeFlag(bool _bFlag)
{
	AssertEx(mbAppliedPhysics, L"RigidBody::SetTriggerShapeFlag() - 물리가 들어가지 않은 오브젝트에 대한 SetTriggerShapeFlag 호출");
	mShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, _bFlag);
}
void CRigidBody::SetActorInLevelFlag(bool _bFlag)
{
	mbIsActorInLevel = _bFlag;
}
void CRigidBody::AddForce(const Vector3& _force)
{
	AssertEx(mbAppliedPhysics, L"RigidBody::AddForce() - 물리가 들어가지 않은 오브젝트에 대한 AddForce 호출");
	AssertEx(ACTOR_TYPE::Static != mPhysicsInfo.eActorType, L"RigidBody::AddForce() - Static 객체에 대해 힘 적용");

	PxVec3 pxForce = PxVec3(_force.x, _force.y, _force.z);

	PxRigidBodyExt::addForceAtPos(
		*GetDynamicActor(),
		pxForce,
		GetOwner()->Transform()->GetPhysicsPosition(),
		physx::PxForceMode::eFORCE
	);
}

void CRigidBody::Test()
{
	//mRigidActor->
	//mActor->
}

void CRigidBody::CreateBoxGeometry()
{
	mPhysicsInfo.pGeometries = new Geometries(mPhysicsInfo.eGeomType, mPhysicsInfo.size);
}
void CRigidBody::CreateCapsuleGeometry()
{
	mPhysicsInfo.pGeometries = new Geometries(mPhysicsInfo.eGeomType, mPhysicsInfo.size.x, mPhysicsInfo.size.y);
}
void CRigidBody::CreatePlaneGeometry()
{
	mPhysicsInfo.pGeometries = new Geometries(mPhysicsInfo.eGeomType);
}
void CRigidBody::CreateSphereGeometry()
{
	mPhysicsInfo.pGeometries = new Geometries(mPhysicsInfo.eGeomType, mPhysicsInfo.size.x);
}
void CRigidBody::CreateGeometry()
{
	mPhysicsInfo.size /= 2.f;
	switch (mPhysicsInfo.eGeomType)
	{
	case GEOMETRY_TYPE::Box:
	{
		CreateBoxGeometry();
	}
		break;

	case GEOMETRY_TYPE::Capsule:
		CreateCapsuleGeometry();
		break;

	case GEOMETRY_TYPE::Sphere:
	{
		CreateSphereGeometry();
	}
		break;

	case GEOMETRY_TYPE::Plane:
		CreatePlaneGeometry();
		break;
	}

	AssertEx(mPhysicsInfo.pGeometries, L"RigidBody::CreateGeometry() - Geometry 생성 실패");
}
void CRigidBody::CreateShape()
{
	switch (mPhysicsInfo.eGeomType)
	{
	case GEOMETRY_TYPE::Box:
		mShape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mPhysicsInfo.pGeometries->boxGeom, *mMaterial);
		break;
	case GEOMETRY_TYPE::Capsule:
		mShape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mPhysicsInfo.pGeometries->capsuleGeom, *mMaterial);
		break;
	case GEOMETRY_TYPE::Sphere:
		mShape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mPhysicsInfo.pGeometries->sphereGeom, *mMaterial);
		break;
	case GEOMETRY_TYPE::Plane:
		mShape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mPhysicsInfo.pGeometries->planeGeom, *mMaterial);
		break;
	}

	AssertEx(mShape, L"RigidBody::CreateShape() - Shape 생성 실패");
}
void CRigidBody::CreateActor()
{
	float _x = GetOwner()->Transform()->GetRelativePos().x;
	float _y = GetOwner()->Transform()->GetRelativePos().y;
	float _z = GetOwner()->Transform()->GetRelativePos().z;
	switch (mPhysicsInfo.eActorType)
	{
	case ACTOR_TYPE::Dynamic:
		mRigidBody = Physics::GetInst()->GetPhysics()->
			createRigidDynamic(physx::PxTransform(PxVec3(_x, _y, _z )));
		mActor = mRigidBody;
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
	mMaterial = Physics::GetInst()->GetPhysics()->createMaterial(mPhysicsInfo.massProperties.staticFriction,
		mPhysicsInfo.massProperties.dynamicFriction,
		mPhysicsInfo.massProperties.restitution);
	mMaterial->setRestitution(0.f);
}
void CRigidBody::InitializeActor()
{
	physx::PxRigidActor* pActor = mActor->is<physx::PxRigidActor>();
	pActor->userData = GetOwner();

	int layerIdx = GetOwner()->GetLayerIndex();
	mPhysicsInfo.filterData.word0 = 1 << layerIdx;

	std::array<UINT, MAX_LAYER> collisionGroup = CCollisionMgr::GetInst()->GetMat();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		int mask = 0;
		mask = 1 << i;
		if((collisionGroup[layerIdx] & mask) != 0)
			mPhysicsInfo.filterData.word1 |= 1 << i;
	}

	mShape->setSimulationFilterData(mPhysicsInfo.filterData);
	Vector3 trPos = GetOwner()->Transform()->GetRelativePos();
	physx::PxVec3 myPos = physx::PxVec3(trPos.x, trPos.y, trPos.z);
	pActor->setGlobalPose(physx::PxTransform(myPos));

	switch (mPhysicsInfo.eActorType)
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