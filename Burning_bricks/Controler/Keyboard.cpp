#include "stdafx.h"
#include "ControlerExceptionDefine.h"
#include "Keyboard.h"
#include "InputLayer.h"

cKeyboard::cKeyboard(LPDIRECTINPUT8 di, HWND hWnd)
{
	m_pTarget = nullptr;
	ClearTable();

	HRESULT hr = di->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr);
	if (FAILED(hr)) {
		throw cControlerError("Keyboard could not be created\n");
	}
	try {
	hr = m_pDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		throw cControlerError("Set Keyboard data format failed\n");
	}
	hr = m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) {
		throw cControlerError("Keyboard coop level could not be changed\n");
	}
	}
	catch (cControlerError& e) {
		if (m_pDevice){
			m_pDevice->Release();
			m_pDevice = nullptr;
		}
	}
}

cKeyboard::~cKeyboard()
{
	if (m_pDevice) {
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

void cKeyboard::ClearTable()
{
	memset(m_KeyState, 0, sizeof(char) * 256);
}

bool cKeyboard::Poll(int key) const
{
	if (m_KeyState[key] & 0x80) {
		return true;
	}
	else {
		return false;
	}
}

void cKeyboard::SetReceiver(iKeyboadrReceiver* pTarget)
{
	m_pTarget = pTarget;
}

eResult cKeyboard::Update()
{
	BYTE newState[256];
	HRESULT hr = m_pDevice->Poll();
	hr = m_pDevice->GetDeviceState(sizeof(newState), newState);
	if (FAILED(hr)) {
		hr = m_pDevice->Acquire();
		if (FAILED(hr)) {
			return eResult::resFailed;
		}
		hr = m_pDevice->Poll();
		hr = m_pDevice->GetDeviceState(sizeof(newState), newState);
		if (FAILED(hr)) {
			return eResult::resFailed;
		}
	}
	if (m_pTarget) {
		for (int i = 0; i < 256; i++) {
			if (m_KeyState[i] != newState[i]) {
				if (m_KeyState[i] & 0x80) {
					m_pTarget->KeyUp(i);
				}
				else {

				}
			}
			m_KeyState[i] = newState[i];
			if (Poll(i)) {
				m_pTarget->KeyDown(i);
			}
		}
	}
	else {
		memcpy(m_KeyState, newState, sizeof(char) * 256);
	}
	return eResult::resAllGood;
}
