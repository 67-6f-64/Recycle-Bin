#include"ijl15.h"

void BypassLoader(){
	HMODULE hDLL = LoadLibraryA("Bypass.dll");
	if(!hDLL){
		MessageBoxA(NULL, "Bypass.dll�̓ǂݍ��݂Ɏ��s���܂���", "BypassLoader", NULL);
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		CreateExports();
		BypassLoader();
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}
