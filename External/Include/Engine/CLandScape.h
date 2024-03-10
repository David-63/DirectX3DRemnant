#pragma once
#include "CRenderComponent.h"
#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CWeightMapShader.h"


#define DEFAULT_FACE  32  

enum class LANDSCAPE_MOD
{
    HEIGHT_MAP,
    SPLAT,
    NONE,
};

// 기본 Wdight4 사용
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
    tUINTS                  m_FaceSize;

    Vec2                    m_vBrushScale;
    Ptr<CTexture>           m_pBrushTex;

    Ptr<CRaycastShader>     m_pCSRaycast;
    CStructuredBuffer*      m_pCrossBuffer;

    Ptr<CHeightMapShader>   m_pCSHeightMap;
    Ptr<CTexture>           m_HeightMap;
    string                  m_heightMapName;

    Ptr<CWeightMapShader>   m_pCSWeightMap;
    CStructuredBuffer*      m_pWeightMapBuffer;
    UINT                    m_iWeightWidth;
    UINT                    m_iWeightHeight;
    UINT                    m_iWeightIdx;

    LANDSCAPE_MOD           m_eMod;
    Ptr<CTexture>           m_pTileArrTex;

public:

public:
    void SetFaceMesh(Ptr<CMesh> _mesh)
    {
        SetMesh(_mesh);
        SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LandScapeMtrl"), 0);
    }

    // face mesh create
    void SetFaceSize(UINT _iFaceX, UINT _iFaceZ) { m_FaceSize.set(_iFaceX, _iFaceZ); }
    void SetFaceSize(tUINTS _faceSize) { m_FaceSize = _faceSize; }
    tUINTS& GetFaceSize() { return m_FaceSize; }
        
    
    void CreateMesh();                                                      // 이건 코드상에서 생성할 때
    // 규칙:
    //  1 이름에 face 숫자 기입해주기
    //  2 이름에 F 붙여주기
    // ex) F_MainStage_128x128
    void MakeFaceMesh(string _strAnimName, UINT _iFaceX, UINT _iFaceZ);     // 파일로 생성할 때

public:
    void SetHeightMap(Ptr<CTexture> _pHeightMap) { m_HeightMap = _pHeightMap; }
    Ptr<CTexture> GetHeightMap() { return m_HeightMap; }
    
    void CreateHeightMap();

public:
    // tile arr tex
    void SetTileArrTex(Ptr<CTexture> _pTileArrTex) { m_pTileArrTex = _pTileArrTex; }
    Ptr<CTexture> GetTileArrTex() { return m_pTileArrTex; }

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

private:
    void init();
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

