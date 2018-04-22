#include<Windows.h>

void SysCallHook(DWORD dwReturn){
	DWORD newmem = (DWORD)new BYTE[0x100];
	DWORD x64syscall = *(DWORD *)0x7EFDD0C0;//win7 x64 only
	DWORD old;
	
	VirtualProtect((DWORD *)newmem, 0x100, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((DWORD *)x64syscall, 7, PAGE_EXECUTE_READWRITE, &old);
	
	*(BYTE *)(newmem) = 0x81;
	*(BYTE *)(newmem+1) = 0x3C;
	*(BYTE *)(newmem+1+1) = 0x24;
	*(DWORD *)(newmem+1+1+1) = dwReturn;
	*(WORD *)(newmem+1+1+1+4) = 0x850F;
	*(DWORD *)(newmem+1+1+1+4+2) = 0x07;
	*(DWORD *)(newmem+1+1+1+4+2+4) = 0x03240483;
	*(WORD *)(newmem+1+1+1+4+2+4+4) = 0xC031;
	*(BYTE *)(newmem+1+1+1+4+2+4+4+2) = 0xC3;
	memcpy((DWORD *)(newmem+1+1+1+4+2+4+4+2+1), (DWORD *)x64syscall, 7); 
	
	*(BYTE *)(x64syscall) = 0xE9;
	*(DWORD *)(x64syscall+1) = newmem-x64syscall-5;
}

void WaitForHS(){
	HMODULE EHSvc = NULL;
	DWORD NTDLL = NULL;
	HMODULE orignalNTDLL;
	DWORD dwNtGCT;

	//wait for hs
	while(!EHSvc){
		EHSvc = GetModuleHandleA("EHSvc.dll");
		Sleep(50);
	}

	//wait for ntdll tmp
	while(!NTDLL){
		NTDLL = *(DWORD *)((DWORD)EHSvc+0x13C13C);
		Sleep(50);
	}

	dwNtGCT = (DWORD)GetProcAddress((HMODULE)NTDLL, "NtGetContextThread") +0x12;


	SysCallHook(dwNtGCT);
}

void hello(){
	WaitForHS();
	return;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH) return FALSE;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hello, NULL, NULL, NULL);

	return TRUE;
}