#include "pch.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	: m_pCurrentAnim(nullptr), m_pPrevAnim(nullptr), m_bRepeat(false), m_isRun(false)
	, m_isFinalMatUpdate(false), m_BoneFinalMatBuffer(nullptr)
	, m_blendTime(BlendEndTime), m_blendRatio(0.f), m_prevFrameIdx(0)
	, m_isModifyUse(), m_modifyIdx(), m_modifyRotScalar()
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_BoneFinalMatBuffer = new CStructuredBuffer();
	m_modifyIndicesBuffer = new CStructuredBuffer();
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pCurrentAnim(_origin.m_pCurrentAnim)
	, m_bRepeat(_origin.m_bRepeat)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_BoneFinalMatBuffer = new CStructuredBuffer;
	m_modifyIndicesBuffer = new CStructuredBuffer();
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

	if (m_pCurrentAnim->IsFinish())
	{
		if (events)
			events->CompleteEvent();
		if (m_bRepeat)
			m_pCurrentAnim->Reset();
		else
		{
			m_pCurrentAnim->TimeClear();
			m_isRun = false;
		}			
	}

	UINT frameIndex = m_pCurrentAnim->finaltick();
	m_isFinalMatUpdate = false;

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
			pUpdateShader->SetNextFrameIdx(m_pCurrentAnim->GetNextFrame());
			pUpdateShader->SetFrameRatio(m_blendRatio);
		}
		else
		{
			pUpdateShader->SetFrameDataBuffer(pCurMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetFrameDataBuffer_next(pCurMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetOffsetMatBuffer(pCurMesh.Get()->GetBoneOffsetBuffer());
			pUpdateShader->SetOutputBuffer(m_BoneFinalMatBuffer);

			pUpdateShader->SetBoneCount(iBoneCount);
			pUpdateShader->SetFrameIndex(m_pCurrentAnim->GetCurFrame());
			pUpdateShader->SetNextFrameIdx(m_pCurrentAnim->GetNextFrame());
			pUpdateShader->SetFrameRatio(m_pCurrentAnim->GetFrameRatio());
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

		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();

		m_isFinalMatUpdate = true;
	}

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
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

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
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


void CAnimator3D::Add(Ptr<CAnimClip> _clip)
{
	_clip->m_Owner = this;
	m_mapAnim.insert(make_pair(_clip->GetKey(), _clip.Get()));
	m_pCurrentAnim = _clip.Get();
	//m_pCurrentAnim->Stop();

	UINT iBoneCount = m_pCurrentAnim->GetOriginMesh().Get()->GetMTBoneCount();
	m_vecFinalBoneMat.resize(iBoneCount);

	m_curBones = m_pCurrentAnim->GetOriginMesh()->GetMTBones();
	CollectChildrenIndices(36);
	if (m_modifyIndicesBuffer->GetElementCount() != m_modifyIndices.size())
	{
		m_modifyIndicesBuffer->Create(sizeof(UINT), m_modifyIndices.size(), SB_TYPE::READ_ONLY, false, nullptr);
	}

	Events* events = FindEvents(_clip->GetKey());
	if (events)
		return;

	events = new Events();
	// 최대 프레임수만큼
	int maxFrame = (m_pCurrentAnim->GetEndTime() - m_pCurrentAnim->GetBeginTime()) * 30 + 1;
	events->ActionEvents.resize(maxFrame);
	m_Events.insert(std::make_pair(_clip->GetKey(), events));
}

void CAnimator3D::CollectChildrenIndices(int current_index)
{
	// 뼈 개수만큼 순회 (260개)
	for (int i = 0; i < m_curBones.size(); ++i)
	{
		if (current_index == m_curBones[i].iParentIdx)
		{
			m_modifyIndices.push_back(i); // 자식 뼈의 인덱스 추가
			CollectChildrenIndices(i); // 재귀 호출
		}
	}
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
	//m_pCurrentAnim->Stop();

	Events* events = FindEvents(_strAnimName);
	if (events)
		return;
	
	events = new Events();
	// 최대 프레임수만큼
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
	//m_pCurrentAnim->Stop();

	Events* events = FindEvents(strPath);
	if (events)
		return;

	events = new Events();
	// 최대 프레임수만큼
	int maxFrame = (_endTime - _startTime) * 30 + 1;
	events->ActionEvents.resize(maxFrame);
	m_Events.insert(std::make_pair(strPath, events));
}

void CAnimator3D::Play(const wstring& _strName, bool _bRepeat)
{
	m_isRun = true;
	Events* events;

	m_pPrevAnim = m_pCurrentAnim;
	m_pCurrentAnim = findAnim(_strName).Get();

	// 키값을 비교
	if (m_pPrevAnim->GetKey() != m_pCurrentAnim->GetKey())
	{
		// =============
		//Blend 진행 로직
		// =============

		// 1. 방금까지 재생중인 애님의 현재 프레임을 기록
		m_prevFrameIdx = m_pPrevAnim->GetCurFrame();
		// 2. 블렌드 조건 수락
		m_blendTime.Activate();
		// 3. 현재 애니메이션의 종료 이벤트 호출 및 초기화
		events = FindEvents(m_pPrevAnim->GetKey());
		if (events)
			events->EndEvent();
		// 4. 이전 애님은 초기화 해주기
		m_pPrevAnim->Reset();
	}
	else
	{
		// 바뀐게 없음
		if (m_pCurrentAnim->IsFinish())
		{
			m_pCurrentAnim->Reset();
		}
	}
	m_bRepeat = _bRepeat;
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
	// map 객체 저장
	// vector 방식이라면 size 부터 저장해서 순회하며 SaveResRef 함수로 저장하는데
	
	UINT iEventsCount = (UINT)m_Events.size();
	fwrite(&iEventsCount, sizeof(int), 1, _pFile);
	for (auto& animEvent : m_Events)
	{
		// key
		SaveWString(animEvent.first, _pFile);		
		// value
		animEvent.second->Save(_pFile);
	}
	
	UINT iClipCount = (UINT)m_mapAnim.size();
	fwrite(&iEventsCount, sizeof(int), 1, _pFile);
	for (auto& animClip : m_mapAnim)
	{
		// key
		SaveWString(animClip.first, _pFile);
		// value
		SaveResRef(animClip.second.Get(), _pFile);		
	}

	// 현 클립 정보 저장
	SaveResRef(m_pCurrentAnim, _pFile);
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
	// map 객체 로드
	m_Events.clear();
	UINT iEventsCount;
	fread(&iEventsCount, sizeof(int), 1, _pFile);
	wstring eventKey;
	Events* eventValue = new Events();
	for (int curEvent = 0; curEvent < iEventsCount; ++curEvent)
	{
		LoadWString(eventKey, _pFile);		
		m_Events.insert(make_pair(eventKey, eventValue->Load(_pFile)));
	}

	m_mapAnim.clear();
	UINT iClipCount;
	fread(&iClipCount, sizeof(int), 1, _pFile);
	wstring clipKey;
	Ptr<CAnimClip> clipVlaue;
	for (int curClip = 0; curClip < iClipCount; ++curClip)
	{
		LoadWString(clipKey, _pFile);
		LoadResRef(clipVlaue, _pFile);
		m_mapAnim.insert(make_pair(clipKey, clipVlaue));
	}

	// 현 클립 정보 로드
	Ptr<CAnimClip> pAnim;
	LoadResRef(pAnim, _pFile);
	m_pCurrentAnim = pAnim.Get();
	UINT iBoneCount = m_pCurrentAnim->GetOriginMesh().Get()->GetMTBoneCount();
	m_vecFinalBoneMat.resize(iBoneCount);
	m_BoneFinalMatBuffer = new CStructuredBuffer();

	// 제어 변수 세팅
	m_isRun = false;
	m_bRepeat = false;
	m_blendTime.SetFinishTime(BlendEndTime);
	m_blendRatio = 0.f;
	m_prevFrameIdx = -1;
}