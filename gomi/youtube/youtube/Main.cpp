#include<Windows.h>

HWND hWndParent = 0;
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam){
	char classname[0x100];

	if(GetClassNameA(hWnd, classname, sizeof(classname))){
		if(strcmp(classname, "MozillaWindowClass") == 0){
			char title[256];
			if(GetWindowTextA(hWnd, title, sizeof(title)) && strstr(title, "- YouTube")){
				SetParent(hWnd, hWndParent);
				return FALSE;
			}
		}
	}
	return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
		switch(uMsg){
			case WM_CREATE:
				{
					hWndParent = hWnd;
					EnumWindows(EnumWindowsProc, NULL);
					break;
				}
			case WM_COMMAND:
				break;
			case WM_NOTIFY:
				break;
			case WM_DESTROY:
					PostQuitMessage(0);
				break;
			default:
				break;
		}
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

void AdjustWindow(HWND hWnd, int nWidth, int nHeight){
	RECT W_RECT, C_RECT;
	//枠を含めたウィンドウの角の座標を取得
	GetWindowRect(hWnd, &W_RECT);
	//枠を含めない(クライアント領域)の角の座標を取得
	GetClientRect(hWnd, &C_RECT);
	//指定サイズに変更
	SetWindowPos(hWnd, HWND_TOP, NULL, NULL, (nWidth + (W_RECT.right - W_RECT.left) - (C_RECT.right - C_RECT.left)), (nHeight + (W_RECT.bottom - W_RECT.top) - (C_RECT.bottom - C_RECT.top)), SWP_NOMOVE);
	ShowWindow(hWnd, SW_SHOW);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int){
	WNDCLASSEXA wc = {0};

	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc =  WindowProc;
	wc.lpszClassName = "testClass";
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClassExA(&wc)){
		return 1;
	}

	HWND hWnd = CreateWindowExA(NULL, "testClass", "1920x1080", (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);

	if(hWnd){
		AdjustWindow(hWnd, 1920, 1080);

		MSG lpMsg;
		while(GetMessageA(&lpMsg, NULL, NULL, NULL) > 0){
			TranslateMessage(&lpMsg);
			DispatchMessageA(&lpMsg);
		}

		SendMessageA(hWnd, WM_CLOSE, 0, 0);
		UnregisterClassA("testClass", hInstance);
	}
	return 0;
}