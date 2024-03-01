#include "pch.h"
#include "TestLevel.h"
#include "CLevelSaveLoad.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine/CResMgr.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CRigidBody.h>
#include <Engine/Physics.h>
#include <Engine/CPrefab.h>

// Scripts
#include <Script/CCameraMoveScript.h>
#include <Script/CTestScript.h>
#include <Script/CShoulderViewScript.h>
#include <Script/CCharacterMoveScript.h>


void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);
	pCurLevel->SetName(L"TestLevel");

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Background");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Objects");
	pCurLevel->GetLayer(4)->SetName(L"Ground");
	pCurLevel->GetLayer(5)->SetName(L"Box");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");

	CCollisionMgr::GetInst()->SetColLayer(2, 4);
	CCollisionMgr::GetInst()->SetColLayer(2, 2);
	CCollisionMgr::GetInst()->SetColLayer(4, 5);
	CCollisionMgr::GetInst()->SetColLayer(2, 5);
	CCollisionMgr::GetInst()->SetColLayer(5, 5);

	// camera
	
		// Main Camera Object 생성
		CGameObject* pMainCam = new CGameObject;
		pMainCam->SetName(L"MainCamera");

		pMainCam->AddComponent(new CTransform);
		pMainCam->AddComponent(new CCamera);
		pMainCam->AddComponent(new CCameraMoveScript);
		
		
		// 숄더뷰용
		//pMainCam->AddComponent(new CShoulderViewScript);
		//CShoulderViewScript* pCamMoveScript = pMainCam->GetScript<CShoulderViewScript>(); 

		pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
		pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
		pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.
		pMainCam->Camera()->SetFar(20000.f);
		SpawnGameObject(pMainCam, Vec3(200.f, 133.f, -500.f), 0);

		// UI cameara
		CGameObject* pUICam = new CGameObject;
		pUICam->SetName(L"UICamera");

		pUICam->AddComponent(new CTransform);
		pUICam->AddComponent(new CCamera);

		pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		pUICam->Camera()->SetCameraIndex(1);		// Sub 카메라로 지정
		pUICam->Camera()->SetLayerMask(31, true);	// 31번 레이어만 체크
		SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);


	//


	// SkyBox 추가
	{
		CGameObject* pSkyBox = new CGameObject;
		pSkyBox->SetName(L"SkyBox");

		pSkyBox->AddComponent(new CTransform);
		pSkyBox->AddComponent(new CSkyBox);

		pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100));
		pSkyBox->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::CUBE);
		pSkyBox->SkyBox()->SetSkyBoxTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\SkyWater.dds"));

		SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), 0);
	}

	// 광원 추가
	{
		CGameObject* pLightObj = new CGameObject;
		pLightObj->SetName(L"Directional Light");

		pLightObj->AddComponent(new CTransform);
		pLightObj->AddComponent(new CLight3D);

		pLightObj->Transform()->SetRelativeRot(Vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));
		pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
		pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
		pLightObj->Light3D()->SetLightAmbient(Vec3(0.5f, 0.5f, 0.5f));

		SpawnGameObject(pLightObj, Vec3(-2000, 2000.f, -2000.f), 0);
	}

	// ============
	// FBX Loading
	// ============	
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;

		// 인스턴싱 테스트
		//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\house.fbx");
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\monster.fbx");
		//pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\monster.mdat");
		pObj = pMeshData->Instantiate();
		pObj->AddComponent(new CTestScript());

		pObj->SetName(L"Player");
		pObj->Transform()->SetDebugSphereUse(true);
		pObj->SetLayerIdx(2);
		pObj->Transform()->SetRelativePos(Vec3(40.f, 100.f, 40.f));
		pObj->AddComponent(new CRigidBody);

		tShapeInfo info = {};
		info.eGeomType = GEOMETRY_TYPE::Sphere;
		info.size = Vector3(15.f, 15.f, 15.f);
		info.massProperties.restitution = 0.99f;
		pObj->RigidBody()->PushBackShapeInfo(info);

		tShapeInfo info2 = {};
		info2.eGeomType = GEOMETRY_TYPE::Sphere;
		info2.size = Vector3(15.f, 15.f, 15.f);
		info2.massProperties.restitution = 0.99f;
		pObj->RigidBody()->PushBackShapeInfo(info2);

		tShapeInfo info3 = {};
		info3.eGeomType = GEOMETRY_TYPE::Sphere;
		info3.size = Vector3(8.f, 8.f, 8.f);
		info3.massProperties.restitution = 0.99f;
		pObj->RigidBody()->PushBackShapeInfo(info3);

		pObj->RigidBody()->SetPhysical(ACTOR_TYPE::Dynamic);
		pObj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
		pObj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
		pObj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
		pObj->RigidBody()->GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);

		int num = pObj->RigidBody()->GetRigidActor()->getNbShapes();
		pObj->RigidBody()->SetShapeLocalPos(0, Vec3(5.f, 7.5f, 0.f));
		pObj->RigidBody()->SetShapeLocalPos(1, Vec3(5.f, 22.5f, 0.f));
		pObj->RigidBody()->SetShapeLocalPos(2, Vec3(5.f, 34.f, 0.f));

		pObj->AddComponent(new CCollider3D);
		pObj->Collider3D()->SetType(COLLIDER3D_TYPE::Player);
		pObj->RigidBody()->AddActorToLevel();

		SpawnGameObject(pObj, Vec3(40.f, 100.f, 40.f), 2);
	}
	//prefab
	{
		/*Ptr<CPrefab> fab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\phsxT.pref");
		
		CGameObject* cloneObj = fab->Instantiate(Vec3(300.f, 0.f, 400.f), 2);
		CGameObject* cloneObj2 = fab->Instantiate(Vec3(300.f, 0.f, 400.f), 2);
		
		SpawnGameObject(cloneObj, Vec3(300.f, 100.f, 400.f), L"Player");
		SpawnGameObject(cloneObj2, Vec3(300.f, 100.f, 400.f), L"Player");*/
	}
	//ground
	{
		CGameObject* pGround = new CGameObject;
		pGround->SetName(L"Ground");
		pGround->AddComponent(new CTransform);
		pGround->Transform()->SetRelativeScale(10000.f, 10.f, 10000.f);
		pGround->SetLayerIdx(5);
		pGround->Transform()->SetRelativePos(Vec3(0.f, -5.f, 0.f));


		pGround->AddComponent(new CCollider3D);
		pGround->AddComponent(new CRigidBody);
		tShapeInfo info = {};
		info.eGeomType = GEOMETRY_TYPE::Box;
		info.size = Vector3(10000.f, 10.f, 10000.f);
		info.massProperties.restitution = 0.2f;
		info.massProperties.dynamicFriction = 0.3f;
		info.massProperties.staticFriction = 0.3f;
		pGround->RigidBody()->PushBackShapeInfo(info);

		pGround->RigidBody()->SetPhysical(ACTOR_TYPE::Static);
		pGround->RigidBody()->AddActorToLevel();

		pGround->AddComponent(new CMeshRender);
		Ptr<CMesh> mesh = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_Debug");
		pGround->MeshRender()->SetMesh(mesh);
		Ptr<CMaterial> mater = CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl");
		pGround->MeshRender()->SetMaterial(mater, 0);

		PxVec3 pos = pGround->Transform()->GetPhysicsPosition();

		SpawnGameObject(pGround, Vec3(0.f, -5.f, 0.f), 4);
	}

	bool hit = Physics::GetInst()->RayCast(Vec3(500.f, 100.f, 500.f), Vec3(0.f, 0.f, -1.f), 1000.f);

	// LandScape Object
	{
		CGameObject* pLandScape = new CGameObject;
		pLandScape->SetName(L"LandScape");
		pLandScape->AddComponent(new CTransform);
		pLandScape->AddComponent(new CLandScape);

		pLandScape->Transform()->SetRelativeScale(Vec3(1000.f, 4000.f, 1000.f));
		//pLandScape->LandScape()->SetHeightMap(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\HeightMap_01.jpg"));

		SpawnGameObject(pLandScape, Vec3(0.f, 0.f, 0.f), 4);
	}
	//파티클
	{
		CGameObject* pParticleObj = new CGameObject;
		pParticleObj->SetName(L"Particle");

		pParticleObj->AddComponent(new CTransform);
		pParticleObj->AddComponent(new CParticleSystem);

		SpawnGameObject(pParticleObj, Vec3(0.f, 0.f, 0.f), 0);
	}


}
