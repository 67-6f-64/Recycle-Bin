#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")
#include"Asm.h"
#include"CRCbypass.h"


namespace ijl15{
	
	DWORD dwInit;
	DWORD dwFree;
	DWORD dwRead;
	DWORD dwWrite;
	DWORD dwGetLibVersion;
	DWORD dwErrorStr;
	
	BOOL Initialization();
}

BYTE ijl15_Init[0x05];
BYTE ijl15_Free[0x05];
BYTE ijl15_Read[0x05];
BYTE ijl15_Write[0x05];
BYTE ijl15_GetLibVersion[0x05];
BYTE ijl15_ErrorStr[0x05];

BOOL ijl15::Initialization(){
	HMODULE ijl15;

	ijl15 = LoadLibraryA("DLL/ijl15.dll");
	
	if(ijl15 == 0){
		MessageBoxA(NULL, "Please put ijl15.dll into DLL folder", "ijl15.dll", MB_OK);
		return FALSE;
	}
	
	
	dwInit = (DWORD)GetProcAddress(ijl15, "ijlInit");
	dwFree = (DWORD)GetProcAddress(ijl15, "ijlFree");
	dwRead = (DWORD)GetProcAddress(ijl15, "ijlRead");
	dwWrite = (DWORD)GetProcAddress(ijl15, "ijlWrite");
	dwGetLibVersion = (DWORD)GetProcAddress(ijl15, "ijlGetLibVersion");
	dwErrorStr = (DWORD)GetProcAddress(ijl15, "ijlErrorStr");
	
	
	Asm::Write_Hook("jmp", (DWORD)ijl15_Init, dwInit);
	Asm::Write_Hook("jmp", (DWORD)ijl15_Free, dwFree);
	Asm::Write_Hook("jmp", (DWORD)ijl15_Read, dwRead);
	Asm::Write_Hook("jmp", (DWORD)ijl15_Write, dwWrite);
	Asm::Write_Hook("jmp", (DWORD)ijl15_GetLibVersion, dwGetLibVersion);
	Asm::Write_Hook("jmp", (DWORD)ijl15_ErrorStr, dwErrorStr);

	if(dwInit == NULL || dwFree == NULL || dwRead == NULL || dwWrite == NULL || dwGetLibVersion == NULL || dwErrorStr == NULL){
		MessageBoxA(NULL, "ijl15.dll is broken", "ijl15.dll", MB_OK);
		return FALSE;
	}

	return TRUE;
}


BOOL _stdcall ijl_SendHook(DWORD *param){
	int i;

	switch(param[1]){
		case 0x01BF35FC:
			for(i=0; i<4; i++) *(BYTE *)(*(DWORD *)(param[2]+4)+2+6+i) = rand()%0x100;//?
			break;
		
		case 0x0057DCE1:
			for(i=0; i<4; i++) *(BYTE *)(*(DWORD *)(param[2]+4)+16+i) = rand()%0x100;
			break;

		case 0x018B8FD9:
		case 0x0194AB6E:
			return TRUE;//block
		
		default:
			break;
	}

	return FALSE;
}

DWORD dwTitleHook_ret;
char Title[0x100] = "HSlessStory v101.1";

void _declspec(naked) TitleHook(){
	_asm{
		push eax
		mov ecx,[ebp-0x54]
		neg ecx
		lea eax,[Title]
		mov dword ptr [esp+0x04],eax
		jmp dwTitleHook_ret
	}
}

DWORD HSCheck(DWORD retAddr){
	HMODULE EHSvc = GetModuleHandleA("EHSvc.dll");
	MODULEINFO EHSvcInfo;
	//EHSvc is not loaded
	if(!EHSvc) return 0;

	if(GetModuleInformation(GetCurrentProcess(), EHSvc, &EHSvcInfo, sizeof(MODULEINFO))){
		//EHSvc retAddr CHECK
		if(retAddr >= (DWORD)EHSvcInfo.lpBaseOfDll && retAddr <= (DWORD)EHSvcInfo.lpBaseOfDll + (DWORD)EHSvcInfo.SizeOfImage) return (DWORD)EHSvcInfo.lpBaseOfDll;
	}

	return 0;
}


BOOL _stdcall HookTestv(DWORD *param){
	DWORD dwEHSvc;
	dwEHSvc = HSCheck(param[0]);
	if(!dwEHSvc) return FALSE;
	SetLastError(ERROR_SUCCESS);
	return TRUE;
}


BOOL _stdcall HookTestv2(DWORD *param){
	DWORD dwEHSvc;
	dwEHSvc = HSCheck(param[1]);//callback
	if(!dwEHSvc) return FALSE;
	//printf("CallBack:EHSvc.dll+%08X...\n", param[1]-dwEHSvc);
	SetLastError(ERROR_SUCCESS);
	return TRUE;
}

BOOL _stdcall HookTest1337v(DWORD *param){
	DWORD dwEHSvc;
	dwEHSvc = HSCheck(param[0]);
	if(!dwEHSvc) return FALSE;
	if(param[2]!=0x80000040) return FALSE;
	SetLastError(ERROR_SUCCESS);
	return TRUE;
}


void DeadShieldv2(){
	HMODULE EHSvc = NULL;
	MODULEINFO EHSvcInfo;
	OSVERSIONINFO OSver;

	printf("DeadShield v2...\n\n");
	
	OSver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionExA(&OSver);

	if(OSver.dwMajorVersion != 6){
		printf("OS:???\nSorry, this OS is not supported\n\n");
		return;
	}

	switch(OSver.dwMinorVersion){
		case 0:
			printf("OS:Windows Vista\n");
			printf("Sorry, this OS is not supported\n\n");
			printf("Win7 Mode......\n");
			//Asm::HookAPI2("WINMM.dll", "DriverCallback", (DWORD)HookTestv2, 7);
			Asm::HookAPI2("USER32.dll", "EnumWindows", (DWORD)HookTestv, 2);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumProcesses", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumProcessModules", (DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumDeviceDrivers", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNEL32.dll", "K32GetMappedFileNameA", (DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNEL32.dll", "DeviceIoControl", (DWORD)HookTest1337v, 8, 1);
			break;

		case 1://Windows7
			printf("OS:Windows 7\n");
			Asm::HookAPI2("WINMM.dll", "DriverCallback", (DWORD)HookTestv2, 7);
			Asm::HookAPI2("USER32.dll", "EnumWindows", (DWORD)HookTestv, 2);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumProcesses", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumProcessModules", (DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumDeviceDrivers", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNEL32.dll", "K32GetMappedFileNameA", (DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNEL32.dll", "DeviceIoControl", (DWORD)HookTest1337v, 8, 1);
			break;

		case 2:
			printf("OS:Windows 8\n");
			Asm::HookAPI2("USER32.dll", "EnumWindows", (DWORD)HookTestv, 2);
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumProcesses", (DWORD)HookTestv, 3);//
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumProcessModules",(DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumDeviceDrivers", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNELBASE.dll", "K32GetMappedFileNameA", (DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNEL32.dll", "DeviceIoControl", (DWORD)HookTest1337v, 8, 1);
			break;

		case 3://Windows8.1
			printf("OS:Windows 8.1\n");
			Asm::HookAPI2("USER32.dll", "EnumWindows", (DWORD)HookTestv, 2);
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumProcesses", (DWORD)HookTestv, 3);//
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumProcessModules", (DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumDeviceDrivers", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNELBASE.dll", "K32GetMappedFileNameA", (DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNEL32.dll", "DeviceIoControl", (DWORD)HookTest1337v, 8, 1);
			break;

		default:
			break;

	}
	printf("DeadShield v2 was loaded\n\n");
	return;
}


void DeadShieldv2_x86(){
	HMODULE EHSvc = NULL;
	MODULEINFO EHSvcInfo;
	OSVERSIONINFO OSver;

	printf("DeadShield v2 x86 mode...\n\n");
	
	OSver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionExA(&OSver);

	if(OSver.dwMajorVersion != 6){
		printf("OS:???\nSorry, this OS is not supported\n\n");
		return;
	}

	switch(OSver.dwMinorVersion){
		case 0:
			printf("OS:Windows Vista\n");
			printf("Sorry, this OS is not supported\n\n");
			printf("Win7 Mode......\n");
			Asm::HookAPI2("USER32.dll", "EnumWindows", (DWORD)HookTestv, 2);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumProcesses", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumProcessModules", (DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumDeviceDrivers", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNEL32.dll", "K32GetMappedFileNameA", (DWORD)HookTestv, 4);
			break;

		case 1://Windows7
			printf("OS:Windows 7\n");
			//Asm::HookAPI2("WINMM.dll", "DriverCallback", (DWORD)HookTestv2, 7);
			Asm::HookAPI2("USER32.dll", "EnumWindows", (DWORD)HookTestv, 2);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumProcesses", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumProcessModules", (DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNEL32.dll", "K32EnumDeviceDrivers", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNEL32.dll", "K32GetMappedFileNameA", (DWORD)HookTestv, 4);
			//Asm::HookAPI2("KERNEL32.dll", "DeviceIoControl", (DWORD)HookTest1337v, 8, 1);
			break;

		case 2:
			printf("OS:Windows 8\n");
			Asm::HookAPI2("USER32.dll", "EnumWindows", (DWORD)HookTestv, 2);
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumProcesses", (DWORD)HookTestv, 3);//
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumProcessModules",(DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumDeviceDrivers", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNELBASE.dll", "K32GetMappedFileNameA", (DWORD)HookTestv, 4);
			//Asm::HookAPI2("KERNEL32.dll", "DeviceIoControl", (DWORD)HookTest1337v, 8, 1);
			break;

		case 3://Windows8.1
			printf("OS:Windows 8.1\n");
			Asm::HookAPI2("USER32.dll", "EnumWindows", (DWORD)HookTestv, 2);
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumProcesses", (DWORD)HookTestv, 3);//
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumProcessModules", (DWORD)HookTestv, 4);
			Asm::HookAPI2("KERNELBASE.dll", "K32EnumDeviceDrivers", (DWORD)HookTestv, 3);
			Asm::HookAPI2("KERNELBASE.dll", "K32GetMappedFileNameA", (DWORD)HookTestv, 4);
			//Asm::HookAPI2("KERNEL32.dll", "DeviceIoControl", (DWORD)HookTest1337v, 8, 1);
			break;

		default:
			break;

	}
	printf("DeadShieldv2 x86 was loaded\n\n");
	return;
}


int InjectFromList(){
	char buf[0x100];
	char InjectList[0x100] = "DLL/";
	FILE *fp;
	int i,j;

	printf("Reading DLL/InjectList.txt...\n");

	fp = fopen("DLL/InjectList.txt", "r");

	if(fp == NULL){
		printf("Couldn't find DLL/InjectList.txt\n");
		return 0;
	}

	for(i=0; fgets(buf, 0x100, fp)!=NULL; i++){

		ZeroMemory(&InjectList[4], 0x100 - 4);

		for(j=0; buf[j]!='/'; j++){
			InjectList[j+4] = buf[j];
		}

		if(InjectList[5] == 0x00){
			continue;
		}

		if(LoadLibraryA(InjectList) == NULL){
			printf("Couldn't Inject %s\n", InjectList);
		}else{
			printf("%s was injected\n", InjectList);
		}

	}
	printf("DLL Injection was ended...\n\n");
	return i;
}

//#include<excpt.h>
//#pragma comment(lib, "coredll.lib")
void WRYYY(HANDLE hThread){
	int ew;
	Asm::Debug(TRUE);
	//printf("GetExceptionInformation:%p\n", (DWORD)GetExceptionInformation);
	//printf("GetExceptionCode:%p\n\n\n", (DWORD)GetExceptionCode);

	printf("ijl15 BYPASS for v101.1 EMS...\n\n");
	printf("Wait for memory uncompressing...\n");
	CRCbypass(hThread);//STOP MAINT THREAD^^
	printf("CRCBypass is OK\n\n");

	printf("HSLESS = 0\nHSCRC++ = 1\nHSCRC 32bit = 2\n>>");
	scanf("%d", &ew);
	if(ew == 0){
		printf("HSLESS MODE\n");
	}
	if(ew == 1){
		printf("HSCRC MODE\n");
	}
	if(ew == 2){
		printf("HSCRC 32bit MODE\n");
	}

	//Asm::StackHook(0x018AF0D3, 7, (DWORD)ijl_SendHook, "C9 C2 04 00",TRUE);
	//printf("SendHook = 01E17642\nBlocking ProcessList and Randomizing MacAddress\n(01BAEAE0, 01B104C1, 01C4E512, 00544B1D)\n\n");


	printf("Fidning Addresses...\n");
	
	DWORD dwHSless1= Asm::AobScan("81 EC 20 01 00 00 A1 ?? ?? ?? ?? 33 C4 89 84 24 1C 01 00 00 55 56 6A 00 8B F1 E9");
	DWORD dwHSless2 = Asm::AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 56 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 00 00 00 00 8B B4 24 ?? ?? ?? ?? 68 92 01 00 00");
	DWORD dwVEHDebug = Asm::AobScan("55 8B EC 81 EC F0 02 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 6A 00 E9");
	DWORD dwTitleHook = Asm::AobScan("50 8B 4D AC F7 D9 1B C9");
	DWORD dwUpdateSkip= Asm::AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 C4 18 F2 FF FF");
	DWORD dwMutexPrev = Asm::AobScan("75 F5 56 89 35 ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? 89 3D ?? ?? ?? ?? 89 3D ?? ?? ?? ?? E9")+0x1F;
	DWORD dwMutexNext = Asm::AobScan("FF 15 ?? ?? ?? ?? 3D B7 00 00 00 0F 85");
	DWORD dwLauncherSkip = Asm::AobScan("83 EC 5C 55 56 33 ED 55 FF 15");
	DWORD dwProcessList = Asm::AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 ?? ?? ?? ?? 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 ?? ?? ?? ?? 8B AC 24 ?? ?? ?? ?? 33 DB 53 8B F9 E9");
	printf("dwHSless1 = %p\nddwHSless2 = %p\ndwVEHDebug = %p\ndwTitleHook = %p\ndwUpdateSkip = %p\ndwMutexPrev = %p\ndwMutexNext = %p\ndwLauncherSkip = %p\ndwProcessList = %p\n\n",
		dwHSless1, dwHSless2, dwVEHDebug, dwTitleHook, dwUpdateSkip, dwMutexPrev, dwMutexNext, dwLauncherSkip, dwProcessList);


	if(ew == 0){
		Asm::Write_code(dwHSless1, "C2 04 00");
		Asm::Write_code(dwHSless2, "C2 04 00");
	}
	Asm::Write_code(dwProcessList, "31 C0 C2 04 00");
	Asm::Write_code(dwVEHDebug, "31 C0 C3");

	if(ew == 0){
		dwTitleHook_ret = Asm::Write_Hook("jmp", dwTitleHook, (DWORD)TitleHook, 1);
	}

	Asm::Write_Hook("jmp", dwMutexPrev, dwMutexNext);

	if(ew == 0){
		Asm::Write_code(dwUpdateSkip, "8B C3 C2 10 00");
	}

	Asm::Write_code(dwLauncherSkip, "31 C0 40 C3");

	if(ew == 1){
		LoadLibrary("DLL/Riremito.dll");
		//DeadShieldv2();
	}

	if(ew == 2){
		DeadShieldv2_x86();
	}

	ResumeThread(hThread);//RESTART MAIN THREAD
	//InjectFromList();
	printf("WRYYYYYYYYYYYYYYYYYY\n");
	Sleep(10000);
	Asm::Debug(FALSE);
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	if(fdwReason != DLL_PROCESS_ATTACH) return FALSE;
	if(ijl15::Initialization() == FALSE) return FALSE;
	srand(time(NULL));
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, NULL, GetCurrentThreadId());//FULLACCESS MAIN THREAD

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WRYYY, hThread, NULL, NULL);

	return TRUE;
}