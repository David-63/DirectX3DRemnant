#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CRigidBody.h"
//#include "CTransform.h"
#include "Physics.h"

CCollisionMgr::CCollisionMgr()
    : m_matrix{}
{

}


CCollisionMgr::~CCollisionMgr()
{

}


void CCollisionMgr::tick()
{
	//CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	//for (UINT iRow = 0; iRow < MAX_LAYER; ++iRow)
	//{
	//	for (UINT iCol = iRow; iCol < MAX_LAYER; ++iCol)
	//	{
	//		if (!(m_matrix[iRow] & (1 << iCol)))
	//			continue;

	//		// iRow ���̾�� iCol ���̾�� ���� �浹�˻縦 �����Ѵ�.
	//		CollisionBtwLayer(pLevel->GetLayer(iRow), pLevel->GetLayer(iCol));
	//	}
	//}
}

bool CCollisionMgr::IsAgainstWall(Vec3 _PlayerPos, Vec3 _PlayerDir, CCollider3D* _Wall)
{
	//// 0 -- 1
	//// |    |
	//// 3 -- 2
	//Vec3 arrLocal[8] =
	//{
	//	Vec3(-0.5f, 0.5f, 0.5f),
	//	Vec3(0.5f, 0.5f, 0.5f),
	//	Vec3(0.5f, 0.5f, -0.5f),
	//	Vec3(-0.5f, 0.5f, -0.5f),
	//	Vec3(-0.5f, -0.5f, 0.5f),
	//	Vec3(0.5f, -0.5f, 0.5f),
	//	Vec3(0.5f, -0.5f, -0.5f),
	//	Vec3(-0.5f, -0.5f, -0.5f),
	//};

	//Matrix mat = _Wall->GetOwner()->Transform()->GetWorldMat();
	//Vec3 square[4] = {};
	//square[0] = XMVector3TransformCoord(arrLocal[0], mat);
	//square[1] = XMVector3TransformCoord(arrLocal[1], mat);
	//square[2] = XMVector3TransformCoord(arrLocal[2], mat);
	//square[3] = XMVector3TransformCoord(arrLocal[3], mat);
	//square[4] = XMVector3TransformCoord(arrLocal[4], mat);
	//square[5] = XMVector3TransformCoord(arrLocal[5], mat);
	//square[6] = XMVector3TransformCoord(arrLocal[6], mat);
	//square[7] = XMVector3TransformCoord(arrLocal[7], mat);

	tRayCastInfo* info = Physics::GetInst()->RayCast(_PlayerPos, _PlayerDir, 1000.f);
	if (info->hit)
	{
		PxRigidActor* actor = info->hitActor;
		const char* name = actor->getName();
		
	}
	else
	{
		return true;
	}
}



//void CCollisionMgr::CollisionBtwLayer(CLayer* _Left, CLayer* _Right)
//{
//	const vector<CGameObject*>& vecLeft = _Left->GetObjects();
//	const vector<CGameObject*>& vecRight = _Right->GetObjects();
//
//	if (_Left == _Right)
//	{
//		for (size_t i = 0; i < vecLeft.size(); ++i)
//		{
//			for (size_t j = i + 1; j < vecRight.size(); ++j)
//			{
//				CollisionBtwObject(vecLeft[i], vecRight[j]);				
//			}
//		}
//	}
//	else
//	{
//		for (size_t i = 0; i < vecLeft.size(); ++i)
//		{
//			for (size_t j = 0; j < vecRight.size(); ++j)
//			{
//				CollisionBtwObject(vecLeft[i], vecRight[j]);				
//			}
//		}
//	}
//}

//void CCollisionMgr::CollisionBtwObject(CGameObject* _LeftObject, CGameObject* _RightObject)
//{
//	if (!(_LeftObject->Collider2D() && _RightObject->Collider2D()))
//		return;
//
//	// �浹ü ID ����
//	CollisionID id = {};
//	id.LeftID = _LeftObject->Collider3D()->GetID();
//	id.RightID = _RightObject->Collider3D()->GetID();
//	
//	// ID �˻�
//	map<UINT_PTR, bool>::iterator iter = m_mapColID.find(id.id);
//	if (iter == m_mapColID.end())
//	{
//		m_mapColID.insert(make_pair(id.id, false));
//		iter = m_mapColID.find(id.id);
//	}
//
//	// �� �� �ϳ��� ���� ���� ���¶��(Dead ����)
//	bool bDead = false;
//	if (_LeftObject->IsDead() || _RightObject->IsDead())
//	{
//		bDead = true;
//	}
//	
//	// �� �浹ü�� ���� �浹 ������ Ȯ��
//	if (CollisionBtwCollider(_LeftObject->Collider2D(), _RightObject->Collider2D()))
//	{
//		// ������ �浹�� ���� �ְ�, ���� �ϳ� �̻��� ���� �����̶��
//		if (bDead && iter->second)
//		{
//			_LeftObject->Collider2D()->EndOverlap(_RightObject->Collider2D());
//			_RightObject->Collider2D()->EndOverlap(_LeftObject->Collider2D());
//		}
//		else if (iter->second)
//		{
//			// ��� �浹 ��
//			_LeftObject->Collider2D()->OnOverlap(_RightObject->Collider2D());
//			_RightObject->Collider2D()->OnOverlap(_LeftObject->Collider2D());
//		}
//		else
//		{
//			// �̹� �����ӿ� �浹
//			if (!bDead) // ���� �ϳ��� Dead ���¸� �浹�� �����Ѵ�.
//			{
//				_LeftObject->Collider2D()->BeginOverlap(_RightObject->Collider2D());
//				_RightObject->Collider2D()->BeginOverlap(_LeftObject->Collider2D());
//				iter->second = true;
//			}			
//		}
//	}
//
//	else
//	{
//		// �浹 ����
//		if (iter->second)
//		{
//			_LeftObject->Collider2D()->EndOverlap(_RightObject->Collider2D());
//			_RightObject->Collider2D()->EndOverlap(_LeftObject->Collider2D());
//			iter->second = false;
//		}
//	}
//
//
//}

// �� �浹ü�� �浹 �˻� ����
//bool CCollisionMgr::CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight)
//{
//	// 0 -- 1
//	// |    |
//	// 3 -- 2
//	Vec3 arrLocal[4] =
//	{
//		Vec3(-0.5f, 0.5f, 0.f),
//		Vec3(0.5f, 0.5f, 0.f),
//		Vec3(0.5f, -0.5f, 0.f),
//		Vec3(-0.5f, -0.5f, 0.f),
//	};
//
//	// �� �浹ü�� �� ǥ�� ���� 2���� ����
//	Vec3 arrProj[4] = {};
//
//	arrProj[0] = XMVector3TransformCoord(arrLocal[1], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
//	arrProj[1] = XMVector3TransformCoord(arrLocal[3], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
//	
//	arrProj[2] = XMVector3TransformCoord(arrLocal[1], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());
//	arrProj[3] = XMVector3TransformCoord(arrLocal[3], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());
//	
//	// �� �浹ü�� �߽����� ����
//	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());
//	
//
//	// �и��� �׽�Ʈ
//	for (int i = 0; i < 4; ++i)
//	{
//		Vec3 vProj = arrProj[i];
//		vProj.Normalize();
//
//		// 4���� ǥ����͸� ������ ���������� ������Ų �Ÿ��� ���� / 2
//		float fProjDist = 0.f;
//		for (int j = 0; j < 4; ++j)
//		{
//			fProjDist += fabsf(arrProj[j].Dot(vProj));
//		}
//		fProjDist /= 2.f;
//
//		float fCenter = fabsf(vCenter.Dot(vProj));
//
//		if (fProjDist < fCenter)
//			return false;
//	}
//
//
//    return true;
//}



void CCollisionMgr::SetColLayer(UINT _left, UINT _right)
{
	UINT iRow = (UINT)_left;
	UINT iCol = (UINT)_right;

	/*if (iRow > iCol)
	{
		UINT iTemp = iCol;
		iCol = iRow;
		iRow = iTemp;
	}*/

	m_matrix[iRow] |= (1 << iCol);
	m_matrix[iCol] |= (1 << iCol);

	//���� ��ǻ� �Ⱦ�.
	/*CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	std::vector<CGameObject*> gameObjects_L = pLevel->GetLayer(_left)->GetParentObject();
	for (CGameObject* pGameObject : gameObjects_L)
	{
		if(pGameObject->RigidBody())
			pGameObject->RigidBody()->SetOtherLayerInFilterData(_right);
	}*/

}

void CCollisionMgr::SetColLayer(const wstring& _strLeftLayer, const wstring& _strRightLayer)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	CLayer* pLeft = pCurLevel->FindLayerByName(_strLeftLayer);
	CLayer* pRight = pCurLevel->FindLayerByName(_strRightLayer);

	SetColLayer(pLeft->GetLayerIndex(), pRight->GetLayerIndex());
}

