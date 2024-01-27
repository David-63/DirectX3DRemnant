#ifndef _LANDSCAPE
#define _LANDSCAPE

#include "value.fx"
#include "func.fx"
#include "struct.fx"

// ========================
// LandScape
#define FaceX           g_int_0
#define FaceZ           g_int_1
#define ColorTexture    g_tex_0
#define NormalTexture   g_tex_1
#define HeightMap       g_tex_2
// ========================

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_LandScape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vLocalPos = _in.vPos;
    output.vUV = _in.vUV;

    return output;
}

// ===========
// Hull Shader
// ===========
// Patch Constant Function
// ��ġ�� ���� Ƚ���� �����ϴ� �Լ�
struct PatchOutput
{
    float Edges[3] : SV_TessFactor;
    float Inside : SV_InsideTessFactor;
};

// LOD Option
PatchOutput PatchConstFunc(InputPatch<VS_OUT, 3> _input
    , uint PatchID : SV_PrimitiveID)
{
    PatchOutput output = (PatchOutput) 0.f;

    output.Edges[0] = 16;
    output.Edges[1] = 16;
    output.Edges[2] = 16;
    output.Inside = 16;

    return output;
}

struct HS_OUT
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

[domain("tri")]
[partitioning("integer")]   // ����, �Ǽ�
//[partitioning("fractional_odd")] // ����, �Ǽ�
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstFunc")]
[maxtessfactor(32)]
HS_OUT HS_LandScape(InputPatch<VS_OUT, 3> _input
    , uint i : SV_OutputControlPointID
    , uint PatchID : SV_PrimitiveID)
{
    HS_OUT output = (HS_OUT) 0.f;

    output.vLocalPos = _input[i].vLocalPos;
    output.vUV = _input[i].vUV;

    return output;
}

struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;

    float3 vViewPos : POSITION;
    float3 vViewNormal : NORMAL;
    float3 vViewTangent : TANGENT;
    float3 vViewBinormal : BINORMAL;
};


[domain("tri")]
DS_OUT DS_LandScape(const OutputPatch<HS_OUT, 3> _origin
    , float3 _vWeight : SV_DomainLocation
    , PatchOutput _patchtess)
{
    DS_OUT output = (DS_OUT) 0.f;

    float3 vLocalPos = (float3) 0.f;
    float2 vUV = (float2) 0.f;

    for (int i = 0; i < 3; ++i)
    {
        vLocalPos += _origin[i].vLocalPos * _vWeight[i];
        vUV += _origin[i].vUV * _vWeight[i];
    }

    // ���̸� ����
    float2 vHeightMapUV = vUV / float2(FaceX, FaceZ);
    vLocalPos.y = HeightMap.SampleLevel(g_sam_0, vHeightMapUV, 0).x;

    // Normal, Tangent, Binormal �� ���
    // ������ ���̴� ������ �ֺ�(��, �Ʒ�, ��, ��) �� �����ϱ� ���� ����
    float fLocalStep = 1.f / _patchtess.Inside;
    float2 vUVStep = fLocalStep / float2(FaceX, FaceZ);

    // ������ ���� �ֺ� ���� ��ġ��(���� ��ǥ��) ���ϱ�
    float3 vUp = mul(float4(vLocalPos.x, HeightMap.SampleLevel(g_sam_0, vHeightMapUV + float2(0.f, -vUVStep.y), 0).x, vLocalPos.z + fLocalStep, 1.f), g_matWorld).xyz;
    float3 vDown = mul(float4(vLocalPos.x, HeightMap.SampleLevel(g_sam_0, vHeightMapUV + float2(0.f, +vUVStep.y), 0).x, vLocalPos.z - fLocalStep, 1.f), g_matWorld).xyz;
    float3 vLeft = mul(float4(vLocalPos.x - fLocalStep, HeightMap.SampleLevel(g_sam_0, vHeightMapUV + float2(-vUVStep.x, 0.f), 0).x, vLocalPos.z, 1.f), g_matWorld).xyz;
    float3 vRight = mul(float4(vLocalPos.x + fLocalStep, HeightMap.SampleLevel(g_sam_0, vHeightMapUV + float2(+vUVStep.x, 0.f), 0).x, vLocalPos.z, 1.f), g_matWorld).xyz;

    // ���� ���� ���ϱ�
    float3 vTangent = normalize(vRight - vLeft);
    float3 vBinormal = normalize(vDown - vUp);
    float3 vNormal = normalize(cross(vTangent, vBinormal));

    // ������ǥ��
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;

    output.vViewPos = mul(float4(vLocalPos, 1.f), g_matWV).xyz;
    output.vViewNormal = normalize(mul(float4(vNormal, 0.f), g_matView)).xyz;
    output.vViewTangent = normalize(mul(float4(vTangent, 0.f), g_matView)).xyz;
    output.vViewBinormal = normalize(mul(float4(vBinormal, 0.f), g_matView)).xyz;

    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
    float4 vData : SV_Target3;
    float4 vEmissive : SV_Target4;
};

PS_OUT PS_LandScape(DS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;

    output.vColor = float4(0.8f, 0.8f, 0.8f, 1.f);
    output.vNormal = float4(_in.vViewNormal, 1.f);
    output.vPosition = float4(_in.vViewPos, 1.f);
    output.vData;
    output.vEmissive;
    
    return output;
}

#endif
