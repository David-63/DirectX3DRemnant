#ifndef _LANDSCAPE
#define _LANDSCAPE

#include "value.fx"
#include "func.fx"
#include "struct.fx"


// ========================
// LandScape
#define                     FaceX                           g_int_0
#define                     FaceZ                           g_int_1

#define                     ColorTexture                    g_tex_0
#define                     NormalTexture                   g_tex_1
#define                     HeightMap                       g_tex_2

#define                     TileCount                       g_float_1   // �迭 ����
#define                     WeightMapResolution             g_vec2_1    // ����ġ ���� �ػ�


#define                     CamWorldPos                     g_vec4_0.xyz// ī�޶� ���� ��ǥ
#define                     TileTexArr                      g_arrtex_0  // Tile �迭 �ؽ���
StructuredBuffer<float4>    WEIGHT_MAP : register(t17);                 // ����ġ ����
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
    
    float3 vWorldPos : POSITION1;
};

VS_OUT VS_LandScape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vLocalPos = _in.vPos;
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    
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

PatchOutput PatchConstFunc(InputPatch<VS_OUT, 3> _input, uint PatchID : SV_PrimitiveID)
{
    PatchOutput output = (PatchOutput) 0.f;
    
    float3 vUpDown = (_input[1].vWorldPos + _input[2].vWorldPos) / 2.f;
    float3 vLeftRight = (_input[0].vWorldPos + _input[2].vWorldPos) / 2.f;
    float3 vSlide = (_input[0].vWorldPos + _input[1].vWorldPos) / 2.f;
    float3 vMid = (_input[0].vWorldPos + _input[1].vWorldPos + _input[2].vWorldPos) / 3.f;
        
    float3 vCamWorldPos = CamWorldPos;
    vCamWorldPos.y = 0.f;
    
    output.Edges[0] = pow(2, (int) GetTessFactor(distance(vCamWorldPos, vUpDown), 1, 4, 2000.f, 20000.f));
    output.Edges[1] = pow(2, (int) GetTessFactor(distance(vCamWorldPos, vLeftRight), 1, 4, 2000.f, 20000.f));
    output.Edges[2] = pow(2, (int) GetTessFactor(distance(vCamWorldPos, vSlide), 1, 4, 2000.f, 20000.f));
    output.Inside = pow(2, (int) GetTessFactor(distance(vCamWorldPos, vMid), 1, 4, 2000.f, 20000.f));

    return output;
}

struct HS_OUT
{
    float3 vLocalPos : POSITION;
    float2 vUV       : TEXCOORD;
};

[domain("tri")]
[partitioning("integer")]   // ����, �Ǽ�
//[partitioning("fractional_odd")] // ����, �Ǽ�
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstFunc")]
[maxtessfactor(32)]
HS_OUT HS_LandScape(InputPatch<VS_OUT, 3> _input, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID)
{
    HS_OUT output = (HS_OUT) 0.f;

    output.vLocalPos = _input[i].vLocalPos;
    output.vUV = _input[i].vUV;

    return output;
}

struct DS_OUT
{
    float4 vPosition     : SV_Position;
    float2 vUV           : TEXCOORD;
    float2 vFullUV       : TEXCOORD1;

    float3 vViewPos      : POSITION;
    float3 vViewNormal   : NORMAL;
    float3 vViewTangent  : TANGENT;
    float3 vViewBinormal : BINORMAL;
};


[domain("tri")]
DS_OUT DS_LandScape(const OutputPatch<HS_OUT, 3> _origin, float3 _vWeight : SV_DomainLocation, PatchOutput _patchtess)
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
    output.vFullUV = vHeightMapUV;

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
    
    float3 vViewNormal = _in.vViewNormal;
    
    // Ÿ�� �迭 �ε����� �ִٸ�
    output.vColor = float4(0.7f, 0.7f, 0.7f, 1.f);
    
    if (g_btexarr_0)
    {
        float2 derivX = ddx(_in.vUV); // �����ȼ��� x�� ��̺а��� ���Ѵ�
        float2 derivY = ddy(_in.vUV); // �����ȼ��� y�� ��̺а��� ���Ѵ�

        // Ÿ�� ����
        int2 iWeightIdx = (int2) (_in.vFullUV * WeightMapResolution);
        float4 vWeight = WEIGHT_MAP[iWeightIdx.y * (int) WeightMapResolution.x + iWeightIdx.x];
        float4 vColor = (float4) 0.f;

        int iMaxWeightIdx = -1;
        float fMaxWeight = 0.f;

        // SampleGrad �Լ��� ���ڷ� ��̺� ���� ������ �ڵ����� mipLevel�� �´� �ؽ��ĸ� �Ҵ���
        
        for (int i = 0; i < TileCount; ++i)
        {
            // �迭 �ؽ��ĸ� ���ø��� �� UV�� 3��° ���� �迭�ε���
            vColor += TileTexArr.SampleGrad(g_sam_0, float3(_in.vUV, i), derivX, derivY) * vWeight[i];
            //vColor += TileTexArr.SampleLevel(g_sam_0, float3(_in.vUV, i), 0) * vWeight[i];

            if (fMaxWeight < vWeight[i])
            {
                fMaxWeight = vWeight[i];
                iMaxWeightIdx = i;
            }
        }
       
        output.vColor = float4(vColor.rgb, 1.f);

        // Ÿ�� �븻
        if (-1 != iMaxWeightIdx)
        {            
            float3 vTangentSpaceNormal = TileTexArr.SampleGrad(g_sam_0, float3(_in.vUV, iMaxWeightIdx + TileCount), derivX, derivY).xyz;
            //float3 vTangentSpaceNormal = TileTexArr.SampleLevel(g_sam_0, float3(_in.vUV, iMaxWeightIdx + TileCount), 0).xyz;
            vTangentSpaceNormal = vTangentSpaceNormal * 2.f - 1.f;

            float3x3 matTBN = { _in.vViewTangent, _in.vViewBinormal, _in.vViewNormal };
            vViewNormal = normalize(mul(vTangentSpaceNormal, matTBN));
        }
    }

    output.vNormal = float4(_in.vViewNormal, 1.f);
    output.vPosition = float4(_in.vViewPos, 1.f);
    output.vData;
    output.vEmissive;
    
    return output;
}

#endif
