#pragma once
#include <Engine\CScript.h>
class CCharacterMoveScript :
    public CScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;


public:
    Vec3    m_vDir;
    float   m_fHzInput;
    float   m_fVertiInput;
    Vec3    m_vMoveDirection;

private:
    CLONE(CCharacterMoveScript);

public:
    CCharacterMoveScript();
    ~CCharacterMoveScript();


};

