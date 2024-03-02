#include "CAnimClip.h"

#include "CTimeMgr.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
#include "CAnimation3DShader.h"
#include "CResMgr.h"
#include "CMeshRender.h"

CAnimClip::CAnimClip(bool _bEngine) : CRes(RES_TYPE::ANIMCLIP, _bEngine), m_iFrameCount(30) { }
CAnimClip::~CAnimClip() { }

void CAnimClip::MakeAnimClip(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _pMesh)
{
	SetKey(_strAnimName);
	m_originMesh = _pMesh;
	
	string convert(_strAnimName.begin(), _strAnimName.end());
	m_AnimName = convert;
	// 클립 정보
	m_AnimData.AnimClipIdx = _clipIdx;
	m_AnimData.BeginTime = _startTime;
	m_AnimData.EndTime = _endTime;

	// 애니메이션 정보
	m_AnimData.FinishTime = m_AnimData.EndTime - m_AnimData.BeginTime;
}

int CAnimClip::Save(const wstring& _strRelativePath)
{
	if (IsEngineRes())
		return E_FAIL;


	// 상대경로 저장
	SetRelativePath(_strRelativePath);

	// 파일 경로 만들기
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	// 파일 쓰기모드로 열기
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);


	// 키값, 상대 경로
	const wstring& name = GetName();
	SaveWString(name, pFile);
	const wstring& key = GetKey();
	SaveWString(key, pFile);
	const wstring& path = GetRelativePath();
	SaveWString(path, pFile);

	// tAnim3DData	애니메이션 정보
	fwrite(&m_AnimData, sizeof(tAnim3DData), 1, pFile);
	
	// mtData
	SaveResRef(m_originMesh.Get(), pFile);


	fclose(pFile);

	return S_OK;
}

int CAnimClip::Load(const wstring& _strFilePath)
{
	// 읽기모드로 파일열기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	// 키값, 상대경로
	wstring strName, strKey, strRelativePath;
	LoadWString(strName, pFile);
	LoadWString(strKey, pFile);
	LoadWString(strRelativePath, pFile);

	SetName(strName);
	SetKey(strKey);
	SetRelativePath(strRelativePath);


	string convert(strKey.begin(), strKey.end());
	m_AnimName = convert;

	// tAnim3DData	애니메이션 정보
	fread(&m_AnimData, sizeof(tAnim3DData), 1, pFile);

	// mtData
	LoadResRef(m_originMesh, pFile);


	m_iFrameCount = 30;
	fclose(pFile);
	return S_OK;
}
