#pragma once
#include "CEntity.h"


#include "CTexture.h"
#include "ptr.h"


// Multi Render Target
class CMRT :
    public CEntity
{
private:
    Ptr<CTexture>   m_arrRT[8];
    UINT            m_RTCount;
    Ptr<CTexture>   m_DSTex;

    Vec4            m_ClearColor[8];

    D3D11_VIEWPORT  m_Viewport;

public:
    void Create(Ptr<CTexture>* _arrRTTex, UINT _RTCount, Ptr<CTexture> _DSTex);
    void SetClearColor(Vec4 _clearColor, UINT _RTIdx) { m_ClearColor[_RTIdx] = _clearColor; }
    void ClearTarget();
    void OMSet(bool _isStay = false);

public:
    CLONE_DISABLE(CMRT);

public:
    CMRT();
    ~CMRT();
};

