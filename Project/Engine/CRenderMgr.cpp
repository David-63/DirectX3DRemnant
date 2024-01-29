#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CCamera.h"
#include "CLight2D.h"
#include "CLight3D.h"

#include "CResMgr.h"
#include "CMRT.h"

CRenderMgr::CRenderMgr()
    : m_Light2DBuffer(nullptr)
    , RENDER_FUNC(nullptr)
    , m_pEditorCam(nullptr)
    , m_MRT{}
{
    Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
    m_RTCopyTex = CResMgr::GetInst()->CreateTexture(L"RTCopyTex"
                                                    , (UINT)vResolution.x, (UINT)vResolution.y
                                                    , DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE
                                                    , D3D11_USAGE_DEFAULT);

    CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl")->SetTexParam(TEX_0, m_RTCopyTex);

    CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl")->SetTexParam(TEX_0, m_RTCopyTex);

}

CRenderMgr::~CRenderMgr()
{
    if (nullptr != m_Light2DBuffer)
        delete m_Light2DBuffer;

    if (nullptr != m_Light3DBuffer)
        delete m_Light3DBuffer;

    DeleteArray(m_MRT);
}

void CRenderMgr::render()
{
    // ���� �� ���� ������ ������Ʈ �� ���ε�
    UpdateData();
    // MRT Clear    
    ClearMRT();

    // �׸��� ������
    render_shadowmap();

    // ī�޶� ���� �Լ� ȣ��
    (this->*RENDER_FUNC)();
    
    // ���� ����
    Clear();
}

void CRenderMgr::render_shadowmap()
{
    m_MRT[(UINT)MRT_TYPE::SHADOWMAP]->OMSet();
    for (size_t i = 0; i < m_vecLight3D.size(); ++i)
    {
        m_vecLight3D[i]->render_shadowmap();
    }
}



void CRenderMgr::render_play()
{
    // ī�޶� ���� ������
    for (size_t i = 0; i < m_vecCam.size(); ++i)
    {
        if (nullptr == m_vecCam[i])
            continue;
        m_vecCam[i]->SortObject();
        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();
        m_vecCam[i]->render();
    }
}

void CRenderMgr::render_editor()
{
    // ��ü �з�
    m_pEditorCam->SortObject(); // <- Frustum Culling ����κ�

    // ��� Ÿ�� ����    
    m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();
    m_pEditorCam->render();
}


int CRenderMgr::RegisterCamera(CCamera* _Cam, int _idx)
{
    if (m_vecCam.size() <= _idx)
    {
        m_vecCam.resize(_idx + 1);
    }

    m_vecCam[_idx] = _Cam;    
    return _idx;
}

void CRenderMgr::SetRenderFunc(bool _IsPlay)
{
    if(_IsPlay)
        RENDER_FUNC = &CRenderMgr::render_play;
    else
        RENDER_FUNC = &CRenderMgr::render_editor;
}

void CRenderMgr::CopyRenderTarget()
{
    Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
    CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::UpdateData()
{
    // ����ȭ������ ũ�Ⱑ ���ڶ�� �� ũ�� ���� �����.
    if (m_Light2DBuffer->GetElementCount() < m_vecLight2DInfo.size())
    {
        m_Light2DBuffer->Create(sizeof(tLightInfo), m_vecLight2DInfo.size(), SB_TYPE::READ_ONLY, true);
    }

    // ����ȭ���۷� ���� �����͸� �ű��.
    m_Light2DBuffer->SetData(m_vecLight2DInfo.data(), sizeof(tLightInfo) * m_vecLight2DInfo.size());
    m_Light2DBuffer->UpdateData(12, PIPELINE_STAGE::PS_PIXEL);

    if (m_Light3DBuffer->GetElementCount() < m_vecLight3DInfo.size())
    {
        m_Light3DBuffer->Create(sizeof(tLightInfo), m_vecLight3DInfo.size(), SB_TYPE::READ_ONLY, true);
    }

    // ����ȭ���۷� ���� �����͸� �ű��.
    m_Light3DBuffer->SetData(m_vecLight3DInfo.data(), sizeof(tLightInfo) * m_vecLight3DInfo.size());
    m_Light3DBuffer->UpdateData(13, PIPELINE_STAGE::PS_PIXEL);


    // GlobalData �� ���� �������� ����
    GlobalData.Light2DCount = m_vecLight2D.size();
    GlobalData.Light3DCount = m_vecLight3D.size();

    // ���� ��� ������ ���ε�
    CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
    pGlobalBuffer->SetData(&GlobalData, sizeof(tGlobal));
    pGlobalBuffer->UpdateData();
    pGlobalBuffer->UpdateData_CS();
}

void CRenderMgr::Clear()
{
    m_vecLight2D.clear();
    m_vecLight2DInfo.clear();
    m_vecLight3D.clear();
    m_vecLight3DInfo.clear();
}
