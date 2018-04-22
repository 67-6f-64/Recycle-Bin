#include<Windows.h>
#include"MainWindow.h"
#include"SubWindow.h"
#include"ListView.h"

void CreateControls(HWND hwnd){
	Air::SubWindow SW;

	SW.Init(hwnd);
	SW.CreateCheckBox(0x100, "Scripter1", 800+20, 20+20*1);
	SW.CreateCheckBox(0x101, "ListView1", 800+20, 20+20*2);
	SW.CreateCheckBox(0x102, "checkbox3", 800+20, 20+20*3);
	SW.CreateButton(0x200, "Button1", 800+20, 20+20*4);
	SW.CreateEdit(0x300, "Edit1", 800+20, 20+20*5);
	SW.CreateEdit(0x301, "Edit2", 800+20, 20+20*6);
	SW.CreateStaticText(0x400, "StaticText", 800+20, 20+20*7);
	SW.CreateEdit(0x500, "Multi\r\nLine\r\nPlease\r\n", 800+20, 20+20*8, 100, false, 5);
	SW.AddString(0x500, "faf");
}

HWND h2, h3;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			CreateControls(hwnd);
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case 0x100:
						if(IsDlgButtonChecked(hwnd, LOWORD(wParam))){
							ShowWindow(h2, SW_SHOW);
						}else{
							ShowWindow(h2, SW_HIDE);
						}
					break;

				case 0x101:
						if(IsDlgButtonChecked(hwnd, LOWORD(wParam))){
							ShowWindow(h3, SW_SHOW);
						}else{
							ShowWindow(h3, SW_HIDE);
						}
					break;

				default:
					break;
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			break;
	}

	return Air::_DefWindowProcA(hwnd, msg, wParam, lParam);
}

Air::SubWindow SW2;

void CreateControls2(HWND hwnd){

	SW2.Init(hwnd);
	SW2.CreateEdit(0x500, "", 5, 5, 390, false, 20);
	SW2.CreateStaticText(0x100, "Script Editor", 5, 250, 390);
	SW2.CreateButton(0x200, "Execute", 140, 270, 100);
	//SW.ReadOnly(0x500, true);
	//SW.AddString(0x500, "faf");
}

void Commands2(WORD wUID){
	switch(wUID){
		case 0x200:
			//SW2.ReadOnly(0x500);
			SW2.SetString(0x100, SW2.GetString(0x500));
			break;

		default:
			break;
	}
}


LRESULT CALLBACK WndProc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			CreateControls2(hwnd);
			break;

		case WM_COMMAND:
			Commands2(LOWORD(wParam));
			break;

		case WM_DESTROY:
		case WM_CLOSE:
			wParam = NULL;
			lParam = NULL;
			msg = NULL;
			ShowWindow(hwnd, SW_HIDE);
			break;

		default:
			break;
	}

	return Air::_DefWindowProcA(hwnd, msg, wParam, lParam);
}


void CreateControls3(HWND hwnd){
	Air::ListView LV;
	LPSTR lpString[3] ={"Offset0","Offset1","Offset2"};

	LV.Init(hwnd, 3, 3, 400-6, 300-6);
	LV.CreateHeader("Header0", 100);
	LV.CreateHeader("Header1", 100);
	LV.CreateHeader("Header2", 150);
	
	for(int i=0; i<0x10; i++){
		LV.AddList(lpString);
	}

	//LV.Clear();
	LV.GetList(0, 0);
}


LRESULT CALLBACK WndProc3(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			CreateControls3(hwnd);
			break;

		case WM_COMMAND:
			//Commands2(LOWORD(wParam));
			break;

		case WM_DESTROY:
		case WM_CLOSE:
			wParam = NULL;
			lParam = NULL;
			msg = NULL;
			ShowWindow(hwnd, SW_HIDE);
			break;

		default:
			break;
	}

	return Air::_DefWindowProcA(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){

	h2 = Air::CreateSimpleWindow(hInstance, "Scripter", 400, 300, WndProc2);
	h3 = Air::CreateSimpleWindow(hInstance, "ListWindow1", 400, 300, WndProc3);
	Air::CreateSimpleWindow(hInstance, "SimpleWindow1", 800 + 200, 600, WndProc);

	Air::WaitForQuit();

	return 0x1337;
}
