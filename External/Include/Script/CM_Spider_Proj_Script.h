#pragma once
#include <Engine\CScript.h>

class CM_Spider_Proj_Script : public CScript
{
private:
	CGameObject* mSpider;
	Vec3 mOffset;
	bool m_bShootStart;
	float m_fDurationTime;

public:
	CM_Spider_Proj_Script();
	~CM_Spider_Proj_Script();
	CLONE(CM_Spider_Proj_Script);

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetSpider(CGameObject* _obj) { mSpider = _obj; }
	void SetOffset(Vec3 _offset) { mOffset = _offset; }
	void ShootStart(bool is) { m_bShootStart = is; }

public:
	virtual void BeginOverlap(CCollider3D* _Other);
	virtual void OnOverlap(CCollider3D* _Other);
	virtual void EndOverlap(CCollider3D* _Other);
};

