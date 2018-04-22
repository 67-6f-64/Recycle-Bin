#include"AirWindow.h"

void AirWindow::EmbedWindow(HWND hWnd, int X, int Y, int cx, int cy){
	SetWindowLongA(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	SetParent(hWnd, hWndAW);
	SetWindowPos(hWnd, 0, X, Y, cx, cy, SWP_NOZORDER);
}