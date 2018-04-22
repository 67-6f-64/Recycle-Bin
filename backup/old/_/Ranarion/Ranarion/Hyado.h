#pragma once
#include<Windows.h>
#include<Richedit.h>


LRESULT CALLBACK TestWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void WindowResize(HWND hwnd, int Width, int Height);
bool CreateSimpleWindow(HINSTANCE hInstance, char *Caption, int Width = 400, int Height = 300, WNDPROC lpfnWndProc = TestWndProc, int invisible = 0);
