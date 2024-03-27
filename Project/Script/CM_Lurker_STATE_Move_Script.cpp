#include "pch.h"
#include "CM_Lurker_STATE_Melee_Script.h"
#include "CM_Lurker_STATE_Move_Script.h"
#include "CM_Lurker_FSMScript.h"
#include "CPathFinderScript.h"
#include "CMonsterMoveScript.h"
#include <random>


CM_Lurker_STATE_Move_Script::CM_Lurker_STATE_Move_Script()
	: m_iCatchStage(0)
	, m_fAniTurnSpeed(1.f)
	, m_fJustTurnSpeed(2.f)
	, m_iGetBackStage(0)

{
	SetStateType(static_cast<UINT>(eM_States::MOVE));
}

CM_Lurker_STATE_Move_Script::~CM_Lurker_STATE_Move_Script()
{

}

void CM_Lurker_STATE_Move_Script::begin()
{
	CM_Lurker_StatesScript::begin();

	m_CMoveScript = m_MHQ->GetOwner()->GetScript<CMonsterMoveScript>();
	m_CMoveScript->SetSpeed(200.f);

	m_MHQ->Animator3D()->CompleteEvent(Lurker_Turn180) = std::bind(&CM_Lurker_STATE_Move_Script::TurnComplete180, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_Turn90L) = std::bind(&CM_Lurker_STATE_Move_Script::TurnComplete90L, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_Turn90R) = std::bind(&CM_Lurker_STATE_Move_Script::TurnComplete90R, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_Turn135L) = std::bind(&CM_Lurker_STATE_Move_Script::TurnComplete135L, this);
	m_MHQ->Animator3D()->CompleteEvent(Lurker_Turn135R) = std::bind(&CM_Lurker_STATE_Move_Script::TurnComplete135R, this);
}

void CM_Lurker_STATE_Move_Script::tick()
{
	/*if (KEY_TAP(KEY::Y))
	{
		mKey = true;
	}

	if (mKey)
	{
		if (JustTurn(Vec3(1.0, 0.f, 0.f)))
		{
			mKey = false;
		}
	}*/

	float dist = DistBetwPlayer();
	if (dist < 180.f)
		m_CMoveScript->SetNearPlayer(true);
	else
		m_CMoveScript->SetNearPlayer(false);

	if (m_bEnterDistCheck)
	{
		if (dist < 180.f)
			GetBack();
		else
		{
			m_bEnterDistCheck = false;
			m_iGetBackStage = 0;
		}
	}
	else
	{
		if (m_bEnterSign)
		{
			if (MoveStart())
				m_bEnterSign = false;
		}
		else
		{
			if (m_iCatchStage == 0)
				RenewPath();

			CatchCheck();
		}
	}
}

void CM_Lurker_STATE_Move_Script::Clear()
{
	m_bTurnSet = false;
	m_BTurned180 = false;
	m_BTurned90L = false;
	m_BTurned90R = false;
	m_BTurned135L = false;
	m_BTurned135R = false;
	m_bTurnBtn = false;
	m_bMoveStartOnce = false;
	m_bEnterSign = false;
	m_bTurnAtStart = false;
	m_bPathRenewed = false;
	m_bGetDirOnce = false;
	m_bTurnAtRenew = false;
	m_fElapsedTime = 0.f;
	m_iCatchStage = 0;
}

bool CM_Lurker_STATE_Move_Script::AniTurn(Vec3 _dir)
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
	case CM_Lurker_STATE_Move_Script::eTurnAni::R90:
		if (Turn90R(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CM_Lurker_STATE_Move_Script::eTurnAni::R135:
		if (Turn135R(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CM_Lurker_STATE_Move_Script::eTurnAni::L90:
		if (Turn90L(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CM_Lurker_STATE_Move_Script::eTurnAni::L135:
		if (Turn135L(m_fTurnDegree))
		{
			m_bTurnSet = false;
			return true;
		}
		break;
	case CM_Lurker_STATE_Move_Script::eTurnAni::L180:
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

bool CM_Lurker_STATE_Move_Script::JustTurn(Vec3 _dir)
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

		//m_MHQ->Animator3D()->Play(Lurker_IDLESTAND, true);
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

Vec3 CM_Lurker_STATE_Move_Script::CalMoveDir()
{
	std::stack<Vec3>* stack = m_CMoveScript->GetStack();

	if (stack->size() <= 2)
		return Vec3(0.f, 0.f, 0.f);

	Vec3 startPos = stack->top();
	stack->pop();
	Vec3 secondPos = stack->top();
	stack->push(startPos);

	Vec3 dir = secondPos - startPos;
	dir = dir.Normalize();

	return dir;
}

bool CM_Lurker_STATE_Move_Script::MoveStart()
{
	if (!m_bMoveStartOnce)
	{
		m_CMoveScript->SetAndGetPath(GetPlayer());

		RotationReset();
		float prevDegree = CalTurnDegree(m_vTurndir);

		m_vTurndir = CalMoveDir();
		float curDegree = CalTurnDegree(m_vTurndir);
		float diff = abs(prevDegree - curDegree);

		if (diff > 70.f)
			m_bUseAniTurn = true;
		else
			m_bUseAniTurn = false;

		if (diff < 10.f && DistBetwPlayer() < 200.f)
			return true;
		
		m_bMoveStartOnce = true;
	}

	if (m_bUseAniTurn)
	{
		if (AniTurn(m_vTurndir))
		{
			m_CMoveScript->MoveOn(true);
			m_bMoveStartOnce = false;
			m_bEnterSign = false;

			return true;
		}
	}
	else
	{
		if (JustTurn(m_vTurndir))
		{
			m_CMoveScript->MoveOn(true);
			m_bMoveStartOnce = false;
			m_bEnterSign = false;

			return true;
		}
	}

	return false;
}

void CM_Lurker_STATE_Move_Script::CatchCheck()
{
	if (m_iCatchStage == 0)
	{
		switch (m_eMoveMelee)
		{
		case eMeleeState::Dash:
			if (DistBetwPlayer() < 450.f)
			{
				m_vToPlayerDir = GetToPlayerDir();
				m_iCatchStage++;
			}
			break;
		case eMeleeState::Heavy1:
			if (DistBetwPlayer() < 350.f)
			{
				m_vToPlayerDir = GetToPlayerDir();
				m_iCatchStage++;
			}
			break;
		case eMeleeState::Slash:
			if (DistBetwPlayer() < 250.f)
			{
				m_vToPlayerDir = GetToPlayerDir();
				m_iCatchStage++;
			}
			break;
		default:
			break;
		}
	}
	else if (m_iCatchStage == 1)
	{
		switch (m_eMoveMelee)
		{
		case eMeleeState::Dash:
			if(JustTurn(m_vToPlayerDir))
				m_iCatchStage++;
			break;
		case eMeleeState::Heavy1:
			if (JustTurn(m_vToPlayerDir))
				m_iCatchStage++;
			break;
		case eMeleeState::Slash:
			if (JustTurn(m_vToPlayerDir))
				m_iCatchStage++;
			break;
		default:
			break;
		}
	}
	else if (m_iCatchStage == 2)
	{
		switch (m_eMoveMelee)
		{
		case eMeleeState::Dash:
				m_CMoveScript->MoveOn(false);
				m_MHQ->ChangeState((UINT)eM_States::MELEE);
				m_iCatchStage = 0;
			break;
		case eMeleeState::Heavy1:
				m_CMoveScript->MoveOn(false);
				m_MHQ->ChangeState((UINT)eM_States::MELEE);
				m_iCatchStage = 0;
			break;
		case eMeleeState::Slash:
				m_CMoveScript->MoveOn(false);
				m_MHQ->ChangeState((UINT)eM_States::MELEE);
				m_iCatchStage = 0;
			break;
		default:
			break;
		}
	}
}

void CM_Lurker_STATE_Move_Script::RenewPath()
{
	if (m_CMoveScript->RenewPath())
	{
		m_bPathRenewed = true;
		RotationReset();
	}

	if (m_bPathRenewed) //&& m_CMoveScript->IsJustTurned() > 0)
	{
		if (!m_bGetDirOnce)
		{
			m_vTurndir = CalMoveDir();
			m_bGetDirOnce = true;
		}

		if (JustTurn(m_vTurndir))
		{
			m_bPathRenewed = false;
			m_bGetDirOnce = false;
		}
	}
}

void CM_Lurker_STATE_Move_Script::DecidePattern()
{
	float dist = DistBetwPlayer();
	if (dist > 450.f)
	{
		std::random_device rd;
		std::mt19937_64 gen(rd());
		std::uniform_int_distribution<int> distribution(0, 2);

		m_eMoveMelee = (eMeleeState)distribution(gen);
		dynamic_cast<CM_Lurker_STATE_Melee_Script*>(m_MHQ->FindStateScript(1))->SetMeleeState(m_eMoveMelee);
	}
	else
	{
		std::random_device rd;
		std::mt19937_64 gen(rd());
		std::uniform_int_distribution<int> distribution(1, 2);

		m_eMoveMelee = (eMeleeState)distribution(gen);
		dynamic_cast<CM_Lurker_STATE_Melee_Script*>(m_MHQ->FindStateScript(1))->SetMeleeState(m_eMoveMelee);
	}
}

bool CM_Lurker_STATE_Move_Script::DistCheck()
{
	if (DistBetwPlayer() < 200.f)
		return true;
	else
		return false;
}

void CM_Lurker_STATE_Move_Script::GetBack()
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

void CM_Lurker_STATE_Move_Script::RotationReset()
{
	Vec3 rot = m_MHQ->Transform()->GetRelativeRot();
	TransRadToDegreeVector(rot);
	if (rot.y > 360.f) 
	{
		rot.y = fmod(rot.y, 360.f);


		rot = TransDegreeToRadVector(rot);
		m_MHQ->Transform()->SetRelativeRot(rot);
	}
	else if (rot.y < -360.f)
	{
		rot.y = fmod(rot.y, 360.f);

		rot = TransDegreeToRadVector(rot);
		m_MHQ->Transform()->SetRelativeRot(rot);
	}
	
	
}

void CM_Lurker_STATE_Move_Script::Enter()
{
	m_bEnterSign = true;
	DecidePattern();
	m_MHQ->Animator3D()->Play(Lurker_MoveFront, true);
	if (DistCheck())
		m_bEnterDistCheck = true;
}

void CM_Lurker_STATE_Move_Script::Exit()
{
	Clear();
	m_CMoveScript->Clear();
}

void CM_Lurker_STATE_Move_Script::TurnComplete180()
{
	m_BTurned180 = true;

	m_MHQ->Animator3D()->Play(Lurker_MoveFront, true);
}
void CM_Lurker_STATE_Move_Script::TurnComplete90L()
{
	m_BTurned90L = true;
	m_MHQ->Animator3D()->Play(Lurker_MoveFront, true);
}
void CM_Lurker_STATE_Move_Script::TurnComplete90R()
{
	m_BTurned90R = true;
	m_MHQ->Animator3D()->Play(Lurker_MoveFront, true);
}
void CM_Lurker_STATE_Move_Script::TurnComplete135L()
{
	m_BTurned135L = true;
	m_MHQ->Animator3D()->Play(Lurker_MoveFront, true);
}
void CM_Lurker_STATE_Move_Script::TurnComplete135R()
{
	m_BTurned135R = true;
	m_MHQ->Animator3D()->Play(Lurker_MoveFront, true);
}

bool CM_Lurker_STATE_Move_Script::Turn180(float _degree)
{
	if (!m_bTurnBtn)
	{

		m_MHQ->Animator3D()->Play(Lurker_Turn180, false);

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
bool CM_Lurker_STATE_Move_Script::Turn90L(float _degree)
{
	if (!m_bTurnBtn)
	{

		m_MHQ->Animator3D()->Play(Lurker_Turn90L, false);

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
bool CM_Lurker_STATE_Move_Script::Turn90R(float _degree)
{
	if (!m_bTurnBtn)
	{
		m_MHQ->Animator3D()->Play(Lurker_Turn90R, false);

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
bool CM_Lurker_STATE_Move_Script::Turn135L(float _degree)
{
	if (!m_bTurnBtn)
	{
		m_MHQ->Animator3D()->Play(Lurker_Turn135L, false);

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
bool CM_Lurker_STATE_Move_Script::Turn135R(float _degree)
{
	if (!m_bTurnBtn)
	{

		m_MHQ->Animator3D()->Play(Lurker_Turn135R, false);

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