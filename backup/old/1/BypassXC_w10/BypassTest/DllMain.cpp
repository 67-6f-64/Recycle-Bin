#include<Windows.h>
#include"MapleStory.h"
#include"XignCode.h"

void init(DWORD ThreadId){
	//HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, ThreadId);
	//SuspendThread(hThread);
	apihook_ms();
	//ResumeThread(hThread);
	//CloseHandle(hThread);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)init, (LPVOID)GetCurrentThreadId(), NULL ,NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}
