#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CAnimation3DShader : public CComputeShader
{
private:
	CStructuredBuffer* m_pFrameDataBuffer;			// t13
	CStructuredBuffer* m_pOffsetMatBuffer;			// t14 
	CStructuredBuffer* m_pOutputBuffer;				// u0

public:
	// g_int_0 : BonCount, g_int_1 : Frame Index
	void SetBoneCount(int _iBoneCount) { m_Const.arrInt[0] = _iBoneCount; }
	void SetFrameIndex(int _iFrameIdx) { m_Const.arrInt[1] = _iFrameIdx; }
	void SetNextFrameIdx(int _iFrameIdx) { m_Const.arrInt[2] = _iFrameIdx; }
	void SetFrameRatio(float _fFrameRatio) { m_Const.arrFloat[0] = _fFrameRatio; }
	void SetFrameDataBuffer(CStructuredBuffer* _buffer) { m_pFrameDataBuffer = _buffer; }
	void SetOffsetMatBuffer(CStructuredBuffer* _buffer) { m_pOffsetMatBuffer = _buffer; }
	void SetOutputBuffer(CStructuredBuffer* _buffer) { m_pOutputBuffer = _buffer; }

public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CAnimation3DShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CAnimation3DShader();
};