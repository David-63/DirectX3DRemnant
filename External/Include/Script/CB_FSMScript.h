#pragma once
#include "CC_FSMScript.h"
#include "CB_StateScript.h"

#define B_Alert                L"animclip\\B_Alert.animclip"
#define B_BloodDrink_End       L"animclip\\B_BloodDrink_End.animclip"
#define B_BloodDrink_Loop      L"animclip\\B_BloodDrink_Loop.animclip"
#define B_BloodDrink_Start     L"animclip\\B_BloodDrink_Start.animclip"

#define B_Walk_Fast_F          L"animclip\\B_Fast_Walk_F.animclip"
#define B_Walk_Fast_FL         L"animclip\\B_Fast_Walk_FL.animclip"
#define B_Walk_Fast_FR         L"animclip\\B_Fast_Walk_FR.animclip"

#define B_Walk_B               L"animclip\\B_Melee_Walk_B.animclip"
#define B_Walk_BL              L"animclip\\B_Melee_Walk_BL.animclip"
#define B_Walk_BR              L"animclip\\B_Melee_Walk_BR.animclip"
#define B_Walk_F               L"animclip\\B_Melee_Walk_F.animclip"
#define B_Walk_FL              L"animclip\\B_Melee_Walk_FL.animclip"
#define B_Walk_FR              L"animclip\\B_Melee_Walk_FR.animclip"

#define B_Down_End              L"animclip\\B_UN_Down_End.animclip"
#define B_Down_Loop             L"animclip\\B_UN_Down_Loop.animclip"
#define B_Down_Start            L"animclip\\B_UN_Down_Start.animclip"

#define B_Impact_Heavy_Head_L   L"animclip\\B_Impact_Heavy_Head_L.animclip"
#define B_Impact_Heavy_Head_R   L"animclip\\B_Impact_Heavy_Head_R.animclip"

#define B_Melee_Atk_L01         L"animclip\\B_Melee_Atk_L01.animclip"
#define B_Melee_Atk_L02         L"animclip\\B_Melee_Atk_L02.animclip"
#define B_Melee_Atk_L03         L"animclip\\B_Melee_Atk_L03.animclip"

#define B_Melee_Atk_R01         L"animclip\\B_Melee_Atk_R01.animclip"
#define B_Melee_Atk_R02         L"animclip\\B_Melee_Atk_R02.animclip"
#define B_Melee_Atk_R03         L"animclip\\B_Melee_Atk_R03.animclip"

#define B_Melee_Atk_Double01    L"animclip\\B_Melee_Atk_Double_01.animclip"
#define B_Melee_Atk_Double02    L"animclip\\B_Melee_Atk_Double_02.animclip"


#define B_Melee_Atk_Weapon_AOE  L"animclip\\B_Melee_Atk_Weapon_AOE.animclip"
#define B_NC_Atk_Summon_Weapons L"animclip\\B_UN_Atk_Summon_Weapons.animclip"

#define B_Melee_Death           L"animclip\\B_Melee_Death.animclip"

#define B_Melee_Down_End        L"animclip\\B_Melee_Down_End.animclip"
#define B_Melee_Down_Loop       L"animclip\\B_Melee_Down_Loop.animclip"
#define B_Melee_Down_Start      L"animclip\\B_Melee_Down_Start.animclip"

#define B_Melee_Evade_B         L"animclip\\B_Melee_Evade_B.animclip"
#define B_Melee_Evade_F         L"animclip\\B_Melee_Evade_F.animclip"
#define B_Melee_Evade_L         L"animclip\\B_Melee_Evade_L.animclip"
#define B_Melee_Evade_R         L"animclip\\B_Melee_Evade_R.animclip"

#define B_Melee_Idle            L"animclip\\B_Melee_Idle.animclip"
#define B_Melee_Return_Passive  L"animclip\\B_Melee_Return_Passive.animclip"

#define B_NC_Agree              L"animclip\\B_NC_Agree.animclip"
#define B_NC_Disagree           L"animclip\\B_NC_Disagree.animclip"
#define B_NC_Idle               L"animclip\\B_NC_Idle.animclip"

#define B_Turn90_L              L"animclip\\Undying_Turn90_L.animclip"
#define B_Turn90_R              L"animclip\\Undying_Turn90_R.animclip"
#define B_Turn135_L              L"animclip\\Undying_Turn135_L.animclip"
#define B_Turn135_R              L"animclip\\Undying_Turn135_R.animclip"
#define B_Turn180_L              L"animclip\\Undying_Turn180_L.animclip"
#define B_Turn180_R              L"animclip\\Undying_Turn180_R.animclip"


class CB_FSMScript :
    public CC_FSMScript
{
public:
    enum class eBossStance_NoWeapon
    {
        NORMAL_WALK, // 1������
        SPAWN_SPELL,
        End,
    };

    enum class eBossStance_Weapon
    {
        MELEE_ATK,
        AOE,
        EVADE,
        FAST_WALK, // 1������
        BLOOD_DRINK_START,
        BLOOD_DRINK_LOOP,
        BLOOD_DRINK_END,
        END,
    };

    enum class eBossMoveDir
    {
        LF, F, RF,
        L, R,
        LB, B, RB,
        END,

    };

    struct tBossStat
    {
        float MoveSpeed = 100.f;

    };

    struct tB_Info
    {
        tBossStat     B_Stat;
        tHealthStat   B_Health = 10000.f;
    };


private:
    tB_Info                  m_tBossInfo;
    eBossMoveDir             B_MoveDir;
    bool                     m_bPlaying;
    int                      m_iAnimCount;
    eBossStance_NoWeapon     m_eNoWeapon_Stance;
    eBossStance_Weapon	     m_eWeapon_Stance;
    float                    m_iRatio;
    tBossStat				 m_tMoveSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;


public:
    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

public:
    void PlayAnim(wstring _name, bool _repeat);
    Vec3 GetPlayerPos();

    void SetRatio(float _f) { m_iRatio = _f; }
    float GetRatio() { return m_iRatio; }

    eBossMoveDir GetMoveDir() { return B_MoveDir; }
    tB_Info GetBossInfo() { return m_tBossInfo; }

    void SetStance_NoWeapon(eBossStance_NoWeapon _e) { m_eNoWeapon_Stance = _e; }
    eBossStance_NoWeapon GetStance_NoWeapon() { return m_eNoWeapon_Stance; }

    void SetStance_Weapon(eBossStance_Weapon _e) { m_eWeapon_Stance = _e; }
    eBossStance_Weapon GetStance_Weapon() { return m_eWeapon_Stance; }

    void SetMoveSpeed(float _f) { m_tMoveSpeed.MoveSpeed = _f; }

    bool IsPlaying() { return m_bPlaying; }

    void SetPlaying(bool _b) { m_bPlaying = _b; }

    eBossMoveDir RandomDir_SomeExclude(); // �Ⱦ�
    eBossMoveDir RandomDir();


    void RandomStance_Melee_WithoutEvade();
    eBossStance_Weapon RandomStance_Melee();

    void Phase1_AnimEnd();
    void BoneRigidbody_Check();





public:
    CLONE(CB_FSMScript);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

public:
    CB_FSMScript();
    virtual ~CB_FSMScript();





};

