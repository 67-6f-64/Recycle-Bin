#include<Windows.h>
#include"AirWindow.h"

AirWindow MW;

void GuiControl(WORD wID){
	switch(wID){
		case 0x200:
			MW.SetString(0x100, NULL);
			break;
		default:
			break;
	}
}
/*
LRESULT CALLBACK AirProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_COMMAND:
			GuiControl(LOWORD(wParam));
			break;

		default:
			break;
	}

	return MW._DefWindowProcA(hwnd, msg, wParam, lParam);
}
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	MW.CreateSimpleWindow(hInstance, "TestClient", 400, 300, GuiControl, (WNDPROC)1);
	MW.CreateEditBox(0x100, "sdgsr", 3, 3, 400-6, false, 10);
	MW.CreateButton(0x200, "Press!", 3, 200);
	MW.SetString(0x100,"‚ ‚¢‚¤‚¦‚¨\r\n");

	MW.AddFormatString(0x100, "‚Ä‚·‚Æ %i %w %s %d", 123, 0x1337, "‚ ‚¤‚ ‚¤‚ ", 0x1234567);
	MW.WaitForQuit();

	return 0;
}
