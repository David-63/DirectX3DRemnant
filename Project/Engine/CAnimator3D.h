#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CAnim3D.h"

class CStructuredBuffer;
class CAnimator3D : public CComponent
{
private:
	const vector<tMTBone>*		m_pVecBones;
	const vector<tMTAnimClip>*	m_pVecClip;

	map<string, CAnim3D*>		m_mapAnim;  // Animation ���
	CAnim3D*					m_pCurAnim; // ���� ������� Animation
	bool						m_bRepeat;  // �ݺ�

public:
	virtual void finaltick() override;
	void UpdateData();
	void ClearData();

public:
	void CreateAnimation3D(const string& _strAnimName, int _clipIdx, float _startTime, float _endTime);
	void Play(const string& _strName, bool _bRepeat);

	void Stop() { m_pCurAnim->Stop(); }
	void Continue(bool _bRepeat) { m_pCurAnim->Continue(); m_bRepeat = _bRepeat; }
	void Change(const string& _strName);
	CAnim3D* FindAnim(const string& _strName);


public:
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; }
	const vector<tMTBone>* GetBones() { return m_pVecBones; }
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip) { m_pVecClip = _vecAnimClip; }
	const vector<tMTAnimClip>* GetAnimClip() { return m_pVecClip; }
	void SetRepeat(bool _isRepeat) { m_bRepeat = _isRepeat; }
	bool IsRepeat() { return m_bRepeat; }


	const map<string, CAnim3D*>& GetAnims() { return m_mapAnim; }
	CAnim3D* GetCurAnim() { return m_pCurAnim; }
	
	// �̰� �Ƹ� �Ⱦ��µ�?
	CStructuredBuffer* GetFinalBoneMat()
	{
		if (nullptr != m_pCurAnim)
			return m_pCurAnim->GetFinalBoneMat();
	}

	// Anim3D�� �����ϴ� �Լ�
	UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }

	// GUI�� �����ϴ� �Լ�
	tMTAnimClip GetCurClip()
	{
		if (nullptr != m_pCurAnim)
			return m_pVecClip->at(m_pCurAnim->GetAnimClipIdx());
	}
	int GetClipIdx() { return m_pCurAnim->GetAnimClipIdx(); }

	float* GetStartTime() { return m_pCurAnim->GetStartTime(); }
	float* GetEndTime() { return m_pCurAnim->GetEndTime(); }
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

