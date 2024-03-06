#include "pch.h"
#include "CLandScape.h"

#include "CResMgr.h"
#include "CTransform.h"

#include "CRenderMgr.h"
#include "CKeyMgr.h"
#include "CStructuredBuffer.h"
#include "CCamera.h"


int CLandScape::m_makeCnt = 0;

CLandScape::CLandScape() : CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_FaceSize(1, 1), m_vBrushScale(0.2f, 0.2f), m_eMod(LANDSCAPE_MOD::NONE)
{
	// init ���־���
	init();
	SetFrustumCheck(false);
	//SetFace(64, 64);
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
	else if (KEY_TAP(KEY::NUM_3))
	{
		m_iWeightIdx++;
		if (3 <= m_iWeightIdx)
			m_iWeightIdx = 0;
	}

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

	//GetMaterial(0)->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);

	GetMaterial(0)->SetScalarParam(INT_0, &m_FaceSize.X);
	GetMaterial(0)->SetScalarParam(INT_1, &m_FaceSize.Y);
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

	// Camera World Pos ����
	Vec3 vCamWorldPos = CRenderMgr::GetInst()->GetMainCam()->Transform()->GetWorldPos();
	GetMaterial(0)->SetScalarParam(VEC4_0, &vCamWorldPos);

	// ���� ���ε�(������Ʈ)
	GetMaterial(0)->UpdateData();

	// ����
	GetMesh()->render(0);

	// ==========
	// ���ҽ� ����
	// ==========
	m_pWeightMapBuffer->Clear();
}

void CLandScape::render(UINT _iSubset)
{
	render();
}

void CLandScape::SaveHeightMap()
{
	wstring strResKey = L"texture\\landscape\\";

	wstring meshName(m_heightMapName.begin(), m_heightMapName.end());
	strResKey += meshName;
	strResKey += L".tex";
	m_HeightMap->Save(strResKey);
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
	m_pCSRaycast->SetFaceCount(m_FaceSize);
	m_pCSRaycast->SetCameraRay(CamRay);
	m_pCSRaycast->SetOuputBuffer(m_pCrossBuffer);

	m_pCSRaycast->Execute();

	//m_pCrossBuffer->GetData(&out);
	//int a = 0;
}

void CLandScape::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);
	fwrite(&m_FaceSize, sizeof(tUINTS), 1, _File);
	fwrite(&m_vBrushScale, sizeof(Vec2), 1, _File);
	SaveResRef(m_pBrushTex.Get(), _File);
	SaveWString(m_pCSRaycast.Get()->GetKey(), _File);
	SaveWString(m_pCSHeightMap.Get()->GetKey(), _File);
	SaveResRef(m_HeightMap.Get(), _File);
	SaveWString(m_pCSWeightMap.Get()->GetKey(), _File);

	fwrite(&m_iWeightWidth, sizeof(UINT), 1, _File);
	fwrite(&m_iWeightHeight, sizeof(UINT), 1, _File);
	fwrite(&m_iWeightIdx, sizeof(UINT), 1, _File);
	fwrite(&m_eMod, sizeof(LANDSCAPE_MOD), 1, _File);
	SaveResRef(m_pTileArrTex.Get(), _File);
}

void CLandScape::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);
	fread(&m_FaceSize, sizeof(tUINTS), 1, _File);
	fread(&m_vBrushScale, sizeof(Vec2), 1, _File);
	LoadResRef(m_pBrushTex, _File);

	wstring CSRaycastName;
	LoadWString(CSRaycastName, _File);
	m_pCSRaycast = (CRaycastShader*)CResMgr::GetInst()->FindRes<CComputeShader>(CSRaycastName).Get();

	wstring CSHeightMapName;
	LoadWString(CSHeightMapName, _File);
	m_pCSHeightMap = (CHeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(CSHeightMapName).Get();

	LoadResRef(m_HeightMap, _File);
	
	wstring CSWeightMapName;
	LoadWString(CSWeightMapName, _File);
	m_pCSWeightMap = (CWeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(CSWeightMapName).Get();


	fread(&m_iWeightWidth, sizeof(UINT), 1, _File);
	fread(&m_iWeightHeight, sizeof(UINT), 1, _File);
	fread(&m_iWeightIdx, sizeof(UINT), 1, _File);
	fread(&m_eMod, sizeof(LANDSCAPE_MOD), 1, _File);
	LoadResRef(m_pTileArrTex, _File);

	if (!m_pCrossBuffer)
		m_pCrossBuffer = new CStructuredBuffer;
	m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true);

	if (!m_pWeightMapBuffer)
		m_pWeightMapBuffer = new CStructuredBuffer;
	m_pWeightMapBuffer->Create(sizeof(tWeight_4), m_iWeightWidth * m_iWeightHeight, SB_TYPE::READ_WRITE, false);
}
