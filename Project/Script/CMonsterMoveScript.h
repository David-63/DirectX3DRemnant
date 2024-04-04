#pragma once
#include <Engine\CScript.h>

class CMonsterMoveScript
	: public CScript
{
private:
    float m_fSpeed;
    float m_Pad;
    float m_XDiff;
    float m_YDiff;
    int   m_RenewCount;
    Vec3  m_finalDst;

    bool m_bTargetOn;
    bool m_bMoveOn;
    bool m_bNearPlayer;

    bool m_bDestbyPos;


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
    void SetAndGetPath(Vec3 _pos);
    void MoveOn(bool _is);
    std::stack<Vec3>* GetStack() { return m_Stack; } // 사이즈가 0이나 1
    bool RenewPath();
    void SetNearPlayer(bool is) { m_bNearPlayer = is; }

    void SetSpeed(float _speed) { m_fSpeed = _speed; }
    void Clear();

private:
    void Trace();



};

