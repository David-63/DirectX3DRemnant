#pragma once
#include "CEntity.h"

enum FACE_TYPE
{
	FT_NEAR,
	FT_FAR,
	FT_UP,
	FT_DOWN,
	FT_LEFT,
	FT_RIGHT,
	FT_END,
};

class CCamera;
class CFrustum : public CEntity
{
private:
	CCamera* m_pCam;

	Vec3 m_arrProj[8];
	Vec4 m_arrFace[FT_END];

	Vec4 m_arrFace_Near[FT_END];
	Vec4 m_arrFace_Middle[FT_END];
	Vec4 m_arrFace_Far[FT_END];


public:
	void finaltick();

public:
	bool FrustumCheck(Vec3 _vPos);
	bool FrustumCheckBound(Vec3 _vPos, float _fRadius);


public:
	void SetCamera(CCamera* _cam) { m_pCam = _cam; }

public:
	CLONE(CFrustum);
	CFrustum();
	CFrustum(const CFrustum& _other);
	CFrustum(CCamera* _camera);
	~CFrustum();
};

