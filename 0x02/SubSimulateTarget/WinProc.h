#ifndef WINPROC_H
#define WINPROC_H

#include <Windows.h>
#include <WindowsX.h>

#define CLASSNAME_SPYDER	TEXT("ClassName-Spyder")

#define SPYDER_CONTINUE		-1
#define SPYDER_SUCCESS		0
#define SPYDER_FAILURE		1

BOOL CreateWindowClass(HINSTANCE hInstance);

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

int WhenWmCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);

int WhenWmClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

int WhenWmPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);

int WhenWmDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif
