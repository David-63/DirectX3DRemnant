#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CStructuredBuffer.h"

struct tAnim3DData
{
    int                         AnimClipIdx;        // Ŭ�� �ε��� (���� ���� ���)
    float                       BeginTime;          // Ŭ���� ���۽ð�
    float                       EndTime;            // Ŭ���� ����ð�

    float                       FinishTime;         // �ִϸ��̼� ����ð�
    bool                        IsFinish;             // �ִϸ��̼� ����

    tAnim3DData() : AnimClipIdx(0), BeginTime(0.f), EndTime(0.f), FinishTime(0.f), IsFinish(false)
    {}
    ~tAnim3DData() {}
};

class CAnimator3D;
class CStructuredBuffer;
class CAnimClip : public CRes
{
private:
    Ptr<CMesh>                  m_originMesh;

    // �ִϸ��̼� ����
    string                      m_AnimName;             // Gui�� ����ϴ� �뵵
    tAnim3DData                 m_AnimData;
    int							m_iFrameCount;          // ���� ������ (30 ����)

public:
    void MakeAnimClip(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _pMesh);


public:
    void SetAnimName(const string& _name) { m_AnimName = _name; }
    const string& GetAnimName() { return m_AnimName; }
    const tAnim3DData& GetAnimData() { return m_AnimData; }
    int GetFrameCount() { return m_iFrameCount; }

    // MT ��ü
public:
    Ptr<CMesh> GetOriginMesh() { return m_originMesh; }
    const vector<tMTAnimClip>& GetMTAnimClips() { return m_originMesh.Get()->GetMTAnimClips(); }
    const vector<tMTBone>& GetMTBones() { return m_originMesh.Get()->GetMTBones(); }
    UINT GetMTBoneCount() { return m_originMesh.Get()->GetMTBoneCount(); }
            
public:
    int Save(const wstring& _strRelativePath);
    int Load(const wstring& _strFilePath);
        
public:
    CAnimClip(bool _bEngine = false);
    ~CAnimClip();

    friend class CAnimator3D;
};

