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
	// Layer 이름설정
	{
		pCurLevel->ChangeState(LEVEL_STATE::STOP);
		pCurLevel->SetName(L"TestLevel");
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
	}

	// camera
	{
		// Main Camera Object 생성
		CGameObject* pMainCam = new CGameObject;
		pMainCam->SetName(L"MainCamera");
		pMainCam->AddComponent(new CTransform);
		pMainCam->AddComponent(new CCamera);
		pMainCam->AddComponent(new CCameraMoveScript);
		pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		pMainCam->Camera()->SetCameraIndex(0);
		pMainCam->Camera()->SetLayerMaskAll(true);
		pMainCam->Camera()->SetLayerMask(31, false);
		pMainCam->Camera()->SetFar(20000.f);
		SpawnGameObject(pMainCam, Vec3(200.f, 133.f, -500.f), 0);

		// UI cameara
		CGameObject* pUICam = new CGameObject;
		pUICam->SetName(L"UICamera");
		pUICam->AddComponent(new CTransform);
		pUICam->AddComponent(new CCamera);
		pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		pUICam->Camera()->SetCameraIndex(1);
		pUICam->Camera()->SetLayerMask(31, true);
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
		pLightObj->Light3D()->SetName(L"DirLightComp");
		pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
		pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
		pLightObj->Light3D()->SetLightAmbient(Vec3(0.5f, 0.5f, 0.5f));

		SpawnGameObject(pLightObj, Vec3(-2000, 2000.f, -2000.f), 0);
	}

	// LandScape Object
	{
		CGameObject* pLandScape = new CGameObject;
		pLandScape->SetName(L"LandScape");
		pLandScape->AddComponent(new CTransform);
		pLandScape->AddComponent(new CLandScape);
		pLandScape->Transform()->SetRelativeScale(Vec3(500.f, 1000.f, 500.f));
		//pLandScape->LandScape()->SetFaceSize(64, 64);
		//pLandScape->LandScape()->SetFaceName("sampleFace");
		//pLandScape->LandScape()->CreateMesh();
		//pLandScape->LandScape()->SetHeightMap(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\HeightMap_01.jpg"));
		SpawnGameObject(pLandScape, Vec3(0.f, 0.f, 0.f), 4);
	}

	// ============
	// FBX Loading
	// ============	
	{		
	}
	
	//ground
	{
	}
}