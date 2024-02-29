#pragma once
#include "CEntity.h"
#include "CDevice.h"

class CStructuredBuffer : public CEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB;   // register binding
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    ComPtr<ID3D11Buffer>                m_SB_CPU_Read;  // GPU -> Sys
    ComPtr<ID3D11Buffer>                m_SB_CPU_Write; // Sys -> GPU

    D3D11_BUFFER_DESC                   m_tDesc;

    UINT                                m_iElementSize;
    UINT                                m_iElementCount;

    SB_TYPE                             m_Type;
    bool                                m_bSysAccess;

    UINT                                m_iRecentRegisterNum;

public:
    void Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _Type, bool _bUseSysAccess, void* _pSysMem = nullptr);
    void SetData(void* _pSrc, UINT _iSize = 0);
    void GetData(void* _pDst);

    // PIPELINE_STAGE
    void UpdateData(UINT _iRegisterNum, UINT _iPipeLineStage);
    void UpdateData_CS(UINT _iRegisterNum, bool _IsShaderRes);

    void Clear();
    void Clear_CS(bool _IsShaderRes);

    UINT GetElementSize() { return m_iElementSize; }
    UINT GetElementCount() { return m_iElementCount; }
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount;}

    template<typename T>
    void GetData(vector<T>& _vecDst);


    CLONE_DISABLE(CStructuredBuffer);
public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

template<typename T>
inline void CStructuredBuffer::GetData(vector<T>& _vecDst)
{
    // Main Buffer -> CPU ReadBuffer
    CONTEXT->CopyResource(m_SB_CPU_Read.Get(), m_SB.Get());

    // CPU ReadBuffer -> CPU
    // 버퍼의 크기를 확인합니다.
    D3D11_BUFFER_DESC desc;
    m_SB_CPU_Read->GetDesc(&desc);
    size_t bufferSize = desc.ByteWidth;
    // 새로운 메모리 영역을 할당합니다.
    T* bufferData = new T[bufferSize / sizeof(T)];

    // 버퍼의 데이터를 새로운 메모리 영역에 복사합니다.
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CONTEXT->Map(m_SB_CPU_Read.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
    memcpy(bufferData, mappedResource.pData, bufferSize);
    CONTEXT->Unmap(m_SB_CPU_Read.Get(), 0);

    // 새로운 메모리 영역에 있는 데이터를 vector에 복사합니다.
    _vecDst.assign(bufferData, bufferData + bufferSize / sizeof(T));

    // 새로운 메모리 영역을 해제합니다.
    delete[] bufferData;
}
