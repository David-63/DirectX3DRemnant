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
#include <Script/CM_Lurker_STATE_Idle_Script.h>


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
	CCollisionMgr::GetInst()->SetColLayer((UINT)LAYER_TYPE::Wall, (UINT)LAYER_TYPE::HitBoxMonster);
	CCollisionMgr::GetInst()->SetColLayer((UINT)LAYER_TYPE::Wall, (UINT)LAYER_TYPE::HitBoxPlayer);

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
	pUICam->Camera()->SetUIOnly();
	pUICam->Camera()->SetFar(20000.f);
	SpawnGameObject(pUICam, Vec3(19.f, -46.f, -500.f), 0);


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

	{
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"CrossHair");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetRelativeScale(Vec3(45.f, 45.f, 1.f));

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DUIMtrl"), 0);
		pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\aim2.png"));

		SpawnGameObject(pObject, Vec3(19.f, -46.f, -500.f), L"ViewPort UI");
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
	//{
	//	Ptr<CMeshData> data = CResMgr::GetInst()->LoadFBX(L"fbx\\Spider\\Wasteland_Spider_Alert.fbx");
	//	CGameObject* obj = data->Instantiate();
	//	obj->Animator3D()->SimpleGen(L"animclip\\Spider\\Wasteland_Spider_Alert.animclip");
	//}
	//Annointed
	/*{
		Ptr<CMeshData> data = CResMgr::GetInst()->LoadFBX(L"fbx\\Spider\\Wasteland_Spider_Death_F_01.fbx");
		CGameObject* obj = data->Instantiate();
		obj->Animator3D()->SimpleGen(L"animclip\\Spider\\Wasteland_Spider_Death_F_01.animclip");

		obj->SetName(L"Spider");
		obj->AddComponent(new CM_Spider_FSMScript);
		obj->AddComponent(new CHitBoxScript);
		obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);

		obj->AddComponent(new CPathFinderScript());
		obj->AddComponent(new CMonsterMoveScript());

		obj->Transform()->SetRelativePos(Vec3(-500.f, 0.f, 0.f));

		obj->AddComponent(new CRigidBody);

		tShapeInfo info = {};
		info.eGeomType = GEOMETRY_TYPE::Sphere;
		info.size = Vector3(15.f, 15.f, 15.f);
		info.massProperties.restitution = 0.99f;
		info.CollideType = (UINT)COLLIDE_TYPE::Monster;
		obj->RigidBody()->PushBackShapeInfo(info);

		tShapeInfo info2 = {};
		info2.eGeomType = GEOMETRY_TYPE::Sphere;
		info2.size = Vector3(150.f, 15.f, 15.f);
		info2.massProperties.restitution = 0.99f;
		info2.CollideType = (UINT)COLLIDE_TYPE::Monster;
		obj->RigidBody()->PushBackShapeInfo(info2);


		obj->RigidBody()->SetPhysical(ACTOR_TYPE::Kinematic);
		obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
		obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
		obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
		obj->RigidBody()->GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);


		int num = obj->RigidBody()->GetRigidActor()->getNbShapes();
		obj->RigidBody()->SetShapeLocalPos(0, Vec3(5.f, 7.5f, 0.f));
		obj->RigidBody()->SetShapeLocalPos(1, Vec3(5.f, 130.f, 0.f));


		obj->AddComponent(new CCollider3D);

		SpawnGameObject(obj, Vec3(-500.f, 0.f, 0.f), (UINT)LAYER_TYPE::Monster);
	}*/


	//맵오브젝트
	/*{
		Ptr<CMeshData> data = CResMgr::GetInst()->LoadFBX(L"fbx\\Map\\BossBack.fbx");
		CGameObject* obj = data->Instantiate();

		obj->SetName(L"BossBack");
		obj->SetLayerIdx((UINT)LAYER_TYPE::Wall);

		obj->AddComponent(new CRigidBody);
		obj->Transform()->SetRelativePos(Vec3(42.f, 0.f, 4107.f));
		obj->Transform()->SetRelativeScale(Vec3(4.f, 4.f, 4.5f));
		obj->Transform()->SetRelativeRot(Vec3(0.f, -XM_PIDIV2, 0.f));

		tShapeInfo info = {};
		info.eGeomType = GEOMETRY_TYPE::Box;
		info.size = Vector3(1400.f, 760.f, 220);
		info.CollideType = (UINT)COLLIDE_TYPE::Other;
		obj->RigidBody()->PushBackShapeInfo(info);
		obj->RigidBody()->SetPhysical(ACTOR_TYPE::Static);

		obj->RigidBody()->SetShapeLocalPos(0, Vec3(0.f, 360.f, 300.f));

		SpawnGameObject(obj, Vec3(42.f, 0.f, 4107.f), (UINT)LAYER_TYPE::Wall);
	}*/

	//lurker    
	/*{
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
		info.CollideType = (UINT)COLLIDE_TYPE::Monster;
		obj->RigidBody()->PushBackShapeInfo(info);

		tShapeInfo info1 = {};
		info1.eGeomType = GEOMETRY_TYPE::Sphere;
		info1.size = Vector3(90.f, 2.f, 2.f);
		info1.CollideType = (UINT)COLLIDE_TYPE::Monster;
		obj->RigidBody()->PushBackShapeInfo(info1);

		tShapeInfo info2 = {};
		info2.eGeomType = GEOMETRY_TYPE::Sphere;
		info2.size = Vector3(90.f, 2.f, 2.f);
		info2.CollideType = (UINT)COLLIDE_TYPE::Monster;
		obj->RigidBody()->PushBackShapeInfo(info2);

		tShapeInfo info3 = {};
		info3.eGeomType = GEOMETRY_TYPE::Sphere;
		info3.size = Vector3(30.f, 2.f, 2.f);
		info3.CollideType = (UINT)COLLIDE_TYPE::Monster;
		obj->RigidBody()->PushBackShapeInfo(info3);



		obj->RigidBody()->SetPhysical(ACTOR_TYPE::Kinematic);
		obj->RigidBody()->SetShapeLocalPos(0, Vec3(0.f, 1.f, -10.f));
		obj->RigidBody()->SetShapeLocalPos(1, Vec3(0.f, 45.f, -10.f));
		obj->RigidBody()->SetShapeLocalPos(2, Vec3(0.f, 120.f, -10.f));
		obj->RigidBody()->SetShapeLocalPos(3, Vec3(0.f, 160.f, -10.f));


		obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
		obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
		obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
		obj->RigidBody()->GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);



		SpawnGameObject(obj, Vec3(0.f, 0.f, 0.f), (UINT)LAYER_TYPE::Monster);
	}*/


	//Ptr<CMeshData> pMeshData = nullptr;
	//CGameObject* player = nullptr;

	// 
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\house.fbx");
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\monster.fbx");
	//pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\monster.mdat");
	//player->AddComponent(new CTestScript());
	//player = pMeshData->Instantiate();
	//
	//player->SetName(L"Player");
	//player->Transform()->SetDebugSphereUse(true);
	//player->SetLayerIdx((UINT)LAYER_TYPE::Player);
	//player->Transform()->SetRelativePos(Vec3(900.f, 0.f, 50.f));
	//player->AddComponent(new CRigidBody);

	//tShapeInfo info = {};
	//info.eGeomType = GEOMETRY_TYPE::Sphere;
	//info.size = Vector3(15.f, 15.f, 15.f);
	//info.CollideType = (UINT)COLLIDE_TYPE::Player;
	//player->RigidBody()->PushBackShapeInfo(info);

	//tShapeInfo info2 = {};
	//info2.eGeomType = GEOMETRY_TYPE::Sphere;
	//info2.size = Vector3(15.f, 15.f, 15.f);
	//info2.CollideType = (UINT)COLLIDE_TYPE::Player;
	//player->RigidBody()->PushBackShapeInfo(info2);

	//tShapeInfo info3 = {};
	//info3.eGeomType = GEOMETRY_TYPE::Sphere;
	//info3.size = Vector3(8.f, 8.f, 8.f);
	//info3.CollideType = (UINT)COLLIDE_TYPE::Player;
	//player->RigidBody()->PushBackShapeInfo(info3);

	//player->RigidBody()->SetPhysical(ACTOR_TYPE::Dynamic);
	//player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
	//player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
	//player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
	//player->RigidBody()->GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);


	////int num = player->RigidBody()->GetRigidActor()->getNbShapes();
	//player->RigidBody()->SetShapeLocalPos(0, Vec3(5.f, 7.5f, 0.f));
	//player->RigidBody()->SetShapeLocalPos(1, Vec3(5.f, 22.5f, 0.f));
	//player->RigidBody()->SetShapeLocalPos(2, Vec3(5.f, 34.f, 0.f));

	//player->AddComponent(new CCollider3D);
	//player->Collider3D()->SetType(COLLIDER3D_TYPE::Player);

	//SpawnGameObject(player, Vec3(900, 0.f, 50.f), (UINT)LAYER_TYPE::Player);

	////ruler
	//{
	//	CGameObject* obj = new CGameObject;
	//	obj->AddComponent(new CMeshRender);
	//	obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_Debug"));
	//	obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

	//	obj->AddComponent(new CTestScript());
	//	obj->AddComponent(new CTransform());
	//	obj->AddComponent(new CRigidBody);

	//	tShapeInfo info = {};
	//	info.eGeomType = GEOMETRY_TYPE::Sphere;
	//	info.size = Vector3(10.f, 15.f, 15.f);
	//	info.CollideType = (UINT)COLLIDE_TYPE::Other;
	//	obj->RigidBody()->PushBackShapeInfo(info);
	//	SpawnGameObject(obj, Vec3(0, 0.f, -20.f), (UINT)LAYER_TYPE::Wall);
	//}





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
		info.CollideType = (UINT)COLLIDE_TYPE::Other;

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

		//player
	{
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"can");
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CTestScript);

		SpawnGameObject(pObj, Vec3(000.f, 0.f, 000.f), 0);
	}

	tRayCastInfo* hit = Physics::GetInst()->RayCast(Vec3(500.f, 100.f, 500.f), Vec3(0.f, 0.f, -1.f), 1000.f);










	//바닥
	{
		Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\FloorTile.pref", L"prefab\\FloorTile.pref");
		fab->PrefabLoad(L"prefab\\FloorTile.pref");
		CGameObject* empty = nullptr;

		for (int i = 0; i <= 3; ++i)
		{
			for (int j = 0; j <= 3; ++j)
			{
				Vec3 spawnPos = Vec3(-5000.f + (j * 3200), -1.f, 5000.f - (i * 3200));
				empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Ground);
				empty->Transform()->SetRelativeScale(8.f, 1.f, 8.f);
				empty->MeshRender()->SetFrustumCheck(false);
				empty->SetName(L"FloorTile");
				empty->SetLayerIdx((UINT)LAYER_TYPE::Ground);
				SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Ground);
			}
		}
		fab->FabClear();
	}

#define WallSize 13
	{
		Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\BC_Wall.pref", L"prefab\\BC_Wall.pref");
		fab->PrefabLoad(L"prefab\\BC_Wall.pref");
		CGameObject* empty = nullptr;
		Vec3 spawnPos = Vec3(0, 0, 0);
		for (int size = 0; size < WallSize; ++size)
		{
			spawnPos = Vec3(-4600.f + (size * 800.f), 0.f, 5000);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		}
		for (int size = 0; size < WallSize; ++size)
		{
			spawnPos = Vec3(-4600.f + (size * 800.f), 0.f, -5000);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		}

		for (int size = 0; size < WallSize; ++size)
		{
			spawnPos = Vec3(-4600.f + (size * 800.f), 0.f, 5000);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		}

		for (int size = 0; size < WallSize; ++size)
		{
			spawnPos = Vec3(-5000.f, 0.f, -4600.f + (size * 800.f));
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
			empty->Transform()->SetRelativeRot(Vec3(0.f, XM_PIDIV2, 0.f));
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		}
		for (int size = 0; size < WallSize; ++size)
		{
			spawnPos = Vec3(5000.f, 0.f, -4600.f + (size * 800.f));
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
			empty->Transform()->SetRelativeRot(Vec3(0.f, XM_PIDIV2, 0.f));
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		}

		for (int size = 0; size < 3; ++size)
		{
			spawnPos = Vec3(-4600.f + (size * 800.f), 0.f, -3400);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		}

		spawnPos = Vec3(-2600.f, 0.f, -3800);
		empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
		empty->AddComponent(new CCollider3D);
		empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
		empty->Transform()->SetRelativeRot(Vec3(0.f, XM_PIDIV2, 0.f));
		SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);

		for (int size = 0; size < 2; ++size)
		{
			spawnPos = Vec3(-2200.f + (size * 800.f), 0.f, -4200);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		}
		spawnPos = Vec3(-1000.f, 0.f, -3800);
		empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
		empty->AddComponent(new CCollider3D);
		empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
		empty->Transform()->SetRelativeRot(Vec3(0.f, XM_PIDIV2, 0.f));
		SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		spawnPos = Vec3(600.f, 0.f, -4600);
		empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
		empty->AddComponent(new CCollider3D);
		empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
		empty->Transform()->SetRelativeRot(Vec3(0.f, XM_PIDIV2, 0.f));
		SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		spawnPos = Vec3(600.f, 0.f, -3800);
		empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
		empty->AddComponent(new CCollider3D);
		empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
		empty->Transform()->SetRelativeRot(Vec3(0.f, XM_PIDIV2, 0.f));
		SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);


		for (int size = 0; size < 6; ++size)
		{
			spawnPos = Vec3(1000.f + (size * 800.f), 0.f, -3400);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		}
		for (int size = 0; size < 2; ++size)
		{
			spawnPos = Vec3(-2200.f + (size * 800.f), 0.f, -3400);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeScale(2.f, 2.f, 1.f);
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		}

		fab->FabClear();
	}

	//StatueC
	{
		Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\StatueC.pref", L"prefab\\StatueC.pref");
		fab->PrefabLoad(L"prefab\\StatueC.pref");
		CGameObject* obj = fab->Instantiate(Vec3(1200.f, 0.f, 4400.f), (UINT)LAYER_TYPE::Wall);
		fab->FabClear();
		obj->AddComponent(new CCollider3D);
		obj->SetName(L"StatueC");
		obj->SetLayerIdx((UINT)LAYER_TYPE::Wall);

		SpawnGameObject(obj, Vec3(1200.f, 0.f, 4200.f), (UINT)LAYER_TYPE::Wall);
	}

	////StatueB
	{
		Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Prop_StatueB.pref", L"prefab\\Prop_StatueB.pref");
		fab->PrefabLoad(L"prefab\\Prop_StatueB.pref");
		CGameObject* obj = fab->Instantiate(Vec3(-1200.f, 0.f, 4400.f), (UINT)LAYER_TYPE::Wall);
		fab->FabClear();
		obj->AddComponent(new CCollider3D);
		obj->SetName(L"StatueB");
		obj->SetLayerIdx((UINT)LAYER_TYPE::Wall);

		SpawnGameObject(obj, Vec3(-1200.f, 0.f, 4200.f), (UINT)LAYER_TYPE::Wall);
	}

	//석관들
	{
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\prop_SarcoPhagus.pref", L"prefab\\prop_SarcoPhagus.pref");
			fab->PrefabLoad(L"prefab\\prop_SarcoPhagus.pref");
			CGameObject* obj = fab->Instantiate(Vec3(2565.f, 0.f, 1998.f), (UINT)LAYER_TYPE::Wall);
			fab->FabClear();
			obj->AddComponent(new CCollider3D);
			obj->SetName(L"Sarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Wall);

			SpawnGameObject(obj, Vec3(2565.f, 0.f, 1998.f), (UINT)LAYER_TYPE::Wall);
		}
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\prop_SarcoPhagus.pref", L"prefab\\prop_SarcoPhagus.pref");
			fab->PrefabLoad(L"prefab\\prop_SarcoPhagus.pref");
			CGameObject* obj = fab->Instantiate(Vec3(2565.f, 0.f, -379.f), (UINT)LAYER_TYPE::Wall);
			fab->FabClear();
			obj->SetName(L"Sarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Wall);

			SpawnGameObject(obj, Vec3(2565.f, 0.f, -379.f), (UINT)LAYER_TYPE::Wall);
		}
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\prop_SarcoPhagus.pref", L"prefab\\prop_SarcoPhagus.pref");
			fab->PrefabLoad(L"prefab\\prop_SarcoPhagus.pref");
			CGameObject* obj = fab->Instantiate(Vec3(2565.f, 0.f, -2178.f), (UINT)LAYER_TYPE::Wall);
			fab->FabClear();
			obj->SetName(L"Sarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Wall);

			SpawnGameObject(obj, Vec3(2565.f, 0.f, -2178.f), (UINT)LAYER_TYPE::Wall);
		}
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\prop_SarcoPhagus.pref", L"prefab\\prop_SarcoPhagus.pref");
			fab->PrefabLoad(L"prefab\\prop_SarcoPhagus.pref");
			CGameObject* obj = fab->Instantiate(Vec3(-2565.f, 0.f, 1998.f), (UINT)LAYER_TYPE::Wall);
			fab->FabClear();
			obj->SetName(L"Sarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Wall);

			SpawnGameObject(obj, Vec3(-2565.f, 0.f, 1998.f), (UINT)LAYER_TYPE::Wall);
		}
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\prop_SarcoPhagus.pref", L"prefab\\prop_SarcoPhagus.pref");
			fab->PrefabLoad(L"prefab\\prop_SarcoPhagus.pref");
			CGameObject* obj = fab->Instantiate(Vec3(-2565.f, 0.f, -379.f), (UINT)LAYER_TYPE::Wall);
			fab->FabClear();
			obj->SetName(L"Sarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Wall);

			SpawnGameObject(obj, Vec3(-2565.f, 0.f, -379.f), (UINT)LAYER_TYPE::Wall);
		}
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\prop_SarcoPhagus.pref", L"prefab\\prop_SarcoPhagus.pref");
			fab->PrefabLoad(L"prefab\\prop_SarcoPhagus.pref");
			CGameObject* obj = fab->Instantiate(Vec3(-2565.f, 0.f, -2178.f), (UINT)LAYER_TYPE::Wall);
			fab->FabClear();
			obj->SetName(L"Sarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Wall);

			SpawnGameObject(obj, Vec3(-2565.f, 0.f, -2178.f), (UINT)LAYER_TYPE::Wall);
		}
	}

	//파이프
	{
		Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Prop_Pipe.pref", L"prefab\\Prop_Pipe.pref");
		fab->PrefabLoad(L"prefab\\Prop_Pipe.pref");
		CGameObject* obj = fab->Instantiate(Vec3(483.f, 0.f, -4897.f), (UINT)LAYER_TYPE::Wall);
		fab->FabClear();
		obj->AddComponent(new CCollider3D);
		obj->SetName(L"Pipe");
		obj->SetLayerIdx((UINT)LAYER_TYPE::Wall);

		SpawnGameObject(obj, Vec3(483.f, 0.f, -4897.f), (UINT)LAYER_TYPE::Wall);
	}

	//보스쪽벽면
	{
		Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\BossBack.pref", L"prefab\\BossBack.pref");
		fab->PrefabLoad(L"prefab\\BossBack.pref");
		CGameObject* obj = fab->Instantiate(Vec3(42, 0.f, 4207), (UINT)LAYER_TYPE::Wall);
		fab->FabClear();
		obj->AddComponent(new CCollider3D);
		obj->SetName(L"BossBack");
		obj->SetLayerIdx((UINT)LAYER_TYPE::Wall);

		SpawnGameObject(obj, Vec3(42.f, 0.f, 4207.f), (UINT)LAYER_TYPE::Wall);
	}

	//기둥들
	{
		Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Pillar.pref", L"prefab\\Pillar.pref");
		fab->PrefabLoad(L"prefab\\Pillar.pref");
		CGameObject* empty = nullptr;
		Vec3 spawnPos = Vec3(0, 0, 0);
		{
			spawnPos = Vec3(-2600.f, 0.f, 0);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeRot(Vec3(0.f, -XM_PIDIV2, 0.f));
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
			spawnPos = Vec3(2600.f, 0.f, 0);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeRot(Vec3(0.f, XM_PIDIV2, 0.f));
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);

			spawnPos = Vec3(-2600.f, 0.f, 2400.f);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeRot(Vec3(0.f, -XM_PIDIV2, 0.f));
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
			spawnPos = Vec3(2600.f, 0.f, 2400.f);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeRot(Vec3(0.f, XM_PIDIV2, 0.f));
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);

			spawnPos = Vec3(-2600.f, 0.f, -1800.f);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeRot(Vec3(0.f, -XM_PIDIV2, 0.f));
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
			spawnPos = Vec3(2600.f, 0.f, -1800.f);
			empty = fab.Get()->Instantiate(spawnPos, (UINT)LAYER_TYPE::Wall);
			empty->AddComponent(new CCollider3D);
			empty->Transform()->SetRelativeRot(Vec3(0.f, XM_PIDIV2, 0.f));
			SpawnGameObject(empty, spawnPos, (UINT)LAYER_TYPE::Wall);
		}
		fab->FabClear();
	}


	//러커배치(석관)
	{
		//3시
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Monster_Lurker.pref", L"prefab\\Monster_Lurker.pref");
			fab->PrefabLoad(L"prefab\\Monster_Lurker.pref");
			CGameObject* obj = fab->Instantiate(Vec3(2565.f, 0.f, -457.f), (UINT)LAYER_TYPE::Monster);
			fab->FabClear();
			obj->SetName(L"Lurker1ClockSarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);

			dynamic_cast<CM_Lurker_FSMScript*>(obj->GetScript<CM_Lurker_FSMScript>())->SetIdlePose(5);

			SpawnGameObject(obj, Vec3(2565.f, 0.f, -457.f), (UINT)LAYER_TYPE::Monster);
		}
		//5시
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Monster_Lurker.pref", L"prefab\\Monster_Lurker.pref");
			fab->PrefabLoad(L"prefab\\Monster_Lurker.pref");
			CGameObject* obj = fab->Instantiate(Vec3(2565.f, 0.f, -2279.f), (UINT)LAYER_TYPE::Monster);
			fab->FabClear();
			obj->SetName(L"Lurker1ClockSarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);

			dynamic_cast<CM_Lurker_FSMScript*>(obj->GetScript<CM_Lurker_FSMScript>())->SetIdlePose(5);

			SpawnGameObject(obj, Vec3(2565.f, 0.f, -2279.f), (UINT)LAYER_TYPE::Monster);
		}
		//11시
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Monster_Lurker.pref", L"prefab\\Monster_Lurker.pref");
			fab->PrefabLoad(L"prefab\\Monster_Lurker.pref");
			CGameObject* obj = fab->Instantiate(Vec3(-2565.f, 0.f, 1920.f), (UINT)LAYER_TYPE::Monster);
			fab->FabClear();
			obj->SetName(L"Lurker1ClockSarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);

			dynamic_cast<CM_Lurker_FSMScript*>(obj->GetScript<CM_Lurker_FSMScript>())->SetIdlePose(5);

			SpawnGameObject(obj, Vec3(-2565.f, 0.f, 1920.f), (UINT)LAYER_TYPE::Monster);
		}
		//9시
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Monster_Lurker.pref", L"prefab\\Monster_Lurker.pref");
			fab->PrefabLoad(L"prefab\\Monster_Lurker.pref");
			CGameObject* obj = fab->Instantiate(Vec3(-2565.f, 0.f, -457.f), (UINT)LAYER_TYPE::Monster);
			fab->FabClear();
			obj->SetName(L"Lurker1ClockSarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);

			dynamic_cast<CM_Lurker_FSMScript*>(obj->GetScript<CM_Lurker_FSMScript>())->SetIdlePose(5);

			SpawnGameObject(obj, Vec3(-2565.f, 0.f, -457.f), (UINT)LAYER_TYPE::Monster);
		}
	}

	//러커배치(벽)
	{
		//1시
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Monster_Lurker.pref", L"prefab\\Monster_Lurker.pref");
			fab->PrefabLoad(L"prefab\\Monster_Lurker.pref");
			CGameObject* obj = fab->Instantiate(Vec3(2208.f, 0.f, 2788.f), (UINT)LAYER_TYPE::Monster);
			fab->FabClear();
			obj->SetName(L"Lurker1ClockSarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);
			obj->Transform()->SetRelativeRot(Vec3(0.f, XM_PIDIV2, 0.f));

			dynamic_cast<CM_Lurker_FSMScript*>(obj->GetScript<CM_Lurker_FSMScript>())->SetIdlePose(3);

			SpawnGameObject(obj, Vec3(2208.f, 0.f, 2788.f), (UINT)LAYER_TYPE::Monster);
		}
		//5시
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Monster_Lurker.pref", L"prefab\\Monster_Lurker.pref");
			fab->PrefabLoad(L"prefab\\Monster_Lurker.pref");
			CGameObject* obj = fab->Instantiate(Vec3(2208.f, 0.f, -1404.f), (UINT)LAYER_TYPE::Monster);
			fab->FabClear();
			obj->SetName(L"Lurker5ClockSarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);
			obj->Transform()->SetRelativeRot(Vec3(0.f, XM_PIDIV2, 0.f));

			dynamic_cast<CM_Lurker_FSMScript*>(obj->GetScript<CM_Lurker_FSMScript>())->SetIdlePose(3);

			SpawnGameObject(obj, Vec3(2208.f, 0.f, -1404.f), (UINT)LAYER_TYPE::Monster);
		}
		//6시
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Monster_Lurker.pref", L"prefab\\Monster_Lurker.pref");
			fab->PrefabLoad(L"prefab\\Monster_Lurker.pref");
			CGameObject* obj = fab->Instantiate(Vec3(-2268.f, 0.f, 411.f), (UINT)LAYER_TYPE::Monster);
			fab->FabClear();
			obj->SetName(L"Lurker3ClockSarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);
			obj->Transform()->SetRelativeRot(Vec3(0.f, -XM_PIDIV2, 0.f));

			dynamic_cast<CM_Lurker_FSMScript*>(obj->GetScript<CM_Lurker_FSMScript>())->SetIdlePose(4);

			SpawnGameObject(obj, Vec3(-2268.f, 0.f, 411.f), (UINT)LAYER_TYPE::Monster);
		}
		//7시
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Monster_Lurker.pref", L"prefab\\Monster_Lurker.pref");
			fab->PrefabLoad(L"prefab\\Monster_Lurker.pref");
			CGameObject* obj = fab->Instantiate(Vec3(-2268.f, 0.f, -1404.f), (UINT)LAYER_TYPE::Monster);
			fab->FabClear();
			obj->SetName(L"Lurker5ClockSarco");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);
			obj->Transform()->SetRelativeRot(Vec3(0.f, -XM_PIDIV2, 0.f));

			dynamic_cast<CM_Lurker_FSMScript*>(obj->GetScript<CM_Lurker_FSMScript>())->SetIdlePose(4);

			SpawnGameObject(obj, Vec3(-2268.f, 0.f, -1404.f), (UINT)LAYER_TYPE::Monster);
		}
	}

	//스파이더
	{
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Monster_Spider.pref", L"prefab\\Monster_Spider.pref");
			fab->PrefabLoad(L"prefab\\Monster_Spider.pref");
			CGameObject* obj = fab->Instantiate(Vec3(-1143.f, 0.f, 3338.f), (UINT)LAYER_TYPE::Monster);
			fab->FabClear();
			obj->SetName(L"Spider");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);


			SpawnGameObject(obj, Vec3(-1143.f, 0.f, 3338.f), (UINT)LAYER_TYPE::Monster);
		}
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Monster_Spider.pref", L"prefab\\Monster_Spider.pref");
			fab->PrefabLoad(L"prefab\\Monster_Spider.pref");
			CGameObject* obj = fab->Instantiate(Vec3(-46.f, 0.f, 3338.f), (UINT)LAYER_TYPE::Monster);
			fab->FabClear();
			obj->SetName(L"Spider");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);


			SpawnGameObject(obj, Vec3(-46.f, 0.f, 3338.f), (UINT)LAYER_TYPE::Monster);
		}
		{
			Ptr<CPrefab> fab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Monster_Spider.pref", L"prefab\\Monster_Spider.pref");
			fab->PrefabLoad(L"prefab\\Monster_Spider.pref");
			CGameObject* obj = fab->Instantiate(Vec3(1245.f, 0.f, -844.f), (UINT)LAYER_TYPE::Monster);
			fab->FabClear();
			obj->SetName(L"Spider");
			obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);


			SpawnGameObject(obj, Vec3(1245.f, 0.f, -844.f), (UINT)LAYER_TYPE::Monster);
		}
	}
}
