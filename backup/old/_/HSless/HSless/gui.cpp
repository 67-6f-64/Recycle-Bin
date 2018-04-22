#include"gui.h"
#include"lib/Air.h"
#pragma comment(lib, "lib/AirLib2.lib")
#include"HSless.h"

#define LogViewer 0x200

void CreateControls(HWND hWndParent){
	Air::CreateStaticText(hWndParent, 0x900, "Log Window", 3 + 100, 3);
	Air::CreateEdit(hWndParent, LogViewer, NULL, 3, 3 + 20, 400-6, false, 20);
	Air::ReadOnly(hWndParent, LogViewer, true);
	Air::AddFormatString(hWndParent, LogViewer, "HSless");
	ScanAddresses(hWndParent, LogViewer);
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


void MainGUI(HINSTANCE hInstance){
	Air::SetMainInstance(hInstance);
	Air::CreateSimpleWindow(hInstance, "HSless", 400, 300, AirProc);
	Air::WaitForQuit();
}