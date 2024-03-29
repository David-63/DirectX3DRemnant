#include "pch.h"
#include "CM_Spider_STATE_Atk_Script.h"
#include "CM_Spider_STATE_Move_Script.h"
#include "CM_Spider_FSMScript.h"
#include "CPathFinderScript.h"
#include "CMonsterMoveScript.h"
#include <random>
#include "Engine//Physics.h"


CM_Spider_STATE_Move_Script::CM_Spider_STATE_Move_Script()
	: m_fAniTurnSpeed(2.f)
	, m_fJustTurnSpeed(3.f)
	, m_iGetBackStage(0)

{
	SetStateType(static_cast<UINT>(eM_A_States::MOVE));
}

CM_Spider_STATE_Move_Script::~CM_Spider_STATE_Move_Script()
{

}

void CM_Spider_STATE_Move_Script::begin()
{
	CM_Spider_StatesScript::begin();

	m_CMoveScript = m_MHQ->GetOwner()->GetScript<CMonsterMoveScript>();
	m_CMoveScript->SetSpeed(200.f);

	m_MHQ->Animator3D()->CompleteEvent(Spi_Turn180L) = std::bind(&CM_Spider_STATE_Move_Script::TurnComplete180, this);
	m_MHQ->Animator3D()->CompleteEvent(Spi_Turn90L) = std::bind(&CM_Spider_STATE_Move_Script::TurnComplete90L, this);
	m_MHQ->Animator3D()->CompleteEvent(Spi_Turn90R) = std::bind(&CM_Spider_STATE_Move_Script::TurnComplete90R, this);
	m_MHQ->Animator3D()->CompleteEvent(Spi_Turn135L) = std::bind(&CM_Spider_STATE_Move_Script::TurnComplete135L, this);
	m_MHQ->Animator3D()->CompleteEvent(Spi_Turn135R) = std::bind(&CM_Spider_STATE_Move_Script::TurnComplete135R, this);
}

void CM_Spider_STATE_Move_Script::tick()
{
	TurnAction();
	ActionByDist();

}

void CM_Spider_STATE_Move_Script::Clear()
{
	m_bTurnSet = false;
	m_BTurned180 = false;
	m_BTurned90L = false;
	m_BTurned90R = false;
	m_BTurned135L = false;
	m_BTurned135R = false;
	m_bTurnBtn = false;
	m_bMoveStartOnce = false;
	m_fElapsedTime = 0.f;

	m_bDirCheck = false;
	m_bShootReady = false;
	m_bBackProgress = false;
}

bool CM_Spider_STATE_Move_Script::AniTurn(Vec3 _dir)
{
	if (!m_bTurnSet)
	{
		m_bTurnSet = true;
		m_fTurnDegree = CalTurnDegree(_dir);

		/*if (-15 < m_fTurnDegree && m_fTurnDegree < 15)
		{
			m_bTurnSet = false;
			return true;
		}*/

		if (0.f <= m_fTurnDegree && m_fTurnDegree < 100.f)
			m_eTurnAni = eTurnAni::R90;
		else if (100 <= m_fTurnDegree && m_fTurnDegree < 160.f)
			m_eTurnAni = eTurnAni::R135;
		else if (160 <= m_fTurnDegree && m_fTurnDegree <= 180.f)
			m_eTurnAni = eTurnAni::L180;
		else if (-100.f <= m_fTurnDegree && m_fTurnDegree < 0.f)
			m_eTurnAni = eTurnAni::L90;
		else if (-135 <= m_fTurnDegree && m_fTurnDegree < -100.f)
			m_eTurnAni = eTurnAni::L135;
		else if (-180 <= m_fTurnDegree && m_fTurnDegree < -135.f)
			m_eTurnAni = eTurnAni::L180;
	}

	switch (m_eTurnAni)
	{
	case CM_Spider_STATE_Move_Script::eTurnAni::R90:
		if (Turn90R(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CM_Spider_STATE_Move_Script::eTurnAni::R135:
		if (Turn135R(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CM_Spider_STATE_Move_Script::eTurnAni::L90:
		if (Turn90L(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CM_Spider_STATE_Move_Script::eTurnAni::L135:
		if (Turn135L(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CM_Spider_STATE_Move_Script::eTurnAni::L180:
		if (Turn180(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

bool CM_Spider_STATE_Move_Script::JustTurn(Vec3 _dir)
{
	if (!m_bJustTurnSet)
	{
		m_bJustTurnSet = true;
		m_fTurnDegree = CalTurnDegree(_dir);
		m_vInitialRot = m_MHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += m_fTurnDegree;
		m_fFinishTime = 1.f;
		m_fElapsedTime = 0.f;

		m_MHQ->Animator3D()->Play(Spi_Idle, true);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Idle, true);
	}

	m_fElapsedTime += DT * m_fJustTurnSpeed;

	if (m_fElapsedTime < m_fFinishTime)
	{
		Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
		vecRot = TransDegreeToRadVector(vecRot);
		m_MHQ->Transform()->SetRelativeRot(vecRot);
	}
	else
	{
		m_vFinalRot = TransDegreeToRadVector(m_vFinalRot);

		m_MHQ->Transform()->SetRelativeRot(m_vFinalRot);

		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;
		m_bJustTurnSet = false;

		return true;
	}


	return false;
}

void CM_Spider_STATE_Move_Script::RenewPath()
{
	m_CMoveScript->RenewPath();
}


void CM_Spider_STATE_Move_Script::GetBack()
{
	if (m_iGetBackStage == 0)
	{
		m_vToPlayerDir = GetToPlayerDir();
		m_iGetBackStage++;
	}
	else if (m_iGetBackStage == 1)
	{
		if (JustTurn(m_vToPlayerDir))
		{
			m_iGetBackStage++;
		}
	}
	else if (m_iGetBackStage == 2)
	{
		Vec3 myPos = GetMyPos();
		myPos += GetMyFront() * DT * 100.f;
		m_MHQ->Transform()->SetRelativePos(myPos);
	}
}


bool CM_Spider_STATE_Move_Script::ShootTest()
{
	Vec3 MyPos = GetMyPos();	
	MyPos.y += 2.f;

	Vec3 playerdir = GetToPlayerDir();

	tRayCastInfo* info = Physics::GetInst()->RayCast(MyPos, playerdir, 2500.f, true);

	if (info->hit)
	{
		CGameObject* Data1 = (CGameObject*)info->hitActor->userData;
		CGameObject* PlayerAd = (CGameObject*)GetPlayer();

		if (Data1 == PlayerAd)
			return true;
	}

	/*Vec3 AdPosM = GetPlayerPos();
	AdPosM.x -= 5.f;
	AdPosM.z -= 5.f;
	Vec3 AdjustDirM = AdPosM - MyPos;
	AdjustDirM = AdjustDirM.Normalize();

	tRayCastInfo* infoM = Physics::GetInst()->RayCast(MyPos, AdjustDirM, 2500.f, true);
	if (info->hit)
	{
		CGameObject* Data1 = (CGameObject*)info->hitActor->userData;
		CGameObject* PlayerAd = (CGameObject*)GetPlayer();

		if (Data1 == PlayerAd)
			return true;
	}*/

	return false;
}

void CM_Spider_STATE_Move_Script::TurnAction()
{
	if (m_bMoveStartOnce || m_bBackProgress)
		return;

	if (!m_bDirCheck)
	{
		m_vToPlayerDir = GetToPlayerDir();
		m_bDirCheck = true;

		m_fToPlayerDegree = CalTurnDegree(m_vToPlayerDir);
		m_fToPlayerDegree = abs(m_fToPlayerDegree);
	}
	else if (m_bDirCheck)
	{
		if (m_fToPlayerDegree < 10.f)
		{
			if (JustTurn(m_vToPlayerDir))
			{
				m_bDirCheck = false;
				m_bShootReady = true;
			}
		}
		else
		{
			if (AniTurn(m_vToPlayerDir))
			{
				m_bDirCheck = false;
				m_bShootReady = true;
			}
		}
	}
}

void CM_Spider_STATE_Move_Script::ActionByDist()
{
	float dist = DistBetwPlayer();
	if (dist < 220.f)
	{
		m_bBackProgress = false;
		m_bBackAniOnce = false;
		m_bMoveStartOnce = false;
		m_CMoveScript->MoveOn(false);

		m_CMoveScript->SetNearPlayer(true);
		dynamic_cast<CM_Spider_STATE_Atk_Script*>(m_MHQ->FindStateScript((UINT)eM_A_States::ATTACK))->SetAtkState(eAtkState::Push);
		m_MHQ->ChangeState((UINT)eM_A_States::ATTACK);
		return;
	}
	else
	{
		m_CMoveScript->SetNearPlayer(false);
	}

	if (220.f <= dist && dist <= 300.f)
	{
		m_bBackProgress = true;
		m_bBackAniOnce = false;
		m_bMoveStartOnce = false;
		m_CMoveScript->MoveOn(false);

		if (!m_bBackAniOnce)
		{
			m_MHQ->Animator3D()->Play(Spi_WalkB, true);
			m_MHQ->GetGun()->Animator3D()->Play(SpiGun_WalkB, true);
			m_bBackAniOnce = true;
		}
		GetBack();
	}
	else if (300.f < dist && dist <= 2000.f)
	{
		m_bBackProgress = false;
		m_bBackAniOnce = false;
		m_bMoveStartOnce = false;
		m_CMoveScript->MoveOn(false);

		if (!m_bShootReady)
			return;

		dynamic_cast<CM_Spider_STATE_Atk_Script*>(m_MHQ->FindStateScript((UINT)eM_A_States::ATTACK))->SetAtkState(eAtkState::Shoot);
		m_MHQ->ChangeState((UINT)eM_A_States::ATTACK);
	}
	else if (2000.f < dist)
	{
		if (!m_bMoveStartOnce)
		{
			m_MHQ->Animator3D()->Play(Spi_WalkF, true);
			m_MHQ->GetGun()->Animator3D()->Play(SpiGun_WalkF, true);

			m_CMoveScript->SetAndGetPath(GetPlayer());
			m_CMoveScript->MoveOn(true);
			m_bMoveStartOnce = true;
		}
		else
		{
			RenewPath();
		}
	}
}

void CM_Spider_STATE_Move_Script::Enter()
{
	m_MHQ->Animator3D()->Play(Spi_Idle, true);
	m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Idle, true);
	
}

void CM_Spider_STATE_Move_Script::Exit()
{
	Clear();
	m_CMoveScript->Clear();
}

void CM_Spider_STATE_Move_Script::TurnComplete180()
{
	m_BTurned180 = true;

	m_MHQ->Animator3D()->Play(Spi_Idle, true);
	m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Idle, true);
}
void CM_Spider_STATE_Move_Script::TurnComplete90L()
{
	m_BTurned90L = true;
	m_MHQ->Animator3D()->Play(Spi_Idle, true);
	m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Idle, true);
}
void CM_Spider_STATE_Move_Script::TurnComplete90R()
{
	m_BTurned90R = true;
	m_MHQ->Animator3D()->Play(Spi_Idle, true);
	m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Idle, true);
}
void CM_Spider_STATE_Move_Script::TurnComplete135L()
{
	m_BTurned135L = true;
	m_MHQ->Animator3D()->Play(Spi_Idle, true);
	m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Idle, true);
}
void CM_Spider_STATE_Move_Script::TurnComplete135R()
{
	m_BTurned135R = true;
	m_MHQ->Animator3D()->Play(Spi_Idle, true);
	m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Idle, true);
}

bool CM_Spider_STATE_Move_Script::Turn180(float _degree)
{
	if (!m_bTurnBtn)
	{

		m_MHQ->Animator3D()->Play(Spi_Turn180L, false);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Turn180L, false);

		m_vInitialRot = m_MHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += _degree;
		m_fFinishTime = m_MHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * m_fAniTurnSpeed;


	if (!m_BTurned180)
	{
		if (m_fElapsedTime < m_fFinishTime)
		{
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_MHQ->Transform()->SetRelativeRot(vecRot);
		}
		else
		{
			m_fElapsedTime = m_fFinishTime;
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_MHQ->Transform()->SetRelativeRot(vecRot);

			m_BTurned180 = false;
			m_bTurnBtn = false;
			m_vInitialRot = { 0.f, 0.f, 0.f };
			m_vFinalRot = { 0.f,0.f,0.f };
			m_fFinishTime = 0.f;
			m_fElapsedTime = 0.f;

			return true;
		}
	}
	else
	{
		m_BTurned180 = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}
bool CM_Spider_STATE_Move_Script::Turn90L(float _degree)
{
	if (!m_bTurnBtn)
	{

		m_MHQ->Animator3D()->Play(Spi_Turn90L, false);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Turn90L, false);

		m_vInitialRot = m_MHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += _degree;
		m_fFinishTime = m_MHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * m_fAniTurnSpeed;


	if (!m_BTurned90L)
	{
		if (m_fElapsedTime < m_fFinishTime)
		{
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_MHQ->Transform()->SetRelativeRot(vecRot);
		}
		else
		{
			m_fElapsedTime = m_fFinishTime;
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_MHQ->Transform()->SetRelativeRot(vecRot);

			m_BTurned90L = false;
			m_bTurnBtn = false;
			m_vInitialRot = { 0.f, 0.f, 0.f };
			m_vFinalRot = { 0.f,0.f,0.f };
			m_fFinishTime = 0.f;
			m_fElapsedTime = 0.f;

			return true;
		}
	}
	else
	{
		m_BTurned90L = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}
bool CM_Spider_STATE_Move_Script::Turn90R(float _degree)
{
	if (!m_bTurnBtn)
	{
		m_MHQ->Animator3D()->Play(Spi_Turn90R, false);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Turn90R, false);

		m_vInitialRot = m_MHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += _degree;
		m_fFinishTime = m_MHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * m_fAniTurnSpeed;

	if (!m_BTurned90R)
	{
		if (m_fElapsedTime < m_fFinishTime)
		{
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_MHQ->Transform()->SetRelativeRot(vecRot);
		}
		else
		{
			m_fElapsedTime = m_fFinishTime;
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_MHQ->Transform()->SetRelativeRot(vecRot);

			m_BTurned90R = false;
			m_bTurnBtn = false;
			m_vInitialRot = { 0.f, 0.f, 0.f };
			m_vFinalRot = { 0.f,0.f,0.f };
			m_fFinishTime = 0.f;
			m_fElapsedTime = 0.f;

			return true;
		}
	}
	else
	{
		m_BTurned90R = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}
bool CM_Spider_STATE_Move_Script::Turn135L(float _degree)
{
	if (!m_bTurnBtn)
	{
		m_MHQ->Animator3D()->Play(Spi_Turn135L, false);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Turn135L, false);

		m_vInitialRot = m_MHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += _degree;
		m_fFinishTime = m_MHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * m_fAniTurnSpeed;


	if (!m_BTurned135L)
	{
		if (m_fElapsedTime < m_fFinishTime)
		{
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_MHQ->Transform()->SetRelativeRot(vecRot);
		}
		else
		{
			m_fElapsedTime = m_fFinishTime;
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_MHQ->Transform()->SetRelativeRot(vecRot);

			m_BTurned135L = false;
			m_bTurnBtn = false;
			m_vInitialRot = { 0.f, 0.f, 0.f };
			m_vFinalRot = { 0.f,0.f,0.f };
			m_fFinishTime = 0.f;
			m_fElapsedTime = 0.f;

			return true;
		}
	}
	else
	{
		m_BTurned135L = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}
bool CM_Spider_STATE_Move_Script::Turn135R(float _degree)
{
	if (!m_bTurnBtn)
	{

		m_MHQ->Animator3D()->Play(Spi_Turn135R, false);
		m_MHQ->GetGun()->Animator3D()->Play(SpiGun_Turn135R, false);

		m_vInitialRot = m_MHQ->Transform()->GetRelativeRot();
		m_vInitialRot = TransRadToDegreeVector(m_vInitialRot);
		m_vFinalRot = m_vInitialRot;
		m_vFinalRot.y += _degree;
		m_fFinishTime = m_MHQ->Animator3D()->GetFinishTime();
		m_fElapsedTime = 0.f;

		m_bTurnBtn = true;
	}

	m_fElapsedTime += DT * m_fAniTurnSpeed;


	if (!m_BTurned135R)
	{
		if (m_fElapsedTime < m_fFinishTime)
		{
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_MHQ->Transform()->SetRelativeRot(vecRot);
		}
		else
		{
			m_fElapsedTime = m_fFinishTime;
			Vec3 vecRot = Vector3::Lerp(m_vInitialRot, m_vFinalRot, m_fElapsedTime / m_fFinishTime);
			vecRot = TransDegreeToRadVector(vecRot);
			m_MHQ->Transform()->SetRelativeRot(vecRot);

			m_BTurned135R = false;
			m_bTurnBtn = false;
			m_vInitialRot = { 0.f, 0.f, 0.f };
			m_vFinalRot = { 0.f,0.f,0.f };
			m_fFinishTime = 0.f;
			m_fElapsedTime = 0.f;

			return true;
		}
	}
	else
	{
		m_BTurned135R = false;
		m_bTurnBtn = false;
		m_vInitialRot = { 0.f, 0.f, 0.f };
		m_vFinalRot = { 0.f,0.f,0.f };
		m_fFinishTime = 0.f;
		m_fElapsedTime = 0.f;

		return true;
	}

	return false;
}