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

    tAnim3DData() : AnimClipIdx(0), BeginTime(0.f), EndTime(0.f), FinishTime(0.f)
    {}
    ~tAnim3DData() {}
};

class CAnimator3D;
class CStructuredBuffer;
class CAnim3D : public CRes
{
private:    
    CAnimator3D*                m_Owner;                // Owner�� �˾ƾ� ������Ʈ�� ���� ������
    string                      m_AnimName;

    int							m_iFrameCount;          // ���� ������ (30 ����)

    // �ִϸ��̼� ����
    tAnim3DData                 m_AnimData;

    vector<float>				m_AnimUpdateTime;       // �ִϸ��̼� ���� �����ð�

    int							m_CurFrameIdx;          // ���� �������� ������
    int							m_NextFrameIdx;         // ���� ������ �ε���
    float						m_Ratio;                // �� �����Ӱ� ���൵ ����

    bool                        m_isRun;
    bool                        m_Finish;


    vector<Matrix>				m_vecFinalBoneMat;      // �� ������ŭ ��� ����
    CStructuredBuffer*          m_pBoneFinalMatBuffer;  // CS�� ������ �� ��� ����
    bool						m_bFinalMatUpdate;      // ������Ʈ üũ��

public:
    int finaltick();
    void UpdateData();
    void ClearData();
    void check_mesh(Ptr<CMesh> _pMesh);

    // �ִϸ����Ϳ��� ����ϴ� �Լ�
public:
    void NewAnimClip(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime);
    void Edit(float _begin, float _end);

public: 
    bool IsRun() { return m_isRun; }
    bool IsFinish() { return m_Finish; }

    void Play() { m_isRun = true; }
    void Stop() { m_isRun = false; }
    // ������ �ִϸ��̼��� �ʱ���·� ��������, �����Ű�� ����
    void Reset() { m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f; m_Finish = false; }
    void TimeClear() { m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f; }

    // �Ⱦ��� �Լ�
    int ConvertTimeToFrame(float _idxTime) {}

    // GUI�� �����Ű�� �Լ�
public: // Ŭ�� ����    
    const string& GetAnimName() { return m_AnimName; }
    int GetClipIdx() { return m_AnimData.AnimClipIdx; }
    const float& GetBeginTime() { return m_AnimData.BeginTime; }
    const float& GetEndTime() { return m_AnimData.EndTime; }

    // �ִ� ����
    const float& GetFinishTime() { return m_AnimData.FinishTime; }
    const float& GetCurTime() { return m_AnimUpdateTime[m_AnimData.AnimClipIdx]; }
    const int& GetCurFrame() { return m_CurFrameIdx; }

public:
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
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
