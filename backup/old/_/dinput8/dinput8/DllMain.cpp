#include<Windows.h>
#include"dinput8.h"
#include"MSCRC.h"
#include"Additional.h"
#include"HWID.h"

DWORD MSFileScan  = 0x010F4DE2;
HANDLE hMainThread = 0;
DWORD CallCounter = 0;

bool WaitForUnpacking(){
	DWORD old;
	DWORD i = 0;
	BYTE Backup10[10] = {0};

	VirtualProtect((DWORD *)MSFileScan, 0x100, PAGE_EXECUTE_READWRITE, &old);

	//wait for unpacking function
	for(i=0; i<200; i++){
		if(*(BYTE *)(MSFileScan+0x16) == 0xC3){
			break;
		}
		Sleep(50);
	}


	if(i == 200){
		MessageBoxA(NULL, "Unpacking is too slowly\nPlease, restart MapleStory or update bypass", "dinput8 bypass v103.1", NULL);
		return false;
	}
	
	//backup
	RtlCopyMemory(Backup10, (void *)MSFileScan, 10);

	//write unpacking checker
	*(WORD *)(MSFileScan) = 0x05FF;
	*(DWORD *)(MSFileScan+2) = (DWORD)&CallCounter;
	*(DWORD *)(MSFileScan+2+4) = 0xC348C031;

	//wait for executing function (unpacking check)
	while(!CallCounter){
		Sleep(50);
	}

	SuspendThread(hMainThread);
	//restore memory
	memcpy((void *)MSFileScan, Backup10, 10);

	MSCRCBypass();
	DisableAntiHack();
	HWIDBypass();
	HSless();

	//rewrite
	*(DWORD *)(MSFileScan) = 0xC348C031;
	ResumeThread(hMainThread);
	
	return true;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	if(!InitProxyDll()){
		return FALSE;
	}

	hMainThread = OpenThread(THREAD_ALL_ACCESS, NULL, GetCurrentThreadId());

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WaitForUnpacking, NULL, NULL, NULL);

	return TRUE;
}