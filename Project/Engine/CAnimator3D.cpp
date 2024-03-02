#include "pch.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"
#include "Physics.h"

CAnimator3D::CAnimator3D()
	: CComponent(COMPONENT_TYPE::ANIMATOR3D)
	, m_pCurrentAnim(nullptr), m_pPrevAnim(nullptr), m_bRepeat(false), m_isRun(false)
	, m_isFinalMatUpdate(false), m_BoneFinalMatBuffer(nullptr)
	, m_CurFrameIdx(0), m_NextFrameIdx(0), m_AnimRatio(0.f)
	, m_blendTime(BlendEndTime), m_blendRatio(0.f), m_prevFrameIdx(0)
	, m_isModifyUse(false), m_modifyRotScalar(180.f)
{
	m_BoneFinalMatBuffer = new CStructuredBuffer();
	m_modifyIndicesBuffer = new CStructuredBuffer();
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: CComponent(_origin)
	, m_pCurrentAnim(nullptr), m_pPrevAnim(nullptr), m_bRepeat(_origin.m_bRepeat), m_isRun(false)
	, m_isFinalMatUpdate(false), m_BoneFinalMatBuffer(nullptr)
	, m_CurFrameIdx(0), m_NextFrameIdx(0), m_AnimRatio(0.f)
	, m_blendTime(_origin.m_blendTime), m_blendRatio(0.f), m_prevFrameIdx(0)
	, m_isModifyUse(_origin.m_isModifyUse), m_modifyRotScalar(180.f)
{
	m_BoneFinalMatBuffer = new CStructuredBuffer();
	m_modifyIndicesBuffer = new CStructuredBuffer();


	if (_origin.m_Events.empty())
	{
		m_Events = _origin.m_Events;
	}

	if (_origin.m_mapAnim.empty())
	{
		m_mapAnim.insert(_origin.m_mapAnim.begin(), _origin.m_mapAnim.end());
	}

	if (_origin.m_pCurrentAnim)
	{
		changeAnimClip(_origin.m_pCurrentAnim->GetKey());
	}

	// ���� �������ֱ�?
	m_modifyIndices = _origin.m_modifyIndices;
	if (m_modifyIndicesBuffer->GetElementCount() != m_modifyIndices.size())
	{
		m_modifyIndicesBuffer->Create(sizeof(UINT), m_modifyIndices.size(), SB_TYPE::READ_ONLY, false, m_modifyIndices.data());
	}
}

CAnimator3D::~CAnimator3D()
{
	Safe_Del_Map(m_Events);
	if (nullptr != m_BoneFinalMatBuffer)
	{
		delete m_BoneFinalMatBuffer;
		m_BoneFinalMatBuffer = nullptr;
	}
	if (nullptr != m_modifyIndicesBuffer)
	{
		delete m_modifyIndicesBuffer;
		m_modifyIndicesBuffer = nullptr;
	}
}


void CAnimator3D::finaltick()
{
	if (nullptr == m_pCurrentAnim)
		return;

	if (m_isRun)
	{
		animaTick();
		if (m_blendTime.IsActivate())
			blendTick();
	}
	
}

void CAnimator3D::animaTick()
{
	Events* events = FindEvents(m_pCurrentAnim->GetKey());
	
	// �ִϸ��̼� ����
	if (m_CurAnimData.IsFinish)
	{
		if (events)
			events->CompleteEvent();
		if (m_bRepeat)
			Reset();
		else
		{
			TimeClear();
			m_isRun = false;
		}
	}

	// �ִϸ��̼� ����
	m_AnimUpdateTime[m_CurAnimData.AnimClipIdx] += ScaleDT;
	if (m_AnimUpdateTime[m_CurAnimData.AnimClipIdx] >= m_CurAnimData.FinishTime)
		m_CurAnimData.IsFinish = true;

	double dFrameIdx =
		(m_AnimUpdateTime[m_CurAnimData.AnimClipIdx] + m_CurAnimData.BeginTime) * (double)30;
	m_CurFrameIdx = (int)(dFrameIdx);

	if (m_CurFrameIdx >= m_pCurrentAnim->GetMTAnimClips().at(m_CurAnimData.AnimClipIdx).iFrameLength - 1)
		m_NextFrameIdx = m_CurFrameIdx;
	else
		m_NextFrameIdx = m_CurFrameIdx + 1;

	m_AnimRatio = (float)(dFrameIdx - (double)m_NextFrameIdx);

	UINT frameIndex = m_CurFrameIdx - (m_CurAnimData.BeginTime * 30);
	m_isFinalMatUpdate = false;

	// �̺�Ʈ ����
	if (events)
	{
		if (frameIndex >= events->ActionEvents.size())
			return;		
		if (frameIndex != -1 && events->ActionEvents[frameIndex].mEvent)
		{
			events->ActionEvents[frameIndex].mEvent();
		}
	}
}

void CAnimator3D::blendTick()
{
	m_blendTime.curTime += ScaleDT;

	if (m_blendTime.IsFinish())
	{
		m_blendTime.ResetTime();
		
		Events* events;
		events = FindEvents(m_pCurrentAnim->GetKey());
		if (events)
			events->StartEvent();
	}

	m_blendRatio = m_blendTime.curTime / m_blendTime.maxTime;
}

void CAnimator3D::UpdateData()
{
	if (nullptr == m_pCurrentAnim)
		return;

	if (!m_isFinalMatUpdate)
	{		
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();

		Ptr<CMesh> pCurMesh = m_pCurrentAnim->GetOriginMesh();
		check_mesh(pCurMesh);
		UINT iBoneCount = (UINT)pCurMesh.Get()->GetMTBoneCount();
		
		if (m_blendTime.IsActivate())
		{
			Ptr<CMesh> pPrevMesh = m_pPrevAnim->GetOriginMesh();
			check_mesh(pPrevMesh);

			pUpdateShader->SetFrameDataBuffer(pPrevMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetFrameDataBuffer_next(pCurMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetOffsetMatBuffer(pCurMesh.Get()->GetBoneOffsetBuffer());
			pUpdateShader->SetOutputBuffer(m_BoneFinalMatBuffer);

			pUpdateShader->SetBoneCount(iBoneCount);
			pUpdateShader->SetFrameIndex(m_prevFrameIdx);
			pUpdateShader->SetNextFrameIdx(m_NextFrameIdx);
			pUpdateShader->SetFrameRatio(m_blendRatio);
		}
		else
		{
			pUpdateShader->SetFrameDataBuffer(pCurMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetFrameDataBuffer_next(pCurMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetOffsetMatBuffer(pCurMesh.Get()->GetBoneOffsetBuffer());
			pUpdateShader->SetOutputBuffer(m_BoneFinalMatBuffer);

			pUpdateShader->SetBoneCount(iBoneCount);
			pUpdateShader->SetFrameIndex(m_CurFrameIdx);
			pUpdateShader->SetNextFrameIdx(m_NextFrameIdx);
			pUpdateShader->SetFrameRatio(m_AnimRatio);
		}		
		
		if (m_isModifyUse)
		{
			pUpdateShader->SetModifyUse(m_isModifyUse);
			pUpdateShader->SetModifyCount(m_modifyIndices.size());
			pUpdateShader->SetModifyRotScalar(m_modifyRotScalar);
			pUpdateShader->SetModifyIdxBuffer(m_modifyIndicesBuffer);
		}
		else
			pUpdateShader->SetModifyUse(m_isModifyUse);

		// ������Ʈ ���̴� ����
		pUpdateShader->Execute();

		m_isFinalMatUpdate = true;
	}

	// t30 �������Ϳ� ������� ������(��������) ���ε�		
	m_BoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);

	m_BoneFinalMatBuffer->GetData(m_vecFinalBoneMat);
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



void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetMTBoneCount();

	if (m_BoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_BoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, true, nullptr);
	}
}



Ptr<CAnimClip> CAnimator3D::findAnim(const wstring& _strName)
{
	map<wstring, Ptr<CAnimClip>>::iterator iter = m_mapAnim.find(_strName);
	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

// Play�� �����ų� Add�� ���� ���
void CAnimator3D::changeAnimClip(wstring _strAnimName)
{
	// cur Anim ����
	m_pCurrentAnim = findAnim(_strAnimName).Get();
	if (m_pPrevAnim)
	{
		if (m_pPrevAnim->GetKey() == m_pCurrentAnim->GetKey())
		{
			Events* events;

			m_prevFrameIdx = m_CurFrameIdx;
			m_blendTime.Activate();
			events = FindEvents(m_pPrevAnim->GetKey());
			if (events)
				events->EndEvent();
		}
	}	

	// curAnim�� �������ְ�, ���� �����ϱ�
	m_CurAnimData = m_pCurrentAnim->GetAnimData();
	m_AnimUpdateTime.clear();
	m_AnimUpdateTime.resize(m_pCurrentAnim->GetOriginMesh().Get()->GetMTAnimClips().size());
	m_AnimUpdateTime[m_CurAnimData.AnimClipIdx] = 0.f;
	m_CurFrameIdx = 0;
	m_NextFrameIdx = 0;
	m_AnimRatio = 0.f;
	if (m_CurAnimData.IsFinish)
		Reset();
}

void CAnimator3D::Add(Ptr<CAnimClip> _clip)
{
	// map�� �̺�Ʈ�� �ִ��� Ȯ��
	Events* events = FindEvents(_clip->GetKey());
	// ������ clip�� ���� �� �̺�Ʈ ���� �� map�� �߰��ϱ�
	if (!events)
	{
		events = new Events();
		int maxFrame = (m_CurAnimData.EndTime - m_CurAnimData.BeginTime) * 30 + 1;
		events->ActionEvents.resize(maxFrame);
		m_Events.insert(std::make_pair(_clip->GetKey(), events));
	}

	// Ŭ���� map�� �߰��ϱ�	
	m_mapAnim.insert(make_pair(_clip->GetKey(), _clip.Get()));

	// �ִϸ��̼� �������ֱ�
	changeAnimClip(_clip->GetKey());

	// ���� ���� �����ϱ�
	UINT iBoneCount = m_pCurrentAnim->GetOriginMesh().Get()->GetMTBoneCount();
	m_vecFinalBoneMat.resize(iBoneCount);

	// boneSocket �ɼ�
	{
		m_curBones = m_pCurrentAnim->GetOriginMesh()->GetMTBones();
		//m_modifyIndices.push_back(33);	// �㸮
		//m_modifyIndices.push_back(34);	// ����
		//m_modifyIndices.push_back(35);	// ��ġ
		m_modifyIndices.push_back(36);	// ����
		//m_modifyIndices.push_back(37);	// ��
		//m_modifyIndices.push_back(38);	// �Ӹ�
		//m_modifyIndices.push_back(39);	// ��
		//m_modifyIndices.push_back(40);	// ��
		//m_modifyIndices.push_back(136);	// ���
		//m_modifyIndices.push_back(168);	// �ո�

		//CollectChildrenIndices(33);
		//CollectChildrenIndices(34);
		//CollectChildrenIndices(35);
		CollectChildrenIndices(36);
		//CollectChildrenIndices(37);
		//CollectChildrenIndices(38);
		//CollectChildrenIndices(39);
		//CollectChildrenIndices(136);
		//CollectChildrenIndices(168);

		std::sort(m_modifyIndices.begin(), m_modifyIndices.end());
		int bufferPadding = m_modifyIndices.size() % 16;
		if (0 != bufferPadding)
		{
			for (int i = 0; i < 16 - bufferPadding; ++i)
			{
				m_modifyIndices.push_back(999 + i);
			}
		}

		if (m_modifyIndicesBuffer->GetElementCount() != m_modifyIndices.size())
		{
			m_modifyIndicesBuffer->Create(sizeof(UINT), m_modifyIndices.size(), SB_TYPE::READ_ONLY, false, m_modifyIndices.data());
		}
	}	
}

void CAnimator3D::CollectChildrenIndices(int current_index)
{
	// �� ������ŭ ��ȸ (260��)
	for (int i = 0; i < m_curBones.size(); ++i)
	{
		if (current_index == m_curBones[i].iParentIdx)
		{
			auto iter = find(m_modifyIndices.begin(), m_modifyIndices.end(), i);
			if (iter == m_modifyIndices.end())			
				m_modifyIndices.push_back(i); // �ڽ� ���� �ε��� �߰�
			CollectChildrenIndices(i); // ��� ȣ��
		}
	}
}

void CAnimator3D::Remove(const wstring& _key)
{
	auto item = m_Events.find(_key);
	if (item != m_Events.end())
	{
		delete item->second;
		m_Events.erase(item);
	}
	m_mapAnim.erase(_key);
	m_pCurrentAnim = nullptr;
}

void CAnimator3D::MakeAnimClip(string _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _inMesh)
{
	// �̸� �ߺ� �˻�
	wstring strPath = wstring(_strAnimName.begin(), _strAnimName.end());
	Ptr<CAnimClip> pAnim = CResMgr::GetInst()->FindRes<CAnimClip>(strPath);
	if (nullptr != pAnim)
		return;
	// ���� �� ���ҽ� ���
	pAnim = new CAnimClip(false);
	pAnim->MakeAnimClip(strPath, _clipIdx, _startTime, _endTime, _inMesh);
	pAnim->SetKey(strPath);
	pAnim->SetRelativePath(strPath);
	pAnim->SetAnimName(_strAnimName);
	pAnim->Save(strPath);
	CResMgr::GetInst()->AddRes<CAnimClip>(pAnim->GetKey(), pAnim.Get());

	// map �� Ŭ�� ���
	m_mapAnim.insert(make_pair(pAnim->GetKey(), pAnim.Get()));

	// �̺�Ʈ üũ
	Events* events = FindEvents(strPath);
	// ������ �����ؼ� map�� ���
	if (!events)
	{
		events = new Events();
		int maxFrame = (_endTime - _startTime) * 30 + 1;
		events->ActionEvents.resize(maxFrame);
		m_Events.insert(std::make_pair(strPath, events));
	}
	// �ִϸ��̼� ��������
	changeAnimClip(pAnim->GetKey());
}

void CAnimator3D::Play(const wstring& _strName, bool _bRepeat)
{
	m_isRun = true;
	m_pPrevAnim = m_pCurrentAnim;
	changeAnimClip(_strName);
	m_bRepeat = _bRepeat;
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
	// map ��ü ����
	UINT iEventsCount = (UINT)m_Events.size();
	fwrite(&iEventsCount, sizeof(int), 1, _pFile);					// �̺�Ʈ�� ũ�� ����
	for (auto& animEvent : m_Events)
	{
		// key : �̰� ���ڿ��� �����ϴϱ� ���� ����
		SaveWString(animEvent.first, _pFile);						// �̺�Ʈ �̸�
		// value : ���⼭ ���� �����?
		animEvent.second->Save(_pFile);								// �̺�Ʈ �Լ�
	}
	
	UINT iClipCount = (UINT)m_mapAnim.size();
	fwrite(&iClipCount, sizeof(int), 1, _pFile);					// �ִϸ��̼� ���� ����
	for (auto& animClip : m_mapAnim)
	{
		// key
		SaveWString(animClip.first, _pFile);						// �ִϸ��̼� �̸�
		// value
		SaveResRef(animClip.second.Get(), _pFile);					// �ִϸ��̼� ������
	}
	// �� Ŭ�� ���� ����
	SaveResRef(m_pCurrentAnim, _pFile);
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
	// map ��ü �ε�
	m_Events.clear();												// �̺�Ʈ �� �ʱ�ȭ
	UINT iEventsCount;
	fread(&iEventsCount, sizeof(int), 1, _pFile);					// �̺�Ʈ �� ũ�� ������
	
	for (int curEvent = 0; curEvent < iEventsCount; ++curEvent)		// ũ�⸸ŭ ��ȸ
	{
		wstring eventKey;
		Events* eventValue = new Events();
		LoadWString(eventKey, _pFile);
		eventValue->Load(_pFile);
		m_Events.insert(make_pair(eventKey, eventValue));
	}

	m_mapAnim.clear();
	UINT iClipCount;
	fread(&iClipCount, sizeof(int), 1, _pFile);
	
	for (int curClip = 0; curClip < iClipCount; ++curClip)
	{
		wstring clipKey;
		Ptr<CAnimClip> clipVlaue;
		LoadWString(clipKey, _pFile);
		LoadResRef(clipVlaue, _pFile);
		m_mapAnim.insert(make_pair(clipKey, clipVlaue));
	}

	// �� Ŭ�� ���� �ε�
	Ptr<CAnimClip> pAnim;
	LoadResRef(pAnim, _pFile);

	if (pAnim.Get())
	{
		changeAnimClip(pAnim.Get()->GetKey());
		UINT iBoneCount = m_pCurrentAnim->GetOriginMesh().Get()->GetMTBoneCount();
		m_vecFinalBoneMat.resize(iBoneCount);
	}
	
	if (nullptr == m_BoneFinalMatBuffer)
		m_BoneFinalMatBuffer = new CStructuredBuffer();
	if (nullptr == m_modifyIndicesBuffer)
		m_modifyIndicesBuffer = new CStructuredBuffer();
	// ���� ���� ����
	m_isRun = false;
	m_bRepeat = false;
	m_blendTime.SetFinishTime(BlendEndTime);
	m_blendRatio = 0.f;
	m_prevFrameIdx = -1;
}