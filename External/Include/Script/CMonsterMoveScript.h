#pragma once
#include <Engine\CScript.h>

class CMonsterMoveScript
	: public CScript
{
private:
    float m_Speed;
    float m_Pad;
    float m_XDiff;
    float m_YDiff;
    int   m_RenewCount;
    Vec3  m_finalDst;

    bool m_bCatch;
    bool m_bTargetOn;


    class CPathFinderScript* m_sPathFinder;
    std::stack<Vec3>* m_Stack;
    CGameObject* m_TargetObj;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMonsterMoveScript);
public:
    CMonsterMoveScript();
    ~CMonsterMoveScript();

public:
    void MoveTo(Vec3 _dstPos);
    void SetAndGetPath(CGameObject* _pObject);

private:
    void Trace();
    bool RenewPath();

};

