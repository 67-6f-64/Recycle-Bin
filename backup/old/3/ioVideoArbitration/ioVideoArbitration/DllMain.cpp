#include<Windows.h>
#include"AirMemory.h"
#pragma comment(lib, "AirMemory.lib")


AirMemory d3d9;

void Chain(DWORD ThreadId){
	HANDLE h = OpenThread(THREAD_ALL_ACCESS, NULL, ThreadId);
	d3d9.Init("d3d9.dll");
	SuspendThread(h);
	MessageBoxA(NULL, "d3d9", "stop", NULL);
	ResumeThread(h);
}

DWORD _ioVA_DwmEnableComposition;

void _declspec(naked) ioVA_DwmEnableComposition(){
	_asm{
		jmp dword ptr [_ioVA_DwmEnableComposition]
	}
}

DWORD _ioVA_IsEnableOverlay;
void _declspec(naked) ioVA_IsEnableOverlay(){
	_asm{
		jmp dword ptr [_ioVA_IsEnableOverlay]
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	HMODULE hModule;
	DWORD old;

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	hModule = LoadLibraryA("_ioVideoArbitration.dll");
	_ioVA_DwmEnableComposition = (DWORD)GetProcAddress(hModule, "ioVA_DwmEnableComposition");
	_ioVA_IsEnableOverlay = (DWORD)GetProcAddress(hModule, "ioVA_IsEnableOverlay");

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Chain, (void *)GetCurrentThreadId(), NULL, NULL);
	MessageBoxA(NULL, "ioVideoArbitration.dll", "StopPLZ", NULL);
	return TRUE;
}