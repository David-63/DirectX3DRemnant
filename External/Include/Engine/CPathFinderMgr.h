#pragma once
#include "CSingleton.h"
#include <functional>
#include <map>
#include <queue>
#include <unordered_map>

using namespace std;

class CPathFinderMgr 
	: public CSingleton<CPathFinderMgr>
{
	SINGLE(CPathFinderMgr);
private:
	const float m_fLength = 20.f;
	const int m_iXCount = 500;
	const int m_iYCount = 500;

	int m_iRenewCount;
	
	vector<tRangeYX> m_vStaticYX = {};
	vector<tRangeYX> m_vDynamicYX = {};

public:
	int GetXCount() { return m_iXCount; }
	int GetYCount() { return m_iYCount; }
	float GetLength() { return m_fLength; }
	

public:
	void tick();

public:
	void MakeStaticMap();
	vector<tRangeYX> GetStaticMap() { return m_vStaticYX; }
	vector<tRangeYX> GetDynamicMap() { return m_vDynamicYX; }

private:
	void MakeDynamicMap();

	void CalBoxYX(Vec3 _pos, Vec3 _size, vector<tRangeYX>* _vector);
	void CalSphereYX(Vec3 _pos, Vec3 _size, vector<tRangeYX>* _vector);

	tYX TransToYX(Vec3 _pos);
};

