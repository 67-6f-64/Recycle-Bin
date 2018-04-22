#include<Windows.h>
#include"Air.h"

//GUISTART

void CreateControls(HWND hWndParent){
	Air::CreateCheckBox(hWndParent, 0x100, "CheckBox1", 3, 3);
	Air::CreateEdit(hWndParent, 0x200, "EDIT", 3, 3 + 20*1, 100);
	Air::CreateEdit(hWndParent, 0x201, NULL, 3, 3 + 20*2, 200, false, 10);
	Air::AddString(hWndParent, 0x201, "******** LogWindow ********", true);
	Air::AddString(hWndParent, 0x201, "123", true);
	Air::ReadOnly(hWndParent, 0x201);
	Air::AddFormatString(hWndParent, 0x201, "test1 String = %s\r\ntest2 DWORD = %d\r\ntest3 WORD = %w\r\ntest4 INT(Positive) = %i\r\ntest5 INT(Negative) = %i", "ABCDEF", 0x00001337, 0x1337, 1337, -1337);
}

LRESULT CALLBACK AirProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			CreateControls(hwnd);
			break;

		default:
			break;
	}

	return Air::_DefWindowProcA(hwnd, msg, wParam, lParam);
}
//GUIEND

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){

	Air::SetMainInstance(hInstance);
	Air::CreateSimpleWindow(hInstance, "Main", 400, 300, AirProc);
	Air::CreateSimpleWindow(hInstance, "Sub", 400, 300);
	Air::WaitForQuit();

	return 0;
}