#ifndef __AIRGUI_MAIN_H__
#define __AIRGUI_MAIN_H__

#include<Windows.h>
#include<Richedit.h>

//MainWindow
namespace Air{
	extern HWND hWndList[0x100];
	extern DWORD WndCount;

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND CreateSimpleWindow(HINSTANCE hInstance, LPCSTR lpWindowName, int nWidth = 800, int nHeight = 600, WNDPROC lpfnWndProc = WindowProc, bool bShow = true, BYTE bAlpha = 0);
	LRESULT WINAPI _DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	void WaitForQuit();
}

#endif