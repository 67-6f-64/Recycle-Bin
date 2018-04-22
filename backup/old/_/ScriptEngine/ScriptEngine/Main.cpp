#include<Windows.h>
#include"Air.h"
#include"Script.h"

Air::SubWindow SW;

void CreateControls(HWND hwnd){
	SW.Init(hwnd);
	SW.CreateStaticText(0x100, "Source", 5, 5);
	SW.CreateEdit(0x200, "", 5, 5 + 20, 390, false, 20);
	SW.CreateStaticText(0x101, "Debug", 5 + 400, 5);
	SW.CreateEdit(0x201, "", 5 + 400, 5 + 20, 390, false, 20);
	SW.CreateButton(0x300, "Execute", 140, 290, 100);
}

void Commands(WORD wUID){
		switch(wUID){
			case 0x300:
					SW.SetString(0x201, NULL);
					Scripter(SW.GetString(0x200));
				break;

			default:
				break;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			CreateControls(hwnd);
			break;

		case WM_COMMAND:
			Commands(LOWORD(wParam));
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			break;
	}

	return Air::_DefWindowProcA(hwnd, msg, wParam, lParam);
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){

	Air::CreateSimpleWindow(hInstance, "Script Engine", 800, 600, WndProc);
	Air::WaitForQuit();

	return 0x1337;
}