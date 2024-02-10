#pragma once
#include "CSingleton.h"
#include <array>


class CLayer;
class CCollider2D;


union CollisionID
{
	struct
	{
		UINT LeftID;
		UINT RightID;
	};

	UINT_PTR id;
};


class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	std::array<UINT, MAX_LAYER>		m_matrix;
	map<UINT_PTR, bool>				m_mapColID;
	

public:
	void SetColLayer(UINT _left, UINT _right);
	void SetColLayer(const wstring& _strLeftLayer, const wstring& _strRightLayer);

	void Clear()
	{
		memset(&m_matrix, 0, sizeof(UINT) * MAX_LAYER);
	}

	std::array<UINT, MAX_LAYER> GetMat() { return m_matrix; }

public:
	void tick();

private:
	//void CollisionBtwLayer(CLayer* _LeftLayer, CLayer* _RightLayer);
	//void CollisionBtwObject(CGameObject* _LeftObject, CGameObject* _RightObject);
	//bool CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight);
};

