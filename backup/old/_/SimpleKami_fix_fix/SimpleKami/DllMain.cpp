#include<Windows.h>
#include"SimpleGUI.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GUI::DialogInit, hinstDLL, NULL, NULL);
	//DLL Injection is successfully
	return TRUE;
}