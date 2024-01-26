#include "pch.h"

#include "CAnimator3D.h"

#include "CAnim3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	: m_pVecBones(nullptr)
	, m_pVecClip(nullptr)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{	
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pVecBones(_origin.m_pVecBones)
	, m_pVecClip(_origin.m_pVecClip)
	, m_pCurAnim(_origin.m_pCurAnim) // 이렇게 만들면 안되용 (임시)
	, m_bRepeat(_origin.m_bRepeat)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
}

CAnimator3D::~CAnimator3D()
{
	Safe_Del_Map(m_mapAnim);
}


void CAnimator3D::finaltick()
{
	if (nullptr != m_pCurAnim)
	{
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->Reset();
		}

		m_pCurAnim->finaltick();
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


void CAnimator3D::CreateAnimation3D(const string& _strAnimName, int _clipIdx, float _startTime, float _endTime)
{
	CAnim3D* pAnim = new CAnim3D();
	pAnim->m_Owner = this;
	pAnim->CreateAnimation3D(_strAnimName, _clipIdx, _startTime, _endTime);
	m_mapAnim.insert(make_pair(_strAnimName, pAnim));
	m_pCurAnim = pAnim;
	m_pCurAnim->Stop();
}

void CAnimator3D::Play(const string& _strName, bool _bRepeat)
{
	CAnim3D* pAnim = FindAnim(_strName);
	assert(pAnim);
	m_pCurAnim->Reset();	// 초기화 한 다음에 변경해주기
	m_pCurAnim = pAnim;
	m_pCurAnim->Reset();	// 변경한 애니메이션을 초기화 해줌	
	m_bRepeat = _bRepeat;
}

void CAnimator3D::Change(const string& _strName)
{
	CAnim3D* pAnim = FindAnim(_strName);
	assert(pAnim);
	m_pCurAnim->Reset();	// 초기화 한 다음에 변경해주기
	m_pCurAnim = pAnim;
	m_pCurAnim->Reset();	// 변경한 애니메이션을 초기화 해줌	
	m_bRepeat = false;
}

CAnim3D* CAnimator3D::FindAnim(const string& _strName)
{
	map<string, CAnim3D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}


void CAnimator3D::SaveAnimClip()
{
	//m_pCurAnim->Save();
}

void CAnimator3D::LoadAnimClip()
{
}

void CAnimator3D::SaveToLevelFile(FILE* _pFile)
{
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
}