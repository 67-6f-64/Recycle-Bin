#include"AirWindow.h"

namespace Air{
	HCURSOR AWCursor = LoadCursorA(NULL, IDC_ARROW);

	void FixCursor(HWND hwnd, UINT uMsg);
	LRESULT CALLBACK AirWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void WaitForQuit();
	void StringCopy(char *src, char *s);
}

typedef struct tagProcFunc{
	bool IsMainWindow;
	void (*Create)(HWND);
	void (*Control)(WORD);
	void (*Notify)();
	AirWindow *AW;
} ProcFunc;

void Air::FixCursor(HWND hwnd, UINT uMsg){
		if(GetFocus() != hwnd){
		switch(uMsg){
			case WM_MOUSEMOVE://マウスがメインウィンドウ上を移動
				SetCursor(AWCursor);//カーソルを通常のアイコンに変更
				break;

			case WM_LBUTTONUP://メインウィンドウ上でクリックがされた
				SetFocus(hwnd);
				break;

			default:
				break;
		}
	}
}

void Air::WaitForQuit(){
	MSG lpMsg;
	while(GetMessageA(&lpMsg, NULL, NULL, NULL) > 0){
		TranslateMessage(&lpMsg);
		DispatchMessageA(&lpMsg);
	}
}

void Air::StringCopy(char *src, char *s){
	while(*s){
		*src++ = *s++;
	}
	*src = 0;
}

LRESULT CALLBACK Air::AirWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
		switch(uMsg){
			case WM_CREATE://GUI作成
				SetWindowLongA(hwnd, GWL_USERDATA, (LONG)(((CREATESTRUCT *)(lParam))->lpCreateParams));//must be written here
				((ProcFunc *)(((CREATESTRUCT *)(lParam))->lpCreateParams))->AW->SetData(((CREATESTRUCT *)(lParam))->hInstance, hwnd);
				if(((ProcFunc *)(((CREATESTRUCT *)(lParam))->lpCreateParams))->Create){
					((ProcFunc *)(((CREATESTRUCT *)(lParam))->lpCreateParams))->Create(hwnd);
				}
			
				break;

			case WM_COMMAND://GUI操作
				if(((ProcFunc *)(GetWindowLongA(hwnd, GWL_USERDATA)))->Control){
					((ProcFunc *)(GetWindowLongA(hwnd, GWL_USERDATA)))->Control(LOWORD(wParam));
				}
				break;

			case WM_NOTIFY:
				if(((ProcFunc *)(GetWindowLongA(hwnd, GWL_USERDATA)))->Notify){
					((ProcFunc *)(GetWindowLongA(hwnd, GWL_USERDATA)))->Notify();
				}
				break;

			case WM_CLOSE:
				if(!((ProcFunc *)(GetWindowLongA(hwnd, GWL_USERDATA)))->IsMainWindow){
					ShowWindow(hwnd, SW_HIDE);
					return 0;
				}
				break;

			case WM_DESTROY:
				if(((ProcFunc *)(GetWindowLongA(hwnd, GWL_USERDATA)))->IsMainWindow){
					PostQuitMessage(0);
				}
				break;

			default:
				FixCursor(hwnd, uMsg);
				break;
		}

	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

AirWindow::AirWindow(){
	hWndAW = NULL;
	hInstanceAW = NULL;
	InitCommonControls();
}

void AirWindow::SetData(HINSTANCE hInstance, HWND hWnd){
	hInstanceAW = hInstance;
	hWndAW = hWnd;
}

void AirWindow::AdjustWindow(int nWidth, int nHeight){
	RECT W_RECT, C_RECT;
	//枠を含めたウィンドウの角の座標を取得
	GetWindowRect(hWndAW, &W_RECT);
	//枠を含めない(クライアント領域)の角の座標を取得
	GetClientRect(hWndAW, &C_RECT);
	//指定サイズに変更
	SetWindowPos(hWndAW, HWND_TOP, NULL, NULL, (nWidth + (W_RECT.right-W_RECT.left)-(C_RECT.right-C_RECT.left)), (nHeight + (W_RECT.bottom-W_RECT.top)-(C_RECT.bottom-C_RECT.top)), SWP_NOMOVE);
}

void AirWindow::Init(HINSTANCE hInstance, LPSTR lpWindowName, int nWidth, int nHeight, bool IsMainWindow, void (*Create)(HWND), void (*Control)(WORD), void (*Notify)()){
	WNDCLASSEXA wc;
	ProcFunc *pf = (ProcFunc *)malloc(sizeof(ProcFunc));
	ZeroMemory(pf, sizeof(ProcFunc));

	ZeroMemory(&wc, sizeof(WNDCLASSEXA));

	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc =  Air::AirWindowProc;
	wc.lpszClassName = lpWindowName;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//wc.hIcon = LoadIconA(hInstance, "CCPLZ");//CCPLZ Icon

	if(!RegisterClassExA(&wc)){
		return;
	}

	pf->Create = Create;
	pf->Control = Control;
	pf->Notify = Notify;
	pf->AW = this;
	pf->IsMainWindow = IsMainWindow;

	Air::StringCopy(lpClassNameAW, lpWindowName);

	if(CreateWindowExA(NULL, lpWindowName, lpWindowName, (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, pf)){
		AdjustWindow(nWidth, nHeight);
		ShowWindow(hWndAW, SW_SHOW);
		if(IsMainWindow){
			Air::WaitForQuit();
			UnInit();
		}

	}
}

void AirWindow::UnInit(){
		SendMessageA(hWndAW, WM_CLOSE, 0, 0);
		UnregisterClassA(lpClassNameAW, hInstanceAW);
}

void AirWindow::Show(){
	ShowWindow(hWndAW, SW_SHOW);
}