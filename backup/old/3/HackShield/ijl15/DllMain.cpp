#include"ijl15.h"
#include"ExportHook.h"

HMODULE (WINAPI *_LoadLibraryA)(LPCSTR lpLibFileName);
bool bOnce = false;
HMODULE WINAPI LoadLibraryAHook(LPCSTR lpLibFileName){
	if(strstr(lpLibFileName, "HShield\\EHSvc.dll")){
		if(!bOnce){
			bOnce = true;
			return _LoadLibraryA("EHSvc.dll");
		}
	}
	return _LoadLibraryA(lpLibFileName);
}

void Hook(){
	ExportHook kernel32;
	kernel32.Init("kernel32.dll");
	kernel32.Hook("LoadLibraryA", LoadLibraryAHook, &_LoadLibraryA);
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateExports();
	Hook();
	return TRUE;
}
