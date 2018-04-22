#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")
#include"Asm2.h"

#define Windows7 7
#define Windows8 8

void WaitForHS(int version){
	HMODULE EHSvc = NULL;
	MODULEINFO EHSvcInfo;

	//Wait for HS
	while(!EHSvc){
		EHSvc = GetModuleHandleA("EHSvc.dll");
		Sleep(50);
	}

	GetModuleInformation(GetCurrentProcess(), EHSvc, &EHSvcInfo, sizeof(MODULEINFO));
	//2
	switch(version){
		case Windows7:
			break;
		case Windows8:
			break;
		default:
			break;
	}
}


int CheckOS(){
	OSVERSIONINFOA lpVersionInfo;
	lpVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA); 

	GetVersionExA(&lpVersionInfo);

	if(lpVersionInfo.dwMajorVersion!=6) return 0;

	switch(lpVersionInfo.dwMajorVersion){
		case 1:
			return Windows7;
		case 2:
		case 3:
			return Windows8;
		default:
			return 0;
	}

	return 0;
}



void hello(){
	int version = CheckOS();
	BOOL checkbit;
	if(version > 0){
		IsWow64Process(GetCurrentProcess(), &checkbit);
			if(checkbit){
				MessageBoxA(NULL, "Press OK = Install HSBypass!\n\nCreated by AIRRIDE", "Public HackShield Bypass", MB_OK);
				WaitForHS(version);
				return;
			}
	}
	MessageBoxA(NULL, "Your OS is not supported Sorry!\nPlease Buy Windows7 or 8 x64\n\nCreated by AIRRIDE", "Public HackShield Bypass", MB_OK);
	return;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH) return FALSE;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hello, NULL, NULL, NULL);

	return TRUE;
}