#include "pch.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	: m_pCurrentAnim(nullptr), m_pPrevAnim(nullptr), m_bRepeat(false)

	, m_isBlend(false), m_isFinalMatUpdate(false), m_BoneFinalMatBuffer(nullptr)
	, m_blendUpdateTime(0.f), m_blendFinishTime(0.32f), m_blendRatio(0.f), m_prevFrameIdx(0)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_BoneFinalMatBuffer = new CStructuredBuffer();
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pCurrentAnim(_origin.m_pCurrentAnim)
	, m_bRepeat(_origin.m_bRepeat)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_BoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	//Safe_Del_Map(m_mapAnim);
	Safe_Del_Map(m_Events);

	if (nullptr != m_BoneFinalMatBuffer)
	{
		delete m_BoneFinalMatBuffer;
		m_BoneFinalMatBuffer = nullptr;
	}
}


void CAnimator3D::finaltick()
{
	if (nullptr == m_pCurrentAnim)
		return;
	animaTick();

	if (m_isBlend)
		blendTick();
}

void CAnimator3D::UpdateData()
{
	if (nullptr == m_pCurrentAnim)
		return;

	if (!m_isFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();

		// ���带 �ؾ��ϸ�
		if (m_isBlend)
		{
			// mesh�� �ΰ� �޾ƾ���
			Ptr<CMesh> pPrevMesh = m_pPrevAnim->GetOriginMesh();
			check_mesh(pPrevMesh);

			Ptr<CMesh> pCurMesh = m_pCurrentAnim->GetOriginMesh();
			check_mesh(pCurMesh);



			// ���̴��� ����ϴ� ���۴� �޽��κ��� ����������
			// ���������δ� �� �ִϸ����͸��� ���Ǵ� ������ ������ �ٸ��� ������
			// ������ �ִϸ��̼� Ŭ���� ����ص� ������ ������
			pUpdateShader->SetFrameDataBuffer(pPrevMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetFrameDataBuffer_next(pCurMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetOffsetMatBuffer(pCurMesh.Get()->GetBoneOffsetBuffer());
			pUpdateShader->SetOutputBuffer(m_BoneFinalMatBuffer);

			// m_Const ������ ���� ������
			UINT iBoneCount = (UINT)pCurMesh.Get()->GetMTBoneCount();
			pUpdateShader->SetBoneCount(iBoneCount);
			pUpdateShader->SetFrameIndex(m_prevFrameIdx);
			pUpdateShader->SetNextFrameIdx(m_pCurrentAnim->GetNextFrame());
			pUpdateShader->SetFrameRatio(m_blendRatio);
		}
		else
		{
			Ptr<CMesh> pCurMesh = m_pCurrentAnim->GetOriginMesh();
			check_mesh(pCurMesh);



			// ���̴��� ����ϴ� ���۴� �޽��κ��� ����������
			// ���������δ� �� �ִϸ����͸��� ���Ǵ� ������ ������ �ٸ��� ������
			// ������ �ִϸ��̼� Ŭ���� ����ص� ������ ������
			pUpdateShader->SetFrameDataBuffer(pCurMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetFrameDataBuffer_next(pCurMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetOffsetMatBuffer(pCurMesh.Get()->GetBoneOffsetBuffer());
			pUpdateShader->SetOutputBuffer(m_BoneFinalMatBuffer);

			// m_Const ������ ���� ������
			UINT iBoneCount = (UINT)pCurMesh.Get()->GetMTBoneCount();
			pUpdateShader->SetBoneCount(iBoneCount);
			pUpdateShader->SetFrameIndex(m_pCurrentAnim->GetCurFrame());
			pUpdateShader->SetNextFrameIdx(m_pCurrentAnim->GetNextFrame());
			pUpdateShader->SetFrameRatio(m_pCurrentAnim->GetFrameRatio());
		}		

		// ������Ʈ ���̴� ����
		pUpdateShader->Execute();

		m_isFinalMatUpdate = true;
	}

	// t30 �������Ϳ� ������� ������(��������) ���ε�		
	m_BoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
}

void CAnimator3D::ClearData()
{
	m_BoneFinalMatBuffer->Clear();

	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh �˸���
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::animaTick()
{
	// �ִϸ��̼ǿ� �ִ� �̺�Ʈ ��������
	Events* events = FindEvents(m_pCurrentAnim->GetKey());

	if (m_pCurrentAnim->IsFinish())
	{
		if (events)
			events->CompleteEvent();
		if (m_bRepeat)
			m_pCurrentAnim->Reset();
		else
			m_pCurrentAnim->TimeClear();
	}

	// ������ Ŭ���� Start / End Time�� 0 ~ end�� �����ϱ�
	// �� -> Frame ��ȯ ���
	UINT frameIndex = m_pCurrentAnim->finaltick();
	m_isFinalMatUpdate = false;


	if (events)
	{
		if (frameIndex >= events->ActionEvents.size())
			return;
		// Complete�� �ƴϰ�, ������ �߰��� ActionEvent�� �ִٸ� �װ��� ����
		if (frameIndex != -1 && events->ActionEvents[frameIndex].mEvent)
		{
			events->ActionEvents[frameIndex].mEvent();
		}
	}
}

void CAnimator3D::blendTick()
{
	m_blendUpdateTime += ScaleDT;
	
	if (m_blendUpdateTime >= m_blendFinishTime)
	{
		m_blendUpdateTime = 0.f;
		m_isBlend = false;

		Events* events;
		events = FindEvents(m_pCurrentAnim->GetKey());
		if (events)
			events->StartEvent();
	}

	m_blendRatio = m_blendUpdateTime / m_blendFinishTime;
}

void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetMTBoneCount();

	if (m_BoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_BoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
}


void CAnimator3D::Add(Ptr<CAnimClip> _clip)
{
	_clip->m_Owner = this;
	m_mapAnim.insert(make_pair(_clip->GetKey(), _clip.Get()));
	m_pCurrentAnim = _clip.Get();
	m_pCurrentAnim->Stop();

	Events* events = FindEvents(_clip->GetKey());
	if (events)
		return;

	events = new Events();
	// �ִ� �����Ӽ���ŭ
	int maxFrame = (m_pCurrentAnim->GetEndTime() - m_pCurrentAnim->GetBeginTime()) * 30 + 1;
	events->ActionEvents.resize(maxFrame);
	m_Events.insert(std::make_pair(_clip->GetKey(), events));
}

void CAnimator3D::Remove(const wstring& _key)
{
	m_mapAnim.erase(_key);
	m_pCurrentAnim = nullptr;
}

void CAnimator3D::CreateAnimClip(wstring _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _inMesh)
{	
	Ptr<CAnimClip> pAnim = CResMgr::GetInst()->FindRes<CAnimClip>(_strAnimName);
	if (nullptr != pAnim)
		return;
	pAnim = new CAnimClip(true);
	pAnim->NewAnimClip(_strAnimName, _clipIdx, _startTime, _endTime, _inMesh);
	
	pAnim->m_Owner = this;
	m_mapAnim.insert(make_pair(pAnim->GetKey(), pAnim.Get()));
	m_pCurrentAnim = pAnim.Get();
	m_pCurrentAnim->Stop();

	Events* events = FindEvents(_strAnimName);
	if (events)
		return;
	
	events = new Events();
	// �ִ� �����Ӽ���ŭ
	int maxFrame = (_endTime - _startTime) * 30 + 1;
	events->ActionEvents.resize(maxFrame);
	m_Events.insert(std::make_pair(_strAnimName, events));
}

void CAnimator3D::NewAnimClip(string _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _inMesh)
{
	wstring strPath = wstring(_strAnimName.begin(), _strAnimName.end());


	Ptr<CAnimClip> pAnim = CResMgr::GetInst()->FindRes<CAnimClip>(strPath);
	if (nullptr != pAnim)
		return;
	pAnim = new CAnimClip(false);
	pAnim->NewAnimClip(strPath, _clipIdx, _startTime, _endTime, _inMesh);

	pAnim->SetKey(strPath);
	pAnim->SetRelativePath(strPath);
	CResMgr::GetInst()->AddRes<CAnimClip>(pAnim->GetKey(), pAnim.Get());

	pAnim->SetAnimName(_strAnimName);
	pAnim->Save(strPath);


	pAnim->m_Owner = this;
	m_mapAnim.insert(make_pair(pAnim->GetKey(), pAnim.Get()));
	m_pCurrentAnim = pAnim.Get();
	m_pCurrentAnim->Stop();

	Events* events = FindEvents(strPath);
	if (events)
		return;

	events = new Events();
	// �ִ� �����Ӽ���ŭ
	int maxFrame = (_endTime - _startTime) * 30 + 1;
	events->ActionEvents.resize(maxFrame);
	m_Events.insert(std::make_pair(strPath, events));
}

void CAnimator3D::Play(const wstring& _strName, bool _bRepeat)
{
	// ���� �ִ԰� ���Ŀ� ���� �ִ��� ��
	m_pPrevAnim = m_pCurrentAnim;
	m_pCurrentAnim = FindAnim(_strName).Get();
	Events* events;

	// Ű���� ��
	if (m_pPrevAnim->GetKey() != m_pCurrentAnim->GetKey())
	{
		//Blend ����
		m_prevFrameIdx = m_pPrevAnim->GetCurFrame();
		m_isBlend = true;

		// ���� �ִϸ��̼��� ���� �̺�Ʈ ȣ�� �� �ʱ�ȭ
		events = FindEvents(m_pPrevAnim->GetKey());
		if (events)
			events->EndEvent();
		m_pPrevAnim->Reset();

		// ���� �ִϸ��̼��� ���� �ִϸ��̼����� ��ü
		m_pCurrentAnim->Play();

		// �ִϸ��̼� ����... ������ ���߿� �ϴ°ŷ�
		events = FindEvents(m_pCurrentAnim->GetKey());
		if (events)
			events->StartEvent();
	}
	else
	{
		// �ٲ�� ����
		if (m_pCurrentAnim->IsFinish())
		{
			m_pCurrentAnim->Reset();
			m_pCurrentAnim->Play();
		}
		else
			m_pCurrentAnim->Play();
	}

	m_bRepeat = _bRepeat;
}

Ptr<CAnimClip> CAnimator3D::FindAnim(const wstring& _strName)
{
	map<wstring, Ptr<CAnimClip>>::iterator iter = m_mapAnim.find(_strName);
	//animclip//AnimTest
	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

Events* CAnimator3D::FindEvents(const std::wstring& name)
{
	std::map<std::wstring, Events*>::iterator iter
		= m_Events.find(name);

	if (iter == m_Events.end())
		return nullptr;

	return iter->second;
}

std::function<void()>& CAnimator3D::StartEvent(const std::wstring& name)
{
	Events* events = FindEvents(name);
	return events->StartEvent.mEvent;
}

std::function<void()>& CAnimator3D::CompleteEvent(const std::wstring& name)
{
	Events* events = FindEvents(name);
	return events->CompleteEvent.mEvent;
}

std::function<void()>& CAnimator3D::EndEvent(const std::wstring& name)
{
	Events* events = FindEvents(name);
	return events->EndEvent.mEvent;
}

std::function<void()>& CAnimator3D::ActionEvent(const std::wstring& name, UINT index)
{
	Events* events = FindEvents(name);
	return events->ActionEvents[index].mEvent;
}



void CAnimator3D::SaveToLevelFile(FILE* _pFile)
{
	
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
}