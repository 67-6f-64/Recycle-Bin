#include<Windows.h>

namespace AIR{
		BOOL OpenWindow(HINSTANCE hInstance, char Title[], int nWidth, int nHeight, WNDPROC lpfnWndProc);
		HWND SetButton(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight, int fsize = 12, char fname[] = "�l�r �S�V�b�N");
		HWND SetCheck(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight, int fsize = 12, char fname[] = "�l�r �S�V�b�N");
		HWND SetStatic(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight, int fsize = 12, char fname[] = "�l�r �S�V�b�N");
		HWND SetEdit(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight, int fsize = 12, char fname[] = "�l�r �S�V�b�N");
		HFONT GetFont(int fsize, char fname[]);
}
