#pragma once
#include <Engine\CScript.h>

class CM_Spider_Proj_Script : public CScript
{
public:
	CM_Spider_Proj_Script();
	~CM_Spider_Proj_Script();
	CLONE(CM_Spider_Proj_Script);

public:
	virtual void begin() override;
	virtual void tick() override;


public:
	virtual void BeginOverlap(CCollider3D* _Other);
	virtual void OnOverlap(CCollider3D* _Other);
	virtual void EndOverlap(CCollider3D* _Other);
};

