#include "WinProc.h"

#define DRAW_PAINT		0
#define DRAW_CLEAN		1

#define WIDTH			800
#define HEIGHT			480

// 速度
#define SPEED_X			21
#define SPEED_Y			10

#define X_AXIS			50
#define Y_AXIS			25
#define X_TOP			50
#define Y_TOP			25
#define X_BOTTOM		750
#define Y_BOTTOM		455

void DrawCircle(HDC hdc, POINT core);

void CoreNext(PPOINT p);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR lpCmdline, INT nCmdshow){
	HWND		hWnd;
	MSG			Msg;
	POINT		core = {50, 25};

	if(FALSE == CreateWindowClass(hInstance)){
		return 0;
	}

	hWnd = CreateWindowW(CLASSNAME_SPYDER, TEXT("内存绘图"),
		WS_POPUPWINDOW | WS_CAPTION | WS_SYSMENU
		| WS_VISIBLE | WS_MINIMIZEBOX,
		0, 0, 806, 508, NULL, NULL, hInstance, NULL);
	if(!hWnd){
		return 0;
	}

	HDC hdc = GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, 800, 480);
	SelectBitmap(hdcMem, hbmMem);
	
	HPEN white_pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	HBRUSH black_brush = CreateSolidBrush(RGB(0, 0, 0));
	SelectPen(hdcMem, white_pen);
	SelectBrush(hdcMem, black_brush);

	while(TRUE){
		if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)){
			if(Msg.message == WM_QUIT){
				break;
			}else if(Msg.message == WM_PAINT){
				BitBlt(hdc, 0, 0, 800, 480, hdcMem, 0, 0, SRCCOPY);
			}
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		DWORD start_time = GetTickCount();
		while((GetTickCount() - start_time) < 33);
		
		CoreNext(&core);
		DrawCircle(hdcMem, core);
		BitBlt(hdc, 0, 0, 800, 480, hdcMem, 0, 0, SRCCOPY);
	}

	DeleteObject(hdcMem);
	DeleteObject(hbmMem);
	DeleteObject(white_pen);
	DeleteObject(black_brush);

	return Msg.wParam;
}

void DrawCircle(HDC hdc, POINT core){
	Ellipse(hdc, core.x - X_AXIS, core.y - Y_AXIS,
		core.x + X_AXIS, core.y + Y_AXIS);
}

void CoreNext(PPOINT p){
	// 方向
	static int vx = 1;
	static int vy = 1;
	p->x = p->x + vx * SPEED_X;
	p->y = p->y + vy * SPEED_Y;

	if(p->x > X_BOTTOM){
		vx = vx * (-1);
		p->x = X_BOTTOM + vx * (p->x - X_BOTTOM);
	}else if(p->x < X_TOP){
		vx = vx * (-1);
		p->x = X_TOP + vx * (X_TOP - p->x);
	}else if(p->y > Y_BOTTOM){
		vy = vy * (-1);
		p->y = Y_BOTTOM + vy * (p->y - Y_BOTTOM);
	}else if(p->y < Y_TOP){
		vy = vy * (-1);
		p->y = Y_TOP + vy * (Y_TOP - p->y);
	}else{
		;
	}
}
