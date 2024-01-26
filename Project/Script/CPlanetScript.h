#pragma once
#include <Engine\CScript.h>

class CPlanetScript : public CScript
{
private:
    bool isOrbit = false;
    float radius = 1.f;
    float moveSpeed = 1.0f;
    bool isRotate = true;
    float rotateSpeed = 1.f;
    float accTime = 0.f;
    bool isSatellite = false;
    float moonDistance = 1.f;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetOrbit(bool _isOrbit = true) { isOrbit = _isOrbit; }
    void SetMoveSpeed(float _speed) { moveSpeed = _speed; }

    void SetRotate(bool _isRotate = false) { isRotate = _isRotate; }
    void SetRotateSpeed(float _speed) { rotateSpeed = _speed; }

    void SetSatellite(bool _isSatellite = true) { isSatellite = _isSatellite; }
    void SetRadius(float _radius) { moonDistance = _radius; }
private:
    void orbit();
    void rotationPlanet();
    void doSatellite();
public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CPlanetScript);

    CPlanetScript();
    ~CPlanetScript();
};

