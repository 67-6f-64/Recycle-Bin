#include<Windows.h>
#include<stdio.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

DWORD dwHSST = 0;
DWORD dwHSEN = 0;

void WaitForHS(){
	HMODULE EHSvc = NULL;
	MODULEINFO EHSvcInfo, MSInfo;

	//Wait for HS
	while(!EHSvc){
		EHSvc = GetModuleHandleA("EHSvc.dll");
		Sleep(50);
	}
	
	GetModuleInformation(GetCurrentProcess(), EHSvc, &EHSvcInfo, sizeof(MODULEINFO));
	GetModuleInformation(GetCurrentProcess(), NULL, &MSInfo, sizeof(MODULEINFO));

	dwHSST = (DWORD)EHSvcInfo.lpBaseOfDll;
	dwHSEN = dwHSST+(DWORD)EHSvcInfo.SizeOfImage;
}


void _declspec(naked) SystemCall64(){
	_asm{
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
	}
}


void _stdcall OutputData(DWORD dwEAX, LPDWORD dwStack){
	if(dwHSST==0) return;
	//if(dwHSST > dwStack[1]) return;
	//if(dwHSEN < dwStack[1]) return;
	if(0x10000000 > dwStack[0]) return;
	if(0x70000000 < dwStack[0]) return;
	if(dwEAX == 0x01) return;
	if(dwEAX == 0x20) return;
	if(dwEAX == 0x2E) return;

	FILE *fp;
	fp = fopen("Output_dderewg.txt", "a");
	fprintf(fp, "EAX:%p | ESP1:%p | ESP2:%p\n", dwEAX, dwStack[0], dwStack[1]);
	fclose(fp);

	return;
}

void _declspec(naked) HyperHook(){
	_asm{
		push ebx
		lea ebx,[esp+4]
		pushad
		push ebx
		push eax
		call OutputData
Ending:
		popad
		pop ebx
		jmp SystemCall64
	}
}

void hello(){
	DWORD old;
	DWORD Sys64;

	_asm{
		mov eax,fs:[0x18]
		mov eax,[eax+0xC0]
		mov dword ptr [Sys64],eax
	}

	VirtualProtect((DWORD *)Sys64, 7, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((DWORD *)SystemCall64, 7, PAGE_EXECUTE_READWRITE, &old);
	memcpy((DWORD *)SystemCall64, (DWORD *)Sys64, 7);
	
	*(BYTE *)(Sys64) = 0xE9;
	*(DWORD *)(Sys64+1) = (DWORD)HyperHook-Sys64-5;

	WaitForHS();

	return;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH) return FALSE;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hello, NULL, NULL, NULL);

	return TRUE;
}