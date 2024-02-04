#include "pch.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	: m_pCurrentAnim(nullptr), m_pNextAnim(nullptr), m_bRepeat(false)

	, m_isBlend(false), m_isBlendMatUpdate(false)
	, m_blendUpdateTime(0.f), m_blendFinishTime(1.f), m_blendRatio(0.f)
	, m_prevFrameIdx(0), m_nextFrameIdx(0), m_BlendAnimMatBuffer(nullptr)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_BlendAnimMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pCurrentAnim(_origin.m_pCurrentAnim)
	, m_bRepeat(_origin.m_bRepeat)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_BlendAnimMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	//Safe_Del_Map(m_mapAnim);
	Safe_Del_Map(m_Events);
	if (nullptr != m_BlendAnimMatBuffer)
	{
		delete m_BlendAnimMatBuffer;
		m_BlendAnimMatBuffer = nullptr;
	}
}


void CAnimator3D::finaltick()
{
	if (nullptr == m_pCurrentAnim)
		return;

	if (m_isBlend)
		blendTick();
	else
		animaTick();
}

void CAnimator3D::UpdateData()
{
	if (m_isBlend)
		blendUpdateData();
	else
		animaUpdateData();
}

void CAnimator3D::ClearData()
{
	if (m_isBlend)
		blendClearData();
	else
		animaClearData();	
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

void CAnimator3D::animaUpdateData()
{
	if (nullptr != m_pCurrentAnim)
		m_pCurrentAnim->UpdateData();

}

void CAnimator3D::blendUpdateData()
{
	if (!m_isBlendMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();

		// Bone Data
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		check_mesh(pMesh);

		// ���̴��� ����ϴ� ���۴� �޽��κ��� ����������
		// ���������δ� �� �ִϸ����͸��� ���Ǵ� ������ ������ �ٸ��� ������
		// ������ �ִϸ��̼� Ŭ���� ����ص� ������ ������
		pUpdateShader->SetFrameDataBuffer(pMesh.Get()->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh.Get()->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_BlendAnimMatBuffer);

		// m_Const ������ ���� ������
		UINT iBoneCount = (UINT)pMesh.Get()->GetMTBoneCount();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_prevFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_nextFrameIdx);
		pUpdateShader->SetFrameRatio(m_blendRatio);

		// ������Ʈ ���̴� ����
		pUpdateShader->Execute();

		m_isBlendMatUpdate = true;
	}

	// t30 �������Ϳ� ������� ������(��������) ���ε�		
	m_BlendAnimMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
}

void CAnimator3D::animaClearData()
{
	if (nullptr != m_pCurrentAnim)
		m_pCurrentAnim->ClearData();
}

void CAnimator3D::blendClearData()
{
	m_BlendAnimMatBuffer->Clear();

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
	if (m_BlendAnimMatBuffer->GetElementCount() != iBoneCount)
	{
		m_BlendAnimMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
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
	m_pNextAnim = FindAnim(_strName).Get();
	Events* events;

	// Ű���� ��
	if (m_pCurrentAnim->GetKey() != m_pNextAnim->GetKey())
	{
		//Blend ����
		m_prevFrameIdx = m_pCurrentAnim->GetCurFrame();
		m_nextFrameIdx = m_pNextAnim->GetStartFrame();
		m_isBlend = true;

		// ���� �ִϸ��̼��� ���� �̺�Ʈ ȣ�� �� �ʱ�ȭ
		events = FindEvents(m_pCurrentAnim->GetKey());
		if (events)
			events->EndEvent();
		m_pCurrentAnim->Reset();

		// ���� �ִϸ��̼��� ���� �ִϸ��̼����� ��ü
		m_pCurrentAnim = m_pNextAnim;
		m_pCurrentAnim->Reset();
		m_pCurrentAnim->Play();							// m_isBlend �� true �� ��������, ���尡 ���������� �۵� ���Ұ���

		// �ִϸ��̼� ����... ������ ���߿� �ϴ°ŷ�
		/*events = FindEvents(m_pNextAnim->GetKey());
		if (events)
			events->StartEvent();*/
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

	

	//Events* events;
	//m_pPreAnim = m_pCurrentAnim;

	//m_pCurrentAnim = FindAnim(_strName).Get();
	//assert(m_pCurrentAnim);

	//// ���� �ִϸ��̼ǰ� ���� �ִϸ��̼��� Ű�� �� : ���� �ִϸ��̼��̸� �׳� Play
	//if (m_pPreAnim->GetKey() == m_pCurrentAnim->GetKey())
	//{
	//	if (m_pCurrentAnim->IsFinish())
	//	{
	//		m_pCurrentAnim->Reset();
	//		m_pCurrentAnim->Play();
	//	}
	//	else
	//		m_pCurrentAnim->Play();
	//}	
	//// �ٸ� �ִϸ��̼��̸� EndEvent ������ �ʱ�ȭ���ֱ�
	//else
	//{
	//	events = FindEvents(m_pPreAnim->GetKey());
	//	if (events)
	//		events->EndEvent();
	//	m_pPreAnim->Reset();

	//	events = FindEvents(m_pCurrentAnim->GetKey());
	//	if (events)
	//		events->StartEvent();
	//	m_pCurrentAnim->Reset();
	//	m_pCurrentAnim->Play();		
	//}
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