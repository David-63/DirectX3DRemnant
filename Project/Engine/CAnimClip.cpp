#include "CAnimClip.h"

#include "CTimeMgr.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
#include "CAnimation3DShader.h"
#include "CResMgr.h"
#include "CMeshRender.h"

CAnimClip::CAnimClip(bool _bEngine) : CRes(RES_TYPE::ANIMCLIP, _bEngine)
	, m_Owner(nullptr)
	, m_iFrameCount(30), m_AnimUpdateTime()
	, m_CurFrameIdx(0), m_NextFrameIdx(0), m_Ratio(0.f)
	, m_isRun(false), m_Finish(false)
{ }

CAnimClip::~CAnimClip() { }

int CAnimClip::finaltick()
{
	// Animator�� ��������� ���߱�?
	if (nullptr == m_Owner)
		return -1;
	// run ���°� �ƴϸ� ���߱�
	if (!m_isRun)
		return -1;
	// ���� ���¸� ���߱�
	if (m_Finish)
		return -1;


	// Is Finish
	m_AnimUpdateTime[m_AnimData.AnimClipIdx] += ScaleDT;
	if (m_AnimUpdateTime[m_AnimData.AnimClipIdx] >= m_AnimData.FinishTime)
		m_Finish = true;

	// CurFrame : BeginTime + UpdateTime * defaultFrameRate(30)
	double dFrameIdx = 
		(m_AnimUpdateTime[m_AnimData.AnimClipIdx] + m_AnimData.BeginTime) * (double)m_iFrameCount;
	m_CurFrameIdx = (int)(dFrameIdx);
	
	// GetNextFrame
	
	if (m_CurFrameIdx >= m_originMesh.Get()->GetMTAnimClips().at(m_AnimData.AnimClipIdx).iFrameLength - 1)
		m_NextFrameIdx = m_CurFrameIdx;
	else
		m_NextFrameIdx = m_CurFrameIdx + 1;

	// GetRatio
	m_Ratio = (float)(dFrameIdx - (double)m_NextFrameIdx);

	// ReturnFrameIdx
	int retFrameIdx = m_CurFrameIdx - (m_AnimData.BeginTime * 30);

	return retFrameIdx;
}

void CAnimClip::NewAnimClip(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _pMesh)
{
	SetKey(_strAnimName);
	m_originMesh = _pMesh;
	
	string convert(_strAnimName.begin(), _strAnimName.end());
	m_AnimName = convert;
	// Ŭ�� ����
	m_AnimData.AnimClipIdx = _clipIdx;
	m_AnimData.BeginTime = _startTime;
	m_AnimData.EndTime = _endTime;

	// �ִϸ��̼� ����
	m_AnimData.FinishTime = m_AnimData.EndTime - m_AnimData.BeginTime;

	// Ŭ�� �� ��ŭ �迭 �ø���
	m_AnimUpdateTime.resize(m_originMesh.Get()->GetMTAnimClips().size());
	m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f;
}

void CAnimClip::Edit(float _begin, float _end)
{
	m_AnimData.BeginTime = _begin;
	m_AnimData.EndTime = _end;
	m_AnimData.FinishTime = m_AnimData.EndTime - m_AnimData.BeginTime;
	m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f;
}

int CAnimClip::ConvertTimeToFrame(float _idxTime)
{
	double dFrameIdx = (_idxTime + m_AnimData.BeginTime) * (double)m_iFrameCount;
	int calcFrameidx = (int)(dFrameIdx);
	// ���ڰ� Ŭ�� �ִ�ġ�� �ѱ��
	if (_idxTime >= m_AnimData.FinishTime)
	{
		return calcFrameidx = m_AnimData.EndTime * (double)m_iFrameCount;
	}
	return calcFrameidx -= (m_AnimData.BeginTime * 30);
}

const int& CAnimClip::GetStartFrame()
{
	double dFrameIdx = (m_AnimData.BeginTime) * (double)m_iFrameCount;
	return (int)(dFrameIdx);
}


int CAnimClip::Save(const wstring& _strRelativePath)
{
	if (IsEngineRes())
		return E_FAIL;


	// ����� ����
	SetRelativePath(_strRelativePath);

	// ���� ��� �����
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	// ���� ������� ����
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);


	// Ű��, ��� ���
	const wstring& name = GetName();
	SaveWString(name, pFile);
	const wstring& key = GetKey();
	SaveWString(key, pFile);
	const wstring& path = GetRelativePath();
	SaveWString(path, pFile);

	// tAnim3DData	�ִϸ��̼� ����
	fwrite(&m_AnimData, sizeof(tAnim3DData), 1, pFile);
	// UpdateTime ����
	UINT iTimeCount = (UINT)m_AnimUpdateTime.size();
	fwrite(&iTimeCount, sizeof(int), 1, pFile);
		
	// mtData
	SaveResRef(m_originMesh.Get(), pFile);


	fclose(pFile);

	return S_OK;
}

int CAnimClip::Load(const wstring& _strFilePath)
{
	// �б���� ���Ͽ���
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	// Ű��, �����
	wstring strName, strKey, strRelativePath;
	LoadWString(strName, pFile);
	LoadWString(strKey, pFile);
	LoadWString(strRelativePath, pFile);

	SetName(strName);
	SetKey(strKey);
	SetRelativePath(strRelativePath);


	string convert(strKey.begin(), strKey.end());
	m_AnimName = convert;

	// tAnim3DData	�ִϸ��̼� ����
	fread(&m_AnimData, sizeof(tAnim3DData), 1, pFile);
	// UpdateTime ����
	int iTimeCount = 0;
	fread(&iTimeCount, sizeof(int), 1, pFile);
	m_AnimUpdateTime.resize(iTimeCount);
	for (int i = 0; i < iTimeCount; ++i)
	{
		m_AnimUpdateTime[i] = 0.f;
	}

	// mtData
	LoadResRef(m_originMesh, pFile);


	// ���� ����
	m_isRun = false;
	m_Finish = false;
	m_iFrameCount = 30;
	fclose(pFile);
	return S_OK;
}
