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

    float                       StartTime;          // �ִϸ��̼� ���۽ð�   // ��¼�� 0�̶� ��� �Ǵ� ����
    float                       FinishTime;         // �ִϸ��̼� ����ð�

    tAnim3DData() : AnimClipIdx(0), BeginTime(0.f), EndTime(0.f)
        , StartTime(0.f), FinishTime(0.f)
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
    void CreateAnimation3D(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime);
    int ConvertTimeToFrame(float _idxTime);

public: 
    bool IsFinish() { return m_Finish; }

    // ������ �ִϸ��̼��� �ʱ���·� ��������, �����Ű�� ����
    void TimeReset() { m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f; }
    void Reset() { m_AnimUpdateTime[m_AnimData.AnimClipIdx] = 0.f; m_Finish = false; }
    void Stop() { m_Finish = true; }
    void Continue()
    {
        if (m_AnimUpdateTime[m_AnimData.AnimClipIdx] >= m_AnimData.FinishTime)
        {
            m_AnimUpdateTime[m_AnimData.AnimClipIdx] = m_AnimData.StartTime;
            m_Finish = true;
        }
        m_Finish = false;
    }
    void Edit() { m_AnimData.FinishTime = m_AnimData.EndTime - m_AnimData.BeginTime; }

    // GUI�� �����Ű�� �Լ�
public: // Ŭ�� ����    
    const string& GetAnimName() { return m_AnimName; }
    int GetAnimClipIdx() { return m_AnimData.AnimClipIdx; }
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    const float& GetBeginTime() { return m_AnimData.BeginTime; }
    const float& GetEndTime() { return m_AnimData.EndTime; }

    // �ִ� ����
    const float& GetFinishTime() { return m_AnimData.FinishTime; }
    const float& GetCurTime() { return m_AnimUpdateTime[m_AnimData.AnimClipIdx]; }
    const int& GetCurFrame() { return m_CurFrameIdx; }

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

