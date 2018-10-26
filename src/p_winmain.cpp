#include "p_private.h"
// Testing:
#include <iostream>
#include "chrono.h"
// User functions.
bool Setup();
bool Update();
void Exit();
// Error message box.
void MessageError(const LPCWSTR text)
{
	MessageBox(NULL,text,L"Error'd",MB_ICONERROR);
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// Windows entry point.
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR /*pCmdLine*/, int nCmdShow)
{
	// Load icon.
	HICON icon=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(101));
	// Register the window class.
	const wchar_t CLASS_NAME[]=L"MyWindowClass";
	WNDCLASSEX wc{};
	wc.cbSize=sizeof(WNDCLASSEX);
	wc.style=CS_OWNDC;
	wc.lpfnWndProc=WindowProc;
	wc.hIcon=icon;
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hInstance=hInstance;
	wc.lpszClassName=CLASS_NAME;
	if (RegisterClassEx(&wc)==0)
		return 0;
	// Create the window.
	platform::hwnd = CreateWindowEx(
						 /* Extended styles */0,
						 /*    Window class */CLASS_NAME,
						 /*     Window text */L"Windows program",
						 /*    Window style *///WS_OVERLAPPEDWINDOW,
						 WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
						 /*        Position */CW_USEDEFAULT,CW_USEDEFAULT,
						 /*            Size */platform::kDisplayWidth,platform::kDisplayHeight,
						 /*   Parent window */NULL,
						 /*            Menu */NULL,
						 /* Instance handle */hInstance,
						 /* Additional data */NULL
					 );
	if (platform::hwnd==NULL)
	{
		MessageError(L"Failed to create window!");
		return EXIT_FAILURE;
	}
	// Create bitmap buffer.
	if (!platform::buffer.Init(platform::hwnd))
	{
		MessageError(L"Failed to create back buffer.");
		return EXIT_FAILURE;
	}
	ShowWindow(platform::hwnd,nCmdShow);
	SetTimer(platform::hwnd,0,500,NULL);
	Chrono c;
	if (!Setup())
		return 0;
	// Set time granularity.
	timeBeginPeriod(10);
	// Run the message loop.
	MSG msg{};
	while (msg.message!=WM_QUIT)
	{
		// Dispatches incoming sent messages, checks the thread message queue for a posted message, and retrieves the message (if any exist).
		if (PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			// Translates virtual-key messages into character messages.
			TranslateMessage(&msg);
			// Dispatches a message to a window procedure.
			DispatchMessage(&msg);
		}
		else
		{
			c.Start();
			if (!Update())
				PostQuitMessage(0);
			c.Stop();
			// Invalidating the whole window is less costly than doing it for each item.
			InvalidateRect(platform::hwnd,NULL,FALSE);
			/*
				Sleep() has a default granularity of about 15 ms. This is a
				system wide setting.
			*/

			// TODO: fix
			Sleep(10);
		}
	}
	Exit();
	timeEndPeriod(10);
	return EXIT_SUCCESS;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	/*case WM_CLOSE:
		if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}
		return 0;
		break;*/
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_ERASEBKGND:
		return FALSE;
		break;
	case WM_KEYUP:
		platform::last_key=0;
		return TRUE;
		break;
	case WM_KEYDOWN:
		platform::last_key=wParam;
		return TRUE;
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps{};
		HDC hdc = BeginPaint(hwnd, &ps);
		LONG left  =ps.rcPaint.left;
		LONG top   =ps.rcPaint.top;
		LONG bottom=ps.rcPaint.bottom;
		LONG right =ps.rcPaint.right;
		BitBlt(hdc,left,top,right,bottom,platform::buffer.Hdc(),left,top,SRCCOPY);
		EndPaint(hwnd, &ps);
		return 0;
	}
	break;
	case WM_MOUSEMOVE:
	{
		platform::mouse_x=GET_X_LPARAM(lParam);
		platform::mouse_y=GET_Y_LPARAM(lParam);
		return 0;
	}
	break;
	case WM_TIMER:
	{
		//std::wcout<<L"<timer>";
		return 0;
	}
	break;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
