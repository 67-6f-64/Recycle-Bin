#include"AIR_GUI.h"


BOOL AIR::OpenWindow(HINSTANCE hInstance, char Title[], int nWidth, int nHeight, WNDPROC lpfnWndProc){
	WNDCLASSEX wc;
	HWND hwnd;
	RECT W_RECT, C_RECT;
	int V_WIDTH, V_HEIGHT;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	//ウィンドウ情報
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = lpfnWndProc;
	wc.lpszClassName = Title;
	wc.style		= CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground	= (HBRUSH)COLOR_APPWORKSPACE;

	if(!RegisterClassEx(&wc)){
		return FALSE;
	}

	//ウィンドウ作成
	hwnd = CreateWindowExA(NULL, Title, Title, (WS_EX_LAYERED | WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, nWidth, nHeight, NULL, NULL, hInstance, NULL);

	//ウィンドウ領域計算
	GetWindowRect(hwnd, &W_RECT);
	GetClientRect(hwnd, &C_RECT);
	V_WIDTH	= (W_RECT.right - W_RECT.left) - (C_RECT.right - C_RECT.left);
	V_HEIGHT = (W_RECT.bottom - W_RECT.top) - (C_RECT.bottom - C_RECT.top);

	//クライアント領域+枠のサイズ決定
	SetWindowPos(hwnd, HWND_TOP, NULL, NULL, (nWidth + V_WIDTH), (nHeight + V_HEIGHT), SWP_NOMOVE);

	//SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	//SetLayeredWindowAttributes(hwnd, 0, (255 * 80) / 100, LWA_ALPHA);
	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);

	return TRUE;
}

HFONT AIR::GetFont(int fsize, char fname[]){//12,"ＭＳ ゴシック
	return CreateFont(fsize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, fname);
}


HWND AIR::SetButton(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight, int fsize, char fname[]){
	HWND newhwnd;
		newhwnd = CreateWindowExA(NULL, "BUTTON", Caption, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, nWidth, nHeight, hwnd, (HMENU)INDEX, (HINSTANCE)GetWindowLongA(hwnd, GWL_HINSTANCE), NULL);
		SendMessage(newhwnd, WM_SETFONT, (WPARAM)AIR::GetFont(fsize, fname), MAKELPARAM(FALSE, 0));//フォント変更
	return newhwnd;
}

HWND AIR::SetCheck(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight, int fsize, char fname[]){
	HWND newhwnd;
		newhwnd = CreateWindowExA(NULL, "BUTTON", Caption, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, x, y, nWidth, nHeight, hwnd, (HMENU)INDEX, (HINSTANCE)GetWindowLongA(hwnd, GWL_HINSTANCE), NULL);
		SendMessage(newhwnd, WM_SETFONT, (WPARAM)AIR::GetFont(fsize, fname), MAKELPARAM(FALSE, 0));
	return newhwnd;
}

HWND AIR::SetStatic(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight, int fsize, char fname[]){
	HWND newhwnd;
		newhwnd = CreateWindowExA(NULL, "STATIC", Caption, WS_CHILD | WS_VISIBLE | SS_LEFT, x, y, nWidth, nHeight, hwnd, (HMENU)INDEX, (HINSTANCE)GetWindowLongA(hwnd, GWL_HINSTANCE), NULL);
		SendMessage(newhwnd, WM_SETFONT, (WPARAM)AIR::GetFont(fsize, fname), MAKELPARAM(FALSE, 0));
	return newhwnd;
}

HWND AIR::SetEdit(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight, int fsize, char fname[]){
	HWND newhwnd;
		newhwnd = CreateWindowExA(NULL, "EDIT", Caption, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER, x, y, nWidth, nHeight, hwnd, (HMENU)INDEX, (HINSTANCE)GetWindowLongA(hwnd, GWL_HINSTANCE), NULL);
		SendMessage(newhwnd, WM_SETFONT, (WPARAM)AIR::GetFont(fsize, fname), MAKELPARAM(FALSE, 0));
	return newhwnd;
}