#pragma once
#include "CComponent.h"


class CState :
    public CComponent
{
public:
    CState();
	virtual ~CState();

public:
	CCollider3D();
	CCollider3D(const CCollider3D& _collider);
	CLONE(CCollider3D);
	virtual ~CCollider3D();

	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

};
