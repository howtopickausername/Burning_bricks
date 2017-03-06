#include "stdafx.h"
#include "GlobalVars.h"

cGlobalVars gGlobal;


cGlobalVars::cGlobalVars()
	:hInst(nullptr), hWnd(nullptr), 
	posX(CW_USEDEFAULT), posY(CW_USEDEFAULT),
	width(1440), height(900)
{
	ZeroMemory(szTitle, MAX_LOADSTRING);
	ZeroMemory(szWindowClass, MAX_LOADSTRING);
}
