#pragma once
#include <Engine\CScript.h>

// 현재 지금 안봐도 되는 내용임 (길찾기 관련 코드) 
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
    std::stack<Vec3>* m_Stack; // 경로를 stack에 넣어둠
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

