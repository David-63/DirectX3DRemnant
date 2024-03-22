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
	CRigidBody(const CRigidBody& _Other);
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

	void SetVelocity(const Vector3 _velocity);
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

	void SetSleep(bool _isSleep);
	void SetDrawCollider(bool _is) { mbdrawCollider = _is; }


	physx::PxRigidBody* GetRigidBody() { return mActor->is<physx::PxRigidBody>(); }
	physx::PxRigidActor* GetRigidActor() { return GetActor<physx::PxRigidActor>(); }


	void SetShapeLocalPos(int _idx, Vec3 _localPos);

	void SetShapeLocalPosByBone(int _idx, UINT _boneIdx);
	void SetBoneSoket(int _shapeIdx, int _boneIdx, Vec3 _offset);


	void AttachShape(int _idx);
	Vec3 GetShapePosition(int _shapeIdx);


	float GetRad() { return mShapeInfos[0].size.x / 2.f; }
	Vector3 GetShapeSize() { return mShapeInfos[0].size; }
	GEOMETRY_TYPE GetGeomType() { return mShapeInfos[0].eGeomType; }

	//IMGUI¿ë
	std::vector<tShapeInfo>* GetShapeInfos() { return &mShapeInfos; }
	Vec3 GetTempSize() { return mTempSize; }
	Vec3 GetTempOffset() { return mTempOffset; }
	void SetTempSize(Vec3 _size) { mTempSize = _size; }
	void SetTempOffset(Vec3 _offset) { mTempOffset = _offset; }
	bool GetTempGeomType() { return mTempGeomType; }
	void SetTempGeomType(bool _is) { mTempGeomType = _is; }

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
	std::vector<Geometries*> mGeometries;

	std::vector<physx::PxShape*> mShapes;
	physx::PxRigidActor* mRigidActor;
	physx::PxActor* mActor;
	physx::PxMaterial* mMaterial;

	Vector3 mVelocity;
	float mMaxVelocity;

	bool mbAppliedPhysics;
	bool mbAppliedGravity;
	bool mbIsActorInLevel;
	bool mbdrawCollider;

	Vec3 mTempSize;
	Vec3 mTempOffset;
	bool mTempGeomType; //true: Sphere, false: Box
	
	bool m_bSoketUse;
};

