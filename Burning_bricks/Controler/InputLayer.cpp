#include "stdafx.h"
#include "InputLayer.h"
#include "ControlerExceptionDefine.h"

bool cInputLayer::m_Exist = false;
cInputLayer::cInputLayer(HINSTANCE instance, HWND hWnd, bool bExclusive, bool bUseKeyboard /*= true*/, bool bUseMouse /*= true */)
{
	assert(!m_Exist, "cInputLayer alread initialized");
	m_Keyboard = nullptr;
	m_Mouse = nullptr;

	HRESULT hr;
	hr = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, nullptr);
	if (FAILED(hr)) {
		throw cControlerError("DirectInput8 could not be created\n");
	}
	try {
		if (bUseKeyboard) {
			m_Keyboard = std::make_shared<cKeyboard>(m_pDI, hWnd);
		}
		if (bUseMouse) {
			m_Mouse = std::make_shared<cMouse>(m_pDI, hWnd, bExclusive);
		}
	}
	catch (cControlerError& e) {
		if (m_pDI) {
			m_pDI->Release();
			m_pDI = nullptr;
		}
		throw;
	}
	m_Exist = true;
}

cInputLayer::~cInputLayer()
{
	if (m_pDI) {
		m_pDI->Release();
		m_pDI = nullptr;
	}
}

void cInputLayer::UpdateDevice()
{
	if (m_Keyboard) {
		m_Keyboard->Update();
	}
	if (m_Mouse) {
		m_Mouse->Update();
	}
}

void cInputLayer::SetFocus()
{
	if (m_Keyboard) {
		m_Keyboard->ClearTable();
	}
	if (m_Mouse) {
		m_Mouse->Acquire();
	}
}

void cInputLayer::KillFocue()
{
	if (m_Keyboard) {
		m_Keyboard->ClearTable();
	}
	if (m_Mouse) {
		m_Mouse->UnAcquire();
	}
}