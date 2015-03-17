#include "SubDll.h"

// Shared Global Value
#pragma comment(linker, "/section:.SHARDAT,rws")
#pragma data_seg(".SHARDAT")
static HINSTANCE	hInst		= NULL;
static HHOOK		hhk			= NULL;
static HWND			hWnd		= NULL;
static HWND			hWnd_App	= NULL;
static WNDPROC		OldProc		= NULL;
static UINT			WM_USER_SUB_START	= 0;
static UINT			WM_USER_SUB_RESTORE = 0;
static UINT			WM_USER_SUB_EXIT	= 0;
#pragma data_seg()

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD  Reason, LPVOID Reserved){
	switch(Reason){
		case DLL_PROCESS_ATTACH:{
			hInst = hInstance;
		}break;

		case DLL_PROCESS_DETACH:{
			;
		}break;

		case DLL_THREAD_ATTACH:{
			;
		}break;

		case DLL_THREAD_DETACH:{
			;
		}

		default:break;
	}

	return TRUE;
}

BOOL InstallHook(){
	hWnd		= FindWindowW(APP_TARGET_CLASS_NAME, NULL);
	hWnd_App	= FindWindowW(APP_CLASS_NAME, NULL);
	if(hWnd == NULL){
		MessageBox(NULL, TEXT("Target Application Don't Start"), 
			APP_TARGET_CLASS_NAME, 
			MB_OK);
		exit(0);
	}

	hhk	= SetWindowsHookExW(WH_GETMESSAGE, (HOOKPROC)HookGetMsgProc, hInst, 
		GetWindowThreadProcessId(hWnd, NULL));
	if(hhk != NULL){
		WM_USER_SUB_START	= RegisterWindowMessage(TEXT("WM_USER_SUB_START"));
		WM_USER_SUB_RESTORE	= RegisterWindowMessage(TEXT("WM_USER_SUB_RESTORE"));
		PostMessage(hWnd, WM_USER_SUB_START, 0, 0);
		return TRUE;
	}else{
		return FALSE;
	}
}

BOOL UnInstallHook(){
	return UnhookWindowsHookEx(hhk);
}

BOOL RestoreWndProc(){
	return PostMessage(hWnd, WM_USER_SUB_RESTORE, 0, 0);
}

LONG HookWndProc(){
	OldProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG)SubclassWndProc);
	if(OldProc == NULL){
		PrintErrorMessage();
	}
	return (LONG)OldProc;
}

LONG UnHookWndProc(){
	LONG ret = SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG)OldProc);
	if(ret == NULL){
		PrintErrorMessage();
	}

	WM_USER_SUB_EXIT = RegisterWindowMessage(TEXT("WM_USER_SUB_EXIT"));
	PostMessage(hWnd_App, WM_USER_SUB_EXIT, 0, 0);

	return ret;
}

LRESULT CALLBACK HookGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam){
	if(nCode == HC_ACTION){
		LPMSG msg = (LPMSG)lParam;
		if(msg->message == RegisterWindowMessage(TEXT("WM_USER_SUB_START"))){
			HookWndProc();
		}else if(msg->message == RegisterWindowMessage(TEXT("WM_USER_SUB_RESTORE"))){
			UnHookWndProc();
		}else{
			;
		}
	}
	return CallNextHookEx(hhk, nCode, wParam, lParam);
}

/******************************************************************************
* 当鼠标点击窗口的关闭按钮的时候，通过spy++测得，依次大致向窗口发送以下消息：
*	WM_NCLBUTTONDOWN
*	WM_SYSCOMMAND	uCmdType:SC_CLOSE xPos:** yPos:**
*	WM_CLOSE
*	WM_DESTROY
******************************************************************************/

LRESULT CALLBACK SubclassWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	switch(Msg){
		case WM_CLOSE:{
			MessageBoxA(NULL, "WM_CLOSE Be Hooked", "SubStart", MB_OK);
			return 0;
		}break;

		default:break;
	}

	return CallWindowProc(OldProc, hWnd, Msg, wParam, lParam);
}

void PrintErrorMessage(){
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, 
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, 
		NULL);
		
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("SubStart Error"), MB_OK);
}
