#pragma once
#include <dinput.h>
#include "eResult.h"

class iKeyboadrReceiver
{
public:
	virtual void KeyUp(int key) = 0;
	virtual void KeyDown(int key) = 0;
};

class cKeyboard
{
public:
	cKeyboard(LPDIRECTINPUT8 di, HWND hWnd);
	~cKeyboard();
	void ClearTable();
	bool Poll(int key) const;
	void SetReceiver(std::shared_ptr<iKeyboadrReceiver> pTarget);
	eResult Update();
	constexpr static unsigned int NumOfKeys = 256;

private:
	LPDIRECTINPUTDEVICE8 m_pDevice;
	std::shared_ptr<iKeyboadrReceiver> m_pTarget;
	BYTE m_KeyState[NumOfKeys];
};