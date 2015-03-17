#include <Windows.h>

#define APP_CLASS_NAME	TEXT("ClassName-SubStart")

int main(int argc, char* argv[]){
	HWND hWnd = FindWindow(APP_CLASS_NAME, NULL);
	if(hWnd == NULL){
		exit(0);
	}
	
	UINT WM_USER_CLOSE = RegisterWindowMessage(TEXT("WM_USER_CLOSE"));
	PostMessage(hWnd, WM_USER_CLOSE, 0, 0);
	
	return 0;
}
