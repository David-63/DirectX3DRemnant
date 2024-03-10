#ifndef _ANIMATION
#define _ANIMATION

#include "value.fx"

/*
여기서 수행하는 역할

1. 전체 애니메이션 뼈 행렬 정보를 가져와서, 현재 프레임에 해당하는 BoneOffset, BoneFrameData 정보를 가져옴

2. 원소 단위로 존재하는 행렬 정보를 조립함 (Scale, Transform, Rotate)

3. outputBuffer에 담아서 반환
*/




float4 VectorLess(float4 _vQ1, float4 _vQ2)
{
    float4 vReturn =
    {
        (_vQ1[0] < _vQ2[0]) ? asfloat((uint) 0xFFFFFFFF) : 0.f,
        (_vQ1[1] < _vQ2[1]) ? asfloat((uint) 0xFFFFFFFF) : 0.f,
        (_vQ1[2] < _vQ2[2]) ? asfloat((uint) 0xFFFFFFFF) : 0.f,
        (_vQ1[3] < _vQ2[3]) ? asfloat((uint) 0xFFFFFFFF) : 0.f
    };

    return vReturn;
}

void VectorPermute(uint PermuteX, uint PermuteY, uint PermuteZ, uint PermuteW
    , in float4 V1, in float4 V2
    , out float4 _vOut)
{
    float4 aPtr[2] = { V1, V2 };
    float4 Result = (float4) 0.f;

    const uint i0 = PermuteX & 3;
    const uint vi0 = PermuteX >> 2;
    Result[0] = aPtr[vi0][i0];

    const uint i1 = PermuteY & 3;
    const uint vi1 = PermuteY >> 2;
    Result[1] = aPtr[vi1][i1];

    const uint i2 = PermuteZ & 3;
    const uint vi2 = PermuteZ >> 2;
    Result[2] = aPtr[vi2][i2];

    const uint i3 = PermuteW & 3;
    const uint vi3 = PermuteW >> 2;
    Result[3] = aPtr[vi3][i3];

    _vOut = Result;
}


float4 VectorShiftLeft(in float4 _V1, in float4 _V2, uint _Elements)
{
    float4 vOut = (float4) 0.f;

    VectorPermute(_Elements, ((_Elements) + 1), ((_Elements) + 2), ((_Elements) + 3), _V1, _V2, vOut);

    return vOut;
}

float4 VectorSelect(float4 _vQ1, float4 _vQ2, float4 _vControl)
{
    uint4 iQ1 = asuint(_vQ1);
    uint4 iQ2 = asuint(_vQ2);
    uint4 iControl = asuint(_vControl);

    int4 iReturn =
    {
        (iQ1[0] & ~iControl[0]) | (iQ2[0] & iControl[0]),
        (iQ1[1] & ~iControl[1]) | (iQ2[1] & iControl[1]),
        (iQ1[2] & ~iControl[2]) | (iQ2[2] & iControl[2]),
        (iQ1[3] & ~iControl[3]) | (iQ2[3] & iControl[3]),
    };

    return asfloat(iReturn);
}

float4 VectorXorInt(float4 _V1, float4 _V2)
{
    uint4 iV1 = { asuint(_V1.x), asuint(_V1.y), asuint(_V1.z), asuint(_V1.w) };
    uint4 iV2 = { 2147483648, 0, 0, 0 };

    uint4 Result =
    {
        iV1[0] ^ iV2[0],
        iV1[1] ^ iV2[1],
        iV1[2] ^ iV2[2],
        iV1[3] ^ iV2[3]
    };

    return float4(asfloat(Result.x), asfloat(Result.y), asfloat(Result.z), asfloat(Result.w));
}


void MatrixRotationQuaternion(in float4 Quaternion, out matrix _outMat)
{
    float4 Constant1110 = float4(1.f, 1.f, 1.f, 0.f);

    float4 Q0 = Quaternion + Quaternion;
    float4 Q1 = Quaternion * Q0;

    float4 V0 = (float4) 0.f;
    VectorPermute(1, 0, 0, 7, Q1, Constant1110, V0);

    float4 V1 = (float4) 0.f;
    VectorPermute(2, 2, 1, 7, Q1, Constant1110, V1);

    float4 R0 = Constant1110 - V0;
    R0 = R0 - V1;

    V0 = float4(Quaternion[0], Quaternion[0], Quaternion[1], Quaternion[3]);
    V1 = float4(Q0[2], Q0[1], Q0[2], Q0[3]);
    V0 = V0 * V1;

    V1 = float4(Quaternion.w, Quaternion.w, Quaternion.w, Quaternion.w);
    float4 V2 = float4(Q0[1], Q0[2], Q0[0], Q0[3]);
    V1 = V1 * V2;

    float4 R1 = V0 + V1;
    float4 R2 = V0 - V1;

    VectorPermute(1, 4, 5, 2, R1, R2, V0);
    VectorPermute(0, 6, 0, 6, R1, R2, V1);

    matrix M = (matrix) 0.f;
    VectorPermute(0, 4, 5, 3, R0, V0, M._11_12_13_14);
    VectorPermute(6, 1, 7, 3, R0, V0, M._21_22_23_24);
    VectorPermute(4, 5, 2, 3, R0, V1, M._31_32_33_34);
    M._41_42_43_44 = float4(0.f, 0.f, 0.f, 1.f);
    _outMat = M;
}


float4x4 inverse(float4x4 m)
{
    float det = determinant(m);
    if (det == 0.0f)
    {
        // 역행렬이 존재하지 않음
        return float4x4(0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f);
    }
    else
    {
        // 전치행렬과 행렬식의 역수를 곱하여 역행렬 계산
        return transpose(m) / det;
    }
}

void MatrixAffineTransformation(in float4 Scaling
    , in float4 RotationOrigin
    , in float4 RotationQuaternion
    , in float4 Translation
    , out matrix _outMat)
{
    matrix MScaling = (matrix) 0.f;
    MScaling._11_22_33 = Scaling.xyz;

    float4 VRotationOrigin = float4(RotationOrigin.xyz, 0.f);
    float4 VTranslation = float4(Translation.xyz, 0.f);

    matrix MRotation = (matrix) 0.f;
    MatrixRotationQuaternion(RotationQuaternion, MRotation);

    matrix M = MScaling;
    M._41_42_43_44 = M._41_42_43_44 - VRotationOrigin;
    M = mul(M, MRotation);
    M._41_42_43_44 = M._41_42_43_44 + VRotationOrigin;
    M._41_42_43_44 = M._41_42_43_44 + VTranslation;
    _outMat = M;
}

void MatrixAffineTransformation(in float4 Scaling
    , in float4 RotationOrigin
    , in float4 RotationQuaternion
    , in matrix AdditionalRotation
    , in matrix targetIndexMatrix
    , in float4 Translation
    , out matrix _outMat)
{
    // 메트릭스는 (크기, 자전, 이동) 으로 구성됨
    // 특정 뼈에 회전을 주고 싶다면
    // 1 - 전체 행렬 구하기
    matrix MScaling = (matrix) 0.f;
    MScaling._11_22_33 = Scaling.xyz;

    float4 VRotationOrigin = float4(RotationOrigin.xyz, 0.f);
    float4 VTranslation = float4(Translation.xyz, 0.f);

    matrix MRotation = (matrix) 0.f;
    MatrixRotationQuaternion(RotationQuaternion, MRotation);
    
    // totalM은 현재 뼈의 최종 행렬
    matrix totalM = MScaling;
    totalM._41_42_43_44 = totalM._41_42_43_44 - VRotationOrigin;
    totalM = mul(totalM, MRotation);
    totalM._41_42_43_44 = totalM._41_42_43_44 + VRotationOrigin;
    totalM._41_42_43_44 = totalM._41_42_43_44 + VTranslation;
    
    // 2 - 최종 행렬에서 기준 메트릭스의 역행렬을 적용하여 기준 이후의 뼈 행렬을 구함
    matrix inverseTargetMat = inverse(targetIndexMatrix);
    matrix partMat = mul(totalM, inverseTargetMat);
    
    // 3 - 회전 행렬을 적용
    partMat = mul(partMat, AdditionalRotation);
    partMat = mul(partMat, targetIndexMatrix);
    
    _outMat = partMat;
}

float4 QuternionLerp(in float4 _vQ1, in float4 _vQ2, float _fRatio)
{
    float4 vT = float4(_fRatio, _fRatio, _fRatio, _fRatio);

    // Result = Q1 * sin((1.0 - t) * Omega) / sin(Omega) + Q2 * sin(t * Omega) / sin(Omega)
    const float4 OneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };

    float fQDot = dot(_vQ1, _vQ2);
    float4 CosOmega = float4(fQDot, fQDot, fQDot, fQDot);

    const float4 Zero = (float4) 0.f;
    float4 Control = VectorLess(CosOmega, Zero);
    float4 Sign = VectorSelect(float4(1.f, 1.f, 1.f, 1.f), float4(-1.f, -1.f, -1.f, -1.f), Control);

    CosOmega = CosOmega * Sign;
    Control = VectorLess(CosOmega, OneMinusEpsilon);

    float4 SinOmega = float4(1.f, 1.f, 1.f, 1.f) - (CosOmega * CosOmega);
    SinOmega = float4(sqrt(SinOmega.x), sqrt(SinOmega.y), sqrt(SinOmega.z), sqrt(SinOmega.w));

    float4 Omega = float4(atan2(SinOmega.x, CosOmega.x)
        , atan2(SinOmega.y, CosOmega.y)
        , atan2(SinOmega.z, CosOmega.z)
        , atan2(SinOmega.w, CosOmega.w));

    float4 SignMask = float4(asfloat(0x80000000U), asfloat(0x80000000U), asfloat(0x80000000U), asfloat(0x80000000U));
    float4 V01 = VectorShiftLeft(vT, Zero, 2);
    SignMask = VectorShiftLeft(SignMask, Zero, 3);

    V01 = VectorXorInt(V01, SignMask);
    V01 = float4(1.0f, 0.0f, 0.0f, 0.0f) + V01;

    float4 InvSinOmega = float4(1.f, 1.f, 1.f, 1.f) / SinOmega;

    float4 S0 = V01 * Omega;
    S0 = float4(sin(S0.x), sin(S0.y), sin(S0.z), sin(S0.w));
    S0 = S0 * InvSinOmega;
    S0 = VectorSelect(V01, S0, Control);

    float4 S1 = float4(S0.y, S0.y, S0.y, S0.y);
    S0 = float4(S0.x, S0.x, S0.x, S0.x);

    S1 = S1 * Sign;

    float4 Result = _vQ1 * S0;
    Result = (_vQ2 * S1) + Result;

    return Result;
}

float4 EulerToQuaternion(float3 euler)
{
    float c1 = cos(euler.z / 2);
    float s1 = sin(euler.z / 2);
    float c2 = cos(euler.y / 2);
    float s2 = sin(euler.y / 2);
    float c3 = cos(euler.x / 2);
    float s3 = sin(euler.x / 2);
    float c1c2 = c1 * c2;
    float s1s2 = s1 * s2;
    float4 q;
    q.x = c1c2 * s3 + s1s2 * c3;
    q.y = s1 * c2 * c3 + c1 * s2 * s3;
    q.z = c1 * s2 * c3 - s1 * c2 * s3;
    q.w = c1c2 * c3 - s1s2 * s3;
    return q;
}


struct tFrameTrans
{
    float4 vTranslate;
    float4 vScale;
    float4 qRot;
};

struct tAnimIndices
{
    uint idx;
    uint targetIdx;
    uint padd2;
    uint padd3;
};

StructuredBuffer<tFrameTrans> g_arrFrameTrans : register(t16);
StructuredBuffer<matrix> g_arrOffset : register(t17);
StructuredBuffer<tFrameTrans> g_arrFrameTrans_next : register(t18);
StructuredBuffer<tAnimIndices> g_modifyIndices : register(t19);

RWStructuredBuffer<matrix> g_arrFinelMat : register(u0);

// ===========================
// Animation3D Compute Shader
#define BoneCount       g_int_0
#define CurFrame        g_int_1
#define NextFrame       g_int_2
#define Ratio           g_float_0
#define RotScalar       g_float_1
#define ModifyActive    g_iModifyUse
#define ModifyCnt       g_iModifyCount
// ===========================
[numthreads(256, 1, 1)]
void CS_Animation3D(int3 _iThreadIdx : SV_DispatchThreadID)
{
    if (BoneCount <= _iThreadIdx.x)
        return;

    // _iThreadIdx.x : 1차원 배열 형태의 스레드를 통해서 각 스레드마다 한개의 뼈를 담당하여 연산    
    float4 vQZero = float4(0.f, 0.f, 0.f, 1.f);
    float4 vtQZero = float4(0.f, 0.f, 0.f, 1.f);
    matrix matBone = (matrix) 0.f;
    matrix targetBoneMat = (matrix) 0.f;
    
    
    
    // Frame Data Index == Bone Count * Frame Count + _iThreadIdx.x
    uint iFrameDataIndex = BoneCount * CurFrame + _iThreadIdx.x;
    uint iNextFrameDataIdx = BoneCount * NextFrame + _iThreadIdx.x;
    
    
    // 보간
    float4 vScale = lerp(g_arrFrameTrans[iFrameDataIndex].vScale, g_arrFrameTrans_next[iNextFrameDataIdx].vScale, Ratio);
    float4 vTrans = lerp(g_arrFrameTrans[iFrameDataIndex].vTranslate, g_arrFrameTrans_next[iNextFrameDataIdx].vTranslate, Ratio);
    float4 qRot = QuternionLerp(g_arrFrameTrans[iFrameDataIndex].qRot, g_arrFrameTrans_next[iNextFrameDataIdx].qRot, Ratio);
    
    bool modiUse = false;
    matrix addRotMatrix = (matrix) 0.f;
    
    // 변형 조건 진입
    if (ModifyActive)
    {
        // 회전 행렬
        float4 quaternion = { cos(radians(RotScalar) / 2), 0, 0, sin(radians(RotScalar) / 2) };
        MatrixRotationQuaternion(quaternion, addRotMatrix);
        
        // 영향 받는 뼈 찾기
        for (int idx = 0; idx < 173; ++idx)
        {
            if (_iThreadIdx.x == g_modifyIndices[idx].idx)
            {
                modiUse = true;                
                uint PiFrameDataIndex = BoneCount * CurFrame + g_modifyIndices[idx].targetIdx;
                uint PiNextFrameDataIdx = BoneCount * NextFrame + g_modifyIndices[idx].targetIdx;
                float4 vtScale = lerp(g_arrFrameTrans[PiFrameDataIndex].vScale, g_arrFrameTrans_next[PiNextFrameDataIdx].vScale, Ratio);
                float4 vtTrans = lerp(g_arrFrameTrans[PiFrameDataIndex].vTranslate, g_arrFrameTrans_next[PiNextFrameDataIdx].vTranslate, Ratio);
                float4 qtRot = QuternionLerp(g_arrFrameTrans[PiFrameDataIndex].qRot, g_arrFrameTrans_next[PiNextFrameDataIdx].qRot, Ratio);
                
                
                MatrixAffineTransformation(vtScale, vtQZero, qtRot, vtTrans, targetBoneMat);
                
                
                break;
            }
        }
    }    
    
    // 최종 본행렬 연산 (lerp)
    if (modiUse)
    {
        // targetBoneMat 행렬을 넣어서 modi 행렬을 구함
        MatrixAffineTransformation(vScale, vQZero, qRot, addRotMatrix, targetBoneMat, vTrans, matBone);
    }
    else
    {
        MatrixAffineTransformation(vScale, vQZero, qRot, vTrans, matBone);        
    }
    // 최종 본행렬 연산    
    //MatrixAffineTransformation(g_arrFrameTrans[iFrameDataIndex].vScale, vQZero, g_arrFrameTrans[iFrameDataIndex].qRot, g_arrFrameTrans[iFrameDataIndex].vTranslate, matBone);

    // 담당하는 뼈의 위치를 offset(기본) 위치로 이동시킴
    matrix matOffset = transpose(g_arrOffset[_iThreadIdx.x]);

    // 구조화버퍼에 결과값 저장 : 프레임위치에 오프셋으로 이동시킨 뼈를 곱해서 최종 뼈위치를 구함
    g_arrFinelMat[_iThreadIdx.x] = mul(matOffset, matBone);
}


#endif