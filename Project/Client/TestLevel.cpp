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
		Ptr<CMeshData> pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\Wasteland_Spider_Turn180_L.mdat");
		CGameObject* obj = pMeshData->Instantiate();
		//obj->Animator3D()->SimpleGen(L"animclip\Spider\Wasteland_Spider_Turn180_L.animclip");
		Vec3 startpos = Vec3(600.f, 0.f, 0.f);

		obj->SetName(L"Spider");
		obj->AddComponent(new CM_Spider_FSMScript);
		obj->AddComponent(new CHitBoxScript);
		obj->SetLayerIdx((UINT)LAYER_TYPE::Monster);

		obj->AddComponent(new CPathFinderScript());
		obj->AddComponent(new CMonsterMoveScript());

		obj->AddComponent(new CRigidBody);
		obj->Transform()->SetRelativePos(startpos);

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


		obj->RigidBody()->SetPhysical(ACTOR_TYPE::Dynamic);
		obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
		obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
		obj->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
		obj->RigidBody()->GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);


		int num = obj->RigidBody()->GetRigidActor()->getNbShapes();
		obj->RigidBody()->SetShapeLocalPos(0, Vec3(5.f, 7.5f, 0.f));
		obj->RigidBody()->SetShapeLocalPos(1, Vec3(5.f, 130.f, 0.f));


		obj->AddComponent(new CCollider3D);

		SpawnGameObject(obj, startpos, (UINT)LAYER_TYPE::Monster);
	}

	{

		//{
		//   Ptr<CMeshData> pMeshData = nullptr;
		//   CGameObject* player = nullptr;

		//   pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\P_R2Idle.mdat");
		//   player = pMeshData->Instantiate();

		//   player->SetName(L"Player");
		//   player->MeshRender()->SetFrustumCheck(false);
		//   CP_FSMScript* pFSM = new CP_FSMScript();
		//   player->AddComponent(pFSM);

		//   // rigidbody 에 전달할 데이터 미리 초기화
		//   Vec3 playerStartPos = Vec3(0, 0.f, 0);
		//   player->Transform()->SetRelativePos(playerStartPos);
		//   player->SetLayerIdx((UINT)LAYER_TYPE::Player);

		//   //// 쉐이프 정의 및 등록
		//   player->AddComponent(new CRigidBody);
		//   tShapeInfo info = {};                        // foot
		//   info.eGeomType = GEOMETRY_TYPE::Sphere;
		//   info.size = Vector3(15.f, 15.f, 15.f);
		//   info.CollideType = (UINT)COLLIDE_TYPE::Player;
		//   player->RigidBody()->PushBackShapeInfo(info);
		//   info = {};                                 // head
		//   info.eGeomType = GEOMETRY_TYPE::Sphere;
		//   info.size = Vector3(35.f, 35.f, 35.f);
		//   info.CollideType = (UINT)COLLIDE_TYPE::Player;
		//   player->RigidBody()->PushBackShapeInfo(info);
		//   info = {};                                 // body
		//   info.eGeomType = GEOMETRY_TYPE::Sphere;
		//   info.size = Vector3(55.f, 55.f, 55.f);
		//   info.CollideType = (UINT)COLLIDE_TYPE::Player;
		//   player->RigidBody()->PushBackShapeInfo(info);
		//   // 피지컬 등록하기
		//   player->RigidBody()->SetPhysical(ACTOR_TYPE::Dynamic);

		//   player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Y, true);
		//   player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_X, true);
		//   player->RigidBody()->SetFreezeRotation(FreezeRotationFlag::ROTATION_Z, true);
		//   player->RigidBody()->GetRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		//   
		//   ////////////////////////
		//   //player->RigidBody()->SetShapeLocalPos(0, Vec3(5.f, 7.5f, 0.f));

		//   player->AddComponent(new CCollider3D);

		//   SpawnGameObject(player, playerStartPos, (UINT)LAYER_TYPE::Player);
		//}
	}
}