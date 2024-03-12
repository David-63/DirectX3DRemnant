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

		// 프레임 최대 크기 저장
		int frameCnt = ActionEvents.size();
		fwrite(&frameCnt, sizeof(int), 1, _pFile);

		// 프레임 순회하며 유효한 이벤트 확인
		int cnt = 0;
		for (; cnt < frameCnt; ++cnt)
		{
			// 빈자리면 넘김
			if (nullptr == ActionEvents[cnt].mEvent)
				continue;
			// 유효한 인덱스 및 이벤트 등록
			fwrite(&cnt, sizeof(int), 1, _pFile);
			fwrite(&ActionEvents[cnt].mEvent, sizeof(Event), 1, _pFile);
		}
		// 마감 숫자 등록
		cnt = -1;
		fread(&cnt, sizeof(int), 1, _pFile);
	}

	Events* Load(FILE* _pFile)
	{
		fread(&StartEvent, sizeof(Event), 1, _pFile);
		fread(&CompleteEvent, sizeof(Event), 1, _pFile);
		fread(&EndEvent, sizeof(Event), 1, _pFile);

		// 벡터 크기 갱신
		int frameCnt = 0;
		fread(&frameCnt, sizeof(int), 1, _pFile);
		ActionEvents.resize(frameCnt);

		for (int i = 0; i < frameCnt; ++i)
		{
			// 인덱스가 마감인 경우 탈출
			UINT idx = -1;
			fread(&idx, sizeof(int), 1, _pFile);
			if (idx == -1)
				break;

			// 찾은 숫자로 인덱싱해서 이벤트 등록
			fread(&ActionEvents[idx].mEvent, sizeof(Event), 1, _pFile);
		}
		return this;
	}	
};


class CStructuredBuffer;
class CAnimator3D : public CComponent
{
private:
	// Animator value
	map<wstring, Events*>		m_Events;
	map<wstring, Ptr<CAnimClip>>m_mapAnim;
	
	// local value
	bool                        m_isRun;					// animation ctrl
	bool						m_bRepeat;					// .
	CAnimClip*					m_pPrevAnim;				// clip progress
	CAnimClip*					m_pCurrentAnim;				//
	tAnim3DData                 m_CurAnimData;				//
	vector<float>				m_AnimUpdateTime;			//
	int							m_CurFrameIdx;				//
	int							m_NextFrameIdx;				//
    float						m_AnimRatio;				// . 
	bool						m_isFinalMatUpdate;			// buffer ctrl
	CStructuredBuffer*			m_BoneFinalMatBuffer;		//
	vector<Matrix>				m_vecFinalBoneMat;			// .

	// blend value
	tTimeCtrl					m_blendTime;												
	int							m_prevFrameIdx;
	float						m_blendRatio;

	// modify value
	bool						m_isModifyUse;
	float						m_modifyRotScalar;
	CStructuredBuffer*			m_modifyIndicesBuffer;
	UINT						m_targetBone;
	vector<tPassIndices>		m_modifyIndices;
	vector<tMTBone>				m_curBones;

public:
	virtual void finaltick() override;
	void UpdateData();
	void ClearData();

public:
	void SimpleGen();
	void SimpleGen(wstring _name);


private:
	void animaTick();
	void blendTick();
	void check_mesh(Ptr<CMesh> _pMesh);
	Ptr<CAnimClip> findAnim(const wstring& _strName);
	void changeAnimClip(wstring _strAnimName);

public:
	void Reset() { m_AnimUpdateTime[m_CurAnimData.AnimClipIdx] = 0.f; m_CurAnimData.IsFinish = false; }
	void TimeClear() { m_AnimUpdateTime[m_CurAnimData.AnimClipIdx] = 0.f; }


	// modify func
public:
	void SetModifyUse(bool _use) { m_isModifyUse = _use; }
	void SetModifyRotScalar(float _scalar) { m_modifyRotScalar = _scalar; }
	bool GetModifyUse() { return m_isModifyUse;}
	float GetModifyRotScalar() { return m_modifyRotScalar; }




	// btn func
public:
	void Add(Ptr<CAnimClip> _clip);
	void Add(wstring _name);
	void Remove(const wstring& _key);
	void MakeAnimClip(string _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _inMesh);		// UI에서 생성할 때
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
			return m_pCurrentAnim->GetMTAnimClips().at(m_CurAnimData.AnimClipIdx);
	}
	UINT GetCurMTBoneCount()
	{
		if (nullptr != m_pCurrentAnim)
			return m_pCurrentAnim->GetMTBoneCount();
	}
	const vector<Matrix>& GetBonesSocket() { return m_vecFinalBoneMat; }
	Matrix& GetBoneSocket(UINT _idx) { return m_vecFinalBoneMat[_idx]; }

	// 이거 아마 안쓰는듯?
	CStructuredBuffer* GetFinalBoneMat() { return m_BoneFinalMatBuffer; }

	// Anim func
public:
	const map<wstring, Ptr<CAnimClip>>& GetAnims() { return m_mapAnim; }
	CAnimClip* GetCurAnim() { return m_pCurrentAnim; }
	const tAnim3DData& GetCurAnimData() { return m_CurAnimData; }
	
	// debugging func
public:
	const float& GetFinishTime() { return m_CurAnimData.FinishTime; }
	const float& GetCurTime() { return m_AnimUpdateTime[m_CurAnimData.AnimClipIdx]; }
	const int& GetCurFrame() { return m_CurFrameIdx; }
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

