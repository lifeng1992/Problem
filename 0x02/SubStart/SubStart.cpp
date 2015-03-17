#include "resource.h"
#include <Windows.h>
#include <WindowsX.h>
#include <stdio.h>

#define WM_USER_TRAY	(WM_USER + 100)

#define APP_CLASS_NAME	TEXT("ClassName-SubStart")

typedef BOOL (*PINSTALL)(void);
typedef BOOL (*PUNINSTALL)(void);
typedef BOOL (*PRESTORE)(void);

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
	static UINT WM_USER_SUB_EXIT = RegisterWindowMessage(TEXT("WM_USER_SUB_EXIT"));
	
	static	HINSTANCE	hModule		= NULL;
	static	PINSTALL	pInstall	= NULL;
	static	PUNINSTALL	pUnInstall	= NULL;
	static  PRESTORE	pRestore	= NULL;

	switch(Msg){
		case WM_CREATE:{
			hModule = LoadLibraryA("SubDll.dll");
			if(hModule == NULL){
				MessageBoxA(NULL, "SubDll.dll文件不存在", "DLL加载失败", MB_OK);
				exit(0);
			}

			pInstall	= (PINSTALL)GetProcAddress(hModule, "InstallHook");
			pUnInstall	= (PUNINSTALL)GetProcAddress(hModule, "UnInstallHook");
			pRestore	= (PRESTORE)GetProcAddress(hModule, "RestoreWndProc");
			if(pInstall == NULL || pUnInstall == NULL || pRestore == NULL){
				MessageBoxA(NULL, "DLL函数加载失败", "加载失败", MB_OK);
				FreeLibrary(hModule);
				exit(0);
			}

			BOOL res = pInstall();
			if(res == FALSE){
				MessageBoxA(NULL, "Hook Target Process Error", 
					"SubStart Error", MB_OK);
			}

			InitTray(hWnd, &nid);
			Shell_NotifyIconW(NIM_ADD, &nid);
			return 0;
		}break;

		case WM_PAINT:{
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
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
				pRestore();
				return 0;
			}else if(Msg == WM_USER_SUB_EXIT){
				BOOL ret = pUnInstall();
				if(ret == FALSE){
					MessageBoxA(NULL, "UnHook Target Process Failure",
						"SubStart Error", MB_OK);
				}
				FreeLibrary(hModule);
				PostQuitMessage(0);
			}else{
				;
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
