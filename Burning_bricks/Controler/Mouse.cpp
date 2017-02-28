#include "stdafx.h"
#include "ControlerExceptionDefine.h"
#include "Mouse.h"
#include "InputLayer.h"

cMouse::cMouse(LPDIRECTINPUT8 di, HWND hWnd, bool bExclusive)
{
	m_pTarget = nullptr;
	RepeatAcquire = true;
	m_hWnd = hWnd;
	m_bExclusive = bExclusive;
	HRESULT hr = di->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr);
	HRESULT hrAbs = di->CreateDevice(GUID_SysMouse, &m_pDeviceAbs, nullptr);
	if (FAILED(hr) || FAILED(hrAbs)) {
		throw cControlerError("[cMouse init] could not create device!\n");
	}
	hr = m_pDevice->SetDataFormat(&c_dfDIMouse);
	try {
		if (FAILED(hr)) {
			throw cControlerError("[cMouse init] could not set dateformat!\n");
		}
		if (m_bExclusive) {
			hr = m_pDevice->SetCooperativeLevel(m_hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
		}
		else {
			hr = m_pDevice->SetCooperativeLevel(m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		}
		if (FAILED(hr)) {
			throw cControlerError("[cMouse init]:setcooprativelevel failed!\n");
		}
		ZeroMemory(&m_LastState, sizeof(m_LastState));
	}
	catch (cControlerError &e) {
		if (m_pDevice) {
			m_pDevice->Release();
			m_pDevice = nullptr;
		}
		throw;
	}
}

cMouse::~cMouse()
{
	if (m_pDevice) {
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

eResult cMouse::Acquire()
{
	HRESULT hr = m_pDevice->Acquire();
	if (FAILED(hr)) {
		return eResult::resFailed;
	}
	else {
		return eResult::resAllGood;
	}
}

void cMouse::UnAcquire()
{
	m_pDevice->Unacquire();
}

void cMouse::SetReceiver(iMouseReceiver* pTarget)
{
	m_pTarget = pTarget;
}

void cMouse::SetExclusive(bool bExclusive)
{
	if (m_bExclusive == bExclusive)
		return;

	m_bExclusive = bExclusive;
	UnAcquire();
	HRESULT hr;
	if (bExclusive) {
		hr = m_pDevice->SetCooperativeLevel(m_hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	}
	else {
		hr = m_pDevice->SetCooperativeLevel(m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	}
}

eResult cMouse::Update()
{
	DIMOUSESTATE currState;
	HRESULT hr = m_pDevice->Poll();
	hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &currState);
	if (FAILED(hr)) {
		hr = m_pDevice->Acquire();
		if (FAILED(hr)) {
			return eResult::resFailed;
		}
		m_pDevice->Poll();
		hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &currState);
		if (FAILED(hr)) {
			return eResult::resFailed;
		}
	}
	if (m_pTarget) {
		int dx = currState.lX;
		int dy = currState.lY;
		if (dx || dy) {
			m_pTarget->MouseMoved(dx, dy);
		}
		if (currState.rgbButtons[0] & 0x80) {
			m_pTarget->MouseButtonDown(0);
		}
		if (currState.rgbButtons[1] & 0x80) {
			m_pTarget->MouseButtonDown(1);
		}
		if (currState.rgbButtons[2] & 0x80) {
			m_pTarget->MouseButtonDown(2);
		}
		if (!(currState.rgbButtons[0] & 0x80)
			&& m_LastState.rgbButtons[0] & 0x80) {
			m_pTarget->MouseButtonUp(0);
		}
		if (!(currState.rgbButtons[1] & 0x80)
			&& m_LastState.rgbButtons[1] & 0x80) {
			m_pTarget->MouseButtonUp(1);
		}
		if (!(currState.rgbButtons[2] & 0x80)
			&& m_LastState.rgbButtons[2] & 0x80) {
			m_pTarget->MouseButtonUp(2);
		}
	}
	m_LastState = currState;
	return eResult::resAllGood;
}
