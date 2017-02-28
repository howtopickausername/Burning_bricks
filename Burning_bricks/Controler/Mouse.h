#pragma once
#include <dinput.h>
#include "eResult.h"

class iMouseReceiver
{
public:
	virtual void MouseMoved(int dx, int dy) = 0;
	virtual void MouseButtonDown(int button) = 0;
	virtual void MouseButtonUp(int button) = 0;
};

class cMouse
{
public:
	cMouse(LPDIRECTINPUT8 di, HWND hWnd, bool bExclusive);
	~cMouse();
	eResult Acquire();
	void UnAcquire();
	bool Poll(int key);
	void SetReceiver(iMouseReceiver* pTarget);
	void SetExclusive(bool bExclusive);
	eResult Update();
	bool RepeatAcquire;

protected:
	HWND m_hWnd;
	bool m_bExclusive;
	iMouseReceiver* m_pTarget;
	LPDIRECTINPUTDEVICE8 m_pDevice;
	LPDIRECTINPUTDEVICE8 m_pDeviceAbs;
	DIMOUSESTATE m_LastState;
};