#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CRenderMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "components.h"

#include "CMaterial.h"
#include "CGraphicsShader.h"

#include "CRenderMgr.h"
#include "CMRT.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "CKeyMgr.h"
#include "CInstancingBuffer.h"


CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_Frustum(this)
	, m_fAspectRatio(1.f)
	, m_fScale(1.f)
	, m_Far(10000.f)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_iLayerMask(0)
	, m_iCamIdx(-1)
	, m_Fov(XM_PI / 3.f)
	, m_OrthoWidth(0.f)
	, m_OrthoHeight(0.f)
{
	SetName(L"Camera");

	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();
	m_fAspectRatio = vRenderResol.x / vRenderResol.y;

	m_OrthoWidth = vRenderResol.x;
	m_OrthoHeight = vRenderResol.y;
}

CCamera::CCamera(const CCamera& _Other)
	: CComponent(_Other)
	, m_Frustum(this)
	, m_fAspectRatio(_Other.m_fAspectRatio)
	, m_fScale(_Other.m_fScale)
	, m_Far(_Other.m_Far)
	, m_ProjType(_Other.m_ProjType)
	, m_iLayerMask(_Other.m_iLayerMask)
	, m_iCamIdx(-1)
	, m_Fov(_Other.m_Fov)
	, m_OrthoWidth(_Other.m_OrthoWidth)
	, m_OrthoHeight(_Other.m_OrthoHeight)
{
}

CCamera::~CCamera()
{
}

void CCamera::begin()
{
	if (-1 != m_iCamIdx)
	{
		CRenderMgr::GetInst()->RegisterCamera(this, m_iCamIdx);
	}
}

void CCamera::finaltick()
{
	CalcViewMat();

	CalcProjMat();
	m_Frustum.finaltick();
	CalRay();
}

void CCamera::CalRay()
{
	// ���콺 ������ ���ϴ� Ray ���ϱ�
	// SwapChain Ÿ���� ViewPort ����
	CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT tVP = pMRT->GetViewPort();

	//  ���� ���콺 ��ǥ
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	// ������ ī�޶��� ��ǥ�� �ݵ�� ������.
	m_ray.vStart = Transform()->GetWorldPos();

	// view space ������ ����
	m_ray.vDir.x = ((((vMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProj._31) / m_matProj._11;
	m_ray.vDir.y = (-(((vMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProj._32) / m_matProj._22;
	m_ray.vDir.z = 1.f;

	// world space ������ ����
	m_ray.vDir = XMVector3TransformNormal(m_ray.vDir, m_matViewInv);
	m_ray.vDir.Normalize();
}

void CCamera::CalcViewMat()
{
	// ==============
	// View ��� ���
	// ==============
	m_matView = XMMatrixIdentity();

	// ī�޶� ��ǥ�� �������� �̵�
	Vec3 vCamPos = Transform()->GetRelativePos();
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// ī�޶� �ٶ󺸴� ������ Z ��� �����ϰ� ����� ȸ�� ����� ����
	Matrix matViewRot = XMMatrixIdentity();

	Vec3 vR = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matViewRot._11 = vR.x;	matViewRot._12 = vU.x;	matViewRot._13 = vF.x;
	matViewRot._21 = vR.y;	matViewRot._22 = vU.y;	matViewRot._23 = vF.y;
	matViewRot._31 = vR.z;	matViewRot._32 = vU.z;	matViewRot._33 = vF.z;

	m_matView = matViewTrans * matViewRot;


	// View ����� ���ϱ�
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
}
void CCamera::CalcProjMat()
{
	// =============
	// ���� ��� ���
	// =============
	m_matProj = XMMatrixIdentity();

	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// ���� ����
		m_matProj = XMMatrixOrthographicLH(m_OrthoWidth * (1.f / m_fScale), m_OrthoHeight * (1.f / m_fScale), 1.f, 10000.f);
	}
	else
	{
		// ���� ����
		m_matProj = XMMatrixPerspectiveFovLH(m_Fov, m_fAspectRatio, 1.f, m_Far);
	}

	// ������� ����� ���ϱ�
	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);
}

void CCamera::updateMatrix()
{
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;

	g_transform.matProj = m_matProj;
	g_transform.matProjInv = m_matProjInv;
}

void CCamera::SetLayerMask(int _iLayer, bool _Visible)
{
	if (_Visible)
	{
		m_iLayerMask |= 1 << _iLayer;
	}
	else
	{
		m_iLayerMask &= ~(1 << _iLayer);
	}
}
void CCamera::SetLayerMaskAll(bool _Visible)
{
	if (_Visible)
		m_iLayerMask = 0xffffffff;
	else
		m_iLayerMask = 0;
}

void CCamera::SetCameraIndex(int _idx)
{
	m_iCamIdx = _idx;
	CRenderMgr::GetInst()->RegisterCamera(this, m_iCamIdx);
}

void CCamera::SortObject()
{
	clear();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	for (UINT layerIdx = 0; layerIdx < MAX_LAYER; ++layerIdx)
	{
		if (m_iLayerMask & (1 << layerIdx))
		{
			CLayer* pLayer = pCurLevel->GetLayer(layerIdx);
			const vector<CGameObject*>& vecObject = pLayer->GetObjects();

			for (size_t objIdx = 0; objIdx < vecObject.size(); ++objIdx)
			{
				CRenderComponent* pRenderCom = vecObject[objIdx]->GetRenderComponent();

				if (nullptr == pRenderCom)
					continue;

				Vec3 objPos = vecObject[objIdx]->Transform()->GetWorldPos();
				float radius = vecObject[objIdx]->Transform()->GetSphereRadius();
				if (pRenderCom->IsUseFrustumCheck()
					&& false == m_Frustum.FrustumCheckBound(objPos, radius))
					continue;

				UINT iMtrlCount = pRenderCom->GetMtrlCount();
				for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
				{
					// ������ ���ų�, ������ ���̴��� ������ �ȵ� ���
					if (nullptr == pRenderCom->GetMaterial(iMtrl)
						|| nullptr == pRenderCom->GetMaterial(iMtrl)->GetShader())
					{
						continue;
					}

					// ���̴� �����ο� ���� �з�					
					SHADER_DOMAIN eDomain = pRenderCom->GetMaterial(iMtrl)->GetShader()->GetDomain();
					Ptr<CGraphicsShader> pShader = pRenderCom->GetMaterial(iMtrl)->GetShader();

					switch (eDomain)
					{
					case SHADER_DOMAIN::DOMAIN_DEFERRED:
					case SHADER_DOMAIN::DOMAIN_DEFERRED_DECAL:
					case SHADER_DOMAIN::DOMAIN_OPAQUE:
					case SHADER_DOMAIN::DOMAIN_MASK:
					{
						// Shader �� Pov �� ���� �ν��Ͻ� �׷��� �з�
						map<ULONG64, vector<tInstObj>>* pMap = NULL;
						Ptr<CMaterial> pMtrl = pRenderCom->GetMaterial(iMtrl);
						if (SHADER_DOMAIN::DOMAIN_DEFERRED == pShader->GetDomain())
						{
							pMap = &m_mapInstGroup_D;
						}
						else if (SHADER_DOMAIN::DOMAIN_OPAQUE == pShader->GetDomain()
							|| SHADER_DOMAIN::DOMAIN_MASK == pShader->GetDomain())
						{
							pMap = &m_mapInstGroup_F;
						}
						else
						{
							assert(nullptr);
							continue;
						}

						uInstID uID = {};
						uID.llID = pRenderCom->GetInstID(iMtrl);

						// ID �� 0 �� ==> Mesh �� Material �� ���õ��� �ʾҴ�.
						if (0 == uID.llID)
							continue;

						map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
						if (iter == pMap->end())
						{
							pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{ vecObject[objIdx], iMtrl }}));
						}
						else
						{
							iter->second.push_back(tInstObj{ vecObject[objIdx], iMtrl });
						}
					}
					break;
					case SHADER_DOMAIN::DOMAIN_DECAL:
						m_vecDecal.push_back(vecObject[objIdx]);
						break;
					case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
						m_vecTransparent.push_back(vecObject[objIdx]);
						break;
					case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
						m_vecPost.push_back(vecObject[objIdx]);
						break;
					case SHADER_DOMAIN::DOMAIN_UI:
						m_vecUI.push_back(vecObject[objIdx]);
						break;
					}
				}
			}
		}
	}
}
void CCamera::SortObject_Shadow()
{
	clear_shadow();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	for (UINT layerIdx = 0; layerIdx < MAX_LAYER; ++layerIdx)
	{
		if (m_iLayerMask & (1 << layerIdx))
		{
			CLayer* pLayer = pCurLevel->GetLayer(layerIdx);
			const vector<CGameObject*>& vecObject = pLayer->GetObjects();

			// ���⼭ �ν��Ͻ� �ؾ���
			for (size_t objIdx = 0; objIdx < vecObject.size(); ++objIdx)
			{
				CRenderComponent* pRenderCom = vecObject[objIdx]->GetRenderComponent();

				if (nullptr == pRenderCom)
					continue;

				Vec3 objPos = vecObject[objIdx]->Transform()->GetWorldPos();
				float radius = vecObject[objIdx]->Transform()->GetSphereRadius();
				if (pRenderCom->IsUseFrustumCheck()
					&& false == m_Frustum.FrustumCheckBound(objPos, radius))
					continue;

				UINT iMtrlCount = pRenderCom->GetMtrlCount();
				for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
				{
					// ������ ���ų�, ������ ���̴��� ������ �ȵ� ���
					if (nullptr == pRenderCom->GetMaterial(iMtrl)
						|| nullptr == pRenderCom->GetMaterial(iMtrl)->GetShader())
					{
						continue;
					}

					// ���̴� �����ο� ���� �з�
					SHADER_DOMAIN eDomain = pRenderCom->GetMaterial(iMtrl)->GetShader()->GetDomain();
					Ptr<CGraphicsShader> pShader = pRenderCom->GetMaterial(iMtrl)->GetShader();

					switch (eDomain)
					{
					case SHADER_DOMAIN::DOMAIN_DEFERRED:
					case SHADER_DOMAIN::DOMAIN_DEFERRED_DECAL:
					case SHADER_DOMAIN::DOMAIN_OPAQUE:
					case SHADER_DOMAIN::DOMAIN_MASK:
					{
						// Shader �� Pov �� ���� �ν��Ͻ� �׷��� �з�
						map<ULONG64, vector<tInstObj>>* pMap = NULL;
						Ptr<CMaterial> pMtrl = pRenderCom->GetMaterial(iMtrl);
						if (SHADER_DOMAIN::DOMAIN_DEFERRED == pShader->GetDomain()
							|| SHADER_DOMAIN::DOMAIN_OPAQUE == pShader->GetDomain()
							|| SHADER_DOMAIN::DOMAIN_MASK == pShader->GetDomain())
						{
							pMap = &m_mapInstGroup_S;
						}
						else
						{
							assert(nullptr);
							continue;
						}

						uInstID uID = {};
						uID.llID = pRenderCom->GetInstID(iMtrl);

						// ID �� 0 �� ==> Mesh �� Material �� ���õ��� �ʾҴ�.
						if (0 == uID.llID)
							continue;

						map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
						if (iter == pMap->end())
						{
							pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{ vecObject[objIdx], iMtrl }}));
						}
						else
						{
							iter->second.push_back(tInstObj{ vecObject[objIdx], iMtrl });
						}
					}
					break;
					}
				}
			}
		}
	}
}

void CCamera::render()
{
	geometryRender();
	lightRender();
	mergeRender();

	render_forward();
	render_decal();
	render_transparent();

	render_postprocess();
	render_ui();
}
void CCamera::render_shadowmap()
{
	updateMatrix();

	Ptr<CMaterial> pShadowMapMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");

	for (auto& pair : m_mapSingleObj_S)
	{
		pair.second.clear();
	}

	tInstancingData tInstData = {};
	for (auto& pair : m_mapInstGroup_S)
	{
		if (pair.second.empty())
			continue;

		// �ν��Ͻ� ���� (���н� �̱� ������Ʈ)
		if (pair.second.size() <= 1 || pair.second[0].pObj->Animator2D()
			|| nullptr == pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst())
		{
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj_S.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj_S.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj_S.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		// �ν��Ͻ� ��ü�κ��� Obj, Mesh, Mtrl ������
		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		//Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);
		CInstancingBuffer::GetInst()->Clear();

		// �ν��Ͻ̿� �ʿ��� ������ �Է�
		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->UpdateData();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pShadowMapMtrl->SetAnim3D(true);
			pShadowMapMtrl->SetBoneCount(pMesh->GetMTBoneCount());
		}

		pShadowMapMtrl->UpdateData_Inst();
		pMesh->render_instancing(pair.second[0].iMtrlIdx);

		if (bHasAnim3D)
		{
			pShadowMapMtrl->SetAnim3D(false);
			pShadowMapMtrl->SetBoneCount(0);
		}
	}

	for (auto& pair : m_mapSingleObj_S)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->UpdateData();

		for (auto& instObj : pair.second)
		{
			if (0 == instObj.iMtrlIdx)
				instObj.pObj->render_shadowmap();
			else
				instObj.pObj->render_shadowmap(instObj.iMtrlIdx);

		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}

void CCamera::clear()
{
	for (auto& pair : m_mapInstGroup_D)
		pair.second.clear();
	for (auto& pair : m_mapInstGroup_F)
		pair.second.clear();

	m_vecOpaque.clear();
	m_vecMask.clear();
	m_vecDecal.clear();
	m_vecTransparent.clear();
	m_vecPost.clear();
	m_vecUI.clear();
}
void CCamera::clear_shadow()
{
	for (auto& pair : m_mapInstGroup_S)
		pair.second.clear();
	for (auto& pair : m_mapSingleObj_S)
		pair.second.clear();

	m_vecShadow.clear();
}

void CCamera::render_deferred()
{
	updateMatrix();

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	tInstancingData tInstData = {};
	for (auto& pair : m_mapInstGroup_D)
	{
		// �׷� ������Ʈ�� ���ų�, ���̴��� ���� ���
		if (pair.second.empty())
			continue;

		// instancing ���� ���� �����̰ų�
		// Animation2D ������Ʈ�ų�(��������Ʈ �ִϸ��̼� ������Ʈ)
		// Shader �� Instancing �� �������� �ʴ°��
		if (pair.second.size() <= 1 || pair.second[0].pObj->Animator2D()
			|| nullptr == pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst())
		{
			// �ش� ��ü���� ���� ���������� ��ȯ
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

		// Instancing ���� Ŭ����
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->UpdateData();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// �ν��Ͻ̿� �ʿ��� �����͸� ����(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh �˸���
			pMtrl->SetBoneCount(pMesh->GetMTBoneCount());
		}

		pMtrl->UpdateData_Inst();
		pMesh->render_instancing(pair.second[0].iMtrlIdx);

		// ����
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh �˸���
			pMtrl->SetBoneCount(0);
		}
	}

	// ���� ������
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->UpdateData();

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}

void CCamera::render_forward()
{
	updateMatrix();

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	tInstancingData tInstData = {};
	for (auto& pair : m_mapInstGroup_F)
	{
		// �׷� ������Ʈ�� ���ų�, ���̴��� ���� ���
		if (pair.second.empty())
			continue;

		// instancing ���� ���� �̸��̰ų�
		// Animation2D ������Ʈ�ų�(��������Ʈ �ִϸ��̼� ������Ʈ)
		// Shader �� Instancing �� �������� �ʴ°��
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// �ش� ��ü���� ���� ���������� ��ȯ
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

		// Instancing ���� Ŭ����
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->UpdateData();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// �ν��Ͻ̿� �ʿ��� �����͸� ����(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh �˸���
			pMtrl->SetBoneCount(pMesh->GetMTBoneCount());
		}

		pMtrl->UpdateData_Inst();
		pMesh->render_instancing(pair.second[0].iMtrlIdx);

		// ����
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh �˸���
			pMtrl->SetBoneCount(0);
		}
	}

	// ���� ������
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->UpdateData();

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}

void CCamera::geometryRender()
{
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->OMSet(true);
	render_deferred();

	// decal�� �ٸ� ��ü���� �׷��� ������ �׷�����
	/*CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED_DECAL)->OMSet();
	for (size_t i = 0; i < m_vecDeferredDecal.size(); ++i)
	{
		m_vecDeferredDecal[i]->render();
	}*/
}

void CCamera::lightRender()
{
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::LIGHT)->OMSet(false);

	const vector<CLight3D*>& vecLight3D = CRenderMgr::GetInst()->GetLight3D();
	for (size_t i = 0; i < vecLight3D.size(); ++i)
	{
		vecLight3D[i]->render();
	}
}

void CCamera::mergeRender()
{
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
	static Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	static Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeMtrl");
	static bool bSet = false;
	if (!bSet)
	{
		bSet = true;
		pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ColorTargetTex"));
		pMtrl->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex"));
		pMtrl->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"SpecularTargetTex"));
		pMtrl->SetTexParam(TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"EmissiveTargetTex"));
		pMtrl->SetTexParam(TEX_4, CResMgr::GetInst()->FindRes<CTexture>(L"ShadowTargetTex"));
	}
	pMtrl->UpdateData();
	pRectMesh->render(0);
}

void CCamera::render_decal()
{
	updateMatrix();
	for (size_t i = 0; i < m_vecDecal.size(); ++i)
	{
		m_vecDecal[i]->render();
	}
}

void CCamera::render_transparent()
{
	updateMatrix();
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->render();
	}
}

void CCamera::render_postprocess()
{
	updateMatrix();
	for (size_t i = 0; i < m_vecPost.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyRenderTarget();
		m_vecPost[i]->render();
	}
}

void CCamera::render_ui()
{
	updateMatrix();
	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->render();
	}
}

void CCamera::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Frustum, sizeof(CFrustum), 1, _File);
	fwrite(&m_fAspectRatio, sizeof(float), 1, _File);
	fwrite(&m_fScale, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_ProjType, sizeof(UINT), 1, _File);
	fwrite(&m_iLayerMask, sizeof(UINT), 1, _File);
	fwrite(&m_iCamIdx, sizeof(int), 1, _File);

}

void CCamera::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Frustum, sizeof(CFrustum), 1, _File);
	m_Frustum.SetCamera(this);

	fread(&m_fAspectRatio, sizeof(float), 1, _File);
	fread(&m_fScale, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_ProjType, sizeof(UINT), 1, _File);
	fread(&m_iLayerMask, sizeof(UINT), 1, _File);
	fread(&m_iCamIdx, sizeof(int), 1, _File);
}
