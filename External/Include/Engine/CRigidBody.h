#pragma once
#include "CComponent.h"
#include "SimpleMath.h"
#include "struct.h"
#include "EnumFlags.h"


enum class FreezeRotationFlag : uint8_t
{
	ROTATION_X = (1 << 1),
	ROTATION_Y = (1 << 2),
	ROTATION_Z = (1 << 4),

	END
};

class CRigidBody : 
	public CComponent
{
public:
	CRigidBody();
	virtual ~CRigidBody();

	CLONE(CRigidBody);
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

public:
	virtual void begin() override;
	virtual void tick() override;
	virtual void finaltick() override;

public:
	virtual CComponent* CloneFromObj(CGameObject* _pGameObject);

	void Destroy();
	void SetPhysical(const tPhysicsInfo& _physicsInfo = tPhysicsInfo());
	bool IsAppliedPhysics();

	physx::PxActor* GetActor() { return mActor; }
	physx::PxRigidActor* GetRigidActor() { return mRigidActor; }

	template<typename T>
	inline T* GetActor() const
	{
		T* pActor = mActor->is<T>();
		assert(pActor);
		return pActor;
	}

	void AddActorToLevel();

	physx::PxRigidDynamic* GetDynamicActor() { return GetActor<physx::PxRigidDynamic>(); }
	physx::PxRigidStatic* GetStaticActor() { return GetActor<physx::PxRigidStatic>(); }

	physx::PxTransform GetPhysicsTransform();
	void			   SetPhysicsTransform(physx::PxTransform _transform);

	ACTOR_TYPE GetActorType() { return mPhysicsInfo.eActorType; }
	GEOMETRY_TYPE GetGeometryType() { return mPhysicsInfo.eGeomType; }
	Vector3 GetGeometrySize() { return mPhysicsInfo.size * 2.f; }
	const physx::PxFilterData& GetFilterData() { return mPhysicsInfo.filterData; }
	void SetOtherLayerInFilterData(int _eOtherLayerIDX) { mPhysicsInfo.filterData.word1 |= 1 << _eOtherLayerIDX; }

	void SetVelocity(const Vector3& _velocity);
	void SetVelocity(AXIS3D_TYPE _eAxis, float _velocity);
	void AddVelocity(const Vector3& _velocity);
	void AddVelocity(AXIS3D_TYPE _eAxis, float _velocity);
	void AddGravity();

	void SetMaxVelocity(float _maxVelocity);
	void CheckMaxVelocity();

	void ApplyGravity();
	void RemoveGravity();

	void SetFreezeRotation(FreezeRotationFlag flag, bool enable);

	void SetLinearDamping(float _damping);
	void SetSimulationShapeFlag(bool _bFlag);
	void SetTriggerShapeFlag(bool _bFlag);

	void SetActorInLevelFlag(bool _bFlag);
	bool IsActorInLevel() { return mbIsActorInLevel; }

	void AddForce(const Vector3& _force);

	void SetMass(float _mass) { if (mRigidBody) mRigidBody->setMass(_mass); }
	void SetRestitution(float _val) { if (mMaterial) mMaterial->setRestitution(_val); }
	void Test();
	physx::PxRigidBody* GetRigidBody() { return mActor->is<physx::PxRigidBody>(); }

private:
	void CreateBoxGeometry();
	void CreateCapsuleGeometry();
	void CreatePlaneGeometry();
	void CreateSphereGeometry();

private:
	void CreateGeometry();
	void CreateShape();
	void CreateActor();
	void CreateMaterial();
	void InitializeActor();

private:
	EnumFlags<FreezeRotationFlag, uint16_t> mFreezeRotationFlag;
	tPhysicsInfo mPhysicsInfo;

	physx::PxRigidBody* mRigidBody;
	physx::PxRigidActor* mRigidActor;
	physx::PxActor* mActor;
	physx::PxShape* mShape;
	physx::PxMaterial* mMaterial;

	Vector3 mVelocity;
	float mMaxVelocity;

	bool mbAppliedPhysics;
	bool mbAppliedGravity;
	bool mbIsActorInLevel;
	bool mbdrawCollider;
};

