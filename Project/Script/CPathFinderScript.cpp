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
	, m_Block(0)
{
	m_iXCount = CPathFinderMgr::GetInst()->GetXCount();
	m_iYCount = CPathFinderMgr::GetInst()->GetYCount();
	m_fLength = CPathFinderMgr::GetInst()->GetLength();
	m_fDiagLength = m_fLength * sqrtf(2.f);
	
	m_ArrNode = {};
	//m_ArrNode.rehash(25000);
	m_OpenList = {};
}

CPathFinderScript::~CPathFinderScript()
{
	Clear();

}

void CPathFinderScript::begin()
{
	m_vStaticMap = CPathFinderMgr::GetInst()->GetStaticMap();
	m_fSelfPad = CalCirclePad(GetOwner()) / m_fLength;
	if (m_fSelfPad < 1)
		m_iSelfPad = 0;
	else
		m_iSelfPad = ceil(m_fSelfPad);

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
			//m_ArrNode.rehash(25000);
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

void CPathFinderScript::SetDestPos(Vec3 _pos)
{
	Clear();

	SetDstYX(_pos);
	SetCurYX();

	m_vDynamicMap = CPathFinderMgr::GetInst()->GetDynamicMap();

	FindPath();
}



float CPathFinderScript::SetDestObject(CGameObject* _pObject)
{
	Clear();

	SetDstYX(_pObject->Transform()->GetRelativePos());
	SetCurYX();

	m_vDynamicMap = CPathFinderMgr::GetInst()->GetDynamicMap();

	m_fDstCirclePad = CalCirclePad(_pObject);
	
	FindPath();

	return m_iSelfPad + m_fDstCirclePad;
}



Vec3 CPathFinderScript::TransYXToPos(tYX _coord)
{
	Vec3 pos = {};

	if (_coord.x > m_iXCount / 2)
		pos.x = (_coord.x - m_iXCount / 2)* m_fLength - m_fLength / 2.f;
	else
		pos.x = -(m_iXCount / 2 + 1 - _coord.x)* m_fLength + m_fLength / 2.f;
	
	if (_coord.y > m_iYCount / 2)
		pos.z = (_coord.y - m_iYCount / 2) * m_fLength - m_fLength / 2.f;
	else
		pos.z = -(m_iYCount / 2 + 1 - _coord.y) * m_fLength + m_fLength / 2.f;
	
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

bool CPathFinderScript::MapTest(int _y, int _x)
{
	if (_x == m_iDestPosX && _y == m_iDestPosY)
		return true;

	for (tRangeYX range : m_vStaticMap)
	{
		if (range.x1 - m_iSelfPad <= _x && _x <= range.x2 + m_iSelfPad
			&& range.y1 - m_iSelfPad <= _y && _y <= range.y2 + m_iSelfPad)
			return false;
	}

	for (tRangeYX range : m_vDynamicMap)
	{
		if (m_iCurPosX - m_iSelfPad == range.x1
			&& m_iCurPosY - m_iSelfPad == range.y1)
			continue;

		if (range.x1 - m_iSelfPad <= _x && _x <= range.x2 + m_iSelfPad
			&& range.y1 - m_iSelfPad <= _y && _y <= range.y2 + m_iSelfPad)
			return false;
	}
	

	return true;
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

void CPathFinderScript::EraseInOpenList(priority_queue<tPNode*, vector<tPNode*>, ComparePathLength>& _queue
										, int _iYIdx, int _iXIdx)
{
	priority_queue<tPNode*, vector<tPNode*>, ComparePathLength> tempQueue;

	while (!_queue.empty())
	{
		if (!(_queue.top()->iIdxX == _iXIdx && _queue.top()->iIdxY == _iYIdx))
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
	m_ArrNode[iCurY * 10000 + iCurX] = pCurNode;

	while (true)
	{
		// ���� �������� 4������ ��带 OpenList �� �ִ´�.
		// UP
		AddOpenList(pCurNode->iIdxX
			, pCurNode->iIdxY - 1, pCurNode, eBlock::UP);

		// RIGHT		
		AddOpenList(pCurNode->iIdxX + 1
			, pCurNode->iIdxY, pCurNode, eBlock::RIGHT);

		// DOWN		
		AddOpenList(pCurNode->iIdxX
			, pCurNode->iIdxY + 1, pCurNode, eBlock::DOWN);

		// LEFT		
		AddOpenList(pCurNode->iIdxX - 1
			, pCurNode->iIdxY, pCurNode, eBlock::LEFT);

		//UPLEFT
		if (!(m_Block == 1000 || m_Block == 10 || m_Block == 1010))
		{
			AddOpenList(pCurNode->iIdxX - 1
				, pCurNode->iIdxY - 1, pCurNode, eBlock::NONE, true);
		}

		//UPRIGHT
		if (!(m_Block == 1000 || m_Block == 1 || m_Block == 1001))
		{
			AddOpenList(pCurNode->iIdxX + 1
				, pCurNode->iIdxY - 1, pCurNode, eBlock::NONE, true);
		}
		//DOWNLEFT
		if (!(m_Block == 100 || m_Block == 10 || m_Block == 110))
		{
			AddOpenList(pCurNode->iIdxX - 1
				, pCurNode->iIdxY + 1, pCurNode, eBlock::NONE, true);
		}
		//DOWNRIGHT
		if (!(m_Block == 100 || m_Block == 1 || m_Block == 101))
		{
			AddOpenList(pCurNode->iIdxX + 1
				, pCurNode->iIdxY + 1, pCurNode, eBlock::NONE, true);
		}
		m_Block = 0;

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

	m_StraightTestStack = {};
	// �� ����
	while (true)
	{
		Vec3 nodePos = TransYXToPos(tYX(pCurNode->iIdxY, pCurNode->iIdxX));
		m_Stack.push(nodePos);
		pCurNode = pCurNode->pPrevNode;

		if (pCurNode->iIdxX == m_iCurPosX && pCurNode->iIdxY == m_iCurPosY)
		{
			//Vec3 nodePos = TransYXToPos(tYX(m_iCurPosY, m_iCurPosX));
			//m_Stack.push(nodePos); ù���� ����.
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

void CPathFinderScript::AddOpenList(int _iXIdx, int _iYIdx, tPNode* _pOrigin,eBlock _dir, bool _bDiagonal)
{
	// ���� �������� �� �� �ִ� ���� OpenList �� �ִ´�.
	// ��� ������ ��� ���
	if (_iXIdx < 0 || _iXIdx >= m_iXCount || _iYIdx < 0 || _iYIdx >= m_iYCount)
		return;

	//����ƽ�ʻ� ���ΰ��
	if (!MapTest(_iYIdx, _iXIdx))
	{
		switch (_dir)
		{
		case CPathFinderScript::eBlock::NONE:
			break;
		case CPathFinderScript::eBlock::UP:
			m_Block += 1000;
			break;
		case CPathFinderScript::eBlock::DOWN:
			m_Block += 100;
			break;
		case CPathFinderScript::eBlock::LEFT:
			m_Block += 10;
			break;
		case CPathFinderScript::eBlock::RIGHT:
			m_Block += 1;
			break;
		default:
			break;
		}

		return;
	}

	auto iter = m_ArrNode.find(_iYIdx * 10000 + _iXIdx);
	//�ش� Ű���� �����鼭 
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
		m_ArrNode[_iYIdx * 10000 + _iXIdx] = pCurNode;
	}

	// Open List �� ����� ����ؼ� �ִ´�.
	if (false == m_ArrNode[_iYIdx * 10000 + _iXIdx]->bOpen)
	{
		CalculateCost(m_ArrNode[_iYIdx * 10000 + _iXIdx], _pOrigin, _bDiagonal);
		// Open List �� �ִ´�.
		m_ArrNode[_iYIdx * 10000 + _iXIdx]->bOpen = true;
		m_OpenList.push(m_ArrNode[_iYIdx * 10000 + _iXIdx]);
	}
	else // �̹� OpenList �� �ִ� ���,
	{
		//����� ����ؼ� �� ȿ���� ���� ������ ��ü�Ѵ�.
		tPNode* pNodePtr = m_ArrNode[_iYIdx * 10000 + _iXIdx];
		tPNode* copy = new tPNode(*pNodePtr);

		CalculateCost(copy, _pOrigin, _bDiagonal); //ī���� �ڽ�Ʈ�� ���

		if (m_ArrNode[_iYIdx * 10000 + _iXIdx]->fFinal > copy->fFinal)
		{
			//���� ��� ����
			EraseInOpenList(m_OpenList, _iYIdx, _iXIdx);
			delete m_ArrNode[_iYIdx * 10000 + _iXIdx];
			m_ArrNode[_iYIdx * 10000 + _iXIdx] = nullptr;
			m_ArrNode.erase(_iYIdx * 10000 + _iXIdx);

			//ī�� ����
			m_ArrNode[_iYIdx * 10000 + _iXIdx] = copy;
			m_OpenList.push(m_ArrNode[_iYIdx * 10000 + _iXIdx]);

			// ���¸���Ʈ(�켱����ť) �缳��
			Rebuild(m_OpenList);
		}
		else // ī�� ���
		{
			delete copy;
			copy = nullptr;
		}
	}
}
