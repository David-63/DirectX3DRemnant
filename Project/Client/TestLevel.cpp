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
#include <Script/CP_FSMScript.h>
#include <Script/CCameraMoveScript.h>
#include <Script/CShoulderViewScript.h>
#include <Script/CCharacterMoveScript.h>


void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);
	pCurLevel->SetName(L"TestLevel");

	// Layer �̸�����
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Background");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Objects");
	pCurLevel->GetLayer(4)->SetName(L"Ground");
	pCurLevel->GetLayer(5)->SetName(L"Box");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");

	// camera
	{
		// Main Camera Object ����
		CGameObject* pMainCam = new CGameObject;
		pMainCam->SetName(L"MainCamera");
		pMainCam->AddComponent(new CTransform);
		pMainCam->AddComponent(new CCamera);
		pMainCam->AddComponent(new CCameraMoveScript);
		// ������
		//pMainCam->AddComponent(new CShoulderViewScript);
		//CShoulderViewScript* pCamMoveScript = pMainCam->GetScript<CShoulderViewScript>(); 
		pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		pMainCam->Camera()->SetCameraIndex(0);		// MainCamera �� ����
		pMainCam->Camera()->SetLayerMaskAll(true);	// ��� ���̾� üũ
		pMainCam->Camera()->SetLayerMask(31, false);// UI Layer �� ���������� �ʴ´�.
		pMainCam->Camera()->SetFar(20000.f);
		SpawnGameObject(pMainCam, Vec3(200.f, 133.f, -500.f), 0);

		// UI cameara
		CGameObject* pUICam = new CGameObject;
		pUICam->SetName(L"UICamera");
		pUICam->AddComponent(new CTransform);
		pUICam->AddComponent(new CCamera);
		pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		pUICam->Camera()->SetCameraIndex(1);		// Sub ī�޶�� ����
		pUICam->Camera()->SetLayerMask(31, true);	// 31�� ���̾ üũ
		SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);
	}
	

	// SkyBox �߰�
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

	// ���� �߰�
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

		// �ν��Ͻ� �׽�Ʈ
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\player\\P_MoveWalk.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Player");
		pObj->AddComponent(new CP_FSMScript);

		SpawnGameObject(pObj, Vec3(40.f, 100.f, 40.f), 2);
	}	
}