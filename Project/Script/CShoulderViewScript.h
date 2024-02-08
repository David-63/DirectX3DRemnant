#pragma once
#include <Engine\CScript.h>

enum class VIEW_STATE
{
    NORMAL,
    AIMING,
};

struct tInt2
{
    int x;
    int y;
};


class CShoulderViewScript :
    public CScript
{
private:
    VIEW_STATE  m_eViewState;

    CGameObject* m_Target;
    Vector3      m_vCamPivot;


    bool        m_firstTick;

    Vector2     m_vPitchMinMax;  // x = Uppear, y = Lower(값이 낮을수록 땅을 뚫고 볼 확률 낮아짐)
    float       m_fMouseSensitivity;

    bool        m_bIsMouseRock;


    float       m_OriginFov;
    float       m_ModifiedFov;
    float       m_CurrentFov;

    float       m_fCamSpeed;




public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

public:
    tInt2 CalcMouseMovement();
    void Input();

    void SetCamTarget(CGameObject* _pTarget) { m_Target = _pTarget; }
    void SmoothCamRot(Vec3 _vCamRot);

    void MouseCenterLock();
    void CameraRot();
    void CameraPos();
    void CharacterRot();


    void ChangeStateFov();


    // Speed 값은 0~1 사이의 값이여야함.
    // 0에 가까울수록 Start값에 가까워지고, 1에 가까울수록 End값에 가까워짐.
    float Lerp(float _Start, float _End, float _Transition);






private:

    CLONE(CShoulderViewScript);


public:
    CShoulderViewScript();
    ~CShoulderViewScript();

};

