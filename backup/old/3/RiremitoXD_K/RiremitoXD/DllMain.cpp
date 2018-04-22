#include"ijl15.h"
#include"AirBypass.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}
	CreateExports();
	HackShieldBypass();
	AirBypass();
	return TRUE;
}
