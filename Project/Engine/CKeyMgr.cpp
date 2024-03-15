#include "pch.h"
#include "CKeyMgr.h"

#include "CEngine.h"

int g_arrVK[(UINT)KEY::END]
=
{
	 VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT,

	 VK_SPACE,
	 VK_RETURN,
	 VK_ESCAPE,
	 VK_LMENU,
	 VK_LCONTROL,
	 VK_LSHIFT,
	 VK_TAB,
	 
	 VK_LBUTTON,
	 VK_RBUTTON,

	 'Q','W','E','R',  'T','Y','U','I','O','P',
	 'A','S','D','F',  
	 'Z','X','C','V',  
	  
	 '0','1','2','3','4','5','6','7','8','9',

	 VK_NUMPAD0,
	 VK_NUMPAD1,
	 VK_NUMPAD2,
	 VK_NUMPAD3,
	 VK_NUMPAD4,
	 VK_NUMPAD5,
	 VK_NUMPAD6,
	 VK_NUMPAD7,
	 VK_NUMPAD8,
	 VK_NUMPAD9,

};





CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}


void CKeyMgr::init()
{
	for (int i = 0; i < (int)KEY::END; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ (KEY)i  , KEY_STATE::NONE });
	}
}

void CKeyMgr::tick()
{
	if (GetFocus())
	{
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(g_arrVK[(UINT)m_vecKey[i].key]) & 0x8000)
			{
				// 이전에는 눌리지 않았다.
				if (false == m_vecKey[i].bPrev)
				{
					m_vecKey[i].state = KEY_STATE::TAP;
					m_vecKey[i].bPrev = true;
				}
				else
				{
					// 지금도 눌려있고, 이전 프레임에서도 눌려있었다.
					m_vecKey[i].state = KEY_STATE::HOLD;
				}
			}
			else
			{
				// 눌려있지 않다.
				if (false == m_vecKey[i].bPrev)
				{
					m_vecKey[i].state = KEY_STATE::NONE;
				}
				else
				{
					m_vecKey[i].state = KEY_STATE::RELEASE;
					m_vecKey[i].bPrev = false;
				}
			}
		}

		// Mouse 위치 갱신
		m_vPrevMousePos = m_vMousePos;

		POINT ptMousePos = {};
		GetCursorPos(&ptMousePos);		
		ScreenToClient(CEngine::GetInst()->GetMainWnd(), &ptMousePos);
		m_vMousePos = Vec2((float)ptMousePos.x, (float)ptMousePos.y);

		m_vMouseDir = m_vMousePos - m_vPrevMousePos;
		m_vMouseDir.y *= -1;


		// law input
		
	}
	
	// Window 가 focus 상태가 아니다
	else
	{
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			if (KEY_STATE::TAP == m_vecKey[i].state || KEY_STATE::HOLD == m_vecKey[i].state)
			{
				m_vecKey[i].state = KEY_STATE::RELEASE;
			}

			else if (KEY_STATE::RELEASE == m_vecKey[i].state)
			{
				m_vecKey[i].state = KEY_STATE::NONE;
			}			 
		}
	}

	Vec2 outInput(0, 0);
	if (m_MouseRawInput.empty())
		m_vMouseRawDir = outInput;
	else
	{
		m_vMouseRawDir = m_MouseRawInput.front();
		m_MouseRawInput.pop();
	}	
}

void CKeyMgr::OnMouseRawInput(int _x, int _y)
{
	Vec2 prev;
	Vec2 input((float)_x, (float)_y);
	if (!m_MouseRawInput.empty())
	{
		prev = m_MouseRawInput.front();
		// 이전이랑 입력이 다르면 초기화 해주고 갱신
		if (prev != input)
		{
			while (!m_MouseRawInput.empty())
			{
				m_MouseRawInput.pop();
			}
			m_MouseRawInput.push(input);
		}
	}
	// 입력이 없었으면 갱신
	m_MouseRawInput.push(input);	
}
