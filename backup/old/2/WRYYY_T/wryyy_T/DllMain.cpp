#include<Windows.h>
#include"gui.h"
#include"apihook.h"
#include<intrin.h>
#pragma intrinsic(_ReturnAddress)

gui wt;
list wtl;
void wt_create(HWND hWnd){
	wtl.Set(hWnd, 3, 3, 394, 294);
	wtl.AddHeader("Thread ID", 70);
	wtl.AddHeader("Return", 150);
	wtl.AddHeader("Routine", 150);
}

void gui_wt(HINSTANCE hInstance){
	wt.Run(hInstance, "WRYYY_T", 400, 300, wt_create/*, wt_control, wt_notify*/);
}

void _stdcall AddLog(DWORD ThreadId, DWORD Return, DWORD Address){
	char s[256], file[256];
	int i, last;
	HMODULE hModule;
	sprintf(s, "%08X", ThreadId);
	wtl.AddItem(0, s);
	GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPSTR)Return, &hModule);
	if(hModule){
		GetModuleFileNameA(hModule, file, 256);
		last = 0;
		for(i=0; file[i]; i++){
			if(file[i] == '\\'){
				last = i + 1;
			}
		}
		sprintf(s, "%s+%08X", &file[last], Return - (DWORD)hModule);
	}
	else{
		sprintf(s, "%08X", Return);
	}
	wtl.AddItem(1, s);
	GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPSTR)Address, &hModule);
	if(hModule){
		GetModuleFileNameA(hModule, file, 256);
		last = 0;
		for(i=0; file[i]; i++){
			if(file[i] == '\\'){
				last = i + 1;
			}
		}
		sprintf(s, "%s+%08X", &file[last], Address - (DWORD)hModule);
	}
	else{
		sprintf(s, "%08X", Address);
	}
	wtl.AddItem(2, s);
}

NTSTATUS (WINAPI *_NtCreateThreadEx)(HANDLE *hThread, LPVOID lpv2, LPVOID lpv3, LPVOID lpv4, DWORD Address, LPVOID lpv6, LPVOID lpv7, LPVOID lpv8, LPVOID lpv9, LPVOID lpv10, LPVOID lpv11);
NTSTATUS WINAPI NtCreateThreadEx_Hook(HANDLE *hThread, LPVOID lpv2, LPVOID lpv3, LPVOID lpv4, DWORD Address, LPVOID lpv6, LPVOID lpv7, LPVOID lpv8, LPVOID lpv9, LPVOID lpv10, LPVOID lpv11){
	NTSTATUS retVal = _NtCreateThreadEx(hThread, lpv2, lpv3, lpv4, Address, lpv6, lpv7, lpv8, lpv9 , lpv10, lpv11);
	AddLog(GetThreadId(*hThread), (DWORD)_ReturnAddress(), Address);
	return retVal;
}
void hook(){
	apihook ntdll("ntdll.dll");
	ntdll.Hook("NtCreateThreadEx", NtCreateThreadEx_Hook, &_NtCreateThreadEx);
}

void start(HINSTANCE hinstDLL){
	hook();
	gui_wt(hinstDLL);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)start, hinstDLL, NULL ,NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}
