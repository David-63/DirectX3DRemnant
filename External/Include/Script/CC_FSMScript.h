#pragma once
#include <Engine/CScript.h>
#include "CC_StatesScript.h"

#define STATE_IDLE 0

class CC_FSMScript : public CScript
{
public:
    struct tHealthStat
    {
        float MaxHp;
        float CurHp;
        bool IsDead;

        void GotDamage(float _in)
        {
            if (CurHp = -_in <= 0)
            {
                CurHp = 0;
                IsDead = true;
            }
        }
        void GotHeal(float _in)
        {
            if (CurHp = +_in >= MaxHp)
            {
                CurHp = MaxHp;
            }
        }
        void Reset() { CurHp = MaxHp; IsDead = false; }

        tHealthStat() : MaxHp(10), CurHp(10), IsDead(false) {}
        tHealthStat(float _in) : MaxHp(_in), CurHp(_in), IsDead(false) {}
    };
    // �Լ��� ���� ����ǹǷ� protected �� ����� �ʿ� ����
private:
    std::map<UINT, CC_StatesScript*>    m_States;
    CC_StatesScript*                    m_CurState;
    UINT                                m_PrevState;

public:
    virtual void begin() override;      // ���� ���� �����ϴ°� ���ſ� �̷ο�
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

public:
    void AddState(CC_StatesScript* _State);
    CC_StatesScript* GetStateScript(UINT _State);
    void ChangeState(UINT _NextState);


public:
    CLONE(CC_FSMScript);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

public:
    CC_FSMScript();
    virtual ~CC_FSMScript();

};

