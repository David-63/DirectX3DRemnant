#ifndef _PARTICLE_RENDER
#define _PARTICLE_RENDER

#include "value.fx"
#include "struct.fx"


// ========================
// Particle Render Shader
// mesh : RectMesh

// Parameter
// g_int_0 : Particle Index
// =========================

StructuredBuffer<tParticle>         ParticleBuffer : register(t20);
StructuredBuffer<tParticleModule>   ParticleModuleData : register(t21);
#define ModuleData                  ParticleModuleData[0]

struct VS_IN
{
    float3 vPos : POSITION;    
    uint iInstID : SV_InstanceID;
};

struct VS_OUT
{
    float3 vPos : POSITION;
    uint iInstID : SV_InstanceID;
};

VS_OUT VS_ParticleRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;      
     
    output.vPos = _in.vPos;
    output.iInstID = _in.iInstID;
    
    return output;
}

// GeometryShader ���
// 1. ���������� ���� (��Ȱ��ȭ�� ��ƼŬ �Ÿ���)
// 2. ������ ó�� (ī�޶� �ٶ󺸴�..)
// 3. �ӵ��� ���� ȸ�� ����
struct GS_OUT
{
    float4  vPosition : SV_Position;
    float2  vUV : TEXCOORD;
    uint    iInstID : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_ParticleRender (point VS_OUT _in[1], inout TriangleStream<GS_OUT> _outstream)
{    
    uint id = _in[0].iInstID;
    
    if (0 == ParticleBuffer[id].Active)
        return;

    float3 vParticleViewPos = mul(float4(ParticleBuffer[id].vWorldPos.xyz, 1.f), g_matView).xyz;
    float2 vParticleScale = ParticleBuffer[id].vWorldScale.xy * ParticleBuffer[id].ScaleFactor;
   
    // 0 -- 1
    // |    |
    // 3 -- 2
    float3 NewPos[4] =
    {
        float3(-vParticleScale.x / 2.f, +vParticleScale.y / 2.f, 0.f),
        float3(+vParticleScale.x / 2.f, +vParticleScale.y / 2.f, 0.f),
        float3(+vParticleScale.x / 2.f, -vParticleScale.y / 2.f, 0.f),
        float3(-vParticleScale.x / 2.f, -vParticleScale.y / 2.f, 0.f)
    };
    
    
    if (ModuleData.Render)
    {        
        if (ModuleData.VelocityScale)
        {
            // ���� ��ƼŬ�� �ӷ��� �˾Ƴ���.
            float fCurSpeed = length(ParticleBuffer[id].vVelocity);            
            if (ModuleData.vMaxSpeed < fCurSpeed)
                fCurSpeed = ModuleData.vMaxSpeed;
            
            // �ִ�ӵ� ��� ���� �ӵ��� ������ ���Ѵ�.
            float fRatio = saturate(fCurSpeed / ModuleData.vMaxSpeed);            
          
            // ������ �´� ũ�⺯ȭ���� ���Ѵ�.
            float3 vDefaultScale = float3(1.f, 1.f, 1.f);
            float3 fScale = vDefaultScale + (ModuleData.vMaxVelocityScale.xyz - vDefaultScale) * fRatio;
                      
            NewPos[0] = NewPos[0] * fScale;
            NewPos[3] = NewPos[3] * fScale;            
        }
        
        
        if (ModuleData.VelocityAlignment)
        {
            // ��ƼŬ ���� ���� �ӵ��� View �������� ��ȯ
            float3 vVelocity = normalize(ParticleBuffer[id].vVelocity.xyz);
            vVelocity = mul(float4(vVelocity, 0.f), g_matView).xyz;
                       
            // ��ƼŬ Right ����� �̵� ������ �����ؼ� �� ������ ������ ���Ѵ�.
            float3 vRight = float3(1.f, 0.f, 0.f);
            float fTheta = acos(dot(vRight, vVelocity));
            
            // ������ ����� �ڻ��� ������ �������� �ϱ� ������, 2���� ���� �ݴ�� ������ �ش�.
            if (vVelocity.y < vRight.y)
            {
                fTheta = (2.f * 3.1415926535f) - fTheta;
            }
            
            // ���� ������ Z �� ȸ�� ����� �����.
            float3x3 matRotZ =
            {
                cos(fTheta),  sin(fTheta),      0,
                -sin(fTheta), cos(fTheta),      0,
                          0,            0,    1.f,
            };
            
            // 4���� ������ ȸ����Ų��.
            for (int i = 0; i < 4; ++i)
            {
                NewPos[i] = mul(NewPos[i], matRotZ);
            }
        }        
    }
    
    
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    output[0].vPosition = mul(float4(NewPos[0] + vParticleViewPos, 1.f), g_matProj);
    output[0].vUV = float2(0.f, 0.f);
    output[0].iInstID = id;
    
    output[1].vPosition = mul(float4(NewPos[1] + vParticleViewPos, 1.f), g_matProj);
    output[1].vUV = float2(1.f, 0.f);
    output[1].iInstID = id;
    
    output[2].vPosition = mul(float4(NewPos[2] + vParticleViewPos, 1.f), g_matProj);
    output[2].vUV = float2(1.f, 1.f);
    output[2].iInstID = id;
    
    output[3].vPosition = mul(float4(NewPos[3] + vParticleViewPos, 1.f), g_matProj);
    output[3].vUV = float2(0.f, 1.f);
    output[3].iInstID = id;
    
    
    // ���� ����
    _outstream.Append(output[0]);
    _outstream.Append(output[1]);
    _outstream.Append(output[2]);
    _outstream.RestartStrip();
    
    _outstream.Append(output[0]);
    _outstream.Append(output[2]);
    _outstream.Append(output[3]);
    _outstream.RestartStrip();
}


float4 PS_ParticleRender(GS_OUT _in) : SV_Target
{   
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if(g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);        
        vOutColor.rgb *= ParticleBuffer[_in.iInstID].vColor.rgb;
    }
    
    return vOutColor;
}


#endif
