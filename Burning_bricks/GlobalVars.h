#pragma once
#include <windows.h>

#define MAX_LOADSTRING 100
class cGlobalVars
{
public:
	cGlobalVars();
	HINSTANCE hInst;                                // current instance
	HWND hWnd;
	int posX;
	int posY;
	int width;
	int height;
	WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
	WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
};