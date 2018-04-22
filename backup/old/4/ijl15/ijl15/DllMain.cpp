#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")
#include"ijl15.h"

#define Windows7 7
#define Windows8 8
#define WindowsUnknown 0

int GetOSVersion(){
	OSVERSIONINFOA lpVersionInfo;
	lpVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA); 

	GetVersionExA(&lpVersionInfo);

	if(lpVersionInfo.dwMajorVersion!=6) return WindowsUnknown;

	switch(lpVersionInfo.dwMinorVersion){
		case 0:
			return WindowsUnknown;
		case 1:
			return Windows7;
		case 2:
		case 3:
			return Windows8;
	}

	return WindowsUnknown;
}

BYTE _CreateWindowExA[17] = {0x60, 0xE8, 0xFF, 0xFF, 0xFF, 0xFF, 0x61, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE9, 0xFF, 0xFF, 0xFF, 0xFF};
DWORD dwCreateWindowExA;

void RestoreHooks(){
	RtlCopyMemory((void *)dwCreateWindowExA, &_CreateWindowExA[7], 5);
}

void _stdcall ChainFunction(){
	RestoreHooks();
	if(!LoadLibraryA("AirBypass2.dll")){
		MessageBoxA(NULL, "where is AirBypass2.dll :wut:", "ijl15", NULL);
	}
}

void FullAccess(DWORD dwAddr, DWORD dwSize){
	DWORD old;
	VirtualProtect((DWORD *)dwAddr, dwSize, PAGE_EXECUTE_READWRITE, &old);
}

void Hook(){
	dwCreateWindowExA = (DWORD)GetProcAddress(GetModuleHandleA("User32.dll"), "CreateWindowExA");
	FullAccess(dwCreateWindowExA, 5);
	FullAccess((DWORD)_CreateWindowExA, 17);
	RtlCopyMemory(&_CreateWindowExA[7], (void *)dwCreateWindowExA, 5);
	*(DWORD *)(&_CreateWindowExA[1+1]) = (DWORD)ChainFunction - (DWORD)&_CreateWindowExA[1] - 5;
	*(DWORD *)(&_CreateWindowExA[12+1]) = (DWORD)(dwCreateWindowExA + 5) - (DWORD)&_CreateWindowExA[12] - 5;
	*(BYTE *)(dwCreateWindowExA) = 0xE9;
	*(DWORD *)(dwCreateWindowExA + 1) = (DWORD)&_CreateWindowExA[0] - dwCreateWindowExA - 5;
}

void ResetTimer(){
	HMODULE winmm = GetModuleHandleA("winmm.dll");

	if(winmm){
		*(DWORD *)((DWORD)winmm + 0x28FE0) = 0;//Windows 7
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		CreateExports();
		if(GetOSVersion() == Windows7){
			ResetTimer();
		}
		Hook();
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}