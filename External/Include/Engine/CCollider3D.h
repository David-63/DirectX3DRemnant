#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CCollider3D :
	public CComponent
{
private:
	COLLIDER3D_TYPE mType;
	int mCollisionCount;
	Vec3 mOtherNormal;

	Ptr<CMesh> mMesh;
	Ptr<CMaterial> mMaterial;

public:
	CCollider3D();
	CCollider3D(const CCollider3D& _collider);
	CLONE(CCollider3D);
	virtual ~CCollider3D();

	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

public:
	virtual void begin() override;
	virtual void tick() override;
	virtual void finaltick() override;

public:
	void OnCollisionEnter(CCollider3D* _otherCollider);
	void OnCollisionStay(CCollider3D* _otherCollider);
	void OnCollisionExit(CCollider3D* _otherCollider);

	void OnTriggerEnter(CCollider3D* _otherCollider);
	void OnTriggerStay(CCollider3D* _otherCollider);
	void OnTriggerExit(CCollider3D* _otherCollider);

	void SetOhterNormal(Vec3 _normal) { mOtherNormal = _normal; }
	Vec3 GetOtherNormal() { return mOtherNormal; }

	bool IsCollision() { return mCollisionCount; }
	void ClearCollisionCount() { mCollisionCount = 0; }

	void SetType(COLLIDER3D_TYPE _type) { mType = _type; }
	COLLIDER3D_TYPE GetType() { return mType; }
};

