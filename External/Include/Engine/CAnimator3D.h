#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CAnimClip.h"


#define BlendEndTime 0.32f


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

	void Save(FILE* _pFile)
	{
		fwrite(&StartEvent, sizeof(Event), 1, _pFile);
		fwrite(&CompleteEvent, sizeof(Event), 1, _pFile);
		fwrite(&EndEvent, sizeof(Event), 1, _pFile);
	}

	Events* Load(FILE* _pFile)
	{
		fread(&StartEvent, sizeof(Event), 1, _pFile);
		fread(&CompleteEvent, sizeof(Event), 1, _pFile);
		fread(&EndEvent, sizeof(Event), 1, _pFile);
		return this;
	}
};

class CStructuredBuffer;
class CAnimator3D : public CComponent
{
private:
	// Animator value
	map<wstring, Events*>		m_Events;
	map<wstring, Ptr<CAnimClip>>m_mapAnim;  // Animation ���

	// local value
	bool                        m_isRun;		// �ִ� ����
	bool						m_bRepeat;		// �ݺ� üũ
	CAnimClip*					m_pPrevAnim;	// ���� �ִ�
	CAnimClip*					m_pCurrentAnim; // ���� ������� �ִ�

	bool						m_isFinalMatUpdate;     // ������Ʈ üũ��
	CStructuredBuffer*			m_BoneFinalMatBuffer;	// CS���� ������Ʈ �Ǵ� ���� �� ���
	vector<Matrix>				m_vecFinalBoneMat;      // �� ����

	// blend value
	tTimeCtrl					m_blendTime;
	int							m_prevFrameIdx;
	float						m_blendRatio;

	// modify value
	bool						m_isModifyUse;
	int							m_modifyIdx;
	float						m_modifyRotScalar;
	CStructuredBuffer*			m_modifyIndicesBuffer;	// CS���� ������Ʈ �Ǵ� ���� �� ���
	vector<UINT>				m_modifyIndices;
	vector<tMTBone>				m_curBones;

public:
	virtual void finaltick() override;
	void UpdateData();
	void ClearData();

public:
	void animaTick();
	void blendTick();
	void check_mesh(Ptr<CMesh> _pMesh);
	Ptr<CAnimClip> findAnim(const wstring& _strName);

	// modify func
public:
	void SetModifyUse(bool _use) { m_isModifyUse = _use; }
	void SetModifyIdx(int _idx) { m_modifyIdx = _idx; }
	void SetModifyRotScalar(float _scalar) { m_modifyRotScalar = _scalar; }
	bool GetModifyUse() { return m_isModifyUse;}
	int GetModifyIdx() { return m_modifyIdx;}
	float GetModifyRotScalar() { return m_modifyRotScalar; }


	// btn func
public:
	void Add(Ptr<CAnimClip> _clip);
	void Remove(const wstring& _key);
	void CreateAnimClip(wstring _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _inMesh);	// Engine���� ������ ��
	void NewAnimClip(string _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _inMesh);		// UI���� ������ ��
	void Play(const wstring& _strName, bool _bRepeat);
	void Stop() { m_isRun = false; }
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
	UINT GetCurMTBoneCount()
	{
		if (nullptr != m_pCurrentAnim)
			return m_pCurrentAnim->GetMTBoneCount();
	}

	// �̰� �Ƹ� �Ⱦ��µ�?
	CStructuredBuffer* GetFinalBoneMat() { return m_BoneFinalMatBuffer; }

	// Anim func
public:
	const map<wstring, Ptr<CAnimClip>>& GetAnims() { return m_mapAnim; }
	CAnimClip* GetCurAnim() { return m_pCurrentAnim; }
	
	// debugging func
public:
	float GetBlendUpdateTime() { return m_blendTime.curTime; }
	float GetBlendRatio() { return m_blendRatio; }
	int GetPrevFrameIdx() { return m_prevFrameIdx; }

	void CollectChildrenIndices(int current_index);

public:
	virtual void SaveToLevelFile(FILE* _pFile) override;
	virtual void LoadFromLevelFile(FILE* _pFile) override;
	CLONE(CAnimator3D);
public:
	CAnimator3D();
	CAnimator3D(const CAnimator3D& _origin);
	~CAnimator3D();

	friend class CAnimClip;
};

