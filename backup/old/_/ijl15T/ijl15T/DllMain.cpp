/*
	by AIRRIDE 2014/06/28
*/

#include<Windows.h>
#include"ijl15.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	
	if(!InitProxyDll()){
		return FALSE;
	}
	

	return TRUE;
}