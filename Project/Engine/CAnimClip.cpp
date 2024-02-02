#include "CAnimClip.h"

#include "CTimeMgr.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
#include "CAnimation3DShader.h"
#include "CResMgr.h"
#include "CMeshRender.h"

//CAnim3D::CAnim3D()
//: m_Owner(nullptr), m_strAnimName()
//, m_iFrameCount(30), m_AnimUpdateTime()
//, m_CurFrameIdx(0), m_NextFrameIdx(0), m_Ratio(0.f)
//, m_pBoneFinalMatBuffer(nullptr)
//, m_bFinalMatUpdate(false), m_Finish(false)
//{
//	m_pBoneFinalMatBuffer = new CStructuredBuffer;
//}
CAnimClip::CAnimClip(bool _bEngine) : CRes(RES_TYPE::ANIMCLIP, _bEngine)
	, m_Owner(nullptr)
	, m_iFrameCount(30), m_AnimUpdateTime()
	, m_CurFrameIdx(0), m_NextFrameIdx(0), m_Ratio(0.f)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false), m_Finish(false)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimClip::~CAnimClip()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;
}

int CAnimClip::finaltick()
{
	// Animator가 사라졌으면 멈추기?
	if (nullptr == m_Owner)
		return -1;
	// run 상태가 아니면 멈추기
	if (!m_isRun)
		return -1;
	// 끝난 상태면 멈추기
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
	if (m_CurFrameIdx >= m_pVecClip->at(m_AnimData.AnimClipIdx).iFrameLength - 1)
		m_NextFrameIdx = m_CurFrameIdx;
	else
		m_NextFrameIdx = m_CurFrameIdx + 1;

	// GetRatio
	m_Ratio = (float)(dFrameIdx - (double)m_NextFrameIdx);
	m_bFinalMatUpdate = false;

	// ReturnFrameIdx
	int retFrameIdx = m_CurFrameIdx - (m_AnimData.BeginTime * 30);

	return retFrameIdx;
}

void CAnimClip::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();

		// Bone Data
		Ptr<CMesh> pMesh = m_Owner->MeshRender()->GetMesh();
		check_mesh(pMesh);

		pUpdateShader->SetFrameDataBuffer(m_FrameDataBuffer);
		pUpdateShader->SetOffsetMatBuffer(m_BoneOffsetBuffer);
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);
				
		UINT iBoneCount = (UINT)m_pVecBones->size();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_CurFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_NextFrameIdx);
		pUpdateShader->SetFrameRatio(m_Ratio);

		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();

		m_bFinalMatUpdate = true;
	}

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
}

void CAnimClip::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	UINT iMtrlCount = m_Owner->MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = m_Owner->MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

void CAnimClip::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetMTBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
}

void CAnimClip::NewAnimClip(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime)
{
	SetKey(_strAnimName);
	string convert(_strAnimName.begin(), _strAnimName.end());
	m_AnimName = convert;
	// 클립 정보
	m_AnimData.AnimClipIdx = _clipIdx;
	m_AnimData.BeginTime = _startTime;
	m_AnimData.EndTime = _endTime;

	// 애니메이션 정보
	m_AnimData.FinishTime = m_AnimData.EndTime - m_AnimData.BeginTime;

	// 클립 수 만큼 배열 늘리기
	m_AnimUpdateTime.resize(m_pVecClip[m_AnimData.AnimClipIdx].size());
	m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f;
}

void CAnimClip::SetAnimationBuffer(Ptr<CMesh> _inMesh)
{
	m_pVecBones = _inMesh->GetMTBones();
	m_pVecClip = _inMesh->GetMTAnimClips();
	m_BoneOffsetBuffer = _inMesh->GetBoneOffsetBuffer();
	m_FrameDataBuffer = _inMesh->GetBoneFrameDataBuffer();
}

void CAnimClip::Edit(float _begin, float _end)
{
	m_AnimData.BeginTime = _begin;
	m_AnimData.EndTime = _end;
	m_AnimData.FinishTime = m_AnimData.EndTime - m_AnimData.BeginTime;
	m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f;
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

	// mtData
	fwrite(&m_pVecBones, sizeof(const vector<tMTBone>*), 1, pFile);
	fwrite(&m_pVecClip, sizeof(const vector<tMTAnimClip>*), 1, pFile);
	// buffers
	fwrite(&m_BoneOffsetBuffer, sizeof(CStructuredBuffer*), 1, pFile);
	fwrite(&m_FrameDataBuffer, sizeof(CStructuredBuffer*), 1, pFile);
	
	// tAnim3DData	애니메이션 정보
	fwrite(&m_AnimData, sizeof(tAnim3DData), 1, pFile);
	// UpdateTime 정보
	fwrite(&m_AnimUpdateTime, sizeof(vector<float>), 1, pFile);

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

	// mtData
	fwrite(&m_pVecBones, sizeof(const vector<tMTBone>*), 1, pFile);
	fwrite(&m_pVecClip, sizeof(const vector<tMTAnimClip>*), 1, pFile);
	// buffers
	fwrite(&m_BoneOffsetBuffer, sizeof(CStructuredBuffer*), 1, pFile);
	fwrite(&m_FrameDataBuffer, sizeof(CStructuredBuffer*), 1, pFile);

	// tAnim3DData	애니메이션 정보
	fread(&m_AnimData, sizeof(tAnim3DData), 1, pFile);
	// vector<Matrix> 뼈 정보
	fread(&m_AnimUpdateTime, sizeof(vector<float>), 1, pFile);
	
	// 제어 변수
	m_isRun = false;
	m_Finish = false;
	m_bFinalMatUpdate = false;
	m_iFrameCount = 30;
	fclose(pFile);
	return S_OK;
}

void CAnimClip::SaveToLevelFile(FILE* _File)
{
}

void CAnimClip::LoadFromLevelFile(FILE* _File)
{
}
