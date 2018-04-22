#include<Windows.h>
#include"ijl15.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateExports();
	MessageBoxA(NULL, "’âŽ~’†", "ijl15_test", NULL);

	return TRUE;
}