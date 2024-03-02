#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CStructuredBuffer.h"

struct tAnim3DData
{
    int                         AnimClipIdx;        // 클립 인덱스 (직접 만들어서 사용)
    float                       BeginTime;          // 클립의 시작시간
    float                       EndTime;            // 클립의 종료시간

    float                       FinishTime;         // 애니메이션 종료시간
    bool                        IsFinish;             // 애니메이션 종료

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

    // 애니메이션 정보
    string                      m_AnimName;             // Gui에 출력하는 용도
    tAnim3DData                 m_AnimData;
    int							m_iFrameCount;          // 현재 프레임 (30 기준)

public:
    void MakeAnimClip(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _pMesh);


public:
    void SetAnimName(const string& _name) { m_AnimName = _name; }
    const string& GetAnimName() { return m_AnimName; }
    const tAnim3DData& GetAnimData() { return m_AnimData; }
    int GetFrameCount() { return m_iFrameCount; }

    // MT 객체
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

