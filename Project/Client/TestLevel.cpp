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
	{
		// Main Camera Object 생성
		CGameObject* pMainCam = new CGameObject;
		pMainCam->SetName(L"MainCamera");

		pMainCam->AddComponent(new CTransform);
		pMainCam->AddComponent(new CCamera);
		pMainCam->AddComponent(new CCameraMoveScript);

		/*
		// 숄더뷰용
		pMainCam->AddComponent(new CShoulderViewScript);
		CShoulderViewScript* pCamMoveScript = pMainCam->GetScript<CShoulderViewScript>(); */

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
	}

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
		pLightObj->Light3D()->SetLightAmbient(Vec3(0.15f, 0.15f, 0.15f));

		SpawnGameObject(pLightObj, Vec3(-2000, 2000.f, -2000.f), 0);
	}

	//============
	//FBX Loading
	//============	
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;		
		// 인스턴싱 테스트
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\P_WalkF.fbx");
		//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\monster.fbx");
		//pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\monster.mdat");		

		for (int i = 0; i < 1; ++i)
		{
			pObj = pMeshData->Instantiate();
			pObj->SetName(L"MonsterA");
			pObj->Transform()->SetDebugSphereUse(true);
			pObj->Transform()->SetRelativeScale(5.f, 5.f, 5.f);
			SpawnGameObject(pObj, Vec3((i + 1) * 50.f, 200.f, 500.f), 0);
		}

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\monster.fbx");

		for (int i = 0; i < 1; ++i)
		{
			pObj = pMeshData->Instantiate();
			pObj->SetName(L"MonsterB");
			pObj->Transform()->SetDebugSphereUse(true);
			pObj->Transform()->SetRelativeScale(5.f, 5.f, 5.f);
			SpawnGameObject(pObj, Vec3((i + 1) * 50.f, 200.f, 100.f), 0);
		}
	}

	//{
	//	Ptr<CMeshData> pMeshData = nullptr;
	//	CGameObject* pObj = nullptr;
	//	// 인스턴싱 테스트
	//	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\house.fbx");
	//	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\monster.mdat");
	//	pObj = pMeshData->Instantiate();
	//	//pObj->AddComponent(new CTestScript());
	//	pObj->SetName(L"Monster2");
	//	pObj->Transform()->SetDebugSphereUse(true);
	//	pObj->SetLayerIdx(2);
	//	pObj->Transform()->SetRelativePos(Vec3(100.f, 600.f, 0.f));
	//	tPhysicsInfo info = {};
	//	info.eActorType = ACTOR_TYPE::Dynamic;
	//	info.eGeomType = GEOMETRY_TYPE::Sphere;
	//	info.size = Vector3(100.f, 100.f, 100.f);
	//	info.massProperties.restitution = 0.1f;
	//	pObj->AddComponent(new CRigidBody);
	//	pObj->RigidBody()->SetPhysical(info);
	//	pObj->AddComponent(new CCollider3D);
	//	pObj->RigidBody()->AddActorToLevel();
	//	pObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 180.f));
	//	SpawnGameObject(pObj, Vec3(200.f, 600.f, 0.f), 2);		
	//}
	//{
	//	CGameObject* pObj = new CGameObject;
	//	pObj->AddComponent(new CTransform);
	//	pObj->AddComponent(new CMeshRender);
	//	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_Debug"));
	//	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);
	//	pObj->SetName(L"staticBox");
	//	pObj->SetLayerIdx(5);
	//	pObj->Transform()->SetRelativePos(Vec3(500.f, 100.f, 0.f));
	//	pObj->Transform()->SetRelativeScale(200.f, 200.f, 200.f);
	//	tPhysicsInfo info = {};
	//	info.eActorType = ACTOR_TYPE::Static;
	//	info.eGeomType = GEOMETRY_TYPE::Box;
	//	info.size = Vector3(200.f, 200.f, 200.f);
	//	//info.massProperties.restitution = 0.1f;
	//	pObj->AddComponent(new CRigidBody);
	//	pObj->RigidBody()->SetPhysical(info);
	//	pObj->AddComponent(new CCollider3D);
	//	pObj->RigidBody()->AddActorToLevel();
	//	SpawnGameObject(pObj, Vec3(500.f, 100.f, 0.f), 5);
	//}
	/*{
		CGameObject* pObj = new CGameObject;
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_Debug"));
		pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

		pObj->SetName(L"DynamicBox");
		pObj->SetLayerIdx(5);
		pObj->Transform()->SetRelativePos(Vec3(200.f, 100.f, 300.f));
		pObj->Transform()->SetRelativeScale(100.f, 100.f, 100.f);

		tPhysicsInfo info = {};
		info.eActorType = ACTOR_TYPE::Dynamic;
		info.eGeomType = GEOMETRY_TYPE::Box;
		info.size = Vector3(100.f, 100.f, 100.f);
		info.massProperties.restitution = 0.f;
		info.massProperties.dynamicFriction = 0.6f;
		info.massProperties.staticFriction = 0.6f;

		pObj->AddComponent(new CRigidBody);
		pObj->RigidBody()->SetPhysical(info);
		
		pObj->RigidBody()->SetMass(20.f);
		pObj->RigidBody()->SetRestitution(0.f);

		pObj->AddComponent(new CCollider3D);
		pObj->RigidBody()->AddActorToLevel();

		SpawnGameObject(pObj, Vec3(200.f, 500.f, 300.f), 5);
	}*/
	/*{
		CGameObject* pObj = new CGameObject;
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
		pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

		pObj->SetName(L"DynamicSphere");
		pObj->SetLayerIdx(5);
		pObj->Transform()->SetRelativePos(Vec3(-200.f, 400.f, 0.f));
		pObj->Transform()->SetRelativeScale(100.f, 100.f, 100.f);

		tPhysicsInfo info = {};
		info.eActorType = ACTOR_TYPE::Dynamic;
		info.eGeomType = GEOMETRY_TYPE::Sphere;
		info.size = Vector3(100.f, 100.f, 100.f);
		info.massProperties.restitution = 0.f;
		info.massProperties.dynamicFriction = 0.6f;
		info.massProperties.staticFriction = 0.6f;

		pObj->AddComponent(new CRigidBody);
		pObj->RigidBody()->SetPhysical(info);

		pObj->RigidBody()->SetMass(20.f);
		pObj->RigidBody()->SetRestitution(0.f);

		pObj->AddComponent(new CCollider3D);
		pObj->RigidBody()->AddActorToLevel();

		SpawnGameObject(pObj, Vec3(-200.f, 400.f, 0.f), 5);


	}*/

	// LandScape Object
	{
		CGameObject* pLandScape = new CGameObject;
		pLandScape->SetName(L"LandScape");

		pLandScape->AddComponent(new CTransform);
		pLandScape->AddComponent(new CLandScape);

		pLandScape->Transform()->SetRelativeScale(Vec3(1000.f, 4000.f, 1000.f));
		pLandScape->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

		pLandScape->LandScape()->SetFace(64, 64);
		pLandScape->LandScape()->SetFrustumCheck(false);
		//pLandScape->LandScape()->SetHeightMap(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\HeightMap_01.jpg"));		
		SpawnGameObject(pLandScape, Vec3(0.f, 0.f, 0.f), 4);
	}

	//{
	//	CGameObject* pGround = new CGameObject;
	//	pGround->SetName(L"Ground");
	//	pGround->AddComponent(new CTransform);
	//	pGround->Transform()->SetRelativeScale(10000.f, 15.f, 10000.f);
	//	pGround->SetLayerIdx(5);
	//	pGround->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	//	

	//	pGround->AddComponent(new CCollider3D);
	//	pGround->AddComponent(new CRigidBody);
	//	//pGround->RigidBody()->CreatePlane();
	//	tPhysicsInfo info = {};
	//	info.eActorType = ACTOR_TYPE::Static;
	//	info.eGeomType = GEOMETRY_TYPE::Box;
	//	info.size = Vector3(10000.f, 15.f, 10000.f);
	//	info.massProperties.restitution = 0.2f;
	//	info.massProperties.dynamicFriction = 0.3f;
	//	info.massProperties.staticFriction = 0.3f;
	//	pGround->RigidBody()->SetPhysical(info);
	//	pGround->RigidBody()->AddActorToLevel();

	//	pGround->AddComponent(new CMeshRender);
	//	Ptr<CMesh> mesh = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_Debug");
	//	pGround->MeshRender()->SetMesh(mesh);
	//	Ptr<CMaterial> mater = CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl");
	//	pGround->MeshRender()->SetMaterial(mater, 0);

	//	PxVec3 pos = pGround->Transform()->GetPhysicsPosition();
	//	
	//	SpawnGameObject(pGround, Vec3(0.f, 0.f, 0.f), 4);
	//}
	
}
