#include "pch.h"
#include "CTimeMgr.h"
#include "Physics.h"
#include "CGameObject.h"
//#include "ContactCallback.h"
//#include "CRigidBody.h"

Physics::Physics()
	:mFoundation(nullptr)
	, mPhysics(nullptr)
	, mTransfort(nullptr)
	, mPvd(nullptr)
	, mSceneClient(nullptr)
	, mScene(nullptr)
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
	mScene->release();
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
	//mCallback = new ContactCallback;

	PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = mGravity;
	sceneDesc.cpuDispatcher = mCpuDispatcher;
	sceneDesc.filterShader = PlayerFilterShader;
	//sceneDesc.simulationEventCallback = mCallback;

	mScene = mPhysics->createScene(sceneDesc);
	mSceneClient = mScene->getScenePvdClient();

	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
	mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
}

void Physics::tick()
{
	mScene->simulate(CTimeMgr::GetInst()->GetDeltaTime());
	mScene->fetchResults(true);
}


void Physics::render()
{
}

void Physics::AddActor(CGameObject* _gameObject)
{
	AssertEx(_gameObject, L"Physics::AddActor() - GameObject is nullptr");
	//AssertEx(_gameObject->GetComponent<Com_RigidBody>(), L"Physics::AddActor() - RigidBody is nullptr");
	//AssertEx(_gameObject->GetComponent<Com_RigidBody>()->IsAppliedPhysics(), L"Physics::AddActor() - Is not applied physics");
	//mScene->addActor(*_gameObject->GetComponent<Com_RigidBody>()->GetActor());
}

void Physics::RemoveActor(CGameObject* _gameObject)
{
	AssertEx(_gameObject, L"Physics::RemoveActor() - GameObject is nullptr");
	//AssertEx(_gameObject->GetComponent<Com_RigidBody>(), L"Physics::RemoveActor() - RigidBody is nullptr");
	//AssertEx(_gameObject->GetComponent<Com_RigidBody>()->IsAppliedPhysics(), L"Physics::RemoveActor() - Is not applied physics");
	//mScene->removeActor(*_gameObject->GetComponent<Com_RigidBody>()->GetActor());
}

PxFilterFlags Physics::PlayerFilterShader(PxFilterObjectAttributes _attributes0, PxFilterData _filterData0, PxFilterObjectAttributes _attributes1, PxFilterData _filterData1, PxPairFlags& _pairFlags, const void* _constantBlock, PxU32 _constantBlockSize)
{
	// Ʈ���ſ� Ʈ���� �Ǵ� Ʈ���ſ� �浹�ϴ� ��ü�� �����Ͽ� ó���մϴ�.
	if (PxFilterObjectIsTrigger(_attributes0) || PxFilterObjectIsTrigger(_attributes1))
	{
		if ((_filterData1.word1 & _filterData0.word0) || (_filterData0.word1 & _filterData1.word0))
		{
			// Ʈ���ſ� �浹�ϴ� ��ü ��� OnTrigger �̺�Ʈ�� ó���ϵ��� �մϴ�.
			_pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
			return PxFilterFlag::eDEFAULT;
		}
	}

	// �浹�ϴ� ��ü�� ��� �浹 �÷��׸� �����մϴ�.
	_pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// �� ���Ͱ� ���� �浹�÷��װ� ������ ���� ���
	if ((_filterData0.word0 & _filterData1.word1) || (_filterData1.word0 & _filterData0.word1))
	{
		_pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
		return PxFilterFlag::eDEFAULT;
	}

	return PxFilterFlag::eKILL;
}
