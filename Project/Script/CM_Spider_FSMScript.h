#pragma once
#include "CC_FSMScript.h"
#include "CM_Spider_StatesScript.h"

class CM_Spider_FSMScript : public CC_FSMScript
{
public:
	enum class eSpiderState
	{
		Idle,
		Mttack,
		Move,
		Damaged,
		Dead,

	};

	struct tMonsterStat
	{
		float MoveSpeed;
		float AlertDist;

	};

	struct tM_Info
	{
		tMonsterStat     M_Stat;
		tHealthStat      M_Health;
	};

private:
	tM_Info      m_tMonsterInfo;
	CGameObject*  m_Gun;

	


public:
	virtual void begin() override;
	virtual void tick() override;

public:
	virtual void BeginOverlap(CCollider3D* _Other) override;
	virtual void OnOverlap(CCollider3D* _Other) override;
	virtual void EndOverlap(CCollider3D* _Other) override;

private:
	void DeathCheck();

public:
	CLONE(CM_Spider_FSMScript);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CM_Spider_FSMScript();
	virtual ~CM_Spider_FSMScript();

	CGameObject* GetGun() { return m_Gun; }

};


//Idle
#define Spi_Idle             L"animclip\\Spider\\Wasteland_Spider_Idle.animclip"
#define SpiGun_Idle          L"animclip\\Spider\\Wasteland_Spider_Gun_Idle.animclip"
#define Spi_IdleP            L"animclip\\Spider\\Wasteland_Spider_Passive_Idle.animclip"
#define SpiGun_IdleP         L"animclip\\Spider\\Wasteland_Spider_Gun_Passive_Idle.animclip"
#define Spi_Alert            L"animclip\\Spider\\Wasteland_Spider_Alert.animclip"
#define SpiGun_Alert         L"animclip\\Spider\\Wasteland_Spider_Gun_Alert.animclip"


//Atk
#define Spi_AtkPush          L"animclip\\Spider\\Wasteland_Spider_Atk_Push.animclip"
#define SpiGun_AtkPush       L"animclip\\Spider\\Wasteland_Spider_Gun_Atk_Push.animclip"
#define Spi_Atk              L"animclip\\Spider\\Wasteland_Spider_Atk.animclip"
#define SpiGun_Atk           L"animclip\\Spider\\Wasteland_Spider_Gun_Atk.animclip"


//Move
#define Spi_WalkF                L"animclip\\Spider\\Wasteland_Spider_Walk_F.animclip"
#define SpiGun_WalkF             L"animclip\\Spider\\Wasteland_Spider_Gun_Walk_F.animclip"
#define Spi_WalkB                L"animclip\\Spider\\Wasteland_Spider_Walk_B.animclip"
#define SpiGun_WalkB             L"animclip\\Spider\\Wasteland_Spider_Gun_Walk_B.animclip"
#define Spi_WalkL                L"animclip\\Spider\\Wasteland_Spider_Walk_L.animclip"
#define SpiGun_WalkL             L"animclip\\Spider\\Wasteland_Spider_Gun_Walk_L.animclip"
#define Spi_WalkR                L"animclip\\Spider\\Wasteland_Spider_Walk_R.animclip"
#define SpiGun_WalkR             L"animclip\\Spider\\Wasteland_Spider_Gun_Walk_R.animclip"

//Death
#define Spi_Death                L"animclip\\Spider\\Wasteland_Spider_Death_F_01.animclip"
#define SpiGun_Death             L"animclip\\Spider\\Wasteland_Spider_Gun_Death_F_01.animclip"

//attacked impact
#define Spi_ImpactF              L"animclip\\Spider\\Wasteland_Spider_Impact_F.animclip"
#define SpiGun_ImpactF           L"animclip\\Spider\\Wasteland_Spider_Gun_Impact_F.animclip"
#define Spi_ImpactHeavyF         L"animclip\\Spider\\Wasteland_Spider_Impact_Heavy_F.animclip"
#define SpiGun_ImpactHeavyF      L"animclip\\Spider\\Wasteland_Spider_Gun_Impact_Heavy_F.animclip"

//Turn
#define Spi_Turn90L              L"animclip\\Spider\\Wasteland_Spider_Turn90_L.animclip"
#define SpiGun_Turn90L           L"animclip\\Spider\\Wasteland_Spider_Gun_Turn90_L.animclip"
#define Spi_Turn90R              L"animclip\\Spider\\Wasteland_Spider_Turn90_R.animclip"
#define SpiGun_Turn90R           L"animclip\\Spider\\Wasteland_Spider_Gun_Turn90_R.animclip"
#define Spi_Turn135L              L"animclip\\Spider\\Wasteland_Spider_Turn135_L.animclip"
#define SpiGun_Turn135L           L"animclip\\Spider\\Wasteland_Spider_Gun_Turn135_L.animclip"
#define Spi_Turn135R              L"animclip\\Spider\\Wasteland_Spider_Turn135_R.animclip"
#define SpiGun_Turn135R           L"animclip\\Spider\\Wasteland_Spider_Gun_Turn135_R.animclip"
#define Spi_Turn180L              L"animclip\\Spider\\Wasteland_Spider_Turn180_L.animclip"
#define SpiGun_Turn180L           L"animclip\\Spider\\Wasteland_Spider_Gun_Turn180_L.animclip"