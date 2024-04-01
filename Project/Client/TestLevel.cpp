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
#include <Script/CPathFinderScript.h>
#include <Script/CMonsterMoveScript.h>
#include <Script/CP_FSMScript.h>
#include <Script/CB_FSMScript.h>


void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);
	pCurLevel->SetName(L"TestLevel");

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer((UINT)LAYER_TYPE::Player)->SetName(L"Player");
	pCurLevel->GetLayer((UINT)LAYER_TYPE::Monster)->SetName(L"Monster");
	pCurLevel->GetLayer((UINT)LAYER_TYPE::Wall)->SetName(L"Wall");
	pCurLevel->GetLayer(4)->SetName(L"Background");
	pCurLevel->GetLayer(5)->SetName(L"Ground");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");

	CCollisionMgr::GetInst()->SetColLayer(1, 2);
	CCollisionMgr::GetInst()->SetColLayer(1, 3);
	CCollisionMgr::GetInst()->SetColLayer(1, 5);
	CCollisionMgr::GetInst()->SetColLayer(2, 3);
	CCollisionMgr::GetInst()->SetColLayer(2, 5);

	// camera
	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	//pMainCam->AddComponent(new CCameraMoveScript);
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
	// Test obj (rigidbody)
	{
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"test Obj");
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CTestScript);
		
		pObj->AddComponent(new CRigidBody);
		tShapeInfo info = {};
		info.eGeomType = GEOMETRY_TYPE::Sphere;
		info.size = Vector3(15.f, 15.f, 15.f);
		pObj->Transform()->SetRelativePos(Vec3(200.f, 100.f, 0.f));
		pObj->SetLayerIdx(0);
		pObj->RigidBody()->PushBackShapeInfo(info);
		pObj->RigidBody()->SetPhysical(ACTOR_TYPE::Dynamic);
		pObj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
		pObj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
		pObj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
		pObj->RigidBody()->GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		
		SpawnGameObject(pObj, Vec3(200.f, 100.f, 0.f), 0);
	}

	{
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"Particle Maker");
		pObj->AddComponent(new CTransform());
		pObj->AddComponent(new CParticleSystem());
		pObj->Transform()->SetDebugSphereUse(true);
		SpawnGameObject(pObj, Vec3(200.f, 100.f, 0.f), 0);
	}
	//{
	

		//Ptr<CMeshData> pMeshData = nullptr;
		//CGameObject* player = nullptr;

		/////pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\P_R2ImpactStagger.fbx");
		//pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\P_R2Idle.mdat");
		//player = pMeshData->Instantiate();

		//player->SetName(L"Player");
		//player->MeshRender()->SetFrustumCheck(false);
		//CP_FSMScript* pFSM = new CP_FSMScript();
		//player->AddComponent(pFSM);

		//// rigidbody 에 전달할 데이터 미리 초기화
		//Vec3 playerStartPos = Vec3(0, 100.f, 0);
		//player->Transform()->SetRelativePos(playerStartPos);
		//player->SetLayerIdx((UINT)LAYER_TYPE::Player);

		////// 쉐이프 정의 및 등록
		//player->AddComponent(new CRigidBody);
		//tShapeInfo info = {};								// foot
		//info.eGeomType = GEOMETRY_TYPE::Sphere;
		//info.size = Vector3(15.f, 15.f, 15.f);
		//player->RigidBody()->PushBackShapeInfo(info);
		//info = {};											// head
		//info.eGeomType = GEOMETRY_TYPE::Sphere;
		//info.size = Vector3(35.f, 35.f, 35.f);
		//player->RigidBody()->PushBackShapeInfo(info);
		//info = {};											// body
		//info.eGeomType = GEOMETRY_TYPE::Sphere;
		//info.size = Vector3(55.f, 55.f, 55.f);
		//player->RigidBody()->PushBackShapeInfo(info);
		//// 피지컬 등록하기
		//player->RigidBody()->SetPhysical(ACTOR_TYPE::Dynamic);

		//player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
		//player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
		//player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
		//player->RigidBody()->GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		//
		//////////////////////////
		////player->RigidBody()->SetShapeLocalPos(0, Vec3(5.f, 7.5f, 0.f));

		////player->AddComponent(new CCollider3D);
		////player->Collider3D()->SetType(COLLIDER3D_TYPE::Player);

		//SpawnGameObject(player, Vec3(0,0,0), (UINT)LAYER_TYPE::Player);
	//}

	{
		CGameObject* pGround = new CGameObject;
		pGround->SetName(L"Ground");
		pGround->AddComponent(new CTransform);
		pGround->Transform()->SetRelativeScale(10000.f, 10.f, 10000.f);
		pGround->SetLayerIdx((UINT)LAYER_TYPE::Ground);
		pGround->Transform()->SetRelativePos(Vec3(0.f, -500.f, 0.f));

		pGround->AddComponent(new CCollider3D);
		pGround->AddComponent(new CRigidBody);
		tShapeInfo info = {};
		info.eGeomType = GEOMETRY_TYPE::Box;
		info.size = Vector3(10000.f, 1000.f, 10000.f);
		info.massProperties.restitution = 0.2f;
		info.massProperties.dynamicFriction = 0.3f;
		info.massProperties.staticFriction = 0.3f;
		pGround->RigidBody()->PushBackShapeInfo(info);
		pGround->RigidBody()->SetPhysical(ACTOR_TYPE::Static);

		pGround->AddComponent(new CMeshRender);
		Ptr<CMesh> mesh = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_Debug");
		Ptr<CMaterial> mater = CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl");
		pGround->MeshRender()->SetMesh(mesh);
		pGround->MeshRender()->SetMaterial(mater, 0);

		SpawnGameObject(pGround, Vec3(0.f, -500.f, 0.f), (UINT)LAYER_TYPE::Ground);
	}
}
