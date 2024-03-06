#include "pch.h"
#include "CLandScape.h"

#include "CResMgr.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"



void CLandScape::init()
{
	CreateMesh();
	CreateComputeShader();
	CreateTexture();

	// 가중치 버퍼
	if (!m_pWeightMapBuffer)
	{
		m_iWeightWidth = 1024;
		m_iWeightHeight = 1024;

		m_pWeightMapBuffer = new CStructuredBuffer;
		m_pWeightMapBuffer->Create(sizeof(tWeight_4), m_iWeightWidth * m_iWeightHeight, SB_TYPE::READ_WRITE, false);
	}

	// 레이캐스팅 결과 받는 버퍼
	if (!m_pCrossBuffer)
	{
		m_pCrossBuffer = new CStructuredBuffer;
		m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true);
	}	
}

void CLandScape::CreateMesh()
{
	// 이미 기본 메쉬가 있다면 생성 안함
	wstring strResKey = L"LandMesh";
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(strResKey);
	if (nullptr != pMesh)
	{
		SetFaceMesh(pMesh);
		return;
	}

	Vtx v;
	vector<Vtx> vecVtx;

	// 변수의 Y를 Z축으로 사용
	for (UINT i = 0; i < DEFAULT_FACE + 1; ++i)
	{
		for (UINT j = 0; j < DEFAULT_FACE + 1; ++j)
		{
			v.vPos = Vec3((float)j, 0.f, (float)i);
			v.vUV = Vec2((float)j, (float)DEFAULT_FACE - i);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

			vecVtx.push_back(v);
		}
	}

	vector<UINT> vecIdx;

	for (UINT i = 0; i < DEFAULT_FACE; ++i)
	{
		for (UINT j = 0; j < DEFAULT_FACE; ++j)
		{
			// 0
			// | \
			// 2--1  
			vecIdx.push_back((DEFAULT_FACE + 1) * (i + 1) + (j));
			vecIdx.push_back((DEFAULT_FACE + 1) * (i)+(j + 1));
			vecIdx.push_back((DEFAULT_FACE + 1) * (i)+(j));

			// 1--2
			//  \ |
			//    0
			vecIdx.push_back((DEFAULT_FACE + 1) * (i)+(j + 1));
			vecIdx.push_back((DEFAULT_FACE + 1) * (i + 1) + (j));
			vecIdx.push_back((DEFAULT_FACE + 1) * (i + 1) + (j + 1));
		}
	}

	// create new mesh
	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());

	// add mesh resource
	CResMgr::GetInst()->AddRes<CMesh>(strResKey, pMesh);

	// setting mesh & mtrl
	SetFaceMesh(pMesh);
}

void CLandScape::MakeFaceMesh(string _strAnimName, UINT _iFaceX, UINT _iFaceZ)
{
	string meshName = _strAnimName;
	string strResKey;
	strResKey = "mesh\\landscape\\";
	strResKey += meshName + ".mesh";
	

	// 이름 중복 검사
	wstring strPath = wstring(strResKey.begin(), strResKey.end());
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(strPath);
	if (nullptr != pMesh)
		return;

	m_FaceSize.X = _iFaceX;
	m_FaceSize.Y = _iFaceZ;


	// 메쉬 만들기
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
	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	
	// 메쉬 등록 및 파일로 저장
	pMesh->SetKey(strPath);
	pMesh->SetRelativePath(strPath);	
	pMesh->Save(strPath);

	CResMgr::GetInst()->AddRes<CMesh>(pMesh->GetKey(), pMesh.Get());

	SetFaceMesh(pMesh);
}


void CLandScape::CreateHeightMap()
{
	wstring strResKey = L"texture\\landscape\\";

	wstring meshName(m_heightMapName.begin(), m_heightMapName.end());
	strResKey += meshName;
	strResKey += L".tex";
	m_HeightMap->Save(strResKey);
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
	// 높이맵
	m_HeightMap = CResMgr::GetInst()->FindRes<CTexture>(L"HeightMap");
	if (nullptr == m_HeightMap)
	{
		m_HeightMap = CResMgr::GetInst()->CreateTexture(L"HeightMap"
			, 2048, 2048
			, DXGI_FORMAT_R32_FLOAT
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
			, D3D11_USAGE_DEFAULT);
	}
	// 브러쉬
	m_pBrushTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\brush\\Brush_01.png");

	// 가중치 타일 배열*
	if (nullptr == m_pTileArrTex)
	{
		m_pTileArrTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_ARRR.dds");
		m_pTileArrTex->GenerateMip(8);
	}
}
