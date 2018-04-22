#include"AirPL.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AirPL_Run, hinstDLL ,NULL, NULL);

	return TRUE;
}