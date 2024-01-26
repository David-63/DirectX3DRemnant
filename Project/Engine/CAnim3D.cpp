#include "CAnim3D.h"

#include "CTimeMgr.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
#include "CAnimation3DShader.h"
#include "CResMgr.h"
#include "CMeshRender.h"

CAnim3D::CAnim3D()
: m_Owner(nullptr), m_strAnimName()
, m_iFrameCount(30), m_AnimUpdateTime()
, m_CurFrameIdx(0), m_NextFrameIdx(0), m_Ratio(0.f)
, m_pBoneFinalMatBuffer(nullptr)
, m_bFinalMatUpdate(false), m_Finish(false)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}
//CAnim3D::CAnim3D(bool _bEngine)
//: CRes(RES_TYPE::ANIMCLIP, _bEngine)
//	, m_Owner(nullptr), m_strAnimName()
//, m_iFrameCount(30), m_AnimUpdateTime()
//, m_CurFrameIdx(0), m_NextFrameIdx(0), m_Ratio(0.f)
//, m_pBoneFinalMatBuffer(nullptr)
//, m_bFinalMatUpdate(false), m_Finish(false)
//{
//	m_pBoneFinalMatBuffer = new CStructuredBuffer;
//}

CAnim3D::~CAnim3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;
}

void CAnim3D::finaltick()
{
	if (nullptr == m_Owner)
		return;

	if (m_Finish)
		return;

	// m_AnimUpdateTime[m_AnimClipIdx] �� ������ CreateAnim �ܰ迡�� StartTime ������ �ʱ�ȭ �Ǿ����

	m_AnimUpdateTime[m_AnimData.AnimClipIdx] += ScaleDT;
	if (m_AnimUpdateTime[m_AnimData.AnimClipIdx] >= m_AnimData.EndTime)
	{
		// �ð��� ���⼭ �ʱ�ȭ �ϴ°� �ƴ϶� �ִϸ����Ϳ��� �ʱ�ȭ�Լ��� ȣ���ϴ� ������� ����		
		m_Finish = true;
	}
	
	double dFrameIdx = m_AnimUpdateTime[m_AnimData.AnimClipIdx] * (double)m_iFrameCount;
	m_CurFrameIdx = (int)(dFrameIdx);

	if (m_CurFrameIdx >= m_Owner->GetAnimClip()->at(m_AnimData.AnimClipIdx).iFrameLength - 1)
		m_NextFrameIdx = m_CurFrameIdx;
	else
		m_NextFrameIdx = m_CurFrameIdx + 1;

	m_Ratio = (float)(dFrameIdx - (double)m_NextFrameIdx);

	m_bFinalMatUpdate = false;
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

		// ������Ʈ ���̴� ����
		pUpdateShader->Execute();

		m_bFinalMatUpdate = true;
	}

	// t30 �������Ϳ� ������� ������(��������) ���ε�		
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

		pMtrl->SetAnim3D(false); // Animation Mesh �˸���
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

void CAnim3D::CreateAnimation3D(const string& _strAnimName, int _clipIdx, float _startTime, float _endTime)
{
	m_strAnimName = _strAnimName;
	m_AnimData.AnimClipIdx = _clipIdx;
	m_AnimData.StartTime = _startTime;
	m_AnimData.EndTime = _endTime;
	m_AnimUpdateTime.resize(m_Owner->GetAnimClip()[m_AnimData.AnimClipIdx].size());
	m_AnimUpdateTime[m_AnimData.AnimClipIdx] = m_AnimData.StartTime;
}

//void CAnim3D::CreateAnimation3D(const string& _strAnimName, int _clipIdx, int _startFrame, int _endFrame)
//{
//	// �Ⱦ���?
//}

int CAnim3D::Save(const wstring& _strFilePath)
{
	//SetRelativePath(_strRelativePath);

	//wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	//FILE* pFile = nullptr;
	//errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	//assert(pFile);

	//// Mesh Key ����	
	//// Mesh Data ����
	//SaveResRef(m_pMesh.Get(), pFile);

	//// material ���� ����
	//UINT iMtrlCount = (UINT)m_vecMtrl.size();
	//fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

	//UINT i = 0;
	//wstring strMtrlPath = CPathMgr::GetInst()->GetContentPath();
	//strMtrlPath += L"material\\";

	//for (; i < iMtrlCount; ++i)
	//{
	//	if (nullptr == m_vecMtrl[i])
	//		continue;

	//	// Material �ε���, Key, Path ����
	//	fwrite(&i, sizeof(UINT), 1, pFile);
	//	SaveResRef(m_vecMtrl[i].Get(), pFile);
	//}

	//i = -1; // ���� ��
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
