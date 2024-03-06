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

// �⺻ Wdight4 ���
struct tWeight_4
{
    float arrWeight[4];
};

struct tWeight_8
{
    float arrWeight[8];
};

// ��ǥ
// 1 �ڵ忡�� ����°� �ƴ϶� Editor���� �������ֱ�
// 2 Save | Load �� ���õǵ��� �ϱ�? <- �̰� ���������� �ϸ� �ɵ�


class CLandScape : public CRenderComponent
{
private:
    static int              m_makeCnt;
    tUINTS                  m_FaceSize;
    Ptr<CMesh>              m_pFaceMesh;
    string                  m_meshName;


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
    // face mesh create
    void SetFaceSize(UINT _iFaceX, UINT _iFaceZ) { m_FaceSize.set(_iFaceX, _iFaceZ); }
    void SetFaceSize(tUINTS _faceSize) { m_FaceSize = _faceSize; }
    tUINTS& GetFaceSize() { return m_FaceSize; }
    void SetFaceName(string _name = "") { m_meshName = _name; }
    string& GetFaceName() { return m_meshName; }
    
    void CreateMesh();

    // height map tex
    void SaveHeightMap();
    void SetHeightMap(Ptr<CTexture> _pHeightMap) { m_HeightMap = _pHeightMap; }
    Ptr<CTexture> GetHeightMap() { return m_HeightMap; }

    // tile arr tex
    void SetTileArrTex(Ptr<CTexture> _pTileArrTex) { m_pTileArrTex = _pTileArrTex; }
    Ptr<CTexture> GetTileArrTex() { return m_pTileArrTex; }


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

