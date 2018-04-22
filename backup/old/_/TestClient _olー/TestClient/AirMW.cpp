#include"AirWindow.h"

HCURSOR DefaultCursor = LoadCursorA(NULL, IDC_ARROW);

AirWindow::AirWindow(){
	hInstanceAW = NULL;
	hWndAW = NULL;
	IsBaseWindow = false;
	GuiControl = NULL;
}

//既定
LRESULT CALLBACK AirWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

	switch(msg){
		case WM_CREATE:
			//GuiControl(LOWORD(wParam));
			break;
		default:
			break;
	}
	return _DefWindowProcA(hwnd, msg, wParam, lParam);
}

//マウスカーソル修正
LRESULT WINAPI AirWindow::_DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){

	if(GetFocus() != hWnd){//キーフォーカスがメインウィンドウに無い
		switch(Msg){
			case WM_MOUSEMOVE://マウスがメインウィンドウ上を移動
				SetCursor(DefaultCursor);//カーソルを通常のアイコンに変更
				break;

			case WM_LBUTTONUP://メインウィンドウ上でクリックがされた
				SetFocus(hWnd);
				break;

			default:
				break;
		}
	}

	switch(Msg){//閉じるの防止(サブウィンドウ)
			case WM_DESTROY:
			case WM_CLOSE:

				if(IsBaseWindow){
					PostQuitMessage(0);
				}
				else{
					Msg = NULL;
					wParam = NULL;
					lParam = NULL;
					ShowWindow(hWnd, SW_HIDE);
				}
				break;

			default:
				break;
		}

	return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

//透明化
void AirWindow::SetLayer(BYTE bAlpha){
		//スタイル付加
		SetWindowLongA(hWndAW, GWL_EXSTYLE, GetWindowLongA(hWndAW, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWndAW, 0, ((bAlpha * 0x100) / 0x64), LWA_ALPHA);
		return;
}

//サイズ調整
void AirWindow::AdjustWindowSize(int nWidth, int nHeight){
	RECT W_RECT, C_RECT;
	//枠を含めたウィンドウの角の座標を取得
	GetWindowRect(hWndAW, &W_RECT);
	//枠を含めない(クライアント領域)の角の座標を取得
	GetClientRect(hWndAW, &C_RECT);
	//指定サイズに変更
	SetWindowPos(hWndAW, HWND_TOP, NULL, NULL, (nWidth + (W_RECT.right-W_RECT.left)-(C_RECT.right-C_RECT.left)), (nHeight + (W_RECT.bottom-W_RECT.top)-(C_RECT.bottom-C_RECT.top)), SWP_NOMOVE);
	return;
}

//ウィンドウ作成
bool AirWindow::CreateSimpleWindow(HINSTANCE hInstance, LPCSTR lpWindowName, int nWidth, int nHeight, void (*pFunction)(WORD), WNDPROC lpfnWndProc, bool bMain, BYTE bAlpha, bool bShow){
	WNDCLASSEXA wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEXA));
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;

	DWORD ndr;

	_asm{
		mov eax,AirWindow::WindowProc
		mov dword ptr [ndr],eax
	}
	
	if(!lpfnWndProc){
		wc.lpfnWndProc =  (WNDPROC)ndr;
	}
	else{
		wc.lpfnWndProc =  (WNDPROC)ndr;

	}

	wc.lpszClassName = lpWindowName;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = DefaultCursor;//なんとなくカーソルのアイコンにしてみる
	wc.hIconSm = DefaultCursor;

	//ウィンドウ登録
	if(!RegisterClassExA(&wc)){
		return false;
	}

	//関数設定
	GuiControl = pFunction;

	//ウィンドウ作成
	hWndAW = CreateWindowExA(NULL, lpWindowName, lpWindowName, (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);
	
	AdjustWindowSize(nWidth, nHeight);//サイズ変更

	//透明化
	if(bAlpha){
		SetLayer(bAlpha);
	}

	if(bShow){
		ShowWindow(hWndAW, SW_SHOW);//可視化
	}else{
		ShowWindow(hWndAW, SW_HIDE);
	}

	hInstanceAW = hInstance;

	if(bMain){
		IsBaseWindow = true;
	}

	return true;
}

//閉じるられるまで待機
void AirWindow::WaitForQuit(){
	MSG lpMsg;

	while(GetMessageA(&lpMsg, NULL, NULL, NULL) > 0){
		TranslateMessage(&lpMsg);
		DispatchMessageA(&lpMsg);
	}

	return;
}