#include<Windows.h>
//#pragma comment(lib, "Airgui.lib")

namespace Airgui{
		BOOL CreateNewDialog(HINSTANCE hInstance, char Title[], int nWidth, int nHeight, WNDPROC lpfnWndProc);
		void SetFont(HWND hwnd, int fsize, char fname[]);
		HWND SetButton(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight);
		HWND SetCheck(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight);
		HWND SetStatic(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight);
		HWND SetEdit(HWND hwnd, int INDEX, char Caption[], int x, int y, int nWidth, int nHeight);
		void Test(HINSTANCE hInstance);
		void SetLayer(HWND hwnd, int invisible);
		BOOL ShowBitmap(HINSTANCE hInstance, HWND hwnd, char file[], int x, int y);
		//
		BOOL SplashImage(HINSTANCE hInstance, HWND hwnd, char file[]);
		BOOL SplashWindow(HINSTANCE hInstance, char Title[], int nWidth, int nHeight, WNDPROC lpfnWndProc);
}


/* Sample Code (don't use this function name)

#define CHECK1 101
#define CHECK2 102
#define CHECK3 103
#define CHECK4 104
#define CHECK5 105

namespace Airgui{
	LRESULT CALLBACK WndProc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Dialog_Test(HINSTANCE hInstance);
}

LRESULT CALLBACK Airgui::WndProc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
  switch(msg)  {
    case WM_CREATE:
			SetCheck(hwnd, CHECK1, "CheckBox1", 3, 30, 200, 20);

			SetCheck(hwnd, CHECK2, "CheckBox2", 3, 60, 200, 20);
		
			SetCheck(hwnd, CHECK3, "CheckBox3", 3, 90, 200, 20);
		
			SetCheck(hwnd, CHECK4, "CheckBox4", 3, 120, 200, 20);

			SetCheck(hwnd, CHECK5, "CheckBox5", 3, 150, 200, 20);
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam)){
				
			case CHECK1:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					//checked
				}else{
					//unchecked
				}
				break;

			case CHECK2:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
				}else{
				}
				break;

			case CHECK3:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
				}else{
				}
				break;

			case CHECK4:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
				}else{
				}
				break;

			case CHECK5:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
				}else{
				}
				break;

  }
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		break;
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}



void Airgui::Dialog_Test(HINSTANCE hInstance){
	MSG msg;

	OpenWindow(hInstance, "Airgui Test", 400, 300, WndProc1);

	//•Â‚¶‚È‚¢‚æ‚¤‚Éƒ‹[ƒv
	while(GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


void Airgui::Test(HINSTANCE hInstance){
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Dialog_Test, hInstance, NULL, NULL);
}


*/