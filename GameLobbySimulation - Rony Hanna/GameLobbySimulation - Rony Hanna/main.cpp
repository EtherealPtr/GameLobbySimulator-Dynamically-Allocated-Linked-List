// --------------------------------------
// Program: A game lobby simulator that uses a dynamically allocated linked list to manage players 
// Created by Rony Hanna (c) 2017
// --------------------------------------

#define WIN32_LEAN_AND_MEAN  

// Local Includes
#include <Windows.h>

#include "resource.h"
#include "Lobby.h"
//#include "vld.h" //<-- For Debugging Purposes

using namespace std;

LPCWSTR WindowName = L"Game Lobby Simulator (c) Rony Hanna 2017";
Lobby GameLobby;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	static int ClientAreaX, ClientAreaY;
	static HDC hdcBackBuffer;
	static HBITMAP hBitmap;
	static HBITMAP hOldBitmap;
	static bool b_AddingPlayer = false;

	switch (msg)
	{
	case WM_CREATE:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);

		ClientAreaX = rect.right;
		ClientAreaY = rect.bottom;

		hdcBackBuffer = CreateCompatibleDC(NULL);

		HDC hdc = GetDC(hwnd);
		hBitmap = CreateCompatibleBitmap(hdc, ClientAreaX, ClientAreaY);
		ReleaseDC(hwnd, hdc);
		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		if (b_AddingPlayer)
		{
			double iXpos = static_cast<int>(LOWORD(lparam));
			double iYpos = static_cast<int>(HIWORD(lparam));
			GameLobby.GetMouseCoord(iXpos, iYpos);
			GameLobby.AddPlayerVector(Vector2D(iXpos, iYpos));
			b_AddingPlayer = false;
		}
		break;
	}

	case WM_COMMAND:
	{
		switch (wparam)
		{
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			break;

		case ID_OPERATIONS_ADDPLAYER:
			b_AddingPlayer = true;
			GameLobby.AddPlayer();
			break;

		case ID_OPERATIONS_KICKPLAYER:
			GameLobby.KickPlayer();
			break;

		case ID_OPERATIONS_CLEARLOBBY:
			GameLobby.ClearLobby();
			break;
			
		case ID_ABOUT_ABOUTTHISPROGRAM:
			MessageBox(NULL, L"A simple Game Lobby simulator using a dynamically allocated singly Linked List. (c) Rony Hanna 2017.", L"About This Program", MB_OK | MB_ICONINFORMATION);
			break;

		default:
			break;
		}
	}
	break;

	case WM_SIZE:
	{
		ClientAreaX = LOWORD(lparam);
		ClientAreaY = HIWORD(lparam);

		SelectObject(hdcBackBuffer, hOldBitmap);

		HDC hdc = GetDC(hwnd);
		hBitmap = CreateCompatibleBitmap(hdc, ClientAreaX, ClientAreaY);
		ReleaseDC(hwnd, hdc);
		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);
	}

	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		BitBlt(hdcBackBuffer, 0, 0, ClientAreaX, ClientAreaY, NULL, NULL, NULL, BLACKNESS);
		GameLobby.RenderPlayer(ClientAreaX, ClientAreaY, hdcBackBuffer);
		BitBlt(hdc, 0, 0, ClientAreaX, ClientAreaY, hdcBackBuffer, 0, 0, SRCCOPY);
		ReleaseDC(hwnd, hdc);
		EndPaint(hwnd, &ps);
	}

	break;

	case WM_CLOSE:
	{
		if (MessageBox(hwnd, L"Did you really want to exit?", L"Game Lobby Simulation with Linked List", MB_YESNO | MB_ICONQUESTION) == IDYES)
			DestroyWindow(hwnd);

		return 0;
	}
	break;

	case WM_DESTROY:
	{
		SelectObject(hdcBackBuffer, hOldBitmap);
		DeleteDC(hdcBackBuffer);
		DeleteObject(hBitmap);
		PostQuitMessage(0);

		return 0;	
	}

	break;

	default:
		break;

	}

	return (DefWindowProc(hwnd, msg, wparam, lparam));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	WNDCLASSEX winclass;
	HWND hwnd;
	MSG msg;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = NULL;
	winclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	winclass.lpszClassName = WindowName;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&winclass))
	{
		MessageBox(NULL, L"ERROR: Unable To Register Window!", L"Registration Error", MB_ICONERROR | MB_OK);
		return -1;
	}

	if (!(hwnd = CreateWindowEx(NULL, WindowName, WindowName, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME | WS_VISIBLE, 0, 0, 800, 600, NULL, NULL, hInstance, NULL)))
	{
		MessageBox(NULL, L"ERROR: Unable To Create Window!", L"Creation Error", MB_ICONERROR | MB_OK);
		return -1;
	}

	ShowWindow(hwnd, ncmdshow);
	UpdateWindow(hwnd);

	bool bDone = false;

	while (!bDone)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = true;
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);		
			}

			InvalidateRect(hwnd, NULL, false);
			UpdateWindow(hwnd);
		}
	}

	UnregisterClass(WindowName, winclass.hInstance);
	return 0;
}