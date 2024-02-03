#include "pch.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	: m_pPreAnim(nullptr)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{	
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pPreAnim(nullptr)
	, m_pCurAnim(_origin.m_pCurAnim) // 이렇게 만들면 안되용 (임시)
	, m_bRepeat(_origin.m_bRepeat)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
}

CAnimator3D::~CAnimator3D()
{
	//Safe_Del_Map(m_mapAnim);
	Safe_Del_Map(m_Events);
}


void CAnimator3D::finaltick()
{
	if (nullptr == m_pCurAnim)
		return;

	// 애니메이션에 있는 이벤트 가져오기
	Events* events = FindEvents(m_pCurAnim->GetKey());

	if (m_pCurAnim->IsFinish())
	{
		if (events)
			events->CompleteEvent();
		if (m_bRepeat)
			m_pCurAnim->Reset();
		else
			m_pCurAnim->TimeClear();
	}
	
	// 임의의 클립을 Start / End Time을 0 ~ end로 보정하기
	// 초 -> Frame 변환 기능

	UINT frameIndex = m_pCurAnim->finaltick();

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

void CAnimator3D::UpdateData()
{
	if (nullptr != m_pCurAnim)
		m_pCurAnim->UpdateData();
}

void CAnimator3D::ClearData()
{
	if (nullptr != m_pCurAnim)
		m_pCurAnim->ClearData();
}


void CAnimator3D::Add(Ptr<CAnimClip> _clip)
{
	_clip->m_Owner = this;
	m_mapAnim.insert(make_pair(_clip->GetKey(), _clip.Get()));
	m_pCurAnim = _clip.Get();
	m_pCurAnim->Stop();

	Events* events = FindEvents(_clip->GetKey());
	if (events)
		return;

	events = new Events();
	// 최대 프레임수만큼
	int maxFrame = (m_pCurAnim->GetEndTime() - m_pCurAnim->GetBeginTime()) * 30 + 1;
	events->ActionEvents.resize(maxFrame);
	m_Events.insert(std::make_pair(_clip->GetKey(), events));
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
	m_pCurAnim = pAnim.Get();
	m_pCurAnim->Stop();

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
	m_pCurAnim = pAnim.Get();
	m_pCurAnim->Stop();

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
	Events* events;
	m_pPreAnim = m_pCurAnim;

	m_pCurAnim = FindAnim(_strName).Get();
	assert(m_pCurAnim);

	// 현재 애니메이션과 이전 애니메이션의 키를 비교 : 같은 애니메이션이면 그냥 Play
	if (m_pPreAnim->GetKey() == m_pCurAnim->GetKey())
	{
		if (m_pCurAnim->IsFinish())
		{
			m_pCurAnim->Reset();
			m_pCurAnim->Play();
		}
		else
			m_pCurAnim->Play();
	}	
	// 다른 애니메이션이면 EndEvent 날리고 초기화해주기
	else
	{
		events = FindEvents(m_pPreAnim->GetKey());
		if (events)
			events->EndEvent();
		m_pPreAnim->Reset();

		events = FindEvents(m_pCurAnim->GetKey());
		if (events)
			events->StartEvent();
		m_pCurAnim->Reset();
		m_pCurAnim->Play();		
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