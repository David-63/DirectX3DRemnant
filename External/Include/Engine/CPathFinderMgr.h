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
	
	std::unordered_map<pair<int, int>, tPNode*, tPairHash> m_GameMap;

public:
	int GetXCount() { return m_iXCount; }
	int GetYCount() { return m_iYCount; }
	float GetLength() { return m_fLength; }
	

public:
	void tick();

public:
	

	
};

