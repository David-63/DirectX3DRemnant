#include "pch.h"
#include "CLandScape.h"

#include "CResMgr.h"
#include "CTransform.h"

#include "CRenderMgr.h"
#include "CKeyMgr.h"
#include "CStructuredBuffer.h"
#include "CCamera.h"

CLandScape::CLandScape() : CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_iFaceX(1), m_iFaceZ(1), m_vBrushScale(0.2f, 0.2f)
{
	init();
}

CLandScape::~CLandScape()
{
	if (nullptr != m_pCrossBuffer)
		delete m_pCrossBuffer;

	if (nullptr != m_pWeightMapBuffer)
		delete m_pWeightMapBuffer;
}


void CLandScape::finaltick()
{
	if (KEY_TAP(KEY::NUM_0))
		m_eMod = LANDSCAPE_MOD::NONE;
	else if (KEY_TAP(KEY::NUM_1))
		m_eMod = LANDSCAPE_MOD::HEIGHT_MAP;
	else if (KEY_TAP(KEY::NUM_2))
		m_eMod = LANDSCAPE_MOD::SPLAT;

	if (LANDSCAPE_MOD::NONE == m_eMod)
	{
		return;
	}

	if (KEY_PRESSED(KEY::LBTN))
	{
		Raycasting();

		if (LANDSCAPE_MOD::HEIGHT_MAP == m_eMod)
		{
			// ���� ��ġ������ ���� ���̸� ���� ��
			m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // ��ŷ ������ HeightMapShader �� ����

			m_pCSHeightMap->SetBrushTex(m_pBrushTex);		// ����� �귯�� �ؽ��� ����
			m_pCSHeightMap->SetBrushIndex(0);				// �귯�� �ε��� ����
			m_pCSHeightMap->SetBrushScale(m_vBrushScale);   // �귯�� ũ��
			m_pCSHeightMap->SetHeightMap(m_HeightMap);
			m_pCSHeightMap->Execute();
		}
		else if (LANDSCAPE_MOD::SPLAT == m_eMod)
		{
			// ���� ��ġ������ ���� ����ġ�� ������	
			m_pCSWeightMap->SetInputBuffer(m_pCrossBuffer); // ���� ĳ��Ʈ ��ġ
			m_pCSWeightMap->SetBrushArrTex(m_pBrushTex);
			m_pCSWeightMap->SetBrushIndex(0);
			m_pCSWeightMap->SetBrushScale(m_vBrushScale); // �귯�� ũ��
			m_pCSWeightMap->SetWeightMap(m_pWeightMapBuffer, m_iWeightWidth, m_iWeightHeight); // ����ġ��, ���� ���� ����			
			m_pCSWeightMap->SetWeightIdx(m_iWeightIdx);
			m_pCSWeightMap->Execute();
		}
	}
}

void CLandScape::render()
{
	if (GetMesh() == nullptr || GetMaterial(0) == nullptr)
		return;

	Transform()->UpdateData();

	GetMaterial(0)->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);

	GetMaterial(0)->SetScalarParam(INT_0, &m_iFaceX);
	GetMaterial(0)->SetScalarParam(INT_1, &m_iFaceZ);
	GetMaterial(0)->SetTexParam(TEX_2, m_HeightMap);

	Vec2 vResolution = Vec2(m_HeightMap->Width(), m_HeightMap->Height());
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, &vResolution);
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_2, m_HeightMap);

	// ����ġ ���� ����
	m_pWeightMapBuffer->UpdateData(17, PIPELINE_STAGE::PS_PIXEL);

	// ����ġ ���� �ػ� ����
	Vec2 vWeightMapResolution = Vec2((float)m_iWeightWidth, (float)m_iWeightHeight);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_1, &vWeightMapResolution);

	// Ÿ�� �迭 ���� ����
	float m_fTileCount = float(m_pTileArrTex->GetArraySize() / 2); // ����, �븻 �������־ ������ 2 ����
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, &m_fTileCount);

	// Ÿ�� �ؽ��� ����
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_ARR_0, m_pTileArrTex);

	// ���� ���ε�(������Ʈ)
	GetMaterial(0)->UpdateData();

	// ����
	GetMesh()->render(0);

	// ==========
	// ���ҽ� ����
	// ==========
	m_pWeightMapBuffer->Clear();
}


void CLandScape::SetFace(UINT _iFaceX, UINT _iFaceZ)
{
	m_iFaceX = _iFaceX;
	m_iFaceZ = _iFaceZ;

	CreateMesh();
}

void CLandScape::Raycasting()
{
	// ���� ī�޶� ������
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return;

	// ���� ���� ������ ������ ���÷� ����
	const Matrix& matWorldInv = Transform()->GetWorldInvMat();
	const tRay& ray = pMainCam->GetRay();

	tRay CamRay = {};
	CamRay.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// ������ ī�޶� Ray �� ������ ����
	tRaycastOut out = { Vec2(0.f, 0.f), 0x7fffffff, 0 };
	m_pCrossBuffer->SetData(&out, 1);

	m_pCSRaycast->SetHeightMap(m_HeightMap);
	m_pCSRaycast->SetFaceCount(m_iFaceX, m_iFaceZ);
	m_pCSRaycast->SetCameraRay(CamRay);
	m_pCSRaycast->SetOuputBuffer(m_pCrossBuffer);

	m_pCSRaycast->Execute();

	//m_pCrossBuffer->GetData(&out);
	//int a = 0;
}