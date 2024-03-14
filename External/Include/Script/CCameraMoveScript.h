#pragma once
#include <Engine\CScript.h>

// 마우스 움직임 감지

// 카메라 제어


class CCameraMoveScript : public CScript
{
private:
    float       m_fCamSpeed;

public:
    virtual void tick() override;

private:
    void Camera2DMove();
    void Camera3DMove();

    CLONE(CCameraMoveScript);
public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};

