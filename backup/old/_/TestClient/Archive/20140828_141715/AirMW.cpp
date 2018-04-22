#include"AirWindow.h"

namespace Air{
	typedef struct WPA{
		HWND *hWnd;
		void (*GuiCreation)(HWND);
		void (*GuiControl)(WORD);
		void (*GuiNotify)(HWND);
		bool *IsBaseWindow;
	};

	HCURSOR DefaultCursor = LoadCursorA(NULL, IDC_ARROW);
	int WindowCount = 0;
	WPA FunctionList[MaxWindowCount] = {0};
}

void Air::DefaultGuiCreation(HWND hwnd){
	return;
}

void Air::DefaultGuiControl(WORD wID){
	return;
}

void Air::DefaultGuiNotify(HWND hWnd){
	return;
}

LRESULT CALLBACK Air::DefaultProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	int i;

	//検索
	for(i=0; *FunctionList[i].hWnd; i++){
		if(*FunctionList[i].hWnd == hwnd){
			break;
		}
	}

	//登録
	if(!*FunctionList[i].hWnd && (FunctionList[i].GuiCreation || FunctionList[i].GuiControl)){
		*FunctionList[i].hWnd = hwnd;
		if(Air::IsDebug()){
			Air::DebugMessage("hwnd = %d is created", hwnd);
		}
	}

	if(GetFocus() != hwnd){
		switch(uMsg){
			case WM_MOUSEMOVE://マウスがメインウィンドウ上を移動
				SetCursor(DefaultCursor);//カーソルを通常のアイコンに変更
				break;

			case WM_LBUTTONUP://メインウィンドウ上でクリックがされた
				SetFocus(hwnd);
				break;

			default:
				break;
		}
	}
	
	if(*FunctionList[i].hWnd){
		switch(uMsg){
			case WM_CREATE://GUI作成
				FunctionList[i].GuiCreation(hwnd);
				break;

			case WM_COMMAND://GUI操作
				FunctionList[i].GuiControl(LOWORD(wParam));
				break;

			case WM_NOTIFY:
				FunctionList[i].GuiNotify(GetFocus());
				break;

			case WM_DESTROY:
			case WM_CLOSE:
				if(*FunctionList[i].IsBaseWindow){
					PostQuitMessage(0);
				}
				else{
					uMsg = NULL;
					wParam = NULL;
					lParam = NULL;
					ShowWindow(hwnd, SW_HIDE);
					if(Air::IsDebug()){
						Air::DebugMessage("hwnd = %d is closed", hwnd);
					}
				}
				break;

			default:
				break;
		}

	}

	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

//閉じるられるまで待機
void Air::WaitForQuit(){
	MSG lpMsg;

	while(GetMessageA(&lpMsg, NULL, NULL, NULL) > 0){
		TranslateMessage(&lpMsg);
		DispatchMessageA(&lpMsg);
	}

	return;
}

void Air::WaitForSeconds(DWORD dwTime){
	MSG lpMsg;
	DWORD dwCurrentTime = GetTickCount();
	while(GetMessageA(&lpMsg, NULL, NULL, NULL) > 0){
		if(GetTickCount()-dwCurrentTime > dwTime*1000){
			break;
		}
		TranslateMessage(&lpMsg);
		DispatchMessageA(&lpMsg);
	}

	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

AirWindow::AirWindow(){
	hInstanceAW = NULL;
	hWndAW = NULL;
	IsBaseWindow = false;
}

HWND AirWindow::GetHWND(){
	return hWndAW;
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
bool AirWindow::CreateSimpleWindow(HINSTANCE hInstance, LPCSTR lpWindowName, int nWidth, int nHeight, void (*GuiCreation)(HWND), void(*GuiControl)(WORD), void(*GuiNotify)(HWND), bool bMain, BYTE bAlpha, bool bShow){
	WNDCLASSEXA wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEXA));

	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc =  Air::DefaultProc;
	wc.lpszClassName = lpWindowName;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//wc.hIcon = DefaultCursor;//なんとなくカーソルのアイコンにしてみる
	//wc.hIconSm = DefaultCursor;

	//ウィンドウ登録
	if(!RegisterClassExA(&wc)){
		return false;
	}

	//関数設定
	if(GuiCreation){
		Air::FunctionList[Air::WindowCount].GuiCreation = GuiCreation;
	}
	else{
		Air::FunctionList[Air::WindowCount].GuiCreation = Air::DefaultGuiCreation;
	}
	
	if(GuiControl){
		Air::FunctionList[Air::WindowCount].GuiControl = GuiControl;
	}
	else{
		Air::FunctionList[Air::WindowCount].GuiControl = Air::DefaultGuiControl;
	}

	if(GuiNotify){
		Air::FunctionList[Air::WindowCount].GuiNotify = GuiNotify;
	}
	else{
		Air::FunctionList[Air::WindowCount].GuiNotify = Air::DefaultGuiNotify;
	}

	Air::FunctionList[Air::WindowCount].hWnd = &hWndAW;

	hInstanceAW = hInstance;

	if(bMain){
		IsBaseWindow = true;
	}

	Air::FunctionList[Air::WindowCount].IsBaseWindow = &IsBaseWindow;

	//ウィンドウ作成
	CreateWindowExA(NULL, lpWindowName, lpWindowName, (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);
	
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

	Air::WindowCount++;

	return true;
}

void AirWindow::CloseSimpleWindow(){
	ShowWindow(hWndAW, SW_HIDE);
}