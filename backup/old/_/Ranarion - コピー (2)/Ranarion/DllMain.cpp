#include<Windows.h>
#include"RanarionGUI.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH) return FALSE;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)StartDialog, hinstDLL, NULL, NULL);

	return TRUE;
}