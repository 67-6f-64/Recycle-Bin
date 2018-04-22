#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include<Windows.h>
#include<Richedit.h>

namespace Air{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND CreateSimpleWindow(HINSTANCE hInstance, LPCSTR lpWindowName, int nWidth = 800, int nHeight = 600, WNDPROC lpfnWndProc = WindowProc, BYTE bAlpha = 0);
	LRESULT WINAPI _DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	void WaitForQuit();
}

#endif