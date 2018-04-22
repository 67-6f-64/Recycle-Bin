#include"AirBypass2.h"

DWORD MainThread;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	MainThread = GetCurrentThreadId();
	LoadLibrary("HShield/EHSvc.dll");
	CreateThread(NULL, 1024, (LPTHREAD_START_ROUTINE)AirBypass2_Run, hinstDLL ,NULL, NULL);

	return TRUE;
}