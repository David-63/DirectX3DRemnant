#include "pch.h"
#include "CB_RangeBallScript.h"
#include "CC_FSMScript.h"

CB_RangeBallScript::CB_RangeBallScript()
	: CScript((UINT)SCRIPT_TYPE::B_FSMSCRIPT)
	, m_fCoolTime(0.f)
	, m_Player(nullptr)
	, m_RangeBall(nullptr)
	, m_bFire(false)
{
}

CB_RangeBallScript::~CB_RangeBallScript()
{
}

void CB_RangeBallScript::begin()
{
	m_vInitialPos = GetOwner()->Transform()->GetRelativePos(); // �ʱ� ��ġ ����
	m_RangeBall = GetOwner();
}

void CB_RangeBallScript::tick()
{

		
		RangeBalToPlayerDir = m_vPlayerPos - m_RangeBall->Transform()->GetRelativePos();
		RangeBalToPlayerDir = RangeBalToPlayerDir.Normalize();

		m_vCurPos = m_RangeBall->Transform()->GetRelativePos();
		m_vCurPos += RangeBalToPlayerDir * DT * 1500.f;

		GetOwner()->Transform()->SetRelativePos(m_vCurPos);
	



	//// �߻�ü�� �ʱ� ��ġ�� �߻�� ���� ��ġ�� �Ÿ��� 1000�� �Ѿ�ų�, �÷��̾�� �ε��ƴٸ� �����Ѵ�. 
	//if (Vector3::Distance(m_vCurPos, m_vInitialPos) > 5000.f)
	//{
	//	DestroyObject(GetOwner());
	//}


}

void CB_RangeBallScript::BeginOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == (UINT)LAYER_TYPE::Player)
	{
		tHitInfo info;
		info.Damage = 10;
		info.KnockBackGrade = 0;
		info.Shooter = GetOwner()->GetReserver();
		info.ShooterPos = GetOwner()->GetReserver()->Transform()->GetRelativePos();
		_Other->GetOwner()->GetScript<CC_FSMScript>()->GiveAtkInfo(info);

		DestroyObject(GetOwner());
	}


}

void CB_RangeBallScript::OnOverlap(CCollider3D* _Other)
{
}

void CB_RangeBallScript::EndOverlap(CCollider3D* _Other)
{
}
