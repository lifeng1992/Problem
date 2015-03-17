#include "resource.h"
#include <Windows.h>
#include <WindowsX.h>
#include <stdio.h>

#define WM_USER_TRAY	(WM_USER + 100)

#define APP_CLASS_NAME	TEXT("ClassName-SubStart")

// 创建一个Windows窗口类
void CreateWindowsClass(WNDCLASSEX *pwinclass, HINSTANCE hInstance);

void InitTray(HWND hWnd, PNOTIFYICONDATA pnid);

// 创建窗口过程WinProc
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR lpCmdline, INT nCmdshow){
	WNDCLASSEX	winclass;
	HWND		hwnd;
	MSG			msg;

	CreateWindowsClass(&winclass, hInstance);
	// 注册Windows类
	if(!RegisterClassEx(&winclass)){
		return 0;
	}
	// 创建窗口
	hwnd = CreateWindowExW(NULL, APP_CLASS_NAME, TEXT("SubStart"),
		WS_POPUP | WS_CAPTION | WS_VISIBLE,
		200, 100, 480, 360, NULL, NULL, hInstance, NULL);
	if(!hwnd){
		return 0;
	}

	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT ps;
	HDC			hdc;

	static NOTIFYICONDATA nid;

	static UINT WM_USER_CLOSE = RegisterWindowMessage(TEXT("WM_USER_CLOSE"));

	switch(Msg){
		case WM_CREATE:{
			InitTray(hWnd, &nid);
			Shell_NotifyIconW(NIM_ADD, &nid);
			return 0;
		}break;

		case WM_PAINT:{
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;
		}break;

		case WM_CLOSE:{
			PostQuitMessage(0);
			return 0;
		}break;

		case WM_USER_TRAY:{
			if(lParam == WM_RBUTTONUP || lParam == WM_LBUTTONUP){
				MessageBoxA(NULL, "Message from tray.", "SubStart", MB_OK);
			}
			return 0;
		}break;

		default:{
			if(Msg == WM_USER_CLOSE){
				PostQuitMessage(0);
				return 0;
			}
		}break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);			  
}

void CreateWindowsClass(WNDCLASSEX *pwinclass, HINSTANCE hInstance){
	pwinclass->cbSize			= sizeof(WNDCLASSEX);
	pwinclass->style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	pwinclass->lpfnWndProc		= WinProc;
	pwinclass->cbClsExtra		= 0;
	pwinclass->cbWndExtra		= 0;
	pwinclass->hInstance		= hInstance;
	pwinclass->hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GITHUB));
	pwinclass->hCursor			= LoadCursor(NULL, IDC_ARROW);
	pwinclass->hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	pwinclass->lpszMenuName		= NULL;
	pwinclass->lpszClassName	= APP_CLASS_NAME;
	pwinclass->hIconSm			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GITHUB));
}

// Tray Icon
void InitTray(HWND hWnd, PNOTIFYICONDATA pnid){
	pnid->cbSize		= sizeof(NOTIFYICONDATA);
	pnid->hWnd			= hWnd;
	pnid->uID			= IDI_GITHUB;
	pnid->uFlags		= NIF_ICON | NIF_TIP | NIF_MESSAGE;
	// Flag:NIF_ICON => hIcon
	pnid->hIcon			= LoadIcon(GetModuleHandle(NULL), 
		MAKEINTRESOURCE(IDI_GITHUB));
	pnid->uVersion		= NOTIFYICON_VERSION_4;
	// Flag:NIF_TIP => szTip
	lstrcpy(pnid->szTip, TEXT("GitHub for Windows"));
	// Flag:NIF_MESSAGE => NIF_MESSAGE
	pnid->uCallbackMessage	= WM_USER_TRAY;
}
