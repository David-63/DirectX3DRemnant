#pragma once
#include "CRenderComponent.h"

#include "ptr.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;
class CParticleUpdateShader;
class CTexture;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    CStructuredBuffer* m_RWBuffer;
    CStructuredBuffer* m_ModuleDataBuffer;

    tParticleModule             m_ModuleData;
    tRWParticleBuffer		    m_RWPrticleBuffer;
    Ptr<CParticleUpdateShader>  m_UpdateCS; // 파티클 업데이트 셰이더

    float                       m_AccTime;
    float                       m_SpawnTime;
    bool                        m_UseTimeSpawn;

    Ptr<CTexture>               m_ParticleTex;
    Ptr<CTexture>               m_NoiseTex;

    bool                        m_bBursts;
    int                         m_bOnceExcute; // 1 : true , 0 : false
    int                         m_iSpawnCount;
    bool                        m_bLoop;
    bool                        m_bUseSpark; // 스파크처럼 튀는거 만들때는 이 옵션을 킬 것
    bool                        m_IsFoward;
    bool                        m_bWantExcute;
    bool                        m_bFinish;

    Ptr<CMesh>                  m_pMesh;
    Ptr<CMaterial>              m_pMtrl;

    wstring                     m_strTexName;
    CGameObject* m_pOwnerObj; // 랜덤스파크 쓸 때 여기에 오브젝트 세팅해줄것





public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;


    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CGameObject* LoadSetting(CGameObject* Obj);

    CLONE(CParticleSystem);


public:
    void UpdateData();

    void SetOwnerObj(CGameObject* _pOwnerObj) { m_pOwnerObj = _pOwnerObj; }
    CGameObject* GetOwnerObj() const { return m_pOwnerObj; }

    // Vec3 CreateRandomDirection(const Vector3& normal, const Vector3& up, const Vector3& Front);
    Vec3 CreateRandomDirection(const Vector3& normal, float openAngle);


    float RandomFloat(float min, float max);

    void SetParticleTex(Ptr<CTexture> _Tex)
    {
        m_ParticleTex = _Tex;
    }

    void SetNoiseTex(Ptr<CTexture> _Tex)
    {
        m_NoiseTex = _Tex;
    }

    void SetModuleData(tParticleModule _ModuleData)
    {
        m_ModuleData = _ModuleData;
    }


    Ptr<CTexture> GetParticleTex() const
    {
        return m_ParticleTex;
    }

    tParticleModule GetModuleData() const
    {
        return m_ModuleData;
    }

    Ptr<CParticleUpdateShader> GetUpdateCS() const
    {
        return m_UpdateCS;
    }

    CStructuredBuffer* GetParticleBuffer() const
    {
        return m_ParticleBuffer;
    }

    void SetBursts(bool _bBursts) { m_bBursts = _bBursts; }
    bool GetBursts() const { return m_bBursts; }

    void SetSpawnTime(float _fSpawnTime) { m_SpawnTime = _fSpawnTime; }
    float GetSpawnTime() const { return m_SpawnTime; }

    void SetLoop(bool _bLoop) { m_bLoop = _bLoop; }
    bool GetLoop() const { return m_bLoop; }


    void SetUseSpark(bool _bUseSpark) { m_bUseSpark = _bUseSpark; }
    bool GetUseSpark() const { return m_bUseSpark; }

    void SetFoward(bool _IsFoward) { m_IsFoward = _IsFoward; }
    bool IsFoward() const { return m_IsFoward; }

    void SetParticleBufferSize();

    void SetTexName(const wstring& _strTexName) { m_strTexName = _strTexName; }
    wstring GetTexName() const { return m_strTexName; }


    void SetUseAnim(bool _bUseAnimation) { m_ModuleData.AnimUse = _bUseAnimation; }
    bool GetUseAnim() const { return m_ModuleData.AnimUse; }

    void SetLoopAnim(bool _bLoopAnim) { m_ModuleData.AnimLoop = _bLoopAnim; }
    bool GetLoopAnim() const { return m_ModuleData.AnimLoop; }

    void SetAnimXCount(int _iAnimXCount) { m_ModuleData.iAnimXCount = _iAnimXCount; }
    int GetAnimXCount() const { return m_ModuleData.iAnimXCount; }

    void SetAnimYCount(int _iAnimYCount) { m_ModuleData.iAnimYCount = _iAnimYCount; }
    int GetAnimYCount() const { return m_ModuleData.iAnimYCount; }

    void SetAnimFrmTime(float _fAnimFrmTime) { m_ModuleData.fAnimFrmTime = _fAnimFrmTime; }
    float GetAnimFrmTime() const { return m_ModuleData.fAnimFrmTime; }

    void SetExcute(bool _bExcute) { m_bWantExcute = _bExcute; }
    bool GetExcute() const { return m_bWantExcute; }

    void SetTimeSpawn(bool _bUseTimeSpawn) { m_UseTimeSpawn = _bUseTimeSpawn; }
    bool GetTimeSpawn() const { return m_UseTimeSpawn; }

    bool isFinish() const { return m_bFinish; }



public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _other);
    virtual ~CParticleSystem();
};

