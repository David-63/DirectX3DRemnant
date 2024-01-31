#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CAnim3D.h"


struct Event
{
	void operator=(std::function<void()> func)
	{
		mEvent = std::move(func);
	}
	void operator()()
	{
		if (mEvent)
			mEvent();
	}
	std::function<void()> mEvent;
};
struct Events
{
	Event StartEvent;
	Event CompleteEvent;
	Event EndEvent;
	std::vector<Event> ActionEvents;
};

class CStructuredBuffer;
class CAnimator3D : public CComponent
{
private:
	const vector<tMTBone>*		m_pVecBones;
	const vector<tMTAnimClip>*	m_pVecClip;
	map<wstring, Events*>		m_Events;

	map<wstring, CAnim3D*>		m_mapAnim;  // Animation ���

	CAnim3D*					m_pCurAnim; // ���� ������� Animation
	bool						m_bRepeat;  // �ݺ�

	// ���⼭ �ִ� ������ ���� ������ �����ϸ� ������

public:
	virtual void finaltick() override;
	void UpdateData();
	void ClearData();

public:
	void CreateAnimation3D(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime);

	// �ݺ��ִϸ��̼��� ��� Play�� ȣ��� �� ������ ���� �ִϸ��̼��� ����� Play�� ȣ���ϴ°�� EndEvent�� �ݵ�� ȣ���
	void Play(const wstring& _strName, bool _bRepeat);

	// �Ʒ��� �ִ� ��ɵ��� ���߿� �պ� ������, �Ⱦ��°� ������?
	void Stop() { m_pCurAnim->Stop(); }
	void Continue(bool _bRepeat) { m_pCurAnim->Continue(); m_bRepeat = _bRepeat; }
	void Change(const wstring& _strName);
	CAnim3D* FindAnim(const wstring& _strName);

	//event func
public:
	Events* FindEvents(const std::wstring& name);
	std::function<void()>& StartEvent(const std::wstring& name);
	std::function<void()>& CompleteEvent(const std::wstring& name);
	std::function<void()>& EndEvent(const std::wstring& name);
	std::function<void()>& ActionEvent(const std::wstring& name, UINT index);


	// animSet func
public:
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; }
	const vector<tMTBone>* GetBones() { return m_pVecBones; }
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip) { m_pVecClip = _vecAnimClip; }
	const vector<tMTAnimClip>* GetAnimClip() { return m_pVecClip; }
	void SetRepeat(bool _isRepeat) { m_bRepeat = _isRepeat; }
	bool IsRepeat() { return m_bRepeat; }


	const map<wstring, CAnim3D*>& GetAnims() { return m_mapAnim; }
	CAnim3D* GetCurAnim() { return m_pCurAnim; }
	
	// �̰� �Ƹ� �Ⱦ��µ�?
	CStructuredBuffer* GetFinalBoneMat()
	{
		if (nullptr != m_pCurAnim)
			return m_pCurAnim->GetFinalBoneMat();
	}

	// Anim3D�� �����ϴ� �Լ�
	UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }

	// getset gpu func
public:
	tMTAnimClip GetCurClip()
	{
		if (nullptr != m_pCurAnim)
			return m_pVecClip->at(m_pCurAnim->GetAnimClipIdx());
	}
	int GetClipIdx() { return m_pCurAnim->GetAnimClipIdx(); }

	float GetBeginTime() { return m_pCurAnim->GetBeginTime(); }
	float GetEndTime() { return m_pCurAnim->GetEndTime(); }
	float GetFinishTime() { return m_pCurAnim->GetFinishTime(); }
	float GetCurTime() { return m_pCurAnim->GetCurTime(); }
	int GetCurFrame() { return m_pCurAnim->GetCurFrame(); }

public:
	// ��� ���ϰ� ����
	//void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

	void SaveAnimClip();
	void LoadAnimClip();

	virtual void SaveToLevelFile(FILE* _pFile) override;
	virtual void LoadFromLevelFile(FILE* _pFile) override;
	CLONE(CAnimator3D);
public:
	CAnimator3D();
	CAnimator3D(const CAnimator3D& _origin);
	~CAnimator3D();

	friend class CAnim3D;
};

