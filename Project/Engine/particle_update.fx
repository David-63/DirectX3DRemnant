#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"
#include "func.fx"

RWStructuredBuffer<tParticle>       ParticleBuffer : register(u0);
RWStructuredBuffer<int4>            ParticleSpawnCount : register(u1);
StructuredBuffer<tParticleModule>   ParticleModuleData : register(t20);
Texture2D                           NoiseTexture : register(t21);


#define ObjectPos           g_vec4_0

#define NoiseTexResolution  g_vec2_0

#define SpawnCount          ParticleSpawnCount[0].x
#define ModuleData          ParticleModuleData[0]
#define ParticleMaxCount    ParticleModuleData[0].iMaxParticleCount

#define SpawnModule         ParticleModuleData[0].Spawn
#define ColorChangeModule   ParticleModuleData[0].ColorChange
#define ScaleChangeModule   ParticleModuleData[0].ScaleChange


[numthreads(128, 1, 1)]
void CS_ParticleUpdate(int3 _ID : SV_DispatchThreadID)
{
    // ������ ID �� ��ƼŬ���� �ִ� ���� �ѱ��� or ������ ��� ��ƼŬ�� ��Ȱ��ȭ ������ ���
    if (ParticleMaxCount <= _ID.x)
        return;
        
    tParticle particle = ParticleBuffer[_ID.x];
           
    if (SpawnModule)
    {
        // ��ƼŬ�� ��Ȱ��ȭ ������ ���
        if (particle.Active == 0)
        {
            // SpawnCount �� Ȯ��
            // ���� SpawnCount �� 0 �̻��̶��, ��ƼŬ�� Ȱ��ȭ��Ŵ      
            while (0 < SpawnCount)
            {
                int orgvalue = SpawnCount;
                int outvalue = 0;
                InterlockedCompareExchange(SpawnCount, orgvalue, SpawnCount - 1, outvalue);
            
                if (orgvalue == outvalue)
                {   
                    particle.Active = 1;
                    
                    // ���� ����� ���� ����
                    float3 vOut1 = (float3) 0.f;
                    float3 vOut2 = (float3) 0.f;
                    float3 vOut3 = (float3) 0.f;
                    
                    // ��ü ��ȿ �������� ���̵� 0 ~ 1 �� ����ȭ
                    float fNormalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID, vOut1);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID + 0.1f, vOut2);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID + 0.2f, vOut3);
                    
                    // Box ����
                    if (ModuleData.SpawnShapeType == 0)
                    {                           
                        particle.vLocalPos.xyz = float3(ModuleData.vBoxShapeScale.x * vOut1.r - ModuleData.vBoxShapeScale.x * 0.5f
                                                      , ModuleData.vBoxShapeScale.y * vOut2.r - ModuleData.vBoxShapeScale.y * 0.5f
                                                      , 0.f);//ModuleData.vBoxShapeScale.z * vOut3.r - ModuleData.vBoxShapeScale.z * 0.5f);
                        particle.vWorldPos.xyz = particle.vLocalPos.xyz + ObjectPos.xyz;
                        
                        
                        // ���� ũ�� ���������� ���� ũ��� ���� (Min, Max �� ��ġ�ϸ� ����ũ��)
                        float4 vSpawnScale = ModuleData.vSpawnScaleMin + (ModuleData.vSpawnScaleMax - ModuleData.vSpawnScaleMin) * vOut3.x;                                                
                        particle.vWorldScale.xyz = vSpawnScale.xyz;
                    }
                    
                    // Sphere ����
                    else if (ModuleData.SpawnShapeType == 1)
                    {
                        float fRadius = 500.f; //vOut1.r * 200.f;
                        float fAngle = vOut2.r * 2 * 3.1415926535f;
                        //particle.vWorldPos.xyz = float3(fRadius * cos(fAngle), fRadius * sin(fAngle), 100.f);
                    }
                    
                    // ��ƼŬ ���� ����
                    particle.Mass = 1.f;                    
                    
                    
                    // AddVelocity ���
                    if (ModuleData.AddVelocity)
                    {
                        // From Center
                        if (ModuleData.AddVelocityType == 0)
                        {
                            float3 vVelocity = normalize(particle.vLocalPos.xyz);
                            particle.vVelocity.xyz = vVelocity * ModuleData.Speed;
                        }
                        
                        // To Center
                        else if (ModuleData.AddVelocityType == 1)
                        {
                            float3 vVelocity = -normalize(particle.vLocalPos.xyz);
                            particle.vVelocity.xyz = vVelocity * ModuleData.Speed;
                        }
                        
                        // Fixed Direction
                        else
                        {
                            particle.vVelocity.xyz = normalize(ModuleData.vVelocityDir.xyz) * ModuleData.Speed;
                        }
                    }                    
                    
                   
                    
                    particle.vColor = ModuleData.vSpawnColor;                                      
                    particle.Age = 0.f;
                    particle.LifeTime = ModuleData.MinLifeTime + (ModuleData.MaxLifeTime - ModuleData.MinLifeTime) * vOut2.r;
                    break;
                }
            }
        }
    }
    
    // ��ƼŬ�� Ȱ��ȭ�� ���
    if(particle.Active)
    {
        // ��ƼŬ�� Age �� �ð��� ������Ŵ
        particle.PrevAge = particle.Age;
        particle.Age += g_DT;
        particle.NomalizedAge = saturate(particle.Age / particle.LifeTime);        
        particle.vForce.xyz = (float3) 0.f;
        
        
        // ��ƼŬ�� ������ ������, �ٽ� ��Ȱ��ȭ ���·� �ǵ���
        if (particle.LifeTime <= particle.Age)
        {
            particle.Active = 0.f;
        }
                
        // NoiseForce ��� (�������� ��) ���� ���
        if (ModuleData.NoiseForce)
        {            
            if (particle.PrevAge == 0.f)
            {
                 // ���� ����� ���� ����
                float3 vOut1 = (float3) 0.f;
                float3 vOut2 = (float3) 0.f;
                float3 vOut3 = (float3) 0.f;
                    
                // ��ü ��ȿ �������� ���̵� 0 ~ 1 �� ����ȭ
                float fNormalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID, vOut1);
                GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID + 0.1f, vOut2);
                GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID + 0.2f, vOut3);
                
                float3 vForce = normalize(float3(vOut1.x, vOut2.x, vOut1.z));
                particle.vRandomForce.xyz = vForce * ModuleData.fNoiseForce;
            }
            else
            {
                int Age = int(particle.Age * (1.f / ModuleData.fNoiseTerm));
                int PrevAge = int(particle.PrevAge * (1.f / ModuleData.fNoiseTerm));

                // ������ ������ �Ѿ ����, ���ο� ���� Force �� �ش�.
                if (Age != PrevAge)
                {
                    // ���� ����� ���� ����
                    float3 vOut1 = (float3) 0.f;
                    float3 vOut2 = (float3) 0.f;
                    float3 vOut3 = (float3) 0.f;
                    
                    // ��ü ��ȿ �������� ���̵� 0 ~ 1 �� ����ȭ
                    float fNormalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID, vOut1);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID + 0.1f, vOut2);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID + 0.2f, vOut3);
                
                    float3 vForce = normalize(float3(vOut1.x, vOut2.x, vOut1.z) * 2.f - 1.f);
                    particle.vRandomForce.xyz = vForce * ModuleData.fNoiseForce;
                }
            }   
            
            particle.vForce.xyz += particle.vRandomForce.xyz;
        }                
       
        // ��ƼŬ�� ���� ���� �� ���, ���� ���� �ӵ��� ��ȭ�� ���
        float3 vAccel = particle.vForce.xyz / particle.Mass;
        particle.vVelocity.xyz += vAccel * g_DT; 
        
        
        // �ӵ� ����(Drag) ���
        if (ModuleData.Drag)
        {
            // ��ƼŬ�� ���� �ӷ�
            float Speed = length(particle.vVelocity);
            float fDrag = ModuleData.StartDrag + (ModuleData.EndDrag - ModuleData.StartDrag) * particle.NomalizedAge;
            
            // �ӵ��� �ݴ�� �������°� ����
            if(fDrag <= 0.f)
                fDrag = 0.001f;
            
            if (fDrag < Speed)
            {
                particle.vVelocity = normalize(particle.vVelocity) * fDrag;
            }
        }                
        
        // �ӵ��� ���� ��ƼŬ��ġ �̵�
        // Sim ��ǥ�迡 ���� �̵���� �б�
        if (ModuleData.Space == 0)
        {                        
            particle.vWorldPos += particle.vVelocity * g_DT;
        }
        else if(ModuleData.Space == 1)
        {
            particle.vLocalPos += particle.vVelocity * g_DT;
            particle.vWorldPos.xyz = particle.vLocalPos.xyz + ObjectPos.xyz;
        }
        
        
        // ũ�� ��ȭ ����� Ȱ��ȭ �Ǿ�������
        if(ModuleData.ScaleChange)
            particle.ScaleFactor = ModuleData.StartScale + particle.NomalizedAge * (ModuleData.EndScale - ModuleData.StartScale);                    
        else
            particle.ScaleFactor = 1.f;
        
        
        // ���� ��ȭ����� Ȱ��ȭ �Ǿ�������
        if(ModuleData.ColorChange)
        {
            particle.vColor = ModuleData.vStartColor + particle.NomalizedAge * (ModuleData.vEndColor - ModuleData.vStartColor);
        }               
        
    }    
    
    // ������ ����
    ParticleBuffer[_ID.x] = particle;
}

#endif



