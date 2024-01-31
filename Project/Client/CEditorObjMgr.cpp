#include "pch.h"
#include "CEditorObjMgr.h"



#include "CGameObjectEx.h"
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CKeyMgr.h>

#include <Script\CCameraMoveScript.h>

CEditorObjMgr::CEditorObjMgr()
	: m_DebugShape{}
{
}

CEditorObjMgr::~CEditorObjMgr()
{
	Safe_Del_Vec(m_vecEditorObj);
	Safe_Del_Array(m_DebugShape);
}

void CEditorObjMgr::init()
{
	// ����� ������ ����
	m_DebugShape[(UINT)SHAPE_TYPE::RECT] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

	m_DebugShape[(UINT)SHAPE_TYPE::CUBE] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugSphereShapeMtrl"), 0);

	// EditorObject ����
	CGameObjectEx* pEditorCamObj = new CGameObjectEx;
	pEditorCamObj->AddComponent(new CTransform);
	pEditorCamObj->AddComponent(new CCamera);
	pEditorCamObj->AddComponent(new CCameraMoveScript);

	pEditorCamObj->Camera()->SetFar(100000.f);
	pEditorCamObj->Camera()->SetLayerMaskAll(true);
	pEditorCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	pEditorCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1500.f));
	m_vecEditorObj.push_back(pEditorCamObj);
	CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCamObj->Camera());
}



void CEditorObjMgr::progress()
{
	// DebugShape ���� ��������
	vector<tDebugShapeInfo>& vecInfo = CRenderMgr::GetInst()->GetDebugShapeInfo();
	m_DebugShapeInfo.insert(m_DebugShapeInfo.end(), vecInfo.begin(), vecInfo.end());
	vecInfo.clear();


	tick();

	render();
}


void CEditorObjMgr::tick()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}
}

void CEditorObjMgr::render()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->render();
	}



	// DebugShape Render
	CGameObjectEx* pShapeObj = nullptr;

	vector<tDebugShapeInfo>::iterator iter = m_DebugShapeInfo.begin();
			
	for (; iter != m_DebugShapeInfo.end();)
	{
		// ����� ��ü�� �����ϴ� ��� ã��
		{
			switch (iter->eShape)
			{
			case SHAPE_TYPE::RECT:
				pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::RECT];
				break;
			case SHAPE_TYPE::CIRCLE:
				pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE];
				break;
			case SHAPE_TYPE::CUBE:
				pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::CUBE];
				break;
			case SHAPE_TYPE::SPHERE:
				pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::SPHERE];
				break;
			}
		}

		// �ش� ��ü�� Transform ���� ������Ʈ
		{
			if (iter->matWorld != XMMatrixIdentity())
			{
				pShapeObj->Transform()->SetWorldMat(iter->matWorld);
			}
			else
			{
				pShapeObj->Transform()->SetRelativePos(iter->vWorldPos);
				pShapeObj->Transform()->SetRelativeScale(iter->vWorldScale);
				pShapeObj->Transform()->SetRelativeRot(iter->vWorldRotation);
				pShapeObj->finaltick();
			}
		}
		
		// ���� ���
		pShapeObj->MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_0, &iter->vColor);

		// ��ü�� ������ ����
		if (iter->bDepthTest)
			pShapeObj->MeshRender()->GetMaterial(0)->GetShader()->SetDSType(DS_TYPE::LESS);
		else
			pShapeObj->MeshRender()->GetMaterial(0)->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		// ������ ȣ��
		pShapeObj->render();


		iter->fCurTime += DT;
		if (iter->fMaxTime < iter->fCurTime)
		{
			iter = m_DebugShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
