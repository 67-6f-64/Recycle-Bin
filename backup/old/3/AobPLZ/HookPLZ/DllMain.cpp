#include<Windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}
	MessageBoxA(NULL, "DLL Injection Test", "HookPLZ", NULL);
	return TRUE;
}
