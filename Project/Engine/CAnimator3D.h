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

	CAnim3D*					m_pPreAnim; // ���� ������� Animation
	CAnim3D*					m_pCurAnim; // ���� ������� Animation
	bool						m_bRepeat;  // �ݺ�

	// ���⼭ �ִ� ������ ���� ������ �����ϸ� ������

public:
	virtual void finaltick() override;
	void UpdateData();
	void ClearData();

public:
	CAnim3D* FindAnim(const wstring& _strName);



	// btn func
public:	
	// �̰� Save Load ����� �����Ǹ� �߰��� ����
	void Add() {}
	void Remove() {}

	void NewAnimClip(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime);
	void Edit(float _begin, float _end) {}


	void Play(const wstring& _strName, bool _bRepeat);
	void Stop() {}

	void SetRepeat(bool _isRepeat) { m_bRepeat = _isRepeat; }
	bool IsRepeat() { return m_bRepeat; }



	//event func
public:
	Events* FindEvents(const std::wstring& name);
	std::function<void()>& StartEvent(const std::wstring& name);
	std::function<void()>& CompleteEvent(const std::wstring& name);
	std::function<void()>& EndEvent(const std::wstring& name);
	std::function<void()>& ActionEvent(const std::wstring& name, UINT index);

	// MT(Meshdata sTruct) func
public:
	tMTAnimClip GetCurMTClip()
	{
		if (nullptr != m_pCurAnim)
			return m_pVecClip->at(m_pCurAnim->GetClipIdx());
	}
	void SetMTBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; }
	const vector<tMTBone>* GetMTBones() { return m_pVecBones; }
	void SetMTAnimClips(const vector<tMTAnimClip>* _vecAnimClip) { m_pVecClip = _vecAnimClip; }
	const vector<tMTAnimClip>* GetMTAnimClips() { return m_pVecClip; }
	UINT GetMTBoneCount() { return (UINT)m_pVecBones->size(); }

	// �̰� �Ƹ� �Ⱦ��µ�?
	CStructuredBuffer* GetFinalBoneMat()
	{
		if (nullptr != m_pCurAnim)
			return m_pCurAnim->GetFinalBoneMat();
	}

	// Anim func
public:
	const map<wstring, CAnim3D*>& GetAnims() { return m_mapAnim; }
	CAnim3D* GetCurAnim() { return m_pCurAnim; }
	
public:
	void SaveAnimClip() {}
	void LoadAnimClip() {}

	virtual void SaveToLevelFile(FILE* _pFile) override;
	virtual void LoadFromLevelFile(FILE* _pFile) override;
	CLONE(CAnimator3D);
public:
	CAnimator3D();
	CAnimator3D(const CAnimator3D& _origin);
	~CAnimator3D();

	friend class CAnim3D;
};

