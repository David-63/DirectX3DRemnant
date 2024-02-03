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
    // mesh로부터 가져온 boneData
    //vector<tMTAnimClip>         m_pVecClip;
    //vector<tMTBone>             m_pVecBones;
    //CStructuredBuffer*          m_BoneOffsetBuffer;     // 뼈 기본 위치              // mesh 로부터 가져와서 사용중
    //CStructuredBuffer*          m_FrameDataBuffer;      // 프레임별 뼈 이동위치       // mesh 로부터 가져와서 사용중

    CStructuredBuffer*          m_pBoneFinalMatBuffer;  // CS에서 업데이트 되는 최종 뼈 행렬



    // 애니메이션 정보
    string                      m_AnimName;             // Gui에 출력하는 용도
    tAnim3DData                 m_AnimData;

    int							m_iFrameCount;          // 현재 프레임 (30 기준)
    vector<float>				m_AnimUpdateTime;       // 애니메이션 진행 누적시간

    int							m_CurFrameIdx;          // 현재 진행중인 프레임
    int							m_NextFrameIdx;         // 다음 프레임 인덱스
    float						m_Ratio;                // 두 프레임간 진행도 비율

    // 애니메이션 제어 변수
    bool                        m_isRun;
    bool                        m_Finish;
    bool						m_bFinalMatUpdate;      // 업데이트 체크용

    vector<Matrix>				m_vecFinalBoneMat;      // 안쓰지만, 혹시 몰라서 냅둠

public:
    int finaltick();
    void UpdateData();
    void ClearData();
    void check_mesh(Ptr<CMesh> _pMesh);

    // 애니메이터에서 사용하는 함수
public:
    void NewAnimClip(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime, Ptr<CMesh> _pMesh);
    void Edit(float _begin, float _end);

public: 
    bool IsRun() { return m_isRun; }
    bool IsFinish() { return m_Finish; }

    void Play() { m_isRun = true; }
    void Stop() { m_isRun = false; }
    // 리셋은 애니메이션을 초기상태로 돌리지만, 실행시키진 않음
    void Reset() { m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f; m_Finish = false; }
    void TimeClear() { m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f; }

    // 안쓰는 함수
    int ConvertTimeToFrame(float _idxTime) {}

    // GUI에 노출시키는 함수
public: 
    // MT 정보
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
    const int& GetCurFrame() { return m_CurFrameIdx; }

public:
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
public:
    int Save(const wstring& _strRelativePath);
    int Load(const wstring& _strFilePath);

    void SaveToLevelFile(FILE* _File);
    void LoadFromLevelFile(FILE* _File);


    CLONE(CAnimClip);
public:
    CAnimClip(bool _bEngine = false);
    ~CAnimClip();

    friend class CAnimator3D;
};

