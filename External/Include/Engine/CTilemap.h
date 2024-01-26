#pragma once

#include "CRenderComponent.h"

class CStructuredBuffer;

class CTileMap :
    public CRenderComponent
{
private:
    UINT                m_iTileCountX;  // Ÿ�� ����
    UINT                m_iTileCountY;  // Ÿ�� ����
    Vec2                m_vSliceSize;   // Ÿ�� �ϳ��� ũ��(UV ����)
    vector<tTile>       m_vecTile;
    CStructuredBuffer*  m_Buffer;

public:
    virtual void finaltick() override;
    virtual void render() override;

    void UpdateData();

    void SetTileCount(UINT _iXCount, UINT _iYCount);
    void SetSliceSize(Vec2 _vSliceSize)  { m_vSliceSize = _vSliceSize; }


    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    CLONE(CTileMap);
public:
    CTileMap();
    ~CTileMap();
};

