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
#include <Script/CM_Lurker_FSMScript.h>
#include <Script/CM_Spider_FSMScript.h>
#include <Script/CHitBoxScript.h>
#include <Script/CP_FSMScript.h>
#include <Script/CB_FSMScript.h>


void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);
	pCurLevel->SetName(L"TestLevel");

	// Layer 이름설정
	pCurLevel->GetLayer((UINT)LAYER_TYPE::Default)->SetName(L"Default");
	pCurLevel->GetLayer((UINT)LAYER_TYPE::Player)->SetName(L"Player");
	pCurLevel->GetLayer((UINT)LAYER_TYPE::Monster)->SetName(L"Monster");
	pCurLevel->GetLayer((UINT)LAYER_TYPE::Wall)->SetName(L"Wall");
	pCurLevel->GetLayer((UINT)LAYER_TYPE::Ground)->SetName(L"Ground");
	pCurLevel->GetLayer((UINT)LAYER_TYPE::HitBoxMonster)->SetName(L"HitBoxMonster");
	pCurLevel->GetLayer((UINT)LAYER_TYPE::HitBoxPlayer)->SetName(L"HitBoxPlayer");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");

	CCollisionMgr::GetInst()->SetColLayer((UINT)LAYER_TYPE::Player, (UINT)LAYER_TYPE::Monster);
	CCollisionMgr::GetInst()->SetColLayer((UINT)LAYER_TYPE::Player, (UINT)LAYER_TYPE::Ground);
	CCollisionMgr::GetInst()->SetColLayer((UINT)LAYER_TYPE::Monster, (UINT)LAYER_TYPE::Ground);
	CCollisionMgr::GetInst()->SetColLayer((UINT)LAYER_TYPE::Player, (UINT)LAYER_TYPE::Wall);
	CCollisionMgr::GetInst()->SetColLayer((UINT)LAYER_TYPE::Monster, (UINT)LAYER_TYPE::Wall);
	CCollisionMgr::GetInst()->SetColLayer((UINT)LAYER_TYPE::Player, (UINT)LAYER_TYPE::HitBoxMonster);
	CCollisionMgr::GetInst()->SetColLayer((UINT)LAYER_TYPE::Monster, (UINT)LAYER_TYPE::HitBoxPlayer);

	// camera

		// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraMoveScript);
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
	// Test obj
	/*{
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
	}*/
	//진짜 플레이어
	//{
	//	Ptr<CMeshData> pMeshData = nullptr;
	//	CGameObject* player = nullptr;

	//	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\P_R2Fire.fbx");
	//	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\P_R2Idle.mdat");
	//	player = pMeshData->Instantiate();
	//	//player = pMeshData->InstMesh();
	//	//player->Animator3D()->SimpleGen(L"animclip\\player\\P_2RHuntReload_End.animclip");
	//	player->SetName(L"Player");
	//	player->MeshRender()->SetFrustumCheck(false);

	//	player->SetLayerIdx((UINT)LAYER_TYPE::Player);
	//	player->Transform()->SetRelativePos(Vec3(200.f, 1000.f, 0.f));
	//	

	//	player->AddComponent(new CP_FSMScript());
	//	SpawnGameObject(player, Vec3(200.f, 1000.f, 0.f), 1);
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

	/*{
		Ptr<CMeshData> data = CResMgr::GetInst()->LoadFBX(L"fbx\\Spider\\Wasteland_Spider_Turn135_L.fbx");
		CGameObject* obj = data->Instantiate();
		obj->Animator3D()->SimpleGen(L"animclip\\Spider\\Wasteland_Spider_Turn135_L.animclip");

	}
	{
		Ptr<CMeshData> data = CResMgr::GetInst()->LoadFBX(L"fbx\\Spider\\Wasteland_Spider_Gun_Turn135_L.fbx");
		CGameObject* obj = data->Instantiate();
		obj->Animator3D()->SimpleGen(L"animclip\\Spider\\Wasteland_Spider_Gun_Turn135_L.animclip");

	}*/
	/*{
		Ptr<CMeshData> data = CResMgr::GetInst()->LoadFBX(L"fbx\\Spider\\Wasteland_Spider_Gun_Turn180_L.fbx");
		CGameObject* obj = data->Instantiate();
		obj->Animator3D()->SimpleGen(L"animclip\\Spider\\Wasteland_Spider_Gun_Turn180_L.animclip");
	}*/
	//Annointed
	{
		Ptr<CMeshData> data = CResMgr::GetInst()->LoadFBX(L"fbx\\Spider\\Wasteland_Spider_Turn180_L.fbx");
		CGameObject* obj = data->Instantiate();
		obj->Animator3D()->SimpleGen(L"animclip\\Spider\\Wasteland_Spider_Turn180_L.animclip");

		obj->SetName(L"Spider");
		obj->AddComponent(new CM_Spider_FSMScript);
		obj->AddComponent(new CHitBoxScript);
		obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);

		obj->AddComponent(new CPathFinderScript());
		obj->AddComponent(new CMonsterMoveScript());

		SpawnGameObject(obj, Vec3(0.f, 0.f, 0.f), (UINT)LAYER_TYPE::Monster);
	}



	//lurker    
	{
		Ptr<CMeshData> data = CResMgr::GetInst()->LoadFBX(L"fbx\\Lurker\\Wasteland_Lurker_Impact_Heavy_F_01.fbx");
		CGameObject* obj = data->Instantiate();
		obj->Animator3D()->SimpleGen(L"animclip\\Lurker\\Wasteland_Lurker_Impact_Heavy_F_01.animclip");
		obj->SetName(L"TestObj");
		obj->AddComponent(new CM_Lurker_FSMScript);
		obj->AddComponent(new CHitBoxScript);
		obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);

		obj->AddComponent(new CPathFinderScript());
		obj->AddComponent(new CMonsterMoveScript());
		obj->AddComponent(new CRigidBody());

		

		tShapeInfo info = {};
		info.eGeomType = GEOMETRY_TYPE::Sphere;
		info.size = Vector3(2.f, 2.f, 2.f);
		obj->RigidBody()->PushBackShapeInfo(info);

		tShapeInfo info1 = {};
		info1.eGeomType = GEOMETRY_TYPE::Sphere;
		info1.size = Vector3(40.f, 40.f, 40.f);
		obj->RigidBody()->PushBackShapeInfo(info1);

		tShapeInfo info2 = {};
		info2.eGeomType = GEOMETRY_TYPE::Sphere;
		info2.size = Vector3(20.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info2);

		tShapeInfo info3 = {};
		info3.eGeomType = GEOMETRY_TYPE::Sphere;
		info3.size = Vector3(30.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info3);

		tShapeInfo info4 = {};
		info4.eGeomType = GEOMETRY_TYPE::Sphere;
		info4.size = Vector3(30.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info4);

		tShapeInfo info5 = {};
		info5.eGeomType = GEOMETRY_TYPE::Sphere;
		info5.size = Vector3(30.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info5);

		tShapeInfo info6 = {};
		info6.eGeomType = GEOMETRY_TYPE::Sphere;
		info6.size = Vector3(30.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info6);

		tShapeInfo info7 = {};
		info7.eGeomType = GEOMETRY_TYPE::Sphere;
		info7.size = Vector3(40.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info7);

		tShapeInfo info8 = {};
		info8.eGeomType = GEOMETRY_TYPE::Sphere;
		info8.size = Vector3(40.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info8);

		tShapeInfo info9 = {};
		info9.eGeomType = GEOMETRY_TYPE::Sphere;
		info9.size = Vector3(30.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info9);

		tShapeInfo info10 = {};
		info10.eGeomType = GEOMETRY_TYPE::Sphere;
		info10.size = Vector3(30.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info10);

		tShapeInfo info11 = {};
		info11.eGeomType = GEOMETRY_TYPE::Sphere;
		info11.size = Vector3(30.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info11);

		tShapeInfo info12 = {};
		info12.eGeomType = GEOMETRY_TYPE::Sphere;
		info12.size = Vector3(30.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info12);

		tShapeInfo info13 = {};
		info13.eGeomType = GEOMETRY_TYPE::Sphere;
		info13.size = Vector3(40.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info13);

		tShapeInfo info14 = {};
		info14.eGeomType = GEOMETRY_TYPE::Sphere;
		info14.size = Vector3(30.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info14);

		tShapeInfo info15 = {};
		info15.eGeomType = GEOMETRY_TYPE::Sphere;
		info15.size = Vector3(30.f, 20.f, 20.f);
		obj->RigidBody()->PushBackShapeInfo(info15);


		obj->RigidBody()->SetPhysical(ACTOR_TYPE::Kinematic);
		//obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
		//obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
		//obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
		obj->RigidBody()->GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);



		obj->RigidBody()->SetBoneSoket(0, 0);
		obj->RigidBody()->SetBoneSoket(1, 1);
		obj->RigidBody()->SetBoneSoket(2, 6);
		obj->RigidBody()->SetBoneSoket(3, 9);
		obj->RigidBody()->SetBoneSoket(4, 27);
		obj->RigidBody()->SetBoneSoket(5, 10);
		obj->RigidBody()->SetBoneSoket(6, 28);
		obj->RigidBody()->SetBoneSoket(7, 8); 
		obj->RigidBody()->SetBoneSoket(8, 4);
		obj->RigidBody()->SetBoneSoket(9, 49);
		obj->RigidBody()->SetBoneSoket(10, 53);
		obj->RigidBody()->SetBoneSoket(11, 50);
		obj->RigidBody()->SetBoneSoket(12, 54);
		obj->RigidBody()->SetBoneSoket(13, 26); 
		obj->RigidBody()->SetBoneSoket(14, 52); 
		obj->RigidBody()->SetBoneSoket(15, 48);




		SpawnGameObject(obj, Vec3(0.f, 0.f, 0.f), (UINT)LAYER_TYPE::Monster);
	}


	Ptr<CMeshData> pMeshData = nullptr;
	CGameObject* player = nullptr;

	// 
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\house.fbx");
	pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\monster.fbx");
	//pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\monster.mdat");
	player = pMeshData->Instantiate();
	player->AddComponent(new CTestScript());

	player->SetName(L"Player");
	player->Transform()->SetDebugSphereUse(true);
	player->SetLayerIdx((UINT)LAYER_TYPE::Player);
	player->Transform()->SetRelativePos(Vec3(900.f, 0.f, 50.f));
	player->AddComponent(new CRigidBody);

	tShapeInfo info = {};
	info.eGeomType = GEOMETRY_TYPE::Sphere;
	info.size = Vector3(15.f, 15.f, 15.f);
	info.CollideType = (UINT)COLLIDE_TYPE::Player;
	player->RigidBody()->PushBackShapeInfo(info);

	tShapeInfo info2 = {};
	info2.eGeomType = GEOMETRY_TYPE::Sphere;
	info2.size = Vector3(15.f, 15.f, 15.f);
	info2.CollideType = (UINT)COLLIDE_TYPE::Player;
	player->RigidBody()->PushBackShapeInfo(info2);

	tShapeInfo info3 = {};
	info3.eGeomType = GEOMETRY_TYPE::Sphere;
	info3.size = Vector3(8.f, 8.f, 8.f);
	info3.CollideType = (UINT)COLLIDE_TYPE::Player;
	player->RigidBody()->PushBackShapeInfo(info3);

	player->RigidBody()->SetPhysical(ACTOR_TYPE::Dynamic);
	player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
	player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
	player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
	player->RigidBody()->GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);


	//int num = player->RigidBody()->GetRigidActor()->getNbShapes();
	player->RigidBody()->SetShapeLocalPos(0, Vec3(5.f, 7.5f, 0.f));
	player->RigidBody()->SetShapeLocalPos(1, Vec3(5.f, 22.5f, 0.f));
	player->RigidBody()->SetShapeLocalPos(2, Vec3(5.f, 34.f, 0.f));

	player->AddComponent(new CCollider3D);
	player->Collider3D()->SetType(COLLIDER3D_TYPE::Player);

	SpawnGameObject(player, Vec3(900, 0.f, 50.f), (UINT)LAYER_TYPE::Player);




	//prefab
	//{
		//Ptr<CMeshData> pMeshData = nullptr;
		//CGameObject* pObj = nullptr;

		//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\monster.fbx");
		////pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\monster.mdat");
		//pObj = pMeshData->Instantiate();
		//
		//pObj->SetName(L"Proto");
		//pObj->Transform()->SetDebugSphereUse(true);
		//pObj->SetLayerIdx(2);
		//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
		//pObj->AddComponent(new CRigidBody);

		//tShapeInfo info = {};
		//info.eGeomType = GEOMETRY_TYPE::Sphere;
		//info.size = Vector3(15.f, 15.f, 15.f);
		//info.massProperties.restitution = 0.99f;
		//pObj->RigidBody()->PushBackShapeInfo(info);

		//tShapeInfo info2 = {};
		//info2.eGeomType = GEOMETRY_TYPE::Sphere;
		//info2.size = Vector3(15.f, 15.f, 15.f);
		//info2.massProperties.restitution = 0.99f;
		//pObj->RigidBody()->PushBackShapeInfo(info2);

		//tShapeInfo info3 = {};
		//info3.eGeomType = GEOMETRY_TYPE::Sphere;
		//info3.size = Vector3(8.f, 8.f, 8.f);
		//info3.massProperties.restitution = 0.99f;
		//pObj->RigidBody()->PushBackShapeInfo(info3);

		//pObj->RigidBody()->SetPhysical(ACTOR_TYPE::Dynamic);
		//pObj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
		//pObj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
		//pObj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
		//pObj->RigidBody()->GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		//

		//int num = pObj->RigidBody()->GetRigidActor()->getNbShapes();
		//pObj->RigidBody()->SetShapeLocalPos(0, Vec3(5.f, 7.5f, 0.f));
		//pObj->RigidBody()->SetShapeLocalPos(1, Vec3(5.f, 22.5f, 0.f));
		//pObj->RigidBody()->SetShapeLocalPos(2, Vec3(5.f, 34.f, 0.f));


		//pObj->AddComponent(new CCollider3D);
		//pObj->Collider3D()->SetType(COLLIDER3D_TYPE::Player);


		//SpawnGameObject(pObj, Vec3(40.f, 100.f, 40.f), 2);

		//CResMgr::GetInst()->SavePrefab(pObj, L"prefab\\prefab01.pref");

	//	Ptr<CPrefab> fab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\sampleObj.pref");
	//	CGameObject* cloneObj = fab->Instantiate(Vec3(300.f, 0.f, 400.f), 2);
	//	CGameObject* cloneObj2 = fab->Instantiate(Vec3(300.f, 0.f, 400.f), 2);
	//	

	//	SpawnGameObject(cloneObj, Vec3(300.f, 0.f, 400.f), L"Player");
	//	SpawnGameObject(cloneObj2, Vec3(300.f, 0.f, 400.f), L"Player");
	//}

		//static box
	{
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"staticBox");
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_Debug"));
		pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);


		pObj->SetLayerIdx((UINT)LAYER_TYPE::Wall);
		pObj->Transform()->SetRelativePos(Vec3(500.f, 100.f, 0.f));
		pObj->Transform()->SetRelativeScale(200.f, 200.f, 200.f);

		tShapeInfo info = {};
		info.eGeomType = GEOMETRY_TYPE::Box;
		info.size = Vector3(200.f, 200.f, 200.f);

		pObj->AddComponent(new CRigidBody);
		pObj->RigidBody()->PushBackShapeInfo(info);
		pObj->RigidBody()->SetPhysical(ACTOR_TYPE::Static);

		pObj->AddComponent(new CCollider3D);
		pObj->Collider3D()->SetType(COLLIDER3D_TYPE::Wall);

		SpawnGameObject(pObj, Vec3(500.f, 100.f, 0.f), (UINT)LAYER_TYPE::Wall);

	}

	//dynamic sphere
	/*{
		CGameObject* pObj = new CGameObject;
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
		pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

		pObj->SetName(L"DynamicSphere");
		pObj->SetLayerIdx(5);
		pObj->Transform()->SetRelativePos(Vec3(-200.f, 700.f, 0.f));
		pObj->Transform()->SetRelativeScale(100.f, 100.f, 100.f);

		tShapeInfo info = {};
		info.eGeomType = GEOMETRY_TYPE::Sphere;
		info.size = Vector3(100.f, 100.f, 100.f);
		info.massProperties.restitution = 0.f;
		info.massProperties.dynamicFriction = 0.6f;
		info.massProperties.staticFriction = 0.6f;

		pObj->AddComponent(new CRigidBody);
		pObj->AddComponent(new CCollider3D);

		SpawnGameObject(pObj, Vec3(-200.f, 700.f, 0.f), 5);

		}*/

	tRayCastInfo* hit = Physics::GetInst()->RayCast(Vec3(500.f, 100.f, 500.f), Vec3(0.f, 0.f, -1.f), 1000.f);
}
