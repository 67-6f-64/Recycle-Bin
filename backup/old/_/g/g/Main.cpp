#include<Windows.h>
#include"Air.h"
#include"blabla.h"
#include"Packet_GUI.h"
//GUISTART

void CreateControls(HWND hWndParent){
	Air::CreateStaticText(hWndParent, 0x900, "Log Window", 3 + 100, 3);
	Air::CreateEdit(hWndParent, 0x201, NULL, 3, 3+20, 300, false, 10);
	Air::ReadOnly(hWndParent, 0x201);
	
	Air::CreateCheckBox(hWndParent, 0x100, "HackShield", 3 + 300 + 10, 3 + 20, true);
	Air::CreateCheckBox(hWndParent, 0x101, "MSCRC", 3 + 300 + 10, 3 + 20*2, true);
	Air::CreateCheckBox(hWndParent, 0x102, "ProcessList", 3 + 300 + 10, 3 + 20*3, true);
	Air::CreateCheckBox(hWndParent, 0x103, "HardwareID", 3 + 300 + 10, 3 + 20*4, true);
	//Air::AddFormatString(hWndParent, 0x201, "test1 String = %s\r\ntest2 DWORD = %d\r\ntest3 WORD = %w\r\ntest4 INT(Positive) = %i\r\ntest5 INT(Negative) = %i", "ABCDEF", 0x00001337, 0x1337, 1337, -1337);
	
	DWORD NewMemory;
	Air::AddFormatString(hWndParent, 0x201, "Memory is allocated at %d", NewMemory = Alloc(0x100));
	Write1(&NewMemory, 0xE9);
	Write4(&NewMemory, 0x12345678);
	Write2(&NewMemory, 0x0023);
	char Temp[0x100];
	Air::CreateFormatString(Temp, "DWORD = %d, WORD = %w\r\nBYTE = %b, int = %i\r\nString = %s", 0x12345678, 0x0123, 0x12, 12345, "blabla");
	Air::AddString(hWndParent, 0x201, Temp);
}

void Commands(HWND hwnd, WORD wID){
	switch(wID){
		case 0x100:
			if(IsDlgButtonChecked(hwnd, wID)){
				//ShowWindow(Air::hWndList[1], SW_SHOW);
			}else{
				//ShowWindow(Air::hWndList[1], SW_HIDE);
			}
			break;

		default:
			break;
	}
}

LRESULT CALLBACK AirProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			CreateControls(hwnd);
			break;

		case WM_COMMAND:
			Commands(hwnd, LOWORD(wParam));
			break;

		default:
			break;
	}

	return Air::_DefWindowProcA(hwnd, msg, wParam, lParam);
}
//GUIEND

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){

	Air::SetMainInstance(hInstance);
	Air::CreateSimpleWindow(hInstance, "Air Bypass", 400, 300, AirProc);
	PacketEditorRun(hInstance);
	Air::WaitForQuit();

	return 0;
}