#include<Windows.h>
#include"AirWindow2.h"
HWND asf;

void Creates(HINSTANCE hInstance, HWND hWnd){
	Air::CreateCheckBox(hInstance, hWnd, 0x101, "Test2", 3, 3);
}

void Commands(HINSTANCE hInstance, HWND hWnd, WORD wID){
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	Air::CreateMainWindow2(hInstance, "AW2Class", "AW2", 400, 300, Creates, Commands);

	//CreateMainWindow(hInstance, "AW22Class", "AW22", 400, 300, Creates, Commands);

	MessageBoxA(NULL, "Return", "WinMain", NULL);
	return 0;
}
