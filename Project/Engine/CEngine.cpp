#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CCollisionMgr.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CFontMgr.h"

CEngine::CEngine()
	: m_hWnd(nullptr)
{
}

CEngine::~CEngine()
{

}

int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	// ���� ������ �ڵ�
	m_hWnd = _hWnd;
	m_vResolution = Vec2((float)_iWidth, (float)_iHeight);

	// �ػ󵵿� �´� �۾����� ũ�� ����
	RECT rt = { 0, 0, (int)_iWidth, (int)_iHeight};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hWnd, true);


	// Device �ʱ�ȭ
	if (FAILED(CDevice::GetInst()->init(m_hWnd, _iWidth, _iHeight)))
	{
		MessageBox(nullptr, L"Device �ʱ�ȭ ����", L"����", MB_OK);
		return E_FAIL;
	}


	// Manager �ʱ�ȭ
	CPathMgr::GetInst()->init();

	CKeyMgr::GetInst()->init();

	CTimeMgr::GetInst()->init();

	CResMgr::GetInst()->init();

	CRenderMgr::GetInst()->init();

	CFontMgr::GetInst()->init();

	CLevelMgr::GetInst()->init();		
	


	return S_OK;
}

void CEngine::progress()
{
	// ���� �Ŵ��� �� ����, ������Ʈ ������ ����
	tick();

	// ī�޶� ����, ī�޶� �ٶ󺸴� �������� ȭ���� �����쿡 �׸�
	render();

	// Event ó��, tick ���� �ٷ� ó���� �Ұ����Ѱ͵��� ��Ƽ� ����ó��
	CEventMgr::GetInst()->tick();
}

void CEngine::tick()
{
	// Manager Tick
	CResMgr::GetInst()->tick();
	CTimeMgr::GetInst()->tick(); // DT(DeltaTime), FPS ���ϱ�
	CKeyMgr::GetInst()->tick();	

	// FMOD Update
	CSound::g_pFMOD->update();

	// Level Update
	// Level �ȿ� �����ϴ� ��� GameObject ���� Tick �� ȣ�����
	CLevelMgr::GetInst()->tick();

	// Level ���� GameObject ���� �������� ���ؼ� �߻��� �浹�� üũ�Ѵ�.
	CCollisionMgr::GetInst()->tick();
}

void CEngine::render()
{	
	CRenderMgr::GetInst()->render();	

	// FPS, DT ���
	CTimeMgr::GetInst()->render();
}
