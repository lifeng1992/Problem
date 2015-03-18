#include "WinProc.h"
#include <stdio.h>

BOOL CreateWindowClass(HINSTANCE hInstance){
	WNDCLASSEX	wndclass;
	wndclass.cbSize			= sizeof(wndclass);
	wndclass.style			= CS_VREDRAW | CS_OWNDC | CS_DBLCLKS | CS_HREDRAW;
	wndclass.lpfnWndProc	= WinProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= CLASSNAME_SPYDER;
	wndclass.hIconSm		= LoadIcon(hInstance, IDI_APPLICATION);

	if(!RegisterClassEx(&wndclass)){
		return FALSE;
	}else{
		return TRUE;
	}
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	static NOTIFYICONDATA nid;

	switch(Msg){
		case WM_CREATE:{
			int ret = WhenWmCreate(hWnd, wParam, lParam);
			if(ret == SPYDER_SUCCESS){
				return 0;
			}
		}break;

		case WM_PAINT:{
			int ret = WhenWmPaint(hWnd, wParam, lParam);
			if(ret == SPYDER_SUCCESS){
				return 0;
			}
		}break;

		case WM_CLOSE:{
			int ret = WhenWmClose(hWnd, wParam, lParam);
			if(ret == SPYDER_SUCCESS){
				return 0;
			}
		}break;

		case WM_DESTROY:{
			int ret = WhenWmDestroy(hWnd, wParam, lParam);
			if(SPYDER_SUCCESS == ret){
				return 0;
			}
		}break;

		default:break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);			  
}

int WhenWmCreate(HWND hWnd, WPARAM wParam, LPARAM lParam){
	return SPYDER_SUCCESS;
}

int WhenWmClose(HWND hWnd, WPARAM wParam, LPARAM lParam){
	return SPYDER_CONTINUE;
}

int WhenWmPaint(HWND hWnd, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT Ps;
	HDC hdc = BeginPaint(hWnd, &Ps);
	EndPaint(hWnd, &Ps);
	
	return SPYDER_SUCCESS;
}

int WhenWmDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam){
	PostQuitMessage(0);
	return SPYDER_SUCCESS;
}
