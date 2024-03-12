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
	enum class eBlock
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};

private:
	int m_iCurPosX;
	int m_iCurPosY;
	int m_iDestPosX;
	int m_iDestPosY;

	int m_iXCount;
	int m_iYCount;

	int m_Block; // �����¿� ex���� 1010, �� 1

	float m_fLength; //��ĭ�� ����
	float m_fDiagLength; //�밢�� ����

	float m_fSelfPad;
	int m_iSelfPad;
	float m_fDstCirclePad;

	bool m_bPathReady;

	// Open List
	priority_queue<tPNode*, vector<tPNode*>, ComparePathLength>	m_OpenList;

	std::unordered_map<UINT, tPNode*> m_ArrNode;
	std::stack<Vec3> m_Stack;

	//from Mgr
	vector<tRangeYX> m_vStaticMap;
	vector<tRangeYX> m_vDynamicMap;

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
	//��ã�� �ٽɷ���
	void Rebuild(priority_queue<tPNode*, vector<tPNode*>, ComparePathLength>& _queue);
	void EraseInOpenList(priority_queue<tPNode*, vector<tPNode*>, ComparePathLength>& _queue, int _iYIdx, int _iXIdx);
	void CalculateCost(tPNode* _pCurNode, tPNode* _pOrigin, bool _bDiagonal = false);
	void AddOpenList(int _iXIdx, int _iYIdx, tPNode* _pOrigin, eBlock _dir, bool _bDiagonal = false);
	void FindPath();

	//��ǥ��ȯ
	void SetDstYX(Vec3 _DstPos);
	tYX TransToYX(Vec3 _pos);
	Vec3 TransYXToPos(tYX _coord);
	void SetCurYX();
	float CalCirclePad(CGameObject* _pObject);

	//map ���� �׽�Ʈ
	bool MapTest(int _y, int _x);

};

