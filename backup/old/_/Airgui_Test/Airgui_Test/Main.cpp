#include<Windows.h>
#include"Airgui.h"

#define CHECK1 101
#define CHECK2 102
#define CHECK3 103
#define CHECK4 104
#define CHECK5 105

#define STATIC1 1

#define EDIT1 201

#define BUTTON1 301

HINSTANCE hMain;

LRESULT CALLBACK WndProc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

  switch(msg)  {
    case WM_CREATE:
			//Airgui::SetLayer(hwnd, 80);

			Airgui::SetCheck(hwnd, CHECK1, "CheckBox1", 3, 30, 100, 20);

			Airgui::SetCheck(hwnd, CHECK2, "CheckBox2", 3, 60, 100, 20);
		
			Airgui::SetCheck(hwnd, CHECK3, "CheckBox3", 3, 90, 100, 20);
		
			Airgui::SetCheck(hwnd, CHECK4, "CheckBox4", 3, 120, 100, 20);
			
			Airgui::SetCheck(hwnd, CHECK5, "CheckBox5", 3, 150, 100, 20);
			
			Airgui::SetStatic(hwnd, STATIC1, "MapID:910000000", 3, 0, 100, 15);
			
			Airgui::SetEdit(hwnd, EDIT1, "test", 3, 180, 100, 18);

			Airgui::SetButton(hwnd, BUTTON1, "button", 113, 180, 80, 20);
		break;


	case WM_PAINT:
		Airgui::ShowBitmap(hMain, hwnd, "IMAGE1", 0, 0);
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


LRESULT CALLBACK WndProc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

  switch(msg)  {
    case WM_CREATE:
			Airgui::SetLayer(hwnd, 80);
		break;


	case WM_PAINT:
		Airgui::SplashImage(hMain, hwnd, "SPLASH1");
		break;


	case WM_COMMAND:
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	case WM_CLOSE:
		break;
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL Flag = FALSE;

void Timer(){
	Sleep(2000);
	Flag = TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	MSG msg;

	hMain = hInstance;

	//Airgui::CreateNewDialog(hInstance, "Airgui Test", 350, 334, WndProc1);
	Airgui::SplashWindow(hInstance, "FuckShield", 200, 149, WndProc2);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Timer, NULL, NULL, NULL);
	//•Â‚¶‚È‚¢‚æ‚¤‚Éƒ‹[ƒv
	while(GetMessage(&msg, NULL, 0, 0) > 0 && Flag == FALSE){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return 0;
}