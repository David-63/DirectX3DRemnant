#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

CPrefab::CPrefab()
	: CRes(RES_TYPE::PREFAB)
	, m_ProtoObj(nullptr)
{
}

CPrefab::~CPrefab()
{
	if (nullptr != m_ProtoObj)
		delete m_ProtoObj;
}

CGameObject* CPrefab::Instantiate()
{
	return m_ProtoObj->Clone();	
}

void CPrefab::RegisterProtoObject(CGameObject* _Proto)
{
	// ���� ������Ʈ�� ���� �Ҽ��� �ƴϿ��� �Ѵ�.
	assert(-1 == _Proto->GetLayerIndex());

	m_ProtoObj = _Proto;
}