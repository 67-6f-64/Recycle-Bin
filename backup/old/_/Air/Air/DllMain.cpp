#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")
#include<stdio.h>
#include"MemoryHacks.h"

DWORD _declspec(naked) GetSystemCallAddr(){
	_asm{
		mov eax,fs:[0x18]
		mov eax,[eax+0xC0]
		ret
	}
}

void _declspec(naked) SystemCallReturn(){
	_asm{
		nop
		nop
		nop
		nop
		nop
		nop
		nop
	}
}

void _declspec(naked) IsCopiedDLL(){
	_asm{
		xor edi,edi
		cmp dword ptr [esp+0x04],0x12345678
		ja  ReturnCheck2
		inc edi
		jmp Ending_IO
 ReturnCheck2:
		cmp dword ptr [esp+0x04],0x87654321
		jb  Ending_IO
		inc edi
Ending_IO:
		ret 0x0004
	}
}

DWORD dwAlreadyLogged[0x100]={0};
DWORD dwCount = 0;

void _stdcall Logger(DWORD dwEAX, DWORD dwReturn){
	int i;

	for(i=0; i<dwCount; i++){
		if(dwAlreadyLogged[i] == dwEAX){
			return;
		}
	}

	dwAlreadyLogged[dwCount] = dwEAX;
	dwCount++;

	FILE *fp;

	fp = fopen("NTDLL_LOG.txt", "a");

	fprintf(fp, "EAX = %p(Ret = %p)\n", dwEAX, dwReturn);

	fclose(fp);
	

}

///
#define ntdll_ZwQueryInformationProcess 0x16
#define ntdll_NtProtectVirtualMemory 0x4D
#define ntdll_NtGetContextThread 0xCA
#define ntdll_NtMapViewOfSection 0x25
#define ntdll_NtProtectVirtualMemory 0x4D
//
#define ntdll_ZwQuerySystemInformation 0x33
#define ntdll_NtDeviceIoControlFile 0x04

DWORD EHSvc_Start = 0;
DWORD EHSvc_End = 0;

bool IsEHsvc(DWORD dwAddr){
	if(!EHSvc_End){
		return false;
	}

	if(EHSvc_Start <= dwAddr && dwAddr <= EHSvc_End){
		return true;
	}
	return false;
}

DWORD _stdcall SystemCallInspector(DWORD dwEAX, DWORD *Argument){

	switch(dwEAX){
		case ntdll_ZwQuerySystemInformation://Process Detection1, comes from EnumProcesses
			if(Argument[0] == 0x05 && Argument[3] == 0x00 && IsEHsvc(Argument[20])){
				return 0;
			}
			break;

		case ntdll_ZwQueryInformationProcess://EnumProcessModules
			if(Argument[4] == 0x00 && Argument[3] == 0x18 && IsEHsvc(Argument[64])){
				return 0;
			}
			break;

		case ntdll_NtDeviceIoControlFile://Driver Loader, comes from DeviceIoControl
			if(Argument[5] == 0x80000040 && IsEHsvc(Argument[34])){
				return 0;
			}
			break;

		//case ntdll_NtGetContextThread://Hardware BreakPoint Detection
		//	if(IsEHsvc(Argument[-2])){
		//		return 0;
		//	}
		//	break;

		case ntdll_NtProtectVirtualMemory:
			break;
		
		//case ntdll_NtMapViewOfSection:
		//	if(IsEHsvc(Argument[13])){
		//		return 0;
		//	}
		//	break;
			

		default:
			break;
	}

	return 0x1337;
}

#define ntdll_ZwOpenThread 0xFE
#define ntdll_NtOpenSection 0x32
#define ntdll_NtCreateMutant 0x9A
#define ntdll_NtQueryVirtualMemory 0x20
DWORD EHSvc_39F6A = NULL;

void _declspec(naked) HSCRC(){
	_asm{
		pushad
		mov esi,[ebp+0x08]//dwAddr
		sub esi,0x00400000
		add esi,Memory
		mov ecx,0x400//code size
		lea edi,[ebp-0x1228]
		repe movsd//memcpy
		popad
		jmp dword ptr [EHSvc_39F6A]
	}
}

DWORD _stdcall SystemCallInspector2(DWORD dwEAX, DWORD *Argument){

	switch(dwEAX){
		case ntdll_NtGetContextThread:
			return 0;

		case ntdll_NtQueryVirtualMemory:
			if(EHSvc_39F6A && Argument[17] == EHSvc_39F6A){
				Argument[17] = (DWORD)HSCRC;
			}
			break;

		default:
			break;
	}

	return 0x1337;
}

///
DWORD temp;
void _declspec(naked) SystemCallHook(){
	_asm{
		push edi
		cmp eax,0x1A3
		ja Ending
//Normal Section
		pushad
		push edx
		push eax
		call SystemCallInspector
		test eax,eax
		je Block0
		popad
//Copied Section
		push [esp+0x04]
		call IsCopiedDLL
		test edi,edi
		je Ending

		//lea edi,[esp+4]
		//pushad
		//push [edi]
		//push eax
		//call Logger
		//popad

		pushad
		push edx
		push eax
		call SystemCallInspector2
		test eax,eax
		je Block0
		popad
		
Ending:
		pop edi
		jmp SystemCallReturn
Block0:
		popad
		pop edi
		add dword ptr [esp],0x03
		xor eax,eax
		ret
Block1:
		popad
		pop edi
		add dword ptr [esp],0x03
		mov eax,1
		ret
Block334:
		popad
		pop edi
		add dword ptr [esp+0x04],0x21
		add dword ptr [esp],0x03
		xor eax,eax
		ret
	}
}


void WaitForHS(){
	HMODULE EHSvc = NULL;
	MODULEINFO EHSvcInfo;//, MSInfo;

	//Wait for HS
	while(!EHSvc){
		EHSvc = GetModuleHandleA("EHSvc.dll");
		Sleep(50);
	}
	
	GetModuleInformation(GetCurrentProcess(), EHSvc, &EHSvcInfo, sizeof(MODULEINFO));
	
	EHSvc_Start = (DWORD)EHSvcInfo.lpBaseOfDll;
	EHSvc_39F6A = EHSvc_Start + 0x39F6A;
	EHSvc_End = EHSvc_Start + (DWORD)EHSvcInfo.SizeOfImage;
}


void StartHook(){
	DWORD dwSystemCall = NULL;
	DWORD old = NULL;

	HMODULE hNTDLL = GetModuleHandleA("ntdll.dll");
	MODULEINFO NTDLLInfo;

	MSCRCBypass();
	RemoveThemidaCRCs();
	VEHBypass();
	StopProcessListPacket();

	GetModuleInformation((HANDLE)0xFFFFFFFF, hNTDLL, &NTDLLInfo, sizeof(MODULEINFO));
	
	VirtualProtect((DWORD *)((DWORD)IsCopiedDLL+2+4), 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((DWORD *)((DWORD)IsCopiedDLL+2+4+4+2+1+2+4), 4, PAGE_EXECUTE_READWRITE, &old);
	
	*(DWORD *)((DWORD)IsCopiedDLL+2+4) = (DWORD)NTDLLInfo.lpBaseOfDll;
	*(DWORD *)((DWORD)IsCopiedDLL+2+4+4+2+1+2+4) = (DWORD)NTDLLInfo.lpBaseOfDll+(DWORD)NTDLLInfo.SizeOfImage;

	dwSystemCall = GetSystemCallAddr();

	VirtualProtect((DWORD *)dwSystemCall, 12, PAGE_EXECUTE_READWRITE, &old); 
	VirtualProtect((DWORD *)SystemCallReturn, 7, PAGE_EXECUTE_READWRITE, &old);
	memcpy((DWORD *)SystemCallReturn, (DWORD *)dwSystemCall, 7);
	
	*(BYTE *)(dwSystemCall+7) = 0xE9;
	*(DWORD *)(dwSystemCall+8) = (DWORD)SystemCallHook-(dwSystemCall+7)-5;
	*(WORD *)(dwSystemCall) = 0x05EB;//short jmp

	WaitForHS();

	MessageBoxA(NULL, "Success(?)", "Air Copied of ntdll.dll Detector", NULL);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)StartHook, hinstDLL, NULL, NULL);

	return TRUE;
}