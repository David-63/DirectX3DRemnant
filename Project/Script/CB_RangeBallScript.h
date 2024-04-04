#pragma once
#include <Engine\CScript.h>

class CB_RangeBallScript :
    public CScript
{
private:
	CGameObject* m_RangeBall;
	CGameObject* m_Player;
	Vec3 m_vInitialPos;
	Vec3 m_vCurPos;
	Vec3 m_vPlayerPos;
	Vec3 RangeBalToPlayerDir;
	Vec3 mOffset;
	Vec3 m_vdir;

	float m_fCoolTime;
	bool m_bFire;

public:
	CB_RangeBallScript();
	~CB_RangeBallScript();
	CLONE(CB_RangeBallScript);

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetRangeBall(CGameObject* _obj) { m_RangeBall = _obj; }
	void SetPlayer(CGameObject* _player) { m_Player = _player; }
	void SetOffset(Vec3 _offset) { mOffset = _offset; }
	void SetFire(bool _fire) { m_bFire = _fire; }

	void initialPos(Vec3 _pos) { m_vInitialPos = _pos; }
	void StorePlayerPos(Vec3 _pos) { m_vPlayerPos = _pos; }




};

