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

    tAnim3DData() : AnimClipIdx(0), BeginTime(0.f), EndTime(0.f), FinishTime(0.f)
    {}
    ~tAnim3DData() {}
};

class CAnimator3D;
class CStructuredBuffer;
class CAnimClip : public CRes
{
private:
    CAnimator3D*                m_Owner;
    Ptr<CMesh>                  m_originMesh;

    // 애니메이션 정보
    string                      m_AnimName;             // Gui에 출력하는 용도
    tAnim3DData                 m_AnimData;

    int							m_iFrameCount;          // 현재 프레임 (30 기준)
    vector<float>				m_AnimUpdateTime;       // 애니메이션 진행 누적시간

    int							m_CurFrameIdx;          // 현재 진행중인 프레임
    int							m_NextFrameIdx;         // 다음 프레임 인덱스
    float						m_Ratio;                // 두 프레임간 진행도 비율

    // 애니메이션 제어 변수
    bool                        m_Finish;


public:
    int finaltick();

    // 애니메이터에서 사용하는 함수
public:
    void NewAnimClip(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _pMesh);
    void Edit(float _begin, float _end);

public: 
    bool IsFinish() { return m_Finish; }

    // 리셋은 애니메이션을 초기상태로 돌리지만, 실행시키진 않음
    void Reset() { m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f; m_Finish = false; }
    void TimeClear() { m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f; }

    
    int ConvertTimeToFrame(float _idxTime);
    const int& GetStartFrame();
    const int& GetCurFrame() { return m_CurFrameIdx; }
    const int& GetNextFrame() { return m_NextFrameIdx; }
    const float& GetFrameRatio() { return m_Ratio; }

    // GUI에 노출시키는 함수
public: 
    // MT 정보
    Ptr<CMesh> GetOriginMesh() { return m_originMesh; }
    vector<tMTAnimClip> GetMTAnimClips() { return m_originMesh.Get()->GetMTAnimClips(); }

    // 클립 정보
    void SetAnimName(const string& _name) { m_AnimName = _name; }
    const string& GetAnimName() { return m_AnimName; }
    int GetClipIdx() { return m_AnimData.AnimClipIdx; }
    const float& GetBeginTime() { return m_AnimData.BeginTime; }
    const float& GetEndTime() { return m_AnimData.EndTime; }

    // 애님 정보
    const float& GetFinishTime() { return m_AnimData.FinishTime; }
    const float& GetCurTime() { return m_AnimUpdateTime[m_AnimData.AnimClipIdx]; }

public:
    int Save(const wstring& _strRelativePath);
    int Load(const wstring& _strFilePath);


    CLONE(CAnimClip);
public:
    CAnimClip(bool _bEngine = false);
    ~CAnimClip();

    friend class CAnimator3D;
};

