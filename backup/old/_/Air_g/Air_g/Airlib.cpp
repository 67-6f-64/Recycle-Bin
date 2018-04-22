#include"Airlib.h"


bool Air::EmebedWindow(HWND hWndNewParent, LPCSTR lpWindowName, int nWidth, int nHeight){
	HWND hWndChild = FindWindowA(NULL, lpWindowName);

	if(!hWndChild){
		return false;
	}

	SetWindowLongA(hWndChild, GWL_STYLE, WS_CHILD|WS_POPUP);
	ShowWindow(hWndChild, SW_SHOW);
	SetParent(hWndChild, hWndNewParent);
	SetWindowPos(hWndChild, HWND_TOP, 0, 0, nWidth, nHeight, NULL);

	return true;
}
