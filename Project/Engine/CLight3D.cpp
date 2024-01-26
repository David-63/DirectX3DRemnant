#include "pch.h"
#include "CLight3D.h"

#include "CResMgr.h"

#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCamera.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_LightIdx(-1)
	, m_isShowRange(true)
	, m_CamObj(nullptr)
{
	m_CamObj = new CGameObject();
	m_CamObj->AddComponent(new CTransform());
	m_CamObj->AddComponent(new CCamera());
	m_CamObj->Camera()->SetLayerMaskAll(true);
	m_CamObj->Camera()->SetLayerMask(31, false);

	SetLightType(LIGHT_TYPE::DIRECTIONAL);
}

CLight3D::CLight3D(const CLight3D& _origin)
	: CComponent(_origin)
	, m_LightIdx(-1)
	, m_isShowRange(_origin.m_isShowRange)
	, m_CamObj(nullptr)
{
	m_CamObj = new CGameObject(*_origin.m_CamObj);
}

CLight3D::~CLight3D()
{
	if (nullptr != m_CamObj)
		delete m_CamObj;
}

void CLight3D::finaltick()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	
	m_LightIdx = (UINT)CRenderMgr::GetInst()->RegisterLight3D(this, m_LightInfo);

	if (m_isShowRange)
	{
		if ((UINT)LIGHT_TYPE::POINT == m_LightInfo.LightType)
		{
			DrawDebugSphere(Transform()->GetWorldMat(), Vec4(0.2f, 1.f, 0.2f, 1.f), 0.f, true);
		}
		else if ((UINT)LIGHT_TYPE::SPOT == m_LightInfo.LightType)
		{
			//DrawDebugCone(Transform()->GetWorldMat(), Vec4(0.2f, 1.f, 0.2f, 1.f), 0.f, true);
		}
	}

	// ������ ������ ī�޶� ������Ʈ�� ��ġ�� ���� ��ġ�� �����ϰ�..
	// finaltick ȣ����Ѽ� ī�޶� ������Ʈ�� ī�޶� ������Ʈ�� view, proj ��� �����Ҽ� �ְ� ��
	*m_CamObj->Transform() = *Transform(); // ������� ����
	m_CamObj->finaltick_module();
}

void CLight3D::render()
{
	Transform()->UpdateData();
	
	// Light ���� ������Ʈ
	m_Mtrl->SetScalarParam(INT_0, &m_LightIdx);

	if ((UINT)LIGHT_TYPE::DIRECTIONAL == m_LightInfo.LightType)
	{
		Matrix lightVPmat = m_CamObj->Camera()->GetViewMat() * m_CamObj->Camera()->GetProjMat();
		m_Mtrl->SetScalarParam(MAT_0, &lightVPmat);
		m_Mtrl->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"DynamicShadowMapTex"));
	}
	m_Mtrl->UpdateData();

	// ���� �޽� ����
	m_Mesh->render(0);
}

void CLight3D::render_shadowmap()
{
	m_CamObj->Camera()->SortObject_Shadow();
	m_CamObj->Camera()->render_shadowmap();
}


void CLight3D::SetRadius(float _Radius)
{
	m_LightInfo.Radius = _Radius;

	// SphereMesh �� ���� �������� 0.5f �̱� ������ 2��� ����
	Transform()->SetRelativeScale(Vec3(_Radius * 2.f, _Radius * 2.f, _Radius * 2.f));
}


void CLight3D::SetLightType(LIGHT_TYPE _type)
{
	m_LightInfo.LightType = (int)_type;

	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		// ������ ������ �� ��, ������ ��������� ����ȭ �� �� �ִ� �޽�(�����޽�) �� ����
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl");
		m_CamObj->Camera()->SetFar(100000.f);
		m_CamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_CamObj->Camera()->SetOrthoWidth(16000.f);
		m_CamObj->Camera()->SetOrthoHeight(16000.f);

	}

	else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	}

	else 
	{		
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"SpotLightMtrl");
	}

	if (nullptr != m_Mtrl)
	{
		m_Mtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex"));
		m_Mtrl->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	}
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_LightInfo, sizeof(tLightInfo), 1, _File);
	SaveResRef(m_Mesh.Get(), _File);
	SaveResRef(m_Mtrl.Get(), _File);
	fwrite(&m_LightIdx, sizeof(UINT), 1, _File);
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_LightInfo, sizeof(tLightInfo), 1, _File);
	LoadResRef(m_Mesh, _File);
	LoadResRef(m_Mtrl, _File);
	fread(&m_LightIdx, sizeof(UINT), 1, _File);
}

