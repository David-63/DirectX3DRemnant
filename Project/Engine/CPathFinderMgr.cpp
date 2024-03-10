#include "pch.h"
#include "CPathFinderMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRigidBody.h"
#include "CTransform.h"

CPathFinderMgr::CPathFinderMgr()
	: m_iRenewCount(0)
{

}
CPathFinderMgr::~CPathFinderMgr()
{

}

void CPathFinderMgr::tick()
{
	m_iRenewCount++;

	if (m_iRenewCount > 60)
	{
		m_iRenewCount = 0;
		MakeDynamicMap();
	}
}

void CPathFinderMgr::MakeStaticMap()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurLevel();
	std::vector<CGameObject*> objects = curLevel->GetLayer((UINT)LAYER_TYPE::Wall)->GetObjects();
	m_vStaticYX = {};

	for (auto& obj : objects)
	{
		Vec3 pos = obj->Transform()->GetRelativePos();
		Vec3 size = obj->RigidBody()->GetShapeSize();
		GEOMETRY_TYPE geomType = obj->RigidBody()->GetGeomType();

		
		if (geomType == GEOMETRY_TYPE::Box)
		{
			CalBoxYX(pos, size, &m_vStaticYX);
		}
		else if (geomType == GEOMETRY_TYPE::Sphere)
		{
			CalSphereYX(pos, size, &m_vStaticYX);
		}
	}

}

void CPathFinderMgr::MakeDynamicMap()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurLevel();
	std::vector<CGameObject*> player = curLevel->GetLayer((UINT)LAYER_TYPE::Player)->GetObjects();
	std::vector<CGameObject*> monsters = curLevel->GetLayer((UINT)LAYER_TYPE::Monster)->GetObjects();
	m_vDynamicYX = {};

	for (auto& obj : monsters)
	{
		Vec3 pos = obj->Transform()->GetRelativePos();
		Vec3 size = obj->RigidBody()->GetShapeSize();

		CalSphereYX(pos, size, &m_vDynamicYX);
	}

	for (auto& obj : player)
	{
		Vec3 pos = obj->Transform()->GetRelativePos();
		Vec3 size = obj->RigidBody()->GetShapeSize();

		CalSphereYX(pos, size, &m_vDynamicYX);
	}
}

void CPathFinderMgr::CalBoxYX(Vec3 _pos, Vec3 _size, vector<tYX>* _vector)
{
	int PadX;
	int PadY;
	if (_size.x <= m_fLength)
		PadX = 0;
	else
	{
		PadX = (_size.x / 2 - m_fLength / 2) / m_fLength + 1;
	}

	if (_size.z <= m_fLength)
		PadY = 0;
	else
	{
		PadY = (_size.z / 2 - m_fLength / 2) / m_fLength + 1;
	}

	tYX center = TransToYX(_pos);

	for (int i = center.y - PadY; i <= center.y + PadY; ++i)
	{
		for (int j = center.x - PadX; j <= center.x + PadX; ++j)
		{
			tYX pad(i, j);
			_vector->push_back(pad);
		}
	}
}

void CPathFinderMgr::CalSphereYX(Vec3 _pos, Vec3 _size, vector<tYX>* _vector)
{
	int PadR = 0;
	float rad = _size.x / 2;

	if (_size.x <= m_fLength)
		PadR = 0;
	else
		PadR = (rad - (m_fLength /2)) / m_fLength + 1;

	tYX center = TransToYX(_pos);

	for (int i = center.y - PadR; i <= center.y + PadR; ++i)
	{
		for (int j = center.x - PadR; j <= center.x + PadR; ++j)
		{
			tYX pad(i, j);
			_vector->push_back(pad);
		}
	}
}

tYX CPathFinderMgr::TransToYX(Vec3 _pos)
{
	int x;
	int y;

	if (_pos.x >= 0)
		x = _pos.x / m_fLength + m_iXCount / 2 + 1;
	else
		x = m_iXCount / 2 + _pos.x / m_fLength + 1;

	if (_pos.z >= 0)
		y = _pos.z / m_fLength + m_iYCount / 2 + 1;
	else
		y = m_iYCount / 2 + _pos.z / m_fLength + 1;

	tYX coordinate(y, x);

	return coordinate;
}


