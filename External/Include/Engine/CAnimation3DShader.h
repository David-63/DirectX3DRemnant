#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CAnimation3DShader : public CComputeShader
{
private:
	CStructuredBuffer* m_pFrameDataBuffer;			// t16
	CStructuredBuffer* m_pOffsetMatBuffer;			// t17

	CStructuredBuffer* m_pFrameDataBuffer_next;		// t18
	CStructuredBuffer* m_pModifyIndicesBuffer;		// t19
	
	CStructuredBuffer* m_pOutputBuffer;				// u0



public:
	void SetFrameDataBuffer(CStructuredBuffer* _buffer) { m_pFrameDataBuffer = _buffer; }
	void SetOffsetMatBuffer(CStructuredBuffer* _buffer) { m_pOffsetMatBuffer = _buffer; }
	void SetFrameDataBuffer_next(CStructuredBuffer* _buffer) { m_pFrameDataBuffer_next = _buffer; }
	void SetOutputBuffer(CStructuredBuffer* _buffer) { m_pOutputBuffer = _buffer; }
	void SetModifyIdxBuffer(CStructuredBuffer* _buffer) { m_pModifyIndicesBuffer = _buffer; }

	// g_int_0 : BonCount, g_int_1 : Frame Index
	void SetBoneCount(int _iBoneCount) { m_Const.arrInt[0] = _iBoneCount; }
	void SetFrameIndex(int _iFrameIdx) { m_Const.arrInt[1] = _iFrameIdx; }
	void SetNextFrameIdx(int _iFrameIdx) { m_Const.arrInt[2] = _iFrameIdx; }
	void SetFrameRatio(float _fFrameRatio) { m_Const.arrFloat[0] = _fFrameRatio; }

	void SetModifyUse(bool _use) { m_Const.arrAnimData[2] = _use; }
	void SetModifyRotScalar(float _scalar) { m_Const.arrFloat[1] = _scalar; }

public:
	virtual void UpdateData() override;
	virtual void Clear();

public:
	CAnimation3DShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CAnimation3DShader();
};