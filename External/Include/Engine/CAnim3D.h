#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CStructuredBuffer.h"

struct tAnim3DData
{
    int                         AnimClipIdx;          // 이 애니메이션의 클립 인덱스 (직접 만들어서 사용)
    float                       StartTime;            // 애니메이션 클립의 시작시간
    float                       EndTime;              // 애니메이션 클립의 종료시간
    tAnim3DData() : AnimClipIdx(0), StartTime(0.f), EndTime(0.f) {}
    ~tAnim3DData() {}
};

class CAnimator3D;
class CStructuredBuffer;
class CAnim3D : public CRes
{
private:    
    CAnimator3D*                m_Owner;                // Owner를 알아야 컴포넌트에 접근 가능함

    int							m_iFrameCount;          // 현재 프레임 (30 기준)

    // 애니메이션 정보
    tAnim3DData                 m_AnimData;

    vector<float>				m_AnimUpdateTime;       // 애니메이션 진행 누적시간

    int							m_CurFrameIdx;          // 현재 진행중인 프레임
    int							m_NextFrameIdx;         // 다음 프레임 인덱스
    float						m_Ratio;                // 두 프레임간 진행도 비율

    bool                        m_Finish;


    vector<Matrix>				m_vecFinalBoneMat;      // 뼈 개수만큼 행렬 생성
    CStructuredBuffer*          m_pBoneFinalMatBuffer;  // CS로 전달할 뼈 행렬 정보
    bool						m_bFinalMatUpdate;      // 업데이트 체크용

public:
    void finaltick();
    void UpdateData();
    void ClearData();
    void check_mesh(Ptr<CMesh> _pMesh);


public:
    void CreateAnimation3D(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime);
    //void CreateAnimation3D(const string& _strAnimName, int _clipIdx, int _startFrame, int _endFrame); // 안씀

public: // GUI에 노출시키는 함수
    const string& GetAnimName()
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(GetKey());
    }
    int GetAnimClipIdx() { return m_AnimData.AnimClipIdx; }
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    float* GetStartTime() { return &m_AnimData.StartTime; }
    float* GetEndTime() { return &m_AnimData.EndTime; }
    const float& GetCurTime() { return m_AnimUpdateTime[m_AnimData.AnimClipIdx]; }
    const int& GetCurFrame() { return m_CurFrameIdx; }

public: // 애니메이터에서 사용하는 함수
    bool IsFinish() { return m_Finish; }

    // 리셋은 애니메이션을 초기상태로 돌리지만, 실행시키진 않음
    void Reset() { m_AnimUpdateTime[m_AnimData.AnimClipIdx] = m_AnimData.StartTime; m_Finish = false; }
    void Stop() { m_Finish = true; }
    void Continue()
    {
        if (m_AnimUpdateTime[m_AnimData.AnimClipIdx] >= m_AnimData.EndTime)
        {
            m_AnimUpdateTime[m_AnimData.AnimClipIdx] = m_AnimData.StartTime;
            m_Finish = true;
        }
        m_Finish = false;
    }
public:
    int Save(const wstring& _strFilePath);
    int Load(const wstring& _strFilePath);

    void SaveToLevelFile(FILE* _File);
    void LoadFromLevelFile(FILE* _File);


    CLONE(CAnim3D);
public:
    CAnim3D(bool _bEngine);
    ~CAnim3D();

    friend class CAnimator3D;
};

