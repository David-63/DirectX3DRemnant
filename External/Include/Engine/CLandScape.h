#pragma once
#include "CRenderComponent.h"
#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CWeightMapShader.h"


enum class LANDSCAPE_MOD
{
    HEIGHT_MAP,
    SPLAT,
    NONE,
};

struct tWeight_4
{
    float arrWeight[4];
};

struct tWeight_8
{
    float arrWeight[8];
};

// 목표
// 1 코드에서 만드는게 아니라 Editor에서 세팅해주기
// 2 Save | Load 로 세팅되도록 하기? <- 이거 프리팹으로 하면 될듯


class CLandScape : public CRenderComponent
{
private:
    static int              m_makeCnt;
    UINT                    m_iFaceX;
    UINT                    m_iFaceZ;

    Vec2                    m_vBrushScale;
    Ptr<CTexture>           m_pBrushTex;

    Ptr<CRaycastShader>     m_pCSRaycast;
    CStructuredBuffer*      m_pCrossBuffer;

    Ptr<CHeightMapShader>   m_pCSHeightMap;
    Ptr<CTexture>           m_HeightMap;

    Ptr<CWeightMapShader>   m_pCSWeightMap;
    CStructuredBuffer*      m_pWeightMapBuffer;
    UINT                    m_iWeightWidth;
    UINT                    m_iWeightHeight;
    UINT                    m_iWeightIdx;

    LANDSCAPE_MOD           m_eMod;
    Ptr<CTexture>           m_pTileArrTex;

public:

public:
    void SetFace(UINT _iFaceX, UINT _iFaceZ);
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMap = _HeightMap; }

    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

private:
    void init();
    void CreateMesh();
    void CreateComputeShader();
    void CreateTexture();

    void Raycasting();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CLandScape);
public:
    CLandScape();
    ~CLandScape();
};

