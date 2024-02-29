#pragma once
#include "ComponentUI.h"
#include <Engine/CMesh.h>
#include <Engine/CRigidBody.h>

class RigidBodyUI : public ComponentUI
{
private:
    int mCurIdx;
    int mSelectIdx;
    bool m_bSelect;
    UINT m_TargetID;

public:
    virtual int render_update() override;

public:


public:
    RigidBodyUI();
    ~RigidBodyUI();
};
