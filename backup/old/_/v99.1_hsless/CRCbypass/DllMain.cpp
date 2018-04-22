#include<Windows.h>
#include"MSCRCbypass.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if (fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}
		CRC::CRCbypass();

	return TRUE;
}