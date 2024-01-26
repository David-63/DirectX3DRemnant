#pragma once
#include "CComponent.h"
#include "CFrustum.h"

class CCamera : public CComponent
{
private:
    CFrustum    m_Frustum;

    float       m_fAspectRatio;
    float       m_fScale;           // Orthograpic ���� ����ϴ� ī�޶� ����
    float       m_Far;
    float       m_Fov;

    float       m_OrthoWidth;
    float       m_OrthoHeight;

    PROJ_TYPE   m_ProjType;

    Matrix      m_matView;
    Matrix      m_matViewInv;

    Matrix      m_matProj;
    Matrix      m_matProjInv;

    UINT        m_iLayerMask;

    int         m_iCamIdx;          // ī�޶� �켱����

    bool        m_isDeferredCamera = true;

    vector<CGameObject*>    m_vecDeferred;
    vector<CGameObject*>    m_vecDeferredDecal;

    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMask;
    vector<CGameObject*>    m_vecDecal;
    vector<CGameObject*>    m_vecTransparent;    
    vector<CGameObject*>    m_vecUI;
    vector<CGameObject*>    m_vecPost;

    vector<CGameObject*>    m_vecShadow;


public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }

    void SetScale(float _fScale) { m_fScale = _fScale; }
    float GetScale() { return m_fScale; }

    void SetFar(float _Far) { m_Far = _Far; }
    float GetFar() { return m_Far; }

    void SetLayerMask(int _iLayer, bool _Visible);
    void SetLayerMaskAll(bool _Visible);

    void SetCameraIndex(int _idx);

    // ���Ŀ� degree ��ȯ ��� �߰��ϱ�
    void SetFov(float _Radian) { m_Fov = _Radian; }
    float GetFov() { return m_Fov; }

    void SetOrthoWidth(float _width) { m_OrthoWidth = _width; }
    void SetOrthoHeight(float _height) { m_OrthoHeight = _height; }

    float GetorthoWidth() { return m_OrthoWidth; }
    float GetOrthoHeight() { return m_OrthoHeight; }



    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

    const Matrix& GetViewInvMat() { return m_matViewInv; }
    const Matrix& GetProjInvMat() { return m_matProjInv; }

    bool IsDeferredCamera() const { return m_isDeferredCamera; }
    void TurnDeferredCamera(bool _power) { m_isDeferredCamera = _power; }
public:
    void SortObject();
    void SortObject_Shadow();
    void render();
    void render_shadowmap();

    void updateMatrix();

public:
    virtual void begin() override;
    virtual void finaltick() override;


private:
    void clear();
    void clear_shadow();
    void render_deferred();

    void geometryRender();
    void lightRender();
    void mergeRender();

    void render_opaque();
    void render_mask();
    void render_decal();
    void render_transparent();
    void render_postprocess();
    void render_ui();


    void CalcViewMat();
    void CalcProjMat();

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCamera);
public:    
    CCamera();
    CCamera(const CCamera& _Other);
    ~CCamera();
};

