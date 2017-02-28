#pragma once
#include <memory>
#include <dinput.h>
#include "Keyboard.h"
#include "Mouse.h"

class cInputLayer
{
public:
	cInputLayer(HINSTANCE instance, HWND hWnd, bool bExclusive, bool bUseKeyboard = true, bool bUseMouse = true);
	virtual ~cInputLayer();
	std::shared_ptr<const cKeyboard> GetKeyboard() const { return m_Keyboard; };
	std::shared_ptr<const cMouse> GetMouse() const { return m_Mouse; };
	void UpdateDevice();
	void SetFocus();
	void KillFocue();

private:
	std::shared_ptr<cKeyboard> m_Keyboard;
	std::shared_ptr<cMouse> m_Mouse;
	LPDIRECTINPUT8 m_pDI;
	static bool m_Exist;
};