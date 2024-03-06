#include "pch.h"
#include "CLandScape.h"

#include "CResMgr.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"



void CLandScape::init()
{
	CreateComputeShader();

	CreateTexture();

	// 레이캐스팅 결과 받는 버퍼
	if (!m_pCrossBuffer)
	{
		m_pCrossBuffer = new CStructuredBuffer;
		m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true);
	}
	if (nullptr == m_pTileArrTex)
	{
		m_pTileArrTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_ARRR.dds");
		//m_pTileArrTex->GenerateMip(8);
	}
}

void CLandScape::CreateMesh()
{
	Vtx v;
	vector<Vtx> vecVtx;

	// 변수의 Y를 Z축으로 사용
	for (UINT i = 0; i < m_FaceSize.Y + 1; ++i)
	{
		for (UINT j = 0; j < m_FaceSize.X + 1; ++j)
		{
			v.vPos = Vec3((float)j, 0.f, (float)i);
			v.vUV = Vec2((float)j, (float)m_FaceSize.Y - i);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

			vecVtx.push_back(v);
		}
	}

	vector<UINT> vecIdx;

	for (UINT i = 0; i < m_FaceSize.Y; ++i)
	{
		for (UINT j = 0; j < m_FaceSize.X; ++j)
		{
			// 0
			// | \
			// 2--1  
			vecIdx.push_back((m_FaceSize.X + 1) * (i + 1) + (j));
			vecIdx.push_back((m_FaceSize.X + 1) * (i)+(j + 1));
			vecIdx.push_back((m_FaceSize.X + 1) * (i)+(j));

			// 1--2
			//  \ |
			//    0
			vecIdx.push_back((m_FaceSize.X + 1) * (i)+(j + 1));
			vecIdx.push_back((m_FaceSize.X + 1) * (i + 1) + (j));
			vecIdx.push_back((m_FaceSize.X + 1) * (i + 1) + (j + 1));
		}
	}


	// mesh create
	Ptr<CMesh> pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	wstring strResKey;

	// save mesh file

	bool isDefault = true;
	// 파일로 생성하는 조건()

	// 매쉬가 없는데 이름이 있음
	if (nullptr == m_pFaceMesh && !m_meshName.empty())
		isDefault = false;
	// 매쉬가 있는데 키값이 다름
	if (nullptr != m_pFaceMesh && m_pFaceMesh.Get()->GetKey() != m_meshName)
		isDefault = false;
	// 메쉬 있는데 이름값이 없으면
	if (nullptr != m_pFaceMesh && m_meshName.empty())
		isDefault = true;

	if (isDefault)
	{
		strResKey = L"LandMesh";
		strResKey += m_makeCnt++;
	}
	else
	{
		wstring meshName(m_meshName.begin(), m_meshName.end());
		strResKey = L"mesh\\landscape\\";
		strResKey += meshName;
		strResKey += L".mesh";
		pMesh->Save(strResKey);
	}
	
	// add mesh resource
	CResMgr::GetInst()->AddRes<CMesh>(strResKey, pMesh);

	// setting mesh & mtrl
	m_pFaceMesh = pMesh;
	SetMesh(pMesh);
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LandScapeMtrl"), 0);
}

void CLandScape::CreateComputeShader()
{
	// ======================
	// 높이 수정 컴퓨트 쉐이더
	// ======================
	m_pCSHeightMap = (CHeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"HeightMapShader").Get();
	if (nullptr == m_pCSHeightMap)
	{
		m_pCSHeightMap = new CHeightMapShader(32, 32, 1);
		m_pCSHeightMap->CreateComputeShader(L"shader\\heightmap.fx", "CS_HeightMap");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"HeightMapShader", m_pCSHeightMap.Get());
	}

	// =====================
	// 지형 피킹 컴퓨트 쉐이더
	// =====================
	m_pCSRaycast = (CRaycastShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"RaycastShader").Get();
	if (nullptr == m_pCSRaycast)
	{
		m_pCSRaycast = new CRaycastShader(32, 32, 1);
		m_pCSRaycast->CreateComputeShader(L"shader\\raycast.fx", "CS_Raycast");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"RaycastShader", m_pCSRaycast.Get());
	}

	// =======================
	// 가중치 수정 컴퓨트 쉐이더
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
	// 높이맵 텍스쳐
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


	// 가중치 버퍼
	m_iWeightWidth = 1024;
	m_iWeightHeight = 1024;

	m_pWeightMapBuffer = new CStructuredBuffer;
	m_pWeightMapBuffer->Create(sizeof(tWeight_4), m_iWeightWidth * m_iWeightHeight, SB_TYPE::READ_WRITE, false);

}
