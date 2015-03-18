#ifndef SUBDLL_H
#define SUBDLL_H

#include <Windows.h>

BOOL InstallHook(HWND hWnd_Application, HWND hWnd_Target);

BOOL UnInstallHook();

BOOL RestoreWndProc();

LONG HookWndProc();

LONG UnHookWndProc();

LRESULT CALLBACK HookGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK SubclassWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void PrintErrorMessage();

#endif
