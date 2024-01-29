#ifndef _WEIGHTMAP
#define _WEIGHTMAP

#include "value.fx"
#include "func.fx"

struct tWeight_4
{
    float arrWeight[4];
};

RWStructuredBuffer<tWeight_4> WEIGHT_MAP : register(u0);
StructuredBuffer<tRaycastOut> LOCATION : register(t16); // �귯�� �»�� ��ġ

#define WIDTH       g_int_0
#define HEIGHT      g_int_1

//#define BRUSH_TEX g_texarr_0
#define BRUSH_TEX   g_tex_0
#define BRUSH_SCALE g_vec2_0
#define BRUSH_IDX   g_int_2

#define WEIGHT_IDX  g_int_3

[numthreads(32, 32, 1)]
void CS_WeightMap(int3 _iThreadID : SV_DispatchThreadID)
{
    if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y || !LOCATION[0].success)
    {
        return;
    }

    int2 vCenterPos = float2(WIDTH, HEIGHT) * LOCATION[0].vUV;
    int2 vBrushScale = float2(WIDTH, HEIGHT) * BRUSH_SCALE;
    
    if (_iThreadID.x < vCenterPos.x - (vBrushScale.x / 2) || vCenterPos.x + (vBrushScale.x / 2) < _iThreadID.x
        || _iThreadID.y < vCenterPos.y - (vBrushScale.y / 2) || vCenterPos.y + (vBrushScale.y / 2) < _iThreadID.y)
    {
        return;
    }
    
    // brush texture �� ���� ����
    int2 vLTPos = vCenterPos - (vBrushScale / 2);
    float2 vUV = float2(_iThreadID.xy - vLTPos) / float2(vBrushScale);

    //float4 vBrushColor = BRUSH_TEX.SampleLevel(g_sam_0, float3(vUV, BRUSH_IDX), 0);
    float4 vBrushColor = BRUSH_TEX.SampleLevel(g_sam_0, vUV, 0);

    // 2���� �ε��� ��ǥ�� 1���� �ε����� ���
    uint iIdx = (_iThreadID.y * WIDTH) + _iThreadID.x;

    // ���� ����ġ
    float arrWeight[4] = WEIGHT_MAP[iIdx].arrWeight;

    // ���� ��
    float fAdd = g_DT * vBrushColor.a * 2.f;
    
    // ����ġ�� �ø���, ������ ������ ������ ���߾� ������.
    arrWeight[WEIGHT_IDX] = saturate(arrWeight[WEIGHT_IDX] + fAdd);

    // �ʰ���
    float fTotal = 0.f;
    for (int i = 0; i < 4; ++i)
    {
        fTotal += arrWeight[i];
    }

    // �� ���� 1�� �Ѿ�� ���
    if (1.f < fTotal)
    {
        float fOver = fTotal - 1.f;

        // ������ ����ġ�� ����
        fTotal = fTotal - arrWeight[WEIGHT_IDX];

        for (int j = 0; j < 4; ++j)
        {
            if (j == WEIGHT_IDX)
                continue;

            // �ʰ����� ������ ���߾� �����, �ٸ� ����ġ �ʿ��� ����.
            arrWeight[j] = saturate(arrWeight[j] - fOver * (arrWeight[j] / fTotal));
        }
    }

    // ���ۿ� ���
    WEIGHT_MAP[iIdx].arrWeight = arrWeight;
}

#endif