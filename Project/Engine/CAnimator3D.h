#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CAnimClip.h"


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
	// Animator value
	map<wstring, Events*>		m_Events;
	map<wstring, Ptr<CAnimClip>>m_mapAnim;  // Animation ���

	// local value
	CAnimClip*					m_pCurrentAnim; // ���� ������� �ִ�
	CAnimClip*					m_pNextAnim;	// ����� �ִ�
	bool						m_bRepeat;		// �ݺ� üũ

	// blend value
	bool						m_isBlend;
	float						m_blendUpdateTime;
	float						m_blendFinishTime;
	float						m_blendRatio;

	int							m_prevFrameIdx;
	int							m_nextFrameIdx;

	CStructuredBuffer*			m_BlendAnimMatBuffer;  // CS���� ������Ʈ �Ǵ� ���� �� ���
	bool						m_isBlendMatUpdate;




public:
	virtual void finaltick() override;
	void UpdateData();
	void ClearData();

public:
	void animaTick();
	void blendTick();

	void animaUpdateData();
	void blendUpdateData();

	void animaClearData();
	void blendClearData();

	void check_mesh(Ptr<CMesh> _pMesh);

public:
	Ptr<CAnimClip> FindAnim(const wstring& _strName);



	// btn func
public:
	// �̰� Save Load ����� �����Ǹ� �߰��� ����
	void Add(Ptr<CAnimClip> _clip);
	void Remove() {}

	void SelectMeshData() {}
	void CreateAnimClip(wstring _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _inMesh);	// Engine���� ������ ��
	void NewAnimClip(string _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _inMesh);		// UI���� ������ ��
	void Edit(float _begin, float _end) {}


	void Play(const wstring& _strName, bool _bRepeat);

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
		if (nullptr != m_pCurrentAnim)
			return m_pCurrentAnim->GetMTAnimClips().at(m_pCurrentAnim->GetClipIdx());
	}
	

	// �̰� �Ƹ� �Ⱦ��µ�?
	CStructuredBuffer* GetFinalBoneMat()
	{
		if (nullptr != m_pCurrentAnim)
			return m_pCurrentAnim->GetFinalBoneMat();
	}

	// Anim func
public:
	const map<wstring, Ptr<CAnimClip>>& GetAnims() { return m_mapAnim; }
	CAnimClip* GetCurAnim() { return m_pCurrentAnim; }
	
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

	friend class CAnimClip;
};

