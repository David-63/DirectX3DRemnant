#ifndef _STRUCT
#define _STRUCT


struct tLightColor
{
    float4 vDiffuse; // ���� ����
    float4 vAmbient; // �ֺ� ��(ȯ�� ��)
};

// LightInfo
struct tLightInfo
{
    tLightColor Color; // ���� ����
    
    float4      vWorldPos; // ������ ���� �����̽� ��ġ
    float4      vWorldDir; // ���� ������ ����
    
    uint        LightType; // ���� Ÿ��(���⼺, ��, ����Ʈ)
    float       Radius; // ���� �ݰ�(��Ÿ�)
    float       Angle; // ���� ����    
    int         Padding;
};


// Particle
struct tParticle
{
    float4  vLocalPos;
    float4  vWorldPos; // ��ƼŬ ��ġ
    float4  vWorldScale; // ��ƼŬ ũ��
    float4  vColor; // ��ƼŬ ����
    float4  vVelocity; // ��ƼŬ ���� �ӵ�
    float4  vForce; // ��ƼŬ�� �־��� ��
    float4  vRandomForce; // ��ƼŬ�� ����Ǵ� ���� ��

    float   Age; // ���� �ð�
    float   PrevAge;  // ���� ������ �����ð�
    float   NomalizedAge; // ������ �����ð��� 0~1�� ����ȭ �� ��
    float   LifeTime; // ����
    float   Mass; // ����
    float   ScaleFactor; // �߰� ũ�� ����
    
    int     Active;
    int     pad;
};

struct tRWParticleBuffer
{
    int  iSpawnCount; // ���� ��ų ��ƼŬ ����
    int3  padding;
};

struct tParticleModule
{
    float3 vRandomSpark;

    // ���� ���
    float4 vSpawnColor;
    float4 vSpawnScaleMin;
    float4 vSpawnScaleMax;
    float3 vBoxShapeScale;

    float fSphereShapeRadius;
    float fSphereOffset;
    int SpawnShapeType; // Sphere , Box

    float fSpawnAreaOffsetFactor;
    int SpawnRate;
    int Space; // 0 World, 1 Local    

    float MinLifeTime;
    float MaxLifeTime;
    int3 spawnpad;

    // Color Change ���
    float4 vStartColor; // �ʱ� ����
    float4 vEndColor; // ���� ����
    bool bStrongColor;

    // Scale Change ���
    float StartScale; // �ʱ� ũ��
    float EndScale; // ���� ũ��	

    // ���� �ִ�ũ��
    int iMaxParticleCount;
    int2 ipad;

    // Add Velocity ���
    float4 vVelocityDir;
    int AddVelocityType; // 0 : From Center, 2 : Fixed Direction	
    float OffsetAngle;
    float Speed;
    int addvpad;

    // Drag ���
    float StartDrag;
    float EndDrag;

    // Gravity ���
    float fGravityForce;

    // NoiseForce ���
    float fNoiseTerm;
    float fNoiseForce;

    // Render ���
    int VelocityAlignment; // 1 : �ӵ����� ���(�̵� �������� ȸ��) 0 : ��� ����
    int VelocityScale; // 1 : �ӵ��� ���� ũ�� ��ȭ ���, 0 : ��� ����	
    float vMaxSpeed; // �ִ� ũ�⿡ �����ϴ� �ӷ�
    float4 vMaxVelocityScale; // �ӷ¿� ���� ũ�� ��ȭ�� �ִ�ġ
    float fRotAngle;
    float fRotSpeed;
    int bRot; // 1 : ȸ�� ���, 0: ���x
    int bUseSpark;
    int bEmissive;
    float4 vEmissiveColor;


    // Module Check
    int Spawn;
    int ColorChange;
    int ScaleChange;
    int AddVelocity;

    int Drag;
    int NoiseForce;
    int Render;
    int Gravity;
};

struct tSkinningInfo
{
    float3 vPos;
    float3 vTangent;
    float3 vBinormal;
    float3 vNormal;
};

struct tRaycastOut
{
    float2 vUV;
    float fDist;
    int success;
};

#endif
