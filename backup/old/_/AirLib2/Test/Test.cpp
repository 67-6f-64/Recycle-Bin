#include"Test.h"

#include"../Release/Air.h"
#pragma comment(lib, "../Release/AirLib2.lib")

#include<psapi.h>
#pragma comment(lib, "psapi.lib")
//IDs
#define __EDIT_LOG 0x200
#define __BUTTON_REFRESH 0x300
//

DWORD GetAvailableProcessCount(){
	DWORD lpidProcess[0x1000] = {0};
	DWORD lpcbNeeded;
	DWORD i, AvailableProcessCount;

	EnumProcesses(lpidProcess, 0x1000*4, &lpcbNeeded);

	AvailableProcessCount = 0;

	for(i=0; i<lpcbNeeded; i++){
		if(lpidProcess[i]){
			AvailableProcessCount++;
		}
	}

	return AvailableProcessCount;
}
DWORD ProcessCount(){
	DWORD lpidProcess[0x1000] = {0};
	DWORD lpcbNeeded;

	EnumProcesses(lpidProcess, 0x1000*4, &lpcbNeeded);

	return lpcbNeeded;
}

ListView LV;

LPCSTR GetProcessFileNames(){
	DWORD lpidProcess[0x1000] = {0};
	DWORD lpcbNeeded;
	DWORD i;
	HANDLE hProcess;
	char lpFileName[0x100];
	LPSTR lpString[2] = {(LPSTR)malloc(0x100), (LPSTR)malloc(0x100)};

	LV.Clear();

	EnumProcesses(lpidProcess, 0x1000*4, &lpcbNeeded);

	for(i=0; i<lpcbNeeded; i++){
		if(lpidProcess[i]){
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, lpidProcess[i]);
			if(hProcess){
				if(GetModuleFileNameExA(hProcess, NULL, lpFileName, 0x100)){
					Air::CreateFormatString(lpString[0], "%d", lpidProcess[i]);
					lpString[1] = lpFileName;
					LV._AddItem(lpString);
					CloseHandle(hProcess);
				}
			}
		}
	}

	return 0;
}


//
void AutoRefresh(){
	DWORD past1 = 0;
	DWORD past2 = 0;

	while(past1 = GetAvailableProcessCount()){
		if(past2 != past1){
			GetProcessFileNames();
			past2 = past1;
		}
		Sleep(500);
	}
}

ListView LV2;

void CreateControls(HWND hWndParent){
	//Air::CreateEdit(hWndParent, __EDIT_LOG, NULL, 3, 3, 300, false, 20);
	//Air::AddFormatString(hWndParent, __EDIT_LOG, "Test and %s", "Test1121");
	//Air::AddFormatString(hWndParent, __EDIT_LOG, "GetAvailableProcessCount = (%i)", GetAvailableProcessCount());
	//GetProcessFileNames(hWndParent);
	//Air::CreateButton(hWndParent, __BUTTON_REFRESH, "Refresh", 3, 450-3-20*2);
	LV.CreateSimpleListView(hWndParent, 3, 3, 600-6, 200);
	LV.CreateHeader("ProcessID", 100);
	LV.CreateHeader("File", 490);
	GetProcessFileNames();
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AutoRefresh, NULL, NULL, NULL);
	LV2.CreateSimpleListView(hWndParent, 3, 3+200+3, 600-6, 200);
	LV2.CreateHeader("DLL", 590);

	WIN32_FIND_DATA lpbla;
	HANDLE hFile = FindFirstFileA("DLL", &lpbla);
	LPSTR lpString[2] = {LPSTR(malloc(0x100))};
	Air::CreateFormatString(lpString[0], "%s", lpbla.cFileName);
	LV2._AddItem(lpString);
}


void Commands(HWND hwnd, WORD wID){
	switch(wID){
		case 0x300:
			GetProcessFileNames();
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

void TestRun(HINSTANCE hInstance){
	Air::SetMainInstance(hInstance);
	Air::CreateSimpleWindow(hInstance, "Air Injector (beta)", 600, 450, AirProc);
	Air::WaitForQuit();
}