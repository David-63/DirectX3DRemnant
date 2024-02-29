#include "pch.h"
#include "CResMgr.h"
#include "CPathMgr.h"


CResMgr::CResMgr()
	: m_Changed(false)
{
	mPrefab = new CPrefab;
}

CResMgr::~CResMgr()
{
	if (nullptr != mPrefab)
	{
		delete mPrefab;
		mPrefab = nullptr;
	}
}

void CResMgr::init()
{
	InitSound();

	CreateDefaultMesh();
	CreateDefaultGraphicsShader();
	CreateDefaultComputeShader();
	CreateDefaultMaterial();	
}

void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}


void CResMgr::tick()
{
	m_Changed = false;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
	, DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex =  FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Width, _Height, _pixelformat, _BindFlag, _Usage)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Tex2D)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

Ptr<CMeshData> CResMgr::LoadFBX(const wstring& _strPath)
{
	wstring strFileName = path(_strPath).stem();

	wstring strName = L"meshdata\\";
	strName += strFileName + L".mdat";

	Ptr<CMeshData> pMeshData = FindRes<CMeshData>(strName);

	if (nullptr != pMeshData)
		return pMeshData;

	pMeshData = CMeshData::LoadFromFBX(_strPath);
	pMeshData->SetKey(strName);
	pMeshData->SetRelativePath(strName);

	m_arrRes[(UINT)RES_TYPE::MESHDATA].insert(make_pair(strName, pMeshData.Get()));

	// meshdata 를 실제파일로 저장
	pMeshData->Save(strName);

	return pMeshData;
}

void CResMgr::DeleteRes(RES_TYPE _type, const wstring& _strKey)
{
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)_type].find(_strKey);

	assert(!(iter == m_arrRes[(UINT)_type].end()));

	m_arrRes[(UINT)_type].erase(iter);	

	m_Changed = true;
}

void CResMgr::DeleteMaterial(const wstring& _strKey)
{
	Ptr<CMaterial> pMtrl = FindRes<CMaterial>(_strKey);
	wstring strRelativePath = pMtrl->GetRelativePath();
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += strRelativePath;

	if (pMtrl.Get())
	{

		string Path = ToString(strPath);

		// 실제 파일 삭제
		if (remove(Path.c_str()) != 0)
		{
#ifdef _DEBUG
			// 비주얼 스튜디오 출력 창에 문구 출력
			std::cerr << "Failed to delete file : " << Path << std::endl;
#endif
		}

		{ 
			// RAM 메모리 내에서도 리소스 삭제
			map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)RES_TYPE::MATERIAL].begin();
			for (; iter != m_arrRes[(UINT)RES_TYPE::MATERIAL].end(); ++iter) {
				if ((*iter).first == _strKey)
				{
					Ptr<CRes> pMtrl = iter->second.Get();
					m_arrRes[(UINT)RES_TYPE::MATERIAL].erase(iter);
					break;
				}
			}
		}
	}
}

void CResMgr::SavePrefab(CGameObject* _obj, const wstring& _strRelativePath)
{
	Ptr<CPrefab> pref = new CPrefab;

	pref->RegisterProtoObject(_obj);
	pref->SetRelativePath(_strRelativePath);

	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _strRelativePath;
	pref->Save(strPath);

	m_arrRes[(UINT)RES_TYPE::PREFAB].insert(make_pair(_strRelativePath, pref.Get()));

	pref->RegisterProtoObject(nullptr);
}


void CResMgr::AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName, UINT _iSlotNum, UINT _iSemanticIdx)
{
	D3D11_INPUT_ELEMENT_DESC LayoutDesc = {};

	if (0 == _iSlotNum)
	{
		LayoutDesc.AlignedByteOffset = m_iLayoutOffset_0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.InstanceDataStepRate = 0;
	}
	else if (1 == _iSlotNum)
	{
		LayoutDesc.AlignedByteOffset = m_iLayoutOffset_1;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc.InstanceDataStepRate = 1;
	}

	LayoutDesc.Format = _eFormat;
	LayoutDesc.InputSlot = _iSlotNum;
	LayoutDesc.SemanticName = _strSemanticName;
	LayoutDesc.SemanticIndex = _iSemanticIdx;

	m_vecLayoutInfo.push_back(LayoutDesc);


	// Offset 증가
	if (0 == _iSlotNum)
		m_iLayoutOffset_0 += GetSizeofFormat(_eFormat);
	else if (1 == _iSlotNum)
		m_iLayoutOffset_1 += GetSizeofFormat(_eFormat);
}