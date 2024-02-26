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
    CStructuredBuffer*          m_ParticleBuffer;
    CStructuredBuffer*          m_RWBuffer;
    CStructuredBuffer*          m_ModuleDataBuffer;
    
    tParticleModule             m_ModuleData;
    tRWParticleBuffer		    m_RWPrticleBuffer;
    Ptr<CParticleUpdateShader>  m_UpdateCS; // 파티클 업데이트 셰이더
   
    float                       m_AccTime;
    float                       m_SpawnTime;

    Ptr<CTexture>               m_ParticleTex;
    Ptr<CTexture>               m_NoiseTex;

    bool                        m_bOnceExcute;
    bool                        m_bBursts;
    bool                        m_bLoop;

    



public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CParticleSystem);


public:
    //void ActiveModule(PARTICLE_MODULE _ModuleType) { m_ModuleData.ModuleCheck[(UINT)_ModuleType] = true; }
    //void DeactivateModule(PARTICLE_MODULE _ModuleType) { m_ModuleData.ModuleCheck[(UINT)_ModuleType] = false; }

    //// 초당 생성 개수 설정
    //void SetSpawnRate(int _SpawnRate) { m_ModuleData.SpawnRate = _SpawnRate; }

    //// 생성 시 초기 색상 설정1
    //void SetSpawnInitialColor(Vec3 _vInitialColor) { m_ModuleData.vSpawnColor = _vInitialColor; }

  //  Vec3 CreateRandomDirection(const Vector3& normal, float openAngle);

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

    void SetOnceExcute(bool _bOnceExcute) { m_bOnceExcute = _bOnceExcute; }
    bool GetOnceExcute() const { return m_bOnceExcute; }

    void SetBursts(bool _bBursts) { m_bBursts = _bBursts; }
    bool GetBursts() const { return m_bBursts; }

    void SetSpawnTime(float _fSpawnTime) { m_SpawnTime = _fSpawnTime; }
    float GetSpawnTime() const { return m_SpawnTime; }

    void SetLoop(bool _bLoop) { m_bLoop = _bLoop; }
    bool GetLoop() const { return m_bLoop; }
    Vec3 CreateRandomDirection(const Vector3& normal, float openAngle);




public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _other);
    virtual ~CParticleSystem();
};

