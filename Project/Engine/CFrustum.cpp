#include "pch.h"
#include "CFrustum.h"
#include "CCamera.h"

CFrustum::CFrustum(CCamera* _camera)
    : m_pCam(_camera), m_arrProj{}, m_arrFace{}
{
    //    4 --- 5 
    //   /|    /|  <-- Far
    //  / |   / |
    // 0 -- 1 - 6
    // |/   |  /   <-- Near
    // 3 -- 2 /
    m_arrProj[0] = Vec3(-1.f, 1.f, 0.f);
    m_arrProj[1] = Vec3(1.f, 1.f, 0.f);
    m_arrProj[2] = Vec3(1.f, -1.f, 0.f);
    m_arrProj[3] = Vec3(-1.f, -1.f, 0.f);

    m_arrProj[4] = Vec3(-1.f, 1.f, 1.f);
    m_arrProj[5] = Vec3(1.f, 1.f, 1.f);
    m_arrProj[6] = Vec3(1.f, -1.f, 1.f);
    m_arrProj[7] = Vec3(-1.f, -1.f, 1.f);
}

CFrustum::~CFrustum()
{
}

void CFrustum::finaltick()
{
    // ���� ������ ndc ��ǥ�� ������ ���� ������ ����
    // ī�޶� �þ߹����� ���� �������� 8���� �𼭸��� ���´�

    const Matrix& matProInv = m_pCam->GetProjInvMat();
    const Matrix& matViewInv = m_pCam->GetViewInvMat();
    Matrix matPVInv = matProInv * matViewInv;

    Vec3 arrWorld[8] = {};
    for (size_t idx = 0; idx < 8; idx++)
    {
        arrWorld[idx] = XMVector3TransformCoord(m_arrProj[idx], matPVInv);
    }

    // ��� ��ǥ 3���� �����Ͽ� ��ü�� ��ȿ���� üũ�ϱ� ������ ������ ����� ��ǥ ������ �߿���
    m_arrFace[FT_NEAR] = XMPlaneFromPoints(arrWorld[0], arrWorld[1], arrWorld[2]);
    m_arrFace[FT_FAR] = XMPlaneFromPoints(arrWorld[5], arrWorld[4], arrWorld[7]);
    m_arrFace[FT_UP] = XMPlaneFromPoints(arrWorld[4], arrWorld[5], arrWorld[1]);
    m_arrFace[FT_DOWN] = XMPlaneFromPoints(arrWorld[3], arrWorld[2], arrWorld[6]);
    m_arrFace[FT_LEFT] = XMPlaneFromPoints(arrWorld[7], arrWorld[4], arrWorld[0]);
    m_arrFace[FT_RIGHT] = XMPlaneFromPoints(arrWorld[1], arrWorld[5], arrWorld[6]);
}

bool CFrustum::FrustumCheck(Vec3 _vPos)
{
    for (size_t idx = 0; idx < FT_END; idx++)
    {
        Vec3 vNormal = m_arrFace[idx];
        // -1 ~ 1
        if (vNormal.Dot(_vPos) + m_arrFace[idx].w > 0)
            return false;
    }

    return true;
}

bool CFrustum::FrustumCheckBound(Vec3 _vPos, float _fRadius)
{
    for (size_t idx = 0; idx < FT_END; idx++)
    {
        Vec3 vNormal = m_arrFace[idx];
        float result = vNormal.Dot(_vPos);
        if (result + m_arrFace[idx].w > _fRadius)
            return false;
    }

    return true;
}
