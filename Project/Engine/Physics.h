#pragma once
#include <PhysX/PxPhysicsAPI.h>
<<<<<<< Updated upstream
#include "global.h"
=======
>>>>>>> Stashed changes
#include "SimpleMath.h"

using namespace physx;

class Physics
{
private:
	PxDefaultAllocator		mAllocCallback;
	PxDefaultErrorCallback  mErrorCallback;
	PxFoundation* mFoundation;
	PxPhysics* mPhysics;

	PxPvdTransport* mTransfort;
	PxPvd* mPvd;
	PxPvdSceneClient* mSceneClient;
	PxScene* mScene;
	PxControllerManager* mControllerMgr;

	PxDefaultCpuDispatcher* mCpuDispatcher;
	class ContactCallback* mCallback;

	PxVec3 mGravity;


public:
	void init();
	void tick();
	void render();

public:
	void AddActor(CGameObject* _gameObject);
	void RemoveActor(CGameObject* _gameObject);

	const PxVec3& GetGravity() { return mGravity; }
	PxPhysics* GetPhysics() { return mPhysics; }

	static PxFilterFlags PlayerFilterShader(
		PxFilterObjectAttributes _attributes0, PxFilterData _filterData0,
		PxFilterObjectAttributes _attributes1, PxFilterData _filterData1,
		PxPairFlags& _pairFlags, const void* _constantBlock, PxU32 _constantBlockSize);

<<<<<<< Updated upstream

public:
	Physics();
	~Physics();
=======
public:
	Physics();
	~Physics();


>>>>>>> Stashed changes
};

