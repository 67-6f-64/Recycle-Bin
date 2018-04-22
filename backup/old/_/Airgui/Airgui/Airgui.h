/*
=======================================================================

	AIRRIDE's Simple C++ (Win32API) Gui Library v2.01 (2013/12/15)

	This library uses Windows.h and richedit.h.

	richedit.h is used for setting Controls into ReadOnly.

	Controls are created by CreateWindowExA.

=======================================================================
*/


#ifndef __Airgui_H__
#define __Airgui_H__

#include<Windows.h>
#include<richedit.h>
//#pragma comment(lib, "lib/Airgui.lib")

namespace Airgui{
		BOOL CreateNewDialog(HINSTANCE hInstance, char Title[], int nWidth, int nHeight, WNDPROC lpfnWndProc);
		HWND SetButton(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth = 100, int nHeight = 15);
		HWND SetCheck(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth = 0, int nHeight = 15);
		HWND SetStatic(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth = 0, int nHeight = 12);
		HWND SetEdit(HWND hwnd, int INDEX, char Caption[],int x, int y, int nWidth = 100, int nHeight = 16);
		void SetLayer(HWND hwnd, int invisible);
		BOOL ShowBitmap(HINSTANCE hInstance, HWND hwnd, char file[], int x = 0, int y = 0);
		HWND SetNumberEdit(HWND hwnd, int INDEX, char Caption[],int x, int y, int nWidth = 100, int nHeight = 16);
		HWND SetMultiLineEdit(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight = 32);
}

#endif