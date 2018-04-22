#include"Airgui.h"

namespace Airgui{
	void SetFont(HWND hwnd, int fsize, char fname[]);
}

/*namespace Airgui{
	void CenterWindow(HWND hwnd);
}


BOOL Airgui::SplashImage(HINSTANCE hInstance, HWND hwnd, char file[]){
        HDC hBmpDC, hdc;
        HBITMAP hBitmap;
        BITMAP bm;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hwnd, &ps);
		hBitmap = LoadBitmapA(hInstance, file);

		if(hBitmap==NULL){
			return FALSE;
		}

		hBmpDC = CreateCompatibleDC(hdc);
		SelectObject(hBmpDC, hBitmap);
        GetObject(hBitmap, sizeof(bm), &bm);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hBmpDC, 0, 0, SRCCOPY);

		//BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hBmpDC, 0, 0, SRCAND);

		DeleteDC(hBmpDC);
		DeleteObject(hBitmap);

        EndPaint(hwnd, &ps);

	return TRUE;
}

void Airgui::CenterWindow(HWND hwnd){
	int  x, y;
	int  nScreenWidth, nScreenHeight;
	RECT rc;

	GetWindowRect(hwnd, &rc);
	
	nScreenWidth  = GetSystemMetrics(SM_CXFULLSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	x = (nScreenWidth - (rc.right - rc.left)) / 2;
	y = (nScreenHeight - (rc.bottom - rc.top)) / 2;

	SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}


BOOL Airgui::SplashWindow(HINSTANCE hInstance, char Title[], int nWidth, int nHeight, WNDPROC lpfnWndProc){
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
	//wc.hbrBackground	= (HBRUSH)COLOR_APPWORKSPACE;
	wc.hbrBackground	= (HBRUSH)COLOR_WINDOW;

	if(!RegisterClassEx(&wc)){
		return FALSE;
	}

	//ウィンドウ作成
	hwnd = CreateWindowExA(NULL, Title, Title, WS_POPUP , CW_USEDEFAULT, CW_USEDEFAULT, nWidth, nHeight, NULL, NULL, hInstance, NULL);

	//ウィンドウ領域計算
	GetWindowRect(hwnd, &W_RECT);
	GetClientRect(hwnd, &C_RECT);
	V_WIDTH	= (W_RECT.right - W_RECT.left) - (C_RECT.right - C_RECT.left);
	V_HEIGHT = (W_RECT.bottom - W_RECT.top) - (C_RECT.bottom - C_RECT.top);

	//クライアント領域+枠のサイズ決定
	SetWindowPos(hwnd, HWND_TOP, NULL, NULL, (nWidth + V_WIDTH), (nHeight + V_HEIGHT), SWP_NOMOVE);
	CenterWindow(hwnd);
	//if(invisible !=255){
	//	SetLayer(hwnd, invisible);
	//}
	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);//半透明化

	return TRUE;
}
*/

BOOL Airgui::ShowBitmap(HINSTANCE hInstance, HWND hwnd, char file[], int x, int y){
        HDC hBmpDC, hdc;
        HBITMAP hBitmap;
        BITMAP bm;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hwnd, &ps);
		hBitmap = LoadBitmapA(hInstance, file);

		if(hBitmap==NULL){
			return FALSE;
		}

		hBmpDC = CreateCompatibleDC(hdc);
		SelectObject(hBmpDC, hBitmap);
        GetObject(hBitmap, sizeof(bm), &bm);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hBmpDC, 0, 0, SRCAND);

		//BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hBmpDC, 0, 0, SRCAND);

		DeleteDC(hBmpDC);
		DeleteObject(hBitmap);

        EndPaint(hwnd, &ps);

	return TRUE;
}

void Airgui::SetLayer(HWND hwnd, int invisible){
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hwnd, 0, (255 * invisible) / 100, LWA_ALPHA);
}

BOOL Airgui::CreateNewDialog(HINSTANCE hInstance, char Title[], int nWidth, int nHeight, WNDPROC lpfnWndProc){
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
	//wc.hbrBackground	= (HBRUSH)COLOR_APPWORKSPACE;
	wc.hbrBackground	= (HBRUSH)COLOR_WINDOW;

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

	//if(invisible !=255){
	//	SetLayer(hwnd, invisible);
	//}
	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);//半透明化

	return TRUE;
}

void Airgui::SetFont(HWND hwnd, int fsize = 12, char fname[] = "ＭＳ ゴシック"){//12,"ＭＳ ゴシック
	HFONT NewFONT = CreateFont(fsize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, fname);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)NewFONT, MAKELPARAM(FALSE, 0));
}


HWND Airgui::SetButton(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight){
	HWND newhwnd;
		newhwnd = CreateWindowExA(NULL, "BUTTON", Caption, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, nWidth, nHeight, hwnd, (HMENU)INDEX, (HINSTANCE)GetWindowLongA(hwnd, GWL_HINSTANCE), NULL);
		SetFont(newhwnd);
	return newhwnd;
}


HWND Airgui::SetCheck(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight){
	HWND newhwnd;
		newhwnd = CreateWindowExA(NULL, "BUTTON", Caption, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, x, y, nWidth, nHeight, hwnd, (HMENU)INDEX, (HINSTANCE)GetWindowLongA(hwnd, GWL_HINSTANCE), NULL);
		SetFont(newhwnd);
	return newhwnd;
}


HWND Airgui::SetStatic(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight){
	HWND newhwnd;
		newhwnd = CreateWindowExA(NULL, "STATIC", Caption, WS_CHILD | WS_VISIBLE | SS_RIGHT, x, y, nWidth, nHeight, hwnd, (HMENU)INDEX, (HINSTANCE)GetWindowLongA(hwnd, GWL_HINSTANCE), NULL);
		SetFont(newhwnd);
	return newhwnd;
}


HWND Airgui::SetEdit(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight){
	HWND newhwnd;
		newhwnd = CreateWindowExA(NULL, "EDIT", Caption, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER, x, y, nWidth, nHeight, hwnd, (HMENU)INDEX, (HINSTANCE)GetWindowLongA(hwnd, GWL_HINSTANCE), NULL);
		SetFont(newhwnd);
	return newhwnd;
}