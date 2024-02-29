#include "pch.h"
#include "CLandScape.h"

#include "CResMgr.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"



void CLandScape::init()
{
	CreateMesh();

	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LandScapeMtrl"), 0);

	CreateComputeShader();

	CreateTexture();

	// ����ĳ���� ��� �޴� ����
	m_pCrossBuffer = new CStructuredBuffer;
	m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true);

	// Ÿ�� �ؽ���(Color, Normal ȥ��, �� 6��)	
	//m_pTileArrTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_ARRR.dds", L"texture\\tile\\TILE_ARRR.dds");
	//m_pTileArrTex = CResMgr::GetInst()->LoadTexture(L"texture\\tile\\TILE_ARRR.dds", L"texture\\tile\\TILE_ARRR.dds", 8);
	m_pTileArrTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_ARRR.dds");
	m_pTileArrTex->GenerateMip(8);
}

void CLandScape::CreateMesh()
{
	Vtx v;
	vector<Vtx> vecVtx;

	for (UINT i = 0; i < m_iFaceZ + 1; ++i)
	{
		for (UINT j = 0; j < m_iFaceX + 1; ++j)
		{
			v.vPos = Vec3((float)j, 0.f, (float)i);
			v.vUV = Vec2((float)j, (float)m_iFaceZ - i);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

			vecVtx.push_back(v);
		}
	}

	vector<UINT> vecIdx;

	for (UINT i = 0; i < m_iFaceZ; ++i)
	{
		for (UINT j = 0; j < m_iFaceX; ++j)
		{
			// 0
			// | \
			// 2--1  
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + (j));
			vecIdx.push_back((m_iFaceX + 1) * (i)+(j + 1));
			vecIdx.push_back((m_iFaceX + 1) * (i)+(j));

			// 1--2
			//  \ |
			//    0
			vecIdx.push_back((m_iFaceX + 1) * (i)+(j + 1));
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + (j));
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + (j + 1));
		}
	}


	Ptr<CMesh> pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());

	wstring meshName = L"LandMesh";
	meshName += m_makeCnt;
	CResMgr::GetInst()->AddRes(meshName, pMesh);

	//pMesh->SetName(L"LandMesh");
	//pMesh->SetKey(L"LandMesh");
	//pMesh->SetRelativePath(L"LandMesh");
	SetMesh(pMesh);
	// Mesh �缳���ϰ� ���� ������ ���󰡱� ������ �ٽ� ����
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LandScapeMtrl"), 0);
	m_makeCnt++;
}

void CLandScape::CreateComputeShader()
{
	// ======================
	// ���� ���� ��ǻƮ ���̴�
	// ======================
	m_pCSHeightMap = (CHeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"HeightMapShader").Get();
	if (nullptr == m_pCSHeightMap)
	{
		m_pCSHeightMap = new CHeightMapShader(32, 32, 1);
		m_pCSHeightMap->CreateComputeShader(L"shader\\heightmap.fx", "CS_HeightMap");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"HeightMapShader", m_pCSHeightMap.Get());
	}

	// =====================
	// ���� ��ŷ ��ǻƮ ���̴�
	// =====================
	m_pCSRaycast = (CRaycastShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"RaycastShader").Get();
	if (nullptr == m_pCSRaycast)
	{
		m_pCSRaycast = new CRaycastShader(32, 32, 1);
		m_pCSRaycast->CreateComputeShader(L"shader\\raycast.fx", "CS_Raycast");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"RaycastShader", m_pCSRaycast.Get());
	}

	// =======================
	// ����ġ ���� ��ǻƮ ���̴�
	// =======================
	m_pCSWeightMap = (CWeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"WeightMapShader").Get();
	if (nullptr == m_pCSWeightMap)
	{
		m_pCSWeightMap = new CWeightMapShader(32, 32, 1);
		m_pCSWeightMap->CreateComputeShader(L"shader\\weightmap.fx", "CS_WeightMap");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"WeightMapShader", m_pCSWeightMap.Get());
	}
}

void CLandScape::CreateTexture()
{
	// ���̸� �ؽ���		
	m_HeightMap = CResMgr::GetInst()->FindRes<CTexture>(L"HeightMap");
	if (nullptr == m_HeightMap)
	{
		m_HeightMap = CResMgr::GetInst()->CreateTexture(L"HeightMap"
			, 2048, 2048
			, DXGI_FORMAT_R32_FLOAT
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
			, D3D11_USAGE_DEFAULT);
	}
	
	m_pBrushTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\brush\\Brush_01.png");


	// ����ġ ����
	m_iWeightWidth = 1024;
	m_iWeightHeight = 1024;

	m_pWeightMapBuffer = new CStructuredBuffer;
	m_pWeightMapBuffer->Create(sizeof(tWeight_4), m_iWeightWidth * m_iWeightHeight, SB_TYPE::READ_WRITE, false);

}
