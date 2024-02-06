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

		// 블렌드를 해야하면
		if (m_isBlend)
		{
			// mesh를 두개 받아야함
			Ptr<CMesh> pPrevMesh = m_pPrevAnim->GetOriginMesh();
			check_mesh(pPrevMesh);

			Ptr<CMesh> pCurMesh = m_pCurrentAnim->GetOriginMesh();
			check_mesh(pCurMesh);



			// 쉐이더에 등록하는 버퍼는 메쉬로부터 가져오지만
			// 내부적으로는 각 애니메이터마다 계산되는 프레임 정보가 다르기 때문에
			// 동일한 애니메이션 클립을 사용해도 개별로 동작함
			pUpdateShader->SetFrameDataBuffer(pPrevMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetFrameDataBuffer_next(pCurMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetOffsetMatBuffer(pCurMesh.Get()->GetBoneOffsetBuffer());
			pUpdateShader->SetOutputBuffer(m_BoneFinalMatBuffer);

			// m_Const 변수에 담기는 데이터
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



			// 쉐이더에 등록하는 버퍼는 메쉬로부터 가져오지만
			// 내부적으로는 각 애니메이터마다 계산되는 프레임 정보가 다르기 때문에
			// 동일한 애니메이션 클립을 사용해도 개별로 동작함
			pUpdateShader->SetFrameDataBuffer(pCurMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetFrameDataBuffer_next(pCurMesh.Get()->GetBoneFrameDataBuffer());
			pUpdateShader->SetOffsetMatBuffer(pCurMesh.Get()->GetBoneOffsetBuffer());
			pUpdateShader->SetOutputBuffer(m_BoneFinalMatBuffer);

			// m_Const 변수에 담기는 데이터
			UINT iBoneCount = (UINT)pCurMesh.Get()->GetMTBoneCount();
			pUpdateShader->SetBoneCount(iBoneCount);
			pUpdateShader->SetFrameIndex(m_pCurrentAnim->GetCurFrame());
			pUpdateShader->SetNextFrameIdx(m_pCurrentAnim->GetNextFrame());
			pUpdateShader->SetFrameRatio(m_pCurrentAnim->GetFrameRatio());
		}		

		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();

		m_isFinalMatUpdate = true;
	}

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
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

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::animaTick()
{
	// 애니메이션에 있는 이벤트 가져오기
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

	// 임의의 클립을 Start / End Time을 0 ~ end로 보정하기
	// 초 -> Frame 변환 기능
	UINT frameIndex = m_pCurrentAnim->finaltick();
	m_isFinalMatUpdate = false;


	if (events)
	{
		if (frameIndex >= events->ActionEvents.size())
			return;
		// Complete가 아니고, 프레임 중간에 ActionEvent가 있다면 그것을 실행
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
	// 최대 프레임수만큼
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
	m_pCurrentAnim->Stop();

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
	// 현재 애님과 이후에 들어올 애님을 비교
	m_pPrevAnim = m_pCurrentAnim;
	m_pCurrentAnim = FindAnim(_strName).Get();
	Events* events;

	// 키값을 비교
	if (m_pPrevAnim->GetKey() != m_pCurrentAnim->GetKey())
	{
		//Blend 진행
		m_prevFrameIdx = m_pPrevAnim->GetCurFrame();
		m_isBlend = true;

		// 현재 애니메이션의 종료 이벤트 호출 및 초기화
		events = FindEvents(m_pPrevAnim->GetKey());
		if (events)
			events->EndEvent();
		m_pPrevAnim->Reset();

		// 다음 애니메이션을 현재 애니메이션으로 교체
		m_pCurrentAnim->Play();

		// 애니메이션 시작... 조건은 나중에 하는거로
		events = FindEvents(m_pCurrentAnim->GetKey());
		if (events)
			events->StartEvent();
	}
	else
	{
		// 바뀐게 없음
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