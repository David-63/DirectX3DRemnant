#pragma once
#include <queue>
#include <Engine\CScript.h>
#include <unordered_map>
#include <stack>


using namespace std;



class ComparePathLength
{
public:
	bool operator() (tPNode* _pLeft, tPNode* _pRight)
	{
		return _pLeft->fFinal > _pRight->fFinal;
	}
};

class CPathFinderScript
	: public CScript
{
private:
	int m_iCurPosX;
	int m_iCurPosY;
	int m_iDestPosX;
	int m_iDestPosY;

	int m_iXCount;
	int m_iYCount;

	float m_fLength; //한칸의 길이
	float m_fDiagLength; //대각선 길이

	float m_fSelfPad;
	float m_fDstCirclePad;

	bool m_bPathReady;

	// Open List
	priority_queue<tPNode*, vector<tPNode*>, ComparePathLength>	m_OpenList;

	std::unordered_map<UINT, tPNode*> m_ArrNode;
	std::stack<Vec3> m_Stack;

	//from Mgr
	vector<tYX> m_vStaticMap;
	vector<tYX> m_vDynamicMap;

public:
	CPathFinderScript();
	~CPathFinderScript();

	CLONE(CPathFinderScript);

public:
	void tick() override;
	void begin() override;

public:
	float SetDestObject(CGameObject* _pObject);
	std::stack<Vec3>* GetPathStack() { return &m_Stack; }
	void Clear();

private:
	//길찾기 핵심로직
	void Rebuild(priority_queue<tPNode*, vector<tPNode*>, ComparePathLength>& _queue);
	void CalculateCost(tPNode* _pCurNode, tPNode* _pOrigin, bool _bDiagonal = false);
	void AddOpenList(int _iXIdx, int _iYIdx, tPNode* _pOrigin, bool _bDiagonal = false);
	void FindPath();

	//좌표변환
	void SetDstYX(Vec3 _DstPos);
	tYX TransToYX(Vec3 _pos);
	Vec3 TransYXToPos(tYX _coord);
	void SetCurYX();
	float CalCirclePad(CGameObject* _pObject);

	//map 정보 받기
	void ApplyStaticMap();
	void ApplyDynamicMap();

};

