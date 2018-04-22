#include"AirGUI_Main.h"

namespace Air{
	HCURSOR DefaultCursor = LoadCursorA(NULL, IDC_ARROW);
	BOOL ReSize(HWND hWnd, int nWidth, int nHeight);
	BOOL SetLayer(HWND hWnd, BYTE bAlpha);
	HWND hMain = 0;
}


//既定の引数
LRESULT CALLBACK Air::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

	switch(msg){
		case WM_CREATE:
			break;

		case WM_COMMAND:
			break;

		default:
			break;
	}

	return _DefWindowProcA(hwnd, msg, wParam, lParam);
}

//マウスカーソル修正
LRESULT WINAPI Air::_DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){

	if(GetFocus() != hWnd){//キーフォーカスがメインウィンドウに無い
		switch(Msg){
			case WM_MOUSEMOVE://マウスがメインウィンドウ上を移動
				SetCursor(DefaultCursor);//カーソルを通常のアイコンに変更
				break;

			case WM_LBUTTONUP://メインウィンドウ上でクリックがされた
				SetFocus(hWnd);
				break;

			case WM_DESTROY:
			case WM_CLOSE:
				if(hWnd == hMain){
					PostQuitMessage(0);
				}
				else{
					Msg = 0;
					wParam = 0;
					lParam = 0;
				}
				break;

			default:
				break;
		}
	}

	return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

//透明化関数
BOOL Air::SetLayer(HWND hWnd, BYTE bAlpha){
		//スタイル付加
		SetWindowLongA(hWnd, GWL_EXSTYLE, GetWindowLongA(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		return SetLayeredWindowAttributes(hWnd, 0, ((bAlpha * 0x100) / 0x64), LWA_ALPHA);
}


BOOL Air::ReSize(HWND hWnd, int nWidth, int nHeight){
	RECT W_RECT, C_RECT;
	//枠を含めたウィンドウの角の座標を取得
	GetWindowRect(hWnd, &W_RECT);
	//枠を含めない(クライアント領域)の角の座標を取得
	GetClientRect(hWnd, &C_RECT);
	//指定サイズに変更
	return SetWindowPos(hWnd, HWND_TOP, NULL, NULL, (nWidth + (W_RECT.right-W_RECT.left)-(C_RECT.right-C_RECT.left)), (nHeight + (W_RECT.bottom-W_RECT.top)-(C_RECT.bottom-C_RECT.top)), SWP_NOMOVE);
}


HWND Air::CreateSimpleWindow(HINSTANCE hInstance, LPCSTR lpWindowName, int nWidth, int nHeight, WNDPROC lpfnWndProc, bool bShow, BYTE bAlpha){
	WNDCLASSEXA wc;
	HWND hwnd;
	ZeroMemory(&wc, sizeof(WNDCLASSEXA));
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = lpfnWndProc;
	wc.lpszClassName = lpWindowName;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	wc.hIcon = DefaultCursor;//なんとなくカーソルのアイコンにしてみる
	wc.hIconSm = DefaultCursor;

	//ウィンドウ登録
	if(!RegisterClassExA(&wc)) return 0;
	//ウィンドウ作成
	hwnd = CreateWindowExA(NULL, lpWindowName, lpWindowName, (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);
	
	ReSize(hwnd, nWidth, nHeight);//サイズ変更

	//透明化
	if(bAlpha){
		SetLayer(hwnd, bAlpha);
	}

	if(bShow){
		ShowWindow(hwnd, SW_SHOW);//可視化
	}else{
		ShowWindow(hwnd, SW_HIDE);
	}

	if(!hMain){
		hMain = hwnd;
	}

	return hwnd;
}


void Air::WaitForQuit(){
	MSG lpMsg;

	while(GetMessageA(&lpMsg, NULL, NULL, NULL) > 0){
		TranslateMessage(&lpMsg);
		DispatchMessageA(&lpMsg);
	}

	return;
}