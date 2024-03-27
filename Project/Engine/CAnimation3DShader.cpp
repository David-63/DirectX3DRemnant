#include "pch.h"
#include "CAnimation3DShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CAnimation3DShader::CAnimation3DShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: m_pFrameDataBuffer(nullptr)
	, m_pOffsetMatBuffer(nullptr)
	, m_pFrameDataBuffer_next(nullptr)
	, m_pOutputBuffer(nullptr)
	, m_pRetBuffer(nullptr)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;
}

CAnimation3DShader::~CAnimation3DShader()
{
}

void CAnimation3DShader::UpdateData()
{
	// 구조화버퍼 전달
	m_pFrameDataBuffer->UpdateData_CS(16, true); // t16
	m_pOffsetMatBuffer->UpdateData_CS(17, true); // t17
	
	m_pFrameDataBuffer_next->UpdateData_CS(18, true); // t18
	if (m_pModifyIndicesBuffer)
	{
		m_pModifyIndicesBuffer->UpdateData_CS(19, true);
	}
	m_pOutputBuffer->UpdateData_CS(0, false);   // u0
	m_pRetBuffer->UpdateData_CS(1, false);		// u1

	// m_Const.arrInt[0] = 뼈 개수만큼 스레드 돌림
	m_iGroupX = (m_Const.arrInt[0] / m_iGroupPerThreadX) + 1;
	m_iGroupY = 1;
	m_iGroupZ = 1;
}


void CAnimation3DShader::Clear()
{
	// 전달한 구조화버퍼 클리어	
	if (nullptr != m_pFrameDataBuffer)
	{
		m_pFrameDataBuffer->Clear_CS(true);
		m_pFrameDataBuffer = nullptr;
	}

	if (nullptr != m_pOffsetMatBuffer)
	{
		m_pOffsetMatBuffer->Clear_CS(true);
		m_pOffsetMatBuffer = nullptr;
	}

	if (nullptr != m_pFrameDataBuffer_next)
	{
		m_pFrameDataBuffer_next->Clear_CS(true);
		m_pFrameDataBuffer_next = nullptr;
	}
	if (nullptr != m_pModifyIndicesBuffer)
	{
		m_pModifyIndicesBuffer->Clear_CS(true);
		m_pModifyIndicesBuffer = nullptr;
	}	
	if (nullptr != m_pOutputBuffer)
	{
		m_pOutputBuffer->Clear_CS(false);
		m_pOutputBuffer = nullptr;
	}
	if (nullptr != m_pRetBuffer)
	{
		m_pRetBuffer->Clear_CS(false);
		m_pRetBuffer = nullptr;
	}
}