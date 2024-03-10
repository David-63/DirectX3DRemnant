#include "pch.h"
#include "CPathFinderScript.h"
#include "Engine/CPathFinderMgr.h"
#include "Engine/CTransform.h"



CPathFinderScript::CPathFinderScript()
	:CScript((UINT)SCRIPT_TYPE::PATHFINDERSCRIPT)
	, m_iCurPosX(0)
	, m_iCurPosY(0)
	, m_iDestPosX(0)
	, m_iDestPosY(0)
{
	m_iXCount = CPathFinderMgr::GetInst()->GetXCount();
	m_iYCount = CPathFinderMgr::GetInst()->GetYCount();
	m_fLength = CPathFinderMgr::GetInst()->GetLength();
	m_fDiagLength = m_fLength * sqrtf(2.f);
	
	m_ArrNode = {};
	m_OpenList = {};
}

CPathFinderScript::~CPathFinderScript()
{
	Clear();

}

void CPathFinderScript::begin()
{
	m_vStaticMap = CPathFinderMgr::GetInst()->GetStaticMap();
}

void CPathFinderScript::tick()
{
	
}


void CPathFinderScript::Clear()
{
	if (!m_ArrNode.empty())
	{
		for (auto iter = m_ArrNode.begin(); iter != m_ArrNode.end(); )
		{
			delete iter->second;
			iter->second = nullptr;
			m_ArrNode.erase(iter++);
		}

		m_ArrNode.clear();
	}
	m_OpenList = priority_queue<tPNode*, vector<tPNode*>, ComparePathLength>();
	m_Stack = {};

	m_fDstCirclePad = 0.f;

	m_iCurPosX = 0;
	m_iCurPosY = 0;
	m_iDestPosX = 0;
	m_iDestPosY = 0;

	m_bPathReady = false;
}

tYX CPathFinderScript::TransToYX(Vec3 _pos)
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



float CPathFinderScript::SetDestObject(CGameObject* _pObject)
{
	Clear();

	SetDstYX(_pObject->Transform()->GetRelativePos());
	SetCurYX();

	ApplyStaticMap();
	//m_vvDynamicMap = CPathFinderMgr::GetInst()->GetDynamicMap();
	//ApplyDynamicMap();

	FindPath();

	m_fDstCirclePad = CalCirclePad(_pObject);
	m_fSelfPad = CalCirclePad(GetOwner());

	return m_fSelfPad + m_fDstCirclePad;
}



Vec3 CPathFinderScript::TransYXToPos(tYX _coord)
{
	Vec3 pos = {};

	if (_coord.x > 250)
		pos.x = (_coord.x - 250)* m_fLength - m_fLength / 2.f;
	else
		pos.x = -(251 - _coord.x)* m_fLength + m_fLength / 2.f;
	
	if (_coord.y > 250)
		pos.z = (_coord.y - 250) * m_fLength - m_fLength / 2.f;
	else
		pos.z = -(251 - _coord.y) * m_fLength + m_fLength / 2.f;
	
	return pos;
}

float CPathFinderScript::CalCirclePad(CGameObject* _pObject)
{
	if (_pObject->RigidBody())
	{
		float rad = _pObject->RigidBody()->GetRad();
		
		return rad;
	}
	
	return 0.f;
}

void CPathFinderScript::SetCurYX()
{
	Vec3 curPos = GetOwner()->Transform()->GetRelativePos();
	tYX yx = TransToYX(curPos);

	m_iCurPosX = yx.x;
	m_iCurPosY = yx.y;
}

void CPathFinderScript::SetDstYX(Vec3 _DstPos)
{
	tYX yx = TransToYX(_DstPos);

	m_iDestPosX = yx.x;
	m_iDestPosY = yx.y;
}

void CPathFinderScript::ApplyStaticMap()
{
	for (tYX yx : m_vStaticMap)
	{
		tPNode* node = new tPNode;
		node->iIdxX = yx.x;
		node->iIdxY = yx.y;
		node->bMove = false;

		m_ArrNode[make_pair(yx.y, yx.x)] = node;
	}
}

void CPathFinderScript::ApplyDynamicMap()
{
	for (tYX yx : m_vDynamicMap)
	{
		tPNode* node = new tPNode;
		node->iIdxX = yx.x;
		node->iIdxY = yx.y;
		node->bMove = false;

		m_ArrNode[make_pair(yx.y, yx.x)] = node;
	}
}







void CPathFinderScript::Rebuild(priority_queue<tPNode*, vector<tPNode*>, ComparePathLength>& _queue)
{
	priority_queue<tPNode*, vector<tPNode*>, ComparePathLength> tempQueue;

	while (!_queue.empty())
	{
		tempQueue.push(_queue.top());
		_queue.pop();
	}

	_queue.swap(tempQueue);
}


void CPathFinderScript::FindPath()
{
	int iCurX = m_iCurPosX;
	int iCurY = m_iCurPosY;

	tPNode* pCurNode = new tPNode;
	pCurNode->iIdxX = m_iCurPosX;
	pCurNode->iIdxY = m_iCurPosY;
	pCurNode->bClosed = true;
	pCurNode->fFromParent = 0;
	m_ArrNode[{m_iCurPosY, m_iCurPosX}] = pCurNode;

	while (true)
	{
		// ���� �������� 4������ ��带 OpenList �� �ִ´�.
		// UP
		AddOpenList(pCurNode->iIdxX
			, pCurNode->iIdxY - 1, pCurNode);

		// RIGHT		
		AddOpenList(pCurNode->iIdxX + 1
			, pCurNode->iIdxY, pCurNode);

		// DOWN		
		AddOpenList(pCurNode->iIdxX
			, pCurNode->iIdxY + 1, pCurNode);

		// LEFT		
		AddOpenList(pCurNode->iIdxX - 1
			, pCurNode->iIdxY, pCurNode);

		//UPLEFT
		AddOpenList(pCurNode->iIdxX - 1
			, pCurNode->iIdxY - 1, pCurNode, true);

		//UPRIGHT
		AddOpenList(pCurNode->iIdxX + 1
			, pCurNode->iIdxY - 1, pCurNode, true);

		//DOWNLEFT
		AddOpenList(pCurNode->iIdxX - 1
			, pCurNode->iIdxY + 1, pCurNode, true);

		//DOWNRIGHT
		AddOpenList(pCurNode->iIdxX + 1
			, pCurNode->iIdxY + 1, pCurNode, true);

		// 2. Open List ���� ���� ȿ���� ���� ��带 ������.
		//  - �ش� ���� ClostList �� �ִ´�.
		pCurNode = m_OpenList.top();
		pCurNode->bClosed = true;

		m_OpenList.pop();

		// �������� ������ ��� Astar ����
		if (pCurNode->iIdxX == m_iDestPosX && pCurNode->iIdxY == m_iDestPosY)
		{
			break;
		}
	}

	// �� ����
	while (true)
	{
		Vec3 nodePos = TransYXToPos(tYX(pCurNode->iIdxY, pCurNode->iIdxX));
		m_Stack.push(nodePos);
		pCurNode = pCurNode->pPrevNode;

		if (pCurNode->iIdxX == m_iCurPosX && pCurNode->iIdxY == m_iCurPosY)
		{
			Vec3 nodePos = TransYXToPos(tYX(m_iCurPosY, m_iCurPosX));
			m_Stack.push(nodePos);
			m_bPathReady = true;
			break;
		}
	}
}


void CPathFinderScript::CalculateCost(tPNode* _pCurNode, tPNode* _pOrigin, bool _bDiagonal)
{
	_pCurNode->pPrevNode = _pOrigin;

	if(!_bDiagonal)
		_pCurNode->fFromParent = m_fLength + _pOrigin->fFromParent;
	else
		_pCurNode->fFromParent = m_fDiagLength + _pOrigin->fFromParent;

	_pCurNode->fToDest = abs(m_iDestPosX - _pCurNode->iIdxX) * m_fLength + abs(m_iDestPosY - _pCurNode->iIdxY) * m_fLength;
	_pCurNode->fFinal = _pCurNode->fFromParent + _pCurNode->fToDest;
}

void CPathFinderScript::AddOpenList(int _iXIdx, int _iYIdx, tPNode* _pOrigin, bool _bDiagonal)
{
	// ���� �������� �� �� �ִ� ���� OpenList �� �ִ´�.
	// ��� ������ ��� ���
	if (_iXIdx < 0 || _iXIdx >= m_iXCount || _iYIdx < 0 || _iYIdx >= m_iYCount)
		return;

	//�ش� Ű���� �����鼭 
	auto iter = m_ArrNode.find(make_pair(_iYIdx, _iXIdx));
	if (iter != m_ArrNode.end())
	{
		//�̵��Ұ��϶�
		if (!iter->second->bMove)
			return;

		// �ش� ���� Closed List �� �ִ� ���, Open List �� ���� �ʴ´�.
		if (iter->second->bClosed)
			return;
	} 

	if (iter == m_ArrNode.end())
	{
		//��� ����
		tPNode* pCurNode = new tPNode;
		pCurNode->iIdxX = _iXIdx;
		pCurNode->iIdxY = _iYIdx;
		m_ArrNode[make_pair(_iYIdx, _iXIdx)] = pCurNode;
	}

	// Open List �� ����� ����ؼ� �ִ´�.
	if (false == m_ArrNode[make_pair(_iYIdx, _iXIdx)]->bOpen)
	{
		CalculateCost(m_ArrNode[make_pair(_iYIdx, _iXIdx)], _pOrigin, _bDiagonal);
		// Open List �� �ִ´�.
		m_ArrNode[make_pair(_iYIdx, _iXIdx)]->bOpen = true;
		m_OpenList.push(m_ArrNode[make_pair(_iYIdx, _iXIdx)]);
	}
	else // �̹� OpenList �� �ִ� ���,
	{
		//����� ����ؼ� �� ȿ���� ���� ������ ��ü�Ѵ�.
		tPNode* pNodePtr = m_ArrNode[make_pair(_iYIdx, _iXIdx)];
		tPNode* copy = new tPNode(*pNodePtr);

		CalculateCost(copy, _pOrigin, _bDiagonal); //ī���� �ڽ�Ʈ�� ���

		if (m_ArrNode[make_pair(_iYIdx, _iXIdx)]->fFinal > copy->fFinal)
		{
			delete m_ArrNode[make_pair(_iYIdx, _iXIdx)];
			m_ArrNode[make_pair(_iYIdx, _iXIdx)] = nullptr;
			m_ArrNode.erase(make_pair(_iYIdx, _iXIdx));

			m_ArrNode[make_pair(_iYIdx, _iXIdx)] = copy;

			// ���¸���Ʈ(�켱����ť) �缳��
			Rebuild(m_OpenList);
		}
		else
		{
			delete copy;
			copy = nullptr;
		}
	}
}
