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
	m_LightInfo = _origin.m_LightInfo;
	SetLightType(static_cast<LIGHT_TYPE>(m_LightInfo.LightType));
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

	// 광원에 부착한 카메라 오브젝트도 위치를 광원 위치랑 동일하게..
	// finaltick 호출시켜서 카메라 오브젝트의 카메라 컴포넌트의 view, proj 행렬 연산할수 있게 함
	*m_CamObj->Transform() = *Transform(); // 행렬정보 전달
	m_CamObj->finaltick_module();
}

void CLight3D::render()
{
	Transform()->UpdateData();
	
	// Light 재질 업데이트
	m_Mtrl->SetScalarParam(INT_0, &m_LightIdx);

	if ((UINT)LIGHT_TYPE::DIRECTIONAL == m_LightInfo.LightType)
	{
		Matrix lightVPmat = m_CamObj->Camera()->GetViewMat() * m_CamObj->Camera()->GetProjMat();
		m_Mtrl->SetScalarParam(MAT_0, &lightVPmat);
		m_Mtrl->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"DynamicShadowMapTex"));
	}
	m_Mtrl->UpdateData();

	// 볼륨 메시 렌더
	m_Mesh->render(0);
}

void CLight3D::render_shadowmap()
{
	m_CamObj->Camera()->SortObject_Shadow();
	m_CamObj->Camera()->render_shadowmap();	// dynamicShadowmap Tex에 자기 자신의 거리(깊이) 남기기
}


void CLight3D::SetRadius(float _Radius)
{
	m_LightInfo.Radius = _Radius;

	// SphereMesh 의 로컬 반지름이 0.5f 이기 때문에 2배로 적용
	Transform()->SetRelativeScale(Vec3(_Radius * 2.f, _Radius * 2.f, _Radius * 2.f));
}


void CLight3D::SetLightType(LIGHT_TYPE _type)
{
	m_LightInfo.LightType = (int)_type;

	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		// 광원을 렌더링 할 때, 광원의 영향범위를 형상화 할 수 있는 메쉬(볼륨메쉬) 를 선택
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl");
		m_CamObj->Camera()->SetFar(200000.f);
		m_CamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_CamObj->Camera()->SetOrthoWidth(10000.f);
		m_CamObj->Camera()->SetOrthoHeight(10000.f);

	}

	else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	}
	// ConeMesh 랑 SpotLight는 없어서 UI패널조작에서 막아놓음
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
	fwrite(&m_isShowRange, sizeof(bool), 1, _File);
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_LightInfo, sizeof(tLightInfo), 1, _File);
	fread(&m_isShowRange, sizeof(bool), 1, _File);

	SetLightType(static_cast<LIGHT_TYPE>(m_LightInfo.LightType));
}
