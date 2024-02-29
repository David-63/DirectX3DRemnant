#pragma once
#include <PhysX/PxPhysicsAPI.h>
#include "SimpleMath.h"

using namespace physx;

class CGameObject;
class Physics :
	public CSingleton<Physics>
{
public:
	Physics();
	~Physics();

public:
	void init();
	void tick();
	void render();

public:
	void AddActor(CGameObject* _gameObject);
	void RemoveActor(CGameObject* _gameObject);

	const Vector3& GetGravity() { return mGravity; }
	PxPhysics* GetPhysics() { return mPhysics; }
	tRayCastInfo* RayCast(Vec3 _rayOrigin, Vec3 _rayDirection, float _rayLength);
	void ReleaseAndCreatePxScene();

	static PxFilterFlags PlayerFilterShader(
		PxFilterObjectAttributes _attributes0, PxFilterData _filterData0,
		PxFilterObjectAttributes _attributes1, PxFilterData _filterData1,
		PxPairFlags& _pairFlags, const void* _constantBlock, PxU32 _constantBlockSize);

private:
	PxDefaultAllocator		mAllocCallback;
	PxDefaultErrorCallback  mErrorCallback;
	PxFoundation* mFoundation;
	PxPhysics* mPhysics;

	PxPvdTransport* mTransfort;
	PxPvd* mPvd;
	PxPvdSceneClient* mSceneClient;
	PxScene* mCurScene;
	std::vector<PxScene*> m_vPxScenes;
	PxControllerManager* mControllerMgr;

	PxDefaultCpuDispatcher* mCpuDispatcher;
	class CColCallBack* mCallback;

	Vector3 mGravity;

	
};



