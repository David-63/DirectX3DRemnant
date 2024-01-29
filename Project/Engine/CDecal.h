#pragma once
#include "CRenderComponent.h"

class CDecal :
    public CRenderComponent
{
private:
    Ptr<CTexture>   m_DecalTex;

    bool            m_bDeferred;
    bool            m_bEmissive;
    bool            m_bShow;

public:
    // ���������� ������ �ٶ� Mtrl ����
    void SetDeferredDecal(bool _bDeferred);
    bool IsDeferredDecal() { return m_bDeferred; }

    // ��Į�� �������� ����Ұ����� �ƴ���
    void ActivateEmissive(bool _bActivate) { m_bEmissive = _bActivate; }
    bool isEmissive() { return m_bEmissive; }
    bool IsDebugRendering() { return m_bShow; }
    void SetDebugRendering(bool _bShow) { m_bShow = _bShow; };
public:
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CDecal);
public:
    CDecal();
    ~CDecal();
};

