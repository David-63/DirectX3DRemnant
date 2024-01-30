#include "CAnim3D.h"

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
CAnim3D::CAnim3D(bool _bEngine) : CRes(RES_TYPE::ANIMCLIP, _bEngine)
	, m_Owner(nullptr)
	, m_iFrameCount(30), m_AnimUpdateTime()
	, m_CurFrameIdx(0), m_NextFrameIdx(0), m_Ratio(0.f)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false), m_Finish(false)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnim3D::~CAnim3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;
}

int CAnim3D::finaltick()
{
	if (nullptr == m_Owner)
		return -1;

	if (m_Finish)
		return -1;

	// m_AnimUpdateTime[m_AnimClipIdx] 이 변수는 CreateAnim 단계에서 StartTime 값으로 초기화 되어야함

	m_AnimUpdateTime[m_AnimData.AnimClipIdx] += ScaleDT;
	if (m_AnimUpdateTime[m_AnimData.AnimClipIdx] >= m_AnimData.FinishTime)
	{
		// 시간은 여기서 초기화 하는게 아니라 애니메이터에서 초기화함수를 호출하는 방식으로 구현		
		m_Finish = true;
	}
	
	// 프레임 구하기 : 클립 시작시간 + 현재 시간 * 기준 프레임(30)
	double dFrameIdx = 
		(m_AnimUpdateTime[m_AnimData.AnimClipIdx] + m_AnimData.BeginTime) * (double)m_iFrameCount;
	m_CurFrameIdx = (int)(dFrameIdx);

	if (m_CurFrameIdx >= m_Owner->GetAnimClip()->at(m_AnimData.AnimClipIdx).iFrameLength - 1)
		m_NextFrameIdx = m_CurFrameIdx;
	else
		m_NextFrameIdx = m_CurFrameIdx + 1;

	m_Ratio = (float)(dFrameIdx - (double)m_NextFrameIdx);
	m_bFinalMatUpdate = false;

	int retFrameIdx = m_CurFrameIdx - (m_AnimData.BeginTime * 30);

	// 프레임이 인덱스를 넘어가지 않게 예외처리

	// 6 * 30 = 180
	int limitFrame = (m_AnimData.EndTime * 30);
	if (retFrameIdx >= limitFrame)
		retFrameIdx = limitFrame - 1;
	return retFrameIdx;
}

void CAnim3D::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();

		// Bone Data
		Ptr<CMesh> pMesh = m_Owner->MeshRender()->GetMesh();
		check_mesh(pMesh);

		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

		
		UINT iBoneCount = m_Owner->GetBoneCount();
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

void CAnim3D::ClearData()
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

void CAnim3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
}

void CAnim3D::CreateAnimation3D(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime)
{
	SetKey(_strAnimName);
	string convert(_strAnimName.begin(), _strAnimName.end());
	m_AnimName = convert;
	// 클립 정보
	m_AnimData.AnimClipIdx = _clipIdx;
	m_AnimData.BeginTime = _startTime;
	m_AnimData.EndTime = _endTime;

	// 애니메이션 정보
	m_AnimData.StartTime = 0.f;
	m_AnimData.FinishTime = m_AnimData.EndTime - m_AnimData.BeginTime;

	// 클립 수만큼 배열 늘리기
	m_AnimUpdateTime.resize(m_Owner->GetAnimClip()[m_AnimData.AnimClipIdx].size());
	m_AnimUpdateTime[m_AnimData.AnimClipIdx] = m_AnimData.StartTime; // 사실상 별 의미 없음
}

int CAnim3D::ConvertTimeToFrame(float _idxTime)
{
	return 0;
}

//void CAnim3D::CreateAnimation3D(const string& _strAnimName, int _clipIdx, int _startFrame, int _endFrame)
//{
//	// 안쓸듯?
//}

int CAnim3D::Save(const wstring& _strFilePath)
{
	//SetRelativePath(_strRelativePath);

	//wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	//FILE* pFile = nullptr;
	//errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	//assert(pFile);

	//// Mesh Key 저장	
	//// Mesh Data 저장
	//SaveResRef(m_pMesh.Get(), pFile);

	//// material 정보 저장
	//UINT iMtrlCount = (UINT)m_vecMtrl.size();
	//fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

	//UINT i = 0;
	//wstring strMtrlPath = CPathMgr::GetInst()->GetContentPath();
	//strMtrlPath += L"material\\";

	//for (; i < iMtrlCount; ++i)
	//{
	//	if (nullptr == m_vecMtrl[i])
	//		continue;

	//	// Material 인덱스, Key, Path 저장
	//	fwrite(&i, sizeof(UINT), 1, pFile);
	//	SaveResRef(m_vecMtrl[i].Get(), pFile);
	//}

	//i = -1; // 마감 값
	//fwrite(&i, sizeof(UINT), 1, pFile);

	//fclose(pFile);
	//return S_OK;
	return S_OK;
}

int CAnim3D::Load(const wstring& _strFilePath)
{
	return S_OK;
}

void CAnim3D::SaveToLevelFile(FILE* _File)
{
}

void CAnim3D::LoadFromLevelFile(FILE* _File)
{
}
