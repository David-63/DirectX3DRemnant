#include "pch.h"
#include "Physics.h"
#include "global.h"
#include "define.h"
#include "CGameObject.h"
#include "CColCallBack.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"


Physics::Physics()
	:mFoundation(nullptr)
	, mPhysics(nullptr)
	, mTransfort(nullptr)
	, mPvd(nullptr)
	, mSceneClient(nullptr)
	, mCurScene(nullptr)
	, mControllerMgr(nullptr)
	, mCpuDispatcher(nullptr)
	, mCallback(nullptr)
	, mGravity(0.f, -9.81f, 0.f)
{
}
Physics::~Physics()
{
	mCpuDispatcher->release();

	delete mCallback;
	mCallback = nullptr;

	mCurScene->release();
	mPhysics->release();
	mPvd->release();
	mTransfort->release();
	mFoundation->release();
}
void Physics::init()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocCallback, mErrorCallback);

	mTransfort = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	mPvd = PxCreatePvd(*mFoundation);
	mPvd->connect(*mTransfort, PxPvdInstrumentationFlag::eALL);

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), false, mPvd);

	mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	mCallback = new CColCallBack;

	PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(mGravity.x, mGravity.y, mGravity.z);
	sceneDesc.cpuDispatcher = mCpuDispatcher;
	sceneDesc.filterShader = PlayerFilterShader;
	sceneDesc.simulationEventCallback = mCallback;
	sceneDesc.flags = PxSceneFlag::eENABLE_CCD;

	mCurScene = mPhysics->createScene(sceneDesc);

	mSceneClient = mCurScene->getScenePvdClient();

	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);

}
void Physics::tick()
{
	mCurScene->simulate(CTimeMgr::GetInst()->GetDeltaTime());
	mCurScene->fetchResults(true);


}
void Physics::render()
{
}
void Physics::AddActor(CGameObject* _gameObject)
{
	AssertEx(_gameObject, L"Physics::AddActor() - GameObject is nullptr");
	AssertEx(_gameObject->RigidBody(), L"Physics::AddActor() - RigidBody is nullptr");
	AssertEx(_gameObject->RigidBody()->IsAppliedPhysics(), L"Physics::AddActor() - Is not applied physics");
	mCurScene->addActor(*_gameObject->RigidBody()->GetActor());
}
void Physics::RemoveActor(CGameObject* _gameObject)
{
	AssertEx(_gameObject, L"Physics::RemoveActor() - GameObject is nullptr");
	AssertEx(_gameObject->RigidBody(), L"Physics::RemoveActor() - RigidBody is nullptr");
	AssertEx(_gameObject->RigidBody()->IsAppliedPhysics(), L"Physics::RemoveActor() - Is not applied physics");
	mCurScene->removeActor(*_gameObject->RigidBody()->GetActor());
}
tRayCastInfo* Physics::RayCast(Vec3 _rayOrigin, Vec3 _rayDirection, float _rayLength)
{
	tRayCastInfo info = {};
	PxVec3 rayOrigin = { _rayOrigin.x, _rayOrigin.y, _rayOrigin.z };
	PxVec3 rayDir = { _rayDirection.x, _rayDirection.y, _rayDirection.z };
	
	PxRaycastBuffer hitBuffer;
	PxQueryFilterData data;
	//data.data.word0 = 0x01;
	
	bool hit = mCurScene->raycast(rayOrigin, rayDir, _rayLength, hitBuffer);

	if (hit)
	{
		PxRaycastHit result = hitBuffer.getAnyHit(0);
		info.hitActor = result.actor;
		info.hitPos = { result.position.x, result.position.y, result.position.z };
		info.hitNormal = { result.normal.x, result.normal.y, result.normal.z };
		info.dist = result.distance;
		info.hit = true;
	}

	else
	{
		info.hit = false;
	}

	return &info;
}
PxFilterFlags Physics::PlayerFilterShader(PxFilterObjectAttributes _attributes0, PxFilterData _filterData0, PxFilterObjectAttributes _attributes1, PxFilterData _filterData1, PxPairFlags& _pairFlags, const void* _constantBlock, PxU32 _constantBlockSize)
{
	// 트리거와 트리거 또는 트리거와 충돌하는 물체를 구분하여 처리합니다.
	if (PxFilterObjectIsTrigger(_attributes0) || PxFilterObjectIsTrigger(_attributes1))
	{
		if ((_filterData1.word1 & _filterData0.word0) || (_filterData0.word1 & _filterData1.word0))
		{
			// 트리거와 충돌하는 물체 모두 OnTrigger 이벤트를 처리하도록 합니다.
			_pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
			return PxFilterFlag::eDEFAULT;
		}
	}

	// 충돌하는 물체의 경우 충돌 플래그만 생성합니다.
	_pairFlags |= PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_CONTACT_POINTS;

	// 두 필터가 서로 충돌플래그가 세워져 있을 경우
	if ((_filterData0.word0 & _filterData1.word1) || (_filterData1.word0 & _filterData0.word1))
	{
		_pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST;
		return PxFilterFlag::eDEFAULT;
	}

	return PxFilterFlag::eKILL;
}


