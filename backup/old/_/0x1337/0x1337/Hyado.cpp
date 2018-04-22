#include"Hyado.h"

LRESULT CALLBACK TestWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			break;
		case WM_COMMAND:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}


void SetLayer(HWND hwnd, int invisible){
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hwnd, 0, (255 * invisible) / 100, LWA_ALPHA);
}

void WindowResize(HWND hwnd, int Width, int Height){
	RECT W_RECT, C_RECT;
	//枠を含めたウィンドウの角の座標を取得
	GetWindowRect(hwnd, &W_RECT);
	//枠を含めない(クライアント領域)の角の座標を取得
	GetClientRect(hwnd, &C_RECT);
	//指定サイズに変更
	SetWindowPos(hwnd, HWND_TOP, NULL, NULL, (Width + (W_RECT.right-W_RECT.left)-(C_RECT.right-C_RECT.left)), (Height + (W_RECT.bottom-W_RECT.top)-(C_RECT.bottom-C_RECT.top)), SWP_NOMOVE);
}

bool CreateSimpleWindow(HINSTANCE hInstance, char *Caption, int Width, int Height, WNDPROC lpfnWndProc, int invisible){
	WNDCLASSEXA wc;
	HWND hwnd;
	MSG lpMsg;
	ZeroMemory(&wc, sizeof(WNDCLASSEXA));
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = lpfnWndProc;
	wc.lpszClassName = Caption;
	wc.style = CS_HREDRAW|CS_VREDRAW;// | CS_DBLCLKS;

	//ウィンドウ登録
	if(!RegisterClassExA(&wc)) return false;
	//ウィンドウ作成
	hwnd = CreateWindowExA(NULL, Caption, Caption, (WS_EX_LAYERED | WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);
	WindowResize(hwnd, Width, Height);//サイズ変更
	if(invisible) SetLayer(hwnd, invisible);
	ShowWindow(hwnd, SW_SHOW);//可視化

	//閉じる処理が来るまで待機ですが注意がありますので読んでね
	/*
		日本語変換ウィンドウはhwndとは別なので
		指定してしまうのはよろしくないです(日本語変換ウィンドウが可視化されなくなる)
		英語だけの場合はhwndで固定してやっていいと思いますよ
	*/
	while(GetMessageA(&lpMsg, NULL, NULL, NULL) > 0){
		TranslateMessage(&lpMsg);
		DispatchMessageA(&lpMsg);
	}

	return true;
}
