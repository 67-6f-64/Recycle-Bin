#include"AirWindow2.h"

namespace Air{
	HCURSOR DefaultCursor = LoadCursorA(NULL, IDC_ARROW);

	typedef struct DustStruct2{
		DWORD ThreadId;
		HWND hWnd;
		HINSTANCE hInstance;
		LPSTR lpClassName;
		LPSTR lpWindowName;
		int iWidth;
		int iHeight;
		void (*Creates)(HINSTANCE, HWND);
		void (*Commands)(HINSTANCE, HWND, WORD);
		void (*Notifies)(HINSTANCE, HWND);
	};

	void DefaultCreates(HINSTANCE hInstance, HWND hWnd);
	void DefaultCommands(HINSTANCE hInstance, HWND hWnd, WORD wID);
	void DefaultNotifies(HINSTANCE hInstance, HWND hWnd);
	void AdjustWindowSize(HWND hWnd, int nWidth, int nHeight);
	LRESULT CALLBACK DefaultProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	void _CreateMainWindow(DustStruct2 *DS2);
}


void Air::AdjustWindowSize(HWND hWnd, int nWidth, int nHeight){
	RECT W_RECT, C_RECT;
	//枠を含めたウィンドウの角の座標を取得
	GetWindowRect(hWnd, &W_RECT);
	//枠を含めない(クライアント領域)の角の座標を取得
	GetClientRect(hWnd, &C_RECT);
	//指定サイズに変更
	SetWindowPos(hWnd, HWND_TOP, NULL, NULL, (nWidth + (W_RECT.right-W_RECT.left) - (C_RECT.right-C_RECT.left)), (nHeight + (W_RECT.bottom-W_RECT.top) - (C_RECT.bottom-C_RECT.top)), SWP_NOMOVE);
}

//何もしない関数(既定値)
void Air::DefaultCreates(HINSTANCE hInstance, HWND hWnd){}
void Air::DefaultCommands(HINSTANCE hInstance, HWND hWnd, WORD wID){}
void Air::DefaultNotifies(HINSTANCE hInstance, HWND hWnd){}

//ダイアログ
LRESULT CALLBACK Air::DefaultProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){

	switch (Msg) {
		case WM_CREATE:
			AdjustWindowSize(hWnd, ((DustStruct2 *)*(DWORD *)lParam)->iWidth, ((DustStruct2 *)*(DWORD *)lParam)->iHeight);
			((DustStruct2 *)*(DWORD *)lParam)->Creates((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), hWnd);
			break;

		case WM_COMMAND:
			((DustStruct2 *)*(DWORD *)lParam)->Commands(((DustStruct2 *)*(DWORD *)lParam)->hInstance, hWnd, LOWORD(wParam));
			break;

		case WM_NOTIFY:
			((DustStruct2 *)*(DWORD *)lParam)->Notifies(((DustStruct2 *)*(DWORD *)lParam)->hInstance, hWnd);
			break;

		case WM_DESTROY:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			return 0;

		default:
			return DefWindowProcA(hWnd, Msg, wParam, lParam);
	}
	/*
	//マウス修正
	if(GetFocus() != hWnd){
		switch(Msg){
			case WM_MOUSEMOVE:
				SetCursor(DefaultCursor);
				break;

			case WM_LBUTTONUP:
				SetFocus(hWnd);
				break;

			default:
				break;
		}
	}
	*/
	return 0;
}

//スレッドで実行
void Air::_CreateMainWindow(DustStruct2 *DS2){
	MSG lpMsg;
	WNDCLASSEXA wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEXA));

	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = DS2->hInstance;
	wc.lpfnWndProc = DefaultProc;
	wc.lpszClassName = DS2->lpClassName;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassExA(&wc);

	DS2->hWnd = CreateWindowExA(NULL, DS2->lpClassName, DS2->lpWindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, DS2->hInstance, DS2);
	
	ShowWindow(DS2->hWnd, SW_SHOW);
	UpdateWindow(DS2->hWnd);

	ResumeThread(OpenThread(THREAD_ALL_ACCESS, NULL, DS2->ThreadId));

	while(GetMessageA(&lpMsg, NULL, NULL, NULL)){
		TranslateMessage(&lpMsg);
		DispatchMessageA(&lpMsg);
	}

	UnregisterClassA(DS2->lpClassName, DS2->hInstance);
}

//ウィンドウ作成
void Air::CreateMainWindow(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpWindowName, int iWidth, int iHeight, void (*Creates)(HINSTANCE, HWND), void (*Commands)(HINSTANCE, HWND, WORD), void (*Notifies)(HINSTANCE, HWND)){
	DustStruct2 DS2;

	DS2.ThreadId = GetCurrentThreadId();
	DS2.hInstance =  hInstance;
	DS2.lpClassName = lpClassName;
	DS2.lpWindowName = lpWindowName;
	DS2.iWidth = iWidth;
	DS2.iHeight = iHeight;
	DS2.Creates = Creates;//? Creates: DefaultCreates;
	DS2.Commands = Commands? Commands: DefaultCommands;
	DS2.Notifies = Notifies? Notifies: DefaultNotifies;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_CreateMainWindow, &DS2, NULL, NULL);

	SuspendThread(OpenThread(THREAD_ALL_ACCESS, NULL, GetCurrentThreadId()));
}
void Air::CreateMainWindow2(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpWindowName, int iWidth, int iHeight, void (*Creates)(HINSTANCE, HWND), void (*Commands)(HINSTANCE, HWND, WORD), void (*Notifies)(HINSTANCE, HWND)){
	DustStruct2 DS2;

	DS2.ThreadId = GetCurrentThreadId();
	DS2.hInstance =  hInstance;
	DS2.lpClassName = lpClassName;
	DS2.lpWindowName = lpWindowName;
	DS2.iWidth = iWidth;
	DS2.iHeight = iHeight;
	DS2.Creates = Creates;//? Creates: DefaultCreates;
	DS2.Commands = Commands? Commands: DefaultCommands;
	DS2.Notifies = Notifies? Notifies: DefaultNotifies;

	_CreateMainWindow(&DS2);

}