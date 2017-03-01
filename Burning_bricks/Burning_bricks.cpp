// Burning_bricks.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Burning_bricks.h"
#include "Game\Locator.h"
#include "Game\GameWorld.h"

#ifndef _CONSOLE
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hMainWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_BURNING_BRICKS, szWindowClass, MAX_LOADSTRING);
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
		std::shared_ptr<cGraphics> graphics(std::make_shared<cGraphics>());
		std::shared_ptr<cInputLayer> inputLayer(std::make_shared<cInputLayer>(hInst, hMainWnd, false));
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
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_BURNING_BRICKS);
	wcex.lpszClassName = szWindowClass;
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
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	hMainWnd = hWnd;
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
		switch (wmId) {
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
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
			g_Scene->HandleInput(cmd);
			g_Scene->Update();
			du -= FrameTime(1);
		}
		accumuT = du;
		std::this_thread::sleep_for(FrameTime(1) - accumuT);
	}
}
#endif // !CONSOLE