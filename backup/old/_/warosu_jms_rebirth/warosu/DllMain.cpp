#include<Windows.h>
#include"warosuGUI.h"
#include"PacketHook.h"


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){return FALSE;}

	PH::PacketHook();

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PE::PacketDialog, hinstDLL, NULL, NULL);

	return TRUE;
}