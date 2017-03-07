// Burning_bricks.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Burning_bricks.h"
#include "GlobalVars.h"
#include "Game\Locator.h"
#include "Graphics\GraphicsLayer.h"
#include "Game\GameWorld.h"

#ifndef _CONSOLE
// Global Variables:
extern cGlobalVars gGlobal;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Preprocess(HINSTANCE hInstance);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	Preprocess(hInstance);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	int rtCode = 0;
	{
		std::shared_ptr<cLog> log(std::make_shared<cLog>());
		std::shared_ptr<cAudio> audio(std::make_shared<cAudio>());
		std::shared_ptr<cPhysics> physics(std::make_shared<cPhysics>());
		std::shared_ptr<cGraphics> graphics(std::make_shared<cGraphicsLayer>());
		graphics->Init(gGlobal.hWnd, gGlobal.width, gGlobal.height);
		std::shared_ptr<cInputLayer> inputLayer(std::make_shared<cInputLayer>(gGlobal.hInst, gGlobal.hWnd, false));
		cLocator::Init(log.get(), audio.get(), physics.get(), graphics.get(), inputLayer.get());
		pGameWorld GW(std::make_shared<cGameWorld>());
		GW->WorldBeginning();

		HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BURNING_BRICKS));
		MSG msg;
		PeekMessage(&msg, nullptr, 0U, 0U, PM_NOREMOVE);
		while (msg.message != WM_QUIT) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0) {
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else {
				using namespace std::chrono;
				static steady_clock::time_point lastT = steady_clock::now();
				static nanoseconds accumuT(0);
				steady_clock::time_point nowT = steady_clock::now();
				auto du = nowT - lastT + accumuT;
				lastT = nowT;
				while (du >= FrameTime(1)) {
					GW->HandleInput();
					GW->Update();
					du -= FrameTime(1);
				}
				steady_clock::time_point rdBegin = steady_clock::now();
				GW->Render();
				auto rdDu = steady_clock::now() - rdBegin;
				accumuT = du + rdDu;
				std::this_thread::sleep_for(FrameTime(1) - accumuT);
			}
		}
		rtCode = (int)msg.wParam;
	}

	return rtCode;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BURNING_BRICKS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_BURNING_BRICKS);
	wcex.lpszClassName = gGlobal.szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	gGlobal.hInst = hInstance; // Store instance handle in our global variable

	RECT rt = { 0, 0, gGlobal.width, gGlobal.height };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	gGlobal.hWnd = CreateWindowW(gGlobal.szWindowClass, gGlobal.szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!gGlobal.hWnd) {
		return FALSE;
	}

	ShowWindow(gGlobal.hWnd, nCmdShow);
	UpdateWindow(gGlobal.hWnd);
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		//switch (wmId) {
		//default:
		//	return DefWindowProc(hWnd, message, wParam, lParam);
		//}
	}
	break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Preprocess(HINSTANCE hInstance)
{
	gGlobal.hInst = hInstance;
	LoadStringW(gGlobal.hInst, IDS_APP_TITLE, gGlobal.szTitle, MAX_LOADSTRING);
	LoadStringW(gGlobal.hInst, IDC_BURNING_BRICKS, gGlobal.szWindowClass, MAX_LOADSTRING);
	STARTUPINFO si;
	GetStartupInfo(&si);
	if (si.dwFlags & STARTF_USEPOSITION){
		gGlobal.posX = si.dwX;
		gGlobal.posY = si.dwY;
	}
	if (si.dwFlags & STARTF_USESIZE) {
		gGlobal.width = si.dwXSize;
		gGlobal.height = si.dwYSize;
	}
}

#else
typedef std::chrono::duration<long long, std::ratio<1, 2> > FrameTime;
int main(int argc, char** argv)
{
	pGameScene g_Scene = std::make_shared<cGameScene>();
	g_Scene->CreateScene();
	while (true) {
		using namespace std::chrono;
		static steady_clock::time_point lastT = steady_clock::now();
		static nanoseconds accumuT(0);
		steady_clock::time_point nowT = steady_clock::now();
		auto du = nowT - lastT + accumuT;
		lastT = nowT;
		//std::cout << du.count() << std::endl;
		while (du >= FrameTime(1)) {
			cCommand cmd(eGoForward);
			g_Scene->DispatchCmd(cmd);
			g_Scene->Update();
			du -= FrameTime(1);
		}
		accumuT = du;
		std::this_thread::sleep_for(FrameTime(1) - accumuT);
	}
}
#endif // !CONSOLE