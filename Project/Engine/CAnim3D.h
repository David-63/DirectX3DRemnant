#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CStructuredBuffer.h"

struct tAnim3DData
{
    int                         AnimClipIdx;          // �� �ִϸ��̼��� Ŭ�� �ε��� (���� ���� ���)
    float                       StartTime;            // �ִϸ��̼� Ŭ���� ���۽ð�
    float                       EndTime;              // �ִϸ��̼� Ŭ���� ����ð�
    tAnim3DData() : AnimClipIdx(0), StartTime(0.f), EndTime(0.f) {}
    ~tAnim3DData() {}
};

class CAnimator3D;
class CStructuredBuffer;
class CAnim3D : public CRes
{
private:    
    CAnimator3D*                m_Owner;                // Owner�� �˾ƾ� ������Ʈ�� ���� ������

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
    void finaltick();
    void UpdateData();
    void ClearData();
    void check_mesh(Ptr<CMesh> _pMesh);


public:
    void CreateAnimation3D(const wstring& _strAnimName, int _clipIdx, float _startTime, float _endTime);
    //void CreateAnimation3D(const string& _strAnimName, int _clipIdx, int _startFrame, int _endFrame); // �Ⱦ�

public: // GUI�� �����Ű�� �Լ�
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

public: // �ִϸ����Ϳ��� ����ϴ� �Լ�
    bool IsFinish() { return m_Finish; }

    // ������ �ִϸ��̼��� �ʱ���·� ��������, �����Ű�� ����
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

