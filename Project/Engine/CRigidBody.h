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
	void SetPhysical(ACTOR_TYPE _eActorType);
	void PushBackShapeInfo(const tShapeInfo _info) { mShapeInfos.push_back(_info); }
	bool IsAppliedPhysics();

	physx::PxActor* GetActor() { return mActor; }

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

	//for clone
	void SetActorType(ACTOR_TYPE _type) { mActorType = _type; }
	ACTOR_TYPE GetActorType() { return mActorType; }
	void SetShapeVector(const std::vector<physx::PxShape*>& _vector, const std::vector<tShapeInfo>& _vectorInfo) { mShapes = _vector; mShapeInfos = _vectorInfo; }

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

	
	physx::PxRigidBody* GetRigidBody() { return mActor->is<physx::PxRigidBody>(); }
	physx::PxRigidActor* GetRigidActor() { return GetActor<physx::PxRigidActor>(); }
	void SetShapeLocalPos(int _idx, CTransform* _transform);
	void SetShapeLocalPos(int _idx, Vec3 _localPos);
	void AttachShape(int _idx);
	Vec3 GetShapePosition(int _shapeIdx);


private:
	void CreateGeometry();
	void CreateShape();
	void CreateActor();
	void CreateMaterial();
	void InitializeActor();

	void DrawDebugMesh();

private:
	EnumFlags<FreezeRotationFlag, uint16_t> mFreezeRotationFlag;
	
	ACTOR_TYPE mActorType;
	std::vector<tShapeInfo> mShapeInfos;

	physx::PxRigidActor* mRigidActor;
	physx::PxActor* mActor;
	std::vector<physx::PxShape*> mShapes;
	physx::PxMaterial* mMaterial;

	Vector3 mVelocity;
	float mMaxVelocity;
	Vec3 mOffset;

	bool mbAppliedPhysics;
	bool mbAppliedGravity;
	bool mbIsActorInLevel;
	bool mbdrawCollider;
};

