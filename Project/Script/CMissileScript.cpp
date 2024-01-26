#include "pch.h"
#include "CMissileScript.h"

CMissileScript::CMissileScript()
	: CScript((UINT)SCRIPT_TYPE::MISSILESCRIPT)
	, m_vDir(Vec3(0.f, 1.f, 0.f))
	, m_fSpeed(100.f)
	, m_TargetObject(nullptr)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::begin()
{
	// ������ �����ϴ� ���͸� ã�Ƽ� Ÿ������ ��´�.
	m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(L"Monster");
}

void CMissileScript::tick()
{
	// �̻��� ������ ��ġ���� �˾Ƴ���.
	Vec3 vPos = Transform()->GetRelativePos();

	if(IsValidObj(m_TargetObject))
	{
		Vec3 vTargetPos = m_TargetObject->Transform()->GetWorldPos();

		// �̻��Ͽ��� Ÿ���� ���� ������ �˾Ƴ���.
		Vec3 vDir = vTargetPos - vPos;
		vDir.z = 0.f;
		vDir.Normalize();
		
		// Ÿ���� ���� �������� m_fSpeed �ӵ��� �̵��Ѵ�.
		vPos += vDir* m_fSpeed* DT;
		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos += m_vDir * DT * m_fSpeed;
		Transform()->SetRelativePos(vPos);
	}

	//int a = 1;
	//MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_2, &a);
}
