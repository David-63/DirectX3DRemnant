#pragma once
#include "CComponent.h"
#include "CFrustum.h"

class CCamera : public CComponent
{
private:
    //CFrustum                m_Frustum;
    tRay                    m_ray;              // 마우스 방향을 향하는 직선
    int                     m_iCamIdx;          // 카메라 우선순위
    PROJ_TYPE               m_ProjType;
    UINT                    m_iLayerMask;
    float                   m_fAspectRatio;
    float                   m_fScale;           // Orthograpic 에서 사용하는 카메라 배율
    float                   m_Far;
    float                   m_Fov;
    float                   m_OrthoWidth;
    float                   m_OrthoHeight;
    bool                    m_uiOnly;

    Matrix                  m_matView;
    Matrix                  m_matViewInv;
    Matrix                  m_matProj;
    Matrix                  m_matProjInv;

    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_S;	    // Foward ( Opaque, Mask )	
    map<INT_PTR, vector<tInstObj>>		m_mapSingleObj_S;		    // Single Object

    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_D;	    // Deferred
    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_F;	    // Foward ( Opaque, Mask )	
    map<INT_PTR, vector<tInstObj>>		m_mapSingleObj;		    // Single Object


    vector<CGameObject*>                m_vecDecal_D;
    vector<CGameObject*>                m_vecDecal;
    vector<CGameObject*>                m_vecParticle_D;



    vector<CGameObject*>                m_vecTransparent;    
    vector<CGameObject*>                m_vecUI;
    vector<CGameObject*>                m_vecPost;

    vector<CGameObject*>                m_vecShadow;
public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }
    void SetUIOnly(bool _able = true) { m_uiOnly = _able; }

    void SetScale(float _fScale) { m_fScale = _fScale; }
    float GetScale() { return m_fScale; }

    void SetFar(float _Far) { m_Far = _Far; }
    float GetFar() { return m_Far; }

    void SetLayerMask(int _iLayer, bool _Visible);
    void SetLayerMaskAll(bool _Visible);

    void SetCameraIndex(int _idx);

    const tRay& GetRay() { return m_ray; }
    // 추후에 degree 변환 기능 추가하기
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

public:
    void SortObject();
    void SortObject_Shadow();
    void render();
    void render_shadowmap();


public:
    virtual void begin() override;
    virtual void finaltick() override;
    void updateMatrix();

protected:
    void CalRay();  // 마우스 방향으로 광선 연산

private:
    void clear();
    void clear_shadow();

    void geometryRender();  // 함수 크기 줄이려고 만든것
    void lightRender();     // 
    void mergeRender();     // 

    void render_deferred();
    void render_deferred_Decal();
    void render_particle();

    void render_forward();
    void render_forward_Decal();

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

